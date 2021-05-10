# $Id: Makefile,v 1.6 2021-04-01 02:48:35-07 - - $

MKFILE      = Makefile
DEPFILE     = ${MKFILE}.dep
NOINCL      = ci clean spotless
NEEDINCL    = ${filter ${NOINCL}, ${MAKECMDGOALS}}
GMAKE       = ${MAKE} --no-print-directory
GPPWARN     = -Wall -Wextra -Werror -Wpedantic -Wshadow -Wold-style-cast
GPPOPTS     = ${GPPWARN} -fdiagnostics-color=never
COMPILECPP  = g++ -g -O0 -std=gnu++17 ${GPPOPTS}
MAKEDEPCPP  = g++ -MM -std=gnu++17 ${GPPOPTS}

CPPHEADER   = hello.h debug.h
CPPSOURCE   = hello.cpp debug.cpp main.cpp
ALLCPPSRC   = ${CPPHEADER} ${CPPSOURCE}
OBJECTS     = ${CPPSOURCE:.cpp=.o}
EXECBIN     = hello
OTHERS      = ${MKFILE}
ALLSOURCES  = ${ALLCPPSRC} ${OTHERS}

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${COMPILECPP} -o $@ ${OBJECTS}

%.o : %.cpp
	- checksource $<
	- cpplint.py.perl $<
	${COMPILECPP} -c $<

ci : ${ALLSOURCES}
	- checksource $^
	cid -is $^

clean :
	- rm ${OBJECTS} ${DEPFILE} core test.out files.log

spotless : clean
	- rm ${EXECBIN}

test : ${EXECBIN}
	./${EXECBIN} foo bar

dep : ${ALLCPPSRC}
	@ echo "# ${DEPFILE} created `LC_TIME=C date`" >${DEPFILE}
	${MAKEDEPCPP} ${CPPSOURCE} >>${DEPFILE}

${DEPFILE} : ${MAKEFILE}
	@ touch ${DEPFILE}
	${GMAKE} dep

ifeq (${NEEDINCL}, )
include ${DEPFILE}
endif
