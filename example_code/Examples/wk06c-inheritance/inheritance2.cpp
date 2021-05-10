// $Id: inheritance2.cpp,v 1.19 2016-07-21 19:46:50-07 - - $

//
// Example using objects, with a base object and two derived objects.
// Similar to inheritance2, but uses gcc demangler.
//

#include <iostream>
#include <memory>
#include <typeinfo>
#include <vector>
using namespace std;

#define LOG cout << "[" << __LINE__ << "]" \
                 << __PRETTY_FUNCTION__ << ": "

#include <cxxabi.h>
template <typename type>
string demangle_typeid (const type& object) {
   const char* name = typeid(object).name();
   int status = 0;
   using deleter = void (*) (void*);
   unique_ptr<char,deleter> result {
      abi::__cxa_demangle (name, nullptr, nullptr, &status),
      std::free,
   };
   return status == 0 ? result.get() : name;
}


/////////////////////////////////////////////////////////////////
// class object
/////////////////////////////////////////////////////////////////

class object {
   private:
      object (const object&) = delete;
      object& operator= (const object&) = delete;
      object (object&&) = delete;
      object& operator= (object&&) = delete;
      static unsigned next_id;;
   protected:
      const unsigned id;
      object(); // abstract class, so only derived can used ctor.
   public:
      virtual ~object(); // must be virtual
      virtual void print (ostream&) const;
};

ostream& operator<< (ostream& out, const object& obj) {
   obj.print (out);
   return out;
}

unsigned object::next_id = 0;

object::object(): id(++next_id) {
   LOG << *this << endl;
}

object::~object() {
   LOG << *this << endl;
}

void object::print (ostream& out) const {
   out << "[" << static_cast<const void *const> (this) << "->"
       << demangle_typeid(*this) << "] id=" << id << ": ";
}


/////////////////////////////////////////////////////////////////
// class square 
/////////////////////////////////////////////////////////////////

class square: public object {
   private:
      size_t width;
   public:
      explicit square (size_t width = 0);
      virtual ~square();
      virtual void print (ostream&) const;
};

square::square (size_t width): width(width) {
   LOG << *this << endl;
}

square::~square() {
   LOG << *this << endl;
}

void square::print (ostream& out) const {
   this->object::print (out);
   out << "width=" << width;
}

/////////////////////////////////////////////////////////////////
// class circle 
/////////////////////////////////////////////////////////////////

class circle: public object {
   private:
      size_t diam;
   public:
      explicit circle (size_t diam = 0);
      virtual ~circle();
      virtual void print (ostream&) const;
};

circle::circle (size_t diam): diam(diam) {
   LOG << *this << endl;
}

circle::~circle() {
   LOG << *this << endl;
}

void circle::print (ostream& out) const {
   this->object::print (out);
   out << "diam=" << diam;
}



/////////////////////////////////////////////////////////////////
// main
/////////////////////////////////////////////////////////////////

int main() {
   LOG << "sizeof (object) = " << sizeof (object) << endl;
   LOG << "sizeof (square) = " << sizeof (square) << endl;
   LOG << "sizeof (circle) = " << sizeof (circle) << endl;

   vector<shared_ptr<object>> vec;
   // ERROR: v.push_back (new object());
   // ERROR: object o;
   vec.push_back (make_shared<circle> (  ));
   vec.push_back (make_shared<circle> (10));
   vec.push_back (make_shared<square> (  ));
   vec.push_back (make_shared<square> ( 5));
   vec.push_back (make_shared<square> ( 8));
   cout << endl;

   for (const auto& ptr: vec) {
      LOG << "Object: " << *ptr << endl;
   }
   cout << endl;

   LOG << "return 0" << endl;
   return 0;
}

/*
//TEST// valgrind --leak-check=full --show-reachable=yes \
//TEST//       inheritance2 >inheritance2.out 2>&1
//TEST// mkpspdf inheritance2.ps inheritance2.cpp* inheritance2.out*
*/

