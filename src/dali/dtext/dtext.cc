/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dtext.cc,v 1.11 2006/02/17 15:29:21 spark Exp $
*
* DESCRIPTION: Implementation of DaliText class
*********************************************************************/
#ifdef __GNUC__
#pragma implementation
#endif

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include <dtext.h>
#include <editor.h>
#include <winapp/window.h>
#include <local/osfile.h>
#include <local/strfile.h>
#include <datroff.h>
#include <datex.h>
#include <ifound/stdc.h>
#include <errno.h>
#include <sys/stat.h>
#include <winapp/waitmsg.h>
#include <local/dirname.h>
#include <local/password.h>
#include <flock.h>

#ifdef HAVE_UNDERSCORE_NAMES
#	define stat _stat
#endif

// static variables
String DaliText::tempDir;
Int DaliText::tabSize_ = 4;

// private methods

String DaliText::encodeTabs(String s)
{
	Int len = s.length(); 
	for (Int i = 0; i < len; i++) {
		if (s[i] == '\t') {
			Int cant = NFILLTAB(i);
			for (Int j = 0; j < cant; j++)
				s.insChar(FILL_TAB, i);
			i += cant;
			len += cant;
		}
	}
	return s;
}

String DaliText::decodeTabs(String s)
{
	Int len = s.length(); 
	Int i = 0;

	while (i < len) {
		if (s[i] != FILL_TAB) 
			i++;
		else {
			s.delChar(i);
			len--; 
		}
	}
	return s;
}

void DaliText::encodeTroffSeq(Int rowb, Int rowe)
{
	idCheck();
	register int i, j;
	const int TrCharSeq = 4;
	const int IdCharSeq = 4;
	static char savefont, currfont;

	// Set default font
	savefont = currfont = 'R';

	// Define cursors for traversing the text
	DaliTextCursor tcupto(*this);
	DaliTextCursor tcfrom(*this);

	// Obtain range
	if (rowb == ERR)
		tcfrom.goToFirst();
	else
		tcfrom.goTo(rowb);

	if (rowe == ERR) {
		tcupto.goToLast();
		tcupto.goToCol(tcupto.value().length());
	} else
		tcupto.goTo(rowe);

	// for each line in the text
	for (; tcfrom <= tcupto; ++tcfrom) { 

		const String &ss = tcfrom.value();

		// for each character in the string
		for (i=0; i <= ss.length(); i++) {

			// This String declarations can not be after the continue
			// because of a bug in gcc-2.5.7!
			UnChar c;
			String tosearch;

			// Skip normal characters
			if (ss[i] != '\\')
				continue;

			// Determine especial sequence
			switch (ss[i+1]) {
				case '(':
					tosearch = ss(i+2, IdCharSeq-2);		// skip \(
					if (TroffSpecialChar::SearchString(tosearch, c) != ERR) {
						tcfrom.goToCol(i);
						for (j=0; j < TrCharSeq; j++)
							tcfrom.delChar();
						tcfrom.insChar(c);
					}
					break;
			}
		}				
	}
}


#define MAXPAR				10
#define DEFAULT_PAR			0x0000
String DaliText::decodeTroffSeq(String ss)
{
	idCheck();
	UnChar intchr;
	register int i, j;
	bool in_sec = false;
	short _par[MAXPAR], npar = 0;
	
	// for each character in the string
	String aux = "";
	Int sslen = ss.length();
	for (j=0; j < sslen; j++)
	{
		// Check for Ideafix characters
		UnChar ch = (UnChar) ss[j];
		if (ch > 128) {
			String searched, prefix("\\(");
			if (TroffSpecialChar::SearchChar(ch, searched) != ERR) 
					aux += prefix + searched;
		}
		// Check for attribute sequences
		else if (in_sec || ch=='\033') {

			if (isdigit(ch)) {
				if (_par[npar] == DEFAULT_PAR) _par[npar] = 0;
				_par[npar] = _par[npar] * 10 + ch - '0';
				continue;
			}

			switch (ch) {
				case '\033':
					npar = 0;
					for (i=0; i < MAXPAR; i++)
						_par[i] = DEFAULT_PAR;
					in_sec = true;
					break;
		
				case ';':
					if (npar < MAXPAR-1)
						npar++;
					break;

				case '[':
					intchr = ch;
					break;

				case 'm':
		  			{
					// \033[1;2;...;nm -> estructura general

					// Determine Troff sequence
					for (i=0; i <= npar; i++) {
						switch (_par[i]) {
							case 0:
								aux += "\\fP";
								break;
							case 1:
								aux += "\\fB";
								break;
							case 4:
								aux += "\\fI";
								break;
						}
					}
					in_sec = false;
					}
				break;
			}
		}
		else {
			// Append normal character
			aux += ch;
		}
	}			
	return aux;
}

