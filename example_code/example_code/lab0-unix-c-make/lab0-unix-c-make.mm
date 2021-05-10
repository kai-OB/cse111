.so Tmac.mm-etc
.if t .Newcentury-fonts
.INITR* \n[.F]
.SIZE 12 14
.ds QUARTER Winter\~2021
.ds SUFFIX s21
.TITLE CSE-111 \*[QUARTER] Lab\~0 "Unix, C, make"
.RCS "$Id: lab0-unix-c-make.mm,v 1.46 2021-03-22 17:37:59-07 - - $"
.PWD
.URL
.nr SHELLCNT 0 1
.de COMMAND
.   br
.   ft CR
.   nop -bash-\\n+[SHELLCNT]$
.   ft CB
.   nop \\$*
.   ft CR
.   br
.   nf
.   tm \\$*
.   pso \\$* | expand
.   br
.   ft R
.   fi
..
.H 1 Introduction
.E= "This lab will not be graded and there is no credit for it."
However, it is important you understand the Unix command
line and submit procedure before you actually use it in the
first assignment.
Learn how to submit files.
Submit these files to
.V= lab0 .
Verify that the submit actually worked by looking
in the submit directory for your username.
The
.V= find (1)
command is very useful for things like this.
.P
The submit command copies your files into a directory
.VTCODE* 1 /afs/cats.ucsc.edu/class/cse111-wm.\*[SUFFIX]/lab0/$USER
and prefixes each file with a sequence number.
You can see the names of the files, 
but not their contents.
The actual syntax of the 
.V= submit
command is\(::
.VTCODE* 1 "submit cse111-wm.\*[SUFFIX] lab0 \fIfiles\|.\|.\|.\fP"
.H 1 "Prior experience"
.E= "Prior experience with Unix is assumed."
You should know the following\(::
.ALX \[bu] 0 "" 0 0
.LI
The C language,
from a prerequisite course.
.LI
General use of Linux commands,
and specifically
.V= submit
and
.V= checksource .
.LI
Use of environment variables and setting the
.V= $PATH
variable.
.LI
Construction and use of
.V= Makefile s.
.LI
A code archival system such as
.V= rcs ,
.V= cvs ,
.V= svn ,
.V= git ,
etc.
.LE
.P
Attend a lab section to ask questions if you don't
understand how submit and the Unix command line in general works.
The TA can explain such things.
Also read the submit checklist\(::
.V= Syllabus/submit-checklist .
.P
There are some scripts that are available, such as
.V= checksource ,
.V= cpplint.py,
.V= cpplint.py.perl,
.V= cid ,
and others.
These can be made available by adding the following to your
.V= $PATH
environment variable\(::
.VTCODE* 1 "/afs/cats.ucsc.edu/courses/cse111-wm/bin"
This can be done with the following line in your
.V= $HOME/.bash_profile \(::
.VTCODE* 1 "export PATH=$PATH:/afs/cats.ucsc.edu/courses/cse111-wm/bin"
.H 1 "Version of C++"
.DS
.B1
.SP .5v
.ft CB
.in +1m
.COMMAND which g++
.COMMAND g++ --version | grep -i g++
.SP .5v
.B2
.DE
Be sure that any textbook or other reference to C++
includes at least C++11 on its cover documentation,
and preferably C++14 or C++17.
Whenever you need to look up information about C++,
or look at 
.V= http://www.cplusplus.com/ .
.P
Any code you develop must compile and run on
.V= unix.ucsc.edu .
That is the only environment the graders will be using.
If you are developing on your own system,
verify that the compiler you are using is at least C++14 compliant.
.H 1 "Review of C and make"
The prerequisite for this course is a knowledge of C, the use of the
.V= make (1)
utility, and the use of Unix in general.
As a review,
write the following program and submit to
.V= lab0 .
There is no credit for this lab and it will not be graded,
but it is useful to verify that you have the necessary knowledge of C.
Alternatively,
write the following program in C++.
.H 1 "Program specification"
Write a program
.V= countwords
that will read in words from all files specified on
the command line, or from the standard input if no files are specified
on the command line.
After all of the files have been read in,
print each word followed by a count of the number of times it occurred.
A word is any sequence of alphabetical
.=V ( isalpha (3))
characters,
and words are converted to lower case
.=V ( tolower (3)).
The following Perl program
.=V ( countwords.perl )
shows the required functionality.
You do not need to know Perl in order to run the program.
.DS
.B1
.SP .5v
.ft CB
.in +1m
.nf
.eo
.so countwords.perl
.ec
.fi
.SP .5v
.B2
.DE
.H 1 "Program implementation"
Write a program in C consisting of three modules as shown here.
Also construct a
.V= Makefile
with the usual targets
.V= all ,
.V= clean ,
and
.V= spotless ,
and recipies to build the object files and binary from source files.
Do not assume anything about the maximum length of a word,
the maximum number of words in the file,
the maximum number of files, 
or any other maxima.
Suggested files\(::
.ALX a () "" 0
.LI
.V= vstring.h ,
.V= vstring.c
\[em]
implements a variable string into which characters can be accumulated,
using array doubling when the length of the string is exceeded.
.LI
.V= wordlist.h ,
.V= wordlist.c
\[em]
implements a list sorted lexicographically with each element of the
list containing a word and a count.
.LI
.V= main.c
\[em]
iterates over all of the files specified on the command line,
or the standard input if none.
For each word found in each line, insert into the list,
updating the count.
At end of file, print out all of the words in lexicographic order
along with the count of the number of times each word appears.
.LI
.V= Makefile
targets\(::
.ALX \[bu] 0 2 0 0
.LI
.V= all\~:
builds the executable binary.
.LI
.V= ${EXECBIN}\~:
builds the executable from the object files.
.LI
.V= %.o\~:
builds object files from source files.
.LI
.V= clean\~:
removes the object files.
.LI
.V= spotless\~:
first does
.V= clean ,
then removes any files not part of the source.
.LI
.V= Makefile.deps\~:
builds the dependency file.
.LE
.LE
.P
As mentioned, this program will not be graded,
so coding it is optional,
but you should verify that you know enough C to do it.
Exact specification details depend on variations on 
implementation preferences.
.P
.H 1 "Alternative versions"
.P
Use a binary search tree instead of a simple linear list.
.P
Use a hash table to hold the information,
then sort the table before printing.
.P
Code in C++, but do not use any of
.V= <map> ,
.V= <regex> ,
.V= <string> ,
.V= <vector> .
Use only raw pointers,
.V= struct s,
.V= malloc (3),
and
.V= free (3)
for the data structures.
.H 1 "Sample output"
.DS
.B1
.SP .5v
.ft CB
.in +1m
.COMMAND countwords.perl countwords.perl mk Makefile | column -c80
.SP .5v
.B2
.DE
