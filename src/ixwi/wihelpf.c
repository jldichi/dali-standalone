/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: wihelpf.c,v 1.3 1999/08/02 17:02:56 eduardoc Exp $
*
*
* DESCRIPTION:
*      Window function Library. Routines to display a text
*      contained in a file.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*WiDispFile     | Display a file
*WiHelpFile		| Display help text contained in a file.
*
*********************************************************************/

#include <ideafix.h>
#include <ideafix/priv/gndefs.h>

/* +++ CONFIG +++ */
#define BSIZE 512				/* Line buffer size					*/
#define MAXLINES 1024			/* Max. number of lines in file		*/
/* --- CONFIG --- */

/* ++ Prototypes ++ */

static char *gt_fil_txt(UShort *offset, int line);
static void set_ofst_tab(UShort *offset, int *fil, int *col);

/* -- Prototypes -- */

/* nro_lines have option_base = 1 */
static UShort nro_lines = 0;

/* Buffer de linea para gt_fil_txt */
static char Buff[BSIZE] = { '\0' };

/* Global static variables                                         */
static FILE *fp = NULL;
static bool hlp_active = FALSE;


/*
 * WiDispFile: Mostrar un archivo en pantalla con posibilidad
 *             de paginar.
 *
 * COMANDOS:
 *            PAGE_UP  PAGE_DOWN  CURS_UP  CURS_DOWN  ENTER END
 *
 * filename: Nombre del archivo a buscar.
 * label   : Etiqueta para la window.
 */
void WiDispFile(char *filename, int nfils, int ncols, char *label)
{
	UShort offset[MAXLINES * sizeof(UShort)];
	int aux1, aux2;

	if( hlp_active) return;

	hlp_active = TRUE;
	if ((fp = fopen(filename,
#ifdef HAVE_TEXT_FILES
			"rb"
#else
			"r"
#endif
			)) == NULL) {
		(void) WiMsg((char*)_WiErrmsg(E_NO_OPEN), filename);
	}
	else {
		set_ofst_tab(offset, &aux1, &aux2);	/* set the offset table */
		WiHelp(nfils, ncols, label,  (CPFPCPI) gt_fil_txt, 
			   (char*)offset);
		(void) fclose(fp);
	}
	hlp_active = FALSE;
}

/*
 * w_helpfile
 * ----------
 *
 * Rutina para desplegar textos de ayuda contenido en un file
 */

void WiHelpFile(char *filename, char *label)
{
	char *p;
	UShort offset[MAXLINES * sizeof(short)];
	int width = 40, 
		height = 10;
	
	if( hlp_active) return;

	(void) sprintf(Buff, "hlp/%s", filename);	

	/* Strip extension if any	*/
	if ((p=strrchr(Buff, '.')))
		*p = '\0';

	hlp_active = TRUE;
	if ((fp = FopenPath(Buff, ".hlp",
#ifdef HAVE_TEXT_FILES
			"rb"
#else
			"r"
#endif
			)) == NULL) {
		(void) WiMsg((char*)_WiErrmsg(NO_HLP_FILE), Buff+4);
	}
	else {
		set_ofst_tab(offset, &height, &width);	/* set the offset tab*/
		WiHelp(height, width, label, (CPFPCPI) gt_fil_txt, 
			   (char*)offset);
		(void) fclose(fp);
	}
	hlp_active = FALSE;
}


static char *gt_fil_txt(UShort *offset, int line)
{
	int c, length;
	register char *pnt;

	if (line<1 || line>nro_lines)
		return NULL;

	(void) fseek(fp, (long) offset[line-1], 0);

	pnt    = Buff;
	length = 0;
	while( (length < BSIZE - 1) && 
			(c=getc(fp))!=EOF && c!='\n' && c!='\r') {
		*(pnt++) = (char)c;
		length++;
	}

	if (length == 0 && c != EOF)
		*(pnt++) = ' ';

	*pnt='\0';
	return Buff;
}

static void set_ofst_tab(UShort *offset, int *fil, int *col)
{
	int max_fil = (WiHeight(SCREEN) / 4) * 3;
	nro_lines=1;
	
	offset[0] = 0;
	*col = 4;
	while (fgets(Buff,BSIZE,fp) && nro_lines < MAXLINES) {
		int len = StrDspLen(Buff);

		if (len > *col) *col = len;
		offset[nro_lines++] = (UShort) ftell(fp);
	}
	nro_lines--;
	*fil = ((int)nro_lines < max_fil) ? (int)nro_lines : max_fil;
}	

