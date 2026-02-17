/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: drawcomm.cc,v 1.1.1.1 1996/04/16 18:52:49 gustavof Exp $
*
* DESCRIPTION:	Implementation of draw commands
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

// para que genere la entrada dentro de la tabla de comandos
#define INITCOMM

#include <command.h>
#include <drawcomm.h>
#include <editor.h>
#include <dtext.h>
#include <dview.h>

declare(PtrList,DaliView);
declare(PtrListCursor,DaliView);

// ****************************************
// Commands that are used only in DRAW mode
// ****************************************

class DrawStatus {

friend class SetDrawMode;
friend class EndDrawMode;

private:

	UnChar IdeafixToGraf[256];
	UnChar GrafToIdeafix[256];   
	
	enum { GM_SLINE = 1, GM_DLINE, GM_OTHER, GM_DELETE, GM_MOVE } GMode;
	
	UnChar 	other;

public:

	Int getGMode()	{
		return (Int)GMode;
	}

	void setModeSline() {
		GMode = GM_SLINE;
		IdeafixToGraf[C_RARROW]	= 0x1;
		IdeafixToGraf[C_LARROW]	= 0x2;
		IdeafixToGraf[C_UARROW]	= 0x4;
		IdeafixToGraf[C_DARROW]	= 0x8;
	}

	void setModeDline() {
		GMode = GM_DLINE;
		IdeafixToGraf[C_RARROW]	= 0x10;
		IdeafixToGraf[C_LARROW]	= 0x20;
		IdeafixToGraf[C_UARROW]	= 0x40;
		IdeafixToGraf[C_DARROW]	= 0x80;
	}

	void setModeOther(UnChar c = '*') 
	{ 
		GMode = GM_OTHER; 
		other = c; 
	}

	void setModeDelete() 
	{ 
		GMode = GM_DELETE; 
	}

	void setModeMove() 
	{ 
		GMode = GM_MOVE; 
	}

	void compose(DaliView &view, DaliTextCursor &tc, UnChar c1)
	{
		bool ins;

		switch (GMode) {
		case GM_SLINE:
		case GM_DLINE:
			c1 = IdeafixToGraf[c1];
			
			// Separado en dos despues del port a IRIX
			UnChar c2;	c2 = IdeafixToGraf[(UnChar)tc.getChar()];
			UnChar c3;	c3 = GrafToIdeafix[c1|c2];    
			
			if (ins = view.inInsMode())
				view.chgInsMode();
			view.insChar(c3);
			view.cursLeft();
			if (ins)
				view.chgInsMode();
			break;
		case GM_OTHER:
			if (ins = view.inInsMode())
				view.chgInsMode();
			view.insChar(other);
			view.cursLeft();
			if (ins)
				view.chgInsMode();
			break;
		case GM_DELETE:
			if (ins = view.inInsMode())
				view.chgInsMode();
			view.insChar(' ');
			view.cursLeft();
			if (ins)
				view.chgInsMode();
			break;
		case GM_MOVE:
		default:
			break;
		}
	}

