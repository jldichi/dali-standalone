#ifndef INC_IWWIN32S_H
#define INC_IWWIN32S_H

#ifdef __NT__

#define WM_COMMAND_CTLID(wParam,lParam) (LOWORD(wParam))
#define WM_COMMAND_HWND(wParam,lParam) ((HWND)(lParam))
#define WM_COMMAND_CODE(wParam,lParam) (HIWORD(wParam))

#define WM_MDIACTIVATE_ACT(wParam,lParam) ((HWND)(lParam))
#define WM_MDIACTIVATE_DEACT(wParam,lParam) ((HWND)(wParam))

#define WM_MENUSELECT_ITEMID(wParam,lParam) (LOWORD(wParam))
#define WM_MENUSELECT_FLAG(wParam,lParam) (HIWORD(wParam))
#define WM_MENUSELECT_HMENU(wParam,lParam) ((HMENU)(lParam))

#define WM_CHARTOITEM_CHAR(wParam,lParam) (HIWORD(wParam))
#define WM_CHARTOITEM_HWND(wParam,lParam) ((HWND)(lParam))
#define WM_CHARTOITEM_CARET(wParam,lParam) (LOWORD(wParam))

#define WM_VKEYTOITEM_VKEY(wParam,lParam) (HIWORD(wParam))
#define WM_VKEYTOITEM_HWND(wParam,lParam) ((HWND)(lParam))
#define WM_VKEYTOITEM_CARET(wParam,lParam) (LOWORD(wParam))

#define WM_HSCROLL_CODE(wParam,lParam) (LOWORD(wParam))
#define WM_HSCROLL_HWND(wParam,lParam) ((HWND)(lParam))
#define WM_HSCROLL_POSITION(wParam,lParam) (HIWORD(wParam))

#define WM_VSCROLL_CODE(wParam,lParam) (LOWORD(wParam))
#define WM_VSCROLL_HWND(wParam,lParam) ((HWND)(lParam))
#define WM_VSCROLL_POSITION(wParam,lParam) (HIWORD(wParam))

#define MDISetMenu(hWnd,fRefresh,hMenuFrame,hMenuWindow) \
((HMENU)((fRefresh)?SendMessage((hWnd),WM_MDIREFRESHMENU,0,0):\
SendMessage((hWnd),WM_MDISETMENU,(WPARAM)(hMenuFrame),(LPARAM)(hMenuWindow))))

#define EditSetSelection(hWnd,fScroll,SelStart,SelEnd) \
(SendMessage((hWnd),EM_SETSEL,(SelStart),(SelEnd)),!(fScroll)?\
SendMessage((hWnd),EM_SCROLLCARET,0,0):0)

#define DlgItemEditSetSelection(hWnd,CtlID,fScroll,SelStart,SelEnd) \
(SendDlgItemMessage((hWnd),(CtlID),EM_SETSEL,(SelStart),(SelEnd)),!(fScroll)?\
SendDlgItemMessage((hWnd),(CtlID),EM_SCROLLCARET,0,0):0)

#define far
#define _far
#define __far
#define __cs
#define __ss

#define HTASK HANDLE
#define GetCurrentTask() GetModuleHandle(NULL)

#define GetWindowID(hWnd) GetWindowLong((hWnd),GWL_ID)

#else /* !__NT__ */

#define WM_COMMAND_CTLID(wParam,lParam) (wParam)
#define WM_COMMAND_HWND(wParam,lParam) ((HWND)LOWORD(lParam))
#define WM_COMMAND_CODE(wParam,lParam) (HIWORD(lParam))

#define WM_MDIACTIVATE_ACT(wParam,lParam) ((HWND)LOWORD(lParam))
#define WM_MDIACTIVATE_DEACT(wParam,lParam) ((HWND)HIWORD(lParam))

#define WM_MENUSELECT_ITEMID(wParam,lParam) (wParam)
#define WM_MENUSELECT_ITEM_HMENU(wParam,lParam) ((HMENU)(wParam))
#define WM_MENUSELECT_FLAG(wParam,lParam) (LOWORD(lParam))
#define WM_MENUSELECT_HMENU(wParam,lParam) ((HMENU)HIWORD(lParam))

#define WM_CHARTOITEM_CHAR(wParam,lParam) (wParam)
#define WM_CHARTOITEM_HWND(wParam,lParam) ((HWND)LOWORD(lParam))
#define WM_CHARTOITEM_CARET(wParam,lParam) (HIWORD(lParam))

#define WM_VKEYTOITEM_VKEY(wParam,lParam) (wParam)
#define WM_VKEYTOITEM_HWND(wParam,lParam) ((HWND)LOWORD(lParam))
#define WM_VKEYTOITEM_CARET(wParam,lParam) (HIWORD(lParam))

#define WM_HSCROLL_CODE(wParam,lParam) (wParam)
#define WM_HSCROLL_HWND(wParam,lParam) ((HWND)HIWORD(lParam))
#define WM_HSCROLL_POSITION(wParam,lParam) (LOWORD(lParam))

#define WM_VSCROLL_CODE(wParam,lParam) (wParam)
#define WM_VSCROLL_HWND(wParam,lParam) ((HWND)HIWORD(lParam))
#define WM_VSCROLL_POSITION(wParam,lParam) (LOWORD(lParam))

#define MDISetMenu(hWnd,fRefresh,hMenuFrame,hMenuWindow) \
((HMENU)SendMessage((hWnd),WM_MDISETMENU,(fRefresh),\
MAKELPARAM((hMenuFrame),(hMenuWindow))))

#define EditSetSelection(hWnd,fScroll,SelStart,SelEnd) \
SendMessage((hWnd),EM_SETSEL,(fScroll),MAKELPARAM((SelStart),(SelEnd)))

#define DlgItemEditSetSelection(hWnd,CtlID,fScroll,SelStart,SelEnd) \
SendDlgItemMessage((hWnd),(CtlID),EM_SETSEL,(fScroll),MAKELPARAM((SelStart),(SelEnd)))

#define GetWindowID(hWnd) GetWindowWord((hWnd),GWW_ID)

#define OF_READ READ

#endif /* __NT__ */

#endif

