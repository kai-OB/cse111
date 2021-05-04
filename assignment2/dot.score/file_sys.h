// $Id: file_sys.h,v 1.10 2021-05-03 17:36:57-07 - - $
//Kai O'Brien (kimobrie@ucsc.edu)
#ifndef __INODE_H__
#define __INODE_H__

#include <exception>
#include <iostream>
#include <memory> //contains shared pointer
#include <map>
#include <vector>
using namespace std;

#include "util.h"

// inode_t -
//    An inode is either a directory or a plain file.

enum class file_type {PLAIN_TYPE, DIRECTORY_TYPE};
class inode;
class base_file;
class plain_file;
class directory;
using inode_ptr = shared_ptr<inode>;
using base_file_ptr = shared_ptr<base_file>;
//implement directory file pointer? mabye not
ostream& operator<< (ostream&, file_type);


// inode_state -
//    A small convenient class to maintain the state of the simulated
//    process:  the root (/), the current directory (.), and the
//    prompt.

class inode_state {  //only one can exist in the entire filesystem
//inode state does not inherit from inode
//however they are cooperating classes

   friend class inode;
   friend ostream& operator<< (ostream& out, const inode_state&);
   //must be a friend not a member
   private:
      inode_ptr root ;
      inode_ptr cwd ;  
      string prompt_ {"% "}; 
   public:
      virtual ~inode_state();   //default or make own?
      inode_state (const inode_state&) = delete; // copy ctor
      inode_state& operator= (const inode_state&) = delete; // op=
 
      inode_state(); //constructor, have to do some work
      const string& prompt() const;
      void prompt (const string&);  //implement later?
      inode_ptr get_root();
      inode_ptr get_cwd();
      void set_cwd(inode_ptr);
      void path(const inode_ptr& state_cwd);
      
      
};

// class inode -
// inode ctor -
//    Create a new inode of the given type.
// get_inode_nr -
//    Retrieves the serial number of the inode.  Inode numbers are
//    allocated in sequence by small integer.
// size -
//    Returns the size of an inode.  For a directory, this is the
//    number of dirents.  For a text file, the number of characters
//    when printed (the sum of the lengths of each word, plus the
//    number of words.(spaces in between words)
//    

class inode {

   //inode does not inherit from inode state
   friend class inode_state;
   private:
      static size_t next_inode_nr;  //next inode number
      size_t inode_nr;//inode number itself
      base_file_ptr contents; //basefile is the abstract base class
      bool is_dir;  
      inode_ptr parent{nullptr}; 
   public:
   virtual ~inode() = default;   //destructor?
      inode (file_type);   //gets filetype or creates filetype?
      size_t get_inode_nr() const;//only copying pointers to inodes
      //hvae to set inode number?
      void set_contents(base_file_ptr);   //setter
      base_file_ptr get_contents(); //getter
      bool isdir(); 
      
      inode_ptr get_parent(); 
      void set_parent(inode_ptr);
      string filename = "";
};


// class base_file -
// Just a base class at which an inode can point.  No data or
// functions.  Makes the synthesized members useable only from
// the derived classes.

class file_error: public runtime_error {
   public:
      explicit file_error (const string& what); 
};

class base_file {
   protected:
      base_file() = default;  
      //becasue we have constructors defined, we need to specify default
      virtual const string& error_file_type() const = 0;
   public:
      virtual ~base_file() = default;
       
      base_file (const base_file&) = delete;
      base_file& operator= (const base_file&) = delete;
      virtual size_t size() const = 0;
     
      virtual const wordvec& readfile() const;
      //will read text from file
      virtual void writefile (const wordvec& newdata);
   
      virtual void remove (const string& filename);
      //only appropriate if we are dealing with directories?
      virtual inode_ptr mkdir (const string& dirname);
      virtual inode_ptr mkfile (const string& filename);
      virtual map<string,inode_ptr>& get_dirents();
     // virtual bool is_dir();
};

// class plain_file -
// Used to hold data.
// synthesized default ctor -
//    Default vector<string> is a an empty vector.
// readfile -
//    Returns a copy of the contents of the wordvec in the file.
// writefile -
//    Replaces the contents of a file with new contents.

class plain_file: public base_file {  
   //friend ostream? 
   private:
      wordvec data;  
      virtual const string& error_file_type() const override {
         static const string result = "plain file";
         return result;    
     
      }
   public:
   virtual ~plain_file() = default;
      virtual size_t size() const override;  //must be overridden

      virtual const wordvec& readfile() const override;  
      virtual void writefile (const wordvec& newdata) override;
     
      
};

// class directory -
// Used to map filenames onto inode pointers.
// default ctor -
//    Creates a new map with keys "." and "..".
// remove -
//    Removes the file or subdirectory from the current inode.
//    Throws an file_error if this is not a directory, the file
//    does not exist, or the subdirectory is not empty.
//    Here empty means the only entries are dot (.) and dotdot (..).
// mkdir -
//    Creates a new directory under the current directory and 
//    immediately adds the directories dot (.) and dotdot (..) to it.
//    Note that the parent (..) of / is / itself.  It is an error
//    if the entry already exists.
// mkfile -
//    Create a new empty text file with the given name.  Error if
//    a dirent with that name exists.

class directory: public base_file {
   private:
      // Must be a map, not unordered_map, so printing is lexicographic
      map<string,inode_ptr> dirents;
      virtual const string& error_file_type() const override {
         static const string result = "directory";
         return result;
      }
   public:
      virtual ~directory() = default;
      virtual size_t size() const override;
      virtual void remove (const string& filename) override;                                                         
      virtual inode_ptr mkdir (const string& dirname) override;
      virtual inode_ptr mkfile (const string& filename) override;
      virtual map<string,inode_ptr>& get_dirents()override;
      inode_ptr get_second(const string& filename); 
      bool file_dne(const string& words);
      bool is_dir_(const string& words); //getter need this??
      inode_ptr update_file(const string& filename, const wordvec&words);
      void print_ls(const string& filename);
      void print_lsr(inode_ptr ino_de,inode_ptr roo);
};

#endif

