/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: gnshidea.cc,v 1.1.1.1 1996/04/16 18:52:01 gustavof Exp $
*
* DESCRIPTION
* A source file to hold definitions of the imported symbols
* pointers.
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>

#if defined(SHIDEAIX) || defined(SHIDEAES)


extern "C" {

char *strcpy(char *, char const *) = 0;
char *strncpy(char *, char const *, SizeT) = 0;
char *strchr(char const *, int) = 0;
char *strrchr(char const *, int) = 0;
char *strcat(char *, char const *) = 0;
char *strncat(char *, char const *, SizeT) = 0;
char *strtok(char *, const char *) = 0;
char *strdup(const char *) = 0;

#ifdef HAVE_STRSTR
	char *strstr(char const *, char const *) = 0;
#endif

void *memcpy(void *, void const *, SizeT) = 0;
void *memmove(void *, void const *, SizeT) = 0;
void *memset(void *, int, SizeT) = 0;
ALLOCRETTYPE malloc(SizeT) = 0;
ALLOCRETTYPE _global_realloc(void *, SizeT) = 0;
char *getcwd(GETCWDPARAM1TYPE,GETCWDPARAM2TYPE) = 0;
char *tempnam(TEMPNAMPARAM1TYPE, TEMPNAMPARAM2TYPE) = 0;
#ifdef HAVE_PID
PidT getpid(void) = 0;	
PidT getppid(void) = 0;	
#endif
int kill(PidT pid, int sig) = 0;
int atoi(char const *) = 0;
int memcmp(void const *, void const *, SizeT) = 0;
int mkdir(MKDIRPARAM1TYPE, ModeT) = 0;
int fcntl(int fildes, int cmd, FCNTLPARAM3TYPE) = 0;
int ioctl(int, int, ...) = 0;
int link(char const *, char const *) = 0;
char *tmpnam(char *) = 0;
char *getenv(char const *) = 0;
ABORTRETTYPE _global_abort(void) = 0;
PidT fork(void) = 0;
int execvp(EXECVPPARAM1TYPE, EXECVPPARAM2TYPE) = 0;
int dup(int) = 0;
int dup2(int, int) = 0;
int chdir(char const *dir) = 0;
char *ttyname(int fildes) = 0;
int msgget(KeyT k, int flg) = 0;
int msgsnd(int, MSGSNDPARAM2TYPE, MSGSNDPARAM3TYPE, int) = 0;
int msgrcv(int, MSGRCVPARAM2TYPE, MSGRCVPARAM3TYPE, long, int) = 0;
int msgctl(int, int, MSGCTLPARAM3TYPE) = 0;
int lockf(int fildes, int function, long size) = 0; 
int unlink(const char *p) = 0;
#ifdef HAVE_FSYNC
int fsync(int fd) = 0;
#endif
SizeT strlen(char const *) = 0;
int strncmp(char const *, char const *, SizeT) = 0;
int strcmp(char const *, char const *) = 0;
ModeT umask(ModeT cmask) = 0;
long atol(char const *) = 0;
READRETTYPE _global_read(int, void *, READPARAM3TYPE) = 0;
WRITERETTYPE _global_write(int, void const *, WRITEPARAM3TYPE) = 0;
int _global_open(char const *, int, OPENPARAM3TYPE) = 0;
PidT setsid(void) = 0;
PidT setpgrp(void) = 0;
int setuid(UidT) = 0;
int setgid(GidT) = 0;
int setpgid(PidT, PidT) = 0;
long _global_time(long *a) = 0;
long lseek(int, long, int) = 0;
int putenv(PUTENVPARAMTYPE) = 0;
int system(const char *s) = 0;
int pipe(int *) = 0;
PidT wait(int *) = 0;
PidT waitpid(PidT, int *, int) = 0;
int semget(KeyT k, int n, int s) = 0;
int semop(int i, struct sembuf *, SEMOPPARAM3TYPE) = 0;
int semctl(int i, int s, int c,  union semun) = 0;
int fstat(int f, struct stat *s) = 0;
int stat(char const *path, struct stat *s) = 0;
SLEEPRETTYPE sleep(SLEEPPARAMTYPE s) = 0;
UidT getuid(void) = 0;
GidT getgid(void) = 0;
PAUSERETTYPE _global_pause(void) = 0;
void free(void *) = 0;
EXITRETTYPE _global_exit(int) = 0;
EXITRETTYPE _exit(int) = 0;
void tzset(void) = 0;
void qsort(void *, QSORTPARAM2TYPE, QSORTPARAM3TYPE, IFPVCPVCP) = 0;
void openlog(const char *ident, int option, int facility) = 0;
void syslog(int priority, const char *format, ...) = 0;
int _global_close(int) = 0;
unsigned alarm(unsigned) = 0;
UidT geteuid(void) = 0;
GidT getegid(void) = 0;
void closelog(void) = 0;
SBRKRETTYPE sbrk(int) = 0;
FCVTRETTYPE fcvt(double, FCVTPARAM2TYPE, int *, int *) = 0;
FCVTRETTYPE ecvt(double, FCVTPARAM2TYPE, int *, int *) = 0;
double atof(char const *s) = 0;
long ulimit(int, long) = 0;
void bcopy(void const *, void *, BCOPYPARAM3TYPE) = 0;
void bzero(void *, int) = 0;
int poll(POLLPARAM1TYPE, POLLPARAM2TYPE, POLLPARAM3TYPE) = 0;
struct passwd *getpwuid(GETPWUIDPARAMTYPE) = 0;
struct passwd *getpwnam(char const *name) = 0;
SYNCRETTYPE _global_sync(void) = 0;

#ifdef HAVE_SYS_UTSNAME_H
		int uname(struct utsname *) = 0;
#endif /* HAVE_SYS_UTSNAME_H */
long fpathconf(int, int) = 0;
int getgroups(int, GidT []) = 0;
void srand48(long s) = 0;
long lrand48(void) = 0;
int chown(char const *path, CHOWNPARAM2TYPE, CHOWNPARAM3TYPE) = 0;
int isatty(int) = 0;
int statfs(const char *, struct statfs *, int, int) = 0;
FILE *fdopen(int, FDOPENPARAM2TYPE) = 0;
FILE *popen(POPENPARAMTYPE, POPENPARAMTYPE) = 0;
int pclose(FILE *) = 0;
FILE *fopen(char const *, char const *) = 0;
int fclose(FILE *) = 0;
int fflush(FILE *) = 0;
FTELLRETTYPE ftell(FTELLPARAM1TYPE) = 0;
int fseek(FILE *, long, int) = 0;
SizeT fread(FREADPARAM1TYPE, SizeT, SizeT, FILE *) = 0;
SizeT fwrite(FWRITEPARAM1TYPE, SizeT, SizeT, FILE *) = 0;
char *getpass(char const *prompt) = 0;
int execl(char const *path, char const *arg1, ...) = 0;
int rmdir(char const *p) = 0;
int access(char const *f, int a) = 0;
int rand(void) = 0;
SRANDRETTYPE srand(unsigned) = 0;
int fputc(FPUTCPARAM1TYPE, FILE *) = 0;
int fputs(char const *, FILE *) = 0;
char *fgets(char *, int , FILE *) = 0;
int fgetc(FILE *) = 0;
FILE _iob[]     = 0;
int toupper(int) = 0;
int tolower(int) = 0;
int errno = 0;
FILE *tmpfile(void) = 0;
void (*sigset(int, void (*)(int)))(void) = 0;
struct group *getgrgid(GidT) = 0;
struct group *getgrnam(char const *) = 0;
int _filbuf(FILE *) = 0;
long altzone = 0;
struct tm *localtime(void) = 0;
char *sys_errlist[] = 0;
int sys_nerr = 0;
int utime(char *, struct utimbuf *) = 0;
long timezone = 0;
void *opendir(void) = 0;
int closedir(void) = 0;
struct dirent *readdir(void) = 0;
char *mktemp(char *) = 0;
int _flsbuf(unsigned char, FILE *) = 0;
int fileno(FILE *) = 0;

int bind(int, BINDPARAM2TYPE, int) = 0;
CONNECTRETTYPE _global_connect(CONNECTPARAM1TYPE, CONNECTPARAM2TYPE, int) = 0;
SOCKETRETTYPE socket(int, int, int) = 0;
int listen(int, int) = 0;
int accept(int, ACCEPTPARAM2TYPE, int *) = 0;
unsigned long inet_addr(const char *) = 0;
int getpeername(int s, void *name, int *namelen) = 0;

#ifdef HAVE_STD_TCP_HEADERS
int gethostname(char *, GETHOSTNPARAM2TYPE) = 0;
struct hostent *gethostbyname(GETHOSTBNPARAMTYPE) = 0;
struct servent *getservbyname(GETSERVBNPARAM1TYPE, GETSERVBNPARAM2TYPE) = 0;
#endif	

}

#endif
