// Minimal test to diagnose getpid issue
// Compile with: cd build && cmake .. && make test_posix
// Or manually: g++ -fno-for-scope -fpermissive -w -D_GNU_SOURCE -Dlinux -DHAVE_BOOL=1 -I../include -I../include/winapp -I../include/local -I../include/ifound -E ../test_posix.cc 2>&1 | grep -c "getpid"

#include <ideafix.h>
#include <cfix.h>
#include <unistd.h>

// Test 1: Is getpid a macro?
#ifdef getpid
#error getpid is defined as a macro - THIS IS THE PROBLEM
#endif

// Test 2: Is __THROW empty or missing?
#ifndef __THROW
#error __THROW is not defined at all
#endif

// Test 3: Can we call getpid?
int main() {
    int pid = getpid();
    int ret = chdir("/tmp");
    unsigned int s = sleep(0);
    return pid + ret + s;
}
