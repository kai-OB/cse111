// $Id: semaphilo.cpp,v 1.37 2020-12-05 19:25:58-08 - - $

//
// Implementation of semaphores.
// Solution to Dining Philosophers problem.
//

#include <condition_variable>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <random>
#include <string>
#include <thread>
using namespace std;

//
// Timer.
//
using clock_type = chrono::high_resolution_clock;
struct timer_type {
   static const clock_type::time_point start;
   double elapsed_seconds() {
      clock_type::time_point now = clock_type::now();
      return chrono::duration_cast<chrono::nanoseconds> (now - start)
             .count() / 1e9;
   }
} timer;
const clock_type::time_point timer_type::start {clock_type::now()};


//
// Printer for synchronized output accepts a variable number
// of arguments.
//

class synch_printer {
   private:
      mutex out_mutex;
      ostream& out;
      void print_();
      template <typename Head, typename... Tail>
      void print_ (const Head& head, Tail... tail);
   public:
      synch_printer (ostream& out): out(out){}
      template <typename... Type>
      void print (Type... params);
};

void synch_printer::print_() {
}

template <typename Head, typename... Tail>
void synch_printer::print_ (const Head& head, Tail... tail) {
   out << head;
   print_ (tail...);
}

template <typename... Type>
void synch_printer::print (Type... params) {
   unique_lock<mutex> lock {out_mutex};
   out << setw(9) << setprecision(6) << fixed
       << timer.elapsed_seconds() << " ";
   print_ (params...);
   out << endl << flush;
}


//
// class semaphore
//    count > 0: number to be let through the gate
//    count = 0: gate is locked, nobody waiting
//    count < 0: gate locked with -count threads waiting
// semaphore lock(1) is a mutex
//

class semaphore {
   private:
      mutex lock;
      condition_variable cond;
      int count{};
   public:
      explicit semaphore (int count = 0);
      semaphore (const semaphore&) = delete; // prevent copying.
      semaphore& operator= (const semaphore&) = delete;
      void down(); // Dijkstra's P ("proberen", Dutch "try")
      void up();   // Dijkstra's V ("verhogen", Dutch "raise")
};

semaphore::semaphore (int count): count(count) {
}

void semaphore::down() {
   unique_lock<mutex> ulock {lock};
   --count;
   if (count < 0) cond.wait (ulock);
}

void semaphore::up() {
   unique_lock<mutex> ulock {lock};
   if (count < 0) cond.notify_one();
   ++count;
}


class fork_manager {
   private:
      enum class fork_state {THINKING = 0, HUNGRY, EATING};
      semaphore lock {1};
      const size_t size;
      vector<semaphore> waiting;
      vector<fork_state> state;
   private:
      size_t left (size_t id) { return (id + size - 1) % size; }
      size_t right (size_t id) { return (id + 1) % size; }
      void check (size_t id);
   public:
      fork_manager (size_t size);
      void take (size_t id);
      void put (size_t id);
};

fork_manager::fork_manager (size_t size): size(size),
              waiting (size), state(size, fork_state::THINKING) {
}

void fork_manager::check (size_t id) {
   if (state[id] == fork_state::HUNGRY
       and state[left(id)] != fork_state::EATING
       and state[right(id)] != fork_state::EATING) {
      state[id] = fork_state::EATING;
      waiting[id].up();
   }
}

void fork_manager::take (size_t id) {
   lock.down();
   state[id] = fork_state::HUNGRY;
   check (id);
   lock.up();
   waiting[id].down();
}

void fork_manager::put (size_t id) {
   lock.down();
   state[id] = fork_state::THINKING;
   check (left (id));
   check (right (id));
   lock.up();
}


//
// Code to simulate one Dining Philosopher.
//

using normal_dist = normal_distribution<double>;
using rand_engine = default_random_engine;
using rand_gen = decltype (bind (normal_dist{}, rand_engine{}));

rand_gen make_rand (double mean, double stdev) {
   auto seed = chrono::system_clock::now().time_since_epoch().count();
   rand_engine engine {seed};
   normal_dist distribution (mean, stdev);
   rand_gen rand = bind (distribution, engine);
   return rand;
}

void perform (synch_printer& mcout, rand_gen& rand, size_t cycle,
              const string& ident, const string& activity) {
   long delay = rand();
   double delay_sec = delay / 1e3;
   mcout.print (ident, activity, " ", cycle,
                " (", setprecision (3), delay_sec, ")");
   this_thread::sleep_for (chrono::milliseconds (delay));
}

void philosopher (size_t id, const string& name,
                  size_t cycles, double mean, double stdev,
                  fork_manager& forks, synch_printer& mcout) {
   rand_gen rand = make_rand (mean, stdev);
   string ident = name + " " + to_string(id) + " is ";
   mcout.print (ident, "STARTING");
   for (size_t cycle = 0; cycle < cycles; ++cycle) {
      perform (mcout, rand, cycle, ident, "thinking");
      mcout.print (ident, "hungry ", cycle);
      forks.take (id);
      perform (mcout, rand, cycle, ident, "eating");
      forks.put (id);
   }
   mcout.print (ident, "FINISHED");
}


//
// Host at the Symposium.
//
// In ancient Greece, the symposium (Greek sumponein, "to drink
// together") was a drinking party.  Literary works that describe
// or take place at a symposium include two Socratic dialogues,
// Plato's Symposium and Xenophon's Symposium, as well as a number
// of Greek poems such as the elegies of Theognis of Megara.
//

int main() {
   synch_printer mcout {cout};
   mcout.print ("Symposium STARTING");
   vector<string> names = {
      "Pythagoras",
      "Socrates",
      "Plato",
      "Aristotle",
      "Zeno",
   };
   fork_manager forks {names.size()};
   vector<thread> philos;
   for (size_t id = 0; id < names.size(); ++id) {
      philos.push_back (thread (philosopher, id, ref (names[id]),
                                5, 1000, 250,
                                ref (forks), ref (mcout)));
   }
   for (auto& phil: philos) phil.join();
   mcout.print ("Symposium FINISHED");
   return 0;
}

//TEST// semaphilo >semaphilo.out 2>&1
//TEST// mkpspdf semaphilo.ps semaphilo.cpp* semaphilo.out

