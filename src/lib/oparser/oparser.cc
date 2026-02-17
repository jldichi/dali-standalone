/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: oparser.cc,v 1.2 2004/09/29 15:36:09 damianm Exp $
*
* DESCRIPTION
* Implementation of option parsing class
*********************************************************************/

#include <ifound/osdep.h>

#ifdef __GNUC__
#pragma implementation
#endif

#include <string.h>

#include <guiconn/guiconn.h>
#include <ifound/oparser.h>
#include <ifound/priv/macros.h>
#include <ifound/stdc.h>
#include <ifound/parray.h>
#include <ifound/libinfo.h>

// +++ Config +++
const Int OPT_REALLOC = 4;
const char *BANNER_STRING = "hide-banner";
const char BANNER_CHAR = 'b';
const char *VAL_SEPARATOR = "\t";
const char HELP_CHAR = 'H';
const char *HELP_STRING = "help";
const char VERS_CHAR = 'V';
const char *VERS_STRING = "version";
const char *COMP_STRING = "compile-info";
const char WIN_CHAR = 'W';
const char *WIN_STRING = "window";
// --- Config ---

declare(PtrArray, Option);

// Option class
                                             
Option &Option::operator=(const Option &o)
{
	if (this != &o) {
		seq = o.seq;
		dsc = o.dsc;
		lett = o.lett;
	}

	return *this;   
}

String Option::description() const
{
	if (dsc[0] != '[')
		return dsc;

	String optdsc = dsc(1, dsc.length() - 2);
	return appMsg(optdsc);
}

void Option::setValue(bool, const char *) {}

bool StrOption::hasArguments() const { return true; }
bool NumOption::hasArguments() const { return true; }
bool BoolOption::hasArguments() const { return false; }

String StrOption::argRep() const { return STR_REP; }
String NumOption::argRep() const { return NUM_REP; }
String BoolOption::argRep() const { return BOOL_REP; }

// end of Option class


// StrOption class

StrOption::StrOption(String &var, const String &s, const String &desc, 
		const String &def, const String &vals)
:	Option(s, s[0], desc), str(var), val(dummyVal), values(vals)
{ str = def; val = findVal(def); }

StrOption::StrOption(String &var, const String &s, WideChar l,
		const String &desc, const String &def, const String &vals)
:	Option(s, l, desc), str(var), val(dummyVal), values(vals)
{ str = def; val = findVal(def); }

StrOption::StrOption(String &var, Int &nval, const String &s,
		const String &desc, const String &def, const String &vals)
:	Option(s, s[0], desc), str(var), val(nval), values(vals)
{ str = def; val = findVal(def); }

StrOption::StrOption(String &var, Int &nval, const String &s, WideChar l,
		const String &desc, const String &def, const String &vals)
:	Option(s, l, desc), str(var), val(nval), values(vals)
{ str = def; val = findVal(def); }

StrOption::StrOption(const StrOption &so)
:	Option(so), str(so.str), val(so.val), values(so.values)
{ }

Int StrOption::dummyVal = 0;

Int StrOption::findVal(const String &s)
{
	if (!s) return ERR;

	// Look for exact match
	Int j = 0;
	String v;
	while (!(v = values.field(j++, VAL_SEPARATOR)).isNull())
		if (v == s)
			return j-1;

	// Now look for first partial match
	Int len = s.length();
	j = 0;
	while (!(v = values.field(j++, VAL_SEPARATOR)).isNull())
		if (v(0, len) == s)
			return j-1;

	return ERR;
}

void StrOption::setValue(bool, const char *v)
{
	if (!values.isNull()) {
		if ((val = findVal(v)) == ERR)
			error(String(v)+" argument not valid for "+seq+
					" option.");
		str = values.field(val, VAL_SEPARATOR);
	} else {
		str = v;
		val = ERR;
	}
}

// end of StrOption class


// NumOption class

