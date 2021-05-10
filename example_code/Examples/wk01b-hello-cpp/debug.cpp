// $Id: debug.cpp,v 1.3 2021-04-01 03:19:29-07 - - $

#include "debug.h"

bool debug::want = false;

void debug::where (const char* file, int line, const char* func) {
   cerr << "DEBUG: " << file << "[" << line << "] " << func << endl;
   cerr << "       ";
}

