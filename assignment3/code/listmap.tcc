// $Id: listmap.tcc,v 1.23 2021-05-22 22:09:18-07 - - $
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
   node* temp1 = anchor()->next;//or just do anchor().next
   node* temp2;
   while(temp1!=anchor()){
      temp2 = temp1;
      temp1 = temp1->next;
      //delete temp2;
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
   //if empty
   if(empty()){
      node *empty_node = new node(anchor(),anchor(),pair);
      anchor_.next = empty_node;
      anchor_.prev = empty_node;
      return iterator(empty_node);
   }
   //otherwise
   //if key is already there, the value is replaced
    node *new_node = new node(nullptr,nullptr,pair);
   for (auto itor = begin(); itor != end(); ++itor) {
         //if the itr is == key, update value
      if(!less(itor->first,pair.first) && 
         !less(pair.first,itor->first)) {
         itor->second = pair.second;//?? maybe
         return itor;
      }
         //if  pair.first is >= itor, not less than itor
      else if(!less(pair.first,itor->first)){
           //at the end of the list, pair.first is greater 
          // than the end, what to do
         new_node->next = itor.where->next;
         new_node->prev = itor.where;
         if(itor.where->next!=nullptr){
            itor.where->next->prev = new_node;
         }
         itor.where->next = new_node;
         break;
      }
   }
   
   return iterator(new_node);

}

//
// listmap::find(const key_type&)
// cant use ==, must use less()
//if not is less and not is greater
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::find (const key_type& that) {
   DEBUGF ('l', that);
   auto itor = begin();
   //for (auto itor = begin(); itor != end(); ++itor) {
    while(itor !=end()){
      if(!less(itor->first,that) && !less(that,itor->first)){
         //return itor;
         //return iterator(itor);
         break;
      }
      ++itor;
   }
  //return end();
  return iterator(itor);
}

//
// iterator listmap::erase (iterator position)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::erase (iterator position) {
   DEBUGF ('l', &*position);
   //dont need to iterate because have .where

   node *temp = position.where;
      iterator p = temp->prev;
      iterator n = temp->next;
   p.where->next = n.where;
   n.where->prev = p.where;

   //delete temp->key;
   delete temp;//valgrind
   return n;//should return temp->next's position
   
}


