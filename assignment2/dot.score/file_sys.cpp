// $Id: file_sys.cpp,v 1.11 2021-05-03 17:36:57-07 - - $
//Kai O'Brien (kimobrie@ucsc.edu)
#include <cassert>
#include <iostream>
#include <stdexcept>

using namespace std;

#include "debug.h"
#include "file_sys.h"

size_t inode::next_inode_nr {1};

ostream& operator<< (ostream& out, file_type type) {
   switch (type) {
      case file_type::PLAIN_TYPE: out << "PLAIN_TYPE"; break;
      case file_type::DIRECTORY_TYPE: out << "DIRECTORY_TYPE"; break;
      default: assert (false);
   };
   return out;
}
     
     


inode_state::inode_state() {

   //inode state constructor
   root = make_shared <inode> (file_type::DIRECTORY_TYPE);
   cwd = root;
   root->parent =  root;
   pair <string, inode_ptr> dot = {".", root};  //sets dot, cwd, to root
   (root->get_contents()->get_dirents()).insert(dot);  
   
   pair <string, inode_ptr> dot_dot  = {"..", root};  //the parent to root
   (root->get_contents()->get_dirents()).insert(dot_dot);
  
}
//inode_state method implementations
void inode_state::prompt(const string& s){
      prompt_  = s;
}  //implement later? its ok
      //sets the prompts
const string& inode_state::prompt() const { return prompt_; }

inode_ptr inode_state::get_root(){ return root; } 

inode_ptr inode_state::get_cwd(){ return cwd; }//need this?

void inode_state::set_cwd(inode_ptr new_cwd){
   cwd = new_cwd;
}
void inode_state::path(const inode_ptr& state_cwd){
  
   wordvec rootpath;
    rootpath.push_back(state_cwd->filename);//then pushback other names?
  cout<<"/";
  cout<< rootpath;
   
   inode_ptr cwd_dirents = state_cwd->get_contents()->get_dirents().find("..")->second;
 
   while(cwd_dirents->inode_nr!= 1){
      cwd_dirents =  cwd_dirents->get_contents()->get_dirents().find("..")->second;
      rootpath.push_back(cwd_dirents->filename);
      rootpath.push_back("/");
     
   }
   string path_output;
  for (unsigned long i = 0; i>rootpath.size();i++){
     cout<< rootpath.at(i);
  }
   
}



void rm_r( inode_ptr roo){
   //depth first search (postorder)
   map<string,inode_ptr>& roo_dirents = (roo->get_contents()->get_dirents());
   //create map of dirents of the file roo
   for(auto ritor = roo_dirents.crbegin(); ritor != roo_dirents.crend(); ++ritor){ 
      //recur over each entry other than dot or dot dot
      if(ritor->first!="." and ritor->first != ".."
         and ritor->second->isdir()
         ==true){//->get_contents()?
         rm_r(ritor->second);
      }
      //if not directory, or empty directory, erase
      roo_dirents.erase(ritor->first);
   }
   roo_dirents.erase("."); 
   roo_dirents.erase(".."); 

}


/////////// inode_state destructor////////  

inode_state::~inode_state(){
   
   rm_r(root);
   cwd = nullptr;
   root = nullptr;
}


ostream& operator<< (ostream& out, const inode_state& state) {
   out << "inode_state: root = " << state.root//machine adresses
       << ", cwd = " << state.cwd;
   return out;
}
/////////////////////////////////inode//////////////////
inode::inode(file_type type): inode_nr (next_inode_nr++) {
   //constructor
   //need a virtual constructor but no such thing in c++
   //so instead pass in anargument
   //depends on the command it is being called from
  // fileType = type;
   switch (type) {
      case file_type::PLAIN_TYPE:
         is_dir = false;
           contents = make_shared<plain_file>();
           break;
      case file_type::DIRECTORY_TYPE:
         is_dir = true;
           contents = make_shared<directory>(); //make shared of a plain or directory
           break;
      default: assert (false);   //for the sake of clarity
   }
   //F ('i', "inode " << inode_nr << ", type = " << type);
}

size_t inode::get_inode_nr() const {
  // DEBUGF ('i', "inode = " << inode_nr);
   return inode_nr;
}

base_file_ptr inode::get_contents(){ return contents; } //getter

bool inode::isdir(){ 
   return is_dir; } 

inode_ptr inode::get_parent(){ 
   return parent;
}
 void inode::set_parent(inode_ptr dir){
    parent = dir;
 }



file_error::file_error (const string& what):
            runtime_error (what) {
}

//all these functions do is throw a file error
// based on the file type

const wordvec& base_file::readfile() const {
   
   throw file_error ("readfile: is a " + error_file_type());
}

void base_file::writefile (const wordvec&) {
   throw file_error ("writefile: is a " + error_file_type());
}

void base_file::remove (const string&) {
   throw file_error ("remove: is a " + error_file_type());
}

inode_ptr base_file::mkdir (const string&) {
   throw file_error ("mkdir:is a " + error_file_type());
}

inode_ptr base_file::mkfile (const string&) {
   throw file_error ("mkfile:is a " + error_file_type());
}
//added functions
map<string,inode_ptr>& base_file::get_dirents() {
   throw file_error ("getdirents: is a " + error_file_type());
}

//plainfile must override read and writefile
   //but can go ahead and inherit remove mkdir and mkfile
size_t plain_file::size() const {   //constant function
   //use wordvec data
   size_t size  = data.size();  //does this work
  // DEBUGF ('i', "size = " << size);{
  for(auto i = data.begin();i<data.end();i++){ 
     size +=i->size();//each word
  }
  if(size>1){
size--;
  }
  
   return size;  //calling size function from map?
}

