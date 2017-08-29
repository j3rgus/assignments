#ifndef _LIBERROR_H
#define _LIBERROR_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <syslog.h>
#include <errno.h>

/* Global variables */
extern int dbg_lvl;
extern int sys_lvl;

/* Macros for setting global variables */
#define setdebug(x)		dbg_lvl = x
#define setsyslog(x)	sys_lvl = x
#define clear_errno		errno = 0

/* Function prototypes */
void print_debug(int level, char *format, ...);
void printerr(int cond, char *msg);
void printerr_ex(int cond, char *msg, int ret);

#endif
