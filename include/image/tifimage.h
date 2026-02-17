/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: tifimage.h,v 1.1.1.1 1996/04/16 18:51:52 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#include <image/tiffio.h>


class TiffImage : public IxImage {
   	TIFF *tif;
	short bps;			// Bit per sample;
	short photo;			// Photometric interpretation;
	short spp;			// Samples per pixel;
	short planarconfig;

	int scanlinesize;


public:
	TiffImage(HWND phwnd, HANDLE appInstance)
		: IxImage(phwnd,appInstance) {
			_type = imageTIFF;tif = NULL;
			}

	imageStatus load(char *file);
	imageStatus readContig();
	imageStatus readSeparate();

};
