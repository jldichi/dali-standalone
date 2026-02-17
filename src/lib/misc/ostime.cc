/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: ostime.cc,v 1.22 2004/09/29 15:37:18 damianm Exp $
*
* DESCRIPTION
* Get time and date from the operating system
* and...
* Class definitions for generic software activation control
*********************************************************************/
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ifound.h>
#include <ifound/stdc.h>
#include <ifound/priv/mangling.h>
#include <ifound/priv/timedefs.h>

#ifdef HAVE_REGISTRY
#	include <windows.h>
#endif

#if HAVE_TIME_H

#include <time.h>


#if HAVE_LOCALTIME

#if HAVE_TIMEZONE
extern long timezone;
#endif

#if HAVE_ALTZONE
extern long altzone;
#endif

long _osTimeZoneAdj(long t)
{
	static long gmtoff = ERR;

	if (gmtoff == ERR) {
#if HAVE_TIMEZONE
#if HAVE_ALTZONE
		gmtoff = -(localtime(&t)->tm_isdst ? altzone : timezone);
#else
		gmtoff = -(localtime((TIMEPARAM1TYPE)&t)->tm_isdst ? timezone-3600 : timezone);
#endif
#else
		gmtoff = localtime(&t)->tm_gmtoff;
#endif
	}
	return t+gmtoff;
}

#else /* HAVE_LOCALTIME */

long _osTimeZoneAdj(long t)
{
#if HAVE_TIMEOFFSET
	return t-TIMEOFFSET%86400;
#else
	return t;
#endif
}

#endif
#ifdef TIME_FROM68
#define DATE_OFFSET		5844L
#else
#define DATE_OFFSET		5113L
#endif

long _osTime()
{
#ifdef HAVE_GETLOCALTIME
	// returns seconds since 01/01/1968 00hs - just like Symantec's C time()
	SYSTEMTIME t;
	Int res1,res2;
	GetLocalTime(&t); // Should I Use systemtime ????
	// really it should be day 0 , but it doesn't exist , so
	// I calculate with day 1 and then rest 1 day .....
	return _dhms_to_time(t.wDay,t.wHour,t.wMinute,t.wSecond) +
		( _dmy_to_date(1,t.wMonth,t.wYear) + DATE_OFFSET - 1) * 86400;
#else
	return _osTimeZoneAdj(_global_time(NULL));
#endif
}

#else /* HAVE_TIME_H */

#if HAVE_SYS_TIMEB_H

#include <sys/types.h>
#include <sys/timeb.h>

long _osTimeZoneAdj(struct timeb *tp)
{
	return (DATE) ((tp->time - tp->timezone*60L) /86400 - 5113);
}

long _osTime()
{
	struct timeb tp;
	ftime(&tp);
	return (DATE) _osTimeZoneAdj(&tp);
}

#endif /* HAVE_SYS_TIMEB_H */
#endif /* HAVE_TIME_H */

//
// now activation procedures
// 

#ifdef __GNUC__
#pragma implementation "activ.h"
#endif

#ifdef HAVE_REGISTRY
#       include <windows.h>
#endif

#include <ifound.h>
#include <ifound/stdc.h>
#include <ifound/priv/mangling.h>
#include <ifound/priv/timedefs.h>

// now activation routines

#define MAX_STR	256
#define F(a)	(a^0x87)

static UnChar termType_[] = {
	F('2'), F('#'), F('$'), F('A'), F('8'), F('f'), F('2'), F('5'),
	F('\0')
};

static UnChar *fix(const UnChar *p)
{
	static UnChar str[MAX_STR] = { '\0' };
	register UnChar *q = str;

	while ((*q++ = F(*p))) {
		// Just in case macro F expands '*p' more than once!
		p++;
		assert(q - str < MAX_STR);
	}
	return str;
}

char *ourPassword()
{
	return (char *) fix(termType_);
}

#if HAVE_SECURITY

// C interface to activations
extern "C" Int ActivationSerialNo();
extern "C" Int ActivationInstNo();
extern "C" long int ActivationHIdInt();
extern "C" long int ActivationHIdRes();
extern "C" void ResetActivation(Int instNo);
extern "C" Int NumActivationEntries();
extern "C" Int ActivationEntry(Int e);
extern "C" Int ActivationPackageUsers(Int pkg);
extern "C" Int ActivationPackageMonths(Int pkey);
extern "C" char *ActivationPackageName(Int pkg);
extern "C" bool ActivePackage(Int pkg, bool doAbort);
extern "C" void ActivatePackage(Int instno, Int pkg, Int key, Int months, Int users);
extern "C" Int ActivationKey(Int instno, Int serial, long int hidint, long int hidres, Int pkg, Int months, Int users);
extern "C" bool ValidTempKey(char *);

