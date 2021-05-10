# $Id: Makefile,v 1.34 2016-07-21 19:04:26-07 - - $

DEPFILE  = Makefile.dep
NOINCL   = ci clean spotless
NEEDINCL = ${filter ${NOINCL}, ${MAKECMDGOALS}}
PROGRAMS = testbool.cpp testint.cpp testpointer.cpp \
           testsort.cpp teststring.cpp testvector.cpp
OBJFILES = ${PROGRAMS:.cpp=.o}
BINARIES = ${PROGRAMS:.cpp=}
TESTRUNS = ${foreach file, ${PROGRAMS}, ${file} ${file:.cpp=.out}}
AUXFILES = xvector.h xvector.tcc Makefile
OUTPUTS  = ${PROGRAMS:.cpp=.out}
LISTING  = Listing.ps

GPP      = g++ -g -O0 -Wall -Wextra -std=gnu++14
GRIND    = valgrind --leak-check=full --show-reachable=yes

all : ${BINARIES}

% : %.o
	${GPP} $< -o $@

%.o : %.cpp
	cpplint.py.perl $<
	${GPP} $< -c
		
ci : ${AUXFILES} ${PROGRAMS}
	cid -is ${AUXFILES} ${PROGRAMS}
	checksource ${AUXFILES} ${PROGRAMS}

out : ${OUTPUTS}

%.out : %
	${GRIND} $< >$@ 2>&1; pstatus >>$@

lis : out
	pkill gv || exit 0
	mkpspdf ${LISTING} ${AUXFILES} ${TESTRUNS}

${DEPFILE} :
	${GPP} -MM ${PROGRAMS} >${DEPFILE}
	cat ${DEPFILE}

clean :
	- rm ${DEPFILE} ${OBJFILES}

spotless : clean
	- rm ${BINARIES} ${OUTPUTS} ${LISTING} ${LISTING:.ps=.pdf}

again :
	${MAKE} spotless ci
	${MAKE} all out lis

ifeq (${NEEDINCL},)
include ${DEPFILE}
endif

