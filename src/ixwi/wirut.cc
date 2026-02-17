/*******************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: wirut.cc,v 1.9 2010/09/17 20:49:51 leonardol Exp $
*
*
* DESCRIPTION:
*      Descripcion del proposito general del modulo
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*
*********************************************************************/

#include <string.h>
#ifdef __hpux
#include <unistd.h> // Para el sleep() del patch a Telecom con Retroactivos
#endif

#include <ideafix.h>
#include <cfix.h>
#include <ideafix/priv/gndefs.h>
#include <cracker/pserconn.h>


#include <ideafix/priv/wmcom.h>

#ifdef IDEAWIN

void WiInterrupts(int flag)	{}
void WiSettty(void)			{}
void WiResettty(void)		{}
int WiCreateProces(int pid, char *module, char *name, char *version)
							{  return 0;    }
int WiDeleteProces(int pid)
							{  return 0;    }
int WiProcess(int stopping)
							{  return 0;    }
#else
#	ifdef HAVE_IPC
#		define BUFFERSIZE WM_COM_BUFF_SIZE-16
#	else
#		define BUFFERSIZE 64
#	endif

static char buff[BUFFERSIZE+1] = {(char)NULL};
static char *pbuff = buff;

window WiCreate(int wi, int f_org, int c_org, int nfils, int ncols,
		attr_type border, char *label, attr_type backgr)
{
	if (crackerClient()) {
		prServerConn(PrCmd::wiCreate) << (Int)wi <<  (Int)f_org << (Int)c_org
									  << (Int)nfils << (Int)ncols
								  	  << (Int)border << label << (Int)backgr;
		return prServerConn.result();
	} else {
		/* cargo los parametros para la rutina _ */
		wm_par[0].i = (short)wi;
		wm_par[1].i = (short)f_org;
		wm_par[2].i = (short)c_org;
		wm_par[3].i = (short)nfils;
		wm_par[4].i = (short)ncols;
		wm_par[5].l = border;
		wm_par[6].l = backgr;
		wm_par[7].s = (UChar *) label;

		(*ExecWm)(WM_WICREATE);
		return wm_par[0].i;
	}
}

void WiDelete(int wi)
{
	if (crackerClient()) {
		prServerConn(PrCmd::wiDelete) << (Int)wi;
		prServerConn.flush();
	} else {
		wm_par[0].i = (short)wi;
		(*ExecWm)(WM_WIDELETE);
	}
}

void WiDeleteAll(void)
{
	if (crackerClient()) 
		prServerConn(PrCmd::wiDeleteAll).flush();
	else
		(*ExecWm)(WM_WIDELETEALL);
}

window WiSwitchTo(window n)
{
	if (crackerClient()) {
		prServerConn(PrCmd::wiSwitchTo) << (Int)n;
		return prServerConn.result();
	} else {
		wm_par[0].i = (short)n;

		(*ExecWm)(WM_WISWITCHTO);
		return wm_par[0].i;
	}
}

void WiRefresh(void)
{
	if (!crackerClient())
		(*ExecWm)(WM_WIREFRESH);
	else {	
		// Sending a '\f' character implies a WiRefresh
		if (prServerConn.currCmd() == PrCmd::wiPutc)
				prServerConn.putC('\f');
		else if (prServerConn.lastCmd() != PrCmd::wiRefresh ) 
				prServerConn(PrCmd::wiRefresh);
		else 
			return;
		prServerConn.flush();
	}
}


void WiRedraw(void)
{
	if (crackerClient())
		prServerConn(PrCmd::wiRedraw).flush();
	else
		(*ExecWm)(WM_WIREDRAW);
}

void WiSetBorder(attr_type border, char *label)
{
	if (crackerClient()) {
		prServerConn(PrCmd::wiSetBorder) << Int(border) << label;
		prServerConn.flush();
	} else {
		wm_par[0].l = border;
		wm_par[1].s  = (UChar *) label;

		(*ExecWm)(WM_WISETBORDER);
	}
}

void WiStatusLine(bool f)
{
	if (crackerClient()) {
		prServerConn(PrCmd::wiStatusLine) << (Int)f;
		prServerConn.flush();
	} else {
		wm_par[0].i = (short)f; 

		(*ExecWm)(WM_WISTATUSLINE);
	}
}

attr_type WiGetAttr(void)
{
	if (crackerClient())
		return prServerConn(PrCmd::wiGetAttr).result();
	else {
		(*ExecWm)(WM_WIGETATTR);
		return wm_par[0].l;
	}
}

int WiOrgCol(window wi)
{
	if (crackerClient()) {
		prServerConn(PrCmd::wiOrgCol) << (Int)wi;
		return prServerConn.result();
	} else {
		/* parametro de la rutina */
		wm_par[0].i = (short)wi;

		(*ExecWm)(WM_WIORGCOL);
		return wm_par[0].i;
	}
}

