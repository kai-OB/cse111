// $Id: pos-indep-exec.cpp,v 1.1 2021-05-04 11:57:25-07 - - $

#include <iostream>
using namespace std;

int main (int argc, char** argv) {
   cout << "&argc   = " << &argc << endl;
   cout << "argv    = " << argv << endl;
   cout << "argv[0] = " << static_cast<void*> (argv[0])
        << " = \"" << argv[0] << "\"" << endl;
   cout << "new int = " << new int << endl;
}

// 
// https://en.wikipedia.org/wiki/Position-independent_code
// 
// Position-independent executables[edit]
// 
// See also: Address space layout randomization
// 
// Position-independent executables (PIE) are executable binaries
// made entirely from position-independent code.  While some
// systems only run PIC executables, there are other reasons they
// are used.  PIE binaries are used in some security-focused Linux
// distributions to allow PaX or Exec Shield to use address space
// layout randomization to prevent attackers from knowing where
// existing executable code is during a security attack using
// exploits that rely on knowing the offset of the executable
// code in the binary, such as return-to-libc attacks.
// 