NumOption::NumOption(Int &var, const String &s, const String &desc,
		Int def, Int minimum, Int maximum)
:	Option(s, s[0], desc), n(var)
{ n = def; min = minimum; max = maximum; }

NumOption::NumOption(Int &var, const String &s, WideChar l,
		const String &desc, Int def, Int minimum, Int maximum)
:	Option(s, l, desc), n(var)
{ n = def; min = minimum; max = maximum; }

NumOption::NumOption(const NumOption &no)
:	Option(no), n(no.n)
{ min = no.min; max = no.max; }

void NumOption::setValue(bool, const char *v)
{
	Int tmp = toInt(v);
	if (tmp >= min && tmp <= max)
		n = tmp;
	else
		warning(String("Option '")+seq+"' not between allowed range ("
				+toString(min)+" - "+toString(max)
				+"),\nassuming default ("+toString(n)+")");
}


// end of NumOption class


// BoolOption class

BoolOption::BoolOption(bool &var, const String &s, const String &desc,
		bool def)
:	Option(s, s[0], desc), b(var)
{ b = def; }

BoolOption::BoolOption(bool &var, const String &s, WideChar l,
		const String &desc, bool def)
:	Option(s, l, desc), b(var)
{ b = def; }

BoolOption::BoolOption(const BoolOption &bo)
:	Option(bo), b(bo.b)
{ }

void BoolOption::setValue(bool negation, const char *)
{
	b = !negation;
}

// end of BoolOption class


// OptionParser class

OptionParser::OptionParser(const String &vers)
:	version(vers),
	opts(*tr_new PtrArray(Option)(0, OPT_REALLOC))
{
}

OptionParser::~OptionParser()
{
	// Clean things up
	Int nOpts = opts.dim(), i;
	Option *opt;
	for (i = 0; i < nOpts; i++)
		if ((opt = opts.elem(i)))
			tr_delete(opt);

	PtrArray(Option) *p = &opts;
	tr_delete(p);
}

String OptionParser::help()
{
	int i;
	Int nOpts = opts.dim();
	String aux;
	// Hardcoded help command...
	aux += "-";
	aux += HELP_CHAR;
	aux += ", --";
	aux += HELP_STRING;
	aux += " : ";
	aux += libMsg("HELPOPT");
	aux += "\n";
	// Hardcoded version command...
	aux += "-";
	aux += VERS_CHAR;
	aux += ", --";
	aux += VERS_STRING;
	aux += " : ";
	aux += libMsg("VERSOPT");
	aux += "\n";
 	// Hardcoded banner command... 
	aux += "-";
	aux += BANNER_CHAR;
	aux += ", --";
	aux += BANNER_STRING;
	aux += " : ";
	aux += libMsg("BANNEROPT");
	aux += "\n";
	String optdsc;
	Option *opt;
	for (i = 0; i < nOpts; i++) {
		opt = opts.elem(i);
		bool hasLetter = false;
		if (opt->letter() != '\0') {
			aux += "-";
			aux += String(1, opt->letter());
			hasLetter = true;
		}
		if (opt->sequence() != NULL_STRING) {
			if (hasLetter)
				aux += ", ";
			aux += "--";
			aux += opt->sequence();
		}
		if (opt->hasArguments()) {
			aux += String(" ") + opt->argRep();
		}
		aux += " : ";
		aux += opt->description() + "\n";
	}
	return aux;
}

