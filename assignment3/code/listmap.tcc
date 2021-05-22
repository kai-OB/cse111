// $Id: listmap.tcc,v 1.15 2019-10-30 12:44:53-07 - - $
//Kai O'Brien (kimobrie@ucsc.edu)

#include "listmap.h"
#include "debug.h"

//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//
//do ~ insert find and erase + main

//
// listmap::~listmap()
//
template <typename key_t, typename mapped_t, class less_t>
listmap<key_t,mapped_t,less_t>::~listmap() {
   DEBUGF ('l', reinterpret_cast<const void*> (this));
   //typical double linked list deconstructor
   //begin() is the "head"
   //DONT DELETE ANCHOR!
   node* temp1 = begin();
   node* temp2;
   while(temp1!=anchor()){
      temp2 = temp1;
      temp1 = temp->next;
      //or delete temp2;
      erase(temp2);
   }
}

//
// iterator listmap::insert (const value_type&)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::insert (const value_type& pair) {
   DEBUGF ('l', &pair << "->" << pair);
   return iterator();
}

//
// listmap::find(const key_type&)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::find (const key_type& that) {
   DEBUGF ('l', that);
   return iterator();
}

//
// iterator listmap::erase (iterator position)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::erase (iterator position) {
   DEBUGF ('l', &*position);
   //dont need to iterate because have .where

   node* temp = position.where;
   iterator rtrn = temp->next;
   temp->prev->next = temp->next;
   temp->next->prev = temp->prev;
   delete temp;
   return rtrn;//should return temp->next's position
}


