// $Id: vstring.c,v 1.1 2019-12-17 14:53:52-08 - - $

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vstring.h"

#define INIT_SIZE 16
struct vstring {
   char* data;
   size_t strlen;
   size_t size;
};

vstring* vstring_new (void) {
   vstring* vstr = malloc (sizeof (vstring));
   assert (vstr != NULL);
   vstr->strlen = 0;
   vstr->size = INIT_SIZE;
   vstr->data = malloc (vstr->size);
   assert (vstr->data != NULL);
   vstr->data[0] = '\0';
   return vstr;
}

void vstring_delete (vstring* vstr) {
   assert (vstr != NULL);
   free (vstr->data);
   free (vstr);
}

void vstring_clear (vstring* vstr) {
   assert (vstr != NULL);
   vstr->strlen = 0;
   vstr->data[0] = '\0';
}

void vstring_append (vstring* vstr, char chr) {
   assert (vstr != NULL);
   assert (vstr->data != NULL);
   assert (vstr->strlen < vstr->size);
   vstr->data[vstr->strlen++] = chr;
   if (vstr->strlen == vstr->size) {
      vstr->size *= 2;
      vstr->data = realloc (vstr->data, vstr->size);
      assert (vstr->data != NULL);
   }
   vstr->data[vstr->strlen] = '\0';
}

const char* vstring_peek (vstring* vstr) {
   assert (vstr != NULL);
   return vstr->data;
}

