/*
 * dali_compat.h - Force-included BEFORE all other headers
 *
 * This ensures POSIX function declarations (getpid, chdir, sleep, etc.)
 * are resolved before IdeaFix headers can interfere with __THROW or
 * other glibc internals.
 */
#ifndef DALI_COMPAT_H
#define DALI_COMPAT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <grp.h>
#include <stdlib.h>

#ifdef __cplusplus
}
#endif

#endif
