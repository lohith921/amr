#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<iostream>

struct edge_map{
	int key1, key2;
	std::vector<int> node_nums;
	struct edge_map *nxt;
};

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

// The following function frees edge map.
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

// The following function is used to set node entries in node map.
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

/*int main(){
	struct edge_map * m = create_edgemap();
	set_edgemap(m,1,3,2);
	
	set_edgemap(m,2,3,1);
	set_edgemap(m,3,2,6);
	
	set_edgemap(m,2,6,3);
	set_edgemap(m,6,2,5);
	
	set_edgemap(m,6,3,2);
	set_edgemap(m,3,6,7);
	
	set_edgemap(m,6,7,3);
	set_edgemap(m,7,6,3);
	
	set_edgemap(m,5,3,4);
	set_edgemap(m,3,5,6);
	
	//display_edgemap(m);
	std::vector<int> n = edgemap_getnodes(m,3,4);
	std::cout << n[0] << " " << n[1] << std::endl;
	return 0;
}*/
