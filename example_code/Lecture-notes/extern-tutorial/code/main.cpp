// $Id: main.cpp,v 1.1 2017-10-11 14:05:24-07 - - $
// This is an example of a module accessing an external variable.

#include <stdlib.h>

#include "ext.h"

int main() {
   print_ext_var();
   ext_var = 56;
   print_ext_var();
   return EXIT_SUCCESS;
}