int WiOrgRow(window wi)
{
	if (crackerClient()) {
		prServerConn(PrCmd::wiOrgRow) << (Int)wi;
		return prServerConn.result();
	} else {
		/* parametro de la rutina */
		wm_par[0].i = (short)wi;

		(*ExecWm)(WM_WIORGROW);
		return wm_par[0].i;
	}
}

int WiLine(window wi)
{
	if (crackerClient()) {
		prServerConn(PrCmd::wiLine) << (Int)wi;
		return prServerConn.result();
	} else {
		/* parametro de la rutina */
		wm_par[0].i = (short)wi;

		(*ExecWm)(WM_WILINE);
		return wm_par[0].i;
	}
}

int WiCol(window wi)
{
	if (crackerClient()) {
		prServerConn(PrCmd::wiCol) << (Int)wi;
		return prServerConn.result();
	} else {
		/* parametro de la rutina */
		wm_par[0].i = (short)wi;

		(*ExecWm)(WM_WICOL);
		return wm_par[0].i;
	}
}

int WiWidth(window wi)
{
	if (crackerClient()) {
		prServerConn(PrCmd::wiWidth) << (Int)wi;
		return prServerConn.result();
	} else {
		/* parametro de la rutina */
		wm_par[0].i = (short)wi;

		(*ExecWm)(WM_WIWIDTH);
		return wm_par[0].i;
	}
}

int WiHeight(window wi)
{
	if (crackerClient()) {
		prServerConn(PrCmd::wiHeight) << (Int)wi;
		return prServerConn.result();
	} else {
		/* parametro de la rutina */
		wm_par[0].i = (short)wi;

		(*ExecWm)(WM_WIHEIGHT);
		return wm_par[0].i;
	}
}

UChar WiInChar(int fil, int col)
{
	if (crackerClient()) {
		prServerConn(PrCmd::wiInChar) << Int(fil) << Int(col);
		return prServerConn.result();
	} else {
		/* parametros de la rutina */
		wm_par[0].i = (short)fil;
		wm_par[1].i = (short)col;
		
		(*ExecWm)(WM_WIINCHAR);
		return wm_par[0].u;
	}
}

attr_type WiInAttr(int fil, int col)
{
	if (crackerClient()) {
		prServerConn(PrCmd::wiInAttr) << Int(fil) << Int(col);
		return prServerConn.result();
	} else {
		/* parametros de la rutina */
		wm_par[0].i = (short)fil;
		wm_par[1].i = (short)col;

		(*ExecWm)(WM_WIINATTR);
		return wm_par[0].l;
	}
}

int WiGetTab(void)
{
	if (crackerClient()) 
		return prServerConn(PrCmd::wiGetTab).result();
	else {
		(*ExecWm)(WM_WIGETTAB);
		return wm_par[0].i;
	}
}

void WiSetTab(int n)
{
	if (crackerClient()) {
		prServerConn(PrCmd::wiSetTab) << (Int)n;
		prServerConn.flush();
	} else {
		/* parametros de la rutina */
		wm_par[0].i = (short)n;

		(*ExecWm)(WM_WISETTAB);
		return;
	}
}

window WiParent(window wi)
{
	if (crackerClient()) {
		prServerConn(PrCmd::wiParent) << (Int)wi;
		return prServerConn.result();
	} else {
		/* parametros de la rutina */
		wm_par[0].i = (short)wi;

		(*ExecWm)(WM_WIPARENT);
		return wm_par[0].i;
	}
}

void WiFlush(void)
{
	if (crackerClient())
		return;
		
	union _WmPar save;

	if (pbuff == buff) return;
	*pbuff = '\0';
	pbuff  = buff;

	/* parametros de la rutina */

	save = wm_par[0];
	wm_par[0].s = (UChar *) buff;

	(*ExecWm)(WM_WIFLUSH);
	wm_par[0] = save;
}

void WiPutc(UChar c)
{
	if (crackerClient()) {
		if (prServerConn.currCmd() != PrCmd::wiPutc)
			prServerConn(PrCmd::wiPutc);
		if (c)
			prServerConn.putC((char)c);
	} else {
		if (pbuff>=buff+BUFFERSIZE)
			WiFlush();
		*pbuff++ = c;
	}
}

UChar WiGetc(void)
{
	if (crackerClient())
		return prServerConn(PrCmd::wiGetC).result();
	else {
		(*ExecWm)(WM_WIGETC);
		return wm_par[0].u;
	}
}

UChar *WiGets(UChar *s)
{
	if (crackerClient()) {
		String aux;
		prServerConn(PrCmd::wiGets) << (char*)s;
		prServerConn.flush();
		prServerConn >> aux;	
		strcpy((char*)s, toCharPtr(aux));
		return s;
	} else {
		/* parametros de la rutina */
		wm_par[0].s = s;

		(*ExecWm)(WM_WIGETS);
		return (UChar *) strcpy((char*)s, (char*)wm_par[0].s);
	}
}

