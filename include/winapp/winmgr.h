/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: winmgr.h,v 1.1.1.1 1996/04/16 18:51:44 gustavof Exp $
*
* DESCRIPTION
* Class definitions for window manager type.
*********************************************************************/

#ifndef WINMGR_H
#define WINMGR_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

#ifndef STR_H
#include <ifound/str.h>
#endif

// dimension del buffer de comunicacion
#define WM_COM_BUFF_SIZE  1024

// longitud del buffer para flush
#define BUFFERSIZE		256

// tamano del TAB
#define	TAB_SIZE		4

// codificacion interna para c/u de los comandos
enum {
	WM_DUMMY,
	WM_STOP,
	WM_WICREATE,
	WM_WIDELETE,
	WM_WIDELETEALL,
	WM_WISWITCHTO,
	WM_WIREFRESH,
	WM_WIREDRAW,
	WM_WISETBORDER,
	WM_WISTATUSLINE,
	WM_WIGETATTR,
	WM_WIORGCOL,
	WM_WIORGROW,
	WM_WILINE,
	WM_WICOL,
	WM_WIWIDTH,
	WM_WIHEIGHT,
	WM_WIINCHAR,
	WM_WIINATTR,
	WM_WIGETTAB,
	WM_WISETTAB,
	WM_WIPARENT,
	WM_WIFLUSH,
	WM_WIGETC,
	WM_WIGETS,
	WM_WIKEYHELP,
	WM_WIINTERRUPTS,
	WM_WICURRENT,
	WM_WICREATEPRO,
	WM_WIDELETEPRO,
	WM_WISETTTY,
	WM_WIRESETTTY,
	WM_WIGETFIELD,
	WM_WIAPLHELP,
	WM_WISETAPLHELP,
	WM_WIPROCESS,
	WM_WISETRAW,
	WM_WIKEYREADY,
	WM_WISETSERVICES,
	WM_WIMOVE,
	WM_WIRESIZE,
	WM_WISERVICE,

	LASTCOMMAND,			// dummy para verificar el rango
};

// programa para simulacion del fg
#define RETSHELL "retshell"

/* define para resultado del mensajes interrumpidos (ipc) */
#define INTPTS -1234

class WinMgr
{
	friend class Window;

// private:

	static WinMgr *defWm;		// window manager por default

	Window *rootWindow;			// root window del wm
	Window *getRootWindow();	// retorna la window root

#ifdef HAVE_IPC
	int  snd_channel;			// descriptores de los 
	int  rcv_channel;			// canales de comunicacion
#else
	int paramNum;				// numero de parametro
#endif

	char *index;				// indice del buffer
	char buffer[WM_COM_BUFF_SIZE];	// buffer para push y pop

	short  ncmd;				// comando que se esta ejecutando

	int  my_pid;				// contiene el nro de proceso

	// lista de wm
	static WinMgr *head;		// puntero al primer wm
	WinMgr *next;				// puntero al proximo wm
	void insNode();				// inserta el wm en la lista
	void delNode();				// borra el wm de la lista

	// lista de window
	Window *headwin;

	// window corriente
	short currwin;

	void connect(const char *tty, const char *module, 
								  const char *version);
	void disConnect(const char *tty);
	int  openChannel(const char *tty, int c, bool create_it);
	int  openSem(const char *tty, bool create_it);
	void rmSem(int semid);
	void rmChannel(int channel);
	int  receiveMsg(int msgid, char *msg, int msgsize, 
						   long msgtype);
	int  sendMsg(int msgid, char *_msg, int msgsize);
	short  createProces(int pid, const char *module, const char *name, 
							 const char *version);
	short  deleteProces(int pid);
	int execCommand(char *titulo, char *cmd);
	void interrupts(bool flag);

	void refreshCurrWin();

	void setServices(bool value);
	bool execService(const String &name);
	void setRaw(Int value);
                                    
    WinMgr(const WinMgr&)  ;                                
    void operator=(const WinMgr&)  ;
    
public:

	// constructores y destructores
	WinMgr(String tty, String module, String version);
	~WinMgr();

	void refresh();
	void redraw();

	void initFunc(short);

	void push(short  );
	void push(UnChar );
	void push(long   );
	void push(UnChar*);
	void push(String );

	void pop(short  &i);
	void pop(UnChar &c);
	void pop(long   &l);
	void pop(UnChar  *);
	void pop(String &s);

	void execCmd();

};

#endif
