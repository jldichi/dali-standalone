// Test 4: Minimal - just unistd.h, NO extra flags
// Run these three commands and paste ALL output:
//
// g++ -D_GNU_SOURCE test_posix4.cc -o /dev/null 2>&1
// g++ -D_GNU_SOURCE -fno-for-scope test_posix4.cc -o /dev/null 2>&1
// g++ -D_GNU_SOURCE -fpermissive test_posix4.cc -o /dev/null 2>&1

#include <unistd.h>
int main() { return getpid(); }
