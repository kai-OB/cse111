// $Id: sortingfn.cpp,v 1.7 2018-06-20 15:44:45-07 - - $

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<string> vs {"foo", "bar", "baz", "qux", "hello", "world"};
vector<int> vi {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};

template <typename item>
void print (const string& label, const vector<item>& v) {
   cout << label << ": ";
   for (const auto& i: v) cout << " " << i;
   cout << endl;
}

int main() {
   print ("unsorted", vs);
   sort (vs.begin(), vs.end(), less<string>());
   print ("less    ", vs);
   sort (vs.begin(), vs.end(), greater<string>());
   print ("greater ", vs);
   print ("unsorted", vi);
   sort (vi.begin(), vi.end(), less<int>());
   print ("less    ", vi);
   sort (vi.begin(), vi.end(), greater<int>());
   print ("greater ", vi);
   return 0;
}

//TEST// ./sortingfn >sortingfn.lis 2>&1
//TEST// mkpspdf sortingfn.ps sortingfn.cpp* sortingfn.lis

