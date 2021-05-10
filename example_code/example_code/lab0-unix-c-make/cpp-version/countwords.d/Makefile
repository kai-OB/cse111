# $Id: Makefile,v 1.1 2020-06-27 19:59:24-07 - - $


GWARN   = -Wall -Wextra -Wpedantic -Wshadow -Wold-style-cast
GOPTS   = ${GWARN} -fdiagnostics-color=never
GPP     = g++ -std=gnu++2a -g -O0 ${GOPTS}
GRIND   = valgrind --leak-check=full --show-reachable=yes
NODEPS  = ${filter ci clean spotless tar, ${MAKECMDGOALS}}
MKTAR   = gtar --create --verbose --gzip

H_FILES =
C_FILES = countwords.cpp
OBJECTS = ${C_FILES:.cpp=.o}
EXECBIN = countwords
SOURCES = ${H_FILES} ${C_FILES} Makefile


all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${GPP} -o $@ $^

%.o : %.cpp
	- cpplint.py.perl $<
	- checksource $<
	${GPP} -c $<

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
	${GPP} -MM ${C_FILES} >Makefile.deps

ifeq (${NODEPS}, )
include Makefile.deps
endif

