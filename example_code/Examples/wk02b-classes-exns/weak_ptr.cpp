#include <iostream>
#include <memory>
#include <string>
using namespace std;

int main() {
   auto sp = make_shared<string> ("hello");
   cout << sp << " " << *sp << endl;
   weak_ptr<string> wp = sp;
   cout << wp.lock() << " " << *wp.lock() << endl;
   cout << "sizeof (shared_ptr) = " << sizeof sp << endl;
   cout << "sizeof (weak_ptr) = " << sizeof wp << endl;
}
