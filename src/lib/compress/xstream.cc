#include <ifound/xstream.h>
//#ifdef IDEAFIX
#if 0
#include <ifound.h>
#include <ifound/stdc.h>
#else
#include <string.h>
#undef assert
#define assert(x)
#endif

X_Stream::X_Stream(int l,char *data, int dl)
: len(l),clearbuf(1),canunget(0),datalen(dl),pos(0),buf((unsigned char*)data)
{
	if(len==-1)
		len = X_CHUNK;
	if(buf==(unsigned char *)-1)
		buf = new unsigned char[len];
	else
		clearbuf = 0;
	assert(buf);
}

X_Stream::~X_Stream()
{
	if(buf && clearbuf) {
		delete [] buf;
		buf = NULL;
	}
}


int X_Stream::putc(int c)
{
	canunget = 0;
	if(pos >= len){
		len+=X_CHUNK;
		buf = (unsigned char *)realloc(buf,len);
	}
	assert(buf);
	buf[pos] = (unsigned char) c;
	if(pos == datalen)
		datalen++;
	pos++;
	return c;

}

int X_Stream::fwrite(char *data,int l,int n)
{
	long s=l*n;
	canunget = 0;
	register int i;
	for(i=0;i<s;i++){
		putc(data[i]);
	}
	return s;
}

int X_Stream::fread(char *data,int l,int n)
{
	long s=l*n;
	canunget = 0;
	if(pos >= datalen)
		return EOF;
	canunget = 1;
	if(pos+s > datalen)
		s = datalen-pos;
	assert(buf);
	if(s>0) {
		memcpy(data,buf+pos,s);
		pos += s;
	}
	return s;
}


int X_Stream::getc()
{
	canunget = 0;
	if(pos >= datalen)
		return EOF;
	canunget = 1;
	return buf[pos++];
}

int X_Stream::ungetc(int c)
{
	if(!canunget)
		return EOF;
	canunget = 0;
	pos--;
	if(buf[pos] == c)
		return c;
	pos++;
	return EOF;
}
