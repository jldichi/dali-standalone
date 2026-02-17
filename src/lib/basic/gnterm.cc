/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: gnterm.cc,v 1.2 2004/09/29 16:46:56 damianm Exp $
*
* DESCRIPTION:
* Activation checking routines
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*********************************************************************/

#include <unistd.h>

#include <ideafix.h>
#include <ifound/priv/macros.h>

/* ++ Prototypes ++ */

extern "C" bool ActivePackage(Int pkg, bool doAbort);
static void CartelNoClave(int level);

/* -- Prototypes -- */

#ifdef HAVE_SECURITY

/*
	Verifica que este habilitado el paquete Nro "level".
	Si level > 0 y no esta habilitado, aborta el programa
	con un mensaje.

	Si level < 0, no aborta el programa.
	OJO: Para chequear sin abortar el paquete "level", 
		 hay que pasar (-level-1) por que si no el 0 no se puede
		 distinguir.

	Retorna ERR si el paquete no esta habilitado.
*/
int CheckPerm(int level)
{
	bool fatal = TRUE;
	char *p;

	if (level < 0) { level = -level - 1; fatal = FALSE; }

	p = ReadEnv("OPENLIC");

	if ((p && *p) || ActivePackage(level, FALSE))
		return level;

	if (fatal) { 
		CartelNoClave(level);
		sleep(20);
		stop(1 | ABNORMAL_STOP); 
	}
	return ERR;
}

static void CartelNoClave(int level)
{
fprintf(stderr,"                                                 \n\r");
fprintf(stderr,"*************************************************\n\r");
if (level < NUM_PCKGS)
fprintf(stderr,"ERROR: Clave de activacion de SOFTWARE DE BASE   \n\r");
else
fprintf(stderr,"ERROR: Clave de activacion de SOFTWARE APLICATIVO\n\r");
fprintf(stderr,"*************************************************\n\r");
fprintf(stderr," ATENCION: Su clave de activacion para el modulo\n\r");
fprintf(stderr,"           Nro: %d no es valida o bien     \n\r",
								level);
fprintf(stderr,"           ha expirado. Comuniquese con InterSoft\n\r");
fprintf(stderr,"           o su representante para obtener las   \n\r");
fprintf(stderr,"           claves de activacion.                 \n\r");
fprintf(stderr,"*************************************************\n\r");
fprintf(stderr,"                                                 \n\r");
}
#else /* HAVE_SECURITY */

extern "C" int CheckPerm(int level)
{
	return level;
}

#endif	/* HAVE_SECURITY */
