/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: wminit.cc,v 1.14 2010/09/28 19:56:54 hardaiz Exp $
*
* DESCRIPTION
* Initialization routines of WM.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*_w_interrupt	The only catched signal is SIGINT (others ignored).
*w_init			Start operations with WM.
*w_interrupts   Enable or disable external interrupts (SIGINT).
*w_end			End operations with WM.
*initialize     Read the .map file.
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>

#include <ifound/str.h>

#include <ideafix/priv/gndefs.h>
#include <ideafix/priv/wmcom.h>

#include <wm.h>

#include <signal.h>
#include <stdlib.h>

/* ++ Prototypes ++ */

static void parse_tfile(void);
static TRAD_ELEM *ifind(UnChar **ps, TRAD_ELEM *t);
static KEY_TAB *find_kptr(UnChar code);
static void install_key(int kcode, UnChar *sec, UnChar *desc, UnChar *name);
static void tf_err(const char *msg, UnChar *s1);
static attr_type findatrname(UnChar *name);

/* -- Prototypes -- */

/* Public variables defined in this module							*/
char *TermName;
UShort wm_mode = MODE_WM;
int Serv = -1;
bool curr_int = TRUE;    	/* Handle or reject interrupts			*/
bool retry;

/* Variables taken from other modules                               */
extern bool suspend;
extern bool hab_susp;
extern bool hab_sig;
extern bool hab_abort;
extern char * dem_file;
char *wm_mod_version;
extern int tmout;
extern int wmstop;

int wmtmout = 0;

/* Private variables												*/
static IFPICCPCCP prev_msg;	/* Function to output Error msgs. 	*/
static IFPICPVL prev_arimsg;	/* Function to output ARI Error msgs. */
static char tfile[64];		/* Nombre del archivo de descr. terminal*/
static int lineno;			/* Linea actual de parsing				*/
static bool errors;		/* Flag de si hubo errores				*/
static int chkperm;		/* permiso para utilizar el wm			*/


int w_chkperm(void)
{
	return chkperm;
}

static int ARIDialog(int inflag, char *fmt, va_list ap)
{
	static bool aborting = FALSE;
	wdflag flags = 0;
	wdflag ret;

	if (aborting)
		return ERR;
		
	if (inflag & MSG_ERROR)	flags |= WD_ERROR;
	if (inflag & MSG_RELEASE) flags |= WD_RELEASE;
	assert(inflag & MSG_ERROR);
	ret = WiVDialog((inflag & MSG_WAIT) ? flags|WD_YES|WD_NO|WD_CANCEL :
			flags, WD_YES, _WmErrmsg("RETRY"), fmt, ap);

	if (ret == WD_CANCEL) {
		aborting = TRUE;
		Stop(1);
	}
	return ret == WD_YES;
}


class libwm {
public:
	static void foo();	
};

static void foolink()
{
	libwm::foo();
}
/*
 * w_init: Inicia el trabajo con el WM.
 * This function does not receive any argument
 * as in older versions.
 */

void w_init(void)
{
#ifdef HAVE_SECURITY
	chkperm = CheckPerm(-PKG_RUNTIME-1);
	if (CheckPerm(-PKG_SERVICES-1) == ERR) Serv = 0;	/* NO! */
											 else Serv = -1; /* Yes */
#else
	Serv = -1;
#endif
	foolink();
	
	/* Inicializar Tablas de traduccion */
	initialize();

	/* Inicializar manejo de pantalla virtual */
	_vs_start();

	vs_cls();

	/* inicializo rutinas para pgm */ 
	WmSetStdAlone();

	/* crear la window SCREEN */
	WiCreate(SCREEN, 0, 0, VS_LINES, VS_COLS, NO_BORDER, "", A_NORMAL);

	/* Configurar interrupciones */
	(void)SetSigInt((FP)SIG_IGN);
	OnStop(w_end);
	prev_msg = SetMessager(WiHandler);

#if (!defined(__NT__) && !defined(hppa) && !defined(hpux) && !defined(Linux7) && !defined(LinuxGcc3) && !defined(LinuxGcc4) && !defined(AIX5) && !defined(SunSolx86) && !defined(SunSol))  
	prev_arimsg = SetAriMessager((IFPICCPVL) ARIDialog);
#else
	prev_arimsg = 0; // to do.
#endif

#ifndef NO_SERVICES
	/* seteo los servicios */
	SetMenuBuiltIn("_print_scr", _print_scr);
	SetMenuBuiltIn("_print_ichset", _print_ichset);
	SetMenuBuiltIn("servmov", servmov);
	SetMenuBuiltIn("calculador", calculador);
	SetMenuBuiltIn("GoToShell", GoToShell);
#ifdef HAVE_FREEMEM
	SetMenuBuiltIn("FreeMem", FreeMemWi);
#endif
#endif

}