#include <ifound/parray.h>
#include <sys/types.h>

#if HAVE_UTIME_H
#	include <utime.h>
#else

	/*
		This is for Interactive since I don't know why the header
		file <utime.h> does not exist!
	*/
	struct utimbuf {
		time_t	actime;		/* set the access time */
		time_t	modtime;	/* set the modification time */
	};

	extern "C" int utime(char *name, struct utimbuf *p);

#endif

#include <sys/stat.h>
#include <local/activ.h>
//#define NROPKGS 55
#define NROPKGS 56

struct pkname {
	int pkg;
	char *name;	      
};

static  pkname pkgNames[NROPKGS] = {
	{0,"IdeaFix Runtime"},
	{1,"IdeaFix Development"},
	{2,"IdeaFix IQL"},
	{3,"IdeaFix Services"},
	{4,"Essentia"},
	{5,"Cracker"},
	{6,"Bpm"},
	{7,"Sql Framework"}, 
	{8,"Isys"},
	{9,"Ideafix with PostgreSQL"},   
	{100,"Aurus (Básico)"}, 

	{101," Aurus: CashFlow"}, 
	{102," Aurus: Presentaciones Impositivas"}, 
	{103," Aurus: Conciliaciones Bancarias"}, 
	{104," Aurus: Retenciones de Ingresos Brutos"}, 
	{105," Aurus: Pagos (Tesorería I)"}, 
	{106," Aurus: Cobranzas (Tesoría II)"}, 
	{107," Aurus: Informes Multimonetarios"}, 
	{108," Aurus: Control Presupuestario"}, 
	{109," Aurus: Autoretención de Ganancias"},
	{110," Aurus: Cuentas a Cobrar"},   

	{200,"Denarius (Básico)"}, 
	{201," Denarius: Embargos"}, 
	{202," Denarius: Interfaz Contable"}, 
	{203," Denarius: Novedades"}, 
	{204," Denarius: Auditoria"}, 
	{205," Denarius: Retroactivos"}, 
	{206," Denarius: Plan Vacacional"}, 
	{207," Denarius: Horas Extras"}, 
	{208," Denarius: Asistencias y Licencias"}, 
	{209," Denarius: Dotaciones"},

	{210," Denarius: Turnos y Fichadas"}, 
	{211," Denarius: Permisos"}, 
	{212," Denarius: Interfaz Denarius-PeopleSoft"}, 
	{213," Denarius: Gestión del Conocimientos"}, 
	{214," Denarius: Empleos"}, 
	{215," Denarius: Evaluación de Desempeño"}, 
	{216," Denarius: Medicina Laboral"}, 
	{217," Denarius: Plan de Carrera"}, 
	{218," Denarius: Remuneraciones"}, 
	{219," Denarius: Sanciones"}, 

	{220," Denarius: Liquidación"}, 
	{221," Denarius: Adelanto de Haberes"}, 
	{222," Denarius: Legales Argentina"}, 
	{223," Denarius: Legales Uruguay"}, 
	{224," Denarius: Legales Paraguay"}, 
	{225," Denarius: OLAP"}, 
	{300,"Ianus (Básico)"}, 
	{301," Ianus: Suministros"}, 
	{302," Ianus: Compras"}, 
	{303," Ianus: Interfaz Ianus-Aurus"}, 

	{304," Ianus: Control Presupuestario"}, 
	{400,"CBA - Concil. Banc. Avanzada (Básico)"}, 
	{401," CBA: Interfaz CBA-Aurus"}, 
	{900,"Bienes de Uso"}, 
	{901,"Presupuesto del Estado"}, 
};

declare(PtrArray, PkgActivation);
implement(PtrArray, PkgActivation);
declare(PtrArrayCursor, PkgActivation);
implement(PtrArrayCursor, PkgActivation);

Activation::Activation()
:	pkgs_(*tr_new PtrArray(PkgActivation)())
{
	instNo_ = ERR;
	rndNo_ = ERR;  
	HIdInt_ = ERR;
	HIdRes_ = ERR;
	// Build file (if it didn't exist)
	if (!load())
		store();
}

