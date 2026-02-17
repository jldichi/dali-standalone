/********************************************************************
* ORGLAND Ltd. IDEAFIX for MS Windows.
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* MODULE        : @(#)iwdispat.h
* VERSION       : 1.0 alpha
* DATE          : 11-26-93 00:42am
* DESCRIPTION   : IdeaFix Windows dispatcher
*
********************************************************************/


#ifndef _INC_IWDISPAT
#define _INC_IWDISPAT

#ifndef _INC_IWWIN
#include <mswi/iwwin.h>
#endif

// Dispatcher window of IDEAFIX

class IWDispatcher {
static   window      FindParCom(window,window);
static   void        RecTree(window,window);
static   BOOL        Ante(window,window);
static   void        DeleteWin(IWWindow*,BOOL);

       IWWindow ** aOpenIWWnds;
       void        FreeVec(IWWindow * lpWnd);
       window      GetNewWd(IWWindow* lpNewWin);

public:
     IWDispatcher();
     ~IWDispatcher();

      BOOL      GetIWWindow(window, IWWindow *&);
      IWWindow* GetIWWindow(window wd)  {return aOpenIWWnds[wd]; }
      window    AddWindow(window,IWWindow*);
      void      DelWindow(window wd,BOOL bDestroy=TRUE);
      void      DelWindow(HWND);
      void      DelAll();
      window    SwitchTo(window);
       BOOL        SetCurrWin(window p);
};

extern IWDispatcher* wdispatcher; // global instance of IWDispatcher
#endif
