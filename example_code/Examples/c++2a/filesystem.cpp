// $Id: filesystem.cpp,v 1.4 2018-11-16 16:31:27-08 - - $

// Experimental filesystem::path classes.
// Apparently this won't work until gcc version 8.

#include <experimental/filesystem>
#include <iostream>
#include <string>
#include <vector>
namespace fs = std::experimental::filesystem::v1;
using namespace std;

int main (int argc, char** argv) {
   vector<string> args (argv[1], argv[argc]);
   for (char** arg = &argv[1]; arg != &argv[argc]; ++arg) {
      string s = *arg;
      fs::path argpath (*arg);
      cout << "argpath = " << argpath << endl;
      cout << "root_name = " << argpath.root_name() << endl;
      cout << "root_directory = " << argpath.root_directory() << endl;
      cout << "root_path = " << argpath.root_path() << endl;
      cout << "relative_path = " << argpath.relative_path() << endl;
      cout << "parent_path = " << argpath.parent_path() << endl;
      cout << "filename = " << argpath.filename() << endl;
      cout << "stem = " << argpath.stem() << endl;
      cout << "extension = " << argpath.extension() << endl;
   }
   return 0;
}