Activation::~Activation()
{
	for (PtrArrayCursor(PkgActivation) c(pkgs_); c; ++c)
		tr_delete(*c);

	PtrArray(PkgActivation) *p = &pkgs_;
	tr_delete(p);
}

void Activation::reset(Int inst)
{                 
#ifdef HAVE_REGISTRY
 	unsigned long sn, maxlen,fsflags;
#endif
 	LDIVRETTYPE result;         
 	srand48(_global_time(NULL));
 	rndNo_ = lrand48();
 	instNo_ = inst;
#ifdef HAVE_GETHOSTID
 	if ((abs(gethostid())-instNo_) > instNo_)
    	result = ldiv(abs(gethostid()),instNo_);
 	else 
    	result = ldiv(instNo_,abs(gethostid()));
#elif HAVE_REGISTRY
 	GetVolumeInformation("C:\\",NULL,0,&sn,&maxlen,&fsflags,NULL,0);
 	if ((sn-instNo_) > instNo_)
    	result = ldiv(sn,instNo_);
 	else 
    	result = ldiv(instNo_,sn);
#else                      
    struct stat st;
	FILE *f = fopen("/", "r");
	if (f == NULL)
		error("Terminfo Error #8");
	if (fstat(fileno(f), &st) == ERR)
		error("Terminfo Error #9");
	fclose(f);
    if ((st.st_ctime - instNo_) > instNo_)
    	result = ldiv(st.st_ctime,instNo_);
 	else 
    	result = ldiv(instNo_,st.st_ctime);
#endif

 	HIdInt_ = result.quot;
 	HIdRes_ = result.rem;
}

Int Activation::fileId() const
{
#ifdef HAVE_REGISTRY
	unsigned long sn, maxlen,fsflags;
	GetVolumeInformation("C:\\",NULL,0,&sn,
		&maxlen,&fsflags,NULL,0);
	return (Int) sn;
#else                  
	struct stat st;
	FILE *f = fopen(file(), "r");
	if (f == NULL)
		error("Terminfo Error #8");
	if (fstat(fileno(f), &st) == ERR)
		error("Terminfo Error #9");
	fclose(f);
	return (Int) st.st_ino;
#endif
}

PkgActivation *Activation::findPkg(Int n)
{
	for (PtrArrayCursor(PkgActivation) c(pkgs_); c; ++c) {
		assert(*c != NULL);
		if (c->pkgNo() == n)
			return *c;
	}
	return NULL;
}

void Activation::activate(Int instno, Int pkg, Int k, Int m, Int u)
{
	assert(pkg >= 0);
#ifdef HAVE_REGISTRY
 	unsigned long sn, maxlen,fsflags;
#endif
 	LDIVRETTYPE result;         
    
	PkgActivation *pa = findPkg(pkg);
	if (pa == NULL)
		pkgs_.add(pa = tr_new PkgActivation);

	// Try up until one week ago
	Date today("NOW");
	Int i,ki;                   

#ifdef HAVE_GETHOSTID
 	if ((abs(gethostid())-instNo_) > instNo_)
    	result = ldiv(abs(gethostid()),instNo_);
 	else 
    	result = ldiv(instNo_,abs(gethostid()));
#elif HAVE_REGISTRY
 	GetVolumeInformation("C:\\",NULL,0,&sn,&maxlen,&fsflags,NULL,0);
 	if ((sn-instNo_) > instNo_)
    	result = ldiv(sn,instNo_);
 	else 
    	result = ldiv(instNo_,sn);
#else                      
    struct stat st;
	FILE *f = fopen("/", "r");
	if (f == NULL)
		error("Terminfo Error #8");
	if (fstat(fileno(f), &st) == ERR)
		error("Terminfo Error #9");
	fclose(f);
    if ((st.st_ctime - instNo_) > instNo_)
    	result = ldiv(st.st_ctime,instNo_);
 	else 
    	result = ldiv(instNo_,st.st_ctime);
#endif
	
	if ( (instno == instNo_) && (result.quot == HIdInt_) && (result.rem == HIdInt_))
	for (i = 0; i < 7; i++) {                        
		if (PkgActivation::sK(serialNo(today - i), pkg, m, u) == k) {
			pa->set(pkg, k, today - i, m, u);
		 return;
		}
	}  
	
	// Wrong key!   
	pa->set(pkg, k, today, m, u);
}

