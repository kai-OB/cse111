// $Id: debug.h,v 1.5 2021-04-01 03:20:53-07 - - $

#ifndef __DEBUG__
#define __DEBUG__

#include <iostream>
using namespace std;

struct debug {
   static bool want;
   static void where (const char*, int, const char*);
};

#define DEBUG(CODE) { \
           if (debug::want) { \
              debug::where (__FILE__, __LINE__, __PRETTY_FUNCTION__); \
              cerr << CODE << endl; \
           } \
        }

#endif

