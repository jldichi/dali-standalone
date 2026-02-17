#ifndef DEBUGER_H
#define DEBUGER_H

#include <stdio.h>
#include <stdarg.h>

class Debuger
{
private:
	char* debugPath;
	FILE *fpdebug;

public:
	void postError(char* fmt, ...);
	void setDebugPath(char* path);
    Debuger() {
		debugPath = "/tmp/IxSca.log";
	};
};

#endif
