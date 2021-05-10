// $Id: thing-iterator.cpp,v 1.7 2021-04-15 17:14:04-07 - - $

// Illustrate usage of iteration over a container.
// Show minimal operations needed to implement an iterator.

#include <iostream>
#include <vector>
using namespace std;

struct thing {
   int array[5] {1, 2, 3, 4, 5};

   struct iterator {
         int* itor = nullptr;
         iterator (int* i): itor(i) {}
         iterator& operator++() { ++this->itor; return *this; }
         int& operator*() { return *this->itor; }
         bool operator!= (iterator i) { return this->itor != i.itor; }
   };

   size_t size() { return sizeof array / sizeof *array; }
   iterator begin() { return iterator(&array[0]); }
   iterator end() { return iterator(&array[size()]); }
   int& operator[] (size_t i) { return array[i]; }
};

int main() {
   thing a;
   for (size_t i = 0; i != a.size(); ++i) cout << " " << a[i];
   cout << endl;
   for (auto p = a.begin(); p != a.end(); ++p) cout << " " << *p;
   cout << endl;
   for (auto& i: a) cout << " " << i;
   cout << endl;
}

