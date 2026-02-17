/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: text.cc,v 1.6 2004/09/29 21:07:53 damianm Exp $
*
* DESCRIPTION: Implementation of Text class
*
*********************************************************************/
#ifdef __GNUC__
#pragma implementation
#endif

#include <string.h>

#include <stdio.h>
#include <winapp/text.h>
#include <winapp/window.h>
#include <local/osfile.h>
#include <local/strfile.h>
#include <sys/stat.h>
#include <winapp/waitmsg.h>
#include <ifound/stdc.h>

// Variables y metodos estaticos

Int Text::tabSize_ = 4;

// Constructores
Text::Text() 
: List(String)(NULL_STRING)
{ 
	idCheck();

	active		= false;
	cview		= 0;
	filename	= NULL_STRING;
	ttype		= READONLY;
}

Text::Text(const String &fileN)
: List(String)(NULL_STRING)
{
	idStart();

#ifdef __VC__
    struct _stat st;
#else
    struct stat st;
#endif    

	active 	 	= false;
	filename 	= fileN;
	ttype 	 	= READONLY;         

	// Si no existe el archivo trato de cargar el administrado	
    if ( statFile(fileN,&st) != OK )
    	loadAdminFile(fileN);
    	  
	cview = 0;
}

Text::Text(const String &cmd , bool abortable)
: List(String)(NULL_STRING)
{                                      
	idStart();
	cview 		= 0;
	ttype 		= ISTREAM;
	active      = true;
	filename 	= cmd;
	fileperm 	= PERM_READ;
	existfile 	= true;
	
	// If text is not abortable process it now
	if ( ! abortable ) {
	    WaitMsg pleaseWait("Processing InputStream ...", NULL_STRING);
	    pleaseWait.execute();

		String aux;
        InputPipe ip(cmd);
        
		while (ip >> aux) 
			add(encodeTabs(aux));
			
		resetModif();
	}
}

Text::~Text()
{ 
	idCheck();
	idStop();
}

bool Text::loadAdminFile(const String &f)
{
	idCheck();

	String pipeline;
	InputPipe getadm(toString("checkout -p ")+f);

	while(getadm >> pipeline) 
		add(encodeTabs(pipeline));

	if(getadm.withError()) {
		delAll();
		return false;       
	}

	ttype				= ADMIN;
	filename			= f;
	fileperm			= PERM_READ;
	existfile			= true;
	active				= true;   
	encodeAttrs();
	resetModif();
	
	return true;
}

void Text::setFile(const String &f, bool loadNewFile) // IN DERIVED ?
{ 
	idCheck();

	if (!active && !isType(ISTREAM | ADMIN)) {
		ttype 		= READONLY;
		filename 	= f;
	} 
	else {                 
		ttype 		= READONLY;
		filename 	= f;
        
		if (loadNewFile) 
			load();

		resetModif();
	}
}

bool Text::load()
{       
	idCheck();

	// Si el tipo es istream o admin ya tengo el texto 
	if (!active || ttype&(ISTREAM|ADMIN)) 
		return false;

	delAll();

	InputFile inputf(filename);

	String aux;
	for(Int i = 0; inputf >> aux; i++) 
		add(encodeTabs(aux));

	// Codifico los atributos
	encodeAttrs();

	resetModif();

	return true;
}

String Text::encodeTabs(String s)
{
	idCheck();

	Int len = s.length(); 
	for (Int i = 0; i < len; i++) {
		if (s[i] == '\t') {
			Int cant = NFILLTAB(i);
			for (Int j = 0; j < cant; j++)
				s.insChar(FILL_TAB, i);
			i += cant;
			len += cant;
		}
	}
	return s;
}

String Text::decodeTabs(String s)
{
	idCheck();

	Int len = s.length(); 
	Int i = 0;

	while (i < len) {
		if (s[i] != FILL_TAB) 
			i++;
		else {
			s.delChar(i);
			len--; 
		}
	}
	return s;
}

