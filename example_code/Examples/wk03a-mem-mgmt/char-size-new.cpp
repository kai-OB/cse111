// $Id: char-size-new.cpp,v 1.9 2021-04-27 18:35:59-07 - - $

#include <iostream>
using namespace std;

const void* cast (const char* p) {
   return static_cast<const void*> (p);
}

int main() {
   const char* p = new char;
   cout << cast (p) << endl;
   for (size_t i = 0; i < 15; ++i) {
      const char* q = new char;
      cout << cast (q) << " " << (q - p) << endl;
      p = q;
   }
   return 0;
}

// 
// Diagram of memory allocated by new char
// 
//          +---+---+---+---+---+---+---+---+
// prev<----|       boundary tag            |
//          +---+---+---+---+---+---+---+---+
//          |       boundary tag            |---->next
//          +---+---+---+---+---+---+---+---+
//          | x |   padding                 |
//          +---+---+---+---+---+---+---+---+
//          |       padding                 |
//          +---+---+---+---+---+---+---+---+
// 
// x is a single byte allocated.
// Smallest chunk of heap memory is 32 bytes.
// 
// Memory always allocted on a 16-byte boundary.
// Memory request rounded up to 16 bytes by padding.
// Actual allocated memory = n chunks of 16 bytes
//                         + 1 boundary tag
// A boundary tag has two pointers, one to the
// previously allocated chunk, and one to the next,
// sorted by address.
// 
// Memory:
//      1 KiB = 2^10 bytes = 1024
//     64 KiB = 2^16 bytes = 65536 (16-bit address)
// Mega 1 MiB = 2^20 byres = 1048576
// Giga 1 GiB = 2^30 bytes = 1073741824
//      4 GiB = 2^32 bytes = 4294967296 (32-bit address)
// Tera 1 TiB = 2^40 bytes = 1099511627776 (new PC SSD?)
// Peta 1 PiB = 2^50 byres = 1125899906842624
// Exa  1 EiB = 2^60 bytes = 1152921504606846976
//     15 EiB = Total estimated data held by Google (May 2018)
//     16 EiB = 2^64 bytes = 18446744073709551616 (64-bit address)
// Zetta
// Yotta
//
// https://www.howtogeek.com/353116/
// how-big-are-gigabytes-terabytes-and-petabytes/
//
