// $Id: dynamic_cast.cpp,v 1.3 2021-04-22 18:41:13-07 - - $

// Example showing a dynamic cast.

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class base {
   public:
      virtual string name() const = 0;
      virtual string type() const = 0;
      virtual ~base() {}
};

class plain: public base {
   private:
      string name_ = "plain anonymous";
   public:
      plain() = default;
      plain (const string& name__): name_(name__) {}
      string name() const override { return name_; }
      string type() const override { return "plain"; }
};

class special: public base {
   private:
      string name_ = "special anonymous";
      string rank_ = "special rank";
   public:
      special() = default;
      special (const string& name__): name_(name__) {}
      string name() const override { return name_ + ": " + rank_; }
      string type() const override { return "special"; }
      void set_rank (const string& rank__) { rank_ = rank__; }
};

void display (base* item) {
   cout << item->type() << ": " << item->name() << endl;
}

int main() {
   vector<base*> data;
   data.push_back (new plain ("hello"));
   {
      special* next = new special ("world");
      next->set_rank ("boss");
      data.push_back (next);
   }
   cout << endl << "first loop:" << endl;
   for (auto itor: data) display (itor);
   for (auto itor: data) {
      special* item = dynamic_cast<special*> (itor);
      if (item == nullptr) {
         cout << "ERROR: " << itor->name() << " is not special" << endl;
      }else {
         cout << itor->name() << " changing rank to foobar" << endl;
         item->set_rank ("foobar");
      }
   }
   cout << endl << "second loop:" << endl;
   for (auto itor: data) display (itor);
}