void DaliText::encodeTeXSeq(Int rowb, Int rowe)
{
	idCheck();
	register int i, j;

	// Define cursors for traversing the text
	DaliTextCursor tcupto(*this);
	DaliTextCursor tcfrom(*this);

	// Obtain range
	if (rowb == ERR)
		tcfrom.goToFirst();
	else
		tcfrom.goTo(rowb);

	if (rowe == ERR) {
		tcupto.goToLast();
		tcupto.goToCol(tcupto.value().length());
	} else
		tcupto.goTo(rowe);

	// for each line in the text
	for (; tcfrom <= tcupto; ++tcfrom) { 

		const String &ss = tcfrom.value();

		// for each character in the string
		for (i=0; i <= ss.length(); i++) {

		   // Skip normal characters
			if (ss[i] != '\\' && ss[i] != '{')    // }
				continue;

		   // Search for TeX sequence
			UnChar c;
			String tosearch = ss(i);                  
			Int texLen = TeXSpecialChar::SearchString(tosearch, c);
			if ( texLen != ERR) {
				tcfrom.goToCol(i);
				for (j=0; j < texLen; j++)
					tcfrom.delChar();
				tcfrom.insChar(c);
			}
			break;
		}				
	}
}

String DaliText::decodeTeXSeq(String ss)
{
	idCheck();
	UnChar intchr;
	register int i, j;
	bool in_sec = false;
	short _par[MAXPAR], npar = 0;
	
	// for each character in the string
	String aux = "";
	Int sslen = ss.length();
	for (j=0; j < sslen; j++)
	{
		// Check for Ideafix characters
		UnChar ch = (UnChar) ss[j];
		if (ch > 128) {
			String searched;
			if (TeXSpecialChar::SearchChar(ch, searched) != ERR) 
					aux += searched;
		}
		// Check for attribute sequences
		else if (in_sec || ch=='\033') {

			if (isdigit(ch)) {
				if (_par[npar] == DEFAULT_PAR) _par[npar] = 0;
				_par[npar] = _par[npar] * 10 + ch - '0';
				continue;
			}

			switch (ch) {
				case '\033':
					npar = 0;
					for (i=0; i < MAXPAR; i++)
						_par[i] = DEFAULT_PAR;
					in_sec = true;
					break;
		
				case ';':
					if (npar < MAXPAR-1)
						npar++;
					break;

				case '[':
					intchr = ch;
					break;
			}
		}
		else {
			// Append normal character
			aux += ch;
		}
	}			
	return aux;
}

void DaliText::addPath()
	// Adds path to filename, just to display it.
{         
	idCheck();
    DirName	fileDir(filename.pathName()),
    	    currDir(Editor::dirName);
    
   	currDir.chDir(fileDir.value());
	filename = currDir.value()+String(PATH_SLASH)+filename.baseFullName();	
}

