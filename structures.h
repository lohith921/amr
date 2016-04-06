/* This is used to store all structure definitions */

#ifndef STRUCTURES_H
#define STRUCTURES_H

#define FILENAMESIZE 2048
#define INPUTLINESIZE 1024
#include <string>

/* Structure to hold properties */
struct amrgeo {
       int nodeindi; // nodefile indicator
       int eleindi; // element file indicator
       std::string elefilename;
       std::string nodefilename;
       REAL minlength;// min length could be passed from command line
};

/* structure for elements*/
 struct element{
	   int ele_no;
	   int nodes[3];
       struct element *next;
       element(){
       		ele_no = 0;
       		nodes[0] = 0;
       		nodes[1] = 0;
       		nodes[2] = 0;
       		next = NULL;
       }
} *head_ele; 

/* structure for nodes*/
 struct node{
       int node_no;
       REAL vertices[2];
	   struct node *next;
 } *head_node;
 
 /*structure to hold vertices */
 struct vertex{
       int no;
       REAL *values;
 };
 
 #endif
