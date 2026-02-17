/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: rpgraph.h,v 1.1.1.1 1996/04/16 18:51:37 gustavof Exp $
*
* DESCRIPTION
* Graphics library interface header.
*********************************************************************/

#define MAX_SER		6
#define MAX_VAL		20

#define	G_ERR1		1	/* Barra muy angosta */ 
#define	G_ERR2		8	/* Ningun dato seteado */
#define	G_ERR3		3	/* Valor Negativo en Grafico de Pilas */
#define	G_ERR4		4	/* Valor Negativo en Grafico Pizza */
#define G_ERR5		5	/* El archivo no puede ser grabado	*/
#define G_ERR6		6	/* Rango X no seteado en grafico XY */
#define G_ERR7		2	/* Ningun Rango A-F seteado en grafico XY */
#define G_ERR8		7	/* ER_NO_MEM */

#define	BARRAS		1
#define	PILAS		5
#define	PIE			2
#define	XY			0

#define	GR_IZQ		0
#define	GR_CEN		1
#define	GR_DER		2

#define GR_AUTO		0x00
#define	GR_MANUAL	0xFF

#define GR_PANT		2
#define	GR_PRINT	1
#define	GR_NODEF	0


#define	GR_NOGRILLA		0
#define	GR_HORIZ		1
#define	GR_VERT			2
#define GR_AMBAS		3


#define GR_NO		0
#define GR_SI		1

extern int _GraphError;

#define GLK_V	C_VERT
#define GLK_H	C_HORIZ
#define GLK_VII	C_LL
#define GLK_UP	C_BOT
#define GLK_VID	C_LR
#define GLK_RG	C_RIGHT
#define GLK_CEN	C_CEN
#define GLK_LT	C_LEFT
#define GLK_VSI	C_UL
#define GLK_DW	C_TOP
#define GLK_VSD	C_UR


/* ++ Prototypes ++ */

#ifdef __cplusplus
extern "C" {
#endif

	extern int PrintScreen(FILE * PatnScr);
	extern void SetearTipo(int Tipo);
	extern void SetearDato(int Serie, int Indice, double Val);
	extern void SetearTitulo(const char *Tit, int Mar);
	extern void SetearSubtitulo(const char *Subtit, int Mar);
	extern void SetearLabelX(const char *Label);
	extern void SetearLabelY(const char *Label);
	extern void SetearLeyenda(int Indice, const char *Ley);
	extern void SetearRangoX(int Indice, const char *String);
	extern void SetearEjes(int XT, int YT, int CeroX, double YInf, double YSup);
	extern void SetearGrilla(int Tipo);
	extern int SetearPantalla(void);
	extern int SetearImpresora(int GX, int GY, int IX, int IY);
	extern void SetearTipoEscalaY(int Tipo);
	extern void SetearPattern(int indice, char car);
	extern void SetearValorEnBarras(int Tipo);
	extern int Graficar(void);
	extern void InitDatos(void);
	extern void InitScreen(int quien);
	extern void InitOpciones(void);
	extern void ResetSerie(int Indice);
	extern void ResetRangoX(void);
	extern void EndGrafics();

#ifdef __cplusplus
}
#endif


/* -- Prototypes -- */

