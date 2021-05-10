// $Id: code-list-sum.cpp,v 1.1 2021-05-04 15:37:41-07 - - $

#include <list>

int sum_list (const std::list<int>& data) {
   int sum = 0;
   for (int n: data) sum += n;
   return sum;
}

