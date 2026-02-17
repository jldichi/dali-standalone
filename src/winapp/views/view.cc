/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: view.cc,v 1.4 2004/04/30 20:53:56 gsposito Exp $
*
* DESCRIPTION: Implementation of class View
*********************************************************************/
#ifdef __GNUG__
#	pragma implementation
#endif

// System library
#include <sys/types.h>
#include <sys/stat.h>

// Local library
#include <local/strfile.h>

// Foundation library
#include <ifound.h>
#include <ifound/msgtable.h>
#include <ifound/stdc.h>
#include <ifound/rexp.h>
#include <ifound/chset.h>
#ifdef ISDEBUG
#	include <ifound/iddebug.h>
#endif

// WinApp library
#include <winapp/view.h>
#include <winapp/text.h>
#include <winapp/ftext.h>
#include <winapp/mldbox.h>
#include <winapp/window.h>
#include <winapp/msgbox.h>
#include <winapp/waitmsg.h>

#define cursor (*_cursor)

MsgTable 	*View::msgs 		= NULL;
View 		*View::head 		= NULL;
Window 		*View::defViewPar 	= NULL;
bool 		View::fullRedraw 	= true;

View::View(const String &file, Text *t, TextCursor *tc)
: Window()
{
	idCheck();

	ccol = cfil = f_orig = c_orig = 0;

   	easyExit_ = false;
	dontExit_ = false;
    
    // If no text received, build it here
	text 	= (t 	== NULL ? constructText(file) 	: t);
	_cursor = (tc	== NULL ? tr_new TextCursor		: tc);

	// numero de view sobre el texto
	ntxt = text->nroViews();

	insNode();
	text->incViews();

	if (ntxt==1) 
		addText();

	// Initially set view's label with the text name.
	label = text->getTextName();
	
	// comienzo con modo insert
	mode = READONLY;

	// despliega toda la linea de status
	stmode = ST_FULL;
}

View::View(const String &cmd, bool abortable, Text *t, TextCursor *tc)
: Window()
{   
	idCheck();

   	ccol = cfil = f_orig = c_orig = 0;

   	easyExit_ = false;
	dontExit_ = false;

    exectime	= NOW;
	processed 	= ! abortable;
	inProcess 	= false;

    // If no text received, build it here
	text 	= (t	== NULL ? constructText(cmd,abortable) 	: t);
	_cursor = (tc 	== NULL ? tr_new TextCursor 				: tc);

	// Initially set view's label with the text name.
	label = text->getTextName();

	// comienzo con modo insert
	stmode = ST_FULL;
}

View::~View()
{
	idCheck();

	assert(text->nroViews() > 0);
	
	text->decViews();

	delNode();

	// Delete cursor before deleting text!
	tr_delete(_cursor);

	if (text->nroViews()==0)
		tr_delete(text);
	else {
		subText();		// redespliego el label de las views de nro >
		if (!existActText())
			text->deActivate();
	}        
}

Text *View::constructText(const String &f)   
	// Used if text has not already been constructed by derived classes
{
	View *pvw;

	if (pvw = existText(f))
		return pvw->text;

	return tr_new Text(f);
}																																	

Text *View::constructText(const String &cmd, bool abortable)
	// Used if text has not already been constructed by derived classes
{
	return tr_new Text(cmd,abortable);
}																																	

Int View::keyWordAttrib(const String &p, const String &ext)
	// No valid keyword handler at this level
{
	idCheck();

	return ERR;
}

Decoration *View::getDecoration(const char *pc, String &ext)
{
	idCheck();

	Decoration *ret, *last_d, *pd;
	Int attr;
	ret = last_d = NULL;
	const char *q = NULL, *p = pc;
	do {
		if (isalpha(*p)) {
			if (q == NULL) q = p;
		} else if (q) {
			if ((attr = keyWordAttrib(String(q, p - q), ext)) !=
					ERR) {
				pd = tr_new Decoration(q - pc, p - q,
						attr != INT_NULL ? attr : A_BOLD);
        	    if (last_d) last_d->next = pd;
            	else       	ret = pd;
            	last_d = pd;
        	}
        	q = NULL;
        }
    } while (*p++);
    return ret;
}        

// metodos privados

