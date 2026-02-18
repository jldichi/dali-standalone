/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: msgtable.cc,v 1.1.1.1 1996/04/16 18:52:32 gustavof Exp $
*
* DESCRIPTION
* Message handling routines.
*********************************************************************/

#ifdef __GNUC__
#	pragma implementation
#endif

#include <ifound/msgtable.h>
#include <ifound/message.h>
#include <ifound/hashtab.h>
#include <ifound/resource.h>
#include <ifound/parray.h>
#include <ifound/stdc.h>
#include <ifound/priv/gfuncs.h>
#include <stdio.h>

// Amount of messages in memory.
#define	MSG_HASH		29
#define MAIN_SECTION	"main"
#define DATA_SUBDIR		"data"
#define ISLINK(x)		((x)[0] == '~')
#define LINKMSG(x)		((x)(1, (x).length()-1))
#define MSGEXT			".msg"

struct MsgFileNode {

	Int section;
	String msgName;

	MsgFileNode() { }
	MsgFileNode(Int s, const String &mN) 
		: section(s), msgName(mN) { }

	friend Int cmp(const MsgFileNode &n1, const MsgFileNode &n2);
	friend Int hashVal(const MsgFileNode &n, Int module);
};

// DALI-STANDALONE: Define cmp/hashVal outside class for ::cmp visibility in HashTab macro
Int cmp(const MsgFileNode &n1, const MsgFileNode &n2) {
	return n1.section != n2.section ? n1.section - n2.section :
			cmp(n1.msgName, n2.msgName);
}
Int hashVal(const MsgFileNode &n, Int module) {
	return ((n.section<<16) + hashVal(n.msgName,module)) % module;
}

declare2(HashTab,MsgFileNode,Int);
implement2(HashTab,MsgFileNode,Int);

declare(PtrArray, String);
declare(PtrArrayCursor, String);

class MsgFile : public Resource {

	friend class MsgTableCursor;
	
// private:

	HashTab(MsgFileNode, Int) &tab;
	PtrArray(String) &sectionMap;

	String 	module;
 	FILE 	*fp;

	static MsgFile *head;
	MsgFile *next;
	
	struct MsgCache {
		Int offset;
		String msgDesc;
		MsgCache() 			{ offset = 0; }
	} cache[MSG_HASH];

	MsgFile(const MsgFile &, bool infError);
	void operator=(const MsgFile &msg);

	void loadIndex();

public:

	MsgFile(const String &module);
	~MsgFile();

	Int sectionNumber(const String &s);
	Int addSection(const String &s);
	
	static MsgFile *find(const String &module);
	static void dispose(MsgFile *file);
	
	String search(Int section, const String &name, bool informError);
};

MsgFile *MsgFile::head = NULL;

MsgFile *MsgFile::find(const String &m)
{
	assert(m != NULL_STRING);

	MsgFile *aux;
	for (aux = head; aux; aux = aux->next)
		if (aux->module == m)
			break;

	if (aux == NULL)
		return tr_new MsgFile(m);
	else {
		aux->ref();			
		return aux;
	}		
}

void MsgFile::dispose(MsgFile *file)
{
	file->unRef();	
}

Int MsgFile::sectionNumber(const String &s)
{
	for (Int i = 0; i < sectionMap.dim(); ++i)
		if (*sectionMap.elem(i) == s)
			return i;
	return ERR;			
}

Int MsgFile::addSection(const String &s)
{
	return sectionMap.add(tr_new String(s));
}

MsgFile::MsgFile(const String &modName)
:	tab(*tr_new HashTab(MsgFileNode,Int)(MSG_HASH)),
	sectionMap(*tr_new PtrArray(String)(0)),
	module(modName)
{
	ref();

	next = head;
	head = this;

	String data = String("DATADIR").getEnv();
	String language = String("LANGUAGE").getEnv();
	if (!data.isNull())
		fp = openDataDirFile(data, language, module+MSGEXT);
	else {
		data = String("IDEAFIX").getEnv() + PATH_SLASH + DATA_SUBDIR;
		String mod = data+PATH_SLASH+language+PATH_SLASH+module+MSGEXT;
		fp = fopen(toCharPtr(mod), FOPEN_READ_MODE);
	}
	loadIndex();
}

