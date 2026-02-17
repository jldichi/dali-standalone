/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: usrsig.h,v 1.1.1.1 1996/04/16 18:51:48 gustavof Exp $
*
*
* DESCRIPTION:
*				Header for notify handling routines.
*
*********************************************************************/

#include <ifound.h>
#include <signal.h> 

class DbTableRep;

SigHandlerT setNotifyHandler(SigHandlerT);
SigHandlerT resetNotifyHandler();
bool isPoll();
bool isNotify(DbTableRep *tab);
bool delPollSocket(Int);
Int chkPoll();
void enablePollSignal();
void disablePollSignal();
void clearSignal(Int);
