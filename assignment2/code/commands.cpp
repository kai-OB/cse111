// $Id: commands.cpp,v 1.20 2021-01-11 15:52:17-08 - - $

#include "commands.h"
#include "debug.h"

command_hash cmd_hash {
   {"cat"   , fn_cat   },
   {"cd"    , fn_cd    },
   {"echo"  , fn_echo  },
   {"exit"  , fn_exit  },
   {"ls"    , fn_ls    },
   {"lsr"   , fn_lsr   },
   {"make"  , fn_make  },
   {"mkdir" , fn_mkdir },
   {"prompt", fn_prompt},
   {"pwd"   , fn_pwd   },
   {"rm"    , fn_rm    },
   {"rmr"   , fn_rmr   },
};

command_fn find_command_fn (const string& cmd) {
   // Note: value_type is pair<const key_type, mapped_type>
   // So: iterator->first is key_type (string)
   // So: iterator->second is mapped_type (command_fn)
  // DEBUGF ('c', "[" << cmd << "]");
   const auto result = cmd_hash.find (cmd);
   if (result == cmd_hash.end()) {
      throw command_error (cmd + ": no such function");
   }
   return result->second;
}

command_error::command_error (const string& what):
            runtime_error (what) {
}

int exit_status_message() {
   int status = exec::status();
   cout << exec::execname() << ": exit(" << status << ")" << endl;
   return status;
}

void fn_cat (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   //The contents of each file is copied to the standard output. An error is
   //reported if no files are specified, a file does not exist, or is a directory.
   if(words.size() == 1){   //if no files are specified
      throw command_error ("cat: No files are specified"); //dont work
   }
   else{
        cerr<< "before for \n";
      for(unsigned long i = 1;i < words.size(); ++i){
          shared_ptr <directory> state_dir = dynamic_pointer_cast<directory>
         (state.get_cwd()->get_contents());
         if(state_dir->file_dne(words.at(i))==true){
            throw command_error("cat: "+ words.at(i) +": No such file or directory");
         }
  map<string,inode_ptr>& state_dirent = (state.get_cwd()->get_contents()->get_dirents());
       // inode_ptr state_dirent = (state.get_cwd()->get_contents()->get_dirents()->first);
         cerr<< "after state dirent \n ";
         
         if(state_dir->get_file_type()==file_type::DIRECTORY_TYPE){//everything is directory?
             cerr<< "first if";
            throw command_error("cat: "+ words.at(i) +": is a directory!");   //throw command error not cerr or cout
         }



         /*cerr<< "inside for\n ";
          shared_ptr <directory> state_dir = dynamic_pointer_cast<directory>
         (state.get_cwd()->get_contents());
         cerr<< "after state dir \n ";
         map<string,inode_ptr> state_dirent = (state.get_cwd()->get_contents()->get_dirents());
         cerr<< "after state dirent \n ";
         
         
         cerr<< "after state second\n ";
        //accounts for more than one file, if file is a directory
         if(state_dirent.second->get_file_type() == file_type::DIRECTORY_TYPE){//everything is directory?
             cerr<< "first if";
            throw command_error("cat: "+ words.at(i) +": is a directory!");   //throw command error not cerr or cout
           
         }
         //if file does not exist, find returns the last
         //else if(state_second->get_inode_nr() == 0 ){
        

       // cout<< state_dirent.second->get_contents()->readfile();
       //.find(words.at(i))->first;*/
      }
   }

}

void fn_cd (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_echo (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   cout << word_range (words.cbegin() + 1, words.cend()) << endl;
}


void fn_exit (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   throw ysh_exit();
}

void fn_ls (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_lsr (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_make (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_mkdir (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_prompt (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_pwd (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_rm (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_rmr (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

