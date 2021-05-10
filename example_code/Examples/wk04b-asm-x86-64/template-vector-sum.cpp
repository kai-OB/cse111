// $Id: template-vector-sum.cpp,v 1.2 2021-05-04 15:41:21-07 - - $

#include <vector>

#include "template-sum.h"

extern std::vector<int> data;
extern int extvar;

int main() {
   extvar = sum_cont (data);
}

