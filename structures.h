/* This is used to store all structure definitions */

#ifndef STRUCTURES_H
#define STRUCTURES_H

#define FILENAMESIZE 2048
#define INPUTLINESIZE 1024
#include <string>
#include <vector>

#ifdef SINGLE
#define REAL float
#else
#define REAL double
#endif

/* Structure to hold properties */
struct amrgeo {
       int nodeindi; // nodefile indicator
       int eleindi; // element file indicator
       int attribindi = 0; // attributes indicator, if set to 1, attributes are available otherwise no.
       int boundindi = 0; // Boundary indicator
       std::string elefilename;
       std::string nodefilename;
       REAL minlength;// min length could be passed from command line
};

/* structure for elements*/
 struct element{
	   int ele_no, visited;
	   int nodes[3];
       struct element *next;
       element(){
       		ele_no = 0;
       		visited = -1;
       		nodes[0] = 0;
       		nodes[1] = 0;
       		nodes[2] = 0;
       		next = NULL;
       }
}; 

/* structure for nodes*/
 struct node{
 	int node_no;
 	int bnd = -1;
    REAL vertices[2];
   	struct node *next;
   	node(){
  		node_no = 0;
       		vertices[0] = 0;
       		vertices[1] = 0;
       		next = NULL;
       	}       		
 };
 
 /*structure to hold vertices */
 struct vertex{
       int no;
       REAL *values;
       vertex(){
       		no = 0;
       		values = NULL;
       	}
 };
  void tokenize(std::string str, std::string *token_v);
  REAL* compute_mid(REAL*, REAL*);
  REAL calc_length(REAL *, REAL *);
  void write_elements(std::string, struct element*, int);
  void write_nodes(std::string ,struct node_map *, int);
  struct element *sort_list(struct element *);
  struct element * find_element(struct element *, int , int , int );
  void display_elements(struct element *);
 #endif
