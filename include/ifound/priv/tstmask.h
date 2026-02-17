/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: tstmask.h,v 1.1.1.1 1996/04/16 18:51:35 gustavof Exp $
*
* DESCRIPTION:
*	   Test defines.
* Files that must be included before this:
* ----------------------------------------
*
*********************************************************************/

#define TST_ANUM	0x01
#define TST_ALPHA	0x02
#define TST_NUM		0x04
#define TST_HEXA	0x08
#define TST_UPPER	0x10
#define TST_LOWER	0x20
#define TST_REQ		0x40
#define TST_CHK_DIG	0x80

/* For output mask */
#define TST_OCHK_DIG	0x01  /* Ctrl-A */


#define MAX_MASK_LENGTH 512