void Text::encodeAttrs(Int filfrom, Int colfrom, Int filto, Int colto)
{
	idCheck();

	Int col, curratr;

	// obtengo la longitud de los atributos
	int nlen  = strlen(ATTR_NORMAL);
	int battr = strlen(BEGIN_ATTR);

	// veo los limites
	TextCursor begin(*this);
	TextCursor end(*this);
	end.goToLast();end.goToCol(end.value().length());

	if (filfrom != ERR && colfrom != ERR && filto != ERR && 
												colto != ERR) {
		begin.goTo(filfrom, colfrom);
		end.goTo(filto, colto);
	}

	// recorro todo el texto
	for (TextCursor tc = begin; tc < end; tc++) {

		// recorro cada linea en busca de un escape
		col = 0;
		while ((col=tc.value().index(*BEGIN_ATTR, col))!=ERR) {

			// verifico si es valido	
			const char *str = toCharPtr(tc.value())+col;

			// si es un atributo normal lo borro y salteo
			if (strncmp(str, ATTR_NORMAL, nlen)==0) {
				tc.goToCol(col);
				for (Int i = 0; i < nlen; i++)
					tc.delChar();
				tc.goToCol(0);
				continue;
			}

			// busco si abre algun atributo
			if (strncmp(str, BEGIN_ATTR, battr) == 0) {

				Int skip;
				if ((curratr = sequenceToAttrib(str, skip)) !=
						ERR) {
					tc.goToCol(col);
					while (skip--)
						tc.delChar();
					tc.goToCol(0);
				} else {
					col += skip;
					continue;
				}
			} else {
				col++;
				continue;
			}

			// busco el cierre del atributo
			bool find = false;
			TextCursor tcur = tc;
			for (tcur.goToCol(col); tcur && !find; tcur++) {

				Int tcol = tcur.column();

				// busco el fin de la secuencia
				while ((tcol=tcur.value().index(*BEGIN_ATTR, tcol))!=
																ERR) {
					str = toCharPtr(tcur.value())+tcol;

					// encontre la sequencia que cierra
					if (strncmp(str, ATTR_NORMAL, nlen)==0) {
						tcur.goToCol(tcol);
						tcur.moveCol(-1);
						tc.goToCol(col);
						setAttr(tc, tcur, curratr);
						tc.goToCol(0);
						find = true;
						break;
					}
					else tcol++;
				}

				tcur.goToCol(0);
			}
			// no lo encontre, lo mando al final del texto
			if (!find) {
				end.moveCol(-1);
				tcur.goTo(end.order(), end.column());
				tc.goToCol(col);
				setAttr(tc, tcur, curratr);
				tc.goToCol(0);
			}
		}
	}
}

String Text::decodeAttrs(TextCursor &tc, Int &curratr, Int cant)
{
	idCheck();

	Int len = tc.value().length();
	Int svcol;

	// chequeo la columna
	if (len <= (svcol = tc.column()))
		return NULL_STRING;

	// corrijo el cant
	if (cant != ERR && cant + svcol > len)
		cant = len - svcol;

	Int col, attr, desp, nlen = strlen(ATTR_NORMAL);

	// tomo los datos necesarios 
	String aux = tc.value()(svcol, 
							len = (cant == ERR ? len - svcol : cant));

	// el desplazamiento para lo que inserto
	desp = 0;

	do {
		// si el atributo es distinto -> opero
		if ((attr = getAttr(tc, col)) != curratr) {
							
			// debo cerrar la secuencia
			if (curratr != A_NORMAL) {
				char *seq = ATTR_NORMAL;
				for (Int c=tc.column()+desp-svcol; *seq;seq++,c++)
					aux.insChar(*seq, c);
				desp += nlen;
			}

			// ahora abro la nueva secuencia
			String strseq = attribToSequence(attr);

			if (strseq) {
				const char *seq = toCharPtr(strseq);
				for (Int c = tc.column() + desp - svcol; *seq; seq++, c++)
					aux.insChar(*seq, c);
				desp += strseq.length();
			}
		}

		// muevo el tc hasta donde termina el atributo
		tc.goToCol(col+1);

		// modifico el atributo corriente
		curratr = attr;

	} while (col-svcol < len-1);

	// restauro la columna
	tc.goToCol(svcol);

	return aux;
}

void Text::setAttr(TextCursor &b, TextCursor &e, Int attr)
{
	idCheck();

	if (attrzn.setAttribute(b, e, attr))
		if (attr != A_STANDOUT)
			setModif();
}

