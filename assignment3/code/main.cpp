// $Id: main.cpp,v 1.25 2021-05-23 00:33:51-07 - - $
//Kai O'Brien (kimobrie@ucsc.edu)

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <unistd.h>
//--------
#include <cassert>
#include <cerrno>
#include <fstream>
#include <iomanip>
#include <regex>
#include <stdexcept>
#include <typeinfo>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;
str_str_map test;//listmap

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            debugflags::setflags (optarg);
            break;
         default:
            complain() << "-" << char (optopt) << ": invalid option"
                       << endl;
            break;
      }
   }
}

void whitespace(string *line){
   //trim leading whitespace and returns position of =sign or -1
   unsigned long first = 0;
   while(first<line->size() &&line->at(first) == ' '){
      line->erase(first,1);//at first position
      if(line->at(first)=='='){
        
      }
      ++first;
   }
   int mid = 1;
   while(first<line->size()){
      if(line->at(first)=='='){
      }
      if(line->at(first)=='\n'){
         line->erase(first,mid);//at first position
      }
      else{
         ++mid;
      }
      ++first;
   }
   //trims trailing whitespace
    ssize_t last = line->size()-1;
   while(last>0 &&line->at(last) == ' '){
      if(line->at(last)=='='){
      }
      
      line->erase(last,line->size()-1);//at first position
      --last;
   }
}

size_t eq_pos(string *line){
   size_t eq = 1234;
   size_t  first =  0;
   while(first<line->size()){
      if(line->at(first) == '='){
         eq = first;
         break;
      }
      ++first;
   }
   return eq;
}

//insert stuff to map when key = value not found
void catfile_helper (istream& infile, const string& filename) {
   static string colons (32, ':');
 //  cout << colons << endl << filename << endl << colons << endl;
   regex comment_regex {R"(^\s*(#.*)?$)"};
   regex key_value_regex {R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
   regex trimmed_regex {R"(^\s*([^=]+?)\s*$)"};
   int i = 1;
   for(;;) {
      string line;
      getline (infile, line);
      whitespace(&line);//trim whitespace
    
     //cout << "input: \"" << line << "\"" << endl;
      if(line.length()>0){
         
         smatch result;
         if (regex_search (line, result, comment_regex)) {
            cout<<filename<<": "<<i<<": "<<line<<endl;
           
         }
        
         else if (regex_search (line, result, key_value_regex)) {
            cout<<filename<<": "<<i<<": "<<line<<endl;
            //= 
           if(line.at(0)=='='){
              //if the = is the only thing
              if(line.size()==1){
                  for (auto itor = test.begin(); 
                     itor != test.end(); ++itor) {
                     cout<< itor->first<< " = " <<itor->second<<endl;
                  }
              }
              else{  //=value              
                  for (auto itor = test.begin(); 
                     itor != test.end(); ++itor) {
                      if(itor->second == result[2]){
                        cout<< itor->first<< " = " <<itor->second<<endl;
                      }
                  }
              }
           }
            else if(line.at(line.size()-1)=='='){ //key = 
               if(test.find(result[1])!=test.end() ){
                  test.erase(test.find(result[1]));//valgrind
               }
               else{
                  cout<< result[1]<< ": " <<"key not found"<<endl;
               }
            }
            else{//key = value
               test.insert(str_str_pair(result[1],result[2]));
               cout<<result[1]<< " = " <<result[2]<<endl;
            }
         }
         //  key = , =, or =value
         else if (regex_search (line, result, trimmed_regex)) {
            cout<<filename<<": "<<i<<": "<<line<<endl;
            size_t eq_pos1 = eq_pos(&line);
            //key 
            if(eq_pos1==1234){
               auto it = test.find(result[1]);
               if(test.find(result[1])!=test.end()){
                  cout<< it->first<< " = " <<it->second<<endl;
               }
               else{
                  cout<< result[1]<< ": " <<"key not found"<<endl;
               }
            }
         }else {
            assert (false and "This can not happen.");
         }         
         i++;
      }
      if (infile.eof()) break;
   }
}

int main (int argc, char** argv) {
   sys_info::execname (argv[0]);
   scan_options (argc, argv);
//-------------------------matchlines
const string cin_name = "-";
int status = 0;
   string progname ( (argv[0]));
   vector<string> filenames (&argv[1], &argv[argc]);
   if (filenames.size() == 0) filenames.push_back (cin_name);
   for (const auto& filename: filenames) {
      if (filename == cin_name) catfile_helper (cin, filename);
      else {
         ifstream infile (filename);
         if (infile.fail()) {
            status = 1;
            cerr << progname << ": " << filename << ": "
                 << strerror (errno) << endl;
         }else {
            catfile_helper (infile, filename);
            infile.close();
         }
      }
   }
   return status;
  // cout << "EXIT_SUCCESS" << endl;
  // return EXIT_SUCCESS;
}