UChar *WiKeyHelp(UChar key, UChar *s)
{
	if (crackerClient()) {
		String aux;
		prServerConn(PrCmd::wiKeyHelp) << (Int)key;
		prServerConn.flush();
		prServerConn >> aux;	
		strcpy((char*)s, toCharPtr(aux));
		return s;
	} else {
		wm_par[0].u = key;
		wm_par[1].s = s;

		(*ExecWm)(WM_WIKEYHELP);
		return (UChar *) strcpy((char*)s, (char*)wm_par[0].s);
	}
}

void WiInterrupts(int flag)
{
	if (crackerClient()) {
		prServerConn(PrCmd::wiInterrupts) << (Int)flag;
		prServerConn.flush();
	} else {
		wm_par[0].i = (short)flag;
		(*ExecWm)(WM_WIINTERRUPTS);
	}
}

window WiCurrent(void)
{
	if (crackerClient())
		return prServerConn(PrCmd::wiCurrent).result();
	else {
		(*ExecWm)(WM_WICURRENT);
		return wm_par[0].i;
	}
}

int WiCreateProces(int pid, char *module, char *name, char *version)
{
	if (crackerClient()) {
		Int perms = 0;
		if (!procAddPerm())
			perms |= A_PERM;
		if (!procDelPerm())
			perms |= D_PERM;
		if (!procUpdatePerm())
			perms |= U_PERM;

#ifdef __hpux
		sleep(1); // Patch para Telecom con Retroactivos
#endif 

	 	prServerConn(PrCmd::wiCreateProces) << (Int)pid << module << name 
	 										<< version << perms;
	 	return prServerConn.result();
	} else { 
		wm_par[0].l = pid;
		wm_par[1].s = (UChar *) module;
		wm_par[2].s = (UChar *) name;
		wm_par[3].s = (UChar *) version;
		(*ExecWm)(WM_WICREATEPRO);
		return wm_par[0].i;
	}
}

int WiDeleteProces(int pid)
{
	if (crackerClient()) {
	 	prServerConn(PrCmd::wiDeleteProces) << (Int)pid;
	 	Int result = prServerConn.result();
	 	return (int) result;
	} else {
		wm_par[0].l = pid;
		(*ExecWm)(WM_WIDELETEPRO);
		return wm_par[0].i;
	}
}

void WiSettty(void)
{
	if (crackerClient())
		prServerConn(PrCmd::wiSettty).flush();
	else
		(*ExecWm)(WM_WISETTTY);
}

void WiResettty(void)
{
	if (crackerClient())
		prServerConn(PrCmd::wiResettty).flush();
	else
		(*ExecWm)(WM_WIRESETTTY);
}

int WiGetField(type t, UChar *datum, UShort opt, short length,
		short olength, short ndec, char *tstmask, char *omask)
{
	if (crackerClient()) {
		String aux;
		prServerConn(PrCmd::wiGetField) << Int(t)  << (char*)datum   << Int(opt)
									<< Int(length) << Int(olength)
									<< Int(ndec)   << tstmask << omask;
		prServerConn >> aux;
		strcpy((char*)datum, toCharPtr(aux));	
		return (int)prServerConn.result();
	} else {
		wm_par[0].i = (short)t;
		wm_par[1].s = datum;
		wm_par[2].i = (short)opt;
		wm_par[3].i = length;
		wm_par[4].i = olength;
		wm_par[5].i = ndec;
		wm_par[6].s = (UChar *) tstmask;
		wm_par[7].s = (UChar *) omask;

		(*ExecWm)(WM_WIGETFIELD);

		strcpy((char*)datum, (char*)wm_par[1].s);
		return wm_par[0].i;
	}
}

void WiAplHelp(void)
{
	if (crackerClient())
		prServerConn(PrCmd::wiAplHelp).flush();
	else
		(*ExecWm)(WM_WIAPLHELP);
}

void WiSetAplHelp(char *mod, char *ver)
{
	if (crackerClient()) {
		prServerConn(PrCmd::wiSetAplHelp) << mod << ver;
		prServerConn.flush();
	} else {
		wm_par[0].s = (UChar *) mod;
		wm_par[1].s = (UChar *) ver;

		(*ExecWm)(WM_WISETAPLHELP);
	}
}

int WiProcess(int stopping)
{
	if (crackerClient()) {
		prServerConn(PrCmd::wiProcess) << (Int)stopping;
		return prServerConn.result();
	} else {
		wm_par[0].i = (short)stopping;

		(*ExecWm)(WM_WIPROCESS);
		return wm_par[0].i;
	}
}

