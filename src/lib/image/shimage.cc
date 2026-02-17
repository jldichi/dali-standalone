/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: shimage.cc,v 1.4 1999/02/16 21:55:12 mauro Exp $
*
* DESCRIPTION
*********************************************************************/

#include <ifound.h>
#include <ideafix.h>
#include <cfix.h>
#include <cfix/types.h>
#include <cfix/db.h>
#include <ideafix/priv/dbdefs.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cracker/prcmd.h>
#include <cracker/pserconn.h>
#ifdef HAVE_UNDERSCORE_NAMES
#	define stat _stat
#endif


#ifdef HAVE_GRAPHICS
#	include <mswi/iwwidefs.h>
#	include <mswi/iwwin.h>
#	include <image/iximage.h>
#	include <image/tifimage.h>
#	include <image/gifimage.h>
#endif
int sendImage(const String &dir, const String &name, const String &title,const Time &ts, Int size, Int type, Int flags, void *buf, Int x, Int y);
static void getRecordId(dbfield fld, long &cid, long &iid );

#define MAX_CHARS	41
static char keytab[MAX_CHARS] = {0};

static void init_keytab()
{
	register int i;
	for(i = 0; i < 10;i++)
		keytab[i]='0'+i;
	for(i = 11; i < 37; i++)
		keytab[i] = 54 + i; // 'A' - 11 + i;
	keytab[36] = '(';
	keytab[37] = ')';
	keytab[38] = '{';
	keytab[39] = '}';
	keytab[40] = '_';
}

// short to 'n' characters
static void s2nc(UShort s, char *p, int n)
{
	assert(n == 2 || n ==3);
	UShort register div = (n == 3) ? MAX_CHARS * MAX_CHARS : MAX_CHARS ;
	for ( int i=0; i < n-1; i++) {
		*(p+i) = keytab[s/div];
		s = s%div;
		div /= MAX_CHARS;
	}
}

// Generate unique directory and file name from 3 longs and 1 short

void genCacheName(Int i1, Int i2, Int i3, short i4, char *dir, char *file)
{
	if(! *keytab)
		init_keytab();
	s2nc((UShort) (i1 & 0xFFFF),			 dir,	3);
	s2nc((UShort) ((i1 & 0xFFFF0000) >> 16), dir+3,	3);
	s2nc((UShort) ((i2 & 0xFFFF0000) >> 16), dir+6,	3);
	s2nc((UShort) ((i2 & 0xFF00) >> 8), 	 dir+9,	2);
	dir[11] = '\0';
	s2nc((UShort) (i2 & 0xFF), 	 			 file,		2);
	s2nc((UShort) (i3 & 0xFFFF),			 file+2,	3);
	s2nc((UShort) ((i3 & 0xFFFF0000) >> 16), file+5,	3);
	s2nc((UShort) (i4 & 0xFFFF),			 file+8,	3);
	file[11] = '\0';
}

#ifdef HAVE_GRAPHICS
int showImage(const String &path, const String & title,BinaryType ty,int x,int y)
{
	IxImage *img = NULL;
	HWND hWnd = NULL;
    IWForm * wptr = __GetIWForm(WiCurrent());
	if(wptr)
		hWnd = wptr->GetHWnd();
	assert (path != NULL_STRING);
	if(ty == BINARY_UNKNOWN)
		return IO_NO_BINARY;
	switch(ty) {
		case BINARY_GIF:
			img = new GifImage(hWnd,__hAppInstance);
			break;
		case BINARY_TIFF:
			img = new TiffImage(hWnd,__hAppInstance);
			break;
		case BINARY_BMP:
			img = new BitmapImage(hWnd,__hAppInstance);
			break;
		default:
			return IO_NO_BINARY;
	}
	(void) img->load(toCharPtr(path));
	img->title(toCharPtr(title));
	if(!img->ok()||img->show(x,y) != imageOK){
		img->error();
		delete img;
		return IO_NO_BINARY;
	}
	return img->imageid();
}
#endif


