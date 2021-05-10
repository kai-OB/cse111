// $Id: basename.cpp,v 1.4 2021-04-10 15:56:39-07 - - $

// Test basename function

#include <iostream>
using namespace std;

string basename (const string &name) {
   return name.substr (name.rfind ('/') + 1);
}

int main (int argc, char** argv) {
   for (char** argp = &argv[0]; argp != &argv[argc]; ++argp) {
       cout << "basename (" << *argp << ") = ("
            << basename (*argp) << ")" << endl;
   }
   cout << "string::npos = " << string::npos << endl;
   cout << "string::npos + 1 = " << string::npos + 1 << endl;
   return 0;
}

