// $Id: hello.cpp,v 1.7 2021-04-01 03:19:29-07 - - $

#include <iostream>
#include <string>
using namespace std;

#include "hello.h"
#include "debug.h"

#define DEBUG_HELLO DEBUG (this << "->\"" <<  message << "\"");

hello::hello() {
   DEBUG_HELLO;
}

hello::~hello() {
   DEBUG_HELLO;
}

hello::hello (const string& message_): message(message_) {
   DEBUG_HELLO;
}

void hello::say (ostream& out) const {
   out << message << endl;
}