extern "C" void CloseImage(int id)
{
	if(!crackerClient()){
#ifdef HAVE_GRAPHICS
		if(id == -1){
			IxImage::closeall();
			return;
		}
		IxImage *img= IxImage::find(id);
		if(img)
		 	img->close();
#endif
	}
	else {
		prServerConn(PrCmd::closeImage) << (Int)id;
		prServerConn.flush();
	}

}
extern "C" void CloseAllImages()
{
		CloseImage(-1);
}

extern "C" int ShowImage(char * path, char *title, int x, int y)
{
	struct stat st;
	if (statFile(path, &st) != 0)
		return IO_NO_BINARY;
	BinaryType ty = BINARY_UNKNOWN;
 	String ext = String(path).extName().toUpper();
	if(ext == "TIF"||ext == "TIFF")
		ty = BINARY_TIFF;
	else if(ext == "GIF")
		ty = BINARY_GIF;
	else if(ext == "BMP")
		ty = BINARY_BMP;
	if(!crackerClient()){
#ifdef HAVE_GRAPHICS
		return showImage(path,title,ty,x,y);
#else
		warning("Cannot show image on non-graphics terminal");
		return  IO_NO_BINARY;
#endif
	}
	else {
#ifdef HAVE_TEXT_FILES
		const char *mode = "rb";
#else
		const char *mode = "r";
#endif
		UChar * buf;
	    	Time ts;
		FILE *fp;
		Int res;
		ts = osTime(st.st_mtime);
		fp = fopen(path,
#ifdef HAVE_TEXT_FILES
		"rb"
#else
		"r"
#endif
		);
		if(!fp)
			return IO_NO_BINARY;
		buf = new UChar[st.st_size];
		if(fread(buf,1,st.st_size,fp) < 0)
			return ERR;

		res = sendImage("IMAGE$",String(path).baseFullName(),title,ts,st.st_size,ty,0,buf,x,y);
		delete[] buf;
		return res;
	}
}

#if 1
extern "C" int ShowDbImage(dbfield f, char * title, int x, int y)
{
	Int res;
	if(!crackerClient()){
#ifdef HAVE_GRAPHICS
		warning("Cannot show image on non-graphics terminal");
		return  IO_NO_BINARY;
#else
		warning("Cannot show image on non-graphics terminal");
		return  IO_NO_BINARY;
#endif
	}
	else {
		char dir[12] = {0},file[12]={0};
		UChar * buf;            
		Int cid,iid;
		struct BinaryInfo bi;
		if((res = GetBinaryInfo(f,&bi)) != OK);
				return IO_NO_BINARY;
		if(bi.binaryType != BINARY_GIF &&
			bi.binaryType != BINARY_TIFF &&
			bi.binaryType != BINARY_BMP)
			return IO_NO_BINARY;
		buf = new UChar[bi.actualSize];
		if(!buf)
				return IO_NO_BINARY;
		if((res = GetBinaryToMem(f,buf,bi.actualSize)) != OK){
				delete[] buf;
				return IO_NO_BINARY;
		} 
		getRecordId(f,cid,iid);
		genCacheName(0,cid,iid,(short)f&0xFF00,dir,file);
		res = sendImage(dir,file,title,osTime(bi.timeStamp),bi.actualSize,bi.binaryType,0,buf,x,y);
		delete[] buf;
		return res;
	}
}

#endif

int sendImage(const String &dir, const String &name, const String &title, const Time &ts, Int size, Int type, Int flags, void *buf,Int x,Int y)
{
		Int result;
		prServerConn(PrCmd::showImage) << dir << name << title << ts
			<< size << type << flags << x << y;
		prServerConn >> result;
		if(result != NEEDFILE)
				return result;
		prServerConn.push(buf, size);
		prServerConn.push(buf, 0);

		prServerConn >> result;
		return result;
}


static void getRecordId(dbfield fld, long &cid, long &iid )
{
	record_prefix *buf;
	buf = (record_prefix *)_GetTabBuff(fld &0xFFFFFF00);
	assert(buf != NULL);
	cid = buf->oid.cid;
	iid = buf->oid.iid;
}