/*
 * Fin de operaciones con el WM
 * vs_refresh redundantes agregados porque en algunas terminales
 * se acumulan caracteres y no resetea bien.
 */
void w_end(void)
{
	_vs_end();
	SetMessager(prev_msg);

#if (!defined(__NT__) && !defined(hppa) && !defined(hpux) && !defined(Linux7) && !defined(LinuxGcc3) && !defined(LinuxGcc4) && !defined(AIX5) && !defined(SunSolx86) && !defined(SunSol))
	SetAriMessager((IFPICCPVL)prev_arimsg);
#else
	SetAriMessager((IFPICCPCCP)prev_arimsg);
#endif
}

/*
 * initialize:
 *				Inicializar tabla de caracteres.
 *				Leer definicion de teclado
 */

void initialize(void)
{
	register int i, k;

#ifdef HAVE_TTY
	attr_type prev_UL;	/* To discover if it has line chars		*/
#endif

	/* Inicializar vector de traduccion input y output */
	for(i = 0; i < NCHSET; i++){
		out_tab[i]   =  chset[i].value = (UnChar)i;
		chset[i].tr  = NULL;
	}

	/* Inicializar valores default definidos en 'wmconf.c' */
	(void) strcpy(tfile, "wmconf.c");
	for(i=0, k=0; key_tab[i].code; i++) {
		lineno = i;
		if (key_tab[i].code == 1) {
			char *msg = _WmErrmsg(String("KEYTAB") + toString(k));
			strncpy(key_tab[i].descr, msg, KEY_HELP_DSC);
			key_tab[i].descr[KEY_HELP_DSC] = '\0';
        	k++;
		} else {
			if (key_tab[i].def_key) {
				install_key(
					key_tab[i].code,
					(UnChar *) key_tab[i].def_key,
					(UnChar *) NULL,
					(UnChar *) _CharName(key_tab[i].code));
			}
			out_tab[key_tab[i].code] = key_tab[i].def_output;
		}
	}
#ifndef __NT__
	for(i=0, k=0; attr_tab[i].std_name ; i++, k++) {
		char *msg = _WmErrmsg(String("ATTRTAB") + toString(k));
		if (msg == NULL)
			msg = "";
		
		//fprintf(stderr, "strlen: %d [%d]- msg: '%s'.\n", strlen(msg), MAX_LANG_NAME, msg);
		assert(strlen(msg) < MAX_LANG_NAME);
		strcpy(attr_tab[i].lang_name, msg);
	}
#endif	
#ifdef HAVE_TTY
	/* Remember the UL character							*/
	prev_UL = out_tab[C_UL];
#endif

	/* Now, read the terminal map file						*/
	parse_tfile();

	/* Discover if this terminal has line ...               */
#ifdef HAVE_TTY
	has_line_drawing_chars = (bool) (out_tab[C_UL] != prev_UL);
#else
	has_line_drawing_chars = TRUE;
#endif
	
	/* Flags errors found in map file						*/
	if (errors) {
		(void) fprintf(stderr, (char*)_GnErrmsg(HITANYKEY));
		if (fgetc(stdin) != '\n')
			_global_exit(1);
	}
}

