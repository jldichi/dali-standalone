/*********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved             
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.      
* The copyright notice above does not evidence any                  
* actual or intended publication of such source code.               
*                                                                   
*
* DATE             : %D%
* TIME             : %T%
*                                                                   
* DESCRIPTION:                                                      
*      Interface between the Window Manager process & WI library    
*                                                                   
* Files that must be included before this:                          
* ----------------------------------------                          
*                                                                   
*                                                                   
*                                                                   
*********************************************************************/

#define INIT_STRING	'\034'
#define INIT_SHORT	'\035'
#define INIT_LONG	'\036'
#define INIT_SEQ	'\037'
#define ACK			'\006'
#define NAK			'\025'


#define	C_WINDOW	0
#define	D_WINDOW	1
#define	D_A_WINDOW	2
#define	C_SUBWIN	3
#define	D_SUBWIN	4
#define	S_T_WIND	5
#define	W_INIT		6
#define	A_WINDOW	7
#define	W_REFRESH	8
#define	W_REDRAW	9
#define	W_BORDER	10
#define	W_KEYHELP	11
#define	W_LINE		12
#define	W_COL		13
#define	W_WIDTH		14
#define	W_HEIGHT	15
#define	SCR_LINE	16
#define	SCR_COL		17
#define	SCR_WIDTH	18
#define	SCR_HEIGHT	19
#define	W_INCHAR	20
#define	SETTTY		21
#define	RESETTTY	22
#define	W_GETC		23
#define	W_GETSTR	24
#define	W_GETNUM	25
#define	W_GETS		29
#define	LINE_SETUP	30
#define	C_PROCESS	32
#define D_PROCESS	33
#define S_ATTR	 	36
#define W_SCROLL	39
#define WPAR_LINE	40
#define W_GETATTR   41
#define W_INATTR	42 
#define WPAR_COL	43
#define W_GETDATE	44
#define W_GETTIME	45
#define W_ORGCOL	46
#define W_ORGROW	47
#define W_GETTAB    48
#define W_SETTAB    49
#define W_INTERRUPTS    50
#define W_SETAPLHELP    51
#define WMF_FIRST	0
#define WMF_LAST	51

#define	RWMF_SYNC_NEW_PROCESS	0
#define RWMF_LAST	51

#define PID_BUFFER	8

/* Utilizados por la biblioteca Wi */

void _w_send( /* int va_alist */);
int _w_rcvint();
long _w_rcvlong();
UChar *_w_rcvstr( /* UChar *s */);
