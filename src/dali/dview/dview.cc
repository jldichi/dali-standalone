/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dview.cc,v 1.5 2005/06/16 22:13:52 albertoi Exp $
*
* DESCRIPTION: Implementation of dview class
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <ifound.h>

#if ISDEBUG
#	include <stdio.h>	
#endif

#include <ifound/msgtable.h>
#include <dview.h>
#include <dtext.h>
#include <winapp/mldbox.h>
#include <local/strfile.h>
#include <ifound/stdc.h>
#include <ifound/rexp.h>
#include <winapp/window.h>
#include <editor.h>
#include <ifound/chset.h>
#include <winapp/msgbox.h>
#include <winapp/waitmsg.h>

#include <sys/types.h>
#include <sys/stat.h>

#ifdef HAVE_UNDERSCORE_NAMES
#	define stat _stat
#endif

#define cursor (*_cursor)
#define block  (*_block)
MsgTable *DaliView::msgs = NULL;
DaliView *DaliView::head = NULL;
Window *DaliView::defViewPar = NULL;
bool DaliView::fullRedraw = true;

struct Decoration
{
	Int from;
	Int to;
	Int attr;
	Decoration *next;
	Decoration(Int f, Int l, Int a) { from=f; to=f+l-1; attr=a; next=NULL;}
};

