// $Id: file_sys.cpp,v 1.10 2021-04-30 21:39:38-07 - - $

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
  // DEBUGF ('i', "root = " << root << ", cwd = " << cwd
  //        << ", prompt = \"" << prompt() << "\"");

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
  // shared_ptr <directory> root_dir = dynamic_pointer_cast<directory>
   //                              (root->get_contents());
   //shared_ptr <directory>  
   cwd = root;
   root->parent =  root;
   pair <string, inode_ptr> dot = {".", root};  //sets dot, cwd, to root
   (root->get_contents()->get_dirents()).insert(dot);  
   
   pair <string, inode_ptr> dot_dot  = {"..", root};  //sets dot dot, the parent to root
   (root->get_contents()->get_dirents()).insert(dot_dot);
  
}
//inode_state method implementations
void inode_state::prompt(const string& s){
      prompt_  = s;
}  //implement later? its ok
      //sets the prompts
const string& inode_state::prompt() const { return prompt_; }
//just returns the prompt


inode_ptr inode_state::get_root(){ return root; } 

inode_ptr inode_state::get_cwd(){ return cwd; }//need this?

void inode_state::set_cwd(inode_ptr new_cwd){
   cwd = new_cwd;
   cout<<"new cwd:";
   cout<< cwd->filename;
}
void inode_state::path(const inode_ptr& state_cwd){
  
   wordvec rootpath;
   cout<<"made rootpath\n";
rootpath.push_back("/");
    rootpath.push_back(state_cwd->filename);//then pushback other names?
   cout<< "pushed back first time\n filename:";
  cout<<"roothpath:";
  cout<< rootpath;
   
   inode_ptr cwd_dirents = state_cwd->get_contents()->get_dirents().find("..")->second;
   cout<<"\nset cwd_dirents";
   while(cwd_dirents->inode_nr!= 1){
     cout<< "\n inwhile";
      cwd_dirents =  cwd_dirents->get_contents()->get_dirents().find("..")->second;//issue here!->use dot  dot not parent!!
      cout<< "in while cwd_dirents changed\n";
      //cout<<"/";
     // cout<<cwd_dirents->filename;
      
      rootpath.push_back(cwd_dirents->filename);
      rootpath.push_back("/");
      cout<< "in while\n rootpath pushedback:";
      cout<<rootpath;
     
   }
   cout<<"after while";
   string path_output;
  for (unsigned long i = 0; i>rootpath.size();i++){
     //path_output.append(rootpath.at(i));
     cout<< rootpath.at(i);
  }
  // cout<< path_output;
   
}



void rm_r( inode_ptr roo){
   //depth first search (postorder)
   map<string,inode_ptr>& roo_dirents = (roo->get_contents()->get_dirents());
   //create map of dirents of the file roo
   for(auto ritor = roo_dirents.crbegin(); ritor != roo_dirents.crend(); ++ritor){ //cr or nah
      //recur over each entry other than dot or dot dot
      if(ritor->first!="." and ritor->first != ".."
         and ritor->second->isdir()
         ==true){//->get_contents()?
         rm_r(ritor->second);
      }
      //if not directory, or empty directory, erase
      roo_dirents.erase(ritor->first);
   }
   roo_dirents.erase("."); //erasing root last
   roo_dirents.erase(".."); //erasing root last

}


/////////// inode_state destructor////////  

inode_state::~inode_state(){
   
   rm_r(root);
   cwd = nullptr;//need to do this?
   root = nullptr;//idk
}


ostream& operator<< (ostream& out, const inode_state& state) {
   //just prints out inode state
   //just used in debug statements in working code
   out << "inode_state: root = " << state.root//machine adresses
       << ", cwd = " << state.cwd;
   return out;
}
/////////////////////////////////inode//////////////////
inode::inode(file_type type): inode_nr (next_inode_nr++) {
   //constructor
   //need a virtual constructor but no such thing in c++
   //so instead pass in anargument
   //default constructor on an inode has been supressed
   //so just say new node and give it the particular filetype that you want to 
   //create
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
               //adjust the file sysem
               //making filesystem friends
               //inode and inode state are already friends
               //base file probably doesnt work well without inode
               //information hiding is not important?? idk
           break;
      default: assert (false);   //for the sake of clarity
   }
   //F ('i', "inode " << inode_nr << ", type = " << type);
}

size_t inode::get_inode_nr() const {

   //just gets inode number, already done
  // DEBUGF ('i', "inode = " << inode_nr);
   return inode_nr;
}
//void inode::set_contents(base_file_ptr new_contents){
//   contents = new_contents;
//}   //dont ever need to set new contents though right?
base_file_ptr inode::get_contents(){ return contents; } //getter

bool inode::isdir(){ 
   return is_dir; } //getter need this??
//or just use is_dir

inode_ptr inode::get_parent(){ 
   cout<<"get_parent filename:\n";
   cout<<parent->filename;
   return parent;
}
 void inode::set_parent(inode_ptr dir){
 
    parent = dir;
    cout<<"set_parent filename:";
   cout<<parent->filename;
 }



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
   throw file_error ("mkfile:is a " + error_file_type()); //dont work
}
//added functions
map<string,inode_ptr>& base_file::get_dirents() {
   throw file_error ("getdirents: is a " + error_file_type()); //dont work
}
/*bool base_file::is_dir() {
   throw file_error ("isdir: is a " + error_file_type()); //dont work
}*/