/*
 * parse_tfile:
 *				Lee un archivo de definicion de terminal
 * Formato:
 *              Nombre    Secuencia  Texto descriptivo.
 *
 *              # Comentarios
 */

			/* read modes */
#define INPUT	0 		/* mode input  */
#define OUTPUT	1		/* mode output */
#define COLOR	2		/* mode color  */

static void parse_tfile(void)
{
	FILE *fp;
	char buf[256], *BLANK = " \t\n";
	UnChar *name, *sec, *desc = NULL;
	int code, i, len;
	char *p;
	int read_mode = INPUT;
	attr_type sub, subbg = 0, outcode, atr;

	/* Abrir archivo terminal. Si no abre, retorna o aborta */
	(void) strcpy(tfile, "map/");

#ifdef HAVE_TTY
	TermName = ReadEnv("TERM");
#else
	{
		FPCPCPI save = SetReadEnvHandler(NULLFP);
		TermName = ReadEnv("TERM");
		SetReadEnvHandler(save);
		if (TermName == NULL) {
			TermName = "PC-Standard";
			return;
		}
	}
#endif
	(void) strcat(tfile, TermName);
	if ( (fp = FopenPath(tfile, ".map", "r")) == NULL ) {
		(void) fprintf(stderr, (char*)_WmErrmsg("MAP_UNDEF"), tfile+4);
		(void) fprintf(stderr, (char*)_GnErrmsg(HITANYKEY));
		if (fgetc(stdin) != '\n') _global_exit(1);
		else return;
	}

	/* Leer lineas, e interpretar nombres */
	lineno = 0;
	while( fgets(buf,sizeof(buf),fp) != NULL ) {
		++lineno;

		/* Tomar el primer token */
		name = (UnChar *) strtok(buf, BLANK);

		/* Si es linea en blanco, o comienza con #, continuar */
		if( name == NULL || name[0] == '#' )
			continue;
		
		/* Verifico si es un token de cambio de contexto */
		if (strncmp((char *) name, "INPUT", 5) == 0
		 || strncmp((char *) name, _WmErrmsg("ST_INPUT"), 
		 						   strlen(_WmErrmsg("ST_INPUT"))) == 0) {
			read_mode = INPUT;
			continue;
		}
		if (strncmp((char *) name, "OUTPUT", 6) == 0
		 || strncmp((char *) name, _WmErrmsg("ST_OUTPUT"), 
		 							strlen(_WmErrmsg("ST_OUTPUT"))) == 0) {
			read_mode = OUTPUT;
			continue;
		}
		if (strncmp((char *) name, "COLORS", 6) == 0
		 || strncmp((char *) name, _WmErrmsg("ST_COLOR"), 
		 							strlen(_WmErrmsg("ST_COLOR"))) == 0) {
			read_mode = COLOR;
			continue;
		}

		switch (read_mode) {
		case INPUT:
			if (strncmp((char *)name, "CODETAB", 7) == 0) {
				static UnChar ii[2]="";

				for (i=128; i <= 255; i++) {
					ii[0] = toascii(i);
					if (ii[0] == ' ' || ii[0] < 128) continue;
					ii[1] = '\0';
					install_key(i, ii,
						(UnChar *)_CharName(i), (UnChar *)_CharName(i));
				}
				continue;
			}
			/* Tenemos un token que es nombre de una tecla */
			if((code=_FindCharCode((char*)name))==ERR && name[1]!=0 &&
										 name[0]!='\\'){
				tf_err("BAD_NAME", name);
				continue;
			}

			/* Leemos la secuencia que  la tecla */
			if( (sec = (UnChar *) strtok((char*)NULL, BLANK)) == NULL){
				tf_err("NO_SECUENCE", name);
				continue;
			}

			if (code != ERR) {
				/* Leemos el texto de ayuda (hasta fin linea) */
				if( (desc = (UnChar *) strtok((char*)NULL,"\n"))==NULL){
					tf_err("NO_DESCRIP", name);
					continue;
				}
			}

			/* Instalar tecla en tabla de traduccion */
			install_key(code, sec, desc, name);
			break;
		case OUTPUT:
			if (strncmp((char *)name,"RAW",3)==0){
				for (i = 0; i < 256; i++) out_tab[i] = i;
				continue;
			}
			if (strncmp((char *)name, "CODETAB", 7) == 0) {
				for (i = 0; i <= 255; i++)
					out_tab[i] = (i<128) ? i : toascii(i);
				continue;
			}

			/* Tenemos un token que es nombre de una tecla */
			if((code=_FindCharCode((char*)name))==ERR && name[1]!=0 &&
										name[0]!='\\'){
				tf_err("BAD_NAME", name);
			continue;
			}

			outcode = A_NORMAL;
			while ( (sec = (UnChar *)strtok((char *)NULL,BLANK))!=NULL) {
				if ((atr = findatrname(sec)) != INT_MAX) outcode |= atr;
				else {
					StrDecode(sec, sec);
					outcode |= sec[0];
				}
			}

			/* Instalo codigo en tabla de output */
			if (name[0] == '\\') /* redef. mediante codigo numerico */
				StrDecode(name, name);
			out_tab[code==ERR ? (int)name[0] : code] = outcode;
			break;
		case COLOR:

			// El responsable de esto es Dino Tonon
			if ((atr = findatrname(name)) == INT_MAX) {
				tf_err("NO_SECUENCE", name);
				continue;
			}
			sub = VS_COLOR(atr);
			subbg = VS_BG_COLOR(atr);

			if((sec = (UnChar *) strtok((char*)NULL, BLANK)) == NULL){
				tf_err("NO_SECUENCE", name);
				continue;
			}

			StrDecode(sec, sec);
			len  = strlen((char*)sec);
			p = (char *) Alloc(len+1);
			strcpy(p, (char *) sec);

			char *uscore = strrchr((char*)name, '_');
			if (uscore != NULL && strcmp(uscore, "_BG") == 0)
				col_bg[subbg] = p;
			else
				col_fg[sub]   = p;
			/*
			if (sub) col_fg[sub]   = p;
			else 	 col_bg[subbg] = p;
			*/
			has_color = TRUE;
		}
	}
	(void) fclose(fp);
}

