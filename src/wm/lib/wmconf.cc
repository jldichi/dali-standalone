/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: wmconf.cc,v 1.1.1.1 1996/04/16 18:50:22 gustavof Exp $
*
* DESCRIPTION
* Definicion de parametros de configuracion.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>

#include <ifound/msgtable.h>
#include <ifound/str.h>

#include <wm.h>


/*
 * Tabla de definicion de nombres de funciones, teclas asociadas,
 * y sus descripciones.
 * Los nombres de las teclas de funcion estan en primer termino 
 * en el lenguaje configurado, y en segundo lugar en lenguage 
 * internacional (ingles).
 */
#ifdef HAVE_TTY

KEY_TAB key_tab[] = {

	/* code			out def			in def		key name */

	/* COMMANDS	*/
	{ 1,			0,				"",			"" },

	{ K_PROCESS,	'\n',			"^W",		"Ctrl-W" },
	{ K_END,		0,				"^E",		"Ctrl-E" },
	{ K_REMOVE,		0,				"^R",		"Ctrl-R" },
	{ K_IGNORE,		0,				"^G",		"Ctrl-G" },

	/* PAGINATION AND CURSOR MOVEMENT */
	{ 1,			0,				"",			"" },

	{ K_ENTER,		'\n',			"^M",		"ENTER" },
	{ K_CURS_UP,	0,				"^P",		"Ctrl-P" },
	{ K_CURS_LEFT,	'\b',			"^B",		"Ctrl-B" },
	{ K_CURS_DOWN,	'\n',			"^N",		"Ctrl-N" },
	{ K_CURS_RIGHT,	0,				"^F",		"Ctrl-F" },
	{ K_PAGE_UP,	0,				"^Z",		"Ctrl-Z" },
	{ K_PAGE_LEFT,	0,				"^]^B",		"Ctrl-] ^B" },
	{ K_PAGE_DOWN,	0,				"^V",		"Ctrl-V" },
	{ K_PAGE_RIGHT,	0,				"^]^F",		"Ctrl-] ^F" },

	/* EDITION */
	{ 1, 			0, 				"",			"" },

	{ K_BACKSPACE,	'\b',			"^H",		"Ctrl-H" },
	{ K_INS,		0,				"^O",		"Ctrl-O" },
	{ K_DEL,		0,				"^D",		"Ctrl-D" },
	{ K_CLEAR_FLD,	0,				"\177",		"DEL" },
	{ K_TAB,		0,				"^I",		"Ctrl-I" },
	{ K_UNDO,		0,				"^U",		"Ctrl-U" },

	/* HELP */
	{ 1, 			0,				"",			"" },

	{ K_APL_HELP,	0,				"^Y",		"Ctrl-Y" },
	{ K_HELP,		0,				"^J",		"Ctrl-J" },

	/* WINDOW MANAGER */
	{ 1, 			0,				"", 		"" },

	{ K_REDRAW,		0,				"^L",		"Ctrl-L" },
	{ K_ATTENTION,	0,				"^T",		"Ctrl-T" },
	{ K_KEYCONF,	0,				"^K",		"Ctrl-K" },
	{ K_INTERRUPT,	0,				"^|",		"Ctrl-|" },
	{ K_SUSPEND,	0,				"^C",		"Ctrl-C" },

	/* MISCELANEOUS */
	{ 1,			0,				"",			"" },

	{ K_META,		0,				"^A",		"Ctrl-A" },
	{ K_CTRLX,		0,				"^X",		"Ctrl-X" },
	{ K_AF1,		0,				"^]1",		"Ctrl-] 1" },
	{ K_AF2,		0,				"^]2",		"Ctrl-] 2" },
	{ K_AF3,		0,				"^]3",		"Ctrl-] 3" },
	{ K_AF4,		0,				"^]4",		"Ctrl-] 4" },

	/* SPANISH CHARACTERS */
	{ 1,			0,				"",			"" },

	{ C_aacute,		A_REVERSE|'a',	NULL,		"" },
	{ C_eacute,		A_REVERSE|'e',	NULL,		"" },
	{ C_iacute,		A_REVERSE|'i',	NULL,		"" },
	{ C_oacute,		A_REVERSE|'o',	NULL,		"" },
	{ C_uacute,		A_REVERSE|'u',	NULL,		"" },
	{ C_Aacute,		A_REVERSE|'A',	NULL,		"" },
	{ C_Eacute,		A_REVERSE|'E',	NULL,		"" },
	{ C_Iacute,		A_REVERSE|'I',	NULL,		"" },
	{ C_Oacute,		A_REVERSE|'O',	NULL,		"" },
	{ C_Uacute,		A_REVERSE|'U',	NULL,		"" },
	{ C_udieresis,	A_REVERSE|'u',	NULL,		"" },
	{ C_Udieresis,	A_REVERSE|'U',	NULL,		"" },
	{ C_Ntilde,		A_REVERSE|'N',	NULL,		"" },
	{ C_ntilde,		A_REVERSE|'n',	NULL,		"" },
	{ C_questdown,	A_REVERSE|'?',	NULL,		"" },
	{ C_exclamdown,	A_REVERSE|'!',	NULL,		"" },

	/* OTHER LINGUISTIC CHARACTERS */
	{ 1,			0,				"",			"" },

	{ C_agrave,		A_REVERSE|'a',	NULL,		"" },
	{ C_egrave,		A_REVERSE|'e',	NULL,		"" },
	{ C_igrave,		A_REVERSE|'i',	NULL,		"" },
	{ C_ograve,		A_REVERSE|'o',	NULL,		"" },
	{ C_ugrave,		A_REVERSE|'u',	NULL,		"" },
	{ C_Agrave,		A_REVERSE|'A',	NULL,		"" },
	{ C_Egrave,		A_REVERSE|'E',	NULL,		"" },
	{ C_Igrave,		A_REVERSE|'I',	NULL,		"" },
	{ C_Ograve,		A_REVERSE|'O',	NULL,		"" },
	{ C_Ugrave,		A_REVERSE|'U',	NULL,		"" },
	{ C_acircunflex,A_REVERSE|'a',	NULL,		"" },
	{ C_ecircunflex,A_REVERSE|'e',	NULL,		"" },
	{ C_icircunflex,A_REVERSE|'i',	NULL,		"" },
	{ C_ocircunflex,A_REVERSE|'o',	NULL,		"" },
	{ C_ucircunflex,A_REVERSE|'u',	NULL,		"" },
	{ C_Acircunflex,A_REVERSE|'A',	NULL,		"" },
	{ C_Ecircunflex,A_REVERSE|'E',	NULL,		"" },
	{ C_Icircunflex,A_REVERSE|'I',	NULL,		"" },
	{ C_Ocircunflex,A_REVERSE|'O',	NULL,		"" },
	{ C_Ucircunflex,A_REVERSE|'U',	NULL,		"" },
	{ C_adieresis,	A_REVERSE|'a',	NULL,		"" },
	{ C_edieresis,	A_REVERSE|'e',	NULL,		"" },
	{ C_idieresis,	A_REVERSE|'i',	NULL,		"" },
	{ C_odieresis,	A_REVERSE|'o',	NULL,		"" },
	{ C_ydieresis,	A_REVERSE|'y',	NULL,		"" },
	{ C_Adieresis,	A_REVERSE|'A',	NULL,		"" },
	{ C_Edieresis,	A_REVERSE|'E',	NULL,		"" },
	{ C_Idieresis,	A_REVERSE|'I',	NULL,		"" },
	{ C_Odieresis,	A_REVERSE|'O',	NULL,		"" },
	{ C_Ydieresis,	A_REVERSE|'Y',	NULL,		"" },
	{ C_atilde,		A_REVERSE|'a',	NULL,		"" },
	{ C_otilde,		A_REVERSE|'o',	NULL,		"" },
	{ C_Atilde,		A_REVERSE|'A',	NULL,		"" },
	{ C_Otilde,		A_REVERSE|'O',	NULL,		"" },
	{ C_aring,		'a',			NULL,		"" },
	{ C_Aring,		'A',			NULL,		"" },
	{ C_AE,			'A',			NULL,		"" },
	{ C_ae,			'a',			NULL,		"" },
	{ C_OE,			'O',			NULL,		"" },
	{ C_oe,			'o',			NULL,		"" },
	{ C_ccedilla,	'c',			NULL,		"" },
	{ C_Ccedilla,	'C',			NULL,		"" },

	/* SPECIAL CHARACTERS */
	{ 1,			0,				"",			"" },

	{ C_Oslash,		'0',			NULL,		"" },
	{ C_oslash,		'o',			NULL,		"" },
	{ C_AUSTRAL,	'A',			NULL,		"" },
	{ C_Cents,		'c',			NULL,		"" },
	{ C_germandbls,	'B',			NULL,		"" },

	/* GRAPHIC CHARACTERS */
	{ 1,			0,				"",			"" },

	{ C_VERT,		'|',			NULL,		"" },
	{ C_HORIZ,		'-',			NULL,		"" },
	{ C_LL,			'+',			NULL,		"" },
	{ C_BOT,		'+',			NULL,		"" },
	{ C_LR,			'+',			NULL,		"" },
	{ C_LEFT,		'+',			NULL,		"" },
	{ C_CEN,		'+',			NULL,		"" },
	{ C_RIGHT,		'+',			NULL,		"" },
	{ C_UL,			'+',			NULL,		"" },
	{ C_TOP,		'+',			NULL,		"" },
	{ C_UR,			'+',			NULL,		"" },
	{ C_BULLET,		'o',			NULL,		"" },
	{ C_UARROW,		'^',			NULL,		"" },
	{ C_DARROW,		'v',			NULL,		"" },
	{ C_D_VERT,		'|',			NULL,		"" },
	{ C_D_HORIZ,	'=',			NULL,		"" },
	{ C_D_LL,		'#',			NULL,		"" },
	{ C_D_BOT,		'#',			NULL,		"" },
	{ C_D_LR,		'#',			NULL,		"" },
	{ C_D_LEFT,		'#',			NULL,		"" },
	{ C_D_CEN,		'#',			NULL,		"" },
	{ C_D_RIGHT,	'#',			NULL,		"" },
	{ C_D_UL,		'#',			NULL,		"" },
	{ C_D_TOP,		'#',			NULL,		"" },
	{ C_D_UR,		'#',			NULL,		"" },
	{ C_DS_RIGHT,	'#',			NULL,		"" },
	{ C_DS_LEFT,	'#',			NULL,		"" },
	{ C_DS_BOT,		'#',			NULL,		"" },
	{ C_DS_TOP,		'#',			NULL,		"" },
	{ C_BLOCK,		A_REVERSE|' ',	NULL,		"" },
	{ C_LARROW,		'<',			NULL,		"" },
	{ C_RARROW,		'>',			NULL,		"" },
	{ C_SHADOW,		A_REVERSE|'#',	NULL,		"" },

	{ 0, 			0,				NULL,		"" },
};

