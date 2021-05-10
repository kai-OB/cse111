// $Id: hello.cpp,v 1.12 2018-06-20 14:54:24-07 - - $

//
// NAME
//    hello - the classic ``hello world'' program
//
// SYNOPSIS
//    hello
//
// DESCRIPTION
//    Prints out the message ``Hello, world!''.
//

#include <cstdlib>
#include <iostream>
using namespace std;

int main (int argc, char** argv) {
   cout << "Hello, world!" << endl;
   cout << "Exec name is " << argv[0] << endl;
   cerr << "argc = " << argc << endl;
   return EXIT_SUCCESS;
}

//TEST// ./hello 1>hello.out 2>hello.err
//TEST// echo EXIT STATUS = $? >hello.exit
//TEST// catnv hello.cpp.log hello.out hello.err hello.exit >hello.lis
//TEST// rm -v hello.cpp.log hello.out hello.err hello.exit
//TEST// mkpspdf hello.ps hello.cpp hello.lis

