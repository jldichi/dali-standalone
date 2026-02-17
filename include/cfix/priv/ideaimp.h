/*
	$Id: ideaimp.h,v 1.3 1998/09/04 15:00:29 mauro Exp $
	$Date: 1998/09/04 15:00:29 $
*/
#ifndef	IDEAIMP_H
#define	IDEAIMP_H

#if defined(SHIDEAIX) || defined(SHIDEAES)

#define strcpy		(*_libidea_strcpy)
#define strncpy		(*_libidea_strncpy)
#define strchr		(*_libidea_strchr)
#define strrchr		(*_libidea_strrchr)
#define strcat		(*_libidea_strcat)
#define strncat		(*_libidea_strncat)
#define strtok		(*_libidea_strtok)
#define strdup		(*_libidea_strdup)

#ifdef HAVE_STRSTR
#	define strstr	(*_libidea_strstr)
#endif
#define memcpy		(*_libidea_memcpy)
#define memmove		(*_libidea_memmove)
#define memset		(*_libidea_memset)
#define malloc		(*_libidea_malloc)
#define getcwd		(*_libidea_getcwd)
#define tempnam		(*_libidea_tempnam)
#define getpid		(*_libidea_getpid)
#define getppid		(*_libidea_getppid)
#define kill		(*_libidea_kill)
#define atoi		(*_libidea_atoi)
#define memcmp		(*_libidea_memcmp)
#define mkdir		(*_libidea_mkdir)
#define fcntl		(*_libidea_fcntl)
#define ioctl		(*_libidea_ioctl)
#define link		(*_libidea_link)
#define tmpnam		(*_libidea_tmpnam)
#define getenv		(*_libidea_getenv)
#define fork		(*_libidea_fork)
#define execvp		(*_libidea_execvp)
#define dup			(*_libidea_dup)
#define chdir		(*_libidea_chdir)
#define ttyname		(*_libidea_ttyname)
#define msgget		(*_libidea_msgget)
#define msgsnd		(*_libidea_msgsnd)
#define msgrcv		(*_libidea_msgrcv)
#define msgctl		(*_libidea_msgctl)
#define lockf		(*_libidea_lockf)
#define unlink		(*_libidea_unlink)
#define fsync		(*_libidea_fsync)
#define strlen		(*_libidea_strlen)
#define strncmp		(*_libidea_strncmp)
#define strcmp		(*_libidea_strcmp)
#define umask		(*_libidea_umask)
#define atol		(*_libidea_atol)
#define setpgrp		(*_libidea_setpgrp)
#define setuid		(*_libidea_setuid)
#define setgid		(*_libidea_setgid)
#define setpgid		(*_libidea_setpgid)
#define lseek		(*_libidea_lseek)
#define putenv		(*_libidea_putenv)
#define system		(*_libidea_system)
#define pipe		(*_libidea_pipe)
#define wait		(*_libidea_wait)
#define waitpid		(*_libidea_waitpid)
#define openlog		(*_libidea_openlog)
#define semget		(*_libidea_semget)
#define semctl		(*_libidea_semctl)
#define semop		(*_libidea_semop)
#define fstat		(*_libidea_fstat)
#define stat(a, b)	(*_libidea_stat)(a, b)
#define sleep		(*_libidea_sleep)
#define umask		(*_libidea_umask)
#define getuid		(*_libidea_getuid)
#define getgid		(*_libidea_getgid)
#define free		(*_libidea_free)
#define _exit		(*_libidea__exit)
#define tzset		(*_libidea_tzset)
#define qsort		(*_libidea_qsort)
#define openlog		(*_libidea_openlog)
#define syslog		(*_libidea_syslog)
#define alarm		(*_libidea_alarm)
#define geteuid		(*_libidea_geteuid)
#define getegid		(*_libidea_getegid)
#define closelog	(*_libidea_closelog)
#define sbrk		(*_libidea_sbrk)
#define fcvt		(*_libidea_fcvt)
#define ecvt		(*_libidea_ecvt)
#define atof		(*_libidea_atof)
#define ulimit		(*_libidea_ulimit)
#define bcopy		(*_libidea_bcopy)
#define bzero		(*_libidea_bzero)
#define poll		(*_libidea_poll)
#define getpwuid	(*_libidea_getpwuid)
#define getpwnam	(*_libidea_getpwnam)
#define uname		(*_libidea_uname)
#define fpathconf	(*_libidea_fpathconf)
#define getgroups	(*_libidea_getgroups)
#define srand48		(*_libidea_srand48)
#define lrand48		(*_libidea_lrand48)
#define chown		(*_libidea_chown)
#define isatty		(*_libidea_isatty)
#define statfs(a, b, c, d)		(*_libidea_statfs)(a, b, c, d)
#define dup2		(*_libidea_dup2)
#define setsid		(*_libidea_setsid)
#define fdopen		(*_libidea_fdopen)
#define popen		(*_libidea_popen)
#define pclose		(*_libidea_pclose)
#define fopen		(*_libidea_fopen)
#define fclose		(*_libidea_fclose)
#define fflush		(*_libidea_fflush)
#define ftell		(*_libidea_ftell)
#define fseek		(*_libidea_fseek)
#define fread		(*_libidea_fread)
#define fwrite		(*_libidea_fwrite)
#define getpass		(*_libidea_getpass)
#define execl		(*_libidea_execl)
#define rmdir		(*_libidea_rmdir)
#define access		(*_libidea_access)
#define rand		(*_libidea_rand)
#define srand		(*_libidea_srand)
#define fputc		(*_libidea_fputc)
#define fputs		(*_libidea_fputs)
#define fgets		(*_libidea_fgets)
#define fgetc		(*_libidea_fgetc)
#define _iob		(*_libidea__iob)
#define toupper		(*_libidea_toupper)
#define tolower		(*_libidea_tolower)
#define errno		(*_libidea_errno)
#define tmpfile		(*_libidea_tmpfile)
#define sigset		(*_libidea_sigset)
#define getgrgid	(*_libidea_getgrgid)
#define getgrnam	(*_libidea_getgrnam)
#define readdir		(*_libidea_readdir)
#define _filbuf		(*_libidea__filbuf)
#define altzone		(*_libidea_altzone)
#define closedir	(*_libidea_closedir)
#define localtime	(*_libidea_localtime)
#define mktemp		(*_libidea_mktemp)
#define opendir		(*_libidea_opendir)
#define sys_errlist	(*_libidea_sys_errlist)
#define sys_nerr	(*_libidea_sys_nerr)
#define timezone	(*_libidea_timezone)
#define utime		(*_libidea_utime)
#define _flsbuf		(*_libidea__flsbuf)
#define fileno		(*_libidea_fileno)

