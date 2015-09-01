#
#
# Makefile.am:
#
# Copyright (c) 2015 James McKenzie <software@madingley.org>,
# All rights reserved.
#
#
#
man1_MANS=nrfdfu.man

INCLUDES= ${BLUEZ_CFLAGS} ${JSON_C_CFLAGS} ${LIBZIP_CFLAGS}

noinst_HEADERS= prototypes.h ${CHDRS} ${BLUEZ_HDRS}

bin_PROGRAMS = nrfdfu

nrfdfu_SOURCES = ${SRCS}
nrfdfu_LDADD = ${BLUEZ_LIBS} ${JSON_C_LIBS} ${LIBZIP_LIBS}

tidy: ${CSRCS} ${CHDRS}
	indent -i2 -ts0 ${CSRCS} ${CHDRS}	
	/bin/rm -f *~

AM_CFLAGS=-g -Wall


CSRCS= \
	nrfdfu.c \
	util.c \
	zip.c \
	ble.c \
	manifest.c \
	dfu.c \
	hexdump.c

CHDRS= \
	ble.h \
	dfu.h \
	manifest.h \
	project.h \

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

protos:
	echo > prototypes.h
	${CPROTO} ${CPPFLAGS}  -e -v ${CSRCS} > prototypes.new
	mv prototypes.new prototypes.h

