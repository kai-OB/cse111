// $Id: template_string.cpp,v 1.5 2020-11-13 23:22:28-08 - - $

#include <iostream>
using namespace std;

template <typename char_type>
struct template_string {

   struct long_str {
      size_t capacity_;
      size_t size_;
      char_type* data_;
   };

   static constexpr size_t CHAR_COUNT = (sizeof (long_str) - 1)
                                       / sizeof (char_type);
   static constexpr size_t MIN_CAP = CHAR_COUNT > 2 ? CHAR_COUNT : 2;

   struct short_str {
      union {
         unsigned char size_;
         char_type padding_;
      };
      char_type data_[MIN_CAP];
   };

   union str_rep {
      long_str long_;
      short_str short_;
   } rep_;

   bool is_long_str() const {
      return rep_.short_.size_ & 1;
   }

};


int offset (const void* base, const void* field) {
   return static_cast<const char*> (field)
        - static_cast<const char*> (base);
}

#define SHOW(X) cout << #X << " = " << X << endl;
template <typename char_type_t>
void print_info (const string& char_type) {
   template_string<char_type_t> v;
   SHOW (char_type);
   SHOW (sizeof (char_type_t));
   SHOW (sizeof v);
   SHOW (sizeof v.rep_);
   SHOW (sizeof v.rep_.long_);
   SHOW (sizeof v.rep_.long_.capacity_);
   SHOW (sizeof v.rep_.long_.size_);
   SHOW (sizeof v.rep_.long_.data_);
   SHOW (sizeof v.rep_.short_);
   SHOW (sizeof v.rep_.short_.size_);
   SHOW (sizeof v.rep_.short_.padding_);
   SHOW (sizeof v.rep_.short_.data_);
   SHOW (v.CHAR_COUNT);
   SHOW (v.MIN_CAP);
   SHOW (offset (&v, &v));
   SHOW (offset (&v, &v.rep_));
   SHOW (offset (&v, &v.rep_.long_));
   SHOW (offset (&v, &v.rep_.long_.capacity_));
   SHOW (offset (&v, &v.rep_.long_.size_));
   SHOW (offset (&v, &v.rep_.long_.data_));
   SHOW (offset (&v, &v.rep_.short_));
   SHOW (offset (&v, &v.rep_.short_.size_));
   SHOW (offset (&v, &v.rep_.short_.padding_));
   SHOW (offset (&v, &v.rep_.short_.data_));
}

#define PRINT(C) print_info<C>(#C);
int main() {
   PRINT(char);
   cout << "\f" << endl;
   PRINT(wchar_t);
   cout << "\f" << endl;
   PRINT(char16_t);
   cout << "\f" << endl;
   PRINT(char32_t);
}

/*
//TEST// ./template_string >template_string.out
//TEST// mkpspdf template_string.ps template_string.cpp* \
//TEST//         template_string.out
*/

