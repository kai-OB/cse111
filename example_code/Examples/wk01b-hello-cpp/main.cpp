// $Id: main.cpp,v 1.11 2021-04-01 03:19:29-07 - - $

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include <libgen.h>

#include "debug.h"
#include "hello.h"

int main (int argc, char** argv) {
   string execname {basename (argv[0])};
   if (argc > 1) debug::want = true;

   DEBUG ("program " << execname << " starting");
   vector<string> args (&argv[1], &argv[argc]);
   cout << execname << " args:";
   for (const auto& arg: args) cout << " " << arg;
   cout << endl;

   hello world;
   world.say (cout);

   hello goodbye {"Goodbye, errors!"};
   goodbye.say (cout);

   auto dynamic_var = new hello ("On the heap.");
   dynamic_var->say (cout);
   delete dynamic_var;

   DEBUG ("program " << execname << " finished");
   return EXIT_SUCCESS;
}