String View::decodeTabs(String s)
{
	idCheck();

	Int len = s.length();

	if (mode&DSPCTROL) {
		// estoy en modo display control
		for (Int i = 0; i < len; i++) {
			switch(s[i]) {
			case '\t':
				s.setChar(C_BULLET, i);
				break;
			case FILL_TAB:
				s.setChar('.', i);
				break;
			default:
				break;
			}
		}
	}
	else {
		for (Int i = 0; i < len; i++) 
			if (s[i] == '\t' || s[i] == FILL_TAB)
				s.setChar(' ', i);
	}		
	return s;
}

String View::decodeChar(const char c)
{   
	idCheck();

	// Representable
	if ((UnChar)c >= ' ')
		return String(1, c);

	return String(1, C_SHADOW);
}

void View::goToCursor()
{
	idCheck();

	goTo(cursor.order()-f_orig, cursor.column()-c_orig);	
}

bool View::actTopOrigin()
{
	idCheck();

	if (cursor.order() < f_orig) {
		f_orig = cursor.order();		
		return true;
	}
	return false;
}

bool View::actBotOrigin()
{
	idCheck();

	if (cursor.order()-f_orig >= nfils) {
		f_orig = cursor.order() + 1 - nfils;
		return true;
	}
	return false;
}

bool View::actColOrigin()
{
	idCheck();

	Int rcol = cursor.column();

	if (rcol-c_orig >= ncols) {
		c_orig = rcol + 1 - ncols;
	   	return true;
	}

	if (rcol < c_orig) {
		c_orig = rcol;
	   	return true;
	}
   	return false;
}

void View::redrawLine(TextCursor &tc, String &ext)
{
	idCheck();

	Int c_hasta, curr_attr = ERR, aux;
	// obtengo el string con los tabs decodificados
	String str = decodeTabs(tc.value());
	// veo cuantos caracteres debo escribir
	Int last = str.length() > ncols+c_orig ? ncols+c_orig                
			 : str.length();

	// inicializo hasta donde es valido el atributo
	c_hasta = ERR;

	// obtengo el puntero al string a desplegar
	const char *pc = toCharPtr(str);

	// obtengo la lista de zonas con adornos (keywords, etc.)
    Decoration *d = NULL, *decor = NULL;
    if (pc && !(mode&DSPCTROL)) {
	   decor = getDecoration(pc, ext);
       for (d = decor; d && d->from < c_orig && d->to < c_orig; d = d->next)
       	;
    }
       
	// despliego el string
	pc += c_orig;
	for (Int i = c_orig; i < last; i++) {

		// verifico si el atributo es valido si no lo reseteo
		if (i > c_hasta) {
			tc.goToCol(i);
			aux = text->getAttr(tc, c_hasta);
			if (d) {
				if (i >= d->from && i <= d->to) {
					aux |= d->attr;
					if (c_hasta > d->to) {
						c_hasta = d->to;
				    	d = d->next;
					}
				}
				else if (c_hasta >= d->from)
					c_hasta = d->from-1;
			}
			if (aux != curr_attr)
				setAttr(curr_attr = aux);
		}

		// ahora lo escribo
		*this << decodeChar(*pc++);
	}

	// borra todos los decorations utilizados
	for (Decoration *nd = d = decor; d; d = nd) {
		nd = d->next;
		tr_delete(d);
	}

	// restauro el atributo
	if (curr_attr != A_NORMAL) setAttr(A_NORMAL);

	// si estoy en modo control coloco el $
	if (mode&DSPCTROL) {
		if (str.length() < ncols+c_orig && c_orig-1 < last)
			*this << '$';
	}
}

bool View::chgLabel(const String &nlabel)
{   
	idCheck();

	label = nlabel;
	
	if (!hidden()) {
		setBorder(VIEW_BORDER, label);
		writeStLine();
	}
	return true;
}

void View::redrawCurrLine()
{
	idCheck();

	// me coloco al principio de la linea y la borro
    String ext = getTextName().baseFullName().extName();
	Int fil  = cursor.order()-f_orig;
	goTo(fil, 0);	
	eraseEol();
	if (cursor) {
        Int col = cursor.column();
        redrawLine(cursor, ext);
		// restauro el numero de columna
		cursor.goToCol(col);
	}

	updateOtherViews();
}

void View::updateOtherViews()
{
	idCheck();

	if (text->nroViews() > 1) {
		for(View *aux=head; aux; aux = aux->next)
			if (aux != this && aux->text == text && !(aux->hidden()))
				aux->update();
		refresh();
	}
}

