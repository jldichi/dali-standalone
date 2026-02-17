/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: ofdfiles.h,v 1.1.1.1 1996/04/16 18:51:49 gustavof Exp $
*
*
* DESCRIPTION:
*				OutputFdFile file class interface
*
*********************************************************************/

#ifndef	OFDFILES_H
#define	OFDFILES_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>
#include <local/istream.h>

class OutputFdFile : public OutputStream {

// private:

	OutputFdFile(const OutputFdFile &);
	
public  :

	int fd;
	explicit OutputFdFile(short fd, short bufsize = DEF_BUFSIZE);
	virtual ~OutputFdFile() { flush(); }
	
	virtual void flush();

	void endMsg();
	Int pause();
	Int pause(const String &o1, const String &o2);

};

extern OutputFdFile stdOut, stdErr;

#endif

