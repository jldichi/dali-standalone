/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: stconf.c,v 1.2 1996/07/08 13:36:15 eduardoc Exp $
*
*
* DESCRIPTION:
*      Configuration file for st routines.           
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
* _ifx_ctype		| Table for the ctype routines.
*SetStrNCmp		| sets the compare with 'n' function.
*SetStrCmp		| sets the compare function.
*StrNCmp	    | Cmp. strings until length 'n' in the compiled language
*StrCmp			| Cmp. strings in the compiled language
*********************************************************************/

#include <ideafix.h>
#include <ideafix/priv/gndefs.h>

#ifdef _cplusplus
extern "C" {
#endif
extern int toascii(int);
#ifdef _cplusplus
}
#endif


#if ENGLISH
#	elif SPANISH
#else
#	define SPANISH 1
#endif
/* #define SPANISH 1 */ /* Defined in <osdep.h> */
/* #define ENGLISH 1 */

/* ++ Prototypes ++ */

#if SPANISH
static int spanish_cmp(UChar *s1, UChar *s2);
static int spanish_n_cmp(UChar *s1, UChar *s2, int n);
static int assign_val(UChar **s, bool expand);
#endif
#if ENGLISH
#	include <string.h>
#endif

/* -- Prototypes -- */

/*	Features for strings compare	*/
#if ENGLISH
static IFPCPCP lex_cmp = (IFPCPCP) strcmp;
static IFPCPCPI lex_n_cmp = (IFPCPCPI) strncmp;
#endif

