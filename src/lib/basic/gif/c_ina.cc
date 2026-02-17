
#include <stdlib.h>

#include <ideafix.h>
#include <ifound.h>
#include "c_ixscaina.h"

static IxscaIna ixina;

char *InaUserLogin(int uid)
{
	static char name[20] = {0};
	memset(name, 0, 20);

	strncpy(name, toCharPtr(ixina.getUserName(uid)), 19);

	return name;
}
