/*
 * dali_compat.h - Force-included BEFORE all other headers via -include
 *
 * This file tests that the -include flag is actually working.
 * If you see #error below trigger, then -include is working correctly.
 */
#ifndef DALI_COMPAT_H
#define DALI_COMPAT_H

/* Diagnostic: uncomment to verify -include is working */
/* #error dali_compat.h is being included - this proves -include works */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <features.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <grp.h>
#include <stdlib.h>
#include <string.h>

#endif
