/***********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: chset.h,v 1.2 1998/04/21 15:14:34 guiller Exp $
*
* DESCRIPTION
*
* Character set stuff
*
* Set map:
* --------
*
*      0 -  31 : Control characters.
*	  32 - 126 : ' ' - '~' ASCII.
*          127 : DEL.
*    128 - 159 : Extended control. InterSoft function keys.
*    160 - 191 : Graphic characters.
*    192 - 254 : Linguistic characters. 
*	       255 : DEL | 0x
*
*********************************************************************/

#ifndef	CHSET_H
#define	CHSET_H

/*
 * Set de caracteres de InterSoft.
 * Definicion de Funciones y caracteres especiales.
 *
 */
 
#define NCHSET	256

#define     K_ESC  			(unsigned char) 27

#define     K_CTRLB  		(unsigned char)  2
#define     K_CTRLD  		(unsigned char)  4
#define     K_CTRLF  		(unsigned char)  6
#define     K_CTRLU  		(unsigned char) 25

#define     K_PROCESS  		(unsigned char) 128
#define     K_END      		(unsigned char) 129
#define     K_REMOVE   		(unsigned char) 130
#define     K_IGNORE   		(unsigned char) 131
#define     K_ENTER			(unsigned char) 132
#define     K_CURS_UP		(unsigned char) 133
#define     K_CURS_LEFT		(unsigned char) 134
#define     K_CURS_DOWN		(unsigned char) 135
#define     K_CURS_RIGHT	(unsigned char) 136
#define     K_PAGE_UP		(unsigned char) 137
#define     K_PAGE_LEFT		(unsigned char) 138
#define     K_PAGE_DOWN		(unsigned char) 139
#define     K_PAGE_RIGHT	(unsigned char) 140
#define		K_BACKSPACE		(unsigned char) 141
#define     K_UNDO			(unsigned char) 142
#define     K_INS      		(unsigned char) 143
#define     K_DEL	 		(unsigned char) 144
#define     K_CLEAR_FLD		(unsigned char) 145
#define     K_TAB	 		(unsigned char) 146
#define     K_APL_HELP		(unsigned char) 147
#define     K_HELP   		(unsigned char) 148
#define     K_REDRAW   		(unsigned char) 149
#define     K_ATTENTION 	(unsigned char) 150
#define		K_KEYCONF		(unsigned char) 151
#define		K_INTERRUPT		(unsigned char) 152
#define		K_SUSPEND		(unsigned char) 153
#define		K_META			(unsigned char) 154
#define		K_ALT			(unsigned char) 155
#define		K_CTRLX			(unsigned char) K_ALT
#define		K_AF1			(unsigned char) 156
#define		K_AF2			(unsigned char) 157
#define		K_AF3 			(unsigned char) 158
#define		K_AF4			(unsigned char) 159

#define		K_FIRST			K_AF1
#define		K_LAST			K_AF2

/*
 * Set de caracteres graficos.
 * Comienza en 160.
 */

/* Lineas simples */
#define	C_VERT		(unsigned char) 160
#define	C_HORIZ		(unsigned char) 161
#define	C_LL		(unsigned char) 162
#define	C_BOT		(unsigned char) 163
#define	C_LR		(unsigned char) 164
#define	C_RIGHT		(unsigned char) 165
#define	C_CEN		(unsigned char) 166
#define	C_LEFT		(unsigned char) 167
#define	C_UL		(unsigned char) 168
#define	C_TOP		(unsigned char) 169
#define	C_UR		(unsigned char) 170

/*
 Otros 
 */
#define	C_BULLET	(unsigned char) 171
#define	C_UARROW	(unsigned char) 172
#define	C_DARROW	(unsigned char) 173
#define	C_LARROW	(unsigned char) 174
#define	C_RARROW	(unsigned char) 175

/* Lineas dobles */

#define	C_D_VERT	(unsigned char) 176
#define	C_D_HORIZ	(unsigned char) 177
#define	C_D_LL		(unsigned char) 178
#define	C_D_BOT		(unsigned char) 179
#define	C_D_LR		(unsigned char) 180
#define	C_D_RIGHT	(unsigned char) 181
#define	C_D_CEN		(unsigned char) 182
#define	C_D_LEFT	(unsigned char) 183
#define	C_D_UL		(unsigned char) 184
#define	C_D_TOP		(unsigned char) 185
#define	C_D_UR		(unsigned char) 186

#define	C_BLOCK		(unsigned char) 187

#define	C_DS_BOT	(unsigned char) 188
#define	C_DS_RIGHT	(unsigned char) 189
#define	C_DS_LEFT	(unsigned char) 190
#define	C_DS_TOP	(unsigned char) 191

/*
 * Set de caracteres linguisticos. 
 * Comienza en (unsigned char) 192.
 */

