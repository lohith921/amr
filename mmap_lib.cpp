/*
 map_lib
 A simple associative-array library for C
*/

#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <iostream>
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

// The following function frees node map.
void map_freenode(struct map_node *map) {
   if(!map)
      return;
   struct map_node *m, *mp;
   m = map;
   while(m != NULL) {
      mp = m;
      m = m->nxt;
      free(mp);
   }
}

// The following function is used to set node entries in node map.
void map_setnode(struct map_node *m,int pt,REAL coords[]) {
   struct map_node *map;
   // First time inserting
    if(m->point == 0){
    m->point = pt;
    m->xandy[0] = coords[0];
    m->xandy[1] = coords[1];
    m->nxt = NULL;
    return;
    }
    // #1 if pt matches any of the points already present
    for(map = m;;map = map->nxt) {
      if(pt == map->point){
          map->xandy[0] = coords[0]; 
	  map->xandy[1] = coords[1];           
          return;
        }// #1 ends here
      // #2 inserting at the last element
      if(map->nxt == NULL) {
          map->nxt = create_nodemap();
         //map->nxt=map_create();
         if(!map->nxt){
            return;
         }
	 map = map->nxt;
	 map->point = pt;
	 map->xandy[0] = coords[0];
	 map->xandy[1] = coords[1];
	 map->nxt = NULL;
         return;
      }// #2 ends here
   }
}

// The following function prints the node map.
void display_map(struct map_node *first){
 printf("Displaying the Node map\n");
 struct map_node *temp;
 temp = first;
 if(temp != NULL){
  while(temp != NULL){
   printf("Node : %d x= %f y= %f \n",temp->point,temp->xandy[0],temp->xandy[1]);
   temp = temp->nxt;
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

// The following function creates an edgemap.
struct edge_map* create_edgemap(){
	struct edge_map *em;
   	em = (struct edge_map *)malloc(sizeof(struct edge_map));
   	if(!em)
      		return NULL;
	em->key1 = 0;
	em->key2 = 0;
   	em->nxt = NULL;
   	return em;
}

// The following function free's edge map.
void free_edgemap(struct edge_map *map) {
   if(!map)
      return;
   struct edge_map *m, *mp;
   m = map;
   while(m!=NULL) {
      mp = m;
      m = m->nxt;
      free(mp);
   }
}

// The following function is used to set node entries in edge map.
void set_edgemap(struct edge_map *m,int k1, int k2,int node) {
   struct edge_map *map;
   // First time inserting
    if(m->node_nums.empty()){
    	m->key1 = k1;
    	m->key2 = k2;
    	m->node_nums.push_back(node);
    	m->nxt = NULL;
    	return;
    }
    // #1 if key1 & key2 matches any of the edges already present
    for(map=m;;map=map->nxt) {
      if ( ((k1 == map->key1) && (k2 == map->key2)) || ((k1 == map->key2) && (k2 == map->key1)) ){
      		map->node_nums.push_back(node);        
          	return;
        }// #1 ends here
      // #2 inserting at the last element
      if (map->nxt == NULL) {
          map->nxt = create_edgemap();
         //map->nxt=map_create();
         if(!map->nxt)
            return;
	 map = map->nxt;
	 map->key1 = k1;  map->key2 = k2;
	 map->node_nums.push_back(node);
	 map->nxt = NULL;
         return;
      }// #2 ends here
   }
}

void display_edgemap(struct edge_map *first){
	printf("displaying the edge map\n");
	struct edge_map *temp;
	temp = first;
	if (temp != NULL){
		while(temp != NULL){
		std::cout << temp->key1 << "," << temp->key2 << " Nodes are: " << temp->node_nums[0] << temp->node_nums[1] <<std::endl;
		temp = temp->nxt;
		}
	}
	else{
		printf("Invalid map\n");
		return; 
	}
}

// The following function is used to get nodes from the edge map.
std::vector<int> edgemap_getnodes(struct edge_map *m,int k1, int k2) {
   struct edge_map *map;
   std::vector<int> dummy;
   dummy = {0,0};
   for(map = m;map != NULL;map = map->nxt) {
     if ( ((map->key1 == k1) && (map->key2 == k2)) || ((map->key1 == k2) && (map->key2 == k1)) ){ 
       return map->node_nums;
     }
   }
   return dummy;
}

/*******************************************************************
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
**************************************************************/
