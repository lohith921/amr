/*
 map_lib
 A simple associative-array library for C
*/

#include<vector>

#ifdef SINGLE
#define REAL float
#else
#define REAL double
#endif

#ifndef MMAP_LIB_H
#define MMAP_LIB_H

struct map_node {
   struct map_node *nxt;
   int point;
   REAL *xandy;
};

struct edge_map{
	int key1, key2;
	std::vector<int> node_nums;
	struct edge_map *nxt;
};

// The following are related to node map.
struct map_node *create_nodemap();
void map_freenode(struct map_node *map);
void map_setnode(struct map_node *m,int pt,REAL coords[]);
REAL* map_getnode(struct map_node *m,int pt);

// The following are related to edge map.
struct edge_map* create_edgemap();
void free_edgemap(struct edge_map *map);
void set_edgemap(struct edge_map *m,int k1, int k2,int node);
void display_edgemap(struct edge_map *first);
std::vector<int> edgemap_getnodes(struct edge_map *,int , int );

#endif

/*struct map_neighbor{
	struct map_neighbor *nxt;
	int t_num;
	int* nbs;
	};
*/