#else /* HAVE_TTY */

KEY_TAB  _far key_tab[] = {

	/* code			out def			in def		key name */

	/* COMMANDS	*/
	{ 1,				0,			"",				"COMANDOS" },

	{ K_PROCESS,		'\n',	 	"\\d0\\d63",	"F5" },
	{ K_END,	 	    0,			"\\d0\\d79",	"Fin" },
	{ K_REMOVE,	 		0,			"\\d0\\d64",	"F6" },
	{ K_IGNORE,  	 	0,			"\\d0\\d66",	"F8" },

	{ 1, 				0, 			"", 			"PAGINACION Y MOVIMIENTO CURSOR" },

	{ K_ENTER, 			'\n',		"\r",			"Enter" },
	{ K_CURS_UP, 	 	0,			"\\d0\\d72",	"\254" },
	{ K_CURS_LEFT, 		'\b',		"\\d0\\d75",	"\256" },
	{ K_CURS_DOWN, 		'\n',		"\\d0\\d80",	"\255" },
	{ K_CURS_RIGHT,  	0,			"\\d0\\d77",	"\257" },
	{ K_PAGE_UP, 		0,			"\\d0\\d73",	"Re P\341g" },
	{ K_PAGE_LEFT, 		0,			"\\d0\\d67",	"F9" },
	{ K_PAGE_DOWN, 		0,			"\\d0\\d81",	"Av P\341g" },
	{ K_PAGE_RIGHT, 	0,			"\\d0\\d68",	"F10" },

	{ 1, 				0, 			"", 			"EDICION" },

	{ K_BACKSPACE,		'\b',		"\b",			"<--" },
	{ K_INS,		 	0,			"\\d0\\d82",	"Insert" },
	{ K_DEL,		 	0,			"\\d0\\d83",	"Supr" },
	{ K_CLEAR_FLD, 		0,			"\\d0\\d89",	"Shift+F6" },
	{ K_TAB, 		 	'\t',		"\t",			"-->|" },
	{ K_UNDO, 			0,			"\\d0\\d65",	"F7" },

	{ 1, 				0, 			"",				"AYUDA" },

	{ K_APL_HELP, 		0,			"\\d0\\d60",	"F2" },
	{ K_HELP,		 	0,			"\\d0\\d59",	"F1" },

	{ 1,				0,			"",				"WINDOW MANAGER" },

	{ K_REDRAW,			0,			"\\d0\\d92",	"Shift+F9" },
	{ K_ATTENTION,		0,			"\\d0\\d61",	"F3" },
	{ K_KEYCONF,		0,			"\\d0\\d93",	"Shift+F10" },
	{ K_INTERRUPT,		0,			"^|",			"Ctrl-|" },
	{ K_SUSPEND,		0,			"\\d0\\d62",	"F4" },

	{ 1, 				0, 			"", 			"MISCELANEOUS" },

	{ K_META,			0,			"\\d0\\d71",	"Inicio" },
	{ K_CTRLX,			0,			"^X",			"Ctrl-X" },
	{ K_AF1,			0,			"\\d0\\d84",	"Shift+F1" },
	{ K_AF2,			0,			"\\d0\\d85",	"Shift+F2" },
	{ K_AF3,			0,			"\\d0\\d86",	"Shift+F3" },
	{ K_AF4,			0,			"\\d0\\d87",	"Shift+F4" },

	{ 1, 				0, 			"", 			"CARACTERES ESPA\321OLES" },

	{ C_aacute,			160,		"\\d160",		"" },
	{ C_eacute,			130,		"\\d130",		"" },
	{ C_iacute,			161,		"\\d161",		"" },
	{ C_oacute,			162,		"\\d162",		"" },
	{ C_uacute,			163,		"\\d163",		"" },
	{ C_Aacute,			'A',		NULL,			"" },
	{ C_Eacute,			144,		"\\d144",		"" },
	{ C_Iacute,			'I',		NULL,			"" },
	{ C_Oacute,			'O',		NULL,			"" },
	{ C_Uacute,			'U',		NULL,			"" },
	{ C_udieresis,		129,		"\\d129",		"" },
	{ C_Udieresis,		154,		"\\d154",		"" },
	{ C_Ntilde,			165,		"\\d165",		"" },
	{ C_ntilde,			164,		"\\d164",		"" },
	{ C_questdown,		168,		"\\d168",		"" },
	{ C_exclamdown,		173,		"\\d173",		"" },

	{ 1, 				0, 			"",				"OTROS CARACTERES LING\334ISTICOS" },

	{ C_agrave,			133,		"\\d133",		"" },
	{ C_egrave,			138,		"\\d138",		"" },
	{ C_igrave,			141,		"\\d141",		"" },
	{ C_ograve,			149,		"\\d149",		"" },
	{ C_ugrave,			151,		"\\d151",		"" },
	{ C_Agrave,			'A',		NULL,			"" },
	{ C_Egrave,			'E',		NULL,			"" },
	{ C_Igrave,			'I',		NULL,			"" },
	{ C_Ograve,			'O',		NULL,			"" },
	{ C_Ugrave,			'U',		NULL,			"" },
	{ C_acircunflex,	131,		"\\d131",		"" },
	{ C_ecircunflex,	136,		"\\d136",		"" },
	{ C_icircunflex,	140,		"\\d140",		"" },
	{ C_ocircunflex,	147,		"\\d147",		"" },
	{ C_ucircunflex,	150,		"\\d150",		"" },
	{ C_Acircunflex,	'A',		NULL,			"" },
	{ C_Ecircunflex,	'E',		NULL,			"" },
	{ C_Icircunflex,	'I',		NULL,			"" },
	{ C_Ocircunflex,	'O',		NULL,			"" },
	{ C_Ucircunflex,	'U',		NULL,			"" },
	{ C_adieresis,		132,		"\\d132",		"" },
	{ C_edieresis,		137,		"\\d137",		"" },
	{ C_idieresis,		139,		"\\d139",		"" },
	{ C_odieresis,		148,		"\\d148",		"" },
	{ C_ydieresis,		152,		"\\d152",		"" },
	{ C_Adieresis,		142,		"\\d142",		"" },
	{ C_Edieresis,		'E',		NULL,			"" },
	{ C_Idieresis,		'I',		NULL,			"" },
	{ C_Odieresis,		153,		"\\d153",		"" },
	{ C_Ydieresis,		'Y',		NULL,			"" },
	{ C_atilde,			166,		"\\d166",		"" },
	{ C_otilde,			167,		"\\d167",		"" },
	{ C_Atilde,			'A',		NULL,			"" },
	{ C_Otilde,			'O',		NULL,			"" },
	{ C_aring,			134, 		"\\d134",		"" },
	{ C_Aring,			143,		"\\d143",		"" },
	{ C_AE,				146,		"\\d146",		"" },
	{ C_ae,				145,		"\\d145",		"" },
	{ C_OE,				'O',		NULL,			"" },
	{ C_oe,				'o',		NULL,			"" },
	{ C_ccedilla,		135,		"\\d135",		"" },
	{ C_Ccedilla,		128,		NULL,			"" },

	{ 1,				0, 			"", 	"CARACTERES ESPECIALES" },

	{ C_Oslash,			'0',		NULL,			"" },
	{ C_oslash, 		'o',		NULL,			"" },
	{ C_AUSTRAL, 		'A',		NULL,			"" },
	{ C_Cents,			155,		"\\d155",		"" },
	{ C_germandbls,		'B',		NULL,			"" },

	{ 1, 				0, 			"", 			"CARACTERES GRAFICOS" },

	{ C_VERT,			179,		"\\d179",		"" },
	{ C_HORIZ,			196,		"\\d196",		"" },
	{ C_LL,				192,		"\\d192",		"" },
	{ C_BOT,			193,		"\\d193",		"" },
	{ C_LR,				217,		"\\d217",		"" },
	{ C_LEFT,			195,		"\\d195",		"" },
	{ C_CEN,			197,		"\\d197",		"" },
	{ C_RIGHT,			180,		"\\d180",		"" },
	{ C_UL,				218,		"\\d218",		"" },
	{ C_TOP,			194,		"\\d194",		"" },
	{ C_UR,				191,		"\\d191",		"" },
	{ C_BULLET,			250,		"\\d250",		"" },
	{ C_UARROW,			'',		NULL,			"" },
	{ C_DARROW,			'',		NULL,			"" },
	{ C_D_VERT,			186,		"\\d186",		"" },
	{ C_D_HORIZ,		205,		"\\d205",		"" },
	{ C_D_LL,			200,		"\\d200",		"" },
	{ C_D_BOT,			202,		"\\d202",		"" },
	{ C_D_LR,			188,		"\\d188",		"" },
	{ C_D_LEFT,			204,		"\\d204",		"" },
	{ C_D_CEN,			206,		"\\d206",		"" },
	{ C_D_RIGHT,		185,		"\\d185",		"" },
	{ C_D_UL,			201,		"\\d201",		"" },
	{ C_D_TOP,			203,		"\\d203",		"" },
	{ C_D_UR,			187,		"\\d187",		"" },
	{ C_DS_RIGHT,		182,		"\\d182",		"" },
	{ C_DS_LEFT,		199,		"\\d199",		"" },
	{ C_DS_BOT,			207,		"\\d207",		"" },
	{ C_DS_TOP,			209,		"\\d209",		"" },
	{ C_BLOCK,			219,		"'b",			"" },
	{ C_SHADOW,			176, 		"'#",			"" },
	{ C_LARROW,			'',		NULL,			"" },
	{ C_RARROW,			26,			NULL,			"" },

	{ 0, 				0, 			NULL,			"" },
};

