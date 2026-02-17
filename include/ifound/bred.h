#ifndef __BRED_H
#define __BRED_H
#ifdef BREAD_IMPLEMENT
#	include <ifound/xstream.h>
long _expand(X_Stream &fpr,X_Stream &fpw, long bsize,long *v,long ratio);
long _reduce (X_Stream &fpr,X_Stream &fpw, long *buff, int buflen);

#define MAX_EXP_ERR	7
static inline void VS(long *v,long val,long n)
{
	register long nZ = 0;
	while(nZ < n)
		v[nZ] = val, nZ++;

}

static inline void VSset(long *v,long n)
{
	register long nZ = 0;
	while(nZ < n)
		v[nZ] = nZ, nZ++;
}

	// BREAD_IMPLEMENT
#endif
const char *expand_err(int );
long reduce(char * inbuf,long in_len, char **outbuf, long *outbuf_len, unsigned char complevel=0);
long expand(char * inbuf,long in_len, char **outbuf, long *outbuf_len, unsigned char complevel=0);
#ifndef UNIX
#define READ_MODE "rb"
#define WRITE_MODE "wb"
#else
#define READ_MODE "r"
#define WRITE_MODE "w"
#endif

#endif
