/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: stat.cc,v 1.6 1998/10/15 16:41:10 mauro Exp $
*
* DESCRIPTION
*********************************************************************/

#include <ifound/str.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef __NT__
#	undef ClosePrinter
#	undef CreateCursor
#	include <windows.h>
#endif

#ifdef __NT__
#ifdef HAVE_UNDERSCORE_NAMES
#	define	stat _stat
#endif

int ntstat(char *fil, struct stat *sstat)
{
	HANDLE hfile;
	BY_HANDLE_FILE_INFORMATION file_info;

	int stat_res = stat(fil, sstat);

	if (stat_res == ERR)
		return stat_res;
		
	hfile = CreateFile(
				fil,
				GENERIC_READ|GENERIC_WRITE,
				FILE_SHARE_READ|FILE_SHARE_WRITE,
				0,
				OPEN_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

	if (hfile != INVALID_HANDLE_VALUE &&
		GetFileInformationByHandle(hfile, &file_info)){
		//sstat->st_dev = file_info.dwVolumeSerialNumber;
		sstat->st_ino = file_info.nFileIndexLow;
	}
	CloseHandle(hfile);
	return stat_res;
}
#endif

int statFile(const String &fil, struct stat *sstat)
{
	static struct stat st = { 0 };
	if (sstat == NULL)
		sstat = &st;

	if (!fil)
		return ERR;
#ifndef __SC__
	return stat(toCharPtr(fil), sstat);
#else
	return ntstat(toCharPtr(fil), sstat);
#endif
}
