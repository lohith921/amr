/* This program takes input a .ele file or a .node files and writes the refined mesh into new files with extension .r.node and .r.ele*/
#ifdef SINGLE
#define REAL float
#else
#define REAL double
#endif

#include <cmath>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "mmap_lib_p.h"
#include "structures.h"

static int num_ele  =  0; // to keep track of total no of elements
static int num_nodes = 0; // to keep track of total no of nodes

/********************************************************************/
void insert_element(struct element *head_ele, struct element *t){
	std::cout << "Entered insert element " << std::endl;
	struct element *temp;
	temp = (struct element *)malloc(sizeof(struct element *));
	temp->next = NULL;
	if(head_ele->next == NULL)
		head_ele->next = t;
	else{
		temp = head_ele->next;
		while(temp->next != NULL)
			temp = temp->next;
		temp->next = t;
	}
	num_ele++;
}

/***********************************************************************/
void print_element(struct element *e){
	printf("#: %d, Nodes %d, %d, %d\n",e->ele_no,e->nodes[0],e->nodes[1],e->nodes[2]);
}
/**********************************************************************/
void process(struct element *el, struct node_map *n_map, struct edge_map* emap, int tol){
     struct element *temp1, *temp2;
     temp1 = el;
     if ((temp1 != NULL) && (temp1->visited == -1) && (n_map != NULL)){
	 	REAL *vertexA,*vertexB,*vertexC;
		REAL *midAB, *midBC, *midCA, length;
		vertexA = new REAL[2];
		vertexB = new REAL[2];
		vertexC = new REAL[2];
		struct element *nw, *nw2;
		vertexA = map_getnode(n_map,temp1->nodes[0]);
		vertexB = map_getnode(n_map,temp1->nodes[1]);
		//vertexC = map_getnode(n_map,temp1->nodes[2]);
		length = calc_length(vertexA, vertexB);
		//std::cout << "Length AB for " << temp1->ele_no << " is " << length << std::endl;
		if (length >= tol){
			midAB  =  new REAL[2]; //(REAL *)malloc(2*sizeof(REAL));
			midAB  =  compute_mid(vertexA, vertexB);
			num_nodes++;
			map_setnode(n_map, num_nodes, midAB);
			nw = new element();
			nw->ele_no  =  ++num_ele;
			nw->nodes[0]  =  num_nodes;
			nw->nodes[1]  =  temp1->nodes[1];
			nw->nodes[2]  =  temp1->nodes[2];
			set_nedgemap(emap, nw->nodes);
			
			temp1->nodes[1]  =  num_nodes;
			set_nedgemap(emap, temp1->nodes);
			nw->next  =  temp1->next;
			temp1->next  =  nw; //new triangles A,mid,C and mid,B,C are formed
			
			int ngb = edgemap_getnodes(emap, temp1->nodes[1], temp1->nodes[0]);
			//std::cout << "Neighboring node is " << ngb << std::endl;	        	
			if (ngb != -1){
				temp2 = find_element(el, temp1->nodes[0], temp1->nodes[1], ngb);
				if(temp2 != NULL){
					//std::cout << " Neighbor tri found is " << temp2->ele_no << std::endl;
					nw2 = new element();
					nw2->ele_no  =  ++num_ele;
					nw2->nodes[0]  = temp1->nodes[0];
					nw2->nodes[1]  = num_nodes;
					nw2->nodes[2]  = ngb;
					set_nedgemap(emap, nw2->nodes);
					
					temp2->nodes[0] = num_nodes;
					temp2->nodes[1] = temp1->nodes[1];
					temp2->nodes[2] = ngb;
					set_nedgemap(emap, temp2->nodes);
					
					nw2->next = temp2->next;
					temp2->next = nw2;
					//std::cout << "Inserted new elements " << nw->ele_no << " " << nw2->ele_no << std::endl;
					//temp2 = nw2;
				}	
				else{
					//std::cout << "No shared triangle found, returning " << std::endl;
					//return;
				}
			}
			//process(temp1, n_map, emap, tol);
			//return;
		}
		vertexB = map_getnode(n_map,temp1->nodes[1]);
		vertexC = map_getnode(n_map,temp1->nodes[2]);
		//std::cout << "Calling calc_length BC for element " << temp1->ele_no << std::endl;
		length = calc_length(vertexB, vertexC);
		//std::cout << "Length BC for " << temp1->ele_no << " is " << length << std::endl;
		if (length >= tol){
       		midBC  =  new REAL[2];
			midBC  =  compute_mid(vertexB, vertexC);
			num_nodes++;
			map_setnode(n_map, num_nodes, midBC);	
			nw  =  new element();
			nw->ele_no  =  ++num_ele;
			nw->nodes[0]  =  temp1->nodes[0];
			nw->nodes[1]  =  num_nodes;
			nw->nodes[2]  =  temp1->nodes[2];
			set_nedgemap(emap, nw->nodes);
			
			temp1->nodes[2]  =  num_nodes;
			set_nedgemap(emap, temp1->nodes);
			nw->next  =  temp1->next;
			temp1->next  =  nw;//new triangles A,B,mid and mid,C,A are formed
			
			int ngb = edgemap_getnodes(emap, temp1->nodes[2], temp1->nodes[1]);
			//std::cout << "Node retrieved is " << ngb << std::endl;
			if (ngb != -1){
				temp2 = find_element(el, temp1->nodes[1], temp1->nodes[2], ngb);
				if (temp2 != NULL){
					nw2 = new element();
					nw2->ele_no  =  ++num_ele;
					nw2->nodes[0]  = temp1->nodes[1];
					nw2->nodes[1]  = ngb;
					nw2->nodes[2]  = num_nodes;
					set_nedgemap(emap, nw2->nodes);
					
					temp2->nodes[0]  = num_nodes;
					temp2->nodes[1]  = ngb;
					temp2->nodes[2]  = temp1->nodes[2];
					set_nedgemap(emap, temp2->nodes);
					nw2->next = temp2->next;
					temp2->next = nw2;
					//std::cout << "Inserted new elements " << nw->ele_no << " " << nw2->ele_no << std::endl;
					//temp2 = nw2;
				}
				else{
					//std::cout << "No shared triangle found, returning " << std::endl;
					//return;
				}
			}
			//processBC(temp1, n_map, emap, vertexB, vertexC);  		
			//process(temp1,n_map, emap, tol);
			//return;
		}
    		//std::cout << "Calling calc_length CA for element " << temp1->ele_no << std::endl;
    	vertexC = map_getnode(n_map,temp1->nodes[2]);
    	vertexA = map_getnode(n_map,temp1->nodes[0]);
    	length  =  calc_length(vertexC,vertexA);
		//std::cout << "Length CA for " << temp1->ele_no << " is " << length << std::endl;
    	if(length >= tol){ //   printf("91\n");
    		midCA  =  new REAL[2];
			midCA  =  compute_mid(vertexC,vertexA); 
			num_nodes++;
			map_setnode(n_map, num_nodes, midCA);
			nw  =  new element();
			nw->ele_no  =  ++num_ele;
			nw->nodes[0] = temp1->nodes[0];
			nw->nodes[1] = temp1->nodes[1];
			nw->nodes[2] = num_nodes;
			set_nedgemap(emap,nw->nodes);
			
			temp1->nodes[0] = num_nodes;
			set_nedgemap(emap, temp1->nodes);
			nw->next = temp1->next;
			temp1->next = nw;//new triangles A,B,mid and mid,B,C are formed
			
			int ngb = edgemap_getnodes(emap, temp1->nodes[0], temp1->nodes[2]);
			if (ngb != -1){
				temp2 = find_element(el, temp1->nodes[2], temp1->nodes[0], ngb);
				if (temp2 != NULL){
					nw2 = new element();
					nw2->ele_no  =  ++num_ele;
					nw2->nodes[0]  = temp1->nodes[2];
					nw2->nodes[1]  = ngb;
					nw2->nodes[2]  = num_nodes;
					set_nedgemap(emap,nw2->nodes);
    	
					temp2->nodes[0]  = ngb;
					temp2->nodes[1]  = temp1->nodes[0];
					temp2->nodes[2]  = num_nodes;
					set_nedgemap(emap,temp2->nodes);
					
					nw2->next = temp2->next;
					temp2->next = nw2;
					//std::cout << "Inserted new elements " << nw->ele_no << " " << nw2->ele_no << std::endl;
					//temp2 = nw2;
				}
				else{
					//std::cout << "No shared triangle found, returning " << std::endl;
					//return;
				}
			}
			//process(temp1,n_map, emap, tol);
			//return;
		}
    	std::cout << "All lengths under limit" << std::endl;
    	temp1->visited = 1;
		return;
		}
}

