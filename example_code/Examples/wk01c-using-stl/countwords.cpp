// $Id: countwords.cpp,v 1.2 2021-01-14 18:47:51-08 - - $

// Count the occurrences of words in cin.
// A word is any whitespace delimited sequence of characters.

#include <iostream>
#include <map>
#include <string>
using namespace std;

int main() {
   map<string,size_t> words;
   string word;
   while (cin >> word) ++words[word];
   for (const auto& item: words) {
      cout << item.first << " " << item.second << endl;
   }
   return 0;
}