static Decoration *getDecoration(const char *pc, String &ext)
{
//	assert(pc);
	
	Decoration *ret, *last_d, *pd;
	Int attrib;
	ret = last_d = NULL;
	const char *q = NULL, *p = pc;
	do {
		if (isalpha(*p)) {
			if (q == NULL) q = p;
		} else if (q) {
			if ((attrib = Editor::keyWordAttrib(String(q, p - q), ext)) !=
					ERR) {
				pd = tr_new Decoration(q - pc, p - q,
						attrib != INT_NULL ? attrib : A_BOLD);
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
String DaliView::decodeTabs(String s)
{
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
	// modo normal
	else {
		for (Int i = 0; i < len; i++) {
			if (s[i] == '\t' || s[i] == FILL_TAB)
				s.setChar(' ', i);
		}
	}
	return s;
}

String DaliView::decodeChar(const char c)
{
	if ((UnChar)c >= ' ')
		return String(1, c);

	if ((!(mode&DSPCTROL) && (UnChar)c < ' ') ||
		((UnChar)c > 125 && (UnChar)c < 160))
		return String(1, C_SHADOW);

	String res;
	res += Editor::attribToSequence(
			Editor::findAttrib("Reverse") | Editor::findAttrib("Bold"));
	res += String(1, c+64);
	res += ATTR_NORMAL;

	return res;
}

void DaliView::blockOn(bool rw)
{
	idCheck();
	// si no hay que marcar me voy
	if (block == cursor) {
		if (rw) redraw();
		return;
	}

	setAttribute(block, A_STANDOUT);

	if (rw) redraw();
}

void DaliView::blockOff(bool rw)
{
	idCheck();
	delAttribute(block, A_STANDOUT);

	if (rw) redraw();
}

void DaliView::goToCursor()
{
	idCheck();
	goTo(cursor.order()-f_orig, cursor.column()-c_orig);	
}

bool DaliView::actTopOrigin()
{
	idCheck();
	if (cursor.order() < f_orig) {
		f_orig = cursor.order();		
		return true;
	}
	return false;
}

bool DaliView::actBotOrigin()
{
	idCheck();
	if (cursor.order()-f_orig >= nfils) {
		f_orig = cursor.order() + 1 - nfils;
		return true;
	}
	return false;
}

bool DaliView::actColOrigin()
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

void DaliView::redrawLine(DaliTextCursor &tc, String &ext)
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

void DaliView::redrawCurrLine()
{
	idCheck();
	// me coloco al principio de la linea y la borro
    String ext = String(getFileName().field(0," ")).baseFullName().extName();
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

void DaliView::updateOtherViews()
{
	idCheck();
	if (text->nroViews() > 1) {
		for(DaliView *aux=head; aux; aux = aux->next)
			if (aux != this && aux->text == text && !(aux->hidden()))
				aux->update();
		Window::flush();
	}
}

void DaliView::update()
{
	idCheck();
	Int fil = nfils;
    String ext = String(getFileName().field(0," ")).baseFullName().extName();
	erase();
	DaliTextCursor tc(*text);
	tc.goTo(f_orig);
	for(; fil && tc ; tc++, fil--) {
        redrawLine(tc, ext);
		// si no estoy en la ultima linea bajo
		if (fil != 1) *this << NEW_LINE;
	}
	// redibujo la linea de status
	writeStLine();
}

void DaliView::redraw()
{
	idCheck();
	update();
	if (fullRedraw)
		updateOtherViews();
}

void DaliView::actCurrLine(Int val)
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

void DaliView::chgStrCursor(Int len, const String newtext)
{
	idCheck();
	Int col = cursor.column();

	// borro el text
	for (Int i = 0; i < len; i++)
		cursor.delChar();

	// inserto el nuevo texto
	const char *ntext = toCharPtr(newtext) + newtext.length() - 1;
	Int  top    = col + newtext.length();
	for (i = col; i < top; i++) {
		cursor.insChar(*ntext--);
		cursor.reAssignNextTab(cursor.column()+1);
	}		
}

void DaliView::setFullRedraw(const bool b)
{
	fullRedraw = b;
}

bool DaliView::sameFile(const String &f1, const String &f2)
{
	if (f1 == f2) return true;
		
	struct stat st1;
	struct stat st2;

	if (statFile(f1, &st1) == ERR || statFile(f2, &st2) == ERR)
		return false;

	return st1.st_ino == st2.st_ino;
}

/*
bool DaliView::sameFile(const String &f1, const String &f2)
{
	if (f1 == f2) return true;
		
	String n1 = f1;
	String n2 = f2;
	String d1 = ".";
	String d2 = ".";

	// divido en nombre del directorio
	if (f1.index('/')!=ERR) {
		n1 = f1.field(-1, "/");
		d1 = f1.field(0, String("/")+n1);
		if (!d1) d1 = ".";
	}

	// divido en nombre del directorio
	if (f2.index('/')!=ERR) {
		n2 = f2.field(-1, "/");
		d2 = f2.field(0, String("/")+n2);
		if (!d2) d2 = ".";
	}

	if (n1 != n2) return false;

	struct stat st1;
	struct stat st2;

	if (statFile(d1, &st1) == ERR || statFile(d2, &st2) == ERR)
		return false;

	// son iguales los directorios -> es el mismo archivo
	if (st1.st_ino == st2.st_ino) return true;

	return false;
}
*/

DaliView *DaliView::existText(const String &name)
{
	for (DaliView *aux = head; aux; aux = aux->next)
		if (sameFile(aux->text->getFile(), name))
			return aux;
			
	return NULL;
}

DaliView *DaliView::existActText()
{
	for(DaliView *aux=head; aux; aux = aux->next)
		if (aux != this && aux->text == text && !(aux->hidden()))
			return aux;
	return NULL;
}

void DaliView::subText()
	// busco la views que tienen el mismo text
	// para redesplegar el label de la window
{
	for (DaliView *aux = head; aux; aux = aux->next) {
		if (aux->text == text && aux->ntxt > ntxt) {
			(aux->ntxt)--;
			aux->chgViewName( aux->getFile() );
		}
	}	
}

void DaliView::addText()
	// busco la views que tienen el mismo text
	// para redesplegar el label de la window
{
	for (DaliView *aux = head; aux; aux = aux->next)
		if (aux->text == text && !(aux->hidden()))
			aux->chgViewName( aux->getFile() );
}

void DaliView::insNode()
{
	idCheck();

	if (head==NULL) {
		head = this;
		next = NULL;
		return;
	}

	for (DaliView *aux=head; aux->next; aux=aux->next)
		;

	aux->next = this;
	this->next = NULL;
}

void DaliView::delNode()
{
	idCheck();

	if (head==this) {
		head = this->next;
		return;
	}

	for (DaliView *aux=head; aux->next && aux->next!=this; aux=aux->next)
		;

	aux->next = aux->next->next;
}

// metodos publicos

void DaliView::tabSize(Int v)
{
	DaliText::tabSize(v);
}

void DaliView::rememberFile()
{
	idCheck();
	text->storeTmp();
}

bool DaliView::isIStream()
{
	idCheck();
	return text->isIStream();
}

bool DaliView::isAdmin()
{
	idCheck();
	return text->isAdmin();
}

bool DaliView::isRdOnly()
{
	idCheck();
	return !(text->getFilePerm()&TXT_WRITE);
}

const String &DaliView::lockOwner()
{
	idCheck();
	return text->lockOwner();
}

bool DaliView::isLocked()
{
	idCheck();
	return (text->getFilePerm()&TXT_LOCKED);
}

bool DaliView::isTroff()
{
	idCheck();
	return text->isTroff();
}

bool DaliView::isTeX()
{
	idCheck();
	return text->isTeX();
}

bool DaliView::isPreserve()
{
	idCheck();
	return text->isPreserve();
}

Int  DaliView::sizeOfText()
{
	idCheck();
	return text->size();
}

bool DaliView::isTextMod()
{
	idCheck();
	return text->modif();
}

void DaliView::setTempDir(const String &s)
{
	DaliText::setTempDir(s);
}

const String &DaliView::getTempDir()
{
	return DaliText::getTempDir();
}

DaliView::DaliView(String f, bool isTemp)
: Window()
{
	idStart();
    footer 		= false;
    scratch		= false;
    inProcess	= false;
	processed 	= false;

  	ccol = cfil = f_orig = c_orig = drawMode = 0;

	if (isTemp) {
		text = tr_new DaliText(f, true);
		insNode();
		text->incViews();
	}
	else {
		DaliView *pvw;
		if (pvw = existText(f))
			text = pvw->text;
		else		
			text = tr_new DaliText(f, false);

		// numero de view sobre el texto
		ntxt = text->nroViews();

		insNode();
		text->incViews();

		// si es la segunda view debo redesplegar
		// el label de la primera
		if (ntxt==1) addText();
	}

	_cursor = tr_new DaliTextCursor();
	_block  = tr_new DaliTextCursor();
	_mark   = tr_new DaliTextCursor();

	// comienzo con modo insert
	mode = INSERT;

	// despliega toda la linea de status
	stmode = ST_FULL;

    // Special case: ibuild files are footers
	if (f.field(-1, ".") == "ib")
		footer	= true;
}

DaliView::DaliView(InputStream &is, const String &cmd):	Window()
{
	idStart();
    footer 		= false;
    scratch		= false;
    exectime	= NOW;
    inProcess	= false;
	processed 	= false;
    	
   	ccol = cfil = f_orig = c_orig = drawMode = 0;

	text  = tr_new DaliText(is, cmd);

	insNode();
	text->incViews();

	// comienzo con modo insert
	mode = INSERT;

	// despliega toda la linea de status
	stmode = ST_FULL;

	_cursor = tr_new DaliTextCursor();
	_block  = tr_new DaliTextCursor();
	_mark   = tr_new DaliTextCursor();
}

DaliView::DaliView(const bool proc, const String &cmd):	Window()
{
	idStart();
    footer 		= false;
    scratch		= false;
    inProcess	= false;
	processed 	= !proc;
    exectime	= processed ? Time(NOW) : toTime(NULL_TIME);
    	
   	ccol = cfil = f_orig = c_orig = drawMode = 0;

	text  = tr_new DaliText(cmd);

	insNode();
	text->incViews();

	// comienzo con modo insert
	mode = INSERT;

	// despliega toda la linea de status
	stmode = ST_FULL;

	_cursor = tr_new DaliTextCursor();
	_block  = tr_new DaliTextCursor();
	_mark   = tr_new DaliTextCursor();
}

DaliView::~DaliView()
{
	idCheck();
	text->decViews();

	delNode();

	// Delete cursor before deleting text!
	tr_delete(_cursor);
	tr_delete(_block);
	tr_delete(_mark);

	if (text->nroViews()==0)
		tr_delete(text);
	else {
		subText();		// redespliego el label de las views de nro >
		if (!existActText())
			text->deActivate();
	}
	idStop();
}

void DaliView::deleteView(DaliView *v)
{
	DaliText *txt = v->text;
	if (txt->nroViews()==1)
 		txt->rmFiles();
 	if (v->isScratch())
		Editor::scratchs--;
	tr_delete(v);
}

void DaliView::activate(Int forg, Int corg, Int fil, Int col, bool curr)
{
	idCheck();
	if (!hidden()) return;

	text->activate();

	cursor = *text;

	cursor.goTo(cfil, ccol);

	// coloco la view corriente
	if (curr) mode |= CURRENT;

	display(defViewPar, forg, corg, fil, col, VIEW_BORDER,
			getFile(), A_NORMAL);

	wrap(false);

	orgnfil = nfils  = Window::height();
	orgncol = ncols  = Window::width();
	orgfil  = Window::orgRow() - defViewPar->orgRow();
	orgcol  = Window::orgCol() - defViewPar->orgCol();

	mode |= ORGSIZE;

	redraw();
	refresh();

	if (isIStream() && ! processed) {
		inProcess = true;
		InputPipe is(text->getFile());
		String aux;
		while (is >> aux) {
			appendLine(aux);
			if (isAborted())
				break;
		}
		text->resetModif();
		inProcess = false;
		processed = true;
		exectime  = NOW;
		redraw();
	}
}

void DaliView::deActivate()
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

bool DaliView::setCurrent(bool f)
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
		chgViewName(getFile());
		return true;
	}

	// debo desactivarla

	// estaba desactiva
	if (!(mode&CURRENT))
		return true;

	mode &= ~CURRENT;
	chgViewName(getFile());
	return true;
}

bool DaliView::load(String f)
{
	idCheck();
	if (hidden()) return false;

	DaliView *pvw;
	if (pvw = existText(f))	{	// existe ya el text

		// si es el mismo archivo -> listo
		if (sameFile(f, text->getFile()))
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
			text = tr_new DaliText(f, false);	
			ntxt = text->nroViews();
			text->incViews();
			text->activate();
		} else text->setFile(f);
	}

    chgViewName(getFile());

	f_orig = c_orig = 0;

	cursor = *text;

	redraw();

	return true;
}

bool DaliView::reload()
{
	idCheck();
	text->setFile(text->getFile(), true);
	return true;
}

void DaliView::store(const String &fname)
{
	idCheck();
	text->store(fname);
}

bool DaliView::store()
{
	idCheck();
	if (block)
		blockOff(false);
	return text->store();
}

Int DaliView::insertFile(const String &fname)
{
	idCheck();
	DaliText txt(fname,false);
	txt.activate();

	Int auxfil = cursor.order();

	// si estoy parado afuera
	if (!(cursor)) cursor.ins(NULL_STRING);

	cursor.ins(txt);

	cursor.goTo(auxfil);

	Int ret = txt.size();
	if (ret)
		redraw();

	return ret;
}

const String DaliView::getFile()
{
	idCheck();
	String label = text->getFile();

	if (text->nroViews() > 1)
		label += String(" [")+toCharPtr(ntxt)+String("]");

	return label;
}

const String DaliView::getFileName()
{
	idCheck();
	return text->getFile();
}

void DaliView::chgInsMode(bool redrw)
{
	idCheck();
	if (mode&INSERT)
		mode &= ~INSERT;
	else
		mode |= INSERT;

	if (redrw)
		writeStLine();
}

void DaliView::chgCtrolMode()
{
	idCheck();
	if (mode&DSPCTROL)
		mode &= ~DSPCTROL;
	else
		mode |= DSPCTROL;
	redraw();
}

UnChar DaliView::getKey()
{
	idCheck();
	UnChar c;
	*this >> c;
	return c;
}

bool DaliView::goCursor(Int fil, Int col, bool center)
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

void DaliView::setOrgSize()
{
	idCheck();
	if (mode&ORGSIZE) return;

	mode  |= ORGSIZE;

	unDisplay();

	display(defViewPar, orgfil, orgcol, orgnfil, orgncol, VIEW_BORDER,
			getFile(), A_NORMAL);

	wrap(false);

	nfils = orgnfil;
	ncols = orgncol;

	actBotOrigin();
	actTopOrigin();
	actColOrigin();

	redraw();

	refresh();
}

void DaliView::chgOrgSize(Int fo, Int co, Int cr, Int cc)
{
	idCheck();
	setSize(fo, co, cr, cc);
	orgnfil = nfils;
	orgncol = ncols;
	orgfil  = Window::orgRow() - defViewPar->orgRow();
	orgcol  = Window::orgCol() - defViewPar->orgCol();
	mode |= ORGSIZE;
}

void DaliView::setSize(Int fo, Int co, Int cr, Int cc)
{
	idCheck();
	mode &= ~ORGSIZE;

	unDisplay();

	display(defViewPar, fo, co, cr, cc, VIEW_BORDER, getFile(), A_NORMAL);

	wrap(false);

	nfils = Window::height();
	ncols = Window::width();

	actBotOrigin();
	actTopOrigin();
	actColOrigin();

	redraw();

	refresh();
}

bool DaliView::exit(bool igntxt)
{
	idCheck();

    String 	name = text->getFile();

	// Check if we are in debug mode and if the view is the debugger_view.
	if	((Editor::getMode() & DEBUG_MODE) && name == DEBUGGER_VIEW)
		return false;

	// If view is a IStream or its text is not modified, it is ok.
	if (isIStream() || (!isTextMod() && !(text->isPreserve())))
		return true;

	// If view`s text is a copy of another view, there is no problem.
	if (((existActText() && (igntxt || ntxt != 0))))
		return true;

    String 	aux;

	// If the text is read-only (by this point it is surely modified),
	// ask user what to do.
	if (isRdOnly()) {
		aux  = msg("EXITRDONLY1") + NEW_LINE + msg("EXITRDONLY2");

		MLDBox dbox(libMsg("WARNING"),name+NEW_LINE+NEW_LINE+aux, DBOX_NO);

		dbox.execute();
		return ( dbox.getOpt() == DBOX_YES );
	}

    // The view is NOT read-only and it is modified.
	aux  = msg("SAVECONFIRM");

	MLDBox dbox(libMsg("MESSAGE"), aux+NEW_LINE+name+"?", DBOX_YES);
	dbox.execute();
	
	switch (dbox.getOpt()) {
		case DBOX_YES:
			store();
			return true;
		case DBOX_NO:
			if (text->isPreserve()) {
				aux = msg("SAVEPRESERVE1")+NEW_LINE+msg("SAVEPRESERVE2");
				MLDBox pdbox(libMsg("MESSAGE"),aux+NEW_LINE+name, DBOX_YES);
				pdbox.execute();
				switch (pdbox.getOpt()) {
				case DBOX_YES:
					store();
					return true;
				case DBOX_NO:
					return true;
				case DBOX_CANCEL:
				case ERR:
					return false;
				}
			}
			return true;
		case DBOX_CANCEL:
		case ERR:
			return false;
	}
	assert(0);
	return false;	// Just to avoid compiler warning
}

#define FUNC_WIDTH 25
void DaliView::writeStLine(const String &s)
   // Imprime la linea de Status y reposiciona el Cursor
{
	idCheck();
	if (hidden())
		return;

	// NOTA :	switch (stmode) {
	//				case ST_NORMAL:
	//				case ST_FULL:

	// Borro el contenido de la linea de status
	goTo(nfils, 0);
	*this << String(ncols, DEL_CHAR);
	
	// Esta modificado el archivo
	if (isTextMod()) {
		goTo(nfils, C_MODIF);
		*this << D_MODIF;
	}

	// modo ins/ovr
	goTo(nfils, C_MODE);
	if (mode & INSERT)	*this << msg("INS");
	else				*this << msg("OVR");

	// las linea y columna corriente
	goTo(nfils, C_LINECOL);
	*this << cursor.order()+1 << ":" << cursor.column()+1;

    if (inProcess) {
		goTo(nfils, C_ABORTMSG);
		*this << msg("ONEMOMENT") + msg("ENDTOABORT");
		return;
	}
    	
	// modo bloque
	if (block) {
		goTo(nfils, C_STBLOCK);
		*this << D_STBLOCK;
	}

	// Archivo read only
	if (isLocked()) {
		goTo(nfils, C_LOCKED);
		*this << D_LOCKED;
		if (lockOwner())
			*this << ": " << lockOwner();
	}
	else {
		 if (isPreserve()) {
			goTo(nfils, C_RDONLY);
			*this << D_PRESERVE;
		 }
         else
			 if (isAdmin()) {
				goTo(nfils, C_RDONLY);
				*this << D_ADMIN;
			 }
			 else
				if( isIStream() ) {
					goTo(nfils, C_RDONLY);
					*this << toString(exectime);
			 	}
				else
					if ( isRdOnly() ) {
						goTo(nfils, C_RDONLY);
						*this << D_RDONLY;
					}
    }
	
	// Archivo de tipo troff
	if (isTroff()) {
		goTo(nfils, C_TROFF);
		*this << D_TROFF;
	}

	// Archivo de tipo tex
	if (isTeX()) {
		goTo(nfils, C_TEX);
		*this << D_TEX;
	}

	// se ven los caracteres de control
	if (mode&DSPCTROL) {
		goTo(nfils, C_CTROL);
		*this << D_CTROL;
	}

    // El modo de dibujo en que se encuentra la view
	Int dmode = getDrawMode();
	if (dmode) {
	    String dmodeStr;
	    switch(dmode) {
	        case DRAWMODE1: dmodeStr = D_DRAW1; 	break;
	        case DRAWMODE2: dmodeStr = D_DRAW2; 	break;
	        case DRAWMODE3: dmodeStr = D_DRAW3; 	break;
	        case DRAWDEL  : dmodeStr = D_DRAWDEL;  	break;
	        case DRAWMOVE : dmodeStr = D_DRAWMOVE; 	break;
	        default       : dmodeStr = "<ERR>";
	    }
		goTo(nfils, C_DRAW);
		*this << dmodeStr;
	}
	
	// Ahora va la opcion pasada como parametro
	if (s != NULL_STRING) {
		*this << "  " << s;
	}	

    // La funcion en la que se encuentra el cursor
    // ATENCION: Esto se escribe en la linea superior de la ventana
    if (Editor::functionSensor != FS_OFF && ncols > 70) {
		String gf;
		if (getFunction(gf)) {
			clearUpperLine();
			goTo(-1,ncols-FUNC_WIDTH);	
			if (!gf.isNull()) {
				gf = "[ "+ gf(0,FUNC_WIDTH-5) + " ]";
				goTo(-1,ncols-gf.length()-1);
				*this << gf;
			}	
		}	
	}

	goToCursor();
}

void DaliView::clearUpperLine()
	// Borra la linea superior de la ventana
	// donde aparece el nombre de la funcion
{
	idCheck();
    goTo(-1,ncols-FUNC_WIDTH);	
	*this << String(FUNC_WIDTH, DEL_CHAR);
}

void DaliView::setAttribute(DaliTextCursor &tc, Int attr)
{
	idCheck();
	// estan el la misma posicion
	if (tc == cursor) return;

	// esta primero el cursor que el tc
	if (tc > cursor)
		text->setAttr(cursor, tc, attr);
	else {
		// estoy dentro del texto
		if (cursor) {
			// estoy parado en numero de columna <> 0
			if (cursor.column()) {
				Int len = cursor.value().length();
				Int col = cursor.column();

				if (len == 0) len = 1;

				if (col < len)
					cursor.moveCol(-1);
				else
					cursor.goToCol(len-1);

				text->setAttr(tc, cursor, attr);

				cursor.goToCol(col);
			}
			// estoy en la columna 0
			else {
				(cursor)--;

				Int len = cursor.value().length();
				if (len == 0) len = 1;

				cursor.goToCol(len-1);

				text->setAttr(tc, cursor, attr);

				cursor.goTo(cursor.order()+1, 0);
			}
		}
		// estoy fuera del texto
		else {
			Int fil = cursor.order();
			Int col = cursor.column();

			cursor.goToLast();

			Int len = cursor.value().length();
			if (len == 0) len = 1;

			cursor.goToCol(len-1);
			text->setAttr(tc, cursor, attr);

			cursor.goTo(fil, col);
		}
	}
}

void DaliView::delAttribute(DaliTextCursor &tc, Int attr)
{
	idCheck();
	// estan el la misma posicion
	if (tc == cursor) return;

	// esta primero el cursor que el tc
	if (tc > cursor)
		text->delAttr(cursor, tc, attr);
	else {
		// estoy dentro del texto
		if (cursor) {
			// estoy parado en numero de columna <> 0
			if (cursor.column()) {
				Int col = cursor.column();
				Int len = cursor.value().length();

				if (len == 0) len = 1;

				if (col < len)
					cursor.moveCol(-1);
				else
					cursor.goToCol(len-1);

				text->delAttr(tc, cursor, attr);

				cursor.goToCol(col);
			}
			// estoy en la columna 0
			else {
				(cursor)--;

				Int len = cursor.value().length();
				if (len == 0) len = 1;

				cursor.goToCol(len-1);

				text->delAttr(tc, cursor, attr);

				cursor.goTo(cursor.order()+1, 0);
			}
		}
		// estoy fuera del texto
		else {
			Int fil = cursor.order();
			Int col = cursor.column();

			cursor.goToLast();

			Int len = cursor.value().length();
			if (len == 0) len = 1;

			cursor.goToCol(len-1);

			text->delAttr(tc, cursor, attr);

			cursor.goTo(fil, col);
		}
	}
}

bool DaliView::beginBlock()
{
	idCheck();
	if (!(cursor)) {
		beep();
		return false;
	}

	block = cursor;

	writeStLine();

	return true;
}

void DaliView::setBlock(DaliTextCursor &tc)
{
	idCheck();
	// si estaba activo -> borro el activo
	if (block) delBlock(false);

	block = tc;

	blockOn();
	writeStLine();
}

bool DaliView::hasBlock()
{
	idCheck();
	if (block) return true;
	return false;
}

DaliTextCursor &DaliView::getBlock()
{
	idCheck();
	return block;
}

void DaliView::delBlock(bool rw)
{
	idCheck();
	if (block) {
		blockOff(rw);
		block.clear();
	}
}

void DaliView::addPath()
{
	idCheck();
	text->addPath();
	chgViewName(getFileName());
}

bool DaliView::chgViewName(const String &fileName)
{
	idCheck();
	if (!hidden()) {
		setBorder(VIEW_BORDER, fileName);
		writeStLine();
	}
	return true;
}

bool DaliView::chgFileName(const String &fileName)
{
	idCheck();
	// son el mismo archivo
	if (sameFile(fileName , text->getFile()))
		return true;

	// el archivo esta editado por otra view, por ahora
	// no se puede!
	if (existText(fileName) != NULL)
		return false;

    // The view is no more a footer
    setFooter(false);

	// el archivo no esta editado por ninguna view
	text->setFile(fileName, false);
	chgViewName(getFile());
	return true;
}

bool DaliView::resize()
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

bool DaliView::move()
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

String DaliView::msg(const String str)
{
	if (msgs == NULL)
		return "(No message table)";
	else
		return msgs->find(str);
}

void DaliView::findTokenBounds(Int &from, Int &to) const
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

String DaliView::getToken()
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

void DaliView::highlightToken(Int from, Int to)
{
	idCheck();
	if (from == ERR)
		return;

	DaliTextCursor tcFrom = cursor, tcTo = cursor;
	tcFrom.goToCol(from);
	tcTo.goToCol(to-1);

	text->setAttr(tcFrom, tcTo, A_STANDOUT);
	redrawCurrLine();
	refresh();
	text->delAttr(tcFrom, tcTo, A_STANDOUT);
}

void DaliView::setTroff()
{
	idCheck();
	text->setTroff();
	writeStLine();
}

void DaliView::setTeX()
{
	idCheck();
	text->setTeX();
	writeStLine();
}

void DaliView::setAdmin()
{
	idCheck();
	text->setAdmin();
	writeStLine();
}

void DaliView::setViewPar(Window *par)
{
	defViewPar = par;
}

#define isFilenameChar(c) (isalnum(c) || c == '_' || c=='.' || c=='/' || c=='+' || c=='-' || c=='$')

String DaliView::parseFileToken(Int &lines)
{
	idCheck();
	// Ideafix  style 			->  "file", line 999, error_description
	// cc && CC style 			->  file:999: error_description
	// Zortech && QNX`s cc style ->  file(999) : Error : error_description
    	
	String tmp = cursor.value();

	Int from,to;
	from = to = cursor.column();

    // Move to next word if necessary
	while (from < tmp.length() && !isFilenameChar(tmp[from]))
		from++;
		
	to = from;

	if (from >= tmp.length() || !isFilenameChar(tmp[from]))
		from = to = ERR;

	while (from >= 0 && isFilenameChar(tmp[from]))
		from--;

	from++;

	while (to < tmp.length() && isFilenameChar(tmp[to]))
		to++;

	if (from == ERR || to == ERR)
		return NULL_STRING;

	String name = tmp(from,to-from);

	tmp.lTrim();

	// Find first digit to the right of the fileName found
	from = to+1;  // Now 'from' is the position to start searching for digits
	while (from < tmp.length() && !isdigit(tmp[from]))
		from++;
		
    to	 = from;
	while (to < tmp.length() && isdigit(tmp[to]))
		to++;

	lines = toInt(String(tmp(from,to-from))) - 1;
	if (lines < 0 || lines == INT_MAX)
		lines = 0;
	
	return name;
}

bool DaliView::isAborted()
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

// end of view																																																																											
