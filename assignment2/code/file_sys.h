// $Id: file_sys.h,v 1.8 2020-10-22 14:37:26-07 - - $

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
      inode_ptr root {nullptr};
      inode_ptr cwd {nullptr};   //need to make a different assignment
      //to them in the constructor!!!
      string prompt_ {"% "};  //cant have the fuction and field name be the same
   public:
      virtual ~inode_state();   //default or make own?
      inode_state (const inode_state&) = delete; // copy ctor
      inode_state& operator= (const inode_state&) = delete; // op=
      //-delete says that if u attempt to copy it the compiler
      //will refuse
      inode_state(); //constructor, have to do some work
      //need to make a destructor?
      //need to make a new inode and point root and cwd at it 

      const string& prompt() const;
      //returns prompt when need to print stuff out
      void prompt (const string&);  //implement later?
      //sets the prompts

      inode_ptr get_root();
      inode_ptr get_cwd();
      void set_cwd(inode_ptr);//??? //yes
      
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
                           //directory and plainfile as subclasses
                           //dir and pl do all the work
      //inode contains a pointer to base file
      file_type fileType;  //to know the filetype
      inode_ptr parent{nullptr}; //initializes it to null in case there isnt a parent?
   public:
   virtual ~inode() = default;   //destructor?
      inode (file_type);   //gets filetype or creates filetype?
      size_t get_inode_nr() const;//only copying pointers to inodes
      //hvae to set inode number?
      void set_contents(base_file_ptr);   //setter
      base_file_ptr get_contents(); //getter

      file_type get_file_type(); //getter need this??
      inode_ptr get_parent(); //need these?
      void set_parent(inode_ptr);

};


// class base_file -
// Just a base class at which an inode can point.  No data or
// functions.  Makes the synthesized members useable only from
// the derived classes.

class file_error: public runtime_error {
   public:
      explicit file_error (const string& what); //its constructor takes in a string
};

class base_file {
   protected:
      base_file() = default;  //basefile has no fieldsl just an abstract class
         //becasue we have constructors defined, we need to specify default
      virtual const string& error_file_type() const = 0;
      //says that this function does not exist in basefile, is an abstract function
      //must be overwritten in a subclass
   public:
      virtual ~base_file() = default;
         //if you dont specify a destructor, it will be specified for you^
         //must declare destructor as virtual, so it doesnt just delete
         //pointers?
      base_file (const base_file&) = delete;
      //movers, not allowing files to be moved
      //means that the implicity generated copier will be prohibited
      //dont want to allow base files to be copied, use POINTERS  
      base_file& operator= (const base_file&) = delete;
      virtual size_t size() const = 0;
      //base file has no meaning so it is 0
      //no meaning because can be one thing on a plain and another on a directory file
      virtual const wordvec& readfile() const;
      //will read text from file
      virtual void writefile (const wordvec& newdata);
      //writes text to file, only available to text files
      //error will be thrown for directory file
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
         return result;    //this is ok? no change?
         //if you try and run a function on the wrong type of file,
         //returns the filetype
         //ex remove f , error f is a plainfile
         //static to avoid creating it every time we call a function
      }
   public:
   virtual ~plain_file() = default;
      virtual size_t size() const override;  //must be overridden
      //set size??
      virtual const wordvec& readfile() const override;  //override error functions in base class
      //could make base class functions abstract then require them to be overwritten
      //in base class?
      //should use keyword override
      //cant override something that doesnt exist in the base class
      // would still compile but makes a compile time error so def use override
      //to make it easier to debug
      //keyword virtal is optional
       virtual void remove (const string& filename)override;
       virtual inode_ptr mkdir (const string& dirname) override;  //error will be thrown if basefile version is called
                                                               //, implement in directory
      virtual void writefile (const wordvec& newdata) override;
      virtual inode_ptr mkfile (const string& filename) override;

     // virtual bool is_dir() override;
      
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
      virtual size_t size() const override;//????? need to override
      virtual void remove (const string& filename) override;   //need to implement to throw error?    
                                                              //if directory name is called
      virtual inode_ptr mkdir (const string& dirname) override;
      virtual inode_ptr mkfile (const string& filename) override;
      virtual void writefile (const wordvec& newdata) override;//????
       //make a num files
      virtual map<string,inode_ptr>& get_dirents()override;
     // virtual bool is_dir() override;
      //inode_ptr get_cwd();  //need to get rid of?

      
};

#endif

