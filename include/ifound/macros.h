/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: macros.h,v 1.14 2004/09/17 18:06:21 damianm Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef IX_MACROS_H
#define IX_MACROS_H

#include <ifound/osdep.h>
#include <cfix/priv/ideaimp.h>

#include <time.h>
#include <sys/timeb.h>

#ifdef DEBUG
#	ifndef ISDEBUG
#		define ISDEBUG	1
#	endif
#endif

#include <ifound/iddebug.h>
#include <ifound/trmalloc.h>

/* Assertion macro */
#ifndef ISDEBUG
#	define NOASSERTH
#endif

#ifdef __cplusplus
extern "C" {
#endif
void _assert(char const *, char const *, int);
void _precond(char const *, char const *, int);
void _postcond(char const *, char const *, int);
void _abnormal(char const *, int);
#ifdef __cplusplus
}
#endif

#ifdef NOASSERTH
#	define hAssert(EX)
#else
#	define hAssert(EX) \
		do { if (EX) ; else _assert(quote(EX), __FILE__, __LINE__); } while(0)
#endif

#ifdef NOPRECONDITIONS
#	define require(EX)
#else
#	define require(EX) \
		do { if (EX) ; else _precond(quote(EX), __FILE__, __LINE__); } while(0)
#endif

#ifdef NOPOSTCONDITIONS
#	define ensure(EX)
#else
#	define ensure(EX) \
		do { if (EX) ; else _postcond(quote(EX), __FILE__, __LINE__); } while(0)
#endif

#define abnormal	_abnormal(__FILE__, __LINE__)

#define assert(EX) \
		do { if (EX) ; else _assert(quote(EX), __FILE__, __LINE__); } while(0)

#define check(EX)		assert(EX)
#define implies(a, b)	(!(a) || (b))

#if defined(__cplusplus) && !defined(NOFTRACE)
#define FTRACE(msg)	\
		FunctionTrace paste(__FUNCTION__,ftrace)(__FUNCTION__, msg)
#else
#define FTRACE(msg)
#endif

#define forever for (;;)

#ifndef NULL
#define NULL	(0)
#endif

#if !HAVE_EXPLICIT
#define explicit
#endif

#if !HAVE_MUTABLE
#define mutable
#endif

#undef HIGH_VAL_IMAGE
#undef HIGH_VALUE_IMAGE
#undef HIGH_VALUE
#undef LOW_VALUE
#undef NULL_VALUE
#define HIGH_VAL_IMAGE		"(high value)"
#define HIGH_VALUE_IMAGE	HIGH_VAL_IMAGE
#define HIGH_VALUE			((char *) 1)
#define LOW_VALUE			((char *) NULL)
#define NULL_VALUE			((char *) NULL)

#ifndef ERR
#define ERR		(-1)
#endif

#ifndef ERROR
#define ERROR	(-1)
#endif

#ifndef OK
#define OK		(0)
#endif