/*
 * Return a pointer to key_tab given a code.
 */
static KEY_TAB *find_kptr(UnChar code)
{
	KEY_TAB *ptr = key_tab;

	while(ptr->code && ptr->code != code) ptr++;

	return ptr;
}

/*
 * ifind:
 *			Encuentra el elemento a partir del cual se
 *			debe insertar una secuencia.
 *          NULL si no es posible agregarla.
 *          Actualiza el string con la secuencia, de modo
 *          de dejarlo apuntando al primer caracter nuevo.
 */
static TRAD_ELEM *ifind(UnChar **ps, TRAD_ELEM *t)
{
	while (**ps) {
		if ((UnChar) t->c == **ps) {
			if (t->action != READ_NEXT) return NULL;
			t = t->match;
			(*ps)++;		/* Pasar al siguiente caracter */
		}
		else if ( t->nomatch == NULL )
			return t;			/* Insertar aca' */
		else
			 t = t->nomatch;
	}
	return NULL;				/* La sec. es prefijo de una exist.  */
}


/*
 * install_key:
 *				Redefine la tecla con codigo 'kcode', con la 
 *              secuencia provista en 'sec'.
 *              Si la secuencia es vacia, no hace nada.
 *              Si la descripcion es NULL, no la instala.
 * 
 */

