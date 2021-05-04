// $Id: commands.cpp,v 1.22 2021-05-03 17:36:57-07 - - $
//Kai O'Brien (kimobrie@ucsc.edu)
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
       wordvec split_path;
        shared_ptr <directory> state_dir = dynamic_pointer_cast<directory>
         (state.get_cwd()->get_contents());
     // int jcount = 1;
      for(unsigned long j = 1;j < words.size(); ++j){
         //if there is a path, but checks on each call
         split_path = split(words.at(j),"/");//skips make call but includes all paths
        
         int count =0;
         if(split_path.size()>1){//if its a path
            for(auto i =split_path.begin(); i< split_path.end()-1;i++){ //skip last cause creating last so wont exist
               if(state_dir->file_dne(split_path.at(count)) == true){
                  throw command_error ("cat: "+split_path.at(count)+": No such file or directory"); 
                  //like foo/wrongdir/bar/newfile
                  //would output wrongdir: no such file or dir
                }
               else if(state_dir->is_dir_(split_path.at(count)) == false){
                  throw command_error ("cat: "+split_path.at(count)+": No such file or directory");
               }
                count++; //cause the auto is an itr
            }
            //file needs to exist
            if(state_dir->file_dne(words.at(count))==true){
               throw command_error("cat: "+ words.at(j) +": No such file or directory");
            }
            //and not be a directory
            else if(state_dir->is_dir_(words.at(count))==true){
               throw command_error("cat: "+ words.at(j) +": Is a directory"); 
            }
            
            cout<< state_dir->get_second(words.at(count))->get_contents()->readfile();//this works
            cout<< "\n";
         }//endif
         //if no path
         else{
            if(state_dir->file_dne(words.at(j))==true){
               throw command_error("cat: "+ words.at(j) +": No such file or directory");
            }
            else if(state_dir->is_dir_(words.at(j))==true){
               throw command_error("cat: "+ words.at(j) +": Is a directory");   
            }
            cout<< state_dir->get_second(words.at(j))->get_contents()->readfile();//this works
            cout<< "\n";
         }
      }
   }
//go back to this
}

void fn_cd (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if(words.size() == 1 or words.at(1) == "/"){   //if no dir specified, send to root
     state.set_cwd(state.get_root());
   }
    else if(words.size()>2){
      throw command_error ("cd: Invalid number of arguments"); 
   }
      else{
      //if it is a path, then go to last one, checking validity
      //to make sure each one is a directory, then set cwd to last dir
      shared_ptr <directory> state_dir = dynamic_pointer_cast<directory>
            (state.get_cwd()->get_contents());
      wordvec split_path = split(words.at(1),"/");
      int count = 0;
      //if its a path   
      if(split_path.size()>1){
         for(auto i =split_path.begin(); i< split_path.end()-1;i++){ //skip last so can set_cwd?
            //if file does not exist at all
            if(state_dir->file_dne(split_path.at(count)) == true){
               throw command_error ("cd: "+words.at(1)+": No such file or directory"); 
            }
            //if exists but is not a directory
            else if(state_dir->is_dir_(split_path.at(count)) == false){
               throw command_error ("cd: "+ words.at(1) +": No such file or directory"); 
            }
            count++;
         }
         if(state_dir->file_dne(split_path.at(count)) == true){
               throw command_error ("cd: "+words.at(1)+": No such file or directory"); 
            }
            //if exists but is not a directory
         else if(state_dir->is_dir_(split_path.at(count)) == false){
            throw command_error ("cd: "+ words.at(1) +": No such file or directory"); 
         }
         else{
            state.set_cwd(state_dir->get_second(words.at(count)));
        
         }
      }
      else{
         if(state_dir->file_dne(words.at(1)) == true){
               throw command_error ("cd: "+words.at(1)+": No such file or directory"); 
            }
            //if exists but is not a directory
         else if(state_dir->is_dir_(words.at(1)) == false){
            throw command_error ("cd: "+ words.at(1) +": No such file or directory"); 
         }
         else{
            state.set_cwd(state_dir->get_second(words.at(1)));
         }
         
      }
   }
}

void fn_echo (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   cout << word_range (words.cbegin() + 1, words.cend()) << endl;
}


void fn_exit (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);

  exec::status(std::stoi(words[1]));
   throw ysh_exit();
}

void fn_ls (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if(words.size()>2){
      throw command_error ("ls: Invalid number of arguments"); //dont work
   }
   else{
      shared_ptr <directory> state_dir = dynamic_pointer_cast<directory>
            (state.get_cwd()->get_contents());
      //if one arg, then cwd is used
      if(words.size()==1){
         state_dir->print_ls("");
      }
      else if(words.at(1)=="/"){
         state_dir->print_ls("");
      }
      else{//use arg specified
         shared_ptr <directory> print_dir1 = dynamic_pointer_cast<directory>
         (state_dir->get_second(words.at(1))->get_contents());
         print_dir1->print_ls(words.at(1));
      }

   }
}

