// Test 3: Bisect exactly where getpid disappears
// Run: g++ -fno-for-scope -fpermissive -w -D_GNU_SOURCE -Dlinux -DHAVE_BOOL=1 -I./include -I./include/winapp -I./include/local -I./include/ifound test_posix3.cc -o /dev/null 2>&1

// Test A: Does getpid work with just unistd.h?  (should work)
#include <unistd.h>
void testA() { int x = getpid(); (void)x; }

// Test B: Does getpid work AFTER ideafix.h?
#include <ideafix.h>
void testB() { int x = getpid(); (void)x; }

// Test C: Does getpid work after cfix.h?
#include <cfix.h>
void testC() { int x = getpid(); (void)x; }

int main() { testA(); testB(); testC(); return 0; }
