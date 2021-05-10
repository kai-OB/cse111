// $Id: template-list-sum.cpp,v 1.2 2021-05-04 15:41:21-07 - - $

#include <list>

#include "template-sum.h"

extern std::list<int> data;
extern int extvar;

int main() {
   extvar = sum_cont (data);
}

