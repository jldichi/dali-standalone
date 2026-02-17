/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: crypt.cc,v 1.2 2004/09/29 15:06:53 damianm Exp $
*
* DESCRIPTION
* Encryption routine.
*
* ROUTINE         |  PURPOSE
*-----------------+---------------------------------------------------
* initCrypt(pw,s) | Init the encryption process
* cryptChar(c)	  | Encrypt a character
*********************************************************************/
#include <string.h>

#include <ifound.h>
#include <ifound/str.h>
#include <ifound/stdc.h>
#include <ifound/priv/mangling.h>

/* ++ Prototypes ++ */

static void shuffle(void);

/* -- Prototypes -- */

#define ROTORSZ 256
#define MASK 0377

static char	t1[ROTORSZ] = { '\0' };
static char	t2[ROTORSZ] = { '\0' };
static char	t3[ROTORSZ] = { '\0' };
static char	deck[ROTORSZ] = { '\0' };
static char	buf[13] = { '\0' };
static int n1 = 0;
static int n2 = 0;
static int nr2 = 0;
static int secureflg = 0;

void initCrypt(char *pw, int sflag)
{
	int ic, i, k, temp;
	unsigned random;
	int seed;        //Se pasa de Int a int porque en 64 bits trae problemas para desencriptar

	strncpy(buf, pw, 8);
	while (*pw)
		*pw++ = '\0';

	buf[8] = buf[0];
	buf[9] = buf[1];

	seed = 123;

	for (i = 0; i < 13; i++)
		seed = seed * buf[i] + i;

	for (i = 0; i < ROTORSZ; i++) {
		t1[i] = char(i);
		deck[i] = char(i);
		t2[i] = t3[i] = 0;
	}

	for (i = 0; i < ROTORSZ; i++) {
		seed = 5 * seed + buf[i % 13];
		random = (unsigned int) (seed % 65521);
		k = ROTORSZ - 1 - i;
		ic = (random & MASK) % (k + 1);
		random >>= 8;
		temp = t1[k];
		t1[k] = t1[ic];
		t1[ic] = char(temp);
		if (t3[k] != 0)
			continue;
		ic = (random & MASK) % k;
		while (t3[ic] != 0)
			ic = (ic + 1) % k;
		t3[k] = char(ic);
		t3[ic] = char(k);
	}

	for (i = 0; i < ROTORSZ; i++)
		t2[t1[i] & MASK] = char(i);

	n1 = 0;
	n2 = 0;
	nr2 = 0;
	secureflg = sflag;
}

static void shuffle(void)
{
	int i, ic, k, temp;
	unsigned random;
	static Int seed = 123;

	for (i = 0; i < ROTORSZ; i++) {
		seed = 5 * seed + buf[i % 13];
		random = (unsigned int) (seed % 65521);
		k = ROTORSZ - 1 - i;
		ic = (random & MASK) % (k + 1);
		temp = deck[k];
		deck[k] = deck[ic];
		deck[ic] = char(temp);
	}
}

int cryptChar(int i)
{
	int nr1, c;

	if (secureflg) {
		nr1 = deck[n1] & MASK;
		nr2 = deck[nr1] & MASK;
	} else {
		nr1 = n1;
	}
	c = t3[(t1[(i + nr1) & MASK] + nr2) & MASK] - nr2;
	i = t2[c & MASK] - nr1;
	n1++;
	if (n1 == ROTORSZ) {
		n1 = 0;
		n2++;
		if (n2 == ROTORSZ)
			n2 = 0;
		if (secureflg) {
			shuffle();
		} else {
			nr2 = n2;
		}
	}
	return i;
}

void cryptString(char *theStr)
{
	if (theStr == NULL)
		return;

	char *p = theStr;
	while (*p != '\0') {
		*p = (char) cryptChar((int) *p);
		++p;
	}
}

#define MAX_TXT_LEN	64
String encryption(const String &txt, const String &key)
{
	if (txt == HIGH_VALUE) return txt;

	char buff[MAX_TXT_LEN];
	assert(txt.length() < MAX_TXT_LEN);
	initCrypt((char *) toCharPtr(key), true);
	char *aux = toCharPtr(txt);
	strcpy(buff, aux ? aux : "");
	cryptString(buff);

	return buff;
}
