// Diagnostic test 2: check __THROW value and unistd.h processing
// Run: g++ -fno-for-scope -fpermissive -w -D_GNU_SOURCE -Dlinux -DHAVE_BOOL=1 -I./include -I./include/winapp -I./include/local -I./include/ifound -E test_posix2.cc 2>&1 | grep "DIAG_THROW_VALUE"

// Step 1: Include ideafix first (like all .cc files do)
#include <ideafix.h>
#include <cfix.h>

// Step 2: Check __THROW value AFTER ideafix headers
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#ifdef __THROW
#pragma message "DIAG_THROW_VALUE=[" TOSTRING(__THROW) "]"
#else
#pragma message "DIAG_THROW_VALUE=[UNDEFINED]"
#endif

// Step 3: Now include unistd AFTER ideafix (like wmalone.cc does)
#include <unistd.h>

// Step 4: Check if getpid got declared
int main() {
    int pid = getpid();
    return pid;
}