#define		C_Agrave		(unsigned char) 192
#define		C_Aacute		(unsigned char) 193
#define		C_Acircunflex	(unsigned char) 194
#define		C_Atilde		(unsigned char) 195
#define		C_Adieresis		(unsigned char) 196
#define		C_Aring			(unsigned char) 197
#define		C_AE			(unsigned char) 198
#define		C_Ccedilla		(unsigned char) 199
#define		C_Egrave		(unsigned char) 200
#define		C_Eacute		(unsigned char) 201
#define		C_Ecircunflex	(unsigned char) 202
#define		C_Edieresis		(unsigned char) 203
#define		C_Igrave		(unsigned char) 204
#define		C_Iacute		(unsigned char) 205
#define		C_Icircunflex	(unsigned char) 206
#define		C_Idieresis		(unsigned char) 207
#define		C_exclamdown	(unsigned char) 208 
#define		C_Ntilde		(unsigned char) 209
#define		C_Ograve		(unsigned char) 210
#define		C_Oacute		(unsigned char) 211
#define		C_Ocircunflex	(unsigned char) 212
#define		C_Otilde		(unsigned char) 213
#define		C_Odieresis		(unsigned char) 214
#define		C_OE			(unsigned char) 215
#define		C_Oslash		(unsigned char) 216
#define		C_Ugrave		(unsigned char) 217
#define		C_Uacute		(unsigned char) 218
#define		C_Ucircunflex	(unsigned char) 219
#define		C_Udieresis		(unsigned char) 220
#define		C_Ydieresis		(unsigned char) 221
#define		C_Cents			(unsigned char) 222
#define		C_germandbls	(unsigned char) 223
#define		C_agrave		(unsigned char) 224
#define     C_aacute		(unsigned char) 225
#define		C_acircunflex	(unsigned char) 226
#define		C_atilde		(unsigned char) 227
#define		C_adieresis		(unsigned char) 228
#define		C_aring			(unsigned char) 229
#define		C_ae			(unsigned char) 230
#define		C_ccedilla		(unsigned char) 231
#define		C_egrave		(unsigned char) 232
#define     C_eacute		(unsigned char) 233
#define	 	C_ecircunflex	(unsigned char) 234
#define		C_edieresis		(unsigned char) 235
#define		C_igrave		(unsigned char) 236
#define     C_iacute		(unsigned char) 237
#define		C_icircunflex	(unsigned char) 238
#define		C_idieresis		(unsigned char) 239
#define 	C_questdown		(unsigned char) 240
#define     C_ntilde		(unsigned char) 241
#define		C_ograve		(unsigned char) 242
#define     C_oacute		(unsigned char) 243
#define		C_ocircunflex	(unsigned char) 244
#define		C_otilde		(unsigned char) 245
#define		C_odieresis		(unsigned char) 246
#define		C_oe			(unsigned char) 247
#define		C_oslash		(unsigned char) 248
#define		C_ugrave		(unsigned char) 249
#define     C_uacute		(unsigned char) 250
#define		C_ucircunflex	(unsigned char) 251
#define     C_udieresis		(unsigned char) 252
#define		C_ydieresis		(unsigned char) 253
#define		C_AUSTRAL		(unsigned char) 254
#define		C_SHADOW		(unsigned char) 255

/********** Ctype.h para el set extendido de intersoft **********/

#define	_U	01	/* Upper case */
#define	_L	02	/* Lower case */
#define	_N	04	/* Numeral (digit) */
#define	_S	010	/* Spacing character */
#define	_P	020	/* Punctuation */
#define	_C	040	/* Control character */
#define	_B	0100	/* Blank */
#define	_X	0200	/* heXadecimal digit */

#define iskey(c)	( ((c) & 0x80) && iscntrl(c))

/* DLL dependant code */

#if defined(__VC__) && defined(_DLL)
#	ifdef _DLLSRC
#		define DLLDEPEND	__declspec(dllexport)
#	else
#		define DLLDEPEND	__declspec(dllimport)
#	endif
#else
#	define DLLDEPEND
#endif

/* Needed because of Symantec Compiler for MS-DOS */
#ifdef __cplusplus
extern "C" {
#endif

extern DLLDEPEND char _ifx_ctype[];
extern int toLowerCase(int x);
extern int toUpperCase(int x);

#ifdef __cplusplus
}
#endif

#define	isalpha(c)	((_ifx_ctype + 1)[(unsigned char)(c)] & (_U | _L))
#define	isupper(c)	((_ifx_ctype + 1)[(unsigned char)(c)] & _U)
#define	islower(c)	((_ifx_ctype + 1)[(unsigned char)(c)] & _L)
#define	isdigit(c)	((_ifx_ctype + 1)[(unsigned char)(c)] & _N)
#define	isxdigit(c)	((_ifx_ctype + 1)[(unsigned char)(c)] & _X)
#define	isalnum(c)	((_ifx_ctype + 1)[(unsigned char)(c)] & (_U | _L | _N))
#define	isspace(c)	((_ifx_ctype + 1)[(unsigned char)(c)] & _S)
#define	ispunct(c)	((_ifx_ctype + 1)[(unsigned char)(c)] & _P)
#define	isprint(c)	((_ifx_ctype + 1)[(unsigned char)(c)] & (_P | _U | _L | _N | _B))
#define	isgraph(c)	((_ifx_ctype + 1)[(unsigned char)(c)] & (_P | _U | _L | _N))
#define	iscntrl(c)	((_ifx_ctype + 1)[(unsigned char)(c)] & _C)
#define	isascii(c)	(!((c) & ~0177))
#define	_toupper(c)	((unsigned char)(c) - 'a' + 'A')
#define	_tolower(c)	((unsigned char)(c) - 'A' + 'a')
#define	toascii(c)	idea_toascii(c)
#define isUChar(c)	(!((c) & ~0xFF))
#ifdef __cplusplus
extern "C" {
#endif
int idea_toascii(int c);
#ifdef __cplusplus
}
#endif

#if NEED_WCHAR_T
#	define __CTYPE_H
typedef unsigned short wchar_t;
#endif

#ifdef __VC__
#	define _CTYPE_DEFINED
#endif

#endif
