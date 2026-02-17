/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: wmget.cc,v 1.8 2002/03/18 19:18:46 hardaiz Exp $
*
*
* DESCRIPTION:
*      Get by whole field.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
* WiGetField	| Read a whole field, checking it.
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>

#include <ifound/itypes.h>
#include <ifound/priv/tstmask.h>


#include <ideafix/priv/gndefs.h>
#include <ideafix/priv/wmcom.h>

#include <wm.h>

/* ++++ CONFIG +++++ */
#define MAX_STR			255		/* longitud maxima de un campo */
#define DEPTH			128		/* Maximo numero de llamadas recursivas */
/* ---- CONFIG ----- */


/* ++ Prototypes ++ */
static int MskDLen(char *m);
static int MaskChkDig(char *m);
static bool _VDate(char *s, int n, int totlen);
static int _WGetDate(UnChar *buff, short length, unsigned int opt, UShort _row, UShort _col);
static int _WGetStr(UnChar *buff, short length, short olength, unsigned opt, char *tstmask, char *omask, UShort _row, UShort _col);
static int _WGetNum(register UnChar *dato, short length, short olength, short ndec, short opt, UShort _row, UShort _col);
static void _DisplayNumField(char* fmt, short olength, short ndec, char* dato, short opt);
static int _WGetTime(UnChar *buff, short length, UShort opt, UShort _row, UShort _col);
static int _WGetFloat(UnChar *dato, short length, UShort opt, UShort _row, UShort _col);
static int _WGetBool(UnChar *buff, short length, UShort opt, UShort _row, UShort _col);
static bool _TstMask(bool del_flg, UnChar *tstmask, UnChar *buff, UnChar *pbuff, UnChar *pebuff);
static void _Display(UnChar *buff, UnChar *omask, UShort len, UShort olen, UShort _row, UShort _col, bool first);
static void _MoveNum(UnChar c, UnChar *buff, int len, short *desp, short olength, UShort _row, UShort _col, short *dsp);
static bool _VTime(char *s, int n, int totlen);
void CopyStringCharacters(char* destiny, const char* provenance);

/* -- Prototypes -- */

extern bool suspend; 			/* declarada en wmgetc.c		   	 */
extern bool hab_susp;			/* declarada en wmgetc.c		   	 */
extern bool signal_sent;		/* declarada en wmsig.c			   	 */