void fn_lsr (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if(words.size()>2){
      throw command_error ("ls: Invalid number of arguments"); //dont work
   }
   else{
      shared_ptr <directory> state_dir = dynamic_pointer_cast<directory>
            (state.get_cwd()->get_contents());
            string print_things;
      //if one arg, then cwd is used
      if(words.size()==1){
   
         state_dir->print_lsr(state.get_root(), state.get_root());
      }
      else{//use arg specified
      
         inode_ptr words1= state_dir->get_second(words.at(1));
         shared_ptr <directory> print_dir1 = dynamic_pointer_cast<directory>
         (state_dir->get_second(words.at(1))->get_contents());
         print_dir1->print_lsr(words1,state.get_root());
      }
   }

}


void fn_make (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   shared_ptr <directory> state_dir = dynamic_pointer_cast<directory>
         (state.get_cwd()->get_contents());
   if(words.size()==1){
      throw command_error ("make: No target specified"); //dont work
   } 
   wordvec split_path = split(words.at(1),"/");//skips make call but includes all paths
   int count = 0;
   //is a path
   if(split_path.size()>1){//if its a path
      for(auto i =split_path.begin(); i< split_path.end()-1;i++){ 
         //if file does not exist at all
         if(state_dir->file_dne(split_path.at(count)) == true){
            throw command_error ("make: "+split_path.at(count)+": No such file or directory"); 
       
         }
         //if file exists but is not a directory, needs to be while in the path
         else if(state_dir->is_dir_(split_path.at(count)) == false){
            throw command_error ("make: "+split_path.at(count)+": No such file or directory"); 
         }
         count++; //cause the auto is an itr
      }
      //after path, file being made
      if(state_dir->file_dne(split_path.at(count))==false){// if file exists
         if(state_dir->is_dir_(split_path.at(count)) == true){
            throw command_error ("make: "+ words.at(1) +": Is a directory"); 
         }
         else
         {  //updates file if exists
            inode_ptr updated_pointer = state_dir->update_file(split_path.at(count),wordvec(words.begin()+2,words.end()));
         }
      }
      else{//if file doesnt exist already, makes it
         inode_ptr new_file = state.get_cwd()->get_contents()->mkfile(split_path.at(count));
         //+2 makes it not include make or filename, jsut contents
         new_file->get_contents()->writefile(wordvec(words.begin()+2,words.end())); 
      }
   }
   //no path, this works!
   else{
     if(state_dir->file_dne(words.at(1))==true){//this works
         //if file dne then can do normal cause isnt dir or file
         inode_ptr new_file = state.get_cwd()->get_contents()->mkfile(words.at(1));
         //+2 makes it not include make or filename, jsut contents
         new_file->get_contents()->writefile(wordvec(words.begin()+2,words.end())); 
      }
      else{//not dir, but updating existing file
         if(state_dir->is_dir_(words.at(1))==true){//this works
             throw command_error ("make: "+ words.at(1) +": Is a directory"); 
         }
         else{
            inode_ptr updated_pointer = state_dir->update_file(words.at(1),wordvec(words.begin()+2,words.end()));
         }
      }
   }
}

void fn_mkdir (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
    if(words.size()==1){
      throw command_error ("mkdir: No target specified"); //dont work
   }
    else if(words.size()>2){
      throw command_error ("mkdir: Invalid number of arguments"); //dont work
   }
   

   wordvec split_path = split(words.at(1),"/");//skips mkdir call but includes all paths

   shared_ptr <directory> state_dir = dynamic_pointer_cast<directory>
         (state.get_cwd()->get_contents());
   //if has path, check if valid
 
      int count = 0;
      if(split_path.size()>1){//if its a path
         for(auto i =split_path.begin(); i< split_path.end()-1;i++){ //skip last cause creating last so wont exist
            if(state_dir->file_dne(split_path.at(count))==true){//this works //everything needs to exist but the last one
               throw command_error ("mkdir: "+split_path.at(count)+": No such file or directory"); //dont work
            }//print whole path?
            count++; //cause the auto is an itr
         }
         if(state_dir->file_dne(split_path.at(count)) == false){
            if(state_dir->is_dir_(split_path.at(count)) == true){
               throw command_error ("mkdir: "+split_path.at(count)+": File exists"); //dont work
            }
         }
         else{
       
            state_dir->mkdir(split_path.at(count));
         }
      }
      //if no path, or end of path, make sure dne
      //should be end if has path or not right?
      else{//if its not a path
         if(state_dir->file_dne(words.at(1)) == false){
             if(state_dir->is_dir_(words.at(1)) == true){
               throw command_error ("mkdir: "+words.at(1)+": File exists"); //dont work
             }
         
            state_dir->mkdir(words.at(1));
         }
         else{
   
            state_dir->mkdir(words.at(1));
         }
      }
}
//worksg
void fn_prompt (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if(words.size()<2){
      throw command_error ("mkdir: Invalid number of arguments"); //dont work
   }
   else{
      string fnprompt ="";
      for(unsigned long j = 1;j < words.size(); ++j){
         fnprompt+=words.at(j);
      }
      state.prompt(fnprompt);
   }
}
//just doesnt print path? segfaults with ivalid paths
//this works with the test cases but thats it i think??, idk  its good for now
void fn_pwd (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if(words.size()>2){
      throw command_error ("mkdir: Invalid number of arguments"); //dont work
   }
   else{
      
   
   state.path(state.get_cwd());
   }
}

void fn_rm (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_rmr (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

