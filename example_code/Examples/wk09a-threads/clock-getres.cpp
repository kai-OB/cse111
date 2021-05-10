// $Id: clock-getres.cpp,v 1.15 2021-03-05 04:02:12-08 - - $

#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
using namespace std;

// 
// CLOCK_REALTIME
//    System-wide clock that measures real (i.e.,
//    wall-clock) time.  Setting this clock requires
//    appropriate privileges.  This clock is affected by
//    discontinuous jumps in the system time (e.g., if the
//    system administrator manually changes the clock), and
//    by the incremental  adjustments performed by adjtime(3)
//    and NTP.
// 
// CLOCK_REALTIME_COARSE (since Linux 2.6.32; Linux-specific)
//    A  faster  but  less  precise version of CLOCK_REALTIME.
//    Use when you need very fast, but not fine-grained
//    timestamps.
// 
// CLOCK_MONOTONIC
//    Clock that cannot be set and represents monotonic time
//    since some unspecified starting point.  This clock  is
//    not affected by discontinuous jumps in the system time
//    (e.g., if the system  administrator  manually changes the
//    clock), but is affected by the incremental adjustments
//    performed by  adjtime(3) and NTP.
// 
// CLOCK_MONOTONIC_COARSE  (since  Linux 2.6.32; Linux-specific)
//    A faster but less precise version of  CLOCK_MONOTONIC.
//    Use  when  you  need  very fast, but not fine-grained
//    timestamps.
// 
// CLOCK_MONOTONIC_RAW (since Linux 2.6.28; Linux-specific)
//    Similar to CLOCK_MONOTONIC, but  provides  access to
//    a raw hardware-based time that is not subject to NTP
//    adjustments or the incremental adjustments performed
//    by adjtime(3).
// 
// CLOCK_BOOTTIME (since Linux 2.6.39; Linux-specific)
//    Identical  to  CLOCK_MONOTONIC,  except  it  also
//    includes any time that the system  is  suspended.  This
//    allows  applications to get a suspend-aware monotonic
//    clock without having to deal  with  the complications
//    of  CLOCK_REALTIME, which may have discontinuities if
//    the time is changed using settimeofday(2).
// 
// CLOCK_PROCESS_CPUTIME_ID
//    High-resolution per-process timer from the CPU.
// 
// CLOCK_THREAD_CPUTIME_ID
//    Thread-specific CPU-time clock.
// 

struct comma_t: numpunct<char> {
   char do_thousands_sep() const { return ','; }
   string do_grouping() const { return "\03"; }
};
locale comma (locale(), new comma_t());

void display (int clock_id, const char* clock_name) {
   cout << clock_name << ": ";
   struct timespec spec;
   int rc = clock_getres (clock_id, &spec);
   if (rc != 0) {
      cout << strerror (errno) << endl;
   }else {
      double resolution = double (spec.tv_sec) + spec.tv_nsec / 1.0e9;
      cout << resolution << " seconds" << endl;
   }
}

int main() {
   cout.imbue (comma);
   display (CLOCK_THREAD_CPUTIME_ID, "CLOCK_THREAD_CPUTIME_ID");
   display (CLOCK_PROCESS_CPUTIME_ID, "CLOCK_PROCESS_CPUTIME_ID");
   display (CLOCK_BOOTTIME, "CLOCK_BOOTTIME");
   display (CLOCK_MONOTONIC_RAW, "CLOCK_MONOTONIC_RAW");
   display (CLOCK_MONOTONIC_COARSE, "CLOCK_MONOTONIC_COARSE");
   display (CLOCK_MONOTONIC, "CLOCK_MONOTONIC");
   display (CLOCK_REALTIME_COARSE, "CLOCK_REALTIME_COARSE");
   display (CLOCK_REALTIME, "CLOCK_REALTIME");
}
