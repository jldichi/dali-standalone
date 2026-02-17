/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: ifdfiles.h,v 1.1.1.1 1996/04/16 18:51:49 gustavof Exp $
*
*
* DESCRIPTION:
*				InputFdFile file class interface
*
*********************************************************************/

#ifndef IFDFILES_H
#define	IFDFILES_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>
#include <local/istream.h>

class InputFdFile : public InputStream {

// private:

	InputFdFile(const InputFdFile &);
	
public:

	int fd;

	explicit InputFdFile(short fd, short bufsize = DEF_BUFSIZE);
	virtual ~InputFdFile() {}

	virtual char fill();

};

extern InputFdFile stdIn;

#endif
