// $Id: insert_list.cpp,v 1.1 2016-08-12 15:33:15-07 - - $

// Example of insert_ascending to a list.
// This runs in O(n) time.
// Obviously there are better algorithms.

#include <list>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
using namespace std;

// Insert ascending order but no duplicates.
// Can't use any equality operator, only less.
template <typename T, class Less = less<T>>
void insert_ascending (list<T>& list, const T& item) {
   Less less;
   auto curr = list.begin();
   while (curr != list.end()) {
      if (not (less (*curr, item))) break;
      ++curr;
   }
   if (curr == list.end()) {
      list.push_back (item);
   }else if (less (item, *curr)) {
      list.insert (curr, item);
   }
}

int main() {
   list<string> list;
   istream_iterator<string> cin_itor (cin);
   istream_iterator<string> end_file;
   ostream_iterator<string> cout_itor (cout, "\n");
   vector<string> data (cin_itor, end_file);
   cout << endl << "Unsorted data:" << endl;
   copy (data.begin(), data.end(), cout_itor);
   for (auto& word: data) insert_ascending (list, word);
   cout << endl << "Sorted data:" << endl;
   copy (list.begin(), list.end(), cout_itor);
}

/*
//TEST// echo hello world foo bar baz qux This is some test data. \
//TEST// | insert_list >insert_list.out 2>&1
//TEST// mkpspdf insert_list.ps insert_list.cpp* \
//TEST//         insert_list.out
*/
