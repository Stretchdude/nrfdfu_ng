#
#
# Makefile.am:
#
# Copyright (c) 2015 James McKenzie <software@madingley.org>,
# All rights reserved.
#
#
#
AUTOMAKE_OPTIONS = subdir-objects

man1_MANS=nrfdfu.man
bin_PROGRAMS = nrfdfu nrfdfulib

#lib_LTLIBRARIES = libnrfdfu-0.1.la
#libnrfdfu_0_1_la_LDFLAGS = -version-info 0:0:0

lib_LTLIBRARIES = libnrfdfu.la

CSRCS_BOTH= \
	util.c \
	zip.c \
	ble.c \
	manifest.c \
	dfu.c \
	hexdump.c \
	crc32.c

CSRCS= \
	nrfdfu.c \
	${CSRCS_BOTH}


CSRCS_LIB= \
	libnrfdfu.c \
	${CSRCS_BOTH}


CHDRS= \
	ble.h \
	dfu.h \
	manifest.h \
	project.h 

BLUEZ_SRCS= \
	bluez/att.c \
	bluez/queue.c \
	bluez/crypto.c \
	bluez/util.c \
	bluez/io-mainloop.c \
	bluez/timeout-mainloop.c \
	bluez/mainloop.c \
	bluez/gatt-db.c \
	bluez/uuid.c \
	bluez/gatt-client.c \
	bluez/gatt-helpers.c

BLUEZ_HDRS= \
	bluez/att.h \
	bluez/att-types.h \
	bluez/crypto.h \
	bluez/gatt-client.h \
	bluez/gatt-db.h \
	bluez/gatt-helpers.h \
	bluez/io.h \
	bluez/mainloop.h \
	bluez/queue.h \
	bluez/timeout.h \
	bluez/util.h \
	bluez/uuid.h


AM_CPPFLAGS = ${BLUEZ_CFLAGS} ${JSON_C_CFLAGS} ${LIBZIP_CFLAGS}

noinst_HEADERS= prototypes.h ${CHDRS} ${BLUEZ_HDRS} libnrfdfu.h


nrfdfu_SOURCES = ${CSRCS} ${BLUEZ_SRCS}
nrfdfu_LDADD = ${BLUEZ_LIBS} ${JSON_C_LIBS} ${LIBZIP_LIBS}

nrfdfulib_SOURCES = example.c
nrfdfulib_LDADD = .libs/libnrfdfu.la  ${BLUEZ_LIBS} ${JSON_C_LIBS} ${LIBZIP_LIBS}


libnrfdfu_la_SOURCES =  ${CSRCS_LIB} ${BLUEZ_SRCS}
#libnrfdfu_la_LDADD = ${BLUEZ_LIBS} ${JSON_C_LIBS} ${LIBZIP_LIBS}
libnrfdfudir=.
libnrfdfu_HEADERS= libnrfdfu.h

#libnrfdfu_0_1_la_SOURCES =  ${CSRCS} ${BLUEZ_SRCS}
##libnrfdfu_la_LDADD = ${BLUEZ_LIBS} ${JSON_C_LIBS} ${LIBZIP_LIBS}
#libnrfdfu_0_1dir=.
#libnrfdfu_0_1_HEADERS= libnrfdfu.h

tidy: ${CSRCS} ${CHDRS}
	indent -i2 -ts0 ${CSRCS} ${CHDRS}	
	/bin/rm -f *~

AM_CFLAGS=-g -Wall
nrfdfu_CFLAGS = $(AM_CFLAGS)
nrfdfulib_CFLAGS = $(AM_CFLAGS)

protos:
	echo > prototypes.h
	${CPROTO} ${AM_CPPFLAGS}  -e -v ${CSRCS} > prototypes.new
	mv prototypes.new prototypes.h

