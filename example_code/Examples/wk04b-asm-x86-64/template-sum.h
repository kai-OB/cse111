// $Id: template-sum.h,v 1.1 2021-05-04 15:27:00-07 - - $

template <typename container>
int sum_cont (const container& cont) {
   int sum = 0;
   for (int n: cont) sum += n;
   return sum;
}