void View::update()
{
	idCheck();

	Int fil = nfils;
    String ext = getTextName().baseFullName().extName();

	erase();

	TextCursor tc(*text);

	for(tc.goTo(f_orig); fil && tc ; tc++, fil--) {
        redrawLine(tc, ext);
		// si no estoy en la ultima linea bajo
		if (fil != 1) *this << "\n";
	}

	// redibujo la linea de status
	writeStLine();
}

void View::redraw()
{
	idCheck();

	update();
	if (fullRedraw)
		updateOtherViews();
}

void View::actCurrLine(Int val)
{
	idCheck();

	cursor += val;

	if (actBotOrigin()) {
		scroll(1);
		redrawCurrLine();	
		return;
	}

	if (actTopOrigin()) {
		scroll(-1);
		redrawCurrLine();	
		return;
	}
}

void View::setFullRedraw(const bool &b)
{ 
	fullRedraw = b; 
}

void View::clear() 
{ 	
	idCheck(); 
	assert(text); 

	text->delAll();
	f_orig = 0; 
	c_orig = 0; 
	redraw();
}

bool View::sameFile(const String &f1, const String &f2)
{
	if (f1 == f2) return true;
		
	String n1 = f1;
	String n2 = f2;
	String d1 = ".";
	String d2 = ".";

	// divido en nombre del directorio
	if (f1.index(PATH_SLASH[0])!=ERR) {
		n1 = f1.field(-1, PATH_SLASH);
		d1 = f1.field(0, String(PATH_SLASH)+n1);
		if (!d1) d1 = ".";
	}

	// divido en nombre del directorio
	if (f2.index(PATH_SLASH[0])!=ERR) {
		n2 = f2.field(-1, PATH_SLASH);
		d2 = f2.field(0, String(PATH_SLASH)+n2);
		if (!d2) d2 = ".";
	}

	if (n1 != n2) return false;

#ifdef __VC__
	struct _stat st1;
	struct _stat st2;
#else
	struct stat st1;
	struct stat st2;
#endif

	if (statFile(d1, &st1) == ERR || statFile(d2, &st2) == ERR)
		return false;

	// son iguales los directorios -> es el mismo archivo
	if (st1.st_ino == st2.st_ino) return true;

	return false;
}

View *View::existText(const String &name)
{           
	for (View *aux = head; aux; aux = aux->next)
		if (sameFile(aux->getTextName(), name))
			return aux;
			
	return NULL;
}

View *View::existActText()
{
	idCheck();

	for(View *aux=head; aux; aux = aux->next)
		if (aux != this && aux->text == text && !(aux->hidden()))
			return aux;                                            

	return NULL;
}

void View::subText()
	// busco la views que tienen el mismo text
	// para redesplegar el label de la window
{
	idCheck();

	for (View *aux = head; aux; aux = aux->next) {
		if (aux->text == text && aux->ntxt > ntxt) {
			(aux->ntxt)--;
			aux->chgLabel( aux->getLabel(true) );
		}
	}	
}

void View::addText()
	// busco la views que tienen el mismo text
	// para redesplegar el label de la window
{          
	idCheck();

	for (View *aux = head; aux; aux = aux->next)
		if (aux->text == text && !(aux->hidden())) 
			aux->chgLabel( aux->getLabel(true) );
}

void View::insNode()
{
	idCheck();

	if (head==NULL) {
		head = this;
		next = NULL;
		return;
	}

	View *aux;
	for (aux=head; aux->next; aux=aux->next)
			;

	aux->next = this;
	this->next = NULL;
}

void View::delNode()
{
	idCheck();

	if (head==this) {
		head = this->next;
		return;
	}

	View *aux;
	for (aux=head; aux->next && aux->next!=this; aux=aux->next);

	aux->next = aux->next->next;
}

void View::tabSize(Int v)
{
	Text::tabSize(v);
}

Int  View::sizeOfText()
{ 
	idCheck();

	return text->size();
}

void View::activate(Int forg, Int corg, Int fil, Int col, bool curr)
{
	idCheck();

	if (!hidden()) return;

	text->activate();

	cursor = *text;
	cursor.goTo(cfil, ccol);

	// coloco la view corriente
	if (curr) mode |= CURRENT;

	display(defViewPar, forg, corg, fil, col, VIEW_BORDER, 
			getLabel(true), A_NORMAL);

	wrap(false);

	orgnfil = nfils  = Window::height();
	orgncol = ncols  = Window::width();
	orgfil  = Window::orgRow() - defViewPar->orgRow();
	orgcol  = Window::orgCol() - defViewPar->orgCol();

	mode |= ORGSIZE;

	redraw();
	refresh();

	if (isTextType(ISTREAM) && !processed) {
		inProcess = true;			// Flag to show msg in writeStLine
		InputPipe is(getTextName());
		String aux;
		while (is >> aux) {
			appendLine(aux);
			if (isAborted())
				break;
		}
		text->resetModif();
		inProcess = false;
		processed = true;
		redraw();
	}
}