bool DaliText::storeTmp()
{
	idCheck();
	if (ttype&(T_ISTREAM|T_ADMIN) || !active) return false;

	{
		OutputFile outputf(tmpfile);

		outputf.seek(0);

		// save file name and length
		outputf << file.name() << "\t" << size() << NEW_LINE;

		Int curratr = A_NORMAL;

		for (DaliTextCursor lin(*this); lin; ++lin)
			outputf << decodeAttrs(lin, curratr) << NEW_LINE;

		outputf.sync();
	}

	return true;
}

bool DaliText::rmFiles()
{
	idCheck();
	if (!active || ttype&(T_ISTREAM|T_ADMIN)) return false;

	tmpfile.free(0,0);
	tmpfile.close();
	tmpfile.unLink();

	FileLocker f;
	String node = "localhost";
	Int pid = getpid();
	f.readCtrlFile();
	f.freeFile(file.fullName(), node, pid);
	f.writeCtrlFile();

	file.free(0,0);
	file.close();
	if (!existfile && file.size()==0) 
		file.unLink();

	return true;
}

void DaliText::mkFiles()
{
	idCheck();
	if (ttype & (T_ISTREAM|T_ADMIN)) 
		return;

	if (ttype & T_PRESERVE) {
		tmpfile.open(NULL_STRING, tmpfilename, NULL_STRING, 
					 OsFile::READ|OsFile::WRITE);
		tmpfile.lock(OsFile::LOCK);

		// obtengo el nombre del archivo 
		InputFile inputf(tmpfile);
		inputf >> filename;
		filename = String(filename.field(0, "\t")).baseFullName();
	}
	else {
		tmpfile.open(NULL_STRING, tmpfilename, NULL_STRING, 
					 OsFile::READ|OsFile::WRITE|OsFile::CREAT);
		tmpfile.lock(OsFile::LOCK);
	}

    String dir;

    // The File has already an absolute dir 
#ifndef __NT__
	if (filename[0] == PATH_SLASH[0])
#else
	if (filename[0] == PATH_SLASH[0] || filename[1] == ':')
#endif
		dir = filename.pathName();     
	else
		dir = Editor::dirName+String(PATH_SLASH)+filename.pathName();	
    
	file.open(dir, filename.baseFullName(), NULL_STRING, 
			  OsFile::READ|OsFile::WRITE|OsFile::FAIL);
			  
	fileperm = TXT_WRITE|TXT_READ;
	existfile = true;

	String user = userName(getuid());
	String node = "localhost";
	Int pid = getpid();

	FileLocker f;
	f.readCtrlFile();

	if (file.errNo() == ENOENT) {   // ENOENT, "No such file or directory"
		file.close();
		file.open(dir, filename.baseFullName(), NULL_STRING, 
				  OsFile::READ|OsFile::WRITE|OsFile::CREAT);
		existfile = false;
		f.lockFile(filename.baseFullName(), user, node, pid, 100000, true);
		file.lock(OsFile::LOCK);
	}
	else {
		bool flockeracq = false;
		if (!file.ok() || !(flockeracq = f.lockFile(file.fullName(),
				user, node, pid, 100000, true))
#ifndef __NT__
				|| file.lock(OsFile::TEST|OsFile::LOCK) == IO_LOCKED) {
#else
				){
#endif
			// already locked -> can only open read only
			fileperm = TXT_READ;
			if (file.ok()) {
				fileperm |= TXT_LOCKED;
				if (!flockeracq) 
					lockowner = user + "@" + node.field(0, ".");
				else {
					// free the acquired flocker lock
					f.freeFile(file.fullName(), node, pid);

					Int lkpid = file.lockOwner();
					if (lkpid != ERR) {
#ifndef __NT__
						Password pwd(lkpid, 0);
						lockowner = pwd.userName();
#else
						lockowner = userName(getuid());
#endif
					}
				}
			}
			file.close();
			file.open(dir, filename.baseFullName(), NULL_STRING, OsFile::READ);
		}
	}
	f.writeCtrlFile();
}

#define MAX_FILE_LEN	1024

