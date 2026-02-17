#ifndef __URLENCODER_H_
#define __URLENCODER_H_

#include <ideafix.h>

class URLEncoder
{
private:
	static String csUnsafeString;
	String decToHex(char num, int radix);
	bool isUnsafe(char compareChar);
	String convert(char val);

public:
	URLEncoder() { };
	virtual ~URLEncoder() { };
	String URLEncode(String vData);
	String URLDecode(String vData);
};

#endif //__URLENCODER_H_