void Text::delAttr(TextCursor &b, TextCursor &e, Int attr)
{
	idCheck();

	if (attrzn.delAttribute(b, e, attr))
		if (attr != A_STANDOUT)
			setModif();
}

bool Text::activate()
{   
	idCheck();

	// No corresponde si ya esta activo o es de tipo istream o admin
	if (active || (ttype&(ISTREAM|ADMIN)))
		return false;
	active = true;
	load();
	return true; 
}

void Text::deActivate()
{    
	idCheck();

	// No corresponde si no esta activo o es de tipo istream o admin
	if (!active || ttype&(ISTREAM|ADMIN)) 
		return;

	delAll();
	resetModif();

	ttype 	= READONLY;
	active 	= false;
}

void Text::changeType(TextType ty)
{
	idCheck();

	if (ttype & ty)
		ttype = TextType(ttype & (~(ty)));
	else 
		ttype = TextType(ttype | ty);
}

Int Text::getAttr(TextCursor &tc, Int &hasta)
{
	idCheck();

	return attrzn.getAttribute(tc, hasta);
}

struct ScreenAttribute {
	char *str;
	Int attr;
	char *seq;
};

static ScreenAttribute attrs[] = {
	{ "Bold",			A_BOLD,			"1" 	},
	{ "Underline",		A_UNDERLINE,	"4" 	},
	{ "Blink",			A_BLINK,		"5"		},
	{ "Reverse",		A_REVERSE,		"7" 	},
	{ "Red",			A_RED,			"31" 	},
	{ "Green",			A_GREEN,		"32" 	},
	{ "Blue",			A_BLUE,			"34"	},
	{ "Yellow",			A_YELLOW,		"33" 	},
	{ "Cyan",			A_CYAN,			"36"	},
	{ "Magenta",		A_MAGENTA,		"35"	},
	{ "White",			A_WHITE,		"37" 	},
	{ "RedBg",			A_RED_BG,		"41;1"	},
	{ "GreenBg",		A_GREEN_BG,		"42" 	},
	{ "BlueBg",			A_BLUE_BG,		"44"	},
	{ "YellowBg",		A_YELLOW_BG,	"43" 	},
	{ "CyanBg",			A_CYAN_BG,		"46;1" 	},
	{ "MagentaBg",		A_MAGENTA_BG,	"45"	},
	{ "WhiteBg",		A_WHITE_BG,		"47" 	}
};                   

#define	NATTRS	Int(sizeof(attrs)/sizeof(attrs[0]))

Int	Text::sequenceToAttrib(const char	*str, Int &skip)
{
	idCheck();

	const char	*backstr = str;
	int l,	i;
	Int cattr = 0;

	//	avanzo para	buscar los atributos
	str +=	sizeof(BEGIN_ATTR)-1;

	forever {
		for (i = 0; i	< NATTRS; i++) {
			if (strncmp(str,	attrs[i].seq, l	= strlen(attrs[i].seq))	== 0) {
				cattr |= attrs[i].attr;
				str	+= l;
				break;
			}
		}
		if (i	== NATTRS) break;
		if (*str == SEPAR_ATTR)
			str++;
		else
			break;
	}

	skip =	str	- backstr;

	//	era	una	secuencia valida
	if	(*str == END_ATTR) {
		skip++;
		return cattr;
	}

	return	ERR;
}

#define	COLOR(a)	((a) &	0x00FF0000)

String Text::attribToSequence(Int attr)
{
	idCheck();

	String	strseq = BEGIN_ATTR;
	bool hasSomething = false;
	Int i;

	for (i	= 0; i < NATTRS; i++) {

		if (COLOR(attrs[i].attr))	{
			if (COLOR(attr) != COLOR(attrs[i].attr))
				continue;
		}	else if	(!(attr	& attrs[i].attr))
			continue;

		if (hasSomething)	strseq += SEPAR_ATTR;
		hasSomething = true;
		strseq +=	attrs[i].seq;
	}

	if	(strseq	!= BEGIN_ATTR) {
		strseq +=	END_ATTR;
		return strseq;
	}

	return	NULL_STRING;
}

// end of text