const String DaliText::tempFile()
{
	static char tmp[MAX_FILE_LEN] = { '0' };
	assert(DaliText::tempDir.length() < MAX_FILE_LEN);
	strcpy(tmp, toCharPtr(DaliText::tempDir));
	mkstemp(tmp);

	// because a linux bug!!!!
	assert(*tmp != 0);
	return tmp;

//	return mktemp(tmp);
}

// metodos publicos

DaliText::DaliText(String f, bool isTemp) 
: List(String)(NULL_STRING)
{
	idStart();
	active 	 	= false;
	
	if (isTemp) {
		tmpfilename = f;
		filename 	= NULL_STRING;
		ttype 		= T_PRESERVE;
	}
	else {         
		tmpfilename = tempFile();
		filename 	= f;
		ttype 	 	= T_NORMAL;         
        struct stat st;
	    if (statFile(filename,&st) != OK) {
		    String pipeline;
		    InputPipe getadm(String("checkout -p ")+f);
		    while(getadm >> pipeline) 
		        add(encodeTabs(pipeline));
    		if(getadm.withError()) {
    		   delAll();
    		}
    		else {  
    		   	ttype		= T_ADMIN;
    		   	tmpfilename = NULL_STRING;
    		   	filename 	= f;
				fileperm 	= TXT_READ;
				existfile 	= true;
    		   	active 		= true;   
    		   	encodeAttrs();
    		   	resetModif();
    		}
	    }
	}

	// Check for troff source files
	if (filename.field(-1, ".") == "mm") 
		ttype |= T_TROFF;

	// Check for troff source files
	if ( (filename.field(-1, ".") == "tex") 
		|| (filename.field(-1, ".") == "latex"))
		ttype |= T_TEX;

	cview = 0;
}

DaliText::DaliText() 
: List(String)(NULL_STRING)
{ 
	idStart();
	filename = NULL_STRING;
	tmpfilename = NULL_STRING;
	active = false;
	cview = 0;
	ttype = T_NORMAL;
}

DaliText::DaliText(InputStream &is, const String &cmd)
: List(String)(NULL_STRING)
{                                      
	idStart();
	ttype 		= T_ADMIN;
	tmpfilename = NULL_STRING;
	cview 		= 0;
	fileperm 	= TXT_READ;
	existfile 	= true;
	filename 	= cmd;

	{
	    WaitMsg pleaseWait(Editor::editorMsg("ONEMOMENT"), 
	    					NULL_STRING);   // , K_END to abort
	    pleaseWait.execute();
		String aux;
		while (is >> aux) {
			add(encodeTabs(aux));
#if 0
			if (pleaseWait.isAborted())
				break;
#endif				
		}
	}

	active = true;
	resetModif();
}

DaliText::DaliText(const String &cmd)
: List(String)(NULL_STRING)
{                                      
	idStart();
	ttype 		= T_ISTREAM;
	tmpfilename = NULL_STRING;
	cview 		= 0;
	fileperm 	= TXT_READ;
	existfile 	= true;
	filename 	= cmd;
	active      = true;
}

DaliText::~DaliText()
{ 
	idCheck();

	if (!(ttype & (T_ISTREAM|T_ADMIN)) && active) {
		// toDo: maybe call rmFiles()?... for the moment we make sure that
		// the file is unlocked now.
		FileLocker f;
		String node = "localhost";
		Int pid = getpid();
		f.readCtrlFile();
		f.freeFile(file.fullName(), node, pid);
		f.writeCtrlFile();
	}

	idStop();
}

void DaliText::setFile(const String &f, bool load_new_file)
{ 
	idCheck();
	// debo dejar el text en el mismo estado que estaba
	if (!active && !isIStream() && !isAdmin()) {
		ttype 		= T_NORMAL;
		filename 	= f;
		tmpfilename = tempFile();
	} 
	else {                 
		if (!(ttype & (T_ADMIN|T_ISTREAM)))
			rmFiles();
	
		ttype 		= T_NORMAL;
		tmpfilename = tempFile();
		filename 	= f;
        
        // Check for troff source files
		if (filename.field(-1, ".") == "mm") 
			ttype |= T_TROFF;

		// Check for troff source files
		if ( (filename.field(-1, ".") == "tex") 
			|| (filename.field(-1, ".") == "latex"))
			ttype |= T_TEX;

		mkFiles();

		if (load_new_file) 
			load();
		resetModif();
	}
}

