/*
 * $Id: fmmacro.h,v 1.1.1.1 1996/04/16 18:51:41 gustavof Exp $
 */
 
#ifndef FMMACRO_H
#define FMMACRO_H 

#define HORIZONTAL		"horizontal"
#define VERTICAL		"vertical"

// wx_box_style
#define RIZED_SOLID		"RIZED_SOLID"
#define RECESSED_SOLID	"RECESSED_SOLID"
#define RIZED_ROUNDED	"RIZED_ROUNDED"
#define RECESSED_ROUNDED "RECESSED_ROUNDED"
#define BUMPED			"BUMPED"
#define DIPPED			"DIPPED"
#define WHITE_SOLID		"WHITE_SOLID"
#define BLACK_SOLID		"BLACK_SOLID"
#define WHITE_ROUNDED	"WHITE_ROUNDED"
#define BLACK_ROUNDED	"BLACK_ROUNDED"

// static separator layout
#define VERT_SEPARATOR	"VERT_SEPARATOR"
#define HORIZ_SEPARATOR "HORIZ_SEPARATOR"

// static box mask
#define HAS_BORDER		"HASBORDER"
#define HAS_BOX			"HASBOX"

#define DEF_GEOM			(-1)
#define DEF_STYLE			(wxCOLOURED)
#define DEF_UNIQUE			false
//#define DEF_NEEDSRECALC		false
#define DEF_AUTOWRITE		false
#define DEF_DSPSTATUS		true

// action options
#define FM_ACTION_READ		"Read"
#define FM_ACTION_READNEXT	"ReadNext"
#define	FM_ACTION_READPREV	"ReadPrev"
#define	FM_ACTION_READFIRST "ReadFirst"
#define FM_ACTION_READLAST	"ReadLast"
#define	FM_ACTION_RESET		"Reset"
#define FM_ACTION_UPDATE	"Update"
#define FM_ACTION_ADD		"Add"
#define FM_ACTION_DELETE	"Delete"
#define FM_ACTION_CLOSE		"Close"
#define FM_ACTION_UNDO		"Undo"
#define FM_ACTION_SUBFORM	"OpenSubform"
#define FM_ACTION_CLEARFLD	"ClearField"

#define READ_BMP		"read.bmp"
#define RESET_BMP		"ignore.bmp"
#define UPDATE_BMP		"process.bmp"
#define ADD_BMP			"add.bmp"
#define DELETE_BMP		"remove.bmp"
#define CLOSE_BMP		"exit.bmp"
#define READ_NEXT_BMP	"next.bmp"
#define READ_PREV_BMP	"prev.bmp"
#define READ_FIRST_BMP	"first.bmp"
#define READ_LAST_BMP	"last.bmp"
#define UNDO_BMP		"undo.bmp"
#define SUBFORM_BMP		"subform.bmp"
#define CLEAR_FLD_BMP	 "clrfld.bmp"

#endif

