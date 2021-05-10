// $Id: code-vector-sum.cpp,v 1.1 2021-05-04 15:37:41-07 - - $

#include <vector>

int sum_vector (const std::vector<int>& data) {
   int sum = 0;
   for (int n: data) sum += n;
   return sum;
}

