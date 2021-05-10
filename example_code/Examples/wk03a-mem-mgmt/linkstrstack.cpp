// $Id: linkstrstack.cpp,v 1.26 2021-04-29 17:18:37-07 - - $

//
// linkstrstack - show the linked list implementation of a stack
// not a template class.  poor locality of reference.
//

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

/////////////////////////////////////////////////////////////////
// linkstrstack.h
/////////////////////////////////////////////////////////////////

class linkstrstack {
   private:
      struct node {
         string item;
         node* link;
         node (const string& itm, node* lnk): item(itm), link(lnk) {};
      };
      node* topnode = nullptr;
      size_t count = 0;
      void delete_this();
      void copy_from (const linkstrstack&);
      void move_from (linkstrstack&&);
   public:
      linkstrstack() = default;
      ~linkstrstack() { delete_this(); }
      linkstrstack (const linkstrstack&);
      linkstrstack& operator= (const linkstrstack&);
      linkstrstack (linkstrstack&&);
      linkstrstack& operator= (linkstrstack&&);
      void push (const string& );
      void pop();
      string& top() { return topnode->item; }
      const string& top() const { return topnode->item; }
      size_t size() const { return count; }
      bool empty() const { return size() == 0; }
      operator bool() const { return not empty(); }
};


/////////////////////////////////////////////////////////////////
// linkstrstack.cpp
/////////////////////////////////////////////////////////////////

void linkstrstack::delete_this() {
   while (not empty()) pop();
}

void linkstrstack::copy_from (const linkstrstack& that) {
   node** thisptr = &topnode;
   for (node* ptr = that.topnode; ptr != nullptr; ptr = ptr->link) {
      *thisptr = new node (ptr->item, nullptr);
      thisptr = &((*thisptr)->link);
   }
   count = that.count;
}

void linkstrstack::move_from (linkstrstack&& that) {
   topnode = that.topnode;
   count = that.count;
   that.topnode = nullptr;
   that.count = 0;
}

linkstrstack::linkstrstack (const linkstrstack& that) {
   copy_from (that);
}

linkstrstack& linkstrstack::operator= (const linkstrstack& that) {
   if (this == &that) return *this;
   delete_this();
   copy_from (that);
   return *this;
}

linkstrstack::linkstrstack (linkstrstack&& that) {
   move_from (std::move (that));
}

linkstrstack& linkstrstack::operator= (linkstrstack&& that) {
   if (this == &that) return *this;
   delete_this();
   move_from (std::move (that));
   return *this;
}


void linkstrstack::push (const string& item) {
   topnode = new node (item, topnode);
   ++count;
}

void linkstrstack::pop() {
   node* tmp = topnode;
   topnode = topnode->link;
   delete tmp;
   --count;
}


/////////////////////////////////////////////////////////////////
// main.cpp
/////////////////////////////////////////////////////////////////

int main (int argc, char** argv) {

   linkstrstack stkstr;
   for (int argi = 1; argi < argc; ++argi) {
      stkstr.push (argv[argi]);
   }
   while (stkstr.size() > size_t (argc / 2)) {
      cout << "loop 1: " << stkstr.top() << endl;
      stkstr.pop();
   }

   linkstrstack newstack = std::move (stkstr);
   while (not stkstr.empty()) {
      cout << "loop stkstr: " << stkstr.top() << endl;
      stkstr.pop();
   }

   linkstrstack other = newstack;
   while (newstack) {
      cout << "loop newstack: " << newstack.top() << endl;
      newstack.pop();
   }

   return 0;
}

/*
//TEST// valgrind --leak-check=full --show-reachable=yes \
//TEST//       linkstrstack this is some test data for the stack \
//TEST//       >linkstrstack.out 2>&1
//TEST// mkpspdf linkstrstack.ps linkstrstack.cpp* linkstrstack.out*
*/

