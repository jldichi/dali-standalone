/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: iximage.h,v 1.3 1999/02/16 21:52:33 mauro Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef _IMAGE_H
#define _IMAGE_H 1

#ifdef CreateCursor
#	undef  CreateCursor
#endif

#ifdef ClosePrinter
#	undef ClosePrinter
#endif

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>


const int PALVERSION = 0x300;
const int MAXPALETTE = 256;		 /* max. # supported palette entries */
static const int INFOSIZE			= sizeof(BITMAPINFOHEADER);
static const int CORESIZE			= sizeof(BITMAPCOREHEADER);

const char szDibWnd[] = "ImageWindow";
const int MAX_IMAGES  = 256;
class IxImage;

#define Debug(msg) MessageBox(NULL,msg,"Debug",MB_OK);

typedef enum {
	imageOK,
	imageNODATA,
	imageERROPENFILE,
	imageNOTBMP,
	imageNOTTIFF,
	imageNOTGIF,
	imageBADFILEHEADER,
	imageBADINFOHEADER,
	imageBADFORMAT,
	imageNOMEM,
	imageNOWIN,
	imageTOOMANY,
	imageGPF,
	imageNOTPALETTE,
	imageNOMEM4PALETTE,
	imageCANTHANDLE,
	imageCANTHANDLESEP,
	imageCANTDITHER,
	imageBADGIFSD,
	imageBADGIFNOSEP,
	image2BIG,
	imageNODLL,
	imageLastStatus,
} imageStatus;


typedef enum {
	imageBMP,
	imageTIFF,
	imageGIF,
	imageUNKNOWN,
} imageType;

typedef struct {
	HWND hwnd;
	IxImage *img;
} imageList;

class IxImage {
	HPALETTE hPal;
	HANDLE instance;
	HWND 	hWnd;
	HWND	_fatherhWnd;
	static registered;
	static imageList list[MAX_IMAGES];

	imageStatus regimage();
	void unregister();
	imageStatus createPalette();

	static long FAR PASCAL _export DibWndProc (HWND hwnd, UINT message, UINT wParam,
                                                          LONG lParam);
	int minim;
	HBITMAP hbm;
	HCURSOR hcur;
	imageStatus create_bitmap();
	void draw(HDC hdc,short xo, short yo);


protected:

	long _width;
	long _height;
	int _depth;			// number of bits per pixel
	imageStatus _status;
	imageType	_type;

	long 				_imagelen;	// size of image strip
	BYTE 				* _image;

	long 				_binfolen;	// size of (INFO header + palette)
	BITMAPINFOHEADER	* _binfo;
	int 				_npalette;	// number of colors in palette
	int pad;
	char _title[64];
	char fname[256];
	int _imageid;

public:
	IxImage(HWND phwnd, HANDLE appInstance);
	~IxImage();

	int show(int x,int y);

	virtual imageStatus load(char *file) = 0;
	void width(long i) 			{ _width = i;}
	void height(long i) 		{ _height = i;}
	void depth(int i)			{ _depth = i;}
	void npalette(int i)				{_npalette  = i;}

	imageStatus status(imageStatus i)	{ return(_status = i);}
	int depth() 			{ return _depth;}
	long width() 			{ return _width; }
	long height() 			{ return _height ;}
	int npalette()			{ return _npalette;}
	imageStatus status()
		{ if(_status>=imageLastStatus) _status = imageLastStatus;return _status;}
	BYTE *image()			{ return _image;}
	char *binfo()			{ return (char *) _binfo;}
	int	calcpad()
			{ return pad = 3 - ((width()*depth()+7)/8+3)%4; }
	int ok()				{ return (_status == imageOK);}
	int	imageid()			{ return _imageid;}
	HWND	fatherhWnd() 	{ return _fatherhWnd;}
	HPALETTE hpal()			{ return hPal;}
	imageStatus error();

	void close();
	void title(char *t) {strcpy(_title,t);}
	static IxImage *_find(HWND hwnd);
	static IxImage * find(long id);
	static void closeall();
};


class BitmapImage : public IxImage {
	BITMAPINFOHEADER tmpHeader;
	long palettesize;
	long palettesizeOS2();
	long palettesizeWIN();

public:
	BitmapImage(HWND phwnd, HANDLE appInstance)
		: IxImage(phwnd,appInstance)
	{_type = imageBMP;}
	imageStatus load(char *file);
};
#endif
