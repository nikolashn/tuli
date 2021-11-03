# See LICENSE for copyright/license information

include config.mk

all:
	${CC} tuli.c -o tuli ${CFLAGS}

debug:
	${CC} tuli.c -o tuli -g ${CFLAGSDEBUG}

clean:
	rm -f tuli

install: all
	mkdir -p ${DESTDIR}${PPATH}/bin
	cp -f tuli ${DESTDIR}${PPATH}/bin
	chmod 755 ${DESTDIR}${PPATH}/bin/tuli

uninstall:
	rm -f ${DESTDIR}${PPATH}/bin/tuli


.PHONY: all clean install uninstall

