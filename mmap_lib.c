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
// The following function creates a map for nodes.
struct map_node *create_nodemap() {
   struct map_node *m;
   m = (struct map_node *)malloc(sizeof(struct map_node));
   if(!m)
      return NULL;
   m->point = 0;
   m->xandy = (REAL *)malloc(sizeof(REAL));
   m->xandy[0] = 0.0;
   m->xandy[1] = 0.0;
   m->nxt = NULL;
   return m;
}

// The following function creates a map for neighbors.
struct map_neighbor *create_neighbormap() {
   struct map_neighbor *m;
   m = (struct map_neighbor *)malloc(sizeof(struct map_node));
   if(!m)
      return NULL;
   m->t_num = 0;
   m->nbs = (int *)malloc(sizeof(int));
   m->nbs[0] = 0;
   m->nbs[1] = 0;
   m->nbs[2] = 0;
   m->nxt = NULL;
   return m;
}

// The following function frees node map.
void map_freenode(struct map_node *map) {
   if(!map)
      return;
   struct map_node *m, *mp;
   m = map;
   while(m!=NULL) {
      mp = m;
      m = m->nxt;
      free(mp);
   }
}

// The following function frees neighbor map.
void map_freeneighbor(struct map_neighbor *map) {
   if(!map)
      return;
   struct map_neighbor *m, *mp;
   m = map;
   while(m!=NULL) {
      mp = m;
      m = m->nxt;
      free(mp);
   }
}

// The following function is used to set node entries in node map.
void map_setnode(struct map_node *m,int pt,REAL coords[]) {
   struct map_node *map;
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
          map->nxt = create_nodemap();
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

// The following function is used to set neighbor entries in neighbor map.
void map_setneighbor(struct map_neighbor *m,int t,int neighbs[]) {
   struct map_neighbor *map;
   // First time inserting
    if(m->t_num == 0){
    m->t_num = t;
    m->nbs[0] = neighbs[0];
    m->nbs[1] = neighbs[1];
    m->nbs[2] = neighbs[2];
    m->nxt = NULL;
    return;
    }
    // #1 if pt matches any of the points already present
    for(map = m;;map = map->nxt) {
      if(t == map->t_num){
          map->nbs[0] = neighbs[0]; 
	  map->nbs[1] = neighbs[1];
	  map->nbs[2] = neighbs[2];           
          return;
        }// #1 ends here
      // #2 inserting at the last element
      if(map->nxt == NULL) {
          map->nxt = create_neighbormap();
         //map->nxt=map_create();
         if(!map->nxt){
            return;
         }
	 map = map->nxt;
	 map->t_num = t;
	 map->nbs[0] = neighbs[0];
	 map->nbs[1] = neighbs[1];
	 map->nbs[2] = neighbs[2];
	 map->nxt = NULL;
         return;
      }// #2 ends here
   }
}

// The following function prints the node map.
void display_map(struct map_node *first){
 printf("displaying the map\n");
 struct map_node *temp;
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

// The following function is used to get a node from the node map.
REAL* map_getnode(struct map_node *m,int pt) {
   struct map_node *map;
   for(map = m;map != NULL;map = map->nxt) {
     if(map->point == pt){ //!strcasecmp(name,map->name)) {
       return map->xandy;
     }
   }
   return NULL;
}

// The following function is used to get neibhors from the neighbor map.
int* map_getneighbors(struct map_neighbor *m,int t) {
   struct map_neighbor *map;
   for(map = m;map != NULL;map = map->nxt) {
     if(map->t_num == t){ //!strcasecmp(name,map->name)) {
       return map->nbs;
     }
   }
   return NULL;
}
