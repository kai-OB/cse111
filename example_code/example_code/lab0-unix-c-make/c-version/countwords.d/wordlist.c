// $Id: wordlist.c,v 1.1 2019-12-17 14:53:52-08 - - $

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wordlist.h"

typedef struct node node;
struct node {
   char* word;
   size_t count;
   node* link;
};
struct wordlist {
   node* head;
};

wordlist* wordlist_new (void) {
   wordlist* list = malloc (sizeof (wordlist));
   assert (list != NULL);
   list->head = NULL;
   return list;
}

void wordlist_delete (wordlist* list) {
   while (list->head != NULL) {
      node* tmp = list->head;
      list->head = tmp->link;
      free (tmp->word);
      free (tmp);
   }
   free (list);
}

void wordlist_incr (wordlist* list, const char* word) {
   node** currp = &(list->head);
   while (*currp != NULL && strcmp ((*currp)->word, word) < 0) {
      currp = &(*currp)->link;
   }
   if (*currp == NULL || strcmp (word, (*currp)->word) < 0) {
      node* link = *currp;
      *currp = malloc (sizeof (node));
      assert (*currp != NULL);
      (*currp)->word = strdup (word);
      assert ((*currp)->word != NULL);
      (*currp)->count = 0;
      (*currp)->link = link;
   }
   ++(*currp)->count;
}

void wordlist_print (wordlist* list) {
   for (node* itor = list->head; itor != NULL; itor = itor->link) {
      printf ("%s %zd\n", itor->word, itor->count);
   }
}