const String &DaliText::getFile()
{
	idCheck();
	if (ttype&T_PRESERVE && !active) 
		return tmpfilename;
	return filename;
}

bool DaliText::load()
{
	idCheck();
	if (!active || ttype&(T_ISTREAM|T_ADMIN)) return false;

	delAll();

	String aux;

	if (ttype & T_PRESERVE) {
		InputFile inputf(tmpfile);
		// Read file name and file length (in lines)
		inputf >> aux;
		Int nl = toInt(aux.field(1, "\t"));
	
		for (Int i = 0; i < nl; i++) {
			inputf >> aux;
			add(encodeTabs(aux));
		}
	} else {
		InputFile inputf(file);

		for(Int i = 0; inputf >> aux; i++) 
			add(encodeTabs(aux));
	}

	// Encode Troff sequences
	if (ttype & T_TROFF) 
		encodeTroffSeq();

	// Encode TeX/Latex sequences
	if (ttype & T_TEX) 
		encodeTeXSeq();

	 // Encode attributes
	encodeAttrs();

	resetModif();

	return true;
}

void DaliText::encodeAttrs(Int filfrom, Int colfrom, Int filto, Int colto)
{
	idCheck();
	Int col, curratr;

	// obtengo la longitud de los atributos
	int nlen  = strlen(ATTR_NORMAL);
	int battr = strlen(BEGIN_ATTR);
	//int blen  = strlen(ATTR_BOLD);
	//int bklen = strlen(ATTR_BLINK);
	//int rlen  = strlen(ATTR_REVERSE);
	//int ulen  = strlen(ATTR_UNDER);

	// veo los limites
	DaliTextCursor begin(*this);
	DaliTextCursor end(*this);
	end.goToLast();end.goToCol(end.value().length());

	if (filfrom != ERR && colfrom != ERR && filto != ERR && 
												colto != ERR) {
		begin.goTo(filfrom, colfrom);
		end.goTo(filto, colto);
	}

	// recorro todo el texto
	for (DaliTextCursor tc = begin; tc < end; ++tc) {

		// recorro cada linea en busca de un escape
		col = 0;
		while ((col=tc.value().index(*BEGIN_ATTR, col))!=ERR) {

			// verifico si es valido	
			const char *str = toCharPtr(tc.value())+col;

			// si es un atributo normal lo borro y salteo
			if (strncmp(str, ATTR_NORMAL, nlen)==0) {
				tc.goToCol(col);
				for (Int i = 0; i < nlen; i++)
					tc.delChar();
				tc.goToCol(0);
				continue;
			}

			// busco si abre algun atributo
			if (strncmp(str, BEGIN_ATTR, battr) == 0) {

				Int skip;
				if ((curratr = Editor::sequenceToAttrib(str, skip)) !=
						ERR) {
					tc.goToCol(col);
					while (skip--)
						tc.delChar();
					tc.goToCol(0);
				} else {
					col += skip;
					continue;
				}
#if 0
				const char *backstr = str;

				// avanzo para buscar los atributos
				str += battr;

				curratr = A_NORMAL;

				forever {
					if (*str == *ATTR_BOLD) 
						curratr |= A_BOLD;
					else if (*str == *ATTR_BLINK) 
						curratr |= A_BLINK;
					else if (*str == *ATTR_REVERSE) 
						curratr |= A_REVERSE;
					else if (*str == *ATTR_UNDER) 
						curratr |= A_UNDERLINE;
					else break;
					if (*++str == SEPAR_ATTR)
						str++;
					else 
						break;
				} 

				// era una secuencia valida
				if (*str == END_ATTR) {
					str++;
					tc.goToCol(col);
					for (Int i = str-backstr; i; i--)
						tc.delChar();
					tc.goToCol(0);
				} else {
					// si no era valida adelanto la columna y sigo
					col += (str - backstr);
					continue;
				}
#endif
			} else {
				col++;
				continue;
			}

			// busco el cierre del atributo
			bool find = false;
			DaliTextCursor tcur = tc;
			for (tcur.goToCol(col); tcur && !find; ++tcur) {

				Int tcol = tcur.column();

				// busco el fin de la secuencia
				while ((tcol=tcur.value().index(*BEGIN_ATTR, tcol))!=
																ERR) {
					str = toCharPtr(tcur.value())+tcol;

					// encontre la sequencia que cierra
					if (strncmp(str, ATTR_NORMAL, nlen)==0) {
						tcur.goToCol(tcol);
						tcur.moveCol(-1);
						tc.goToCol(col);
						setAttr(tc, tcur, curratr);
						tc.goToCol(0);
						find = true;
						break;
					} else
						++tcol;
				}

				tcur.goToCol(0);
			}
			// no lo encontre, lo mando al final del texto
			if (!find) {
				end.moveCol(-1);
				tcur.goTo(end.order(), end.column());
				tc.goToCol(col);
				setAttr(tc, tcur, curratr);
				tc.goToCol(0);
			}
		}
	}
}

