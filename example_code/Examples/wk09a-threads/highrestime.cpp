// $Id: highrestime.cpp,v 1.6 2021-03-04 16:17:09-08 - - $

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

#include <chrono>
#include <cstring>
#include <iomanip>
#include <iostream>
using namespace std;

#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using high_res_clock = chrono::high_resolution_clock;

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
   struct rusage child_usage;
   high_res_clock::time_point start = high_res_clock::now();
   int status = runprog (&argv[1]);
   getrusage (RUSAGE_CHILDREN, &child_usage);
   high_res_clock::time_point finish = high_res_clock::now();
   print_usage (chrono::duration<double> (finish - start).count(),
                seconds (child_usage.ru_stime),
                seconds (child_usage.ru_utime));
   status = status & 0x7F ? (status & 0x7F) | 0x80
                          : (status >> 8) & 0xFF;
   if (status) cout << " $?=" << status;
   cout << endl;
   return status;
}