#define _proto_open		(*_libidea_open)
#define _proto_close	(*_libidea_close)
#define _proto_read		(*_libidea_read)
#define _proto_write	(*_libidea_write)
#define _proto_exit		(*_libidea_exit)
#define _proto_realloc	(*_libidea_ralloc)
#define _proto_time		(*_libidea_time)
#define _proto_abort	(*_libidea_abort)
#define _proto_sync		(*_libidea_sync)
#define _proto_pause	(*_libidea_pause)

#define _global_open	(*_libidea_open)
#define _global_close	(*_libidea_close)
#define _global_read	(*_libidea_read)
#define _global_write	(*_libidea_write)
#define _global_exit	(*_libidea_exit)
#define _global_realloc	(*_libidea_realloc)
#define _global_time	(*_libidea_time)
#define _global_abort	(*_libidea_abort)
#define _global_sync	(*_libidea_sync)
#define _global_pause	(*_libidea_pause)

#define bind			(*_libidea_bind)
#define socket			(*_libidea_socket)
#define listen			(*_libidea_listen)
#define accept			(*_libidea_accept)
#define gethostname		(*_libidea_gethostname)
#define gethostbyname	(*_libidea_gethostbyname)
#define getservbyname	(*_libidea_getservbyname)
#define inet_addr		(*_libidea_inet_addr)
#define _proto_connect	(*_libidea_connect)
#define _global_connect	(*_libidea_connect)
#define getpeername		(*_libidea_getpeername)

#else	/* SHIDEAIX || SHIDEAES */

#define _proto_realloc		realloc
#define _proto_time		time
#define _proto_connect		connect
#define _proto_abort		abort
#define _proto_sync		sync
#define _proto_pause		pause
#define _proto_exit		exit

#ifndef HAVE_UNDERSCORE_NAMES

#	define _proto_open		open
#	define _proto_close		close
#	define _proto_read		read
#	define _proto_write		write

#else

#	define _proto_open		_open
#	define _proto_close		_close
#	define _proto_read		_read
#	define _proto_write		_write

#endif

#ifdef __cplusplus

#	ifndef HAVE_UNDERSCORE_NAMES

#	define _global_open	::open
#	define _global_close	::close
#	define _global_read	::read
#	define _global_write	::write

#	else

#	define _global_open	_open
#	define _global_close	_close
#	define _global_read	_read
#	define _global_write	_write
	
#	endif

#	define _global_realloc	::realloc
#	define _global_time	::time
#	define _global_connect	::connect
#	define _global_abort	::abort
#	define _global_sync	::sync
#	define _global_pause	::pause
#	define _global_exit	::exit

#else	/* __cplusplus */

#ifndef HAVE_UNDERSCORE_NAMES

#define _global_open	open
#define _global_close	close
#define _global_read	read
#define _global_write	write

#else

#define _global_open	_open
#define _global_close	_close
#define _global_read	_read
#define _global_write	_write

#endif

#define _global_realloc	realloc
#define _global_time	time
#define _global_connect	connect
#define _global_abort	abort
#define _global_sync	sync
#define _global_pause	pause
#define _global_exit	exit

#endif	/* __cplusplus */

#endif	/* SHIDEAIX || SHIDEAES */

#endif	/* IDEAIMP_H */
