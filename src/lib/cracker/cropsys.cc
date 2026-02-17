#include <ideafix.h>
#include <cfix.h>

#include <cracker/prcmd.h>
#include <cracker/pserconn.h>

extern "C" int CrackerWiExecCmd(char *title, char *cmd,int wait)
{
	if(!crackerClient())
		return WiExecCmd2(title, cmd, wait);
	Int flags = 0;
	prServerConn(PrCmd::wiExecCmd) << String(title) << String(cmd) << flags << (Int)wait;
	return prServerConn.result();
}


extern "C" int CrackerExecShell(char *cmd)
{
	if(!crackerClient())
		return ExecShell(cmd);
	Int flags = 0;
	prServerConn(PrCmd::execShell) << String(cmd) << flags;
	return prServerConn.result();
}


extern "C" int CrackerReceiveFile(char *localfile, char *remotefile)
{
	if(!crackerClient())
		return OK;
	Int flags = 0;
	String file = remotefile;
#ifdef HAVE_TEXT_FILES
	char *mode = "rb";
#else
	char *mode = "r";
#endif

#ifdef HAVE_DIFFERENT_SLASH
		file = file.replace(PATH_SLASH,PATH_SLASH_UNIV);
#endif
	FILE *f = fopen(localfile,mode);
	if(!f)
		return ERR;
	prServerConn(PrCmd::prReceiveFile) <<  file  << flags;
	Int nbytes=0;
	char buff[TRANSCHUNK];
	if(f) {
		while ((nbytes = fread(buff, 1, TRANSCHUNK, f)) != 0)
			prServerConn.push(buff, nbytes);
	}
	prServerConn.push(buff, nbytes);

	fclose(f);
	return prServerConn.result();
}

extern "C" int CrackerSendFile(char *localfile, char *remotefile)
{
	if(!crackerClient())
		return OK;
	Int flags = 0;
	String file = remotefile;
#ifdef HAVE_TEXT_FILES
	char *mode = "wb";
#else
	char *mode = "w";
#endif

#ifdef HAVE_DIFFERENT_SLASH
		file = file.replace(PATH_SLASH,PATH_SLASH_UNIV);
#endif
	FILE *f = (FILE*) ERR;
	prServerConn(PrCmd::prSendFile) <<  file  << flags;
	char buff[TRANSCHUNK];
	Int nbytes;
	forever {
		prServerConn >> nbytes;
		if (nbytes == ERR) {
			assert(f == (FILE *) ERR);
			return ERR;
		}
		if(f == (FILE*) ERR)
			f = fopen(localfile, mode);
		if (nbytes == 0)
			break;
		assert(nbytes <= TRANSCHUNK);
		prServerConn.pop(buff, nbytes);
		if(f)
			fwrite(buff, 1, nbytes, f);
	}
	if(f) {
		fclose(f);
		return OK;
	}else
		return ERR;
}



extern "C" int CrackerGetClipboardData(char *buf, int maxlen)
{

	return ERR;
}
