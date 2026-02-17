/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: datex.cc,v 1.1.1.1 1996/04/16 18:52:52 gustavof Exp $
*********************************************************************/

#include <datex.h>
#include <ifound/str.h>
#include <winapp/window.h>

// Definition of SpecialCharTab
TeXSpecialChar *TeXSpecialChar::TeXSpecialCharTab[TEX_TABLE_SIZE];

void TeXSpecialChar::InitTable()
{
	TeXSpecialCharTab[ 0] = tr_new TeXSpecialChar(C_aacute,		"\\'{a}"	);
	TeXSpecialCharTab[ 1] = tr_new TeXSpecialChar(C_aacute,		"\\'a"	);
	TeXSpecialCharTab[ 2] = tr_new TeXSpecialChar(C_eacute,		"\\'{e}"	);
	TeXSpecialCharTab[ 3] = tr_new TeXSpecialChar(C_eacute,		"\\'e"	);
	TeXSpecialCharTab[ 4] = tr_new TeXSpecialChar(C_iacute,		"\\'{\\i}"	);
	TeXSpecialCharTab[ 5] = tr_new TeXSpecialChar(C_iacute,		"\\'\\i"	);
	TeXSpecialCharTab[ 6] = tr_new TeXSpecialChar(C_oacute,		"\\'{o}"	);
	TeXSpecialCharTab[ 7] = tr_new TeXSpecialChar(C_oacute,		"\\'o"	);
	TeXSpecialCharTab[ 8] = tr_new TeXSpecialChar(C_uacute,		"\\'{u}"	);
	TeXSpecialCharTab[ 9] = tr_new TeXSpecialChar(C_uacute,		"\\'u"	);
	TeXSpecialCharTab[10] = tr_new TeXSpecialChar(C_agrave,		"\\`{a}"	);
	TeXSpecialCharTab[11] = tr_new TeXSpecialChar(C_agrave,		"\\`a"	);
	TeXSpecialCharTab[12] = tr_new TeXSpecialChar(C_egrave,		"\\`{e}"	);
	TeXSpecialCharTab[13] = tr_new TeXSpecialChar(C_egrave,		"\\`e"	);
	TeXSpecialCharTab[14] = tr_new TeXSpecialChar(C_igrave,		"\\`{\\i}" );
	TeXSpecialCharTab[15] = tr_new TeXSpecialChar(C_igrave,		"\\`\\i"   );
	TeXSpecialCharTab[16] = tr_new TeXSpecialChar(C_ograve,	    "\\`{o}"  );
	TeXSpecialCharTab[17] = tr_new TeXSpecialChar(C_ograve,	    "\\`o"    );
	TeXSpecialCharTab[18] = tr_new TeXSpecialChar(C_ugrave,		"\\`{u}"  );
	TeXSpecialCharTab[19] = tr_new TeXSpecialChar(C_ugrave,		"\\`u"    );
	TeXSpecialCharTab[20] = tr_new TeXSpecialChar(C_acircunflex,	"\\^{a}"	);
	TeXSpecialCharTab[21] = tr_new TeXSpecialChar(C_acircunflex,	"\\^a"	);
	TeXSpecialCharTab[22] = tr_new TeXSpecialChar(C_ecircunflex,	"\\^{e}"	);
	TeXSpecialCharTab[23] = tr_new TeXSpecialChar(C_ecircunflex,	"\\^e"	);
	TeXSpecialCharTab[24] = tr_new TeXSpecialChar(C_icircunflex,	"\\^{\\i}" );
	TeXSpecialCharTab[25] = tr_new TeXSpecialChar(C_icircunflex,	"\\^\\i"	);
	TeXSpecialCharTab[26] = tr_new TeXSpecialChar(C_ocircunflex,	"\\^{o}"	);
	TeXSpecialCharTab[27] = tr_new TeXSpecialChar(C_ocircunflex,	"\\^o"	);
	TeXSpecialCharTab[28] = tr_new TeXSpecialChar(C_ucircunflex,	"\\^{u}"	);
	TeXSpecialCharTab[29] = tr_new TeXSpecialChar(C_ucircunflex,	"\\^u"	);
	TeXSpecialCharTab[30] = tr_new TeXSpecialChar(C_ntilde,		"\\~{n}"	);
	TeXSpecialCharTab[31] = tr_new TeXSpecialChar(C_ntilde,		"\\~n"	);
	TeXSpecialCharTab[32] = tr_new TeXSpecialChar(C_Ntilde,		"\\~{N}"	);
	TeXSpecialCharTab[33] = tr_new TeXSpecialChar(C_Ntilde,		"\\~N"	);
	TeXSpecialCharTab[34] = tr_new TeXSpecialChar(C_udieresis,		"\\:{u}"	);
	TeXSpecialCharTab[35] = tr_new TeXSpecialChar(C_udieresis,		"\\:u"	);
	TeXSpecialCharTab[36] = tr_new TeXSpecialChar(C_exclamdown,	"{!`}"	);
	TeXSpecialCharTab[37] = tr_new TeXSpecialChar(C_exclamdown,	"{!`"	);
	TeXSpecialCharTab[38] = tr_new TeXSpecialChar(C_questdown,		"{?`}"	);
	TeXSpecialCharTab[39] = tr_new TeXSpecialChar(C_questdown,		"{?`"	);
	// Remember to change TEX_TABLE_SIZE when adding tr_new objects
}

void TeXSpecialChar::DestroyTable()
{
	for (register Int i=0; i < TEX_TABLE_SIZE; i++) 
		tr_delete(TeXSpecialCharTab[i]);
}

Int TeXSpecialChar::SearchString(const String str, UnChar &ch)
{                  
	for (register Int i=0; i < TEX_TABLE_SIZE; i++) {
		Int strLen = TeXSpecialChar::TeXSpecialCharTab[i]->TeX.length();
		String texStr = str(0,strLen);
		if (texStr == TeXSpecialChar::TeXSpecialCharTab[i]->TeX) {
			ch = TeXSpecialCharTab[i]->IdeaChar;
			return strLen;
		}
	}
	return ERROR;
}

Int TeXSpecialChar::SearchChar(const UnChar ch, String &str)
{
	for (register Int i=0; i < TEX_TABLE_SIZE; i++) {
		if (ch == TeXSpecialChar::TeXSpecialCharTab[i]->IdeaChar) {
			str = TeXSpecialCharTab[i]->TeX;
			return OK;
		}
	}
	return ERROR;
}

