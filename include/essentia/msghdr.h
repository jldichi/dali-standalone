/********************************************************************
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: msghdr.h,v 1.1.1.1 1996/04/16 18:51:48 gustavof Exp $
*
* DESCRIPTION:
*				MsgBuff used by Connection and sons.
*********************************************************************/
#ifndef  MSGHDR_H
#define  MSGHDR_H

struct s_msgHdr {
	long clientId;
	union {
		long objectId;
		long retValue;
	};
	union {
		short serverOp;
		short status;
	};
	UnShort flags_;
	Int flags()				
		{ return (flags_ & 0xC000) >> (sizeof(flags_)*8 - 2); }
	Int length()			
		{ return flags_ & 0x3FFF; }
	void setFlags(short f)	
		{ flags_ = (flags_ & 0x3fff) | (f << (sizeof(flags_)*8 - 2)); }
	void setLength(Int l)
		{ flags_ = (flags_ & 0xC000) | (l & 0x00003FFF); }
};

typedef struct s_MsgBuff {
	struct s_msgHdr h;
	long data[1];
} *MsgBuff;

#endif //MSGHDR_H