//plainfile must override read and writefile
   //but can go ahead and inherit remove mkdir and mkfile
//all of these need to be done!!!
size_t plain_file::size() const {   //constant function
   //use wordvec data
   size_t size  = data.size();  //does this work
  // DEBUGF ('i', "size = " << size);{
  for(auto i = data.begin();i<data.end();i++){ //will there be an extra space?
     size += i->size();//each word
  }
   return data.size();  //calling size function from map?
}

const wordvec& plain_file::readfile() const {
  // DEBUGF ('i', data);
   return data;   //dont change?
}

void plain_file::writefile (const wordvec& words) {
  // DEBUGF ('i', words);//must change
   data = words;  //sets data to the wordvec words
}
/*
bool plain_file::is_dir() {
   return  false;
}*/
//directory must override remove mkdir and mkfile but can inherit
//readfile and writefile
//all of these need to be done!!!

//could just handle plain files initially
//because need to make an inode for the root directory
//needs to have a directory file in it
//but the last three wont be used if dont test using those, dont make or delete files
size_t directory::size() const {
   //size_t size = dirents.size();  //can use directory.size function in map?
  // DEBUGF ('i', "size = " << size);
   return dirents.size();
}
//just override the base files

void directory::remove (const string& filename) {
   //DEBUGF ('i', filename); //needs to delete something from a directory
   //idk look at this more
   //if empty directory or if file
   //use find() function
   //shouldnt work on root though? idk
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

   /*if(dirents.find(dirname)->second != inode_ptr()){  //if it has been created
      throw file_error ("mkdir: file already exists: " + dirname); //throw error
   }*/
   cout<< "in mkdir";
   inode_ptr newDir = make_shared<inode>(file_type::DIRECTORY_TYPE);
   shared_ptr <directory> new_dir = dynamic_pointer_cast<directory>
         (newDir->get_contents());
  //make new dir
   newDir->filename = dirname;
   
   //newDir->set_parent(this.get_cwd());
   cout<<"\nnewDir filename:";
   cout<< newDir->filename;
    //cout<<"\nparent filename:";
   //cout<< newDir->get_parent()->filename;//parents arent changing
   //insert new dir to dirents
   cout<<"\ninserting new pair\n";
   pair<string,inode_ptr> newPair = {dirname,newDir};
   new_dir->dirents.insert(newPair);
   cout<<"inserted new pair\n";
  // newDir->set_parent(newDir->get_cwd());
   //add dot/dotdot to current dir
   pair <string, inode_ptr> dot = {".", newDir};  //sets dot, cwd
   //(newDir->get_contents()->get_dirents()).insert(dot);  
   cout<<"made dot \n";
   new_dir->dirents.insert(dot);
   cout<<" inserted dot\n";
   //cout<<dirents.find(".")->second->filename;
   newDir->set_parent(dirents.find("..")->second);
    
   pair <string, inode_ptr> dot_dot  = {"..", newDir->get_parent()};  //sets dot dot, the paren(cwd before new dir)
   //(newDir->get_contents()->get_dirents()).insert(dot_dot);
    cout<<" made dot dot\n";
   new_dir->dirents.insert(dot_dot);
   dirents.insert(pair <string, inode_ptr>(newDir->filename,newDir));
    cout<<" inserted dot dot\n";
   // newDir->set_parent(dirents.get_second(".."));
    //cout<< newDir->get_parent();
   // cout<<newDir->get_contents()->get_dirents().find("..")->second->filename;
   return newDir;
}

inode_ptr directory::mkfile (const string& filename) {
   //DEBUGF ('i', filename); //creates file
//file specified is created and the rest of the wordst
//are put in that file
//if the file already exists, a new one is not created but the 
//contents are replaced
//error to specify a directory
//if there are no words the file is empty
   //inode_ptr i_node_ptr = dirents.find(filename)->second;
   /*if(i_node_ptr->isdir() == true){
         throw file_error ("mkfile: file is a directory " + filename); //throw error
   }*/
   //make new file
   inode_ptr newFile = make_shared<inode>(file_type::PLAIN_TYPE);
   newFile->filename  = filename;
  // cout<< "\nfilename:";
  // cout<< filename;
   //newFile->set_parent(inode_state().get_cwd());
   //insert/replace contents
   pair<string,inode_ptr> newFilePair = {filename,newFile};
   //dirents.at(newFile)= newFile;
  // dirents.insert(newFilePair);//dirents[filename]= newFile;
   return newFile;
}

map<string,inode_ptr>& directory::get_dirents() {
   return dirents;
}
/*bool directory::is_dir() {
   return true;
}*/
/*inode_ptr directory::get_cwd(){
   dirents.find(".")->second;
}
*/
//void directory::writefile (const wordvec&) {
//   throw file_error ("writefile: is a " + error_file_type());
//}
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
   dirents.insert(update_pair);//dirents[filename]= newFile;
   return update_ptr;
}
inode_ptr directory::get_second(const string& filename){
   cout<<"checking for seg  fault\n";
   return dirents.find(filename)->second;
}






