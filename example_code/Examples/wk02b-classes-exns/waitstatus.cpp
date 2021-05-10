// $Id: waitstatus.cpp,v 1.19 2021-02-27 16:11:35-08 - - $

#include <cstring>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include <sys/wait.h>

void print_status (int status) {
   status &= 0xFFFF;
   cout << "0x" << setfill('0') << hex << uppercase << setw(4) << status
        << setfill(' ') << dec << setw(6) << status << ":";
   if (WIFEXITED (status))  {
      cout << " Exit " << WEXITSTATUS (status);
   }
   if (WIFSIGNALED (status)) {
      cout << " Signal " << WTERMSIG (status)
           << " " << strsignal (status);
      #ifdef WCOREDUMP
      if (WCOREDUMP (status)) cout << " (core dumped)";
      #endif
   }
   if (WIFSTOPPED (status)) {
      cout << " Stopped " << WSTOPSIG (status);
   }
   if (WIFCONTINUED (status)) {
      cout << " Continued";
   }
   cout << endl;
}

// #define __WIFSTOPPED(status) (((status) & 0xff) == 0x7f)
// #define __W_CONTINUED 0xffff

int main() {
   vector<int> status {0<<8, 1<<8, 2<<8, -1, -2, 0x80, 0x81,
                       0xFFFF, 0xFFFE, 0x7F, 0x0301, 0x0502};
   for (int sig = 1; sig <= 31; ++sig) status.push_back (sig);
   for (int sig: status) print_status (sig);
   return EXIT_SUCCESS;
}

