/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: windos.cc,v 1.1.1.1 1996/04/16 18:52:45 gustavof Exp $
*
* DESCRIPTION:
*   
*********************************************************************/

#include <ideafix/priv/wmdos.h>

private struct _wm_commands commands[] = {
	NULL /* DUMMY */,
	NULL /* _WiStop */,
	_WiCreate,
	_WiDelete,
	_WiDeleteAll,
	_WiSwitchTo,
	_WiRefresh,
	_WiRedraw,
	_WiSetBorder,
	_WiStatusLine,
	_WiGetAttr,
	_WiOrgCol,
	_WiOrgRow,
	_WiLine,
	_WiCol,
	_WiWidth,
	_WiHeight,
	_WiInChar,
	_WiInAttr,
	_WiGetTab,
	_WiSetTab,
	_WiParent,
	_WiFlush,
	_WiGetc,
	_WiGets,
	_WiKeyHelp,
	_WiInterrupts,
	_WiCurrent,
	_WiCreateProces,
	_WiDeleteProces,
	_WiSettty,
	_WiResettty,
	_WiGetField,
	_WiAplHelp,
	_WiSetAplHelp,
	_WiProcess,
	_WiSetRaw,
	_WiKeyReady,
	_WiSetServices,
	_WiMove,
	_WiResize,
	_WiService,
};
