/* map_lib: A simple associative-array library for c++ */

#include <vector>

#ifdef SINGLE
#define REAL float
#else
#define REAL double
#endif

#ifndef MMAP_LIB_P_H
#define MMAP_LIB_P_H

struct node_map {
   struct node_map *nxt;
   int point;
   REAL *xandy;
};

 struct edge_map{
	std::pair<int, int> key_pair;
	int node_num;
	//std::vector<int> node_nums;
	struct edge_map *nxt;
	edge_map(){
		key_pair.first = -1;
		key_pair.second = -1;
		node_num = -1;
		nxt = NULL;
	}
};

// The following are related to node map.
struct node_map *create_nodemap();
void map_freenode(struct node_map *map);
void map_setnode(struct node_map *m,int pt,REAL coords[]);
REAL* map_getnode(struct node_map *m,int pt);

// The following are related to edge map.
//struct edge_map *create_edgemap();
void free_edgemap(struct edge_map *);
void set_edgemap(struct edge_map *,int ,int ,int );
void set_nedgemap(struct edge_map *, int nodes[3]);
void display_edgemap(struct edge_map *);
int edgemap_getnodes(struct edge_map *,int , int );

#endif

/*struct map_neighbor{
	struct map_neighbor *nxt;
	int t_num;
	int* nbs;
	};
*/