static void install_key(int kcode, UnChar *sec, UnChar *desc,
		UnChar *name)
{
	register int code;
	TRAD_ELEM *_new, *t; 
	UnChar buffdesc[80];
	UnChar bname[80];
	UnChar bsec[80];
	UnChar *psec;
	UnChar *bdesc;

	if (!sec || *sec == 0) return;

	if (kcode == ERR) {
		StrDecode(bname, name);
		code = bname[0];	/* Caracter ASCII */
	}
	else {
		KEY_TAB *kw = find_kptr((UnChar)kcode);

		code = kcode; /* Caracter ASCII extendido */
		/* 
		 * Cargar texto de descripcion 
		 * truncando a KEY_HELP_DSC caracteres
		 */
		if (desc) {
			int len;

			StrDecode(buffdesc, desc);
			bdesc = (UnChar*)StrTrim((char *) buffdesc);
			len = strlen((char *) bdesc);
			if (len > KEY_HELP_DSC) 
				bdesc[len = KEY_HELP_DSC] = '\0';
			strcpy(kw->descr, (char *) bdesc);
		}
	}

	/* 
	 * Instalar secuencia en el arbol de traduccion 
	 * Procesamiento especial para tecla de interrupcion.
	 */
	/* Decodificar string de secuencia */
#ifdef __NT__
	bsec[0] = '\0';
#endif
	StrDecode(bsec, sec);
	psec = bsec;

#ifdef HAVE_FULL_SIGNALS
	if (code == K_INTERRUPT) {
		wm_intr_char = psec[0];
		if (psec[1])
			tf_err("BAD_INTRCHAR", (UnChar *) "");
		return;
	}
#endif

	/* Probar si es secuencia de un solo caracter */
	if (psec[1] == 0) {
		/*
		if (chset[(UnChar) psec[0]].tr != NULL)
			tf_err("SEC_IS_PREFIX", bname);
		else
		*/
			chset[(UnChar) psec[0]].value = (UnChar) code;
		return;
	}

	/* Secuencias de mas de un caracter */
	t = chset[(UnChar) *psec++].tr;
	if (t == NULL) {
		_new = (TRAD_ELEM*) Alloc(sizeof(TRAD_ELEM));
		chset[(UnChar) psec[-1]].tr = _new;
	}
	else {
		if ((_new = ifind((UnChar **) &psec, t)) == NULL) {
			tf_err("INVALID_SEC", bname);
			return;
		}
		_new->nomatch = (TRAD_ELEM *) Alloc(sizeof(TRAD_ELEM));
		_new          = _new->nomatch;
	}

	_new->c = *psec;
	while (*++psec) {
		_new->match  = (TRAD_ELEM *) Alloc(sizeof(TRAD_ELEM));
		_new->action = READ_NEXT;
		_new         = _new->match;
		_new->c      = *psec;
	};
	_new->action = (UnChar) code;
}


/*
 *	tf_err:
 *		Imprime un mensaje de Error en interpretacion de
 *		archivo de def. terminal.
 */
static void tf_err(const char *msg, UnChar *s1)
{
	(void) fprintf(stderr, (char*)_WmErrmsg(msg), tfile, lineno, s1);
	(void) fprintf(stderr, "\n");
	errors = TRUE;
}

/*
 * findatrname : dado un atributo en nombre externo devuelve su 
 *				 valor interno.
 */
static attr_type findatrname(UnChar *name)
{
	register ATTR_TAB *p;
	for (p = attr_tab; p->std_name; ++p) {
		if (str_eq(p->lang_name, name) || str_eq(p->std_name, name))
			return p->value;
	}
	return INT_MAX;
}


void _WiInterrupts(void)
{
	if ((curr_int = wm_par[0].i))
		tmout = wmtmout;
	else
		tmout = 0;
	wmstop = FALSE;
}


