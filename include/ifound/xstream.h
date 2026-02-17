#ifndef _X_STREAM_H
#define _X_STREAM_H	1

#include <stdio.h>
#include <stdlib.h>
#ifdef getc
#	undef getc
#endif

#ifdef putc
#	undef putc
#endif
const int X_CHUNK = 512;

class X_Stream
{
	int pos;
	int len;
	int datalen;
	unsigned char * buf;
	signed char canunget;
	signed char clearbuf;
public:
	X_Stream(int l=-1,  char * data=(char *)-1, int dl=0);
	~X_Stream();
	int fwrite(char *,int,int);
	int fread(char *,int,int);
	inline void rewind() {pos = 0;}
	int ftell() {return pos;}
	int putc(int);
	int getc();
	int ungetc(int);
	int length() { return datalen; }
	void * getbuf() { return buf;}
};
#endif