String DaliText::decodeAttrs(DaliTextCursor &tc, Int &curratr, Int cant)
{
	idCheck();
	Int len = tc.value().length();
	Int svcol;

	// check column
	if (len <= (svcol = tc.column()))
		return NULL_STRING;

	// correct cant
	if (cant != ERR && cant + svcol > len)
		cant = len - svcol;

	Int col, attr, desp, nlen = strlen(ATTR_NORMAL);

	// take the necessary data
	String aux = tc.value()(svcol, 
							len = (cant == ERR ? len - svcol : cant));

	// offset for insertion
	desp = 0;

	do {
		// if attribute differs -> operate
		if ((attr = getAttr(tc, col)) != curratr) {
							
			// closing sequence
			if (curratr != A_NORMAL) {
				char *seq = ATTR_NORMAL;
				for (Int c = tc.column()+desp-svcol; *seq; seq++, c++)
					aux.insChar(*seq, c);
				desp += nlen;
			}

			// opening sequence
			String strseq = Editor::attribToSequence(attr);

			if (strseq) {
				const char *seq = toCharPtr(strseq);
				for (Int c = tc.column() + desp - svcol; *seq; seq++, c++)
					aux.insChar(*seq, c);
				desp += strseq.length();
			}
		}

		// move tc to the end of the attribute
		tc.goToCol(col+1);

		// modify current attribute
		curratr = attr;

	} while (col - svcol < len - 1);

	// closing sequence
	if (curratr != A_NORMAL && col != INT_MAX) {
		char *seq = ATTR_NORMAL;
		Int c;
		for (c = tc.column() + desp - svcol; *seq; seq++, c++)
			aux.insChar(*seq, c);
		desp += nlen;
		curratr = A_NORMAL;
	}

	// restore column
	tc.goToCol(svcol);

	return aux;
}

bool DaliText::store()
{
	idCheck();
	if (!active) return false;

	String newfile = filename;

	if (ttype & (T_NORMAL|T_TROFF|T_TEX)) {
		storeTmp();
		file.close();
		newfile = file.fullName();
	}

	store(newfile);

	file.open(OsFile::READ|OsFile::WRITE|OsFile::FAIL);

	assert(file.ok());

	// if lock fails -> view is now locked and read only
	if (file.lock(OsFile::LOCK) == IO_LOCKED) {
		fileperm = TXT_LOCKED|TXT_READ;        
	}		

	existfile = true;
	ttype = T_NORMAL 
			| ((ttype & T_TROFF) ? T_TROFF : 0) 
			| ((ttype & T_TEX) ? T_TEX : 0);

	resetModif();

	return true;
}

