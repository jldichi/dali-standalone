/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: cell.h,v 1.1.1.1 1996/04/16 18:51:43 gustavof Exp $
*
* DESCRIPTION: Class definitions for cell type.
*
*********************************************************************/

#ifndef CELL_H
#define CELL_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <winapp/winobj.h>

class StrHistory;

class Cell : public WinObj
{
//private:
	Cell(const Cell &);
	
protected :

	String datum;  			// buffer para el ingreso de datos
	Int opt;				// opciones para el ingreso
	Int length;				// longitud real del campo
	Int olength;			// longitud de salida del campo
	Int ndec;				// cantidad de decimales
	String tstmask; 		// mascara para el test de ingreso
	String omask;			// mascara para el despliegue del campo
    StrHistory *history;
    
public :

	// constructores y destructores

	Cell(Int o, Int l, Int ol, Int nd, String tm, String om, 
		 Int fo=ERR, Int co=ERR, StrHistory *his = 0);
	virtual ~Cell();

	// metodos varios 
	void writeStr();

	// metodos virtuales

	virtual bool display(Int fo=ERR, Int co=ERR);
	virtual Int  execute(UnChar c=0)=0;
};

class StrCell : public Cell
{
//private:
	StrCell(const StrCell &);
	
public :	

	// constructores y destructores

	StrCell(Int l, Int ol, Int o=0, Int fo=ERR, Int co=ERR, 
			StrHistory *his = NULL);
			
	virtual ~StrCell();

	// metodos varios
	String &value()							{ idCheck(); return datum; }
	void   setValue(const String &val)		{ idCheck(); datum = val; }
	
	// metodos virtuales
	virtual bool display(Int fo=ERR, Int co=ERR);
	virtual Int execute(UnChar c = 0);
};

class FStrCell : public StrCell
{
//private:
	FStrCell(const FStrCell &);
	
public :	

	// constructores y destructores

	FStrCell(String mask, Int o=0, Int fo=ERR, Int co=ERR);
	virtual ~FStrCell();

};
#endif
