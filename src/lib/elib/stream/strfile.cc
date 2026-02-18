/********************************************************************
* Copyright (c) 2026 Master_Solutions S.R.L.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Master_Solutions S.R.L.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: strfile.cc,v 1.2 1998/04/27 18:50:18 mauro Exp $
*
* DESCRIPTION:	Ideafix Stream file class implementation
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <ifound/itypes.h>
#include <local/strfile.h>
#include <ifound/stdc.h>

// InputFile implementation

InputFile::InputFile(short bufsize)
: InputStream(bufsize)
{
	osf = NULL;
}

InputFile::InputFile(OsFile &of, short bufsize)
: InputStream(bufsize)
{
	osf = &of;
	isalloc = false;
}

InputFile::InputFile(const String &fname, const String &fext,
					short bufsize)
: InputStream(bufsize)
{ 
	osf = tr_new OsFile;
	isalloc = true;
	open(fname, fext);
}

bool InputFile::open(const String &fname, const String &fext)
{
	if (osf == NULL) {
		osf = tr_new OsFile;
		isalloc = true;
	} else
		osf->close();
	if (osf->open(NULL_VALUE, fname, fext, OsFile::READ|OsFile::FAIL))
		reset();
	else {
	state = IO_ERROR;
		return false;
	}

	return true;
}

InputFile::InputFile(const String &fpath, const String &fname, 
					const String &fext, short bufsize)
: InputStream(bufsize)
{ 
	String l_path = fpath;

	if (l_path == NULL_VALUE)
		l_path = String("PATH").getEnv();

	Int rv = false;
	StrCursor pathDelim(l_path, String(1, PATH_SEP[0]));
	String p, dot = String(".")+PATH_SLASH;

	osf = tr_new OsFile;

	while (pathDelim.ok()) {

		p = pathDelim.value();		// get the string

		if (p == NULL_STRING )
			p = dot;
		else if (p[p.length() - 1] != PATH_SLASH[0]) 
			p += PATH_SLASH[0];

		if ((rv = osf->open(p, fname, fext, 
					OsFile::READ|OsFile::FAIL)) != false) 
			break;

		pathDelim++;
	}
	if (rv == false) state = IO_ERROR;
	isalloc = true;
}

InputFile::~InputFile() 
{
	if (osf && isalloc) 
		tr_delete(osf);
}

char InputFile::fill()
{
	if (state & IO_ERROR)
		return 0;

	if ((cnt = short(osf->read(ptr = base, offset, size))) > 0 ) {
		offset += cnt--;
		return *ptr++;
	}
	state |= cnt < 0 ? IO_ERROR : END_OF_INPUT;
	return 0;
}

// Move the file pointer to some position inside the file

Int InputFile::seek(Int position)
{
	if (position < offset - cnt - (ptr - base) || position >= offset
		|| state & END_OF_INPUT) {
		state &= ~END_OF_INPUT;
		offset = position - position % size;
		fill();
	}

	// now readjust cnt and ptr

	short new_cnt = short(offset - position);
	ptr += cnt - new_cnt;
	cnt = new_cnt;

	return tell();
}

// OutputFile implementation

OutputFile::OutputFile(short bufsize)
: OutputStream(bufsize)
{
	osf = NULL;
}

OutputFile::OutputFile(OsFile &of, short bufsize)
: OutputStream(bufsize)
{
	osf = &of;
	isalloc = false;
}

OutputFile::OutputFile(const String &fname, const String &fext, 
					short cflag, short bufsize) 
: OutputStream(bufsize)
{ 
	osf = tr_new OsFile;
	isalloc = true;

	open(fname, fext, cflag);
}

bool OutputFile::open(const String &fname, const String &fext, 
					short cflag) 
{
	if (osf == NULL) {
		osf = tr_new OsFile;
		isalloc = true;
	} else
		osf->close();

	if ((osf->open(NULL_VALUE, fname, fext, (cflag == TRUNC_FILE) ? 
			OsFile::WRITE|OsFile::TRUNC|OsFile::CREAT : 
			OsFile::WRITE|OsFile::CREAT|OsFile::APPEND)) == false) {
		state = IO_ERROR;
		return false;
	}

	return true;
}

OutputFile::~OutputFile()
{
	flush();
	if (osf && isalloc) 
		tr_delete(osf);
}

void OutputFile::flush()
{
	if (state & IO_ERROR)
		return;

	Int n = ptr - base;

	if (n && osf->write(ptr = base, offset, n) != n) 
		state |= IO_ERROR;
	else {
		cnt = size - 1;
		offset += n;
	}
}

// Move the file pointer to some position inside the file

Int OutputFile::seek(Int position)
{
	flush();				// flush current page
	offset = position;		// update tr_new offset
	return tell();
}

// end of strfile
