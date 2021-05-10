// $Id: map-insert.cpp,v 1.3 2020-01-29 17:06:19-08 - - $

#include <string>
#include <iostream>
#include <map>
using namespace std;

template <typename map_type>
void dump_map (const string& fname, const map_type& map) {
   cout << fname << ":" << endl;
   for (const auto& item: map) {
      cout << item.first << " -> " << item.second << endl;
   }
}

void use_emplace (int argc, const char* const* argv) {
   map<string,int> msi;
   for (int i = 1; i < argc; ++i) {
      msi.emplace (argv[i], i);
   }
   dump_map (__PRETTY_FUNCTION__, msi);
}

void use_insert (int argc, const char* const* argv) {
   map<string,int> msi;
   for (int i = 1; i < argc; ++i) {
      msi.insert ({argv[i], i});
   }
   dump_map (__PRETTY_FUNCTION__, msi);
}

int main (int argc, char** argv) {
   use_emplace (argc, argv);
   use_insert (argc, argv);
}

//TEST// map-insert foo bar baz qux hello world >map-insert.out 2>&1
//TEST// mkpspdf map-insert.ps map-insert.cpp* map-insert.out