#define TRACE												\
	do {												\
		fprintf(stddbg, "%s (%d)\t :%s\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);		\
		fflush(stddbg);										\
	} while (0);

#define CLASS(str) \
	do { \
		fprintf(stddbg, "%s::%s\n",str,__FUNCTION__); \
		fflush(stddbg); \
	} while (0);

#define TRACEV(fmt, var)										\
	do {														\
		fprintf(stddbg, "%s (%d)\t :%s\n\t", __FILE__, __LINE__, __PRETTY_FUNCTION__);		\
		fprintf(stddbg, quote(var) " = %" quote(fmt) "\n", var);\
		fflush(stddbg);											\
	} while (0);

#define TRACES(txt)                                                                             \
        do {                 \
                fprintf(stddbg, "%s (%d)\t :%s ", __FILE__, __LINE__, __PRETTY_FUNCTION__);             \
                fprintf(stddbg,  "--> " quote(txt) "\n" );\
                fflush(stddbg);                                                                                 \
        } while (0);


#define TRACE_TIME_IN \
        do {       \
                time_t tracetime; \
                struct timeb tracetime2; \
                struct tm *tracetime3; \
                tracetime = time(NULL); \
                tracetime3 = (struct tm *)localtime(&tracetime); \
                ftime(&tracetime2); \
                fprintf(stddbg, "%s (%d)\t :%s ", __FILE__, __LINE__, __PRETTY_FUNCTION__); \
                fprintf(stddbg,  "ENTRO --> %02.2ld:%02.2ld:%02.2ld.%03.3ld\n", tracetime3->tm_hour,tracetime3->tm_min,tracetime3->tm_sec,tracetime2.millitm );\
                fflush(stddbg);  \
        } while (0);


#define TRACE_TIME_OUT \
        do {       \
                time_t tracetime; \
                struct timeb tracetime2; \
                struct tm *tracetime3; \
                tracetime = time(NULL); \
                tracetime3 = (struct tm *)localtime(&tracetime); \
                ftime(&tracetime2); \
                fprintf(stddbg, "%s (%d)\t :%s ", __FILE__, __LINE__, __PRETTY_FUNCTION__); \
                fprintf(stddbg,  "SALIO --> %02.2ld:%02.2ld:%02.2ld.%03.3ld\n", tracetime3->tm_hour,tracetime3->tm_min,tracetime3->tm_sec,tracetime2.millitm );\
                fflush(stddbg);  \
        } while (0);


#ifdef __cplusplus

#undef paste
#undef name2
#undef name3
#undef name4
#undef declare
#undef implement
#undef declare2
#undef implement2
#undef declare3
#undef implement3

#if defined(HAVE_GENERIC_HPP) && !defined(__VC__)
#	include <generic.hpp>
#	define paste(a,b)		name2(a,b)
#else
#	ifdef HAVE_PASTE
#		include <generic.h>
#		define paste(a,b)		_Paste(a,b)
#		define name2(a,b)		_Paste2(a,b)
#		define name3(a,b,c) 	_Paste3(a,b,c)
#		define name4(a,b,c,d) 	_Paste4(a,b,c,d)
#	else
#		define paste(a,b)		a##b
#		define name2(a,b)		a##b
#		define name3(a,b,c)		a##b##c
#		define name4(a,b,c,d)  	a##b##c##d
#	endif
#	define declare(a, t) name2(a, declare)(t)
#	define implement(a, t) name2(a, implement)(t)
#	define declare2(a, t1, t2) name2(a, declare2)(t1,t2)
#	define implement2(a, t1, t2) name2(a, implement2)(t1,t2)
#endif

#define declare3(a, t1, t2, t3) name2(a, declare3)(t1,t2,t3)
#define implement3(a, t1, t2, t3) name2(a, implement3)(t1,t2,t3)

/* Macros to avoid declaring generic types in .h's */
#define gtype(GTYPE, CONTENT)				name2(CONTENT, GTYPE)
#define gtype2(GTYPE, CONTENT1, CONTENT2)	name3(CONTENT1, CONTENT2, GTYPE)
#define gtype3(GTYPE, C1, C2, C3)	name4(C1, C2, C3, GTYPE)

#endif

/*------------------- Messages Functions Constants ------------------*/

#define MSG_PRESERVE 			0x01
#define MSG_WAIT			0x02
#define MSG_RELEASE			0x04
#define MSG_FATALERROR			0x08
#define MSG_ERROR			0x10
#define MSG_OTHER			0x20
#define MSG_DISPLAY			0x40
#define MSG_WARNING			0x80

/* 
 * Packages enumeration:
 *   0 - 99  Reserved for system software packages.
 *   100 - 900 Reserved for application products and their modules.
 */
#define NUM_PCKGS       	100

// Productos de Software de Base 
#define SWBASE_PKGS		15		
#define PKG_RUNTIME		0
#define PKG_DEVELOP		1
#define PKG_IQL	 		2
#define PKG_SERVICES		3
#define PKG_ESSENTIA		4
#define PKG_CRACKER		5
#define PKG_BPM	 		6
#define PKG_SQLFW		7
#define PKG_ISYS  		8
#define PKG_SQLFWPG 		9
/* Future Packages
#define PKG_RESERVED2 		10
#define PKG_RESERVED3 		11
#define PKG_RESERVED4 		12
#define PKG_RESERVED5 		13
#define PKG_RESERVED6 		14
*/
// Compatiblility issues
#define APPL_BPM 		PGK_BPM 
#define APPL_ISYS  		PGK_ISYS
// Productos Aplicativos
#define APPL_AURUS		100
#define APPL_DENARIUS		200
#define APPL_IANUS		300
#define APPL_CBA		400
/* For future products
#define APPL_RESERVED1		500
#define APPL_RESERVED2		600
#define APPL_RESERVED3		700
#define APPL_RESERVED4		800
*/
#define APPL_VARIAS	  	900

/* aplications products and modules	*/

/* Aurus						*/
#define AURUS_MODULES			15		
#define	APPL_AURUS_BASICO		100 // AURUS
#define	APPL_AURUS_AUFIN		101
#define	APPL_AURUS_AURDGI		102
#define	APPL_AURUS_BANCOS		103
#define	APPL_AURUS_IBRUTOS		104
#define	APPL_AURUS_TESOR		105
#define	APPL_AURUS_COB			106
#define	APPL_AURUS_FAS			107
#define	APPL_AURUS_PRESUP		108
#define	APPL_AURUS_AUTGAN		109
#define	APPL_AURUS_CTASCOB		110

/* Denarius						*/
#define DENARIUS_MODULES		30						
#define	APPL_DENARIUS_BASICO		200 // DENARIUS
#define	APPL_DENARIUS_EMBARGO		201
#define	APPL_DENARIUS_PASEC		202
#define	APPL_DENARIUS_NOVEDAD		203
#define	APPL_DENARIUS_AUDIT		204
#define	APPL_DENARIUS_RETROACT		205
#define	APPL_DENARIUS_PLANVAC		206
#define	APPL_DENARIUS_HRSEXT		207
#define	APPL_DENARIUS_ASIST		208
#define	APPL_DENARIUS_DOTACION		209
#define	APPL_DENARIUS_TURNOS		210  
#define	APPL_DENARIUS_PERMS		211
#define	APPL_DENARIUS_INTERPS		212
#define APPL_DENARIUS_GESTCON		213
#define APPL_DENARIUS_EMPLEOS		214
#define APPL_DENARIUS_EVALDES		215
#define APPL_DENARIUS_MEDLAB		216
#define APPL_DENARIUS_PLANCAR		217
#define APPL_DENARIUS_REMUN		218
#define APPL_DENARIUS_SANC		219
#define APPL_DENARIUS_LIQ		220
#define APPL_DENARIUS_ADHABER		221
#define APPL_DENARIUS_LEGAR		222
#define APPL_DENARIUS_LEGUY		223
#define APPL_DENARIUS_LEGPY		224
#define APPL_DENARIUS_OLAP		225

/* Ianus						*/
#define IANUS_MODULES			10						
#define APPL_IANUS_BASICO		300 // IANUS
#define APPL_IANUS_SUMIN		301
#define APPL_IANUS_COMPRAS		302
#define APPL_IANUS_INTIA		303
#define APPL_IANUS_BUDGET		304
                                 
/* CBA - CONGA		*/
#define CBA_MODULES			10						
#define APPL_CBA_BASICO			400  
#define APPL_CBA_INTAUR			401

/*  Productos Varios */
#define VARIAS_MODULES			10						
#define APPL_BDU			900	// Bienes de Uso
#define APPL_PRESEST			901	// Presupuesto del Estado 
                                   
#endif
