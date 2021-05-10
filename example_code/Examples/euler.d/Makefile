# $Id: Makefile,v 1.4 2020-11-19 07:04:19-08 - - $

GROFFDIR = /afs/cats.ucsc.edu/courses/cse110a-wm/groff
GROFFMM  = groff -spte -Tps -mgm -U -b -ww -M${GROFFDIR}/tmac

CPPSRC   = ${wildcard *.cpp}
CPPOUT   = ${CPPSRC:.cpp=.output}

all : formula.pdf ${CPPOUT}
	cid -is Makefile

%.pdf : %.ps
	mkpdf $<

%.ps : %.mm
	cid -is $<
	${GROFFMM} $< >$@
	letterbbox $@
	- pkill -s 0 gv || true
	gv $@ &

%.output : %
	$< >$@

% : %.cpp
	cid -is $<
	cpplint.py.perl $<
	mkc -q $<
	
