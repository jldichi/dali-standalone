/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: gifimage.h,v 1.1.1.1 1996/04/16 18:51:51 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

class GifImage : public IxImage {
	double normaspect;
	BYTE *RawGIF;			/* The heap array to hold it, raw */
	BYTE *Raster;			/* The raster data stream, unblocked */
	int Interlace, HasColormap;
	int _fsize;
	int BitOffset,		/* Bit Offset of next code */
		XC, YC,		/* Output X and Y coords of current pixel */
		Pass,			/* Used by output routine if interlaced _image */
		OutCount ,		/* Decompressor output 'stack count' */
		LeftOfs, TopOfs,		/* image offset */
		BytesPerScanline,		/* bytes per scanline in output raster */
		CodeSize,			/* Code size, read from GIF header */
		InitCodeSize,		/* Starting code size, used during Clear */
		Code,			/* Value returned by readCode */
		MaxCode,			/* limiting value for current code size */
		ClearCode,			/* GIF clear code */
		EOFCode,			/* GIF end-of-information code */
		CurCode, OldCode, InCode,	/* Decompressor variables */
		FirstFree,			/* First free code, generated per GIF spec */
		FreeCode,			/* Decompressor,next free slot in hash table */
		FinChar,			/* Decompressor variable */
		BitMask,			/* AND mask for data size */
		ReadMask,			/* Code AND mask for current code size */
		Misc;               /* miscellaneous bits (interlace, local cmap)*/

	imageStatus gifError(imageStatus);
	void doInterlace(BYTE Index);
	void doPut(BYTE Index);
	int readCode();


public:
	GifImage(HWND phwnd, HANDLE appInstance)
		: IxImage(phwnd,appInstance){
			_type = imageGIF;
			normaspect = 1;
			}


	imageStatus load(char *file);

};