Int OptionParser::parse(int &argc, char **argv)
{
	Int nOpts;
	Option *opt;
	bool negation;
	bool bann = (version != NULL_STRING);
	bool useWindow = false;
	const char *curr, *arg = NULL;
	int i, j = 0;
	bool longOpt;
	for (i = 1; i < argc; i++) {

		longOpt = false;
		curr = argv[i];

		if (*curr != '-') {
			break;
		}

		// Special cases
		if (*++curr == '-') {
			// -- followed by nothing means end of options
			if (isspace(*++curr)) {
				i++;
				break;
			}

			/*
				-- followed by anything that is not a white space means
				we should expect a long option.
			*/
			longOpt = true;
		}

		if (!longOpt) {
			switch (*curr) {
			case HELP_CHAR:
				error(appMsg("USAGE") + "\n" + help());
				break;
			case BANNER_CHAR:
				bann = false;
				goto next_arg;
			case WIN_CHAR:
				useWindow = true;
				goto next_arg;
			case VERS_CHAR:
				{
					String aName = appName();
					error(libMsg("VERSION"), toCharPtr(aName),
							version != NULL_STRING ? toCharPtr(version) : "1.0");
				}
				break;
			}
		} else if (strcmp(curr, HELP_STRING) == 0) {
			error(appMsg("USAGE") + "\n" + help());
		} else if (strcmp(curr, BANNER_STRING) == 0) {
			bann = false;
			goto next_arg;
		} else if (strcmp(curr, WIN_STRING) == 0) {
			useWindow = true;
			goto next_arg;
		} else if (strcmp(curr, VERS_STRING) == 0) {
			String aName = appName();
			error(libMsg("VERSION"), toCharPtr(aName),
					version != NULL_STRING ? toCharPtr(version) : "1.0");
		} else if (strcmp(curr, COMP_STRING) == 0) {
			if (compileData_.isNull())
				error("No compile-info available.");
			else
				error(compileData_ + LibraryInfo::libraries());
		}

		if (strncmp(curr, NEG_PREFIX, sizeof(NEG_PREFIX)-1) == 0) {
			curr += sizeof(NEG_PREFIX)-1;
			negation = true;
		} else
			negation = false;

		// Look for exact match
		nOpts = opts.dim();
		if (longOpt) {
			for (j = 0; j < nOpts; j++) {
				if ((opt = opts.elem(j))->sequence() == curr) {
					if (opt->hasArguments()) {
						if (negation)
							error("Negation only applies to bool values.");
						if (i == argc - 1)
							error(libMsg("EXPECTEDARG"));
						opt->setValue(negation, argv[++i]);
					} else {
						opt->setValue(negation);
					}
					goto next_arg; // yes, and so what?
				}
			}
		} else {

			if (!*curr)
				error(toString(libMsg("INVOPT"), argv[i]));

			while (*curr++) {
				for (j = 0; j < nOpts; j++) {
					if ((opt = opts.elem(j))->letter() == curr[-1]) {
						if (opt->hasArguments()) {

							if (negation)
								error("Negation only applies to bool values.");

							if (curr[-2] != '-')
								warning("options with arguments can not be combined with other options");

							if (*curr) {
								warning("option arguments should be separated by white space");
								arg = curr;
							} else {
								if (i == argc - 1)
									error(libMsg("EXPECTEDARG"));
								else
									arg = argv[++i];
							}

							opt->setValue(negation, arg);
							goto next_arg; // yes, and so what?
						}
						opt->setValue(negation);
						break;
					}
				}
			}
		}

		if (j == nOpts)
			error(toString(libMsg("INVOPT"), argv[i]));
	next_arg:;
	}
	i--;
			
	// Skip all options (as if they weren't there)
	for (j = 1; j < argc; j++)
		argv[j] = argv[j+i];
	argc -= i;

	if (bann)
		banner(::appName(), version);

	if (useWindow) {
#if GUI_APPLICATION
		GUIConnection *gConn = _appConnection();
		if (gConn != NULL) {
			if (!gConn->editOptions(this))
				stop(1 | ABNORMAL_STOP);
		}
		else
#endif
			warning("Can't edit options on non-GUI application.");
	}

	return 0;
}

void OptionParser::addOption(Option *opt)
{
	opts.add(opt);
}

bool OptionParser::execute()
{
	return true;
}

// end of OptionParser class