void View::deActivate()
{
	idCheck();

	if (hidden()) return;

	unDisplay();

	ccol = cursor.column();
	cfil = cursor.order();

	if (!existActText())
		text->deActivate();

	// si era corriente ya no lo es mas
	mode &= ~CURRENT;
}

bool View::setCurrent(bool f)
{
	idCheck();

	// la view no esta activa
	if (hidden())
		return false;

	// debo activarla 
	if (f) {

		// estaba activa
		if (mode&CURRENT)
			return true;

		mode |= CURRENT;
		chgLabel(getLabel(true));
		return true;
	}

	// debo desactivarla

	// estaba desactiva
	if (!(mode&CURRENT))
		return true;

	mode &= ~CURRENT;
	chgLabel(getLabel(true));
	return true;
}

bool View::load(String f)
{
	idCheck();

	if (hidden()) return false;

	View *pvw;
	if (pvw = existText(f))	{	// existe ya el text

		// si es el mismo archivo -> listo
		if (sameFile(f, getTextName()))
			return true;

		// queda alguna con este texto
		if (text->nroViews()!=1) {
			text->decViews();
			subText();				// redespliego el label de views >
			if (!existActText())	// no hay views act. desactivo text
				text->deActivate();
		} else
			tr_delete(text);

		text = pvw->text;
		ntxt = text->nroViews();
		text->incViews();
		text->activate();

		// si es la segunda view debo redesplegar
		// el label de la primera
		if (ntxt==1) addText();

	// el texto nuevo no existe
	} else {
		if (text->nroViews()!=1) {
			text->decViews();
			subText();				// redespliego el label de views >
			if (!existActText())	// no hay views act. desactivo text
				text->deActivate();
			text = tr_new Text(f);	
			ntxt = text->nroViews();
			text->incViews();
			text->activate();
		} else text->setFile(f);
	}
    
    chgLabel(getLabel(true));

	f_orig = c_orig = 0;

	cursor = *text;

	redraw();

	return true;
}

bool View::reload()
{                 
	idCheck();

	text->setFile(getTextName(), true);
	return true;
}

const String View::getTextName()
{                        
	idCheck();
	return text->getTextName();	
}   

const String View::getLabel(const bool addTxtInstance)
{
	idCheck();

	String aux = label;
    if (addTxtInstance && text->nroViews() > 1)
		aux += String(" [")+toCharPtr(ntxt)+String("]");

	return aux;
}

UnChar View::getKey()
{
	idCheck();

	UnChar c;
	*this >> c;
	return c;
}

bool View::goCursor(Int fil, Int col, bool center)
{
	idCheck();

	if (fil < 0)
		fil = 0;

	cursor.goTo(fil, col);

	/*
		Only try to center if we are off the screen.
	*/
	if (fil < f_orig || fil >= f_orig+nfils) {
		/*
			Only try to center if we are going to be able to do it.
		*/
		if (center) {
			Int lineNo;
			if ((lineNo = fil - nfils / 2) < 0)
				lineNo = 0;

			if (lineNo != f_orig) {
				f_orig = lineNo;
				redraw();
				return true;
			}
			goToCursor();
			return false;
		}
	}
	if (actColOrigin() || actBotOrigin() || actTopOrigin()) {
		redraw();
		return true;
	}
	redraw();  
	goToCursor();	
	return false;
}

void View::setOrgSize()
{
	idCheck();

	if (mode&ORGSIZE) return;

	mode  |= ORGSIZE;

	unDisplay();

	display(defViewPar, orgfil, orgcol, orgnfil, orgncol, VIEW_BORDER,
			getLabel(true), A_NORMAL);

	wrap(false);

	nfils = orgnfil;
	ncols = orgncol;

	actBotOrigin();
	actTopOrigin();
	actColOrigin();

	redraw();

	refresh();
}

void View::chgOrgSize(Int fo, Int co, Int cr, Int cc)
{
	idCheck();

	setSize(fo, co, cr, cc);
	orgnfil = nfils;
	orgncol = ncols;
	orgfil  = Window::orgRow() - defViewPar->orgRow();
	orgcol  = Window::orgCol() - defViewPar->orgCol();
	mode |= ORGSIZE;
}

