/*
{// map_lib
// A simple associative-array library for C
//
// License: MIT / X11
// Copyright (c) 2009 by James K. Lawless
// jimbo@radiks.net http://www.radiks.net/~jimbo
// http://www.mailsend-online.com
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
}
*/

#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include "mmap_lib.h"


#ifdef SINGLE
#define REAL float
#else
#define REAL double
#endif

struct map_t *map_create() {
   struct map_t *m;
   m=(struct map_t *)malloc(sizeof(struct map_t));
   if(!m)
      return NULL;
   m->point=0;
   m->xandy=(REAL *)malloc(sizeof(REAL));
   m->xandy[0]=0.0;
   m->xandy[1]=0.0;
   m->nxt=NULL;
   return m;
}

void map_free(struct map_t *map) {
   if(!map)
      return;
   struct map_t *m, *mp;
   m=map;
   while(m!=NULL) {
      mp=m;
      m=m->nxt;
      free(mp);
   }
}

void map_set(struct map_t *m,int pt,REAL coords[]) {
   struct map_t *map;
   // First time inserting
    if(m->point==0){
    m->point=pt;
    m->xandy[0]=coords[0];
    m->xandy[1]=coords[1];
    m->nxt=NULL;
    return;
    }
    // #1 if pt matches any of the points already present
    for(map=m;;map=map->nxt) {
      if(pt==map->point){
          map->xandy[0]=coords[0]; 
	  map->xandy[1]=coords[1];           
          return;
        }// #1 ends here
      // #2 inserting at the last element
      if(map->nxt==NULL) {
          map->nxt=map_create();
         //map->nxt=map_create();
         if(!map->nxt){
            return;
         }
	 map=map->nxt;
	 map->point=pt;
	 map->xandy[0]=coords[0];
	 map->xandy[1]=coords[1];
	 map->nxt=NULL;
         return;
      }// #2 ends here
   }
}
void display_map(struct map_t *first){
 printf("displaying the map\n");
 struct map_t *temp;
 temp=first;
 if(temp!=NULL){
  while(temp!=NULL){
   printf("Node : %d x= %f y= %f \n",temp->point,temp->xandy[0],temp->xandy[1]);
   temp=temp->nxt;
  }
 }
 else{
  printf("Invalid map\n");
  return; 
 }
}
REAL* map_get(struct map_t *m,int pt) {
   struct map_t *map;
   for(map=m;map!=NULL;map=map->nxt) {
     if(map->point==pt){ //!strcasecmp(name,map->name)) {
       return map->xandy;
     }
   }
   return NULL;
}