MsgFile::~MsgFile()
{ 
	if (this == head) 
		head = next;
	else {
		MsgFile *aux;
		for (aux = head; aux->next && aux->next != this;
				aux = aux->next)
			;
		assert(aux->next == this);
		aux->next = next;
	}
	if (fp != NULL)
		fclose(fp);
	tr_delete(&tab);
	for (PtrArrayCursor(String) c(sectionMap); c; ++c)
		tr_delete(*c);
	tr_delete(&sectionMap);
}

void MsgFile::loadIndex()
{
	if (fp == NULL)
		return;

	// seek for the beginning of the messages' index
	Int offset = ldInt(fp);
	//fprintf(stderr,"offset: %ld\n", offset);
	// read section map
	
	String section = ldString(fp);
	while (!section.isNull()) {
		addSection(section);
		section = ldString(fp);
	}

	// Read the index
	Int secNum;
	String msg_name;

	fseek(fp, offset, 0);
	secNum = ldInt(fp);

	// Build the message table
	while (secNum != ERR) {
		msg_name = ldString(fp);
		offset = ldInt(fp);
		tab.add(MsgFileNode(secNum, msg_name), offset);
		secNum = ldInt(fp);
	}
	
}

String MsgFile::search(Int section, const String &name, bool informError)
{
	if (fp != NULL) {
		Int *fpPos;

		MsgFileNode fileNode(section, name);

		if ((fpPos = tab.find(fileNode)) != NULL) {

			Int hashV = hashVal(fileNode, MSG_HASH);

			if (cache[hashV].offset == *fpPos)
				return cache[hashV].msgDesc;

			fseek(fp, *fpPos, 0);

			String ret = ldString(fp);

			if (!ret.isNull()) {
				cache[hashV].offset = *fpPos;
				cache[hashV].msgDesc = ret;
				if (ret.length() > 0 && ISLINK(ret)) {
					String tmp = LINKMSG(ret);
					if (tmp == name)
						return "(ERROR: infinite recursion detected)";

					return search(section, tmp, informError);
				}
			}

			return ret;
		}
	}

	if (informError)
		return HIGH_VALUE;

	assert(sectionMap.elem(section) != NULL);
	return module + "." + *sectionMap.elem(section) + "." + name +
			": Message not found.";
}

MsgTable::MsgTable(const String &m, const String &s, bool infError)
:	informError(infError)
{
	String secName = s;

	// NULL_STRING means main section
	if (secName == NULL_STRING) 
		secName = MAIN_SECTION;

	file = MsgFile::find(m);

	// If this section doesn't exist return a tr_new section
	if ((section = file->sectionNumber(secName)) == ERR)
		section = file->addSection(secName);
}

MsgTable::~MsgTable()
{
	MsgFile::dispose(file);
}

String MsgTable::find(const String &name, bool infErr)
{
	assert(file != NULL);
	return file->search(section, name, infErr);
}

String MsgTable::find(const String &name)
{
	assert(file != NULL);
	return file->search(section, name, informError);
}

String MsgTable::find(const String &sec, const String &name, bool infErr)
{
	assert(file != NULL);

	Int secNo = ERR;

	// If this section doesn't exist return a tr_new section
	if ((secNo = file->sectionNumber(sec)) == ERR)
		secNo = file->addSection(sec);

	return file->search(secNo, name, infErr);
}

String MsgTable::find(const String &sec, const String &name)
{
	return find(sec, name, informError);
}

Message MsgTable::operator()(const String &name)
{
	return Message(find(name));
}

Message MsgTable::operator()(const String &sec, const String &name)
{
	return Message(find(sec, name));
}

// end of msgtable