void _WiGetField(void)
{
	/* parametros de la rutina	*/
	type   t 		= wm_par[0].i;
	UShort opt 		= (UShort) wm_par[2].i;
	short  length 	= wm_par[3].i; 
	short  olength 	= wm_par[4].i; 
	short  ndec 	= wm_par[5].i;
	char   *tstmask = (char *) wm_par[6].s;
	char   *omask	= (char *) wm_par[7].s;
	UnChar   *datum;
	static UnChar  *stack_buff[DEPTH], n = 0;
	int     c;
#ifdef HAVE_TTY
	VS_CELL extr[2];
	int     len;
#endif
	attr_type attr_aux = WiGetAttr();
	UShort _row = (UShort) WiLine(WICURRENT),
		   _col = (UShort) WiCol(WICURRENT);

	/*
		If the check digit option is used, increment the lengths
		as needed.
	*/
	if (opt & WGET_CDIGIT) {
		length++;
		olength++;
	} else
		if (opt & (WGET_CDIGIT_SLASH|WGET_CDIGIT_DASH)) {
			length++;
			olength += 2;
		}

	/* Adjust to prevent fields longer than MAX_STR */
	length = length > MAX_STR ? MAX_STR : length;
#ifdef HAVE_TTY
	len = length;
#endif
	if (stack_buff[n]==NULL) stack_buff[n] = (UnChar *)Alloc(MAX_STR+1);
	datum = stack_buff[n++];
	strncpy((char*)datum, (char*)wm_par[1].s, length);
	datum[length] = 0;

	/* seteo o reverse o coloco los corchetes */
#ifdef HAVE_TTY
	if (term_rev)
		WiSetAttr(A_STANDOUT);
	else {
		int fi = wcurr->f_org+_row;
		int ci = wcurr->c_org+_col;
        int offset = olength;
        
		if (t == TY_DATE || t == TY_TIME) {
			offset += 2;
			if (length<5) offset--;
		}

		extr[0] = vscreen[fi][ci-1];
		extr[1] = vscreen[fi][ci+offset];

		_vs_fill('[' | vsattr, (short)fi, (short)(ci-1), 1, 1);
		_vs_fill(']' | vsattr, (short)fi, (short)(ci+offset), 1, 1);
	}
#else
	WiSetAttr(A_STANDOUT);
#endif

	switch (t) {
	case TY_NUMERIC: 
		c = _WGetNum(datum, length, olength, ndec, opt, _row, _col);
		break;
	case TY_DATE: 
		c = _WGetDate(datum, length, opt, _row, _col);
		break;
	case TY_TIME:
		c = _WGetTime(datum, length, opt, _row, _col);
		break;
	case TY_STRING:
		c = _WGetStr(datum,length,olength,opt,tstmask,omask,_row,_col);
		break;
	case TY_FLOAT:
		/* ojo la longitud debe ser mayor o igual que 7 */
		c = _WGetFloat(datum, length, opt, _row, _col);
		break;
	case TY_BOOL:
		c = _WGetBool(datum, length, opt, _row, _col);
		break;
	default:
		c = ERR;
	}
	if (!suspend && !signal_sent) {

		/*
		restituyo el atributo o lo que habia debajo de los corchetes
		*/
#ifdef HAVE_TTY
		if (term_rev)
			WiSetAttr(attr_aux);
		else {
	        int offset = olength;
        
			if (t == TY_DATE || t == TY_TIME) {
				offset += 2;
				if (length < 5)
					offset--;
			}
			_vs_fill(extr[0],wcurr->f_org+_row,wcurr->c_org+_col-1,1,1);
			_vs_fill(extr[1],wcurr->f_org+_row,wcurr->c_org+_col+offset,1,1);
		}
#else
		WiSetAttr(attr_aux);
#endif
		wm_par[0].i = (short)c;
		wm_par[1].s = datum;
	}
	if (stack_buff[n]) {
		Free(stack_buff[n]);
		stack_buff[n] = NULL;
	}
	n--;
}

static bool _VDate(register char *s, register int n, int totlen)
{
	register int i;
	register char *p;
	int day = ERR, month = ERR;
	int usa_date = GetDateFmt();

	if (s[0] == 0)  return true;

	for (i = 0; i <= n; i++) {
		
		p = &s[i];
		if (!isdigit(*p)) return false;
		if (i > 3) continue;
		
		switch (!usa_date ? i : (i < 2 ? i + 2 : i - 2)) {
		case 0 :
			if (p[0] > '3') return false;
			continue;
		case 1 : 
			day = (p[-1] - '0') * 10 + p[0] - '0';
			if (day < 1 || day > 31) return false;
			if (!usa_date) continue;
			break;
		case 2 :
			if (p[0] != '1' && p[0] != '0') return false;
			continue;
		case 3 :
			month = (p[-1] - '0') * 10 + p[0] - '0';
			if (month < 1 || month > 12) return false;
			if (usa_date) continue;
			break;
		}
		if ((month == 2 && day > 29) ||
				(day == 31 && (month == 4 || month == 6 || month == 9 ||
				month == 11)))
			return false;
	}
	// When we have the year, we must see if it is a valid date
	if (n+1 == totlen) {
		DATE d = StrToD(s);
		if (d == NULL_DATE)
			return false;

		if (day == 29 && month == 2) {
			// We obtain the complete year from the date recently transformed.
			int year = Year(d);

			if (year % 4 || (year % 100 == 0 && year % 400))
				return false;
		}
	}
	return true;
}

