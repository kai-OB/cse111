# $Id: Makefile,v 1.1 2019-12-17 14:53:52-08 - - $

GCC     = gcc -g -Wall -Wextra -std=gnu11
GRIND   = valgrind --leak-check=full --show-reachable=yes
NODEPS  = ${filter ci clean spotless tar, ${MAKECMDGOALS}}
MKTAR   = gtar --create --verbose --gzip

H_FILES = vstring.h wordlist.h
C_FILES = vstring.c wordlist.c main.c
OBJECTS = ${C_FILES:.c=.o}
EXECBIN = countwords
SOURCES = ${H_FILES} ${C_FILES} Makefile


all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${GCC} -o $@ $^

%.o : %.c
	- checksource $<
	${GCC} -c $<

ci : ${SOURCES}
	- checksource $^
	cid -is $^

clean :
	- rm --force ${OBJECTS} test.log test.out test.err

lis : ${SOURCES} Makefile.deps
	mkpspdf Listing.ps $^

spotless : clean
	- rm --force ${EXECBIN} Listing.{ps,pdf} Makefile.deps

tar : ${SOURCES}
	${MAKE} --no-print-directory spotless
	( DIRNAME=$$(basename $$(pwd)) \
	; cd .. \
	; ${MKTAR} --exclude=RCS --file=countwords.tar.gz $$DIRNAME \
	)

test : ${EXECBIN}
	${GRIND} --log-file=test.log \
	         ${EXECBIN} ${SOURCES} 1>test.out 2>test.err

Makefile.deps :
	${GCC} -MM ${C_FILES} >Makefile.deps

ifeq (${NODEPS}, )
include Makefile.deps
endif