void DaliText::store(const String &newfile)
{
	idCheck();

	OutputFile outputf(newfile);

	Int curratr = A_NORMAL;
	if (ttype & T_TROFF) {
		for (DaliTextCursor cursor(*this); cursor; ++cursor) {
			outputf << decodeTroffSeq(
					decodeTabs(decodeAttrs(cursor, curratr))
					) << NEW_LINE;
		}
	} else {
		if (ttype & T_TEX) {
			for (DaliTextCursor cursor(*this); cursor; ++cursor) {
				outputf << decodeTeXSeq(
						decodeTabs(decodeAttrs(cursor, curratr))
						) << NEW_LINE;
			} 
		}
		else {
			for (DaliTextCursor cursor(*this); cursor; ++cursor)
				outputf << decodeTabs(decodeAttrs(cursor, curratr)) << NEW_LINE;
		}				
	}
}

bool DaliText::activate()
{
	idCheck();
	if (active || (ttype&(T_ISTREAM|T_ADMIN)))
		return false;

	if (tmpfilename == NULL_STRING) return false;

	active = true;

	mkFiles();

	load();

	return true;
}

void DaliText::deActivate()
{
	idCheck();
	if (!active || ttype&(T_ISTREAM|T_ADMIN)) return;

	rmFiles();

	delAll();

	resetModif();

	ttype = T_NORMAL;
	active = false;
}

bool DaliText::insertText(List(String) &l, DaliTextCursor &c)
{
	idCheck();
	// no es este texto ERR
	if ((DaliText*)(c.list) != this)
		return false;

	ListCursor(String) lc(l);	

	// la lista esta vacia
	if (!lc) return false;

	// inserto el string en el cursor
	c.insString(lc.value());

	// si la lista tenia solo un elemento -> listo
	if (l.size() == 1) {
		encodeAttrs(c.order(), c.column(), c.order(), 
					c.column()+lc.value().length());
		return true;
	}

	// guardo las coordenadas del cursor
	Int oldfil = c.order();
	Int oldcol = c.column();

	// coloco el cursor para realizar el insNewLine		
	c.moveCol(lc.value().length());

	// inserto el new line
	c.insNewLine(false);

	// inserto el new line
	// ahora inserto la ultima linea
	lc.goToLast();

	// me fijo en el ultimo valor
	if (lc.value() != NULL_STRING)
		c.insString(lc.value());
	
	// hago el insert de las demas lineas
	for (lc.goTo(1); lc.order() < l.size()-1; ++lc) {
		if (!c) {
			c.set(lc.value());
			c++;
		} else c.ins(lc.value());
	}

	// codifico los atributos
	encodeAttrs(oldfil, oldcol, c.order(), 
				c.column()+lc.value().length());

	c.goTo(oldfil, oldcol);

	return true;
}

bool DaliText::getRegion(List(String) &l, DaliTextCursor &begin, DaliTextCursor &end, bool adding)
{     
	idCheck();
	if (!end) {
		end = *this;
		end.goToLast();
		Int len = end.value().length();
		end.goToCol(len); //end.goToCol(len ? len-1 : 0);
	}                          

	// valido los cursores
	if ( ((DaliText*)(begin.list)) != this || 
	     ((DaliText*)(end.list)) != this || 
	     begin >= end  ) return false;

	// atributo en normal para tomar la region
	Int currattr = A_NORMAL;

	// coloco la primer linea
	Int len = begin.value().length();
	if (end.order() == begin.order() && end.column() + adding < len)
		len = end.column() + adding;

	len -= begin.column();

	// veo como inserto la primer linea
	// si hay algo para insertar inserto
	if (len > 0) 
		l.add(decodeAttrs(begin, currattr, len));
	// si no hay pero no es la unica linea -> indica un tr_new line
	else if (len <= 0 && begin.order() != end.order())
		l.add(NULL_STRING);
	 	
	// hay solo una linea -> termino aca
	if (end.order() == begin.order()) return true;

	// copio las demas linea
	for (begin.goTo(begin.order() + 1, 0);
			begin && begin.order() < end.order(); ++begin)
		l.add(decodeAttrs(begin, currattr));

	// coloco la ultima linea
	len = begin.value().length();   

	if (end.column() + adding < len)
		len = end.column() + adding ;

	len -= begin.column();

	if (len <= 0 || begin.column()==end.column()+adding)
		l.add(NULL_STRING);
	else
		l.add(decodeAttrs(begin, currattr, len));

	return true;
}

