/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: stchset.cc,v 1.2 2004/09/29 21:34:47 damianm Exp $
*
* DESCRIPTION
* These are the names of each character over 127.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
* _CharName     | Return the code of a character given it's name
* _FindCharCode | Return the name of a char given it's code.
*********************************************************************/

#include <string.h>

#include <ideafix.h>
#include <cfix.h>
#include <ideafix/priv/gndefs.h>
#include <ifound/priv/gfuncs.h>
#ifdef IDEAWIN
#	include <mswi/ideadll.h>
#endif

/* ++ Prototypes ++ */

static void _ReadMesgs(void);

/* -- Prototypes -- */

static struct CHSET_NAMES {
	char *lang_name;
	char *std_name;
#ifdef IDEAWIN
} ChsetNames[128 + 1 + 16] = {
#else
} ChsetNames[128 + 1] = {
#endif

/*********** FUNCTION KEY CODES ************/
	{ NULL, "PROCESS" },	/* K_PROCESS */

	{ NULL, "END" }, 		/* K_END	 */

	{ NULL, "REMOVE" }, 	/* K_REMOVE, */

	{ NULL, "IGNORE" }, 	/* K_IGNORE  */

	{ NULL, "ENTER" },	 	/* K_ENTER,  */

	{ NULL, "CURS_UP" }, 	/* K_CURS_UP, */

	{ NULL, "CURS_LEFT" }, 	/* K_CURS_LEFT, */

	{ NULL, "CURS_DOWN" }, 	/* K_CURS_DOWN, */

	{ NULL, "CURS_RIGHT" }, /* K_CURS_RIGHT, */

	{ NULL, "PAGE_UP" }, 	/* K_PAGE_UP,    */

	{ NULL, "PAGE_LEFT" },	/* K_PAGE_LEFT,  */

	{ NULL, "PAGE_DOWN" }, 	/* K_PAGE_DOWN,  */

	{ NULL, "PAGE_RIGHT" }, /* K_PAGE_RIGHT, */

	{ NULL, "BACKSPACE" }, 	/* K_BACKSPACE,	 */

	{ NULL, "UNDO" },		/* K_UNDO		*/

	{ NULL, "INSERT" },		/* K_INS,	     */

	{ NULL, "DELETE" },		/* K_DEL,		 */

	{ NULL, "CLEAR_FLD" },	/* K_CLEAR_FLD,  */

	{ NULL, "TAB" }, 		/* K_TAB,		*/

	{ NULL, "APL_HELP" },	/* K_APL_HELP,  */

	{ NULL, "HELP" },		/* K_HELP,      */

	{ NULL, "REDRAW" },		/* K_REDRAW,	*/

	{ NULL, "SERVICES" },	/* K_ATTENTION, */

	{ NULL, "KEYCONF" },	/* K_KEYCONF,  */

	{ NULL, "INTERRUPT" },	/* K_INTERRUPT, */

	{ NULL, "SUSPEND" },	/* K_SUSPEND,	*/

	{ NULL, "META" },		/* K_META,	    */

	{ NULL, "CTRLX" }, 		/* K_CTRLX,   */
#ifdef IDEAWIN
	{ NULL, "FIRST_REC"},	/* K_AF1,      */
	{ NULL, "LAST_REC"}, 	/* K_AF2,	*/
#else
	{ NULL, "AF1"}, 		/* K_AF3,   */
	{ NULL, "AF2"}, 		/* K_AF4,   */
#endif

	{ NULL, "AF3" }, 		/* K_AF3,   */

	{ NULL, "AF4" }, 		/* K_AF4,   */

/********  GRAPHIC CHARACTERS ***********/

	{ NULL, "VERT" },

	{ NULL, "HORIZ" },

	{ NULL, "LL" },

	{ NULL, "BOT" },

	{ NULL, "LR" },

	{ NULL, "RIGHT" },

	{ NULL, "CEN" },

	{ NULL, "LEFT" },

	{ NULL, "UL" },

	{ NULL, "TOP" },

	{ NULL, "UR" },

/*********** OTHER GRAPHICS ***************/
	{ NULL, "BULLET" },

	{ NULL, "UARROW" },

	{ NULL, "DARROW" },

	{ NULL, "LARROW" },

	{ NULL, "RARROW" },

/************ DOUBLE LINES ***************/
	{ NULL, "D_VERT" },

	{ NULL, "D_HORIZ" },

	{ NULL, "D_LL" },

	{ NULL, "D_BOT" },

	{ NULL, "D_LR" },

	{ NULL, "D_RIGHT" },

	{ NULL, "D_CEN" },

	{ NULL, "D_LEFT" },

	{ NULL, "D_UL" },

	{ NULL, "D_TOP" },

	{ NULL, "D_UR" },

/********** OTHER GRAPHIC CHARACTERS ***********/
	{ NULL, "BLOCK" },

	{ NULL, "DS_BOT" },

	{ NULL, "DS_RIGHT" },

	{ NULL, "DS_LEFT" },

	{ NULL, "DS_TOP" },

/******* LINGUISTIC CHARACTERS *****/
	{ NULL, "Agrave" },

	{ NULL, "Aacute" },

	{ NULL, "Acircunflex" },

	{ NULL, "Atilde" },

	{ NULL, "Adieresis" },

	{ NULL, "Aring" },

	{ NULL, "AE" },

	{ NULL, "Ccedilla" },

	{ NULL, "Egrave" },

	{ NULL, "Eacute" },

	{ NULL, "Ecircunflex" },

	{ NULL, "Edieresis" },

	{ NULL, "Igrave" },

	{ NULL, "Iacute" },

	{ NULL, "Icircunflex" },

	{ NULL, "Idieresis" },

	{ NULL, "exclamdown" },

	{ NULL, "Ntilde" },

	{ NULL, "Ograve" },

	{ NULL, "Oacute" },

	{ NULL, "Ocircunflex" },

	{ NULL, "Otilde" },

	{ NULL, "Odieresis" },

	{ NULL, "OE" },

	{ NULL, "Oslash" },

	{ NULL, "Ugrave" },

	{ NULL, "Uacute" },

	{ NULL, "Ucircunflex" },

	{ NULL, "Udieresis" },

	{ NULL, "Ydieresis" },

	{ NULL, "cents" },

	{ NULL, "germandbls" },

	{ NULL, "agrave" },

	{ NULL, "aacute" },

	{ NULL, "acircunflex" },

	{ NULL, "atilde" },

	{ NULL, "adieresis" },

	{ NULL, "aring" },

	{ NULL, "ae" },

	{ NULL, "ccedilla" },

	{ NULL, "egrave" },

	{ NULL, "eacute" },

	{ NULL, "ecircunflex" },

	{ NULL, "edieresis" },

	{ NULL, "igrave" },

	{ NULL, "iacute" },

	{ NULL, "icircunflex" },

	{ NULL, "idieresis" },

	{ NULL, "questdown" },

	{ NULL, "ntilde" },

	{ NULL, "ograve" },

	{ NULL, "oacute" },

	{ NULL, "ocircunflex" },

	{ NULL, "otilde" },

	{ NULL, "odieresis" },

	{ NULL, "oe" },

	{ NULL, "oslash" },

	{ NULL, "ugrave" },

	{ NULL, "uacute" },

	{ NULL, "ucircunflex" },

	{ NULL, "udieresis" },

	{ NULL, "ydieresis" },

	{ NULL, "AUSTRAL" },

	{ NULL, "SHADOW" },
#ifdef IDEAWIN
/* Ideafix For Windows additioanl commands */
	{ NULL, "" },

	{ NULL, "CUT" },

	{ NULL, "COPY" },

	{ NULL, "PASTE" },

	{ NULL, "CLEAR" },

	{ NULL, "TOGGLE_INS" },

	{ NULL, "WORD_LEFT" },

	{ NULL, "WORD_RIGHT" },

	{ NULL, "LEFT_END" },

	{ NULL, "RIGHT_END" },

	{ NULL, "SEL_LEFT" },

	{ NULL, "SEL_RIGHT" },

	{ NULL, "SEL_WORD_LEFT" },

	{ NULL, "SEL_WORD_RIGHT" },

	{ NULL, "SEL_LEFT_END" },

	{ NULL, "SEL_RIGHT_END" },
#endif
	{ 0, 0 },
/**** END OF TABLE ****/

};