#if SPANISH
static short _sp_val[] = {
 	/*   0 ^@ */	0, 
	/*   1 ^A */	1,
	/*   2 ^B */	2,
	/*   3 ^C */	3,
	/*   4 ^D */	4,
	/*   5 ^E */	5,
	/*   6 ^F */	6,
	/*   7 ^G */	7,
	/*   8 ^H */	8,
	/*   9 ^I */	9,
	/*  10 ^J */	10,
	/*  11 ^K */	11,
	/*  12 ^L */	12,
	/*  13 ^M */	13,
	/*  14 ^N */	14,
	/*  15 ^O */	15,
	/*  16 ^P */	16,
	/*  17 ^Q */	17,
	/*  18 ^R */	18,
	/*  19 ^S */	19,
	/*  20 ^T */	20,
	/*  21 ^U */	21,
	/*  22 ^V */	22,
	/*  23 ^W */	23,
	/*  24 ^X */	24,
	/*  25 ^Y */	25,
	/*  26 ^Z */	26,
	/*  27 ^[ */	27,
	/*  28 ^\ */	28,
	/*  29 ^] */	29,
	/*  30 ^^ */	30,
	/*  31 ^_ */	31,

	/*  32 ' '*/	32,
	/*  33  ! */	33,
	/*  34  " */	34,
	/*  35  # */	35,
	/*  36  ( */	36,
	/*  37  % */	37,
	/*  38  & */	38,
	/*  39  ' */	39,
	/*  40  ( */	40,
	/*  41  ) */	41,
	/*  42  * */	42,
	/*  43  + */	43,
	/*  44  , */	44,
	/*  45  - */	45,
	/*  46  . */	46,
	/*  47  / */	47,

	/*  48  0 */	48,
	/*  49  1 */	49,
	/*  50  2 */	50,
	/*  51  3 */	51,
	/*  52  4 */	52,
	/*  53  5 */	53,
	/*  54  6 */	54,
	/*  55  7 */	55,
	/*  56  8 */	56,
	/*  57  9 */	57,

	/*  58  : */	58,
	/*  59  ; */	59,
	/*  60  < */	60,
	/*  61  = */	61,
	/*  62  > */	62,
	/*  63  ? */	63,
	/*  64  @ */	64,

	/*  65  A */	65,
	/*  66  B */	66,
	/*  67  C */	-67, /* 'CH' is the 68th	*/
	/*  68  D */	69,
	/*  69  E */	70,
	/*  70  F */	71,
	/*  71  G */	72,
	/*  72  H */	73,
	/*  73  I */	74,
	/*  74  J */	75,
	/*  75  K */	76,
	/*  76  L */	-77, /* 'LL' is the 78th	*/
	/*  77  M */	79,
	/*  78  N */	80, /* 'EGNE' is the 81th	*/
	/*  79  O */	82,
	/*  80  P */	83,
	/*  81  Q */	84,
	/*  82  R */	85,
	/*  83  S */	86,
	/*  84  T */	87,
	/*  85  U */	88,
	/*  86  V */	89,
	/*  87  W */	90,
	/*  88  X */	91,
	/*  89  Y */	92,
	/*  90  Z */	93,

	/*  91  [ */	94,
	/*  92  \ */	95,
	/*  93  ] */	96,
	/*  94  ^ */	97,
	/*  95  _ */	98,
	/*  96   */	99,

	/*  97  a */	100,
	/*  98  b */	101,
	/*  99  c */	-102, /* ch' is the 103 */
	/* 100  d */	104,
	/* 101  e */	105,
	/* 102  f */	106,
	/* 103  g */	107,
	/* 104  h */	108,
	/* 105  i */	109,
	/* 106  j */	110,
	/* 107  k */	111,
	/* 108  l */	-112, /* 'll' is the 113 */
	/* 109  m */	114,
	/* 110  n */	115, /* 'egne' is the 116 */
	/* 111  o */	117,
	/* 112  p */	118,
	/* 113  q */	119,
	/* 114  r */	120,
	/* 115  s */	121,
	/* 116  t */	122,
	/* 117  u */	123,
	/* 118  v */	124,
	/* 119  w */	125,
	/* 120  x */	126,
	/* 121  y */	127,
	/* 122  z */	128,

	/* 123  { */	129,
	/* 124  | */	130,
	/* 125  } */	131,
	/* 126  ~ */	132,

	/* 127 DEL */	133,
	/* 128 ~@ */	134,
	/* 129 ~A */	135,
	/* 130 ~B */	136,
	/* 131 ~C */	137,
	/* 132 ~D */	138,
	/* 133 ~E */	139,
	/* 134 ~F */	140,
	/* 135 ~G */	141,
	/* 136 ~H */	142,
	/* 137 ~I */	143,
	/* 138 ~J */	144,
	/* 139 ~K */	145,
	/* 140 ~L */	146,
	/* 141 ~M */	147,
	/* 142 ~N */	148,
	/* 143 ~O */	149,
	/* 144 ~P */	150,
	/* 145 ~Q */	151,
	/* 146 ~R */	152,
	/* 147 ~S */	153,
	/* 148 ~T */	154,
	/* 149 ~U */	155,
	/* 150 ~V */	156,
	/* 151 ~W */	157,
	/* 152 ~X */	158,
	/* 153 ~Y */	159,
	/* 154 ~Z */	160,
	/* 155 ~[ */	161,
	/* 156 ~\ */	162,
	/* 157 ~] */	163,
	/* 158 ~^ */	164,
	/* 159 ~_ */	165,

	/* 160    */	166,
	/* 161    */	167,
	/* 162    */	168,
	/* 163    */	169,
	/* 164    */	170,
	/* 165    */	171,
	/* 166    */	172,
	/* 167    */	173,
	/* 168    */	174,
	/* 169    */	175,
	/* 170    */	176,
	/* 171    */	177,
	/* 172    */	178,
	/* 173    */	179,
	/* 174    */	180,
	/* 175    */	181,
	/* 176    */	182,
	/* 177    */	183,
	/* 178    */	184,
	/* 179    */	185,
	/* 180    */	186,
	/* 181    */	187,
	/* 182    */	188,
	/* 183    */	189,
	/* 184    */	190,
	/* 185    */	191,
	/* 186    */	192,
	/* 187    */	193,
	/* 188    */	194,
	/* 189    */	195,
	/* 190    */	196,
	/* 191    */	197,
/*		Special characteres: atilde ,Agrave, etc...  */
	/* 192    */	65,
	/* 193    */	65,
	/* 194    */	65,
	/* 195    */	65,
	/* 196    */	65,
	/* 197    */	65,
	/* 198    */	000, /* AE que peso darle */
	/* 199    */	68,
	/* 200    */	70,	/* E grave = E		*/
	/* 201    */	70,
	/* 202    */	70,
	/* 203    */	70,
	/* 204    */	74,	/*	I grave = I		*/
	/* 205    */	74,
	/* 206    */	74,
	/* 207    */	74,

	/* 208 exclamdown*/		000,

	/* 209  C_Ntilde*/		81,	
	/* 210  Ograve  */		82,	/* O grave = O	*/
	/* 211  Oacute  */		82,
	/* 212          */		82,
	/* 213          */		82,
	/* 214          */		82,
	/* 215          */		000,/* OE		= no se.	*/
	/* 216 			*/		82,
	/* 217 			*/		88,
	/* 218 			*/		88,
	/* 219 			*/		88,
	/* 220 C_Udieresis*/	88,
	/* 221 C_Ydieresis*/	92,

	/* 222			*/		000, /*	Cents	=no se	*/

	/* 223			*/		000, /* Germandbls	=no se	*/

	/* 224 C_agrave */		100,
	/* 225 C_aacute */		100,
	/* 226			*/		100,
	/* 227			*/		100,
	/* 228			*/		100,
	/* 229			*/		100,
	/* 230			*/		000,	/*	ae	= no se	*/
	/* 231			*/		102,	/* ccedilla = c	*/
	/* 232			*/		105,
	/* 233 C_eacute */		105,
	/* 234			*/		105,
	/* 235			*/		105,
	/* 236			*/		109,
	/* 237 C_iacute */		109,
	/* 238			*/		109,
	/* 239			*/		109,

	/* 240			*/		000,	/* exclmdown= no se	*/

	/* 241 C_ntilde */		116,
	/* 242			*/		117,
	/* 243 C_oacute */		117,
	/* 244			*/		117,
	/* 245			*/		117,
	/* 246			*/		117,
	/* 247			*/		000,	/* oe	= no se	*/
	/* 248			*/		117,	/* oslash	= o	*/
	/* 249			*/		123,
	/* 250 C_uacute */		123,
	/* 251			*/		123,
	/* 252 C_udieresis */	123,
	/* 253 Ydieresis*/		127,

	/* 254			*/		65,		/* austral = A	*/

	/* 255			*/		198		/* shadow=ultimo car. grafico	*/
};


