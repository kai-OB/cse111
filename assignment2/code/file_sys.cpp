// $Id: file_sys.cpp,v 1.9 2020-10-26 21:32:08-07 - - $

#include <cassert>
#include <iostream>
#include <stdexcept>

using namespace std;

#include "debug.h"
#include "file_sys.h"

size_t inode::next_inode_nr {1}; //initialized to 1, the first inode number

ostream& operator<< (ostream& out, file_type type) {
   switch (type) {
      case file_type::PLAIN_TYPE: out << "PLAIN_TYPE"; break;
      case file_type::DIRECTORY_TYPE: out << "DIRECTORY_TYPE"; break;
      default: assert (false);
   };
   return out;
}
     
     


inode_state::inode_state() {
   DEBUGF ('i', "root = " << root << ", cwd = " << cwd
          << ", prompt = \"" << prompt() << "\"");

   //inode state constructor
   //establish inode state
   //create root directory /
   //make sure root directory (parent ..) points at itseld
   //can call inode constructor and pass in a filetype /
   //then modify it after the fact?
   //can call the make shared plain file and directory/?
   //i node and inode state are friends
   //which means once you have an inode the inode state can go in and zap 
   //the fields in appropriate manners
   root = make_shared <inode> (file_type::DIRECTORY_TYPE);  //right?
   shared_ptr <directory> root_dir = dynamic_pointer_cast<directory>
                                    (root->get_contents());
   cwd = root;

   root->mkdir(root);
  // parent("."root);  //cal inside mkdir? or here?
  // parent("..",root);
   
}
//inode_state method implementations
void inode_state::prompt(const string& s){
      prompt_  = s;
}  //implement later?
      //sets the prompts

inode_ptr inode_state::get_root(){
   return root;   //state.root? or just root?
}
inode_ptr inode_state::get_cwd(){
   return cwd;
}
void inode_state::set_cwd(inode_ptr new_cwd){
   cwd = new_cwd;
}
void inode_state::set_root(inode_ptr new_root){
   root = new_root;
}    
   

const string& inode_state::prompt() const { return prompt_; }
//just returns the prompt

/////////// inode_state destructor////////  

inode_state::~inode_state(){
   shared_ptr <directory> root_dir = dynamic_pointer_cast<directory>
                                    (root->get_contents());
   clear_dir(root_dir);  //call erase files on all files then root will be delete
   root = nullptr;        //maybe write  inside the deconstructor idk
   cwd = nullptr;

}

ostream& operator<< (ostream& out, const inode_state& state) {
   //just prints out inode state
   //just used in debug statements in working code
   out << "inode_state: root = " << state.root//machine adresses
       << ", cwd = " << state.cwd;
   return out;
}

inode::inode(file_type type): inode_nr (next_inode_nr++) {
   //constructor
   //need a virtual constructor but no such thing in c++
   //so instead pass in anargument
   //default constructor on an inode has been supressed
   //so just say new node and give it the particular filetype that you want to 
   //create
   //depends on the command it is being called from
   fileType = type;
   switch (type) {
      case file_type::PLAIN_TYPE:
           contents = make_shared<plain_file>();
           break;
      case file_type::DIRECTORY_TYPE:
           contents = make_shared<directory>(); //make shared of a plain or directory
               //adjust the file sysem
               //making filesystem friends
               //inode and inode state are already friends
               //base file probably doesnt work well without inode
               //information hiding is not important?? idk
           break;
      default: assert (false);   //for the sake of clarity
   }
   DEBUGF ('i', "inode " << inode_nr << ", type = " << type);
}

size_t inode::get_inode_nr() const {

   //just gets inode number, already done
   DEBUGF ('i', "inode = " << inode_nr);
   return inode_nr;
}
void inode::set_contents(base_file_ptr new_contents){

   contents = new_contents;
}   //setter
base_file_ptr inode::get_contents(){
   return contents;
} //getter

size_t inode::get_next_inode_nr(){  //maybe take away
   return next_inode_nr;//?
}   //getter
      //dont need a setter for next inode number bc will just increment
file_type inode::get_file_type(){
   return fileType;
} //getter need this??

      


file_error::file_error (const string& what):
//implementation of a file error could have been done in line? idk
//need to change?
            runtime_error (what) {
}

//all these functions do is throw a file error based on the file type
//that is in basefile
   //those fucntions will either be inherited or overwritten
   //if dont override, will be inherited

//can leave alone until plainfile size
const wordvec& base_file::readfile() const {
   
   throw file_error ("is a " + error_file_type());
}

void base_file::writefile (const wordvec&) {
   throw file_error ("is a " + error_file_type());
}

void base_file::remove (const string&) {
   throw file_error ("is a " + error_file_type());
}

inode_ptr base_file::mkdir (const string&) {
   throw file_error ("is a " + error_file_type());
}

inode_ptr base_file::mkfile (const string&) {
   throw file_error ("is a " + error_file_type()); //dont work
}

//plainfile must override read and writefile
   //but can go ahead and inherit remove mkdir and mkfile
//all of these need to be done!!!
size_t plain_file::size() const {   //constant function
   size_t size {0};  //must change to actually return size
   DEBUGF ('i', "size = " << size);
   return size;
}

const wordvec& plain_file::readfile() const {
   DEBUGF ('i', data);
   return data;   //must change
}

void plain_file::writefile (const wordvec& words) {
   DEBUGF ('i', words);//must change
}
//directory must override remove mkdir and mkfile but can inherit
//readfile and writefile
//all of these need to be done!!!

//could just handle plain files initially
//because need to make an inode for the root directory
//needs to have a directory file in it
//but the last three wont be used if dont test using those, dont make or delete files
size_t directory::size() const {
   size_t size {0};  //can use directory.size function in map?
   DEBUGF ('i', "size = " << size);
   return size;
}
//just override the base files

void directory::remove (const string& filename) {
   DEBUGF ('i', filename); //needs to delete something from a directory
}

inode_ptr directory::mkdir (const string& dirname) {
   DEBUGF ('i', dirname);//creates directory
   return nullptr;
}

inode_ptr directory::mkfile (const string& filename) {
   DEBUGF ('i', filename); //creates file
   return nullptr;
}