	DrawStatus()
	{ 
		GMode 		= GM_MOVE;

		// Char 0 means that there is nothing in that pos. of the view.
		IdeafixToGraf[0] 		= 0;
		IdeafixToGraf[' '] 		= 0;

		// por default entra en modo linea simple
		IdeafixToGraf[C_RARROW]	= 0x1;
		IdeafixToGraf[C_LARROW]	= 0x2;
		IdeafixToGraf[C_UARROW]	= 0x4;
		IdeafixToGraf[C_DARROW]	= 0x8;

		IdeafixToGraf[C_HORIZ]  = 0x3;
		IdeafixToGraf[C_VERT] 	= 0xC;
		IdeafixToGraf[C_LL]     = 0xA;
		IdeafixToGraf[C_BOT]    = 0xB;
		IdeafixToGraf[C_LR]     = 0x9;
		IdeafixToGraf[C_RIGHT]  = 0xD;
		IdeafixToGraf[C_CEN]    = 0xF;
		IdeafixToGraf[C_LEFT]   = 0xE;
		IdeafixToGraf[C_UL]     = 0x6;
		IdeafixToGraf[C_TOP]    = 0x7;
		IdeafixToGraf[C_UR]     = 0x5;

		IdeafixToGraf[C_D_HORIZ]	= 0x30;
		IdeafixToGraf[C_D_VERT]		= 0xC0;
		IdeafixToGraf[C_D_LL]		= 0xA0;
		IdeafixToGraf[C_D_BOT]		= 0xB0;
		IdeafixToGraf[C_D_LR]		= 0x90;
		IdeafixToGraf[C_D_RIGHT]	= 0xD0;
		IdeafixToGraf[C_D_CEN]		= 0xF0;
		IdeafixToGraf[C_D_LEFT]		= 0xE0;
		IdeafixToGraf[C_D_UL]		= 0x60;
		IdeafixToGraf[C_D_TOP]		= 0x70;
		IdeafixToGraf[C_D_UR]		= 0x50;
		IdeafixToGraf[C_DS_RIGHT]	= 0xC1;
		IdeafixToGraf[C_DS_LEFT]	= 0xC2;
		IdeafixToGraf[C_DS_TOP]		= 0x34;
		IdeafixToGraf[C_DS_BOT]		= 0x38;
		
		// Caracteres que no estan mapeados en ideafix:
		IdeafixToGraf[C_BULLET] = 0xFF;

		for (int i = 0; i < 256; i++)
			GrafToIdeafix[i] = C_BULLET;	

		GrafToIdeafix[0]	= 0;
		GrafToIdeafix[0x1]	= C_RARROW;
		GrafToIdeafix[0x2]	= C_LARROW;
		GrafToIdeafix[0x4]	= C_UARROW;
		GrafToIdeafix[0x8]	= C_DARROW;
		GrafToIdeafix[0x3]	= C_HORIZ;
		GrafToIdeafix[0xC]	= C_VERT;
		GrafToIdeafix[0xA]	= C_LL;
		GrafToIdeafix[0xB]	= C_BOT;
		GrafToIdeafix[0x9]	= C_LR;
		GrafToIdeafix[0xD]	= C_RIGHT;
		GrafToIdeafix[0xF]	= C_CEN;
		GrafToIdeafix[0xE]	= C_LEFT;
		GrafToIdeafix[0x6]	= C_UL;
		GrafToIdeafix[0x7]	= C_TOP;
		GrafToIdeafix[0x5]	= C_UR;

		GrafToIdeafix[0x10] = C_RARROW;
		GrafToIdeafix[0x20] = C_LARROW;
		GrafToIdeafix[0x40] = C_UARROW;
		GrafToIdeafix[0x80] = C_DARROW;
		GrafToIdeafix[0x30] = C_D_HORIZ;
		GrafToIdeafix[0xC0] = C_D_VERT;
		GrafToIdeafix[0xA0] = C_D_LL;
		GrafToIdeafix[0xB0] = C_D_BOT;
		GrafToIdeafix[0xB8] = C_D_BOT;
		GrafToIdeafix[0x90] = C_D_LR;
		GrafToIdeafix[0xD0] = C_D_RIGHT;
		GrafToIdeafix[0xF0] = C_D_CEN;
		GrafToIdeafix[0xE0] = C_D_LEFT;
		GrafToIdeafix[0x60] = C_D_UL;
		GrafToIdeafix[0x70] = C_D_TOP;
		GrafToIdeafix[0x78] = C_D_BOT;
		GrafToIdeafix[0x50] = C_D_UR;

		GrafToIdeafix[0xC1] = C_DS_RIGHT;
		GrafToIdeafix[0x45] = C_DS_RIGHT;
		GrafToIdeafix[0x85] = C_DS_RIGHT;
		GrafToIdeafix[0xC2] = C_DS_LEFT;
		GrafToIdeafix[0x46] = C_DS_LEFT;
		GrafToIdeafix[0x86] = C_DS_LEFT;
		GrafToIdeafix[0x34] = C_DS_TOP;
		GrafToIdeafix[0x15] = C_DS_TOP;
		GrafToIdeafix[0x25] = C_DS_TOP;
		GrafToIdeafix[0x16] = C_DS_TOP;
		GrafToIdeafix[0x26] = C_DS_TOP;
		GrafToIdeafix[0x38] = C_DS_BOT;
		GrafToIdeafix[0x19] = C_DS_BOT;
		GrafToIdeafix[0x1A] = C_DS_BOT;
		GrafToIdeafix[0x29] = C_DS_BOT;
		GrafToIdeafix[0x2A] = C_DS_BOT;
		GrafToIdeafix[0x31] = C_HORIZ;
		GrafToIdeafix[0x32] = C_HORIZ;
		GrafToIdeafix[0xC4] = C_VERT;
		GrafToIdeafix[0xC8] = C_VERT;
		GrafToIdeafix[0x13] = C_D_HORIZ;
		GrafToIdeafix[0x23] = C_D_HORIZ;
		GrafToIdeafix[0x4C] = C_D_VERT;
		GrafToIdeafix[0x8C] = C_D_VERT;
	}
};

static DrawStatus drawst;

// clase SetGrafMode
bool SetDrawMode::redo()
{
	idCheck();
	switch (chr) {
	case 'G':
	case ' ':
	case '0': 
		drawst.setModeMove();			
		break;
	case '1':
		drawst.setModeSline();			
		break;
	case '2':
		drawst.setModeDline();			
		break;
	case '3':
		drawst.setModeOther();			
		break;
	case '4':
		drawst.setModeDelete();			
		break;
	}                                

	drawmode = drawst.getGMode();
    cView.value()->setDrawMode(drawmode);

	if ((editmode=Editor::getMode()) != DRAW_MODE) 
		Editor::setMode(DRAW_MODE);

	return true;
}

