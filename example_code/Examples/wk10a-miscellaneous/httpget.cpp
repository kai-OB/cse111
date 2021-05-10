// $RCSfile: httpget.cpp,v $$Revision: 1.25 $

//
// httpget - get a web page from some host
//
// usage:
//    httpget [-p port] http_host pathname
//
// Default port is 80.  Gets the given page from the given host.
//

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
using namespace std;

#include <libgen.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define DEFAULT_PORT 80

struct program_options {
   const char* execname;
   int status = EXIT_SUCCESS;
   const char* host;
   in_port_t port = DEFAULT_PORT;
   const char* path;
   void init (int argc, char** argv);
   void usage();
} program;

void program_options::init (int argc, char** argv) {
   execname = argv[0];
   cout << "Command:";
   for (int argi = 0; argi != argc; ++argi) cout << " " << argv[argi];
   cout << endl;
   for(;;) {
      int opt = getopt (argc, argv, "p:");
      if (opt == EOF) break;
      switch (opt) {
         case 'p': port = atol (optarg); break;
         default: usage();
      }
   }
   if (optind + 2 != argc) usage();
   host = argv[optind];
   path = argv[optind + 1];
}

void program_options::usage() {
   cerr << "Usage: " << execname << " [-p port] [host [path]]" << endl;
   exit (EXIT_FAILURE);
}

struct http_socket {
   const char* host;
   in_port_t port;
   int sock_fd = 0;
   struct hostent* hostent_ptr = nullptr;
   http_socket (const char*, in_port_t);
   ~http_socket() { close (sock_fd); }
   http_socket (const http_socket&) = delete;
   http_socket& operator= (const http_socket&) = delete;
   void open();
   struct error: public runtime_error {
      error (const string& object, const char* message): 
                  runtime_error (object + ": " + message) {
      }
   };
};

http_socket::http_socket (const char* host_, in_port_t port_):
            host (host_), port (port_) {
   hostent_ptr = gethostbyname (host);
   if (not hostent_ptr) throw error (host, hstrerror (h_errno));
   cout << "host:port = " << host << ":"
        << port << endl;
   cout << "host_name = " << hostent_ptr->h_name << endl;
   for (char** alias = hostent_ptr->h_aliases;
        *alias != nullptr; ++alias) {
      cout << "host_alias = " << *alias << endl;
   }
   for (char** addr = hostent_ptr->h_addr_list;
        *addr != nullptr; ++addr) {
      cout << "host_addr = ";
      for (size_t byte = 0; byte < 4; ++byte) {
         if (byte > 0) cout << ".";
         cout << ((*addr)[byte] & 0xFF);
      }
      cout << endl;
   }
}

void http_socket::open() {

   // create a socket to connect to other machine
   string error_obj = "connect("s + host + ":" + to_string (port) + ")";
   sock_fd = socket (AF_INET, SOCK_STREAM, 0);
   if (sock_fd < 0) throw error (error_obj, strerror (errno));

   // create a struct sockaddr and connect it to the socket
   struct sockaddr_in sock;
   sock.sin_family = AF_INET; 
   sock.sin_port = htons (port);
   sock.sin_addr = *reinterpret_cast<in_addr*> (hostent_ptr->h_addr);
   int rc = connect (sock_fd, reinterpret_cast<sockaddr*> (&sock),
                     sizeof sock);
   if (rc < 0) throw error (error_obj, strerror (errno));
}

int main (int argc, char** argv) {
   program.init (argc, argv);
   try {
      http_socket server (program.host, program.port);
      server.open();

      // read info from the remote and convert to host format
      string getmsg = "GET "s + program.path + "\r\n";
      cout << getmsg;
      write (server.sock_fd, getmsg.c_str(), getmsg.size());
      for(;;){
         char buffer[4096];
         int nbytes = read (server.sock_fd, &buffer, sizeof buffer - 1);
         if (nbytes == 0) break;
         buffer[nbytes] = '\0';
         printf ("%s", buffer);
      }

   }catch (http_socket::error& error) {
      cerr << program.execname << ": " << error.what() << endl;
      program.status = EXIT_FAILURE;
   }
   return program.status;
}

//TEST// echo $0: Running httpget tests: >httpget.out
//TEST// ./httpget >>httpget.out 2>&1
//TEST// ./httpget Unknown.Host >>httpget.out 2>&1
//TEST// ./httpget soe.ucsc.edu >>httpget.out 2>&1
//TEST// ./httpget www2.ucsc.edu /courses/cmps109-wm/ >>httpget.out 2>&1
//TEST// mkpspdf httpget.ps httpget.c* httpget.out

