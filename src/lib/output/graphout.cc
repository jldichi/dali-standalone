/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* graphout.cc,v 1.1.1.1 1996/04/16 18:52:07 gustavof Exp
*
* DESCRIPTION
*********************************************************************/

#include <string.h>

#include <ideafix.h>
#include <cfix.h>
#include <cfix/gn.h>


#include <ifound/str.h>
#include <ifound/parray.h>

#include <ideafix/priv/graphout.h>
#include <ideafix/priv/gndefs.h>

declare(PtrArray, BaseOutputColumn);

#define columns	(*columns_)

#define GWIDTH_DEF	80
#define PWIDTH_DEF	80
#define GLENGTH_DEF	66
#define PHEIGHT_DEF	66

GraphOutput::GraphOutput(Int Type, const String &gt,  const String &gst,
						 const String &glx, const String &gly, bool direc,
						 bool toprinter, Int grwidth, Int grlength,
						 Int frwidth, Int frlength, Int ncopies)
:	gtitle(gt),
	gstitle(gst),
	glabelx(glx),
	glabely(gly),
	gty(Type),
	wp(ERR),
	backgr(A_NORMAL),
	inverted(direc)
{
	if (toprinter) {
		dest = OUT_PRINTER;
		fp = OpenPrinter(ncopies ? ncopies : 1, "");
		gwidth  = grwidth == ERR ? GWIDTH_DEF : grwidth;
		pwidth  = frwidth == ERR ? PWIDTH_DEF : frwidth;
		glength = grlength == ERR ? GLENGTH_DEF : grlength;
		pheight = frlength == ERR ? PHEIGHT_DEF : frlength;
	} else {
		dest		= OUT_TERMINAL;
		WiConnect(NULL, ProcArgs()[0], "");
		gwidth		= (grwidth == ERR) ?
						(UnShort) WiWidth(SCREEN) - 2 : grwidth;
		pwidth		= (frwidth == ERR) ?
						pwidth  = (UnShort) WiWidth(SCREEN) - 2 : frwidth;
		glength		= (grlength == ERR) ?
						pheight = (UnShort) WiHeight(SCREEN) - 1 - 2 : grlength;
		pheight		= (frlength == ERR) ?
						pheight = (UnShort) WiHeight(SCREEN) - 1 - 2 : frlength;
	}
}

GraphOutput::~GraphOutput()
{
	if (dest == OUT_PRINTER)
		ClosePrinter(fp);
	if (lines) {
		for (Int i = 0; i < MAX_VAL; i++)
			Free(lines[i]);
		Free(lines);
	}
}

Int GraphOutput::end()
{
	Int j, k, n, ser, Titulo, ret, i, CantSers, graphError;

	if (!items)
		return (-G_ERR2);

	if (dest == OUT_TERMINAL)
		ret = SetearPantalla();
	else
		ret = SetearImpresora(gwidth, glength, pwidth, pheight);
	if (ret != OK)
		EndGrafics();
	else {
		InitDatos();
		InitOpciones();
		SetearTipo(gty);
 		SetearTitulo(gtitle != NULL_STRING ? toCharPtr(gtitle) : "", GR_CEN);
		SetearSubtitulo(gstitle != NULL_STRING ? toCharPtr(gstitle) : "", GR_CEN);
		SetearLabelX(glabelx != NULL_STRING ? toCharPtr(glabelx) : "");
		SetearLabelY(glabely != NULL_STRING ? toCharPtr(glabely) : "");

		for(i = 0, CantSers = 0; i < ncols ; i++) {
			if (columns[i]->ty==TY_NUMERIC || columns[i]->ty==TY_FLOAT){
				if (!inverted && CantSers < MAX_SER)
					SetearLeyenda(CantSers++, toCharPtr(columns[i]->head));
				else if (inverted)
					SetearRangoX(CantSers++, toCharPtr(columns[i]->head));
			}
		}
		if (!CantSers)
			return (-G_ERR2);

		n = inverted ? MAX_SER : MAX_VAL;
		for (k=0; k<items && k < n; k++) {
			Titulo = false;
			for (j=0,ser = 0;j<ncols && ser < CantSers;j++) {
				if (columns[j]->ty==TY_FLOAT || columns[j]->ty == TY_NUMERIC)	{
					double aux = StrToF((char *)lines[k]+j*16);
					if (!inverted)
						SetearDato(ser++,k, aux);
					else
						SetearDato(k,ser++, aux);
				}
				else if (!Titulo) {
					Titulo = true;
					if (!inverted)
						SetearRangoX(k,(char *)lines[k]+j*16);
					else 
						SetearLeyenda(k,(char *)lines[k]+j*16);
				}
			}
		}
		if (dest == OUT_TERMINAL) {
			if ((graphError = Graficar()) != OK)
				return (-graphError);
		}
		else {
			if ((graphError = PrintScreen(fp)) != OK) 
				return (-graphError);
		}
	}
	return items;
}

void GraphOutput::set(Int gw, Int gl, Int fwidth, Int flength)
{
	dest    = OUT_PRINTER;
	pwidth  = fwidth;
	pheight = flength;
	gwidth  = gw;
	glength = gl;
}

Int GraphOutput::operator<<(const char *bp)
{
	if (!lines) {
		assert(ncols > 0);
		lines = (UChar **) Alloc(MAX_VAL*sizeof(UChar *));
		for (Int i = 0; i < MAX_VAL; i++)
			lines[i] = (UChar *) Alloc(ncols*16);
	}
	assert(currcol < ncols);
	if (items < MAX_VAL)  {
		strncpy((char*)lines[items]+currcol*16, bp,15);
		*(lines[items]+(currcol+1)*16-1) = 0x00;
		if (++currcol >= ncols) {
			currcol = 0;
			items++;
		}
	} else if (items >= MAX_VAL) 
		return ERR;
	return OK;
}

GraphOutput *GraphOutput::getGraphOutput()
{ 
	return this; 
}