#endif	/* HAVE_TTY */

/*
 * Tabla de definicion de nombres de atributos
 * La primera columna esta en idioma configurado, y la segunda
 * en ingles.
 */

ATTR_TAB  _far attr_tab[] = {
	{ "",		  "NORMAL",		 A_NORMAL },
	{ "",			"BOLD",		   A_BOLD },
	{ "",		"STANDOUT",    A_STANDOUT },
	{ "",				"",				0 },
	{ "",	   "UNDERLINE",	  A_UNDERLINE },
	{ "",		   "BLINK",	   	  A_BLINK },
	{ "",	   "ALTERNATE",	  A_ALTERNATE },
	{ "",		 "REVERSE",		A_REVERSE },
	{ "",				"",				0 },
	{ "",				"",				0 },
	{ "",				"",				0 },
	{ "",				"",				0 },
	{ "",				"",				0 },
	{ "",				"",				0 },
	{ "",				"",				0 },
	{ "",				"",				0 },
	{ "",				"",				0 },
	{ "",				"",				0 },
	{ "",				"",				0 },
	{ "",				"",				0 },
	{ "",				"",				0 },
	{ "",				"",				0 },
	{ "",				"",				0 },
	{ "",				"",				0 },
	{ "",				"",				0 },
	{ "",				"",				0 },
	{ "",				"",				0 },
	{ "",				"",				0 },
	{ "",				"",				0 },
	{ "",				"",				0 },
	{ "",		   "BLACK",		  A_BLACK },
	{ "",			 "RED",			A_RED },
	{ "",		   "GREEN",		  A_GREEN },
	{ "",		  "YELLOW",		 A_YELLOW },
	{ "",			"BLUE",		   A_BLUE },
	{ "",		 "MAGENTA",		A_MAGENTA },
	{ "",			"CYAN",		   A_CYAN },
	{ "",		   "WHITE",		  A_WHITE },
	{ "",				"",				0 },
	{ "",				"",				0 },
	{ "",		"BLACK_BG",	   A_BLACK_BG },
	{ "",		  "RED_BG",		 A_RED_BG },
	{ "",		"GREEN_BG",	   A_GREEN_BG },
	{ "",	   "YELLOW_BG",	  A_YELLOW_BG },
	{ "",		 "BLUE_BG",		A_BLUE_BG },
	{ "",	  "MAGENTA_BG",	 A_MAGENTA_BG },
	{ "",		 "CYAN_BG",		A_CYAN_BG },
	{ "",	    "WHITE_BG",	   A_WHITE_BG },
	{ "",			  NULL,				0 },
};

#define POOL_LEN	10
static String msgTab[POOL_LEN];
static Int offset = 0;

char *_WmErrmsg(const String &msg)
{
 	String &aux = msgTab[offset] = appMsg(msg);
	offset = (offset+1)%POOL_LEN;
	return (char*)toCharPtr(aux);
}
