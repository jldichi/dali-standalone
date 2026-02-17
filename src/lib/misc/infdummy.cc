#include <ifound/libinfo.h>
extern "C" char * libInfo()
{
	static String l = LibraryInfo::libraries();
	return toCharPtr(l);
}