static bool status = false;
#define MAX_LINE		1024
#define MAX_PATH		1024

static void _ReadMesgs(void)
{
	status = true;
	
	FILE *fp = openDataDirFile(String("DATADIR").getEnv(),
							   String("LANGUAGE").getEnv(), "chset.txt");
	if (fp == NULL)
		return;

	struct CHSET_NAMES *p;
	char line[MAX_LINE];
	for (p = ChsetNames; p->std_name; p++) {

		if (fgets(line, MAX_LINE, fp) == NULL)
			break;

		// line has an extra newline, so we do not copy it
		p->lang_name = (char *) Alloc(strlen(line));
		strncpy(p->lang_name, line, strlen(line) - 1);
#ifdef IDEAWIN
		p->lang_name[strlen(line)-1] = '\0';
		// If I'm in dll I use malloc that doesn't fill with zeros.
#endif
		// no need to put final zero since Alloc puts zeros everywhere
	}
	fclose(fp);
}

/*
 * Return character code of 'name'. If it isn't a name of
 * an IDEAFIX character, return ERROR.
 */
int _FindCharCode(char *name)
{
	register struct CHSET_NAMES *pn = ChsetNames;

	// Fill the Chset Table
	if (!status)
		_ReadMesgs();

	while (pn->std_name) {
		if ((pn->lang_name && str_eq(name, pn->lang_name)) ||
				str_eq(name, pn->std_name))
			/* Add 128 */
#ifdef IDEAWIN
			return (pn - ChsetNames) + 0x80;  /* Add 128 */
#else
			return (pn - ChsetNames) | 0x80;  /* Add 128 */
#endif
		pn++;
	}
	return ERR;
}

/*
 * Return a name according to character code. First try with the
 * configured name, then with the standard.
 */
char *  _CharName(unsigned n)
{
	static char buff[3] = { '\0' };
	char *ret;
	bool stdname = bool(n & 0x100);

	strcpy(buff, "^?");
	ret = buff;

	if (!status)
		/* Fill the Chset Table */
		_ReadMesgs();

#ifdef IDEAWIN
#define LASTCOD 	(UShort) 0x0110
	if (n > LASTCOD)
		n &= 0xFF;
	else
		stdname = false;
#else
	n &= 0xFF;
#endif

	if (n < ' ')
		ret[1] = (char) n + '@';
	else if (n < 127) {
		ret[0] = (char) n; ret[1] = 0;
	} else if (n > 127) {
		n -= 128;
		ret = (ChsetNames[n].lang_name && *ChsetNames[n].lang_name &&
				!stdname) ? ChsetNames[n].lang_name : ChsetNames[n].std_name;
	}

	return ret;
}
