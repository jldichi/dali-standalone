/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: prcmd.h,v 1.7 2010/07/14 14:57:04 cristiand Exp $
*
* DESCRIPTION:
*********************************************************************/

#ifndef PRCMD_H
#define PRCMD_H
#include <ifound.h>
#include <cfix/types.h>

// Interesting macros - outputs
#define MOREDATA		(Int(1))
#define EO_FRAME		(Int(2))
#define EO_COMMAND		(Int(3))

// Used in the openForm routine, it means that the cracker needs the form
#define NEEDFILE		(Int(-2))
#define TRANSCHUNK		128

// operations allowed when the cracker synchronizes its own data base
#define PUTRECORD		1

class PrRequest {
public:
	enum {
		nullReq,			//  0
		getTxt,				//  1
		execFun,			//  2
		helpInTable,		//  3
		inTableValidate,	//  4
		after,				//  5
		before,				//  6
		useSubform,			//  7
		insMult,			//  8
		delMult,			//  9
		srchTxt,			// 10
		onKey,				// 11
	};
};

class PrCmd {
public:
	enum {
		clearMsg,
		displayMsg,
		popUpMenu,
		popUpVMenu,
		wiAplHelp,
		wiCol,
		wiCreate,
		wiCreateProces,
		wiCurrent,
		wiDelete,
		wiDeleteAll,
		wiDeleteProces,
		wiDialog,
		wiGetC,
		wiGetField,
		wiGetTab,
		wiGets,
		wiHeight,
		wiHelp,
		wiInChar,
		wiInterrupts,
		wiKeyHelp,
		wiKeyReady,
		wiLine,
		wiMove,
		wiOrgCol,
		wiOrgRow,
		wiParent,
		wiProcess,
		wiPutc,
		wiRedraw,
		wiRefresh,
		wiResettty,
		wiResize,
		wiService,
		wiSetAplHelp,
		wiSetBorder,
		wiSetRaw,
		wiSetServices,
		wiSetTab,
		wiSettty,
		wiSwitchTo,
		wiWidth,
		wiStatusLine,
		wiGetAttr,
		wiInAttr,
		wiDefPar,
		wiSetDefPar,
		openForm,
		closeForm,
		doForm,
		fmShowFlds,
		fmSetError,
		openTermOutput,
		openRpOutput,
		openDelimOutput,
		openFmtOutput,
		openRemoteOutput,
		openGraphOutput,
		setOutputColumn,
		columnWidth,
		setList,
		outputColumn,
		closeOutput,
		setRpOutput,
		setGraphOutput,
		columnNdec,
		nColsOutput,
		currColOutput,
		pageWidthOutput,
		doSubForm,
		displayForm,
		clearForm,
		useSubForm,
		onKey,
		execSystem,
		endEventLoop,
		wiDisconnect,
		setDisplayOnly,
		fmSetMask,
		showImage,
		closeImage,
		nextFld,
		fmSetKeyCode,
		wiExecCmd,
		execShell,
		prReceiveFile,
		prSendFile,
		getClipboardData,
		sendPermsToCracker,
		wiFieldProperty,
		flush,
		invalidOp 	// last operation + 1
	};
};

extern char *reqName[];
extern char *cmdName[];

inline void uncodeField(Int code, Int &fldNo, Int &rowno)
{
	fldNo = code >> 16;
	rowno = code & Int(0xFFFF);
}

inline Int codeField(Int fldno, Int rowno)
{
	return (fldno<<16)|rowno;
}

extern bool connDebug;

#endif