void WiSetRaw(int f)
{
	if (crackerClient()) {
		prServerConn(PrCmd::wiSetRaw) << (Int)f;
		prServerConn.flush();
	} else {
		wm_par[0].i = (short)f; 

		(*ExecWm)(WM_WISETRAW);
	}
}

int WiKeyReady(void)
{
	if (crackerClient()) 
		return prServerConn(PrCmd::wiKeyReady).result();
	else {
		(*ExecWm)(WM_WIKEYREADY);
		return wm_par[0].i;
	}
}

void WiSetServices(bool status)
{
	if (crackerClient()) {
		prServerConn(PrCmd::wiSetServices) << (Int)status;
		prServerConn.flush();
	} else {
		/* parametros de la rutina */
		wm_par[0].i = (short) status;

		(*ExecWm)(WM_WISETSERVICES);
	}
}

bool WiMove(window wi)
{
	if (crackerClient()) {
		prServerConn(PrCmd::wiMove) << (Int)wi;
		return prServerConn.result();
	} else {
		/* parametros de la rutina */
		wm_par[0].i = (short) wi;

		(*ExecWm)(WM_WIMOVE);
		return (bool) wm_par[0].i;
	}
}

bool WiResize(window wi)
{
	if (crackerClient()) {
		prServerConn(PrCmd::wiResize) << (Int)wi;
		return prServerConn.result();
	} else {
		/* parametros de la rutina */
		wm_par[0].i = (short) wi;

		(*ExecWm)(WM_WIRESIZE);
		return (bool) wm_par[0].i;
	}
}

int WiService(char *str)
{
	if (crackerClient()) {
		prServerConn(PrCmd::wiService) << str;
		return prServerConn.result();
	} else {
		wm_par[0].s = (UChar *) str;
		(*ExecWm)(WM_WISERVICE);
		return wm_par[0].i;
	}
}

/* ------------------- IXSCA -------------------- */

SNODE GetSessionWM(int hostId) {
    SNODE sNode;
   /* 
    sNode.hostId = 0;
    sNode.idSession = 0;
    sNode.login = "";
    sNode.password = "";
    sNode.profile = "";
    sNode.expDate = 0;
    sNode.expTime = 0;
    */
    if (crackerClient()) {
	// No tendria que llegar nunca...
        return sNode;
    } else {
        wm_par[0].i = (short) hostId;
        (*ExecWm)(WM_WIGETSESSION);
	
	sNode.hostId = hostId;
	sNode.idSession = wm_par[0].l;
	sNode.idUser = wm_par[1].l;
	strcpy(sNode.login, (char*) wm_par[2].s);
	strcpy(sNode.password, (char*) wm_par[3].s);
	strcpy(sNode.profile, (char*) wm_par[4].s);
	sNode.expDate = 0;
	sNode.expTime = 0;

        return sNode;
    }
}


bool SetSessionWM(long hostId, int idSession, int idUser, char* login, char* password, char* profile, DATE expDate, TIME expTime) {
    if (crackerClient()) {
        // No tendria que llegar nunca...
        return false;
    } else {
	char date[30];
	char time[30];
	
	date[29] ='\0';	
	time[29] ='\0';	
        
	wm_par[0].l = hostId;
        wm_par[1].l = idSession;
	wm_par[2].l = idUser;
        wm_par[3].s = (UChar *) login;
        wm_par[4].s = (UChar *) password;
        wm_par[5].s = (UChar *) profile;
	
	if(expDate == NULL_DATE) {
		expDate = Today();
	}
	if(expTime == NULL_TIME) {
		expTime = Hour();
	}

	DToStr(expDate, date, DFMT_SEPAR | DFMT_YEAR4);
	TToStr(expTime, time, TFMT_SEPAR);
        wm_par[6].s = (UChar *) date;
        wm_par[7].s = (UChar *) time;
	wm_par[8].s = NULL;
        (*ExecWm)(WM_WISETSESSION);
        // retorna 0 o 1;
	return (bool) wm_par[0].i;
    }
}

// Consulta al WM el numero de session para
// la terminal y host
// retorna el numero de sesion
// DA
SNODE getSessionFromWM(long hostId) {

    return GetSessionWM((int) hostId);

}

// Setea en uso la terminal en el WM
// Si login es NULL se asume que el resto de los parametrtos son NULL
// DA
bool setSessionToWM(long hostId, char* login, char* pass, int idUser, char* profile, int idSession, DATE expDate, TIME expTime) {

	bool ret;
	char d[30];
	DToStr(expDate, d, DFMT_SEPAR | DFMT_YEAR4);
	char t[30];
	TToStr(expTime, t, TFMT_SEPAR);
	
	ret = SetSessionWM(hostId, idSession, idUser, login, pass, profile, expDate, expTime);

	return ret;
}


#endif