bool DaliText::delRegion(DaliTextCursor &begin, DaliTextCursor &end, bool compress,
					 bool correc)
{
	idCheck();
	// valido los cursores
	if ((DaliText*)(begin.list) != this || (DaliText*)(end.list) != this || 
		begin >= end) return false;

	// no hay que realizar correccion 
	if (!correc) {

		// borro los atributos de la zona
		delAttr(begin, end, A_UNDERLINE|A_REVERSE|A_BLINK|A_BOLD);

		begin.delTo(end.order(), end.column(), false, compress);
	}
	else {
		// estoy dentro del texto
		if (end) {
			// estoy parado en numero de columna <> 0
			if (end.column()) {

				Int len = end.value().length();
				Int col = end.column();

				if (len == 0) len = 1;

				if (col < len)
					end.moveCol(-1);
				else
					end.goToCol(len-1);

				// borro los atributos de la zona
				delAttr(begin,end,A_UNDERLINE|A_REVERSE|A_BLINK|A_BOLD);

				begin.delTo(end.order(), end.column(), false, compress);

				end.goToCol(col);
			}
			// estoy en la columna 0
			else {
				--end;

				Int len = end.value().length();
				if (len == 0) len = 1;

				end.goToCol(len-1);

				// borro los atributos de la zona
				delAttr(begin,end,A_UNDERLINE|A_REVERSE|A_BLINK|A_BOLD);

				begin.delTo(end.order(), end.column(), true, compress);

				end.goTo(end.order()+1, 0);
			}
		}
		// estoy fuera del texto
		else {
			Int fil = end.order();
			Int col = end.column();

			end.goToLast();

			Int len = end.value().length();
			if (len == 0) len = 1;

			end.goToCol(len-1);

			// borro los atributos de la zona
			delAttr(begin, end, A_UNDERLINE|A_REVERSE|A_BLINK|A_BOLD);

			begin.delTo(end.order(), end.column(), true, compress);

			end.goTo(fil, col);
		}
	}

	return true;
}

Int DaliText::getAttr(DaliTextCursor &tc, Int &hasta)
{
	idCheck();
	return attrzn.getAttribute(tc, hasta);
}

void DaliText::setAttr(DaliTextCursor &b, DaliTextCursor &e, Int attr)
{
	idCheck();
	if (attrzn.setAttribute(b, e, attr))
		if (attr != A_STANDOUT)
			setModif();
}

void DaliText::delAttr(DaliTextCursor &b, DaliTextCursor &e, Int attr)
{
	idCheck();
	if (attrzn.delAttribute(b, e, attr))
		if (attr != A_STANDOUT)
			setModif();
}

void DaliText::setTroff()
{
	idCheck();
	if (ttype & T_TROFF)
		ttype &= ~(T_TROFF);
	else ttype |= T_TROFF;

	setModif();
}

void DaliText::setTeX()
{
	idCheck();
	if (ttype & T_TEX)
		ttype &= ~(T_TEX);
	else ttype |= T_TEX;

	setModif();
}

void DaliText::setAdmin()
{
	idCheck();
	if (ttype & T_ADMIN)
		ttype &= ~(T_ADMIN);
	else ttype |= T_ADMIN;
}

const String &DaliText::lockOwner()
{
	idCheck();
	return lockowner;
}

// end of text
