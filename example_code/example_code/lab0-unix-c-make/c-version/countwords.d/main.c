// $Id: main.c,v 1.1 2019-12-17 14:53:52-08 - - $

#include <ctype.h>
#include <errno.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vstring.h"
#include "wordlist.h"

void countwords (FILE* infile, wordlist* list) {
   vstring* vstr = vstring_new();
   bool in_word = false;
   for (;;) {
      int chr = fgetc (infile);
      if (isalpha (chr)) {
         in_word = true;
         vstring_append (vstr, tolower (chr));
      }else {
         if (in_word) wordlist_incr (list, vstring_peek (vstr));
         in_word = false;
         vstring_clear (vstr);
      }
      if (chr == EOF) break;
   }
   vstring_delete (vstr);
}

int main (int argc, char** argv) {
   int exit_status = EXIT_SUCCESS;
   const char* exec_name = basename (argv[0]);
   wordlist* list = wordlist_new();
   if (argc == 1) {
      countwords (stdin, list);
   }else {
      for (int argi = 1; argi != argc; ++argi) {
         const char* filename = argv[argi];
         FILE* infile = fopen (filename, "r");
         if (infile == NULL) {
            exit_status = EXIT_FAILURE;
            fprintf (stderr, "%s: %s: %s\n",
                     exec_name, filename, strerror (errno));
         }else {
            countwords (infile, list);
            fclose (infile);
         }
      }
   }
   wordlist_print (list);
   wordlist_delete (list);
   return exit_status;
}