#ifdef HAVE_SHARE_LIB_TERMINFO_DIR
static UnChar file_[] = {
	F('/'), F('u'), F('s'), F('r'), F('/'), F('s'), F('h'), F('a'),
	F('r'), F('e'), F('/'), F('l'), F('i'), F('b'), F('/'), F('t'),
	F('e'), F('r'), F('m'), F('i'), F('n'), F('f'), F('o'), F('/'),
	F('x'), F('/'), F('x'), F('t'), F('e'), F('r'), F('m'), F('-'), 
	F('x'), F('s'), F('\0')
};
#elif defined (HAVE_SHARE_TERMINFO_DIR)
static UnChar file_[] = {
	F('/'), F('u'), F('s'), F('r'), F('/'), F('s'), F('h'), F('a'),
	F('r'), F('e'), F('/'), F('t'), F('e'), F('r'), F('m'), F('i'), 
	F('n'), F('f'), F('o'), F('/'), F('x'), F('/'), F('x'), F('t'), 
	F('e'), F('r'), F('m'), F('-'), F('x'), F('s'), F('\0')
};
#else
static UnChar file_[] = {
	F('/'), F('u'), F('s'), F('r'), F('/'), F('l'), F('i'), F('b'),
	F('/'), F('t'), F('e'), F('r'), F('m'), F('i'), F('n'), F('f'),
	F('o'), F('/'), F('x'), F('/'), F('x'), F('t'), F('e'), F('r'), 
	F('m'), F('-'),	F('x'), F('s'), F('\0')
};
#endif

#ifdef HAVE_SHARE_LIB_TERMINFO_DIR
static UnChar attribCopyFile_[] = {
	F('/'), F('u'), F('s'), F('r'), F('/'), F('s'), F('h'), F('a'),
	F('r'), F('e'), F('/'), F('l'), F('i'), F('b'), F('/'), F('t'), 
	F('e'), F('r'), F('m'), F('i'), F('n'), F('f'), F('o'), F('/'), 
	F('x'), F('/'), F('x'), F('t'), F('e'), F('r'), F('m'), F('\0')
};
#elif defined (HAVE_SHARE_TERMINFO_DIR)
static UnChar attribCopyFile_[] = {
	F('/'), F('u'), F('s'), F('r'), F('/'), F('s'), F('h'), F('a'),
	F('r'), F('e'), F('/'), F('t'), F('e'), F('r'), F('m'), F('i'), 
	F('n'), F('f'), F('o'), F('/'), F('x'), F('/'), F('x'), F('t'), 
	F('e'), F('r'), F('m'), F('\0')
};
#else
static UnChar attribCopyFile_[] = {
	F('/'), F('u'), F('s'), F('r'), F('/'), F('l'), F('i'), F('b'),
	F('/'), F('t'), F('e'), F('r'), F('m'), F('i'), F('n'), F('f'),
	F('o'), F('/'), F('x'), F('/'), F('x'), F('t'), F('e'), F('r'),
	F('m'), F('\0')
};
#endif

static UnChar tempKey_[] = {
	F('!'), F('R'), F('3'), F('l'), F('l'), F('1'), F('4'), F('n'), F('7'), F('.'), F('\0')
};

char *Activation::file()
{
	return (char *) fix(file_);
}

char *Activation::tempKey()
{
	return (char *) fix(tempKey_);
}

struct ActivationDict {
	Int pkgno;
	Int npkgs;
	Int HIdInt;
	Int HIdRes;
	Int instNo;
	Int rndNo;
};

struct PkgActivationDict {
	Int pkgNo;
	Date baseDate;
	Int months;
	Int key;
	Int users;
};

#ifndef HAVE_REGISTRY
static void setOwner(char *file, UidT u, GidT g)
{
	if (chown(file, u, g) == ERR) {
		error("Terminfo Error #4");
	}
}

static void setTimes(char *file, time_t atm, time_t mtm)
{
	struct utimbuf ut;
	ut.actime = atm;
	ut.modtime = mtm;
	if (utime(file, &ut) == ERR) {
		error("Terminfo Error #5");
	}
}
#endif

static Int shuffle(Int a, Int b)
{
	Int i, res = 0;
	for (i = 0; i < 4; ++i) {
		srand48((a & 0xFF) ^ b);
		a = ~(a >> 8);
		res = ~(res << 8) ^ lrand48();
	}

	// Make sure number returned is non-negative
	return res & 0x7FFFFFFF;
}

