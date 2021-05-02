// $Id: commands.cpp,v 1.25 2021-05-02 02:00:49-07 - - $

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
      for(unsigned long i = 1;i < words.size(); ++i){
         shared_ptr <directory> state_dir = dynamic_pointer_cast<directory>
         (state.get_cwd()->get_contents());
         if(state_dir->file_dne(words.at(i))==true){
            throw command_error("cat: "+ words.at(i) +": No such file or directory");
         }
         if(state_dir->is_dir_(words.at(i))==true){//everything is directory?
            throw command_error("cat: "+ words.at(i) +": is a directory!");   //throw command error not cerr or cout
         }
         /*if((words.at(i))=="/"){//everything is directory?
            throw command_error("cat: "+ words.at(i) +": is a directory!");   //throw command error not cerr or cout
         }*/
        cout<< state_dir->get_second(words.at(i))->get_contents()->readfile();//this works
       
      }
   }
//go back to this
}

void fn_cd (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}
//dont have to do anything???
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
   shared_ptr <directory> state_dir = dynamic_pointer_cast<directory>
         (state.get_cwd()->get_contents());
   if(words.size()==1){
      throw command_error ("make: No target specified"); //dont work
   } 
   else if(state_dir->file_dne(words.at(1))==true){//this works
      //if file dne then can do normal cause isnt dir or file
      inode_ptr new_file = state.get_cwd()->get_contents()->mkfile(words.at(1));
      //+2 makes it not include make or filename, jsut contents
      new_file->get_contents()->writefile(wordvec(words.begin()+2,words.end())); 
   }

   else if(state_dir->is_dir_(words.at(1))==true){
      throw command_error("cat: "+ words.at(1) +": is a directory!");   //throw command error
   }
  /* else if((words.at(1))=="/"){//everything is directory?
      throw command_error("cat: "+ words.at(1) +": is a directory!");   //throw command error 
   }*/
   else{//not dir, but updating existing file
         inode_ptr updated_pointer = state_dir->update_file(words.at(1),wordvec(words.begin()+2,words.end()));
        // cout<< updated_pointer->get_contents()->readfile();  //makes the file, but includes "make"??
    }

}

void fn_mkdir (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
    if(words.size()==1){
      throw command_error ("mkdir: No target specified"); //dont work
   }
    else if(words.size()>2){
      throw command_error ("mkdir: invalid number of arguments"); //dont work
   }
   wordvec split_path = split(words.at(1),"/");//skips mkdir call but includes all paths
   shared_ptr <directory> state_dir = dynamic_pointer_cast<directory>
         (state.get_cwd()->get_contents());
   //if has path, check if valid
      int count = 1;
      if(split_path.size()>1){//if its a path
         for(auto i =split_path.begin()+1; i< split_path.end()-1;i++){ //skip last cause creating last so wont exist
            if(state_dir->file_dne(split_path.at(count))==true){//this works //everything needs to exist but the last one
               throw command_error ("mkdir:"+split_path.at(count)+" directory does not exist"); //dont work
            }
            count++; //cause the auto is an itr
         }
         if(state_dir->file_dne(split_path.at(count)) == false){
            throw command_error ("mkdir:"+split_path.at(count)+" directory already exists"); //dont work
         }
         else{
            state_dir->mkdir(split_path.at(count));
         }
      }
      //if no path, or end of path, make sure dne
      //should be end if has path or not right?
      else{//if its not a path
         if(state_dir->file_dne(split_path.at(0)) == false){
            throw command_error ("mkdir:"+split_path.at(0)+" directory already exists"); //dont work
         }
         else{
            state_dir->mkdir(split_path.at(0));
         }
      }
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

