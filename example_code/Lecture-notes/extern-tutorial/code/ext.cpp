// $Id: ext.cpp,v 1.1 2017-10-11 14:05:24-07 - - $
// This is an example of a module exporting an external variable.

#include <stdio.h>

#include "ext.h"

int ext_var = 44;

void print_ext_var() {
   printf ("ext_var = %d\n", ext_var);
}