Int Activation::serialNo(Date dt) const
{
	Int fid = fileId();
	Int d = toInt(dt);      
	return shuffle(shuffle(fid, d), rndNo_);
}
 
long int Activation::hIdInt() const
{
 return HIdInt_; 
}                            

long int Activation::hIdRes() const 
{
 return HIdRes_; 
}
 
const char subkey[] = "Software\\InterSoft";
char valuename[]="ixset";
void Activation::store() const
{
#ifdef HAVE_REGISTRY
	HKEY h;
	DWORD res;
	int len;
	len = sizeof(ActivationDict) + sizeof(PkgActivationDict) * pkgs_.dim();
	BYTE * const data = new BYTE[len];
	BYTE * pd = (BYTE *)data;
	assert(pd != NULL);
	if(RegCreateKeyEx(HKEY_LOCAL_MACHINE,subkey,0,
		valuename,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL, &h,&res)
		!= ERROR_SUCCESS )
		error("Terminfo Error #1");

	writeIt((FILE *)pd);
	pd += sizeof(ActivationDict);
	for (PtrArrayCursor(PkgActivation) c(pkgs_); c; ++c) {
		assert(*c != NULL);
		c->writeIt((FILE *)pd);
		pd += sizeof(PkgActivationDict);
	}
	if (pd != data+len)
		error("Terminfo Error #5");
	if(RegSetValueEx(h,valuename,0,REG_BINARY,data,len) != ERROR_SUCCESS)
		error("Terminfo Error #2");
	if(RegCloseKey(h) != ERROR_SUCCESS)
		error("Terminfo Error #3");
	delete[] data;
#else
	if (getuid() != 0) {
		error("Terminfo Error #3");
	}

	// make sure file is created readable by everyone
	umask(0);

	FILE *f = fopen(file(), "w");
	if (f == NULL) {
		error("Terminfo Error #1");
	}
	writeIt(f);
	for (PtrArrayCursor(PkgActivation) c(pkgs_); c; ++c) {
		assert(*c != NULL);
		c->writeIt(f);
	}
	fclose(f);

	// Set file owner and times so they don't look suspicious...
	struct stat st;
	f = fopen((char *) fix(attribCopyFile_), "r");
	if (f == NULL || fstat(fileno(f), &st) == ERR) {
		st.st_atime = st.st_mtime = 1409;
		st.st_uid = 2;
		st.st_gid = 2;
	}
	if (f) fclose(f);

	setTimes(file(), st.st_atime, st.st_mtime);
	setOwner(file(), st.st_uid, st.st_gid);
#endif
}

void Activation::writeIt(FILE *f) const
{
#ifdef HAVE_REGISTRY
	assert(f != NULL);
	ActivationDict *ph = (ActivationDict *)f;
	ph->npkgs = pkgs_.dim();
	ph->instNo = instNo_;
	ph->rndNo = rndNo_;  
	ph->HIdInt = HIdInt_;
	ph->HIdRes = HIdRes_;
#else
	ActivationDict h;
	h.npkgs = pkgs_.dim();
	h.instNo = instNo_;
	h.rndNo = rndNo_; 
	h.HIdInt = HIdInt_;
	h.HIdRes = HIdRes_;
	if (fwrite(&h, sizeof(h), 1, f) != 1)
		error("Terminfo Error #2");
#endif
}
                                         
bool Activation::readIt(FILE *f)
{
#ifdef HAVE_REGISTRY
	assert(f != NULL);
	ActivationDict *ph = (ActivationDict *)f;
	pkgs_.reAlloc(ph->npkgs);
	instNo_ = ph->instNo;
	rndNo_ = ph->rndNo;  
	HIdInt_ = ph->HIdInt;
	HIdRes_ =  ph->HIdRes;
#else
	ActivationDict h;
	if (fread(&h, sizeof(h), 1, f) != 1)
		return false;
	pkgs_.reAlloc(h.npkgs);
	instNo_ = h.instNo;
	rndNo_ = h.rndNo; 
	HIdInt_ = h.HIdInt;
	HIdRes_ =  h.HIdRes;
#endif
	return true;
}

void Activation::clearActivations()
{
	for (PtrArrayCursor(PkgActivation) c(pkgs_); c; ++c) {
		assert(*c != NULL);
		tr_delete(*c);
	}
	pkgs_.reAlloc(0);
}

