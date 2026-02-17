/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* finpath.cc,v 1.1.1.1 1996/04/16 18:51:58 gustavof Exp
*
* DESCRIPTION
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
* findInPath
*********************************************************************/

#include <ifound/str.h>
#include <ifound/priv/gfuncs.h>
#include <ifound/stdc.h>
#if !IDEAWIN || IDEADLL
bool isAbsolutePath(const String &path)
{
	// Hay dos: una es esta y la otra esta en String, habria que sacar una!
	return 	path[0] == PATH_SLASH_UNIV[0] 	||
			path[0] == PATH_SLASH[0]		||
			(path[0] == '.'
				&& (path[1] == PATH_SLASH_UNIV[0] || path[1] == PATH_SLASH[0]))
#ifdef VOLUME_LETTER_SEP
			|| path[1] == VOLUME_LETTER_SEP[0]
#endif
		;
}
#endif // !IDEAWIN || IDEADLL

#if !IDEADLL
bool findInPath(const String &name, String ext, const String &path,
				String &fullname)
{
	static String *pathVar = NULL;
	String _path;

	/*
	* Check if file name has extension. If it has, ignore the one
	* supplied as argument.
	*/
	String extension = name.extName();
	if (extension != NULL_STRING)
		ext = NULL_STRING;

	// Check if absolute reference (name begins with SLASH)
	fullname = name + ext;
	if (isAbsolutePath(name)) {
		return statFile(fullname) == OK;
	}
	else {
#ifdef HAVE_CDINPATH
		if (statFile(fullname) == OK)
			return true;
#endif
		// if I have the path parameter use it
		if (path != NULL_STRING)
			_path = path;
		else {
			// else read the PATH environment variable (only once)
			if (pathVar == NULL) {
				pathVar = tr_new String;
				String var = String("PATH").getEnv();
#ifndef IDEAWIN
				assert(var != NULL_STRING);
#endif
#ifndef HAVE_CDINPATH
				if (var[0] == PATH_SEP[0])
					*pathVar = ".";
#endif
				*pathVar += var;
			}
			_path = *pathVar;
			if(PATH_SLASH[0] != PATH_SLASH_UNIV[0])
			   _path = _path.replace("\\","/");
		}
	}
	// Try to open the file for all directories in the PATH
	for (StrCursor c(_path, PATH_SEP); c.ok(); c++) {
		String fullName = c.value().expandEnvVars();
		// Avoid an unnecessary SLASH. DOS complains about it !!
		if (fullName != NULL_STRING &&
				fullName[fullName.length()-1] != PATH_SLASH[0])
			fullName += PATH_SLASH;
		fullname = fullName + name + ext;
		/* If Succesfull, break the loop						*/
		if (statFile(fullname) == OK)
			return true;
	}
	return false;
}

String findInPath(const String &filename, const String &path)
{
	String ret;
	if (findInPath(filename, "", path, ret))
		return ret;
	return filename;
}
#endif // !IDEADLL
