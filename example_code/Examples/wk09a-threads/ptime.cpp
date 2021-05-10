// $Id: ptime.cpp,v 1.8 2021-03-04 16:17:05-08 - - $

//
// NAME
//    proctime - print time used by process
//
// SYNOPSIS
//    proctime prog [args...]
//
// DESCRIPTION
//    Runs the program as a child process and when finished
//    prints the real_time, sys_time, and user_time used.
//

#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
using namespace std;

#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

string execname;

void sysfail (const string& object) {
   cerr << execname << ": " << object << ": " << strerror (errno)
        << endl;
   exit (errno | 0x80);
}

void usage() {
   cerr << "Usage: " << execname << " prog [args...]" << endl;
   exit (1);
}

double seconds (const struct timeval& time) {
   return double (time.tv_sec) + double (time.tv_usec) / 1.0e6;
}

int runprog (char** argv) {
   int pid = fork();
   if (pid < 0) sysfail ("fork()");
   if (pid == 0) {
      int exec = execvp (argv[0], argv);
      if (exec < 0) sysfail (argv[0]);
   }
   int status;
   waitpid (pid, &status, 0);
   return status;
}

void print_usage (double real_time, double sys_time, double user_time) {
  cout << std::fixed
       << setprecision(6) << real_time << " real, "
       << setprecision(6) << sys_time << " sys, "
       << setprecision(6) << user_time << " user.";
}

int main (int argc, char** argv) {
   execname = basename (argv[0]);
   if (argc < 2) usage();
   struct rusage cnild_usage;
   struct timeval start, finish;
   gettimeofday (&start, nullptr);
   int status = runprog (&argv[1]);
   getrusage (RUSAGE_CHILDREN, &cnild_usage);
   gettimeofday (&finish, nullptr);
   print_usage (seconds (finish) - seconds (start),
                seconds (cnild_usage.ru_stime),
                seconds (cnild_usage.ru_utime));
   status = status & 0x7F ? (status & 0x7F) | 0x80
                          : (status >> 8) & 0xFF;
   if (status) cout << " $?=" << status;
   cout << endl;
   return status;
}