/* ==========================================================
				   INTERRUPT PROCESSING    
		
		The only catched signal is SIGINT (others ignored).
		The SIGINT handler is _w_interrupt.

		This handler looks at the 'interrupts' flag. If it's
		OFF, the user is informed that no interrupts are
		posible.

==========================================================*/
void _w_interrupt(void)
{
	int sig = 0;
	bool habaux = hab_susp;
	bool habaux1 = hab_sig;
	char * sv_file = dem_file;

	/* si no estoy habilitado */
	if (!hab_sig) {
#if !defined(HAVE_RESTARTABLE_SYSCALLS) || !defined(HAVE_SIGVEC) 
		retry = TRUE;
#endif                
		(void) SetSigInt(_w_interrupt);
		return;
	}

	hab_sig = hab_susp = retry = suspend = FALSE;

	(void)SetSigInt((FP)SIG_IGN);

	/* deshabilito la demo */
	dem_file = NULL;

	static String intLabel,	intArg, intArg1, intArg2, 
				  intArg3, intArg4;

	if (intLabel == NULL_STRING) {
		intLabel	= appMsg("INTLABEL"),
		intArg		= appMsg("INTARG"),
		intArg1		= appMsg("INTARG1"),
		intArg2		= appMsg("INTARG2"),
		intArg3		= appMsg("INTARG3"),
		intArg4		= appMsg("INTARG4");
	}
		
#ifdef HAVE_PROC
	if (sv_file)			/* si es demo */
		sig = PopUpLMenu(7, 25, (char*)toCharPtr(intLabel), 
								(char*)toCharPtr(intArg),
								(char*)NULL);
	else {
		if (!curr_int) {
			sig = PopUpLMenu(7, 25, (char*)toCharPtr(intLabel), 
									(char*)toCharPtr(intArg),
							 (habaux) ? (char*)toCharPtr(intArg1) 
							 		  : (char*)NULL, (char *)NULL);
			if (sig==1) suspend = TRUE;
			else {
#if !defined(HAVE_RESTARTABLE_SYSCALLS) || !defined(HAVE_SIGVEC)
			retry = TRUE;
#endif
			}                 
			(void)SetSigInt(_w_interrupt);
			hab_susp = habaux;
			hab_sig = habaux1;
			return;
		}
		if (tty_set) {
			if (habaux)
				sig = PopUpLMenu(7, 25, (char*)toCharPtr(intLabel), 
										(char*)toCharPtr(intArg),
										(char*)toCharPtr(intArg1), 
										(char*)toCharPtr(intArg2), 
							hab_abort ?	(char*)toCharPtr(intArg3) : (char*)NULL,
							hab_abort ?	(char*)toCharPtr(intArg4) : (char*)NULL,
										(char*)NULL);
			else
				sig = PopUpLMenu(7, 25, (char*)toCharPtr(intLabel), 
										(char*)toCharPtr(intArg), 
										(char*)toCharPtr(intArg2),
							hab_abort ?	(char*)toCharPtr(intArg3) : (char*)NULL,
							hab_abort ?	(char*)toCharPtr(intArg4) : (char*)NULL,
										(char*)NULL);
		}	
	}
#else
	sig = PopUpLMenu(7, 25, (char*)toCharPtr(intLabel), 
							(char*)toCharPtr(intArg),
						  curr_int ? (char*)toCharPtr(intArg2) : (char*)NULL, 
						  (char*)NULL);
#endif

	switch (sig) {
	case ERR: dem_file = sv_file;
	case 0 : 
#if !defined(HAVE_RESTARTABLE_SYSCALLS) || !defined(HAVE_SIGVEC)
		retry = TRUE; 
#endif             
		break;
#ifndef HAVE_FULL_SIGNALS
	case 1 : _send_signal(SIGTERM); 
			 break;
#else
	case 1 : if (habaux) 
				suspend=TRUE; 
			 else
				_send_signal(SIGTERM); 
			 break;
	case 2 :
#if HAVE_SIGEMT
			_send_signal(habaux ? SIGTERM : SIGEMT);
#else
			_send_signal(SIGTERM);
#endif
			break;

#ifdef HAVE_SIGEMT			
	case 3 : if (habaux) {
				_send_signal(SIGEMT);
				break;
			 }
#endif
	case 4 :{
			 extern int rpid;
			 vs_setbackgr(A_NORMAL);
			 vs_setattr(A_NORMAL);
			 vs_cursor(1);
			 vs_moveto(VS_LINES - 1,0);
			 vs_refresh();
			 vs_cls();
			 vs_refresh();
			 WiResettty();
			 if (rpid) kill(rpid, SIGTERM);
			 Stop(0);
			 break;
			}
#endif
	}

	(void)SetSigInt(_w_interrupt);
	hab_susp = habaux;
	hab_sig  = habaux1;
}
