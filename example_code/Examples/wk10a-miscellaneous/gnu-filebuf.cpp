// $Id: gnu-filebuf.cpp,v 1.2 2021-03-07 19:28:25-08 - - $

#include <cstring>
#include <ext/stdio_filebuf.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

#include <libgen.h>

string execname;

int run_command (const string& command) {
   FILE* pipe = popen (command.c_str(), "r");
   if (pipe == nullptr) {
      cerr << execname << ": " << command << ": " << strerror (errno)
           << endl;
      return EXIT_FAILURE;
   }
   __gnu_cxx::stdio_filebuf<char> filebuf (fileno (pipe), ios::in);
   istream input (&filebuf);
   cout << execname << ": " << command << endl;
   for (;;) {
      string line;
      getline (input, line);
      if (input.eof()) break;
      cout << line << endl;
   }
   return EXIT_SUCCESS;
}

int main (int, char** argv) {
   execname = basename (argv[0]);
   int status = EXIT_SUCCESS;
   status |= run_command ("ls -laog "s + argv[0]);
   status |= run_command ("ls -laog /proc/self/fd"s);
   return status;
}

//TEST// stdio_filebuf >stdio_filebuf.out 2>&1
//TEST// mkpspdf stdio_filebuf.ps stdio_filebuf.cpp* stdio_filebuf.out

