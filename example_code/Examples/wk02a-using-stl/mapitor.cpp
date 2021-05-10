// $Id: mapitor.cpp,v 1.14 2021-01-25 19:58:54-08 - - $

//
// mapitor - put argv values into a map and iterate over them
//

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

using namespace std;

using string_int_map = map<string,int>;

void load_map (string_int_map &map, const char* const* begin,
                                    const char* const* end) {
   for (const char* const* itor = begin; itor != end; ++itor) {
      map.insert ({*itor, itor - begin});
   }
}

void print_map (const string_int_map &map) {

   cout << endl << "Forward iterator:" << endl;
   for (const auto& entry: map) {
      cout << "   " << entry.first << " " << entry.second << endl;
   }

   cout << endl << "Explicit Forward iterator:" << endl;
   for (string_int_map::const_iterator itor = map.begin();
        itor != map.end(); ++itor) {
      cout << "   " << itor->first << " " << itor->second << endl;
   }

   cout << endl << "Reverse iterator:" << endl;
   for (auto ritor = map.crbegin(); ritor != map.crend(); ++ritor) {
      cout << "   " << ritor->first << " " << ritor->second << endl;
   }
}

int main (int argc, char** argv) {
   string_int_map map;
   load_map (map, &argv[0], &argv[argc]);
   print_map (map);
   return EXIT_SUCCESS;
}

//TEST// ./mapitor foo bar baz qux quux >mapitor.lis
//TEST// mkpspdf mapitor.ps mapitor.cpp* mapitor.lis