static int _WGetDate(UnChar *buff, short length, unsigned opt,
		UShort _row, UShort _col)
{
	register UnChar c = 0;
	bool end, _hab_susp = hab_susp;
	short _nc =0;

	buff[length] = '\0';
	WiPrintf("%2.2s/%2.2s/%*s", buff, *buff?buff+2:buff,
						    length-4, *buff?buff+4:buff);
	WiMoveTo(_row, _col);

	end = false;
	while (!end) {

		if (_hab_susp) hab_susp = ! _nc;
		c = WiGetc();
		hab_susp = _hab_susp;
		if (suspend || signal_sent) return c;

		switch (c) {
		case K_END :
		case K_IGNORE :
		case K_UNDO :
		case K_HELP :
			end = true;
			continue;
		case K_TAB :  
#ifndef __NT__
			if (!_nc && !(opt & WGET_DSPONLY)) { 
#else
			if (!_nc) { 
#endif
				DATE d = w_calendar(NULL_DATE, NULL, false);
				DToStr(d, (char*)buff, length == 6 ? 0 : DFMT_YEAR4);
				c = K_ENTER;            
				end = true;
				continue;
			}	
			break;
		case K_CLEAR_FLD :
			/* Not valid if display only			*/
			if (opt & WGET_DSPONLY) break;
			buff[0] = '\0';
			_nc = 0;
			WiMoveTo(_row, _col);
			WiPrintf("  /  /%*s", length-4, "");
			WiMoveTo(_row, _col);
			continue;
		case K_BACKSPACE :
			if (_nc) {
				if (_nc==2 || _nc==4) WiPutc('\b');
				if (_nc!=length) WiPutc('\b');
				WiPuts(" \b");
				buff[--_nc] = '\0';
				continue;
			}
			break;

		default :
			if (iskey(c) && _VDate((char*)buff, length - 1, length)) {
				end = true;
				continue;
			}
		}		

		if (isdigit(c) && _nc < length && !(opt&WGET_DSPONLY)) {
			buff[_nc] = c; 
			if (_VDate((char*)buff, _nc, length)) {
				_nc++; 
				WiPutc(c);
				if (_nc==2 || _nc==4) WiPutc('/');
				else if (_nc==length) {
					WiPutc('\b');
					if (opt & WGET_AUTOENTER) {
						c = K_ENTER;
						break;
					}
				}
				continue;
			}
		}
		w_beep();
	}

	WiMoveTo(_row, _col);
	return c;
}

static int _WGetNum(register UnChar *dato, short length, short olength,
		short ndec, short opt, UShort _row, UShort _col)
{
	int i, len = 0;
	register short ndigits;
	register UnChar c;
	short minus = 0, dot = 0, _nc = 0, _nd = 0 ;
	short desp= _col, offset, dsp;
	char *fmt;
	UnChar buffer[256], *buff = buffer;
	bool edit_mode = false, _hab_susp = hab_susp;
	bool cdigitOK = true;

	/*
	 * Compute the number of digits that fit in length
	 */
	ndigits = length - (ndec > 0) - !!(opt & WGET_SIGNED);
	if (opt & WGET_COMMA) {
		ndigits -= (ndigits - ndec) / 4;
		fmt = "%*.*#,*s";
	}
	else fmt = "%*.*#*s";
	/*
		If check digit and the field is not empty, calculate it and
		append it to the number.
	*/
	if ((opt & (WGET_CDIGIT|WGET_CDIGIT_DASH|WGET_CDIGIT_SLASH)) &&
			dato[0] != '\0') {
		int	cd = CheckDigit(StrToL((char *) dato)),
			lg = strlen((char *) dato);

		dato[lg] = (char)('0'+cd);
		dato[lg+1] = '\0';
	}

	do {
		WiMoveTo(_row, _col);
		_DisplayNumField(fmt, olength, ndec, (char *) dato, opt);
		WiPutc('\b');
		WiMoveTo(_row, _col);

		forever {

			if (_hab_susp) hab_susp = !_nc;
			c = WiGetc();
			hab_susp = _hab_susp;
			if (suspend || signal_sent) break;

			if (c == '>' || c == '<') {
				if (!edit_mode) {
					(void) sprintf((char*)buff, 
									opt&WGET_COMMA ? "%*.*#,*s" :
													 "%*.*#*s", length,
									length, ndec, dato);
					for (;*buff==' ';buff++);
					len = strlen((char*)buff);
					dsp = desp;
					offset =  (short) ((len > olength) ? len-olength : 0);
				}
				_MoveNum(c, buff, len, &offset, olength, _row, _col, &dsp);
				edit_mode = true;
				continue;
			}

			edit_mode = false;

			if (c!=K_BACKSPACE && c!=K_TAB && iskey(c)) break;

			if (opt & WGET_DSPONLY) c = 0;

			if (c == K_TAB && _nc == 0) {
				w_calculator(WiLine(SCREEN)+2,WiCol(SCREEN), ndec, dato,
						length);
				c = K_ENTER;
				break;
			}

			if (c == '.' && dot == 0 && ndec != 0) {
				dot = 1;
				_nd = 0;
				if (_nc==0) dato[_nc++] = '0';
			} 
			else if (c==K_BACKSPACE && (dot || _nc)) {
				if (dot) { 
					if (_nd) 
						_nd--;
					else 
						dot = 0;
				}
				else --_nc;
			}
			else if (isdigit(c) && ((dot && _nd < ndec) || 
									(!dot && _nc+ndec<ndigits+minus))) { 
					if (dot) {
						dato[_nd+_nc] = c;
						_nd++;
					}
					else {
						if (_nc==0) minus = 0;
						dato[_nc++] = c;
					}
			}
			else if (c=='-' && !_nc && (opt & WGET_SIGNED)) {
				dato[_nc++] = c;
				minus = 1;
			}
			else {
				w_beep();
				continue;
			}

			if (_nc==0 && dot==0) 
				dato[0] = '\0';
			else {
				for (i = _nd; i < ndec; i++)
					dato[_nc+i] = '0';
				dato[_nc+i] = '\0';
			}

			/* Remove leading zeroes */
			while (dato[minus] == '0' && _nc > minus+1) {
				strcpy((char*)(dato+minus), (char*)(dato+minus+1));
				_nc--;
			}

			WiMoveTo(_row, _col);

			_DisplayNumField(fmt, olength, ndec, (char *) dato, opt);
			desp = _col + (dato[0] ? (olength - 1 - 
								 (dot ? ndec-1-(_nd==ndec ? _nd-1 : 
															_nd)  : 
										(ndec ? ndec+1 : 0   )))  :
								  0);
			WiMoveTo(_row, desp);

			if (opt & WGET_AUTOENTER && _nc+ndec == ndigits+minus &&
										_nd == ndec) {
				c = K_ENTER;
				break;
			}
		}


		if (c==K_CLEAR_FLD) {
			if (opt & WGET_DSPONLY)
				w_beep();
			else
				dato[0] = '\0';
		}

		/*
			Should we validate check digit?
		*/
		if (c==K_CLEAR_FLD || c==K_IGNORE || c==K_END || dato[0] == '\0') {
			cdigitOK = true;
			/*
				If check digit, delete it before leaving the field
			*/
			if (opt & (WGET_CDIGIT|WGET_CDIGIT_DASH|WGET_CDIGIT_SLASH) &&
					dato[0] != '\0')
				dato[strlen((char *)dato)-1] = '\0';
		} else {
			if (opt & (WGET_CDIGIT|WGET_CDIGIT_DASH|WGET_CDIGIT_SLASH)) {
				char cdigit;
				int ult = strlen((char *)dato)-1;
				/*
					Obtain all characters except the last one.
				*/
				cdigit = dato[ult];
				/*
					Chop off the check digit, if check fails then
					we restore it, else we leave it truncated.
				*/
				dato[ult] = '\0';
				/*
					Verify the check digit.
				*/
				if (!(cdigitOK = ((int)(cdigit-'0') ==
						CheckDigit(StrToL((char *) dato))))) {
					dato[ult] = cdigit;
					w_beep();
					/*
						Display the corresponding error message here.
					*/
				}
			}
		}
	} while (!cdigitOK);
	WiMoveTo(_row, _col);
	return c;
}

/*
	Display the numeric field depending on the check digit option
*/
static void _DisplayNumField(char* fmt, short olength, short ndec, char* dato, short opt)
{
	if (opt & (WGET_CDIGIT_SLASH|WGET_CDIGIT_DASH)) {
		if (dato[0] != '\0') {
			char save;
			int ult = strlen(dato)-1;

			save = dato[ult];
			dato[ult] = '\0';
			(void) WiPrintf(fmt,olength-2,olength-2,ndec,dato);
			(void) WiPrintf((opt & WGET_CDIGIT_DASH) ? (char *)"-%c" : (char *)"/%c",
					save);
			dato[ult] = save;
		} else {
			(void) WiPrintf(fmt,olength-2,olength-2,ndec,dato);
			(void) WiPrintf((opt & WGET_CDIGIT_DASH) ? (char *)"- " : (char *)"/ ");
		}
	} else
		(void) WiPrintf(fmt,olength,olength,ndec,dato);
}

static void _MoveNum(UnChar c, UnChar *buff, int len, short *desp,
		short olength, UShort _row, UShort _col, short *dsp)
{
	switch (c) {
	case '<':
		if (*dsp > (short) _col) 
			(*dsp)--;
		else {
			if (*desp) 
				(*desp)--;
			else
				w_beep();
		}
		break;
	case '>':
		if (*dsp < _col+olength-1) 
			(*dsp)++;
		else {
			if (len - *desp > olength) 
				(*desp)++;
			else
				w_beep();
		}
		break;
	}
	WiMoveTo(_row, _col);
	(void) WiPrintf("%*.*s", olength, olength, buff + *desp);
	WiMoveTo(_row, *dsp);
}

static int _WGetStr(UnChar *buff, short length, short olength,
		unsigned opt, char *tstmask, char *omask, UShort _row,
		UShort _col)
{
	register UnChar c, *pbuff, *pebuff, *endbuff;
	bool end = false, edit_mode = false;
	bool _hab_susp = hab_susp;
	short _nc =0;

	if (*omask == 0) {
		omask	= NULL;
		tstmask = NULL;
	}
	else if (omask) {
		omask[0] = 'L';
		length = (short)strlen(tstmask);
	}

	_Display(buff, (UnChar*) omask, (UShort) olength, (UShort) olength, 
			 _row, _col, true);
	WiMoveTo(_row, _col);

	pebuff  = pbuff   = buff;
	endbuff = buff + length;

	do {

		_nc = (short) (pbuff - buff);
		if (_hab_susp) hab_susp = ! _nc;
		c = WiGetc();
		hab_susp = _hab_susp;
		if (suspend || signal_sent) return c;

		if ( !edit_mode && iskey(c)) break;

		switch (c) {
		case K_END :
		case K_IGNORE :
		case K_UNDO :
		case K_HELP :
			end = true;
			continue;
		case K_CURS_LEFT :
			if (pbuff > buff)
				--pbuff;
			else c = 0;
			break;
		case K_CURS_RIGHT :
			if (pbuff < pebuff) 
				++pbuff;
			else c = 0;
			break;
		case K_DEL :
			if (pbuff == pebuff || (opt&WGET_DSPONLY)) 
				break;
			if ( ! _TstMask(true, (UnChar*)tstmask, buff, pbuff, 
							pebuff)) c = 0;
			else {
				CopyStringCharacters((char*)pbuff, (char*)(pbuff+1));
				//(void) strcpy((char*)pbuff, (char*)(pbuff+1)); //ARREGLO BUG 1781
				pebuff--;
				if (omask) omask[0] = 'L';
				_Display(buff, (UnChar*)omask, olength, olength, 
						 _row, _col, true);
				if (omask) omask[0] = 'l';
			}
			break;

		case K_INS :
			if (pbuff == pebuff || (opt&WGET_DSPONLY)) 
				break;
			if ( ! _TstMask(false, (UnChar*)tstmask, buff, pbuff, 
							pebuff)) c = 0;
			else {
				UnChar *paux;
				for (paux = endbuff - 1; paux > pbuff; paux--)
					paux[0] = paux[-1];
				pbuff[0] = ' ';
				if (pebuff < endbuff) pebuff++;
				_Display(buff, (UnChar*)omask, olength, olength, 
						 _row, _col, true);
			}
			break;
		case K_BACKSPACE :
			if (pbuff <= buff || (opt&WGET_DSPONLY))
				c = 0;
			else {
				if (pbuff == pebuff) {
					*--pebuff = 0;
					--pbuff;
				}
				else {
					if (omask &&
						! (c=TstChrMask(' ',
								tstmask[(short) (pbuff-buff-1)], (char *) buff)))
						break;
					*--pbuff = ' ';
				}
			}
			break;

		default :
			if (iskey(c)) {
				assert(pbuff);
				if (omask == NULL || (pbuff == buff && *pbuff == 0) ||
					TstChrMask(c, tstmask[(short) (pbuff-buff)], (char *) buff)) {
					end = true;
					continue;
				}
				c = 0;
			}
			if (omask && (edit_mode || c != ' ')) {
				c = TstChrMask(c, tstmask[(short) (pbuff-buff)], (char *) buff);
			}
		}		
		if (c==0) {
			w_beep();
			continue;
		}

		if (isprint(c) && pbuff < endbuff) {
			if (!edit_mode) {
				if (c != ' ') {
					if (opt&WGET_DSPONLY)
						w_beep();
					else {
						UnChar *p;
						*pbuff++ = c; 
						for(p = pbuff;p < endbuff;*p++ = 0);
					}
				} 
				pebuff = buff + strlen((char*)buff);
				/*(void) sprintf(buff, "%-*s", length, buff);*/
				_Display(buff, (UnChar*)omask, olength, olength, 
						 _row, _col, true);
				if (omask) omask[0] = 'l';
				WiCursor(true);
				edit_mode = true;
			} else if (opt&WGET_DSPONLY)
				w_beep();
			else {
				*pbuff++ = c; 
				if (pbuff > pebuff) pebuff++;
			}
		}
		_Display(buff, (UnChar*)omask, 
				 (UShort)(pbuff-buff+(pbuff!=endbuff)), olength, _row, 
				 _col, false);
		if (opt & WGET_AUTOENTER && pbuff==endbuff) {
			c   = K_ENTER;
			break;
		}
	} while (!end);

	if (c==K_CLEAR_FLD) { 
		if (opt & WGET_DSPONLY) w_beep();
		else buff[0] = '\0';
	}
	else if ( edit_mode ) {

		/* Remove trailing blanks */

		pbuff = buff + strlen((char*)buff) - 1;
		while (pbuff >= buff && *pbuff == ' ')
			pbuff--;
		pbuff[1] = '\0';
	}
	WiMoveTo(_row, _col);
	if (omask) {
		omask[0] = 'l';
		if ( MaskChkDig(omask) && strlen((char *)buff) == MskDLen(omask)+1) 
			buff[strlen((char *)buff)-1] = 0;
	}
	return c;
}

static bool _TstMask(bool del_flg, UnChar *tstmask, UnChar *buff,
		UnChar *pbuff, UnChar *pebuff)
{
	if ( ! tstmask ) return true;

	tstmask += pbuff - buff;
	if (del_flg)
		pbuff++;
	else if ( ! TstChrMask(' ', *tstmask++, (char *) buff) ) return false;

	while (pbuff < pebuff) {
		UnChar c = TstChrMask(*pbuff, *tstmask++, (char *) buff);
		if (! c ) return false;
		*pbuff++ = c;
	}
	return true;
}

static void _Display(UnChar *buff, UnChar *omask, UShort len,
		UShort olen, UShort _row, UShort _col, bool first)
{
	UnChar bufaux[256], *field=bufaux;
	int offset=0, dsp=len;
	
	if (len > olen) {
		dsp = olen;
		offset = len-olen;
	}

	if ( ! omask ) 
		field = buff;
	else {
		int i,j, chkdig = 0;

		/* In this routine The mask does not need the check digit
		 * because it is already in the datum (buff). So we delete
		 * the check digit if it is there
		 */
		if (MaskChkDig((char *)omask)) { chkdig = 1; omask[strlen((char *)omask)-1] = '_'; }

		omask[0] = 'L';
		/*sprintf((char*)bufaux, "%'*'s", omask, buff); */
		StrMask((char *)bufaux, (char *)omask, (char *)buff);
		omask[0] = 'l';
		if (!first) {
			for (i=0, j=offset+dsp; omask[i+1] && i<j; i++) {
				if (omask[i+1]!='_' && omask[i+1]!='#') {
					if (offset || dsp==olen) offset++;
					else dsp++;
					j++;
				}
			}
		}

		/* If the mask has check digit, restore it */
		if (chkdig) 
			omask[strlen((char *)omask)-1] = TST_OCHK_DIG;
	}

	WiMoveTo(_row, _col);
	(void) WiPrintf("%-*.*s\b", dsp, dsp, field+offset);
}

static bool _VTime(register char *s, register int n, int totlen)
{
	register int i;
	int hours, minutes;

	if (s[0] == 0)  return true;

	for (i = 0; i <= n; i++) {
		
		if (!isdigit(s[i])) return false;
		
		switch (i) {
		case 0 :
			if (s[0] > '2') return false;
			break;
		case 1 : 
			hours = (s[0]-'0')*10 + s[1]-'0';
			if (hours<0 || hours>23) return false;
			break;
		case 2 :
		case 4 :
			if (s[i] > '5') return false;
			break;
		case 3 :
		case 5 :
			minutes = (s[i-1]-'0')*10 + s[i]-'0';
			if (minutes<0 || minutes>59) return false;
			break;
		}
	}
	return true;
}

static int _WGetTime(UnChar *buff, short length, UShort opt,
		UShort _row, UShort _col)
{
	register UnChar c;
	bool end;
	bool _hab_susp = hab_susp;
	short _nc =0;

	buff[length] = '\0';
	WiPrintf("%2.2s:%2.2s%.1s%*s", buff, *buff?buff+2:buff,
						    length-4 ? ":" : "",
						    length-4, *buff?buff+4:buff);
	WiMoveTo(_row, _col);

	end = false;
	while (!end) {

		if (_hab_susp) hab_susp = ! _nc;
		c = WiGetc();
		hab_susp = _hab_susp;
		if (suspend || signal_sent) return c;

		switch (c) {
		case K_END :
		case K_IGNORE :
		case K_UNDO :
		case K_HELP :
			end = true;
			continue;
		case K_CLEAR_FLD :
			/* Not valid in display only			*/
			if (opt & WGET_DSPONLY) break;
			buff[0] = buff[2] = '\0';
			if (length == 6) buff[4] = '\0';
			_nc = 0;
			WiMoveTo(_row, _col);
			WiPrintf("  :  %.1s%*s", length-4 ? ":" : "", length-4, "");
			WiMoveTo(_row, _col);
			continue;
		case K_BACKSPACE :
			if (_nc) {
				if (_nc == 2 || (_nc==4 && length == 6)) WiPutc('\b');
				if (_nc != length) WiPutc('\b');
				WiPuts(" \b");
				buff[--_nc] = '\0';
				continue;
			}
			break;
		default :
			if (iskey(c) && _VTime((char*)buff, length - 1, length)) {
				end = true;
				continue;
			}
		}		

		if (isdigit(c) && _nc < length && !(opt&WGET_DSPONLY)) {
			buff[_nc] = c; 
			if (_VTime((char*)buff, _nc, length)) {
				_nc++; 
				WiPutc(c);
				if (_nc==2 || (_nc==4 && length == 6)) WiPutc(':');
				else if (_nc == length) {
					WiPutc('\b');
					if (opt & WGET_AUTOENTER) {
						c = K_ENTER;
						break;
					}
				}
				continue;
			}
		}
		w_beep();
	}

	WiMoveTo(_row, _col);
	return c;
}

static int _WGetFloat(UnChar *dato, short length, UShort opt, UShort _row,
		UShort _col)
{
	register UnChar c;
	short pos_pto_dec = ERR;
	bool e_exp	 = true,
		 pto_dec = true;
	bool _hab_susp = hab_susp;
	bool sgn = !!(opt & WGET_SIGNED);
	short _nc =0, _length = length - sgn, minus = 0;

	WiMoveTo(_row, _col);
	FmtFloat((char *) dato, StrToF((char*) dato), length);
	WiPrintf("%*s\b", length, dato);
	WiMoveTo(_row, _col);

	forever {

		if (_hab_susp) hab_susp = !_nc;
		c = tolower(WiGetc());
		hab_susp = _hab_susp;
		if (suspend || signal_sent) return c;

		if ((!iskey(c) || c == K_BACKSPACE) &&
				opt & WGET_DSPONLY)
			c = 0;

		if (isdigit(c) && _nc == 1 && dato[0] == '0') _nc = 0;

		if (c == K_BACKSPACE || !iskey(c) || c == K_CLEAR_FLD) {

			if (c == K_BACKSPACE) {
				if (!_nc) continue;
				--_nc;
				if (_nc == 0 && dato[0] == '-') minus = 0;
				if (dato[_nc] == '.') pto_dec = 1;
				if (dato[_nc] == 'e') e_exp = 1;
			} 
			else {
				if (_nc >= _length + minus) {
					w_beep();
					continue;
				}
				if (c == '-') {
					if (_nc == 0 && sgn) {
						dato[_nc++] = c;
						minus = 1;
					}
					else if (_nc > 0 && dato[_nc - 1] == 'e') {
						dato[_nc++] = c;
					}
					else {
						w_beep();
						continue;
					}
				}
				else if (isdigit(c))
					dato[_nc++] = c;
				else if (c == 'e') {
					if (!e_exp || !_nc) continue;
					dato[_nc++] = c;
					e_exp = false;
				}
				else if (c == '.') {
					if (!pto_dec || !e_exp) continue;
					if (!_nc) dato[_nc++] = '0';
					pos_pto_dec = _nc;
					dato[_nc++] = '.';
					pto_dec = false;
				}
				else {
					w_beep();
					continue;
				}
			}

			dato[_nc] = '\0';

			if (opt & WGET_AUTOENTER && _nc == _length + minus) {
				c = K_ENTER;
				break;
			}

			WiMoveTo(_row, _col);
			if (!pto_dec) dato[pos_pto_dec] = DECPOINT();
			WiPrintf("%*s\b", length, dato);
			if (!pto_dec) dato[pos_pto_dec] = '.';
		}
		else break;
	}

	WiMoveTo(_row, _col);
	FmtFloat((char *) dato, StrToF((char*) dato), length);
	WiPrintf("%*s\b", length, dato);
	WiMoveTo(_row, _col);
	return c;
}

static int _WGetBool(UnChar *buff, short length, UShort opt, UShort _row,
		UShort _col)
{
	UnChar c;
	static String yes, no;

	if (yes == NULL_STRING) {
		yes = Boolean(true).format();
		no  = Boolean(false).format();
	}

	forever {
		WiMoveTo(_row, _col);
		(void) WiPrintf("%-*.*s\b", length, length,
						  *buff == '1' ? toCharPtr(yes)
						: *buff == '0' ? toCharPtr(no) : "");
		WiMoveTo(_row, _col);

		c = (UnChar) toupper(WiGetc());
		if (suspend || signal_sent) break;

		if ((!iskey(c) || c == K_BACKSPACE) 
					   && opt & WGET_DSPONLY) c = 0;

		if ((char) c == no[0]) 
			*buff = '0';
		else if ((char) c == yes[0]) 
			*buff = '1';
		else if (c == K_BACKSPACE)
			*buff = '\0';
		else if (iskey(c))
			break;
		else {
			w_beep();
			continue;
		}

		if (opt & WGET_AUTOENTER) {
			c = K_ENTER;
			break;
		}
		
	}
	return c;
}

static int MaskChkDig(char *m)
{
	return strrchr(m, TST_OCHK_DIG) != NULL;
}

static int MskDLen(char *m)
{
	int n = 0;
	while (*m) { if (*m == '_') n ++; m++; }
	return n;
}

//Funciones auxiliares

void CopyStringCharacters(char* destiny, const char* provenance)
{
	while(*destiny++=*provenance++);
}