/************************************************************************/
int main(int argc, char *argv[])
{   
	if (argc != 3){
		std::cout << "You have not provided enough parameters, Please provide the .node/.ele file and the min lenght required" << std::endl;
		exit(0);
	}
	else{
		struct amrgeo *m;
		int i,j,nele,nnode,node_no, pos,nbs[3];
		std::string filebase, filename;
		std::ifstream fnode, fele;
		std::vector<std::string> tok;
		std::string line;
		
		REAL vertices[2], minLen;
		
		struct element *head_ele = new element();
		head_ele->ele_no = -1;
		struct node *head_node = new node();
		struct edge_map *emap;
		
		m = new amrgeo();
 		filename = (std::string)argv[1];
 		if ((pos = filename.find(".node")) != -1){
 			filebase = filename.substr(0,pos);
 		}
 		else if ((pos = filename.find(".ele")) != -1){
 			filebase = filename.substr(0,pos);
 		}
 		else{
 			std::cout << "Please provide a valid .node / .ele file" << std::endl;
 		}
 		m->nodefilename = (filebase + ".node");
 		m->elefilename = (filebase + ".ele");
    	//std::cout << "The file names are: " << m->elefilename << " and " << m->nodefilename << std::endl;
       	minLen = atof(argv[2]);
       	//std::cout << "Min length argument is " << minLen << std::endl;
       	//	exit(0);
    	
    	fele.open(m->elefilename, std::ios::in);
    	//std::cout << "File opening successful " << std::endl;
    	getline(fele, line);
		tokenize(line, tok);
		nele = std::stoi(tok[0]);
		//std::cout << "no of elements is " << nele << std::endl;
		emap = new edge_map();
		//std::cout << "edge map created" << std::endl;
		//emap = create_edgemap();
		//std::cout << "0" << std::endl;
		for (int i = 1; i <= nele; i++){
		     struct element *temp = new element();
		     getline(fele, line);
		     std::cout << "line read is " << line << std::endl;
		     tok.erase(tok.begin(), tok.end());
		     //tok.clear();
		    //std::vector<std::string> tok;
		     std::cout << "cleared size is" << tok.size() << std::endl;
		     tokenize(line, tok);
		     //std::cout << "Tokenized values " << tok[0] << " " << tok[1] << " " << tok[2] << " " << 
		     //tok[3] << std::endl;
		     temp->ele_no = std::stod(tok[0]);
		     temp->nodes[0] = std::stoi(tok[1]);
		     temp->nodes[1] = std::stoi(tok[2]);
		     temp->nodes[2] = std::stoi(tok[3]);
		     //fele >> temp->ele_no >> temp->nodes[0] >> temp->nodes[1] >> temp->nodes[2];
		     std::cout << "before inserting" << std::endl;
		     insert_element(head_ele, temp); 
		    // std::cout << "Calling set_nedgemap " << std::endl;	
		     set_nedgemap(emap, temp->nodes);
		     
		     //std::cout << "before going for next iteration" << std::endl;
		     //line = "";
		}
		fele.close();
		//std::cout << "Reading elements completed" << std::endl;
		//display_elements(head_ele);
		tok.erase(tok.begin(), tok.end());
		// following code reads node file and creates node map. 
		fnode.open(m->nodefilename,std::ios::in);
		//std::cout << "1" << std::endl;
		char dmy1[3];
		char dmy2[1];
		// creating a map of nodes 
		struct	node_map *nodemap  =  create_nodemap();
		getline(fnode,line);
		tokenize(line,tok);
		nnode = std::stoi(tok[0]);
		//fnode >> nnode;
		//std::cout << "# of nodes are " << nnode << std::endl;
		num_nodes  =  nnode;
		for(int i = 1; i <= nnode; i++){
 			getline(fnode,line);
 			tok.erase(tok.begin(), tok.end());
 			tokenize(line,tok);
 			node_no = std::stoi(tok[0]);
 			vertices[0] = std::stod(tok[1]);
 			vertices[1] = std::stod(tok[2]);
 			//fnode >> node_no >> dmy1 >> vertices[0] >> dmy2 >> vertices[1];
 		map_setnode(nodemap, node_no, vertices);
			}
		//std::cout << "Reading nodes is done" << std::endl;
		fnode.close(); 
		//std::cout << "Displaying edge map" << std::endl;
		//display_edgemap(emap);
		struct element *temp = head_ele->next;
		while (temp != NULL){
			process(temp, nodemap, emap, minLen);
		temp = temp->next;
		}
		head_ele = sort_list(head_ele);
		write_elements(filename, head_ele, num_ele);
		write_nodes(filename, nodemap, num_nodes);
		std::cout << "Writing to files successful" << std::endl;
		return 0;
	}
}  		
