// $Id: main.cpp,v 1.13 2021-02-01 18:58:18-08 - - $
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

//m.insert(xpair{key,value})

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;

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
/*
xpair <string, str_str_pair> regex_helper( string &line){
   regex comment_regex {R"(^\s*(#.*)?$)"};
   regex key_value_regex {R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
   regex trimmed_regex {R"(^\s*([^=]+?)\s*$)"};
   for (;;) {
      string line;
      getline (cin, line);
      if (cin.eof()) break;
      cout << "input: \"" << line << "\"" << endl;
      smatch result;
      if (regex_search (line, result, comment_regex)) {
         cout << "Comment or empty line." << endl;
      }else if (regex_search (line, result, key_value_regex)) {
         cout << "key  : \"" << result[1] << "\"" << endl;
         cout << "value: \"" << result[2] << "\"" << endl;
      }else if (regex_search (line, result, trimmed_regex)) {
         cout << "query: \"" << result[1] << "\"" << endl;
      }else {
         assert (false and "This can not happen.");
      }
   }
   //return 0;
  
}*/

//do a trimfile helper function
void whitespace(string *line){
   //trim leading whitespace
  
   unsigned long first = 0;//0 or 1?
   while(first<line->size() &&line->at(first) == ' '){
      line->erase(first,1);//at first position
      ++first;
   }
   int mid = 1;
   while(first<line->size()){
      if(line->at(first)=='\n'){
         line->erase(first,mid);//at first position
      }
      else{
         ++mid;
      }
      ++first;
   }
   //trims trailing whitespace
    ssize_t last = 0;//0 or 1?
   while(last>0 &&line->at(last) == ' '){
      line->erase(last,line->size()-1);//at first position
      --last;
   }
}

void catfile_helper (istream& infile, const string& filename, str_str_map test) {
   static string colons (32, ':');
   cout << colons << endl << filename << endl << colons << endl;
   regex comment_regex {R"(^\s*(#.*)?$)"};
   regex key_value_regex {R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
   regex trimmed_regex {R"(^\s*([^=]+?)\s*$)"};
   int i = 1;
   for(;;) {
      string line;
      getline (infile, line);
      whitespace(&line);//trim whitespace
      //-----regex code
      // cout << "input: \"" << line << "\"" << endl;
      if(line.length()>0){
         smatch result;
         if (regex_search (line, result, comment_regex)) {//prints twice maybe idk
            cout<<filename<<": "<<i<<": "<<line<<endl;
            //cout << "comment." << endl;
         }else if (regex_search (line, result, key_value_regex)) {
            cout<<filename<<": "<<i<<": "<<line<<endl;
            cout<< result[1]<< " = " <<result[2]<<endl;
         // cout << "key  : \"" << result[1] << "\"" << endl;
            //cout << "value: \"" << result[2] << "\"" << endl;
         }else if (regex_search (line, result, trimmed_regex)) {
            cout<<filename<<": "<<i<<": "<<line<<endl;
           // cout<< result[1]<< endl;
            //if its the key print the value
            cout<<"key";
             auto it = test.find(line);
            if(it!=test.end()){
              
               cout<< it->first<< " = " <<it->second<<endl;
            }
            else{
               cout<< result[1]<< ": " <<"key not found"<<endl;

            }

            //cout << "query: \"" << result[1] << "\"" << endl;
         }else {
            assert (false and "This can not happen.");
         }
         //-----
         
      // cout << line << endl;
         i++;
      }
      if (infile.eof()) break;
   }
}
// node* temp = new node(anchor(), anchor(), pair);

int main (int argc, char** argv) {
   sys_info::execname (argv[0]);
   scan_options (argc, argv);
   str_str_map test;//listmap
//-------------------------matchlines
const string cin_name = "-";
int status = 0;
   string progname ( (argv[0]));
   vector<string> filenames (&argv[1], &argv[argc]);
   if (filenames.size() == 0) filenames.push_back (cin_name);
   for (const auto& filename: filenames) {
      if (filename == cin_name) catfile_helper (cin, filename,test);
      else {
         ifstream infile (filename);
         if (infile.fail()) {
            status = 1;
            cerr << progname << ": " << filename << ": "
                 << strerror (errno) << endl;
         }else {
           
             catfile_helper (infile, filename,test);
         
            infile.close();
         }
      }
   }
   return status;
  // cout << "EXIT_SUCCESS" << endl;
  // return EXIT_SUCCESS;
}