void SetDrawMode::undo() 
{ 
	idCheck();
	// si ya no estoy en modo draw me voy 
	if (editmode != DRAW_MODE) {
		Editor::setMode(editmode);
		//cView.value()->writeStLine();
		return;
	}
	// sigo en modo draw debo setear el modo enterior
	switch (drawmode) {
	case DrawStatus::GM_SLINE:
		drawst.setModeSline();			
		break;
	case DrawStatus::GM_DLINE:
		drawst.setModeDline();			
		break;
	case DrawStatus::GM_OTHER:
		drawst.setModeOther();			
		break;
	case DrawStatus::GM_DELETE:
		drawst.setModeDelete();			
		break;
	case DrawStatus::GM_MOVE:
		drawst.setModeMove();			
		break;
	}
}

// fin clase SetGrafMode

bool DrawCursRight::redo()
{
	idCheck();
	DaliView *cview = cView.value();
	
	c1 = cursor().getChar();
	if (c1 == 0) 
		c1 = ' ';

	drawst.compose(*cview, cursor(), C_LARROW);
	
	cview->cursRight();
	c2 = cursor().getChar();
	if (c2 == 0) 
		c2 = ' ';
	drawst.compose(*cview, cursor(), C_RARROW);
	
	cview->redrawCurrLine();	
	return true;
}

void DrawCursRight::undo() 
{ 
	idCheck();
	DaliView *cview = cView.value();
	cursor().setChar(c2);
	cview->cursLeft();
	cursor().setChar(c1);
	cview->redrawCurrLine();	
}

bool DrawCursLeft::redo()
{
	idCheck();
	if (cursor().column() == 0) {
		Editor::beep();
		return false;
	}
	DaliView *cview = cView.value();

	c1 = cursor().getChar();
	if (c1 == 0) c1 = ' ';
	drawst.compose(*cview, cursor(), C_RARROW);

	cview->cursLeft();
	c2 = cursor().getChar();
	if (c2 == 0) c2 = ' ';
	drawst.compose(*cview, cursor(), C_LARROW);

	cview->redrawCurrLine();	
	return true;
}

void DrawCursLeft::undo() 
{ 
	idCheck();
	DaliView *cview = cView.value();
	cursor().setChar(c2);
	cview->cursRight();
	cursor().setChar(c1);
	cview->redrawCurrLine();	
}

bool DrawCursUp::redo()
{
	idCheck();
	if (cursor().order() == 0) {
		Editor::beep();
		return false;
	}
	
	DaliView *cview = cView.value();
	
	c1 = cursor().getChar();
	
	if (c1 == 0) c1 = ' ';
	drawst.compose(*cview, cursor(), C_DARROW);
	
	cview->redrawCurrLine();	
	if (!cview->cursUp()) 
		cview->redrawCurrLine();

	c2 = cursor().getChar();
	if (c2 == 0) c2 = ' ';
	drawst.compose(*cview, cursor(), C_UARROW);
	cview->redrawCurrLine();	
	return true;
}

void DrawCursUp::undo()  
{ 
	idCheck();
	DaliView *cview = cView.value();
	cursor().setChar(c2);
	cview->redrawCurrLine();	
	cview->cursDown();
	cursor().setChar(c1);
	cview->redrawCurrLine();	
}

bool DrawCursDown::redo()
{
	idCheck();
	DaliView *cview = cView.value();
	c1 = cursor().getChar();
	if (c1 == 0) c1 = ' ';
	drawst.compose(*cview, cursor(), C_UARROW);
	cview->redrawCurrLine();	

	if (!cview->cursDown()) cview->redrawCurrLine();
	c2 = cursor().getChar();
	if (c2 == 0) c2 = ' ';
	drawst.compose(*cview, cursor(), C_DARROW);

	cview->redrawCurrLine();	
	return true;
}

void DrawCursDown::undo() 
{ 
	idCheck();
	DaliView *cview = cView.value();
	cursor().setChar(c2);
	cview->redrawCurrLine();	
	cview->cursUp();
	cursor().setChar(c1);
	cview->redrawCurrLine();	
}

bool EndDrawMode::redo()
{
	idCheck();
	drawmode = drawst.getGMode();
	cView.value()->setDrawMode(0);
	Editor::setMode(NULL_MODE);
	return true;
}

void EndDrawMode::undo() 
{ 
	idCheck();
	// debo setear el modo enterior
	switch (drawmode) {
	case DrawStatus::GM_SLINE:
		drawst.setModeSline();			
		break;
	case DrawStatus::GM_DLINE:
		drawst.setModeDline();			
		break;
	case DrawStatus::GM_OTHER:
		drawst.setModeOther();			
		break;
	case DrawStatus::GM_DELETE:
		drawst.setModeDelete();			
		break;
	case DrawStatus::GM_MOVE:
		drawst.setModeMove();			
		break;
	}
	Editor::setMode(DRAW_MODE);
}