void View::setSize(Int fo, Int co, Int cr, Int cc)
{
	idCheck();

	mode &= ~ORGSIZE;

	unDisplay();

	display(defViewPar, fo, co, cr, cc, VIEW_BORDER, 
			getLabel(true), A_NORMAL);

	wrap(false);

	nfils = Window::height();
	ncols = Window::width();

	actBotOrigin();
	actTopOrigin();
	actColOrigin();

	redraw();

	refresh();
}

bool View::exit(bool)
{
	idCheck();

	return !dontExit_;
}

void View::_writeStLine()
{
	idCheck();
	
	// Current line and column
	goTo(nfils, C_LINECOL);
	*this << cursor.order()+1 << ":" << cursor.column()+1;
}

void View::writeStLine(const String &s)
   // Imprime la linea de Status y reposiciona el Cursor
{        
	idCheck();

	if (hidden()) 
		return;

	// Clear status line
	goTo(nfils, 0);
	*this << String(ncols, DEL_CHAR);

	// Write standard information
	_writeStLine();

	if (mode&HELPMODE) {
		goTo(nfils, C_HELPMODE);
		*this << D_HELPMODE;
	}
	
	// Write extra information
	if (s != NULL_STRING) { 
		*this << "  " << s;
	}	
    
	// Update cursor position
	goToCursor();
}            

bool View::resize()
{
	idCheck();

	bool result = Window::resize();

	orgnfil = nfils = Window::height();
	orgncol = ncols = Window::width();

	actBotOrigin();
	actTopOrigin();
	actColOrigin();

	writeStLine();

	return result;
}

bool View::move()
{
	idCheck();

	if (Window::move()) {
		orgfil  = Window::orgRow() - defViewPar->orgRow();
		orgcol  = Window::orgCol() - defViewPar->orgCol();

		actBotOrigin();
		actTopOrigin();
		actColOrigin();

		return true;
	} else
		return false;
}

String View::msg(const String str)
{
	if (msgs == NULL)
		return "(No message table)";
	else
		return msgs->find(str);
}

void View::setViewPar(Window *par)
{
	defViewPar = par;
}                    

bool View::isTextType(int ty)
{ 
	idCheck();

	return text->isType(TextType(ty));
}

bool View::isTextType(TextType ty)
{ 
	idCheck();

	return text->isType(ty);
}

bool View::isAborted()
{
	idCheck();

	assert(!hidden());

	if (keyReady()) {
		UnChar c;
		*this >> c;
		return c == ABORT_KEY;
	}		

	return false;
}

void View::appendLine(const String &txt)
{
	idCheck();

	text->add(txt);
	if (!goCursor(text->size(), 0))
		redraw();
	refresh();
}

void View::deleteView(View *v)
{             
	if (v->fileView() != NULL) {
		Text *txt = v->text;
		if (txt->nroViews()==1)
	 		txt->fileText()->rmFiles();
	}
	tr_delete(v);
}

// end of view


void View::chgCtrolMode()
{
	idCheck();
	if (mode&DSPCTROL)
		mode &= ~DSPCTROL;
	else
		mode |= DSPCTROL;
	redraw();
}

void View::chgHelpMode()
{
	idCheck();
	if (mode&HELPMODE)
		mode &= ~HELPMODE;
	else
		mode |= HELPMODE;
	redraw();
}

void View::findTokenBounds(Int &from, Int &to) const
{
	idCheck();

	String tmp = cursor.value();

	from = to = cursor.column();

	if (from >= tmp.length() || isSeparator(tmp[from]))
		from = to = ERR;

	while (from >= 0 && !isSeparator(tmp[from]))
		from--;

	from++;

	while (to < tmp.length() && !isSeparator(tmp[to]))
		to++;
}

String View::getToken()
{
	idCheck();
	String tmp = cursor.value();

	Int from, to;
	findTokenBounds(from, to);
	highlightToken(from, to);

	if (from == ERR || to == ERR)
		return NULL_STRING;

	return tmp(from, to-from);
}

void View::highlightToken(Int from, Int to)
{
	idCheck();
	if (from == ERR)
		return;

	TextCursor tcFrom = cursor, tcTo = cursor;
	tcFrom.goToCol(from);
	tcTo.goToCol(to-1);

	text->setAttr(tcFrom, tcTo, A_STANDOUT);
	redrawCurrLine();
	refresh();
	text->delAttr(tcFrom, tcTo, A_STANDOUT);
}