const wordvec& plain_file::readfile() const {
  // DEBUGF ('i', data);
   return data;
}

void plain_file::writefile (const wordvec& words) {
  // DEBUGF ('i', words);//must change
   data = words; 
}

//directory must override remove mkdir and mkfile 
//but can inherit
//readfile and writefile

//could just handle plain files initially
size_t directory::size() const {
   //size_t size = dirents.size();  //can use directory.size function in map?
  // DEBUGF ('i', "size = " << size);
   
   return dirents.size();
}
void directory::remove (const string& filename) {
   //DEBUGF ('i', filename); 
     inode_ptr rm_ptr = dirents.find(filename)->second;
   if(rm_ptr->isdir() == false
      ||dirents.find(filename)->first != ".."){
      dirents.erase(filename);
   }
}

inode_ptr directory::mkdir (const string& dirname) {
  // DEBUGF ('i', dirname);//creates directory
   //error if file or directory of same name is already
   //created, or if the complete pathname to the parent of
   //this dir does not already exist
   //dot and dot dot added to dirents
   inode_ptr newDir = make_shared<inode>(file_type::DIRECTORY_TYPE);
   shared_ptr <directory> new_dir = dynamic_pointer_cast<directory>
         (newDir->get_contents());
  //make new dir
   newDir->filename = dirname;
   //insert new dir to dirents
   pair<string,inode_ptr> newPair = {dirname,newDir};
   //add dot/dotdot to current dir
   pair <string, inode_ptr> dot = {".", newDir};  //sets dot, cwd
   new_dir->dirents.insert(dot);
   
   newDir->set_parent(dirents[".."]);
    
   pair <string, inode_ptr> dot_dot  = {"..", newDir->get_parent()}; 
   new_dir->dirents.insert(dot_dot);
   dirents.insert(pair <string, inode_ptr>(newDir->filename,newDir));
  
   return newDir;
}

inode_ptr directory::mkfile (const string& filename) {
   //DEBUGF ('i', filename); //creates file
//file specified is created and the rest of the wordst
   inode_ptr newFile = make_shared<inode>(file_type::PLAIN_TYPE);
   newFile->filename  = filename;
   pair<string,inode_ptr> newFilePair = {filename,newFile};
    dirents.insert(pair <string, inode_ptr>(newFile->filename,newFile));
   return newFile;
}

map<string,inode_ptr>& directory::get_dirents() {
   return dirents;
}

bool directory::file_dne( const string& str){
   if(dirents.find(str)== dirents.end()){
      return true;
   }
   return false;
}
bool directory::is_dir_(const string& words){
  return( dirents.find(words)->second->isdir());
}

inode_ptr directory::update_file(const string& filename, const wordvec& words){
   inode_ptr update_ptr = dirents.find(filename)->second;
   update_ptr->get_contents()->writefile(words);
   pair<string,inode_ptr> update_pair = {filename,update_ptr};
   dirents.insert(update_pair);
   return update_ptr;
}
inode_ptr directory::get_second(const string& filename){
   return dirents.find(filename)->second;
}
void directory::print_ls(const string& filename){
   string print = "/";
   print+=filename;
   print+=":";
 
   for(auto i = dirents.begin();i!=dirents.end();i++){
      if(i->second->isdir()==true){
         shared_ptr <directory> print_dir = dynamic_pointer_cast<directory>
         (i->second->get_contents());
         print += (i->second->get_inode_nr())+'0';
         print += "  ";
         print += to_string(print_dir->size())+" ";
         print += i->first;
         print += "/\n";
      }
      else if(i->second->isdir()==false){
          shared_ptr <plain_file> print_file = dynamic_pointer_cast<plain_file>
         (i->second->get_contents());
         print += (i->second->get_inode_nr())+'0';
         print += "  ";
         print += to_string(print_file->size())+" ";
         print += i->first;
         print += "\n";
      }
   }
   cout<< print;
}
void directory::print_lsr(inode_ptr ino_de,inode_ptr roo){
   string print = "/";
   print+=ino_de->filename;
   print+=":";
  
  for(auto i = dirents.begin();i!=dirents.end();i++){
   
      if(i->second->isdir()==true&&i->second!=roo){
        shared_ptr <directory> print_dir = dynamic_pointer_cast<directory>
         (i->second->get_contents());
        
          if(print_dir->size()>2){
            auto b = i;
            b++;
            if(print_dir->get_second(b->first)->get_parent()->filename==i->second->filename){
               print_dir->print_lsr(print_dir->get_second(b->first),roo);
            }
          }
          print += (i->second->get_inode_nr())+'0';
         print += "  ";
         print += to_string(print_dir->size())+" ";
         print += i->first;
         print += "/\n";
      }
      else if(i->second->isdir()==true&&i->second ==roo){
         shared_ptr <directory> print_dir2 = dynamic_pointer_cast<directory>
         (i->second->get_contents());
         print += (i->second->get_inode_nr())+'0';
         print += "  ";
         print += to_string(print_dir2->size())+" ";
         print += i->first;
         print += "/\n";
      }
      else if(i->second->isdir()==false){
          shared_ptr <plain_file> print_file = dynamic_pointer_cast<plain_file>
         (i->second->get_contents());
         print += (i->second->get_inode_nr())+'0';
         print += "  ";
         print += to_string(print_file->size())+" ";
         print += i->first;
         print += "\n";
      }
      
   }
   cout<< print;

}






