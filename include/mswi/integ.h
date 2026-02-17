/********************************************************************
* ORGLAND Ltd. IDEAFIX for MS Windows.
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* MODULE        :
* VERSION       : 1.0
* DATE          :
*
* DESCRIPTION   : Extended name of module (one string)
*
* LAST CHANGE   : DATE
*
*********************************************************************/
#ifndef _INC_IWINTEGR
#define _INC_IWINTEGR

#define PROGNAMESIZE	9
#define MAXUTIL         18
#define CMDLINESIZE      128
#define FNAMEEXTSIZE	5

#define	ID_FIRST		101
#define	FLAGS_ET		ID_FIRST
#define PARAMS_ET		(ID_FIRST + 1)
#define	BT_BROWSE		(ID_FIRST + 2)

#define DGENFLAGSNUM		10 
#define GENFMFLAGSNUM       5
#define FGENFLAGSNUM        3
#define RGENFLAGSNUM		3
#define DBCHECKFLAGSNUM		1
#define FRECOVERFLAGSNUM	7
#define GENCFFLAGSNUM		6
#define EXPFLAGSNUM			12
#define IMPFLAGSNUM			11
#define IQLFLAGSNUM		10
#define EXECFORMFLAGSNUM	5  //4
#define DOFORMFLAGSNUM		3  //2
#define DOREPORTFLAGSNUM	5  //4
#define CFIXFLAGSNUM		13 //14
#define MENUFLAGSNUM		1
#define IEFLAGSNUM			3

#define SC		"sc"
#define	SCO		"sco"
#define FM		"fm"
#define FMO		"fmo"
#define RP		"rp"
#define RPO		"rpo"
#define IE		"sc;fm;rp"
#define IQL		"sql"
#define DAT		"dat"
#define BAT		"bat"
#define MN		"mn"
#define CF		"c;cpp;obj;lib"

enum {dgen,genfm,fgen,testform,gencf,rgen,exp,imp,dbcheck,frecover,ie,iql,
			doform,doreport,menu,execform,dobatch,cfix};

typedef struct flag {
    BOOL    bFlag,				// is flag active?
            bParam;     		// has the flag additional parmeter
    char    chFlag;
    int		IDTempl,			// ID of control (checkbox) of dialog template
    		IDParamTempl;		// as default IDTempl+1
    char	* szParam;
};

typedef struct util {
	int		nFlags;				// possible number of flags
								// in MS Windows version flag -b
								// is ignored
	flag	* lpszFlagsArray;
	char 	szUtilName[PROGNAMESIZE];
	char 	szFlags[CMDLINESIZE];
	char 	szParams[CMDLINESIZE];
    char    * lpszFnameExt;
    };

extern util aUtilOptions[MAXUTIL];

#define _MAX_PATH	256
extern char szWorkingDir[_MAX_PATH];
#endif  /* _INC_IWINTEGR */

