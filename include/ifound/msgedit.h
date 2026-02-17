#ifndef MSGEDIT_H
#define MSGEDIT_H

#include <ifound/str.h>

class OutputFile;
class gtype(PtrArray, MsgSection);
class gtype(PtrArray, MsgEntry);

class MsgEntry {

// private:

	Int offset_;
	String key_;
	String msg_;

public:

	explicit MsgEntry(const String &k = NULL_STRING,
			const String &m = NULL_STRING,
			Int offs = ERR)
	: offset_(offs), key_(k), msg_(m)
	{ }

	const String &key() const { return key_; }
	void key(const String &k) { key_ = k; }
	const String &msg() const { return msg_; }
	void msg(const String &m) { msg_ = m; }
	Int offset() const { return offset_; }
	void offset(Int offs) { offset_ = offs; }

};

class MsgSection {

// private:

	String name_;
	gtype(PtrArray, MsgEntry) &entries;

public:
	void clear();

	explicit MsgSection(const String &nm = NULL_STRING);
	~MsgSection();

	const String &name() const { return name_; }
	void name(const String &n) { name_ = n; }

	Int nEntries() const;
	MsgEntry *entry(Int n);
	void del(Int i);
	void ins(Int i, const String &key = NULL_STRING,
			const String &msg = NULL_STRING);
	void add(const String &key = NULL_STRING,
			const String &msg = NULL_STRING);
	void update(const String &key = NULL_STRING,
			const String &msg = NULL_STRING);

	void write(OutputFile &ofile);
	void writeIndex(OutputFile &ofile, Int section);

	void sort();
};

class MsgEditor {

// private:

	String path_;
	String file_;
	String language_;
	gtype(PtrArray, MsgSection) &sections;

	void clear();

	const String &path();
	
public:

	explicit MsgEditor(const String &f = NULL_STRING,
					   const String &l = NULL_STRING);
	~MsgEditor();

	void file(const String &f)		{ file_ = f; path_ = NULL_STRING; }
	void language(const String &l);
	Int nSections() const;
	MsgSection *section(Int i);
	MsgSection *section(const String &sname);
	void delSection(const String &sname);
	Int findSection(const String &s) const;

	bool readSectionMap();
	bool read();
	void write() const;

	void sort();
};

#endif
