// $Id: ord-unord-map.cpp,v 1.6 2021-02-04 11:17:05-08 - - $

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

#ifdef __GNUG__

#include <cxxabi.h>
template <typename type>
string demangle (const type &object) {
   const char *const name = typeid (object).name();
   int status = 0;
   using deleter = void (*) (void*);
   unique_ptr<char,deleter> result {
      abi::__cxa_demangle (name, nullptr, nullptr, &status),
      std::free,
   };
   return status == 0 ? result.get() : name; 
}

#else

template <typename type>
string demangle (const type& object) {
   return typeid (object).name();
}

#endif

vector<string> vec {"foo", "bar", "baz", "qux", "hello", "world"};

template <typename map_t>
void dump_map (const string& label, const map_t& cont) {
   cout << "**** " << label << " ****" << endl;
   for (const auto& item: cont) {
      cout << item.first << ": " << item.second << endl;
   }
}

template <typename map_t>
void test_map (map_t&& cont) {
   cout << endl;
   cout << typeid (cont).name() << endl;
   cout << demangle (cont) << endl;
   for (size_t isub = 0; isub < vec.size(); ++isub) {
      cont.emplace (vec[isub], isub);
   }
   dump_map ("emplaced", cont);
   for (size_t isub = 0; isub < vec.size() / 2; ++isub) {
      cont.erase (vec[isub]);
   }
   dump_map ("erased", cont);
}

int main() {
   for (size_t isub = 0; isub < vec.size(); ++isub) {
      cout << isub << ": " << vec[isub] << "; ";
   }
   cout << endl;
   test_map (map<string,size_t>());
   test_map (unordered_map<string,size_t>());
   return EXIT_SUCCESS;
}

