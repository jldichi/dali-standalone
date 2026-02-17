/***********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: str.h,v 1.6 1998/11/16 18:55:57 ernestod Exp $
*
* DESCRIPTION
* Class definitions for the String data type
*********************************************************************/

#ifndef	STR_H
#define	STR_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound/substr.h>

struct srep;
class StrCursor;
class RegExp;
class OutputStream;
class InputStream;

// Forward declarations needed for friend functions used outside class scope
class String;
char *toCharPtr(const String &s);
Int toInt(const String &s, Int base = 0);
String toString(Int v, Int length = 0, Int base = -10);
String toString(String fmt, ...);
String toString(const char *fmt, ...);
String operator+(const String &s1, const String &s2);
String operator+(const String &s1, const char *cs);

// String class

class String {

	friend class SubString;
	friend class StrCursor;
	friend class RegExp;
           
//private :

	srep *p;

	void realloc(UnShort len);

public :

	String() { p = 0; }
	String(Int l, char c);
	String(const String&);
	String(const String&, Int len);
	String(const SubString&);
	String(const char * cs, Int len = ERR);
	String(Int len, const char *cs);		// binary data
	~String() { disconnect(); }

	// Be careful with the use of this function
	// Its pointer is valid as long as its parameter is not destroyed!!!
	// (in particular, compiler generated temporary strings are dangerous)
	friend char *toCharPtr(const String &s);
	friend const char *debugString(long s);
	friend Int toInt(const String &s, Int base);
	friend String toString(Int v, Int length, Int base);
	friend String toString(String fmt, ...);
	friend String toString(const char *fmt, ...);
	friend String toStr(const char *fmt, va_list ap);
	friend String toStr(const String &fmt, va_list ap);

	friend String doFormat(String fmt, ...);
	friend String doFormat(const char* fmt, ...);

#ifdef HAVE_BOOL
	operator bool() const;
#else
	operator const void *() const;
#endif
	bool operator! () const;
	bool isNull() const;

	friend Int  cmp(const String &csr, const char *cs); 
	friend bool operator< (const String &csr, const char *cs) 
					{ return cmp(csr, cs) <  0; }
	friend bool operator> (const String &csr, const char *cs) 
					{ return cmp(csr, cs) >  0; }
	friend bool operator<=(const String &csr, const char *cs) 
					{ return cmp(csr, cs) <= 0; }
	friend bool operator>=(const String &csr, const char *cs) 
					{ return cmp(csr, cs) >= 0; }
	friend bool operator==(const String &csr, const char *cs) 
					{ return cmp(csr, cs) == 0; }
	friend bool operator!=(const String &csr, const char *cs) 
					{ return cmp(csr, cs) != 0; }

	friend Int  cmp(const String &csr, const String &s);
	friend bool operator< (const String &csr, const String &s) 
					{ return cmp(csr, s) <  0; }
	friend bool operator> (const String &csr, const String &s)
					{ return cmp(csr, s) >  0; }
	friend bool operator<=(const String &csr, const String &s)
					{ return cmp(csr, s) <= 0; }
	friend bool operator>=(const String &csr, const String &s)
					{ return cmp(csr, s) >= 0; }
	friend bool operator!=(const String &csr, const String &s)
					{ return cmp(csr, s) != 0; }
	friend bool operator==(const String &csr, const String &s)
					{ return cmp(csr, s) == 0; }

	String & operator=(const char*);
	String & operator=(const String &);
	String & operator=(const char c);

	String operator+=(const String &);
	String operator+=(const char *cs);
	String operator+=(const char c);
	friend String operator+(const String &s1,const String &s2);
	friend String operator+(const String &s1,const char *cs);

	Int length() const;
	static Int storeLength(Int l)	{ return sizeof(Int) + l; }
	Int storeLength() const		 	{ return storeLength(length()); }
	SubString operator()(Int pos, Int len);
	SubString operator()(Int pos);
	const SubString operator()(Int pos, Int len) const;
	const SubString operator()(Int pos) const;

	const SubString field(Int n, const char *cs) const;
	const SubString field(Int n, const String &sep) const;
	SubString field(Int n, const char *cs);
	SubString field(Int n, const String &sep);

	unsigned char operator[](Int n) const;

	bool setChar(char c, Int n);
	bool delChar(Int n);
	bool insChar(char c, Int n);

	Int  index(const String &s, Int pos = 0) const;
	Int  index(char c, Int pos = 0) const;
	Int  backIndex(char c, Int pos = INT_MAX) const;

	Int count(char c, Int pos = 0) const;

	String toLower() const;
	String toUpper() const;
	String toAscii() const;
	String fromAscii() const;

	String &encode();
	String &decode();

	String &operator-=(const String &st);
	String replaceArgs(const String args[]) const;
	String replace(const String &search, const String &repl) const;
	String &rTrim();	// Eliminates whitespace at the end of string
	String &lTrim();	// Eliminates whitespace at the beginning of string
	String &fullTrim();	// Calls rTrim() and lTrim()
	String &prune();	// Similar to fullTrim(), but it also eliminates
						// any number of whitespace between words into
						// a single space.

	String &compileMask();
	String center(int width) const;		// return new string  of length width 
										//  centered between spaces (if needed)
	String format(String omask) const;
	String &reverse();			// reverse myself

	String getEnv() const;
	void putEnv() const;
	String expandEnvVars() const;
	String extName() const;
	String baseName() const;
	String baseFullName() const;
	String pathName() const;
	String capitalized() const;
	friend Int hashVal(const String &s, Int modulo);

	void store(OutputStream &stream) const;
	void load(InputStream &stream);

	void store(char *&buff) const;
	void load(const char *&buff);

	void connect();
	void disconnect();

	char *getBuffer(Int len);
	
	bool startsWith(const String & s) const
	{	return bool(cmp(s, (*this)(0, s.length())) == 0); }
};

//-- This macros are useful for instantiating macro-templates --//
typedef String& StringRef;
typedef String* StringPtr;

// Oscar was documented this class (/usr2/acct/oscar/doc/str/String.Doc)
class StrCursor {

// private :

	String str;				// we take substrings from this string
	String tok;				// separator token
	Int len;
	Int pos;

public :

	StrCursor(const String &s, const String &t);
	explicit StrCursor(const String &s);
	void reStart();
	operator void* ();
	bool ok();
	operator String ();
	String value();
	StrCursor& operator ++ (); // prefix
	StrCursor& operator ++ (int); // postfix
	StrCursor& operator -- (); // prefix
	StrCursor& operator -- (int); // postfix

};

// Needed for Symantec
#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL_STRING_DEF
#	ifdef NO_EXTERN_CONST
		DLLDEPEND extern String NULL_STRING;
#	else
		DLLDEPEND extern const String NULL_STRING;
#	endif
#endif

#ifdef __cplusplus
}
#endif

String libMsg(const String &msg);
String appMsg(const String &msg);
String libMsg(const char *);
String appMsg(const char *);

#endif