/*
*	String compare routine with spanish strings.
*/
static int spanish_cmp(UChar *s1, UChar *s2)
{
	int v1, v2; 
		
	do{
		v1	=assign_val(&s1, TRUE);
		v2	=assign_val(&s2, TRUE);
	}while( v1==v2 && v1);
	return v1-v2;
}


/*
*	String compare routine with spanish strings and fixed length.
*/
static int spanish_n_cmp(UChar *s1, UChar *s2, int n)
{
	int v1, v2;
	UChar *top = s1+n-1;
		
	while (top > s1) {
		v1	=assign_val(&s1, TRUE);
		v2	=assign_val(&s2, TRUE);

		if ( v1!=v2 || v1 == '\0') return v1-v2;
	}
	if (s1 > top) return 0;
	v1	=assign_val(&s1, FALSE);
	v2	=assign_val(&s2, FALSE);
	return v1-v2; 
}

/*
*	Assign a value of a character in spanish and sets the pointer
* string in the correct position.
*/
static int assign_val(UChar **s, bool expand)
{
	int val;
	
	if( (val = toascii( *(*s)++ )) >= 0)
		return val;

	if ( expand ) {
		switch (toupper((int)s[0][-1])) {
		case 'C' :
			/* check if 'ch'	*/
			if( toupper((int)s[0][0]) == 'H') {
				(*s)++;
				--val;
			}
			break;

		case 'L' :
			/* check if 'LL'	*/
			if ( toupper((int)s[0][0]) == 'L') {
				(*s)++;
				--val;
			}
			break;
		}
	}
	return -val;
}

static IFPCPCP lex_cmp = (IFPCPCP)spanish_cmp;
static IFPCPCPI lex_n_cmp= (IFPCPCPI)spanish_n_cmp;
#endif


int StrCmp(void *s1, void *s2)
{
	//return  (*lex_cmp)((const char*)s1, (const char*)s2);
	return strcmp((const char*) s1, (const char *)s2);
}

int StrNCmp(void *s1, void *s2, int n)
{
	return  (*lex_n_cmp)((char*)s1, (char*)s2, n);
}

void SetStrCmp(IFPCPCP function)
{
	lex_cmp=function;
}

void SetStrNCmp(IFPCPCPI function)
{
	lex_n_cmp=function;
}
