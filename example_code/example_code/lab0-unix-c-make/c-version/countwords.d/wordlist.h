// $Id: wordlist.h,v 1.1 2019-12-17 14:53:52-08 - - $

//
// Sorted list of word and frequency counts.
//
// wordlist_new - allocate a new list
// wordlist_delete - free the list
// wordlist_incr - increment the word count, insert if not there
// 

#ifndef __WORDLIST_H__
#define __WORDLIST_H__

typedef struct wordlist wordlist;

wordlist* wordlist_new (void);
void wordlist_delete (wordlist*);
void wordlist_incr (wordlist*, const char*);
void wordlist_print (wordlist* list);

#endif

