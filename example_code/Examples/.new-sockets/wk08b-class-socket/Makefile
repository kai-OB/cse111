# $Id: Makefile,v 1.20 2020-11-13 23:41:20-08 - - $

GPPWARN    = -Wall -Wextra -Werror -Wpedantic -Wshadow -Wold-style-cast
GPPOPTS    = ${GPPWARN} -fdiagnostics-color=never
GPP        = g++ -std=gnu++17 -g -O0 ${GPPOPTS}

DEPFILE    = Makefile.dep
HEADERS    = sockets.h logstream.h
CPPSRCS    = sockets.cpp client.cpp server.cpp
CLIENTOBJS = client.o sockets.o
SERVEROBJS = server.o sockets.o
OBJECTS    = ${CLIENTOBJS} ${SERVEROBJS}
EXECBINS   = client server
LISTING    = Listing.ps
SOURCES    = ${HEADERS} ${CPPSRCS} Makefile

all: ${DEPFILE} ${EXECBINS}

client: ${CLIENTOBJS}
	${GPP} -o $@ ${CLIENTOBJS}

server: ${SERVEROBJS}
	${GPP} -o $@ ${SERVEROBJS}

%.o: %.cpp
	- cpplint.py.perl $<
	${GPP} -c $<

ci:
	- checksource ${SOURCES}
	- cid -is ${SOURCES}

lis: all ${SOURCES} ${DEPFILE}
	mkpspdf ${LISTING} ${SOURCES} ${DEPFILE}

clean:
	- rm ${LISTING} ${LISTING:.ps=.pdf} ${OBJECTS}

spotless: clean
	- rm ${EXECBINS}

dep:
	- rm ${DEPFILE}
	make --no-print-directory ${DEPFILE}

${DEPFILE}:
	${GPP} -MM ${CPPSRCS} >${DEPFILE}

again: ${SOURCES}
	make --no-print-directory spotless ci all lis

include ${DEPFILE}

