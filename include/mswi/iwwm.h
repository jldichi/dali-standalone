#ifndef _INC_IWWM
#define _INC_IWWM

/* -------------- defs from wm.h, wmdefs.h --------- */
#define     MAX_WINDOWS         128

	/* Valores para los flags de una ventana */

#define		WRAP_FLAG		2       /* Wrap ON/OFF (borde derecho) */
#define		LINEOVERFLOW	4       /* Marca, linea sobrepasa borde*/
#define		CURSOR			8	    /* Cursor visible or invisible */
#define		MODIF			16		/* La window a sido modificado */

/* longitud maxima para el nombre de un proceso */
#define LONG_NAME  			48
#define LONG_MODULE		 	16
#define LONG_VERSION		32


#define READ_NEXT   0

/* Errores del window manager */
#if 0 // Ideawin uses own messages !!!
// #define  BAD_NAME		1
// #define  NO_SECUENCE 	2
// #define  NO_DESCRIP  	3
// #define  SEC_IS_PREFIX 	4
// #define  INVALID_SEC	5
// #define  INVALID_TERM	6
// #define  TCAP_TOO_LONG	7
// #define  TCAP_INCOMPLETE	8
// #define  MAP_UNDEF		9
// #define  BAD_INTRCHAR   10
// #define  USER_IN_STAND  11
// #define  NO_FILE_PRTSCR 12
// #define  KEY_NOT_ACT	13
// #define  CR_TASK		14
// #define  NO_INTERR		15
// #define  ST_INPUT		16
// #define  ST_OUTPUT		17
// #define  ST_COLOR		18
// #define  UNDEF_MSG		19
// #define  INTLABEL		20
// #define  INTARG			21	/* Hasta el 25 (incl.)	*/
// #define  ACTIVE_TSK		26
// #define  KEYTAB			27  /* Hasta el 36 (incl.)	*/
// #define  ATTRTAB		37  /* Hasta el 83 (incl.)	*/
// #define  HELP_TITLE		84
// #define  SERV_LABEL		85
// #define  POP_TITLE		86
// #define  NO_HELP		87
// #define  CALC_HLP		88  /* Hasta el 111 (incl.) */
// #define  CHSET			112
// #define  WM_MAIL		113
// #define  CANT_EXEC		114
// #define  RT_SHELL		115
// #define  SELBASE		116
// #define  DECBASE		117
// #define  HEXBASE		118
// #define  OCTBASE		119
// #define  ALR_RUN		120
// #define  CANT_COMM		121
// #define  CANT_INST		122
// #define  STILL_RUN		123
// #define	 PRN_NOT_FOUND	124
// #define	 INSTALL_ERR	125
// #define	 IPC_NOTINST	126
#endif
typedef struct trad {
	unsigned  value;
	unsigned short keyin;
	struct trad *next;
} TRAD_ELEM;

/*
 * Tabla con las asignaciones de default a las teclas de
 * funcion y caracteres especiales. Para cada codigo por
 * arriba de 128 se le da:
 */
typedef struct	{
	unsigned    code;		/* Codigo asociado			*/
	attr_type def_output; /* Output default */
	unsigned short   def_key;	/* Secuencia tecla default	*/
	char 	*descr;		/* String con nombre tecla  */
} KEY_TAB;

typedef struct	{
	char 	*lang_name;	/* Nombre en leng. configurado */
	char 	*std_name;	/* Nombre de la funcion std.   */
	attr_type	value;		/* Valor asociado			*/
} ATTR_TAB;

typedef struct {
	unsigned value;
	TRAD_ELEM *tr;
} CH_SET;

/* Prototypes */
/* ------------ wm.h -------------------- */
#ifdef __cplusplus
extern "C" {
#endif

extern bool hab_susp;
extern short TAB;
extern char TermName[12];

extern void w_init(void);
extern void    WmSetStdAlone(void);
extern void GoToShell(void);
void 	_w_service(void);
char 	*hlptxt(char *t, int n);
unsigned char   AltKey(unsigned char c);
#ifdef __cplusplus
}     /* extern "C" { */

char *_WmErrmsg(const String &msg);
#endif


#endif  /* _INC_IWWM */


