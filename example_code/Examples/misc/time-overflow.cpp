// $Id: time-overflow.cpp,v 1.10 2021-03-03 00:44:19-08 - - $

#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>
using namespace std;

struct comma_t: numpunct<char> {
   char do_thousands_sep() const { return ','; }
   string do_grouping() const { return "\03"; }
};
locale comma (locale(), new comma_t());

void print_time (time_t seconds, const string& comment) {
   cout.imbue (comma);
   cout << setw(25) << seconds << " seconds " << comment << endl;
   time_t days = round (double (seconds) / (24 * 60 * 60));
   cout << setw(25) << days << " days " << comment << endl;
   time_t years = round (days / 365.2425);
   cout << setw(25) << years << " years " << comment << endl;
}

int main() {
   time_t now = time (nullptr);
   print_time (now, "since the epoch");
   time_t max_sec = numeric_limits<time_t>::max();
   print_time (max_sec - now, "until time overflow");
   return 0;
}
