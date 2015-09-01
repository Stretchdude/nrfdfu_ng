dnl
dnl
dnl configure.in:
dnl
dnl Copyright (c) 2015 James McKenzie <software@madingley.org>,
dnl All rights reserved.
dnl
dnl
AC_PREREQ(2.13)
AC_INIT(nrfdfu.c)

AC_PROG_CC
AC_SYS_LARGEFILE

VERSION=1.00
PACKAGE=nrfdfu

AM_INIT_AUTOMAKE($PACKAGE,$VERSION)

AM_CONFIG_HEADER(config.h)

AC_CHECK_PROG(CPROTO,cproto,cproto)


AC_CHECK_HEADERS(fcntl.h stdlib.h stdint.h unistd.h)

AC_C_INLINE
AC_C_CONST
AC_HEADER_TIME
AC_STRUCT_TM
dnl AC_CHECK_FUNCS()

dnl CFLAGS=-g

AC_OUTPUT([Makefile])

