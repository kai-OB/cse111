// $Id: fnmap.cpp,v 1.29 2021-04-09 11:04:22-07 - - $

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

using function = void (*)(int);
using commandmap = unordered_map<string,function>;

void fn_foo (int index) {
   cout << "Function foo index " << index << endl;
}

void fn_bar (int index) {
   cout << "This is function bar index " << index << endl;
}

void fn_qux (int index) {
   cout << "Why would you call qux? index " << index << endl;
}

commandmap cmdmap = {
   {"foo", fn_foo},
   {"bar", fn_bar},
   {"qux", fn_qux},
};

string basename (const string &name) {
   return name.substr (name.rfind ('/') + 1);
}

int main (int argc, char** argv) {
   int exit_status = EXIT_SUCCESS;
   string progname = basename (argv[0]);
   vector<string> args (&argv[1], &argv[argc]);
   for (const auto& cmd: args) {
      int index = &cmd - &args[-1];
      try {
         function fn = cmdmap.at(cmd);
         fn (index);
      }catch (out_of_range &exn) {
         cerr << progname << ": " << index << " " << cmd
              << ": not found " << endl;
         exit_status = EXIT_FAILURE;
      }
   }
   return exit_status;
}

//TEST// ./fnmap foo bar baz qux quux >fnmap.lis 2>&1
//TEST// mkpspdf fnmap.ps fnmap.cpp* fnmap.lis

