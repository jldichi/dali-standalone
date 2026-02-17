/***********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: activ.h,v 1.2 1999/12/20 15:23:51 diegon Exp $
*
* DESCRIPTION
* Class definitions for generic software activation control
*********************************************************************/

#ifndef ACTIV_H
#define ACTIV_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound/itypes.h>
#include <stdio.h>

class gtype(PtrArray, PkgActivation);

class PkgActivation
	// Package activation class
{
	friend class Activation;

// private:

	Int pkgNo_;
	Date baseDate_;
	Int months_;
	Int key_;
	Int users_;

	void writeIt(FILE *f) const;
	bool readIt(FILE *f);

public:

	PkgActivation();
	PkgActivation(Int p, Int k, Date d, Int m, Int u);
	~PkgActivation() {}

	Int pkgNo() const			{ return pkgNo_; }
	Int months() const			{ return months_; }
	Int users() const			{ return users_; }
	Date baseDate() const		{ return baseDate_; }
	Int key() const				{ return key_; }
	static Int sK(Int s, Int p, Int m, Int u);
	Int calculatedK(Int s) const;
	void set(Int p, Int k, Date d, Int m, Int u);
	bool isActive(Int s) const;

#ifdef ISDEBUG
	void print() const;
#endif
};

class Activation
	// Activation class
{

// private:

	long int HIdInt_;
	long int HIdRes_;
	Int instNo_;
	Int rndNo_;
	Date activDate_;

	gtype(PtrArray, PkgActivation) &pkgs_;

	static char *file();
	static char *tempKey();
	void writeIt(FILE *f) const;
	bool readIt(FILE *f);
	PkgActivation *findPkg(Int n);
	bool load();
	Int fileId() const;
	Date activDate() const { return activDate_; }

public:

	Activation();
	~Activation();

	Int serialNo(Date dt) const;
	Int instNo() const						{ return instNo_; }
    long int hIdInt() const;
    long int hIdRes() const; 		 
    void clearActivations();
	void activate(Int instno, Int pkg, Int k, Int m = 0, Int u = 0);
	void reset(Int inst = ERR);
	void store() const;
	bool isActive(Int pkg, bool doAbort = true);
	Int users(Int pkg);
	Int months(Int pkg);

	// Used to iterate over the entries of the activation
	Int numEntries() const;
	Int entry(Int i) const;

	static Int sK(Int in, Int s, Int hii, Int hir, Int p, Int m, Int u);
	static bool validTempKey(const char *k);

#ifdef ISDEBUG
	void print() const;
#endif
};

#endif
