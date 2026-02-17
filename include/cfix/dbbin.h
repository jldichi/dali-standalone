/*********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dbbin.h,v 1.2 1998/04/20 19:02:35 guiller Exp $
*
* DESCRIPTION
*
*********************************************************************/

#ifndef DBBIN_H
#define DBBIN_H

typedef enum {
	BINARY_EMPTY = 0,
	BINARY_TEXT,
	BINARY_GIF,
	BINARY_PM,
	BINARY_PBM,
	BINARY_XBM,
	BINARY_BMP,
	BINARY_PCX,
	BINARY_TIFF,		/* reserved but no supported */
	BINARY_WAV, 		/* reserved but no supported */
	BINARY_UNKNOWN
} BinaryType;

#define IO_OPEN_BINARY	(-2)
#define IO_NO_BINARY	(-3)
#define IO_INV_BUFFER	(-4)

struct BinaryInfo {
	long	   timeStamp;
    BinaryType binaryType;
    long	   actualSize;
};

typedef struct BinaryInfo BinaryInfo;

#endif

