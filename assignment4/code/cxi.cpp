// $Id: cxi.cpp,v 1.1 2020-11-22 16:51:43-08 - - $

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>

using namespace std;

#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "protocol.h"
#include "logstream.h"
#include "sockets.h"

logstream outlog (cout);
struct cxi_exit: public exception {};

unordered_map<string,cxi_command> command_map {
   {"exit", cxi_command::EXIT},
   {"help", cxi_command::HELP},
   {"ls"  , cxi_command::LS  },
   {"get" , cxi_command::GET },
   {"put" , cxi_command::PUT },
   {"rm"  , cxi_command::RM  },



};

static const char help[] = R"||(
exit         - Exit the program.  Equivalent to EOF.
get filename - Copy remote file to local host.
help         - Print help summary.
ls           - List names of files on remote server.
put filename - Copy local file to remote host.
rm filename  - Remove file from remote server.
)||";

void cxi_help() {
   cout << help;
}

void cxi_ls (client_socket& server) {
   cxi_header header;
   header.command = cxi_command::LS;
   outlog << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   outlog << "received header " << header << endl;
   if (header.command != cxi_command::LSOUT) {
      outlog << "sent LS, server did not return LSOUT" << endl;
      outlog << "server returned " << header << endl;
   }else {
      size_t host_nbytes = ntohl (header.nbytes);
      auto buffer = make_unique<char[]> (host_nbytes + 1);
      recv_packet (server, buffer.get(), host_nbytes);
      outlog << "received " << host_nbytes << " bytes" << endl;
      buffer[host_nbytes] = '\0';
      cout << buffer.get();
   }
}
void cxi_get (client_socket& server, vector<string>& splitvec) {
   cxi_header header;
   header.command = cxi_command::GET;
   strncpy(header.filename,splitvec[1].c_str(),splitvec.size());
   
   outlog << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   outlog << "received header " << header << endl;
   //if(!fileout) error
   if (header.command != cxi_command::FILEOUT) {
      outlog << "sent GET, server did not return FILEOUT" << endl;
      outlog << "server returned " << header << endl;
   }
   //else set nbytes, declare buffer, recv, writefile,log<<success
   else {
      size_t host_nbytes = ntohl (header.nbytes);
      auto buffer = make_unique<char[]> (host_nbytes + 1);
      recv_packet (server, buffer.get(), host_nbytes);
   //ged rid of outlogs outlog << "received " << host_nbytes << " bytes" << endl;
      buffer[host_nbytes] = '\0';
      cout << buffer.get();
   //create ofstream and write 
   ofstream write_file (header.filename);
   write_file.write(buffer.get(),host_nbytes);  
   write_file.close();
   write_file << "GET sucess" << endl;
   }
   
}
void cxi_put (client_socket& server, vector<string>& splitvec) {
   cxi_header header;
   header.command = cxi_command::PUT;
   
   strncpy(header.filename,splitvec[1].c_str(),splitvec.size());
   //if !file.exist ->error
   //use ifstream
   ifstream read_file (header.filename);
  /* FILE* pipe = popen ("ls-l", "r");
   if(!pipe){
      cerr<< header.filename << " does not exist" << endl;
      return;
   }
   else{*/
      //init buffer
      struct stat stat_buf;
      int status = stat(header.filename, &stat_buf);
      if(status !=0){
         cerr<< header.filename << " does not exist" << endl;
         return;
      }
      auto buffer = make_unique<char[]> (stat_buf.st_size);
      //send payload
      read_file.read(buffer.get(), stat_buf.st_size);
      //header.command = cxi_command::PUT;????
      send_packet (server, &header, sizeof header);
      send_packet (server, buffer.get(), stat_buf.st_size); //put_nbytes.st_size?
      recv_packet (server, &header, sizeof header);
      
   //}
   if(header.command == cxi_command::ACK){
      cout << "PUT: ACK, sucess" << endl;
   }
   if(header.command == cxi_command::NAK){
      cout << "PUT: NAK, failure" << endl;
   }
   //pclose(pipe);
   read_file.close();// ?? 
}

void cxi_rm (client_socket& server, vector<string>& splitvec) {
   cxi_header header;
   header.command = cxi_command::RM;
   strncpy(header.filename,splitvec[1].c_str(),splitvec.size());

   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   if(header.command == cxi_command::ACK){
      cout << "RM: ACK, sucess" << endl;
   }
   if(header.command == cxi_command::NAK){
      cout << "RM: NAK, failure" << endl;
   }
      
}


void usage() {
   cerr << "Usage: " << outlog.execname() << " [host] [port]" << endl;
   throw cxi_exit();
}
//taken from split function in asg2
vector<string> split (const string& line, const string& delimiters) {
   vector<string> words;
   size_t end = 0;

   // Loop over the string, splitting out words, and for each word
   // thus found, append it to the output wordvec.
   for (;;) {
      size_t start = line.find_first_not_of (delimiters, end);
      if (start == string::npos) break;
      end = line.find_first_of (delimiters, start);
      words.push_back (line.substr (start, end - start));
   }
  // DEBUGF ('u', words);
   return words;
}

int main (int argc, char** argv) {
   outlog.execname (basename (argv[0]));
   outlog << "starting" << endl;
   vector<string> args (&argv[1], &argv[argc]);
   if (args.size() > 2) usage();
   string host = get_cxi_server_host (args, 0);
   in_port_t port = get_cxi_server_port (args, 1);
   outlog << to_string (hostinfo()) << endl;
   try {
      outlog << "connecting to " << host << " port " << port << endl;
      client_socket server (host, port);
      outlog << "connected to " << to_string (server) << endl;
      for (;;) {
         string line;
         getline (cin, line); //split this line after if
         if (cin.eof()) throw cxi_exit();
         vector<string> splitline = split(line," ");//makes new vector
         outlog << "command " << line << endl;
         const auto& itor = command_map.find (splitline[0]);
         cxi_command cmd = itor == command_map.end()
                         ? cxi_command::ERROR : itor->second;
         switch (cmd) { //add appropriate switch cases
         //take split funciton from util.cpp asg2
            case cxi_command::EXIT:
               throw cxi_exit();
               break;
            case cxi_command::HELP:
               cxi_help();
               break;
            case cxi_command::LS:   
               cxi_ls (server);
               break;
            case cxi_command::GET:   
               cxi_get (server, splitline);
               break;
            case cxi_command::PUT:   
               cxi_put (server, splitline);
               break;
            case cxi_command::RM:   
               cxi_rm (server, splitline);
               break;   
            default:
               outlog << line << ": invalid command" << endl;
               break;
         }
      }
   }catch (socket_error& error) {
      outlog << error.what() << endl;
   }catch (cxi_exit& error) {
      outlog << "caught cxi_exit" << endl;
   }
   outlog << "finishing" << endl;
   return 0;
}

