// $Id: util.cpp,v 1.2 2021-04-15 23:37:21-07 - - $
//kai obrien kimobrie
#include <cstring>
using namespace std;

#include "util.h"
#ifndef __APPLE__
std::string basename(std::string const &str)
{
	auto last = str.find_last_of('/');
	return str.substr((last==std::string::npos?0:last+1), str.size());
}
string exec::execname_; // Must be initialized from main().
int exec::status_ = EXIT_SUCCESS;


void exec::execname (const string& argv0) {
   execname_ = basename (argv0.c_str());
   cout << boolalpha;
   cerr << boolalpha;
   DEBUGF ('Y', "execname = " << execname_);
}

void exec::status (int new_status) {
   new_status &= 0xFF;
   if (status_ < new_status) status_ = new_status;
}

ostream& note() {
   return cerr << exec::execname() << ": ";
}

ostream& error() {
   exec::status (EXIT_FAILURE);
   return note();
}

#endif

