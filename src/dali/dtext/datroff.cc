/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: datroff.cc,v 1.1.1.1 1996/04/16 18:52:52 gustavof Exp $
*********************************************************************/

#include <datroff.h>
#include <ifound/str.h>
#include <winapp/window.h>

// Definition of SpecialCharTab
TroffSpecialChar *TroffSpecialChar::TroffSpecialCharTab[TABLE_SIZE];

void TroffSpecialChar::InitTable()
{
	// I don't like this - Santiago
	TroffSpecialCharTab[0] = tr_new TroffSpecialChar(C_aacute,	"a'");
	TroffSpecialCharTab[1] = tr_new TroffSpecialChar(C_eacute,	"e'");
	TroffSpecialCharTab[2] = tr_new TroffSpecialChar(C_iacute,	"i'");
	TroffSpecialCharTab[3] = tr_new TroffSpecialChar(C_oacute,	"o'");
	TroffSpecialCharTab[4] = tr_new TroffSpecialChar(C_uacute,	"u'");
	TroffSpecialCharTab[5] = tr_new TroffSpecialChar(C_agrave,	"a`");
	TroffSpecialCharTab[6] = tr_new TroffSpecialChar(C_egrave,	"e`");
	TroffSpecialCharTab[7] = tr_new TroffSpecialChar(C_igrave,	"i`");
	TroffSpecialCharTab[8] = tr_new TroffSpecialChar(C_ograve,	"o`");
	TroffSpecialCharTab[9] = tr_new TroffSpecialChar(C_ugrave,	"u`");
	TroffSpecialCharTab[10] = tr_new TroffSpecialChar(C_acircunflex,"a^");
	TroffSpecialCharTab[11] = tr_new TroffSpecialChar(C_ecircunflex,"e^");
	TroffSpecialCharTab[12] = tr_new TroffSpecialChar(C_icircunflex,"i^");
	TroffSpecialCharTab[13] = tr_new TroffSpecialChar(C_ocircunflex,"o^");
	TroffSpecialCharTab[14] = tr_new TroffSpecialChar(C_ucircunflex,"u^");
	TroffSpecialCharTab[15] = tr_new TroffSpecialChar(C_ntilde,	"n~");
	TroffSpecialCharTab[16] = tr_new TroffSpecialChar(C_Ntilde,	"N~");
	TroffSpecialCharTab[17] = tr_new TroffSpecialChar(C_udieresis,"u:");
	TroffSpecialCharTab[18] = tr_new TroffSpecialChar(C_VERT,	"S.");
	TroffSpecialCharTab[19] = tr_new TroffSpecialChar(C_HORIZ,"S0");
	TroffSpecialCharTab[20] = tr_new TroffSpecialChar(C_LL,	"S1");
	TroffSpecialCharTab[21] = tr_new TroffSpecialChar(C_BOT,	"S2");
	TroffSpecialCharTab[22] = tr_new TroffSpecialChar(C_LR,	"S3");
	TroffSpecialCharTab[23] = tr_new TroffSpecialChar(C_RIGHT,"S6");
	TroffSpecialCharTab[24] = tr_new TroffSpecialChar(C_CEN,	"S5");
	TroffSpecialCharTab[25] = tr_new TroffSpecialChar(C_LEFT,	"S4");
	TroffSpecialCharTab[26] = tr_new TroffSpecialChar(C_UL,	"S7");
	TroffSpecialCharTab[27] = tr_new TroffSpecialChar(C_TOP,	"S8");
	TroffSpecialCharTab[28] = tr_new TroffSpecialChar(C_UR,	"S9");
	TroffSpecialCharTab[29] = tr_new TroffSpecialChar(C_D_VERT,	"D.");
	TroffSpecialCharTab[30] = tr_new TroffSpecialChar(C_D_HORIZ,	"D0");
	TroffSpecialCharTab[31] = tr_new TroffSpecialChar(C_D_LL,		"D1");
	TroffSpecialCharTab[32] = tr_new TroffSpecialChar(C_D_BOT,	"D2");
	TroffSpecialCharTab[33] = tr_new TroffSpecialChar(C_D_LR,		"D3");
	TroffSpecialCharTab[34] = tr_new TroffSpecialChar(C_D_RIGHT,	"D4");
	TroffSpecialCharTab[35] = tr_new TroffSpecialChar(C_D_CEN,	"D5");
	TroffSpecialCharTab[36] = tr_new TroffSpecialChar(C_D_LEFT,	"D6");
	TroffSpecialCharTab[37] = tr_new TroffSpecialChar(C_D_UL,		"D7");
	TroffSpecialCharTab[38] = tr_new TroffSpecialChar(C_D_TOP,	"D8");
	TroffSpecialCharTab[39] = tr_new TroffSpecialChar(C_D_UR,		"D9");
	TroffSpecialCharTab[40] = tr_new TroffSpecialChar(C_DS_BOT,	"C2");
	TroffSpecialCharTab[41] = tr_new TroffSpecialChar(C_DS_RIGHT,"C6");
	TroffSpecialCharTab[42] = tr_new TroffSpecialChar(C_DS_LEFT,	"C4");
	TroffSpecialCharTab[43] = tr_new TroffSpecialChar(C_DS_TOP,	"C8");
	TroffSpecialCharTab[44] = tr_new TroffSpecialChar(C_exclamdown,	"I!");
	TroffSpecialCharTab[45] = tr_new TroffSpecialChar(C_questdown,	"I?");
	TroffSpecialCharTab[46] = tr_new TroffSpecialChar(C_UARROW,	"ua");
	TroffSpecialCharTab[47] = tr_new TroffSpecialChar(C_DARROW,	"da");
	TroffSpecialCharTab[48] = tr_new TroffSpecialChar(C_LARROW,	"<-");
	TroffSpecialCharTab[49] = tr_new TroffSpecialChar(C_RARROW,	"->");
	// Remember to change TABLE_SIZE when adding tr_new objects
}

void TroffSpecialChar::DestroyTable()
{
	for (register Int i=0; i < TABLE_SIZE; i++) 
		tr_delete(TroffSpecialCharTab[i]);
}

Int TroffSpecialChar::SearchString(const String str, UnChar &ch)
{
	for (register Int i=0; i < TABLE_SIZE; i++) {
		if (str == TroffSpecialChar::TroffSpecialCharTab[i]->Troff) {
			ch = TroffSpecialCharTab[i]->IdeaChar;
			return OK;
		}
	}
	return ERROR;
}

Int TroffSpecialChar::SearchChar(const UnChar ch, String &str)
{
	for (register Int i=0; i < TABLE_SIZE; i++) {
		if (ch == TroffSpecialChar::TroffSpecialCharTab[i]->IdeaChar) {
			str = TroffSpecialCharTab[i]->Troff;
			return OK;
		}
	}
	return ERROR;
}

