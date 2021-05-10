// $Id: hello.h,v 1.4 2021-03-31 19:56:58-07 - - $

#ifndef __HELLO_H__
#define __HELLO_H__

#include <iostream>
#include <string>

class hello {
   private:
      std::string message {"Hello, World!"};
   public:
      hello();                         // default constructor
      hello (const std::string&);      // alternate ctor
      ~hello();                        // destructor
      void say (std::ostream&) const;  // member function
};

#endif

