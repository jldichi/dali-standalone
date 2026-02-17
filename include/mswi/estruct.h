/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
*
* $Id: estruct.h,v 1.2 1996/08/07 16:14:18 gab Exp $
*
*
* DESCRIPTION:
*	ESTRUCT:	Structure and preprocesser definitions
*
*
* Files that must be included before this:
* ----------------------------------------
*		idea.h
*
*
*********************************************************************/

#pragma pack(4)

#if MSDOS
#define V7      0             /* V7 UNIX or Coherent or BSD4.2*/
#else
#define V7      1
#endif

/*	Configuration options	*/

#define CVMVAS  1	/* arguments to page forward/back in pages	*/
#define	NFWORD	1	/* forward word jumps to begining of word	*/
#define	CLRMSG	0	/* space clears the message line with no insert	*/
#define	FILOCK	0	/* file locking under unix BSD 4.2		*/
#define	REVSTA	1	/* Status line appears in reverse video		*/

/*	System dependant library redefinitions	*/


/*	internal constants	*/

#define	NBINDS	200			/* max # of bound keys			*/
#define NFILEN  80          /* # of bytes, file name        */
#define NBUFN	16			/* # of bytes, buffer name		*/
#define NLINE   512         /* # of bytes, line             */
#define	NSTRING	256			/* # of bytes, string buffers	*/
#define NKBDM   256         /* # of strokes, keyboard macro */
#define NPAT    80          /* # of bytes, pattern          */
#define HUGE    1000        /* Huge number                  */
#define	NLOCKS	100			/* max # of file locks active	*/

#define AGRAVE  0x60        /* M- prefix,   Grave (LK201)   */
#define METACH  0x1B        /* M- prefix,   Control-[, ESC  */
#define CTMECH  0x1C        /* C-M- prefix, Control-\       */
#define EXITCH  0x1D        /* Exit level,  Control-]       */
#define CTRLCH  0x1E        /* C- prefix,   Control-^       */
#define HELPCH  0x1F        /* Help key,    Control-_       */

#define P_CTRLX	0x0100      /* Meta flag, or'ed in			*/
#define P_META 	0x0200      /* Meta2 flag, or'ed in			*/
#define CTLX	0x0400      /* ^X flag, or'ed in			*/
#define	SPEC	0x0800		/* special key (function keys)	*/

#define ABORT   2           /* Death, ^G, abort, etc.       */

#define FIOSUC  0           /* File I/O, success.           */
#define FIOFNF  1           /* File I/O, file not found.    */
#define FIOEOF  2           /* File I/O, end of file.       */
#define FIOERR  3           /* File I/O, error.             */
#define	FIOLNG	4			/*line longer than allowed len	*/
#define FIORON  5			/* Flag to inform file is READ ONLY */
#define FIONRE  6           /* File exists, but can't READ  */

#define CFCPCN  0x0001      /* Last command was C-P, C-N    */
#define CFKILL  0x0002      /* Last command was a kill      */

/*
 * There is a window structure allocated for every active display
 * window. The windows are kept in a big list, in top to
 * bottom screen order, with the listhead at "wheadp". u
 * Each window contains its own values of dot and mark.
 * The flag dbfield contains some bits that are set by commands to guide
 * redisplay; although this is a bit of a compromise in terms
 * of decoupling, the full blown redisplay is just too
 * expensive to run for every input
 * character.
 */

#if !IDEAWIN
typedef struct  WINDOW {
		struct  WINDOW *w_wndp;		/* Next window				  */
		struct  BUFFER *w_bufp;		/* Buffer displayed in window   */
		struct  LINE *w_linep;		/* Top line in the window	   */
		struct  LINE *w_dotp;		/* Line containing "."		  */
		short   w_doto;				/* Byte offset for "."		  */
		struct  LINE *w_markp;		/* Line containing "mark"	   */
		short   w_marko;			/* Byte offset for "mark"	   */
		short	w_offset;			/* offset respecto del buffer	*/
		char	w_force;			/* If NZ, forcing row.		 */
		char	w_flag;				/* Flags.				  */
		window	w_descr;		 	/* window descriptor		*/
		attr_type w_border;			/* Border atribute		 */
		short 	w_orow;				/*origin row in the screen	*/
		short 	w_ocol;				/* origin column in the screen	*/
		short	w_height;			/* deep							*/
		short 	w_width;			/*	widht						*/
}       WINDOW;
#endif

/* type for the borders of the windows */
#define ACTIVE_ATTR   A_STANDOUT
#define BACK_ATTR	  A_NORMAL

/* Margen de scroll en caso de tener que ajustar el texto */
#define MARGIN	4

