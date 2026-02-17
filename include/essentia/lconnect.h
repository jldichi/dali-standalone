/****************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: lconnect.h,v 1.1.1.1 1996/04/16 18:51:47 gustavof Exp $
*
* DESCRIPTION
* Class definitions for Local Connection type.
*********************************************************************/

#ifndef LCONNECT_H
#define LCONNECT_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound/convconn.h>
#include <essentia/dbconn.h>
#include <essentia/dbmcom.h>

class LocalConnection : public DbConnection
{

// private:

	bool invert;		// Inversion of int type needed...
	Int conv(Int i) 	{ return invert ? invI(i) : i; }

	LocalConnection(const LocalConnection &);
	
public:

	explicit LocalConnection(DbConnection &con);
	LocalConnection();
	~LocalConnection();

	LocalConnection &operator=(DbConnection &con);

	// necesito redefinir estos operadores para manejar
	// los distintos tipos de longs
	Connection &operator<<(Int i)
		{ return DbConnection::operator<<(conv(i)); }
	Connection &operator>>(Int &i)
		{ DbConnection::operator>>(i); i = conv(i); return *this; }

	bool isInvert() { return invert; }

	void begin(Int, SvMessage) { }
	void _flush() { }
	Int _fill() { return 0; }
};

#endif // LCONNECT_H