bool Activation::load()
{
#ifdef HAVE_REGISTRY
	const Int maxlen = 2048;
	BYTE data[maxlen+2];
	char name[maxlen+2];
	BYTE *pd = data;
	HKEY h;
	DWORD res,rc,nlen,len,i,type;
	
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,subkey,0,KEY_READ,&h)
		!= ERROR_SUCCESS )
		return false;
		
	for(i = 0;nlen=maxlen,len=maxlen,
			(rc = RegEnumValue(h,i,name,&nlen,NULL,&type,data,
			&len)) == ERROR_SUCCESS ; i ++){
			if((type == REG_BINARY) && !strcmpi(name,valuename))
				break;
	}
	if(rc != ERROR_SUCCESS)
		return false;
	clearActivations();
	readIt((FILE *)data);
	pd += sizeof(ActivationDict);

	for (PtrArrayCursor(PkgActivation) c = pkgs_; c; ++c) {
		assert(*c == NULL);
		pkgs_.add(tr_new PkgActivation);
		assert(*c != NULL);
		c->readIt((FILE *)pd);
		pd += sizeof(PkgActivationDict);
	}
	if(pd != data+len)
		error("Terminfo Error #5");

	if(RegCloseKey(h) != ERROR_SUCCESS)
		error("Terminfo Error #3");
#else
	FILE *f = fopen(file(), "r");
	if (f == NULL) {
		return false;
	}
	clearActivations();

	if (!readIt(f))
		return false;

	for (PtrArrayCursor(PkgActivation) c(pkgs_); c; ++c) {
		assert(*c == NULL);
		pkgs_.add(tr_new PkgActivation);
		assert(*c != NULL);
		if (!c->readIt(f))
			return false;
	}
	fclose(f);
#endif
	return true;
}

bool Activation::isActive(Int pkg, bool doAbort)
{
	PkgActivation *pa = findPkg(pkg);
	if (pa != NULL) {
		if (pa->isActive(serialNo(pa->baseDate())))
			return true;
	}
	if (doAbort) {
		String tmp("Package ");
		tmp += toString(pkg);
		tmp += " - ";
		tmp += ActivationPackageName(pkg);
		tmp += " is not active.";
		error(toCharPtr(tmp));
	}
	return false;
}

Int Activation::users(Int pkg)
{
	PkgActivation *pa = findPkg(pkg);
	assert(pa != NULL);
	return pa->users();
}

Int Activation::months(Int pkg)
{
	PkgActivation *pa = findPkg(pkg);
	assert(pa != NULL);
	return pa->months();
}

Int Activation::numEntries() const
{
	return pkgs_.dim();
}

Int Activation::entry(Int i) const
{
	PkgActivation *pa = pkgs_.elem(i);
	assert(pa != NULL);
	return pa->pkgNo();
}

Int Activation::sK(Int instno, Int serial, long int hidint, long int hidres, Int pkg, Int mnths, Int usrs)
{                 
	return shuffle(shuffle(shuffle(shuffle(serial, pkg), shuffle(hidres,mnths)), shuffle(hidint,usrs)),instno);
} 

Int PkgActivation::sK(Int serial, Int pkg, Int mnths, Int usrs)
{                 
	return shuffle(shuffle(shuffle(shuffle(serial, pkg), shuffle(ActivationHIdRes(),mnths)), shuffle(ActivationHIdInt(),usrs)),Activation().instNo());
}
 
bool Activation::validTempKey(const char *k)
{
	return strcmp(k, tempKey()) == 0;
}


PkgActivation::PkgActivation()
{
	pkgNo_ = ERR;
	baseDate_ = LOW_VALUE;
	months_ = ERR;
	key_ = ERR;
	users_ = ERR;
}

PkgActivation::PkgActivation(Int pkg, Int k, Date d, Int m, Int u)
{
	set(pkg, k, d, m, u);
}

void PkgActivation::set(Int pkg, Int k, Date d, Int m, Int u)
{
	pkgNo_ = pkg;
	baseDate_ = d;
	months_ = m;
	key_ = k;
	users_ = u;
}