#define WFFORCE 0x01                  /* Window needs forced reframe*/
#define WFMOVE  0x02                  /* Movement from line to line	*/
#define WFEDIT  0x04                  /* Editing within a line		*/
#define WFHARD  0x08                  /* Better to a full display	*/
#define WFMODE  0x10                  /* Update mode line.			*/
#define WFLIST	0x20				  /* Show controls characters	*/
#define WFDCOL	0x40				  /* Display column number		*/

/*	The Idea modes					*/
#define IDSHELL 0x20			/* buffer in shell mode			*/
#define IDEAQ   0x40			/* buffer in IQL mode			*/
#define EDITOR  0x00			/* buffer in standard mode	*/

/*
 * Text is kept in buffers. A buffer header, described below,
 * exists for every buffer in the system. The buffers are
 * kept in a big list, so that commands that search for a buffer
 * by name can find the buffer header. There is a safe store
 * for the dot and mark in the header, but this is only valid if
 * the buffer is not being displayed (that is, if "b_nwnd" is 0).
 * The text for the buffer is kept in a circularly linked
 * list of lines, with a pointer to the header line in "b_linep".
 * 	Buffers may be "Inactive" which means the files accosiated with them
 * have not been read in yet. These get read in at "use buffer" time.
 */
#if !IDEAWIN
typedef struct  BUFFER {
		struct  BUFFER *b_bufp;     /* Link to next BUFFER          */
		struct  LINE *b_dotp;         /* Link to "." LINE structure   */
		short   b_doto;               /* Offset of "." in above LINE  */
		struct  LINE *b_markp;        /* The same as the above two,   */
		short   b_marko;              /* but for the "mark"           */
		struct  LINE *b_linep;        /* Link to the header LINE      */
		char	b_active;		/* window activated flag	*/
		char    b_nwnd;               /* Count of windows on buffer   */
		char    b_flag;               /* Flags                        */
		int		b_mode;			/* editor mode of this buffer	*/
		char    b_fname[NFILEN];      /* File name                    */
		char    b_bname[NBUFN];      /* Buffer name                  */
		char	b_ideamode;		/*buffer mode in the IDEA soft	*/
}       BUFFER;
#endif

#define BFTEMP  0x01                  /* Internal temporary buffer    */
#define BFCHG   0x02                  /* Changed since last write     */

#define BNAME	0x01				/* They are for the search cond.*/
#define FNAME	0x02				/* in the bfind routine.	*/


#define	MDWRAP		0x0001			/* word wrap			*/
#define	MDCMOD		0x0002			/* C indentation and fence match*/
#define	MDSPELL		0x0004			/* spell error parcing		*/
#define	MDEXACT		0x0008			/* Exact matching for searches	*/
#define	MDVIEW		0x0010			/* read-only buffer		*/
#define MDOVER		0x0020			/* overwrite mode		*/
#define MSGRAPH		0x0040			/* Single graphic mode	*/
#define MDGRAPH		0x0080			/* Double graphic mode	*/
#define MDSGRAPH	0x0100			/* Double Single graphic mode	*/
#define MTROFF		0x0200			/* Troff interpret				*/
#define MFORM		0x0400			/* Form Definition file			*/
#define MREPORT		0x0800			/* Report Definition file		*/
#define MDGEN		0x1000			/* Data	Definition file			*/
#define MSHELL		0x2000			/* Shell Procedure file			*/


/*
 * The starting position of a region, and the size of the region in
 * characters, is kept in a region structure.
 * Used by the region commands.
 */
#if !IDEAWIN
typedef struct  {
        struct  LINE *r_linep;        /* Origin LINE address.         */
        short   r_offset;             /* Origin LINE offset.          */
        short   r_size;               /* Length in characters.        */
}       REGION;
#endif

/*
 * All text is kept in circularly linked lists of "LINE" structures.
 * These begin at the header line (which is the blank line beyond
 * the end of the buffer). This line is pointed to by the
 * "BUFFER". Each line contains a the number of bytes in the
 * line (the "used" size), the size of the text array,
 * and the text. The end of line is not stored as a byte; 
 * it's implied. Future additions will  update hints,
 * and a list of marks into the line.
 */
typedef struct  LINE {
        struct  LINE *l_fp;           /* Link to the next line        */
        struct  LINE *l_bp;           /* Link to the previous line    */
        short   l_size;               /* Allocated size               */
        short   l_used;               /* Used size                    */
        char    l_text[1];            /* A bunch of characters.       */
}       LINE;

#define lforw(lp)       ((lp)->l_fp)
#define lback(lp)       ((lp)->l_bp)
#define lgetc(lp, n)    ((lp)->l_text[(n)]&0xFF)
#define lputc(lp, n, c) ((lp)->l_text[(n)]=(c))
#define llength(lp)     ((lp)->l_used)

#if HAVE_PRAGMA_PACK
#	pragma pack()
#endif

