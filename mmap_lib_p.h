/* map_lib: A simple associative-array library for c++
*/

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
	int key1, key2;
	std::vector<int> node_nums;
	struct edge_map *nxt;
	edge_map(){
		key1 = 0;
		key2 = 0;
		nxt = NULL;
		node_nums.push_back(-1);
		node_nums.push_back(-1);
	}
};

// The following are related to node map.
struct node_map *create_nodemap();
void map_freenode(struct node_map *map);
void map_setnode(struct node_map *m,int pt,REAL coords[]);
REAL* map_getnode(struct node_map *m,int pt);

// The following are related to edge map.
//struct edge_map *create_edgemap();
void free_edgemap(struct edge_map *map);
void set_edgemap(struct edge_map *m,int k1, int k2,int node);
void set_nedgemap(struct edge_map *m, int nodes[3]);
void display_edgemap(struct edge_map *first);
std::vector<int> edgemap_getnodes(struct edge_map *,int , int );

#endif

/*struct map_neighbor{
	struct map_neighbor *nxt;
	int t_num;
	int* nbs;
	};
*/