bool PkgActivation::readIt(FILE *f)
{
#ifdef HAVE_REGISTRY
	assert(f != NULL);
	PkgActivationDict *pd = (PkgActivationDict *)f;
	pkgNo_ = pd->pkgNo;
	baseDate_ = pd->baseDate;
	months_ = pd->months;
	key_ = pd->key;
	users_ = pd->users;
#else
	PkgActivationDict d;
	if (fread(&d, sizeof(d), 1, f) != 1)
		return false;
	pkgNo_ = d.pkgNo;
	baseDate_ = d.baseDate;
	months_ = d.months;
	key_ = d.key;
	users_ = d.users;
#endif
	return true;
}

void PkgActivation::writeIt(FILE *f) const
{
#ifdef HAVE_REGISTRY
	assert(f != NULL);
	PkgActivationDict *pd = (PkgActivationDict *)f;
	pd->pkgNo = pkgNo_;
	pd->baseDate = baseDate_;
	pd->months = months_;
	pd->key = key_;
	pd->users = users_;
#else
	PkgActivationDict d;
	d.pkgNo = pkgNo_;
	d.baseDate = baseDate_;
	d.months = months_;
	d.key = key_;
	d.users = users_;
	if (fwrite(&d, sizeof(d), 1, f) != 1)
		error("Terminfo Error #6");
#endif
}

Int PkgActivation::calculatedK(Int serial) const
{
	return PkgActivation::sK(serial ,pkgNo_, months_, users_);
}

bool PkgActivation::isActive(Int serial) const
{
	// Temporary activation?
	if (months_ > 0) {
		Date today("NOW");
		Date last = baseDate_ + (31 * months_);
		if (today < baseDate_ || today > last)
			return false;
	}                     
	return key_ == calculatedK(serial);
}

#if DEBUG

#include <local/strfile.h>

void PkgActivation::print() const
{
	char usersStr[32];
	if (users_ == 0)
		strcpy(usersStr, "unlimited");
	else
		sprintf(usersStr, "%d", users_);

	printf("Pkg: %02d %-25s (%s user%s)\n", pkgNo_, ActivationPackageName(pkgNo_),
			usersStr, (users_ == 0 || users_ > 1) ? "s" : "");
}

void Activation::print() const
{
	printf("Active packages for installation %ld:\n", instNo_);

	for (PtrArrayCursor(PkgActivation) c(pkgs_); c; ++c) {
		assert(*c != NULL);
		if (c->isActive(serialNo(c->baseDate())))
			c->print();
	}
}
#endif /* DEBUG */

extern "C" Int ActivationInstNo()
{
	return Activation().instNo();
}

extern "C" Int ActivationSerialNo()
{
	return Activation().serialNo(Date("NOW"));
}                      

extern "C" long int ActivationHIdInt()
{
 return  Activation().hIdInt(); 
}

extern "C" long int ActivationHIdRes()
{
 return  Activation().hIdRes(); 
}

extern "C" void ResetActivation(Int inst)
{
	Activation act;
	act.reset(inst);
	act.store();
}

extern "C" void ActivatePackage(Int instno, Int pkg, Int ky, Int mnths, Int usrs)
{
	Activation act;     
	act.activate(instno, pkg, ky, mnths, usrs);
	act.store();
}

extern "C" Int NumActivationEntries()
{
	return Activation().numEntries();
}

extern "C" Int ActivationEntry(Int n)
{
	return Activation().entry(n);
}

extern "C" Int ActivationPackageUsers(Int pkg)
{
	return Activation().users(pkg);
}

extern "C" Int ActivationPackageMonths(Int pkg)
{
	return Activation().months(pkg);
}

extern "C" char *ActivationPackageName(Int pkg)
{
 for (int i=0 ; i < NROPKGS ; i++)
    if (pkgNames[i].pkg==pkg) return pkgNames[i].name;
 return "Reserved Empty Slot";
}

extern "C" Int ActivationKey(Int instno, Int serial, long hidint, long hidres, Int pkg, Int mnths, Int usrs)
{
	return Activation::sK(instno ,serial,  hidint, hidres, pkg, mnths, usrs);
}

extern "C" bool ValidTempKey(char *k)
{
	return Activation::validTempKey(k);
}

extern "C" bool ActivePackage(Int pkg, bool doAbort)
{
#if defined(ACADEMIC_VERSION)
	return true;
#endif
#if HAVE_SECURITY
	Activation act;
	return act.isActive(pkg, doAbort);
#else
	return true;
#endif
}

#endif /* HAVE_SECURITY */

// end of activation routines
// end of ostime

