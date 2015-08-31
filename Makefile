CSRCS=nrfdfu.c util.c zip.c ble.c manifest.c dfu.c

PROG=nrfdfu

OPT=-g


CPROTO=cproto
ZIP_INCLUDES=$(shell pkg-config --cflags libzip)
ZIP_LIBS=$(shell pkg-config --libs libzip)
JSON_C_INCLUDES=$(shell pkg-config --cflags json-c)
JSON_C_LIBS=$(shell pkg-config --libs json-c)
BLUEZ_INCLUDES=$(shell pkg-config --cflags bluez)
BLUEZ_LIBS=$(shell pkg-config --libs bluez)


INCLUDES=${ZIP_INCLUDES} ${JSON_C_INCLUDES} ${BLUEZ_INCLUDES}
LIBS=${ZIP_LIBS} ${JSON_C_LIBS} ${BLUEZ_LIBS}
CPPFLAGS=${DEFINES} ${INCLUDES}
CFLAGS=${OPT}
OBJS=${CSRCS:%.c=%.o}


all: ${PROG}

${PROG}:${OBJS}
	${CC} ${CFLAGS} ${LDFLAGS} -o ${PROG} ${OBJS} ${LIBS}

protos:
	echo > prototypes.h
	${CPROTO} ${CPPFLAGS}  -e -v ${CSRCS} > prototypes.new
	mv prototypes.new prototypes.h

clean:
	/bin/rm -f ${OBJS}
