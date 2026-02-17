#ifdef __NT__ /* unistd compatibility header. Only for NT */

#  include <stdlib.h>		/* for mkdir, etc */

#  define access _access
#  define _global_access _access
#  define R_OK  4               /* Test for read permission.  */
#  define W_OK  2               /* Test for write permission.  */
#  define F_OK  0               /* Test for existence.  */


#define S_IRWXU 00700
#define S_IRUSR 00400
#define S_IWUSR 00200
#define S_IXUSR 00100

#define S_IRWXG 00070
#define S_IRGRP 00040
#define S_IWGRP 00020
#define S_IXGRP 00010

#define S_IRWXO 00007
#define S_IROTH 00004
#define S_IWOTH 00002
#define S_IXOTH 00001

#define mkdir(a,b) _mkdir(a)

#endif // __NT__
