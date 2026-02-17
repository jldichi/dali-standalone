/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: ifxctype.cc,v 1.6 2004/09/29 15:22:38 damianm Exp $
*
* DESCRIPTION
* ctype character array
*********************************************************************/

#include <strings.h>

#include <ifound.h>
#include <ifound/stdc.h>
#include <ifound/str.h>
#include <ifound/chset.h>

#ifdef IDEAWIN
#	include <mswi/winidea.h>
#endif

extern "C" {

DLLDEPEND char _ifx_ctype[] = {
 	/*  -1 EOF*/	(  0 ),
 	/*   0 ^@ */	( _C ),
	/*   1 ^A */	( _C ),
	/*   2 ^B */	( _C ),
	/*   3 ^C */	( _C ),
	/*   4 ^D */	( _C ),
	/*   5 ^E */	( _C ),
	/*   6 ^F */	( _C ),
	/*   7 ^G */	( _C ),
	/*   8 ^H */	( _C ),
	/*   9 ^I */	( _C | _S ),
	/*  10 ^J */	( _C | _S ),
	/*  11 ^K */	( _C | _S ),
	/*  12 ^L */	( _C | _S ),
	/*  13 ^M */	( _C | _S ),
	/*  14 ^N */	( _C ),
	/*  15 ^O */	( _C ),
	/*  16 ^P */	( _C ),
	/*  17 ^Q */	( _C ),
	/*  18 ^R */	( _C ),
	/*  19 ^S */	( _C ),
	/*  20 ^T */	( _C ),
	/*  21 ^U */	( _C ),
	/*  22 ^V */	( _C ),
	/*  23 ^W */	( _C ),
	/*  24 ^X */	( _C ),
	/*  25 ^Y */	( _C ),
	/*  26 ^Z */	( _C ),
	/*  27 ^[ */	( _C ),
	/*  28 ^\ */	( _C ),
	/*  29 ^] */	( _C ),
	/*  30 ^^ */	( _C ),
	/*  31 ^_ */	( _C ),

	/*  32 ' '*/	( _B  | _S ),
	/*  33  ! */	( _P ),
	/*  34  " */	( _P ),
	/*  35  # */	( _P ),
	/*  36  $ */	( _P ),
	/*  37  % */	( _P ),
	/*  38  & */	( _P ),
	/*  39  ' */	( _P ),
	/*  40  ( */	( _P ),
	/*  41  ) */	( _P ),
	/*  42  * */	( _P ),
	/*  43  + */	( _P ),
	/*  44  , */	( _P ),
	/*  45  - */	( _P ),
	/*  46  . */	( _P ),
	/*  47  / */	( _P ),

	/*  48  0 */	( _N | _X),
	/*  49  1 */	( _N | _X),
	/*  50  2 */	( _N | _X),
	/*  51  3 */	( _N | _X),
	/*  52  4 */	( _N | _X),
	/*  53  5 */	( _N | _X),
	/*  54  6 */	( _N | _X),
	/*  55  7 */	( _N | _X),
	/*  56  8 */	( _N | _X),
	/*  57  9 */	( _N | _X),

	/*  58  : */	( _P ),
	/*  59  ; */	( _P ),
	/*  60  < */	( _P ),
	/*  61  = */	( _P ),
	/*  62  > */	( _P ),
	/*  63  ? */	( _P ),
	/*  64  @ */	( _P ),

	/*  65  A */	( _U | _X ),
	/*  66  B */	( _U | _X ),
	/*  67  C */	( _U | _X ),
	/*  68  D */	( _U | _X ),
	/*  69  E */	( _U | _X ),
	/*  70  F */	( _U | _X ),
	/*  71  G */	( _U ),
	/*  72  H */	( _U ),
	/*  73  I */	( _U ),
	/*  74  J */	( _U ),
	/*  75  K */	( _U ),
	/*  76  L */	( _U ),
	/*  77  M */	( _U ),
	/*  78  N */	( _U ),
	/*  79  O */	( _U ),
	/*  80  P */	( _U ),
	/*  81  Q */	( _U ),
	/*  82  R */	( _U ),
	/*  83  S */	( _U ),
	/*  84  T */	( _U ),
	/*  85  U */	( _U ),
	/*  86  V */	( _U ),
	/*  87  W */	( _U ),
	/*  88  X */	( _U ),
	/*  89  Y */	( _U ),
	/*  90  Z */	( _U ),

	/*  91  [ */	( _P ),
	/*  92  \ */	( _P ),
	/*  93  ] */	( _P ),
	/*  94  ^ */	( _P ),
	/*  95  _ */	( _P ),
	/*  96  ` */	( _P ),

	/*  97  a */	( _L | _X ),
	/*  98  b */	( _L | _X ),
	/*  99  c */	( _L | _X ),
	/* 100  d */	( _L | _X ),
	/* 101  e */	( _L | _X ),
	/* 102  f */	( _L | _X ),
	/* 103  g */	( _L ),
	/* 104  h */	( _L ),
	/* 105  i */	( _L ),
	/* 106  j */	( _L ),
	/* 107  k */	( _L ),
	/* 108  l */	( _L ),
	/* 109  m */	( _L ),
	/* 110  n */	( _L ),
	/* 111  o */	( _L ),
	/* 112  p */	( _L ),
	/* 113  q */	( _L ),
	/* 114  r */	( _L ),
	/* 115  s */	( _L ),
	/* 116  t */	( _L ),
	/* 117  u */	( _L ),
	/* 118  v */	( _L ),
	/* 119  w */	( _L ),
	/* 120  x */	( _L ),
	/* 121  y */	( _L ),
	/* 122  z */	( _L ),

	/* 123  { */	( _P ),
	/* 124  | */	( _P ),
	/* 125  } */	( _P ),
	/* 126  ~ */	( _P ),

	/* 127 DEL */	( _C ),
	/* 128 ~@ */	( _C ),
	/* 129 ~A */	( _C ),
	/* 130 ~B */	( _C ),
	/* 131 ~C */	( _C ),
	/* 132 ~D */	( _C ),
	/* 133 ~E */	( _C ),
	/* 134 ~F */	( _C ),
	/* 135 ~G */	( _C ),
	/* 136 ~H */	( _C ),
	/* 137 ~I */	( _C ),
	/* 138 ~J */	( _C ),
	/* 139 ~K */	( _C ),
	/* 140 ~L */	( _C ),
	/* 141 ~M */	( _C ),
	/* 142 ~N */	( _C ),
	/* 143 ~O */	( _C ),
	/* 144 ~P */	( _C ),
	/* 145 ~Q */	( _C ),
	/* 146 ~R */	( _C ),
	/* 147 ~S */	( _C ),
	/* 148 ~T */	( _C ),
	/* 149 ~U */	( _C ),
	/* 150 ~V */	( _C ),
	/* 151 ~W */	( _C ),
	/* 152 ~X */	( _C ),
	/* 153 ~Y */	( _C ),
	/* 154 ~Z */	( _C ),
	/* 155 ~[ */	( _C ),
	/* 156 ~\ */	( _C ),
	/* 157 ~] */	( _C ),
	/* 158 ~^ */	( _C ),
	/* 159 ~_ */	( _C ),

	/* 160    */	( _P ),
	/* 161    */	( _P ),
	/* 162    */	( _P ),
	/* 163    */	( _P ),
	/* 164    */	( _P ),
	/* 165    */	( _P ),
	/* 166    */	( _P ),
	/* 167    */	( _P ),
	/* 168    */	( _P ),
	/* 169    */	( _P ),
	/* 170    */	( _P ),
	/* 171    */	( _P ),
	/* 172    */	( _P ),
	/* 173    */	( _P ),
	/* 174    */	( _P ),
	/* 175    */	( _P ),
	/* 176    */	( _P ),
	/* 177    */	( _P ),
	/* 178    */	( _P ),
	/* 179    */	( _P ),
	/* 180    */	( _P ),
	/* 181    */	( _P ),
	/* 182    */	( _P ),
	/* 183    */	( _P ),
	/* 184    */	( _P ),
	/* 185    */	( _P ),
	/* 186    */	( _P ),
	/* 187    */	( _P ),
	/* 188    */	( _P ),
	/* 189    */	( _P ),
	/* 190    */	( _P ),
	/* 191    */	( _P ),

/* We urgently need in Russia that characters as letters! dnb */
#if !defined(NO_CONTROL_192_255) && defined(SPANISH)
	/* 192    */	( _U ),
	/* 193    */	( _U ),
	/* 194    */	( _U ),
	/* 195    */	( _U ),
	/* 196    */	( _U ),
	/* 197    */	( _U ),
	/* 198    */	( _U ),
	/* 199    */	( _U ),
	/* 200    */	( _U ),
	/* 201    */	( _U ),
	/* 202    */	( _U ),
	/* 203    */	( _U ),
	/* 204    */	( _U ),
	/* 205    */	( _U ),
	/* 206    */	( _U ),
	/* 207    */	( _U ),

	/* 208    */	( _P ),

	/* 209    */	( _U ),
	/* 210    */	( _U ),
	/* 211    */	( _U ),
	/* 212    */	( _U ),
	/* 213    */	( _U ),
	/* 214    */	( _U ),
	/* 215    */	( _U ),
	/* 216    */	( _U ),
	/* 217    */	( _U ),
	/* 218    */	( _U ),
	/* 219    */	( _U ),
	/* 220    */	( _U ),
	/* 221    */	( _U ),

	/* 222    */	( _P ),

	/* 223    */	( _U ),

	/* 224    */	( _L ),
	/* 225    */	( _L ),
	/* 226    */	( _L ),
	/* 227    */	( _L ),
	/* 228    */	( _L ),
	/* 229    */	( _L ),
	/* 230    */	( _L ),
	/* 231    */	( _L ),
	/* 232    */	( _L ),
	/* 233    */	( _L ),
	/* 234    */	( _L ),
	/* 235    */	( _L ),
	/* 236    */	( _L ),
	/* 237    */	( _L ),
	/* 238    */	( _L ),
	/* 239    */	( _L ),

	/* 240    */	( _P ),

	/* 241    */	( _L ),
	/* 242    */	( _L ),
	/* 243    */	( _L ),
	/* 244    */	( _L ),
	/* 245    */	( _L ),
	/* 246    */	( _L ),
	/* 247    */	( _L ),
	/* 248    */	( _L ),
	/* 249    */	( _L ),
	/* 250    */	( _L ),
	/* 251    */	( _L ),
	/* 252    */	( _L ),
	/* 253    */	( _L ),

	/* 254    */	( _P ),

	/* 255    */	( _C )

#else

	/* 192    */	( _U ),
	/* 193    */	( _U ),
	/* 194    */	( _U ),
	/* 195    */	( _U ),
	/* 196    */	( _U ),
	/* 197    */	( _U ),
	/* 198    */	( _U ),
	/* 199    */	( _U ),
	/* 200    */	( _U ),
	/* 201    */	( _U ),
	/* 202    */	( _U ),
	/* 203    */	( _U ),
	/* 204    */	( _U ),
	/* 205    */	( _U ),
	/* 206    */	( _U ),
	/* 207    */	( _U ),
	/* 208    */	( _U ),
	/* 209    */	( _U ),
	/* 210    */	( _U ),
	/* 211    */	( _U ),
	/* 212    */	( _U ),
	/* 213    */	( _U ),
	/* 214    */	( _U ),
	/* 215    */	( _U ),
	/* 216    */	( _U ),
	/* 217    */	( _U ),
	/* 218    */	( _U ),
	/* 219    */	( _U ),
	/* 220    */	( _U ),
	/* 221    */	( _U ),
	/* 222    */	( _U ),
	/* 223    */	( _U ),

	/* 224    */	( _L ),
	/* 225    */	( _L ),
	/* 226    */	( _L ),
	/* 227    */	( _L ),
	/* 228    */	( _L ),
	/* 229    */	( _L ),
	/* 230    */	( _L ),
	/* 231    */	( _L ),
	/* 232    */	( _L ),
	/* 233    */	( _L ),
	/* 234    */	( _L ),
	/* 235    */	( _L ),
	/* 236    */	( _L ),
	/* 237    */	( _L ),
	/* 238    */	( _L ),
	/* 239    */	( _L ),
	/* 240    */	( _L ),
	/* 241    */	( _L ),
	/* 242    */	( _L ),
	/* 243    */	( _L ),
	/* 244    */	( _L ),
	/* 245    */	( _L ),
	/* 246    */	( _L ),
	/* 247    */	( _L ),
	/* 248    */	( _L ),
	/* 249    */	( _L ),
	/* 250    */	( _L ),
	/* 251    */	( _L ),
	/* 252    */	( _L ),
	/* 253    */	( _L ),
	/* 254    */	( _L ),
	/* 255    */	( _L )
#endif
};

#if !IDEAWIN || IDEADLL
#define IDEAFIXDIR			"/usr/ideafix"
#define DEF_LANG			"english"
#define CODETABLE_FILE		"codetab.bin"
#define PATH_LENGTH			1024
#define NON_ASCII_TAB_SIZE	64
#define INV_NON_ASCII_TAB_SIZE	128

static int isTablesInitialized = 0;

// This is conversion table
// It contains last 64 chars of character table.
// Usually user can generate it by simple text editor defining
// characters from 192 to 255 in file $IDEAFIX/data/$LANGUAGE/codetab.bin
// Usually, it's national letters alphabetically sorted.

static UnChar *nonAsciiTab_ = NULL;

// This is inverted character table to convert from
// external character set to internal code of IdeaFIx.
// This table contains 128 characters - last half of codetable.

static UnChar *invNonAsciiTab_ = NULL;

// initialize direct conversion table
// if file with codetable is absent - make it according simple
// conversion like from 'A to A

static UnChar *initNonAsciiTab(void)
{
	int i;
#if IDEAWIN
	HFILE wf = NULL;
#else
	FILE *f = NULL;
#endif
	String path, ideafix, lang;
	char buf[NON_ASCII_TAB_SIZE];

	// do not make job again
	if (isTablesInitialized) return nonAsciiTab_;

	UnChar *tb = tr_new UnChar[NON_ASCII_TAB_SIZE];
	//UnChar *tb = new UnChar[NON_ASCII_TAB_SIZE];

	nonAsciiTab_ = tb;

	//fprintf(stderr,"nonAsciiTab_: %X tb: %X\n", nonAsciiTab_, tb);

	ideafix = String("IDEAFIX").getEnv();
	if (!ideafix)
		ideafix = IDEAFIXDIR;
	lang = String("LANGUAGE").getEnv();
	if (!lang)
		lang = DEF_LANG;

	path = ideafix + PATH_SLASH + "data" + PATH_SLASH + lang
				   + PATH_SLASH + CODETABLE_FILE;

	for (i = 0; i < NON_ASCII_TAB_SIZE; i++) {
		switch (i + 192) {
		case C_Agrave:
		case C_Aacute:
		case C_Acircunflex:
		case C_Atilde:
		case C_Adieresis:
		case C_Aring:
		case C_AE:
			tb[i] = 'A';
			break;
		case C_agrave:
		case C_aacute:
		case C_acircunflex:
		case C_atilde:
		case C_adieresis:
		case C_aring:
		case C_ae:
			tb[i] = 'a';
			break;
		case C_Ccedilla:
			tb[i] = 'C';
			break;
		case C_ccedilla:
			tb[i] = 'c';
			break;
		case C_Egrave:
		case C_Eacute:
		case C_Ecircunflex:
		case C_Edieresis:
			tb[i] = 'E';
			break;
		case C_egrave:
		case C_eacute:
		case C_ecircunflex:
		case C_edieresis:
			tb[i] = 'e';
			break;
		case C_Igrave:
		case C_Iacute:
		case C_Icircunflex:
		case C_Idieresis:
			tb[i] = 'I';
			break;
		case C_igrave:
		case C_iacute:
		case C_icircunflex:
		case C_idieresis:
			tb[i] = 'i';
			break;
		case C_Ograve:
		case C_Oacute:
		case C_Ocircunflex:
		case C_Otilde:
		case C_Odieresis:
		case C_OE:
		case C_Oslash:
			tb[i] = 'O';
			break;
		case C_ograve:
		case C_oacute:
		case C_ocircunflex:
		case C_otilde:
		case C_odieresis:
		case C_oe:
		case C_oslash:
			tb[i] = 'o';
			break;
		case C_Ugrave:
		case C_Uacute:
		case C_Ucircunflex:
		case C_Udieresis:
			tb[i] = 'U';
			break;
		case C_ugrave:
		case C_uacute:
		case C_ucircunflex:
		case C_udieresis:
			tb[i] = 'u';
			break;
		case C_Ydieresis:
			tb[i] = 'Y';
			break;
		case C_ydieresis:
			tb[i] = 'y';
			break;
		case C_Ntilde:
			tb[i] = 'N';
			break;
		case C_ntilde:
			tb[i] = 'n';
			break;
		default:
			tb[i] = ' ';
			break;
		} /* end switch */
	} /* end for */
#if IDEAWIN
	if ((wf = _lopen((char *) toCharPtr(path), OF_READ)) == HFILE_ERROR)
		return tb;
	if (_lread(wf, buf, NON_ASCII_TAB_SIZE) == NON_ASCII_TAB_SIZE) {
		mCopy(tb, buf, NON_ASCII_TAB_SIZE);
	}
	_lclose(wf);
#else
	if ((f = fopen((char *) toCharPtr(path), "r")) == NULL)
		return tb;
	if (fread(buf, 1, NON_ASCII_TAB_SIZE, f) == NON_ASCII_TAB_SIZE) {
		mCopy(tb, buf, NON_ASCII_TAB_SIZE);
	}
	fclose(f);
#endif
	return tb;
}

// allocate memory for inverted character table if needed
static UnChar *invNonAsciiTab()
{
	if (invNonAsciiTab_ == NULL) {
		invNonAsciiTab_ = tr_new UnChar[INV_NON_ASCII_TAB_SIZE];
	}
	return invNonAsciiTab_;
}

static UnChar *nonAsciiTab()
{
	if (nonAsciiTab_ == NULL) {
		nonAsciiTab_ = initNonAsciiTab();
	}
	return nonAsciiTab_;
}

static void initInvNonAsciiTab(void)
{
	int i, j;
#ifdef __SC__
// Compiler bug
	nonAsciiTab();
	invNonAsciiTab();
#endif
	if (isTablesInitialized) return;	/* do not make job again */
	for (i=INV_NON_ASCII_TAB_SIZE; i <= 255; i++) {
		for (j=0; j < NON_ASCII_TAB_SIZE; j++) {
			if (nonAsciiTab()[j] == i) { /* found */
				invNonAsciiTab()[i - INV_NON_ASCII_TAB_SIZE] = j + 192;
				break;
			} else {
				invNonAsciiTab()[i - INV_NON_ASCII_TAB_SIZE] = i;
			}
		}
	}
}


// Convert an Ideafix character to ASCII
#ifdef __VC__
#	undef toascii
#	define toascii idea_toascii
#endif

extern "C" int toascii(int c)
{
	if (!isTablesInitialized) {
		initNonAsciiTab();
		initInvNonAsciiTab();
		isTablesInitialized = 1;
	}
	c &= 0xFF;
	if (c < 128) {
		//delete [] nonAsciiTab_; 
		//isTablesInitialized = 0;
		return c;
	}
	if (c >= 192 && c <= 255)
	{
		//return (int) nonAsciiTab()[c - 192];
		int aux = (int) nonAsciiTab()[c - 192];
		//delete [] nonAsciiTab_; 
		return aux;
	}
	else {
		//fprintf(stderr,"ptr to delete: %X\n", nonAsciiTab_);
		//delete [] nonAsciiTab_; 
		return c;
	}
	
}

// Convert an ASCII character to Ideafix
int fromascii(int c)
{
	if (!isTablesInitialized) {
		initNonAsciiTab();
		initInvNonAsciiTab();
		isTablesInitialized = 1;
	}
	c &= 0xFF;
	if (c < 128)
		return c;
	if (c >= 128 && c <= 255)
		return (int) invNonAsciiTab()[c - 128];
	else
		return ' ';
}

void StrToAscii(register UnChar *p)
{
	while (*p) {
		*p = (UnChar) toascii( *p );
		++p;
	}
}

void StrFromAscii(register UnChar *p)
{
	while (*p) {
		*p = (UnChar) fromascii( *p );
		++p;
	}
}
#endif

}
