// $Id: variadic-template.cpp,v 1.11 2019-11-12 13:14:56-08 - - $

#include <algorithm>
#include <initializer_list>
#include <iostream>
using namespace std;

void varprint() {
   cout << endl;
}

template <typename Head, typename... Tail>
void varprint (Head head, Tail... tail) {
   cout << head;
   varprint (tail...);
}

template <typename T>
void listprint (initializer_list<T> list) {
   for_each (list.begin(), list.end(), [](T x){ cout << " " << x; });
   cout << endl;
}

int main() {
   varprint ("Hello"s, ' ', 3.48e1, boolalpha, ' ', true);
   listprint ({3, 4, 5});
}

/*
//TEST// variadic-template >variadic-template.out 2>&1
//TEST// mkpspdf variadic-template.ps variadic-template.cpp* \
//TEST//         variadic-template.out
*/
