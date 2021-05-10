// $Id: initializer.cpp,v 1.7 2018-06-20 14:56:19-07 - - $

//
// Illustrate initializer lists and for loops.
//

#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

int main (int argc, char** argv) {
   vector<string> args (&argv[0], &argv[argc]);
   for (const auto& arg: args) cout << "arg: " << arg << endl;;
   map<string,double> msd = {
     {"foo",26},
     {"bar",99},
     {"baz",-3}
   };
   for (const auto& i: msd) {
      cout << "msd: " << i.first << " " << i.second << endl;
   }
   return EXIT_SUCCESS;
}

//TEST// ./initializer hello world >initializer.lis 2>&1
//TEST// mkpspdf initializer.ps initializer.cpp* initializer.lis

