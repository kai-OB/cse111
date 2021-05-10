// $Id: ctype.cpp,v 1.14 2021-03-29 17:42:12-07 - - $

// Print out all characters with their <ctype> classes.

#include <cctype>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

using isclass = int (*) (int);
vector<pair<isclass,string>> functions = {
   {isalnum , "isalnum" },
   {isalpha , "isalpha" },
   {isblank , "isblank" },
   {iscntrl , "iscntrl" },
   {isdigit , "isdigit" },
   {isgraph , "isgraph" },
   {islower , "islower" },
   {isprint , "isprint" },
   {ispunct , "ispunct" },
   {isspace , "isspace" },
   {isupper , "isupper" },
   {isxdigit, "isxdigit"},
};

unordered_map<int,string> charnames = {
   {0x00, "NUL"}, {0x01, "SOH"}, {0x02, "STX"}, {0x03, "ETX"},
   {0x04, "EOT"}, {0x05, "ENQ"}, {0x06, "ACK"}, {0x07, "BEL"},
   {0x08, "BS "}, {0x09, "HT "}, {0x0A, "LF "}, {0x0B, "VT "},
   {0x0C, "FF "}, {0x0D, "CR "}, {0x0E, "SO "}, {0x0F, "SI "},
   {0x10, "DLE"}, {0x11, "DC1"}, {0x12, "DC2"}, {0x13, "DC3"},
   {0x14, "DC4"}, {0x15, "NAK"}, {0x16, "SYN"}, {0x17, "ETB"},
   {0x18, "CAN"}, {0x19, "EM "}, {0x1A, "SUB"}, {0x1B, "ESC"},
   {0x1C, "FS "}, {0x1D, "GS "}, {0x1E, "RS "}, {0x1F, "US "},
   {0x7F, "DEL"},
};

int main() {
   for (unsigned ch = 0; ch < 0x80; ++ch) {
      cout << dec << setfill(' ') << setw(3) << ch
           << " 0" << oct << setfill('0') << setw(3) << ch
           << " 0x" << hex << uppercase << setfill('0')
           << setw(2) << ch;
      if (isprint (ch)) cout << " '" << char (ch) << "'";
      else {
         auto chname = charnames.find (ch);
         if (chname == charnames.end()) cout << " ...";
                                   else cout << " " << chname->second;
      }
      for (const auto& fn: functions) {
         if (fn.first (ch)) cout << " " << fn.second;
      }
      cout << endl;
   }
}

