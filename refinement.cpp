/* This program takes input a .ele file or a .node files and a .neigh file with switches -e, -n and -p resp*/
/* There is a small bug, the name of the file without extension should be 3 or more characters length*/
#ifdef SINGLE
#define REAL float
#else
#define REAL double
#endif

#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "mmap_lib_p.h"
#include "structures.h"

 static int num_ele  =  0; // to keep track of total no of elements
 static int num_nodes = 0; // to keep track of total no of nodes

/*********************************************************************/ 
 void info(){
      printf("This program is used to refine the mesh using edge bisection.");
      printf("This program requires a root name of the .node or .ele files");
      printf("Usage shall be\n");
      //printf("amrgeo -en <root name>\n");
 }

/*********************************************************************/
void display_elements(struct element *t){
	struct element *temp;
	temp = t->next;
	if(temp == NULL){
		printf("There are no elements in the linked list\n");
		exit(0);	
	}
	else{
		while(temp !=  NULL){
			printf("Element #: %d, nodes are: %d %d %d\n",temp->ele_no,temp->nodes[0],temp->nodes[1],temp->nodes[2]);
			temp = temp->next;
		}
	}
}

/********************************************************************/
void insert_element(struct element *head_ele, struct element *t){
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
void process(struct element *el,struct node_map *node_map, struct edge_map* emap, int tol){
     printf("Entered process for the element: %d\n",el->ele_no);
     struct element *temp1, *temp2;
     //struct node_map *temp2;
     int i, j, eleno, vertices[3]; 
     int ngAB, ngBC, ngCA;
     if(el->ele_no == -1)// for head element
               temp1 = el->next;
     else
               temp1 = el;
     REAL *vertexA,*vertexB,*vertexC;
     REAL *midAB, *midBC, *midCA, length;
     vertexA = new REAL[2];
     vertexB = new REAL[2];
     vertexC = new REAL[2];
     struct element *nw, *nw2, *nw3;
     while(temp1 != NULL){
                if(node_map != NULL){
                	vertexA = map_getnode(node_map,temp1->nodes[0]);
                	vertexB = map_getnode(node_map,temp1->nodes[1]);
                	vertexC = map_getnode(node_map,temp1->nodes[2]);
                }
                else{
                	std::cout << "The structures are not properly initiated" << std::endl;
                        exit(0);
                }
     		//length calculation begins
     		//std::cout << "Calling calc_length AB for element " << temp1->ele_no << std::endl;
     		length = calc_length(vertexA, vertexB);
     		std::cout << "Length AB for " << temp1->ele_no << " is " << length << std::endl;
     		if (length >= tol){
                	//std::cout << "Entered AB " << std::endl;
                	// ngb has the node numbers of nodes opposite to edge AB.
                	std::vector<int> ngb = edgemap_getnodes(emap, temp1->nodes[0], temp1->nodes[1]);
                	//std::cout << "Get nodes is successful" << std::endl;
                	if(ngb[0] == temp1->nodes[2])
                		ngAB = ngb[1];
			else
				ngAB = ngb[0];
			midAB  =  new REAL[2]; //(REAL *)malloc(2*sizeof(REAL));
                	midAB  =  compute_mid(vertexA, vertexB);
                	num_nodes++;
                	map_setnode(node_map, num_nodes, midAB);
    	        	nw = new element();
    	        	nw->ele_no  =  ++num_ele;
    	        	nw->nodes[0]  =  num_nodes;
    	        	nw->nodes[1]  =  temp1->nodes[1];
    	        	nw->nodes[2]  =  temp1->nodes[2];
    	        	set_nedgemap(emap,nw->nodes);
    			if( ngAB != -1){
				temp2 = find_element(el, temp1->nodes[0], temp1->nodes[1], ngAB);
				std::cout << "Neighbor found is " << temp2->ele_no << std::endl;
    	        		nw2 = new element();
    	        		nw2->ele_no  =  ++num_ele;
    	        		nw2->nodes[0]  = temp1->nodes[0];
    	        		nw2->nodes[1]  = ngAB;
    	        		nw2->nodes[2]  = num_nodes;
    	        		set_nedgemap(emap, nw2->nodes);
    	
    	        		nw3 = new element();
    	        		nw3->ele_no  =  ++num_ele;
    	        		nw3->nodes[0]  = num_nodes;
    	        		nw3->nodes[1]  = ngAB;
    	        		nw3->nodes[2]  = temp1->nodes[1];
    	        		set_nedgemap(emap, nw3->nodes);
    	    	
    	        		temp1->nodes[1]  =  num_nodes;
    	        		nw->next  =  temp1->next;
    	        		temp1->next  =  nw2; //new triangles A,mid,C and mid,B,C are formed
    	        		nw2->next = nw3;
    	        		nw3->next = temp2->next;
    	        		temp2->next = nw3;
    	        		std::cout << "Inserted new elements " << nw->ele_no << " " << nw2->ele_no << " " << nw3->ele_no << 						std::endl;
                		temp1 = nw;
                		temp2 = nw3;
                	}
                	else{
                		temp1->nodes[1]  =  num_nodes;
                		nw->next  =  temp1->next;
                		temp1->next = nw;
                		temp1 = nw;
                		std::cout << "Inserted new element " << nw->ele_no << " " << nw->nodes[0] 
        			<< " " << nw->nodes[1] << " " << nw->nodes[2] << std::endl; 	
                	}
                	process(temp1,node_map, emap, tol);
     		}
    
    		//std::cout << "Calling calc_length BC for element " << temp1->ele_no << std::endl;
    		length = calc_length(vertexB,vertexC);
    		std::cout << "Length BC for " << temp1->ele_no << " is " << length << std::endl;
    		if(length > tol){
              		// printf("90\n");
              		midBC  =  new REAL[2];
              		midBC  =  compute_mid(vertexB,vertexC);
              		num_nodes++;
              		map_setnode(node_map, num_nodes, midBC);
              		std::vector<int> ngb = edgemap_getnodes(emap, temp1->nodes[1], temp1->nodes[2]);
              		if (ngb[0] == temp1->nodes[0])
              			ngBC = ngb[1];
              		else
    				ngBC = ngb[0];
	      		nw  =  new element();
	      		nw->ele_no  =  ++num_ele;
	      		nw->nodes[0]  =  temp1->nodes[0];
	      		nw->nodes[1]  =  num_nodes;
	      		nw->nodes[2]  =  temp1->nodes[2];
	      		set_nedgemap(emap,nw->nodes);
	      		if (ngBC != -1){
				temp2 = find_element(el, temp1->nodes[1], temp1->nodes[2], ngBC);
	      			nw2 = new element();
	      			nw2->ele_no  =  ++num_ele;
	      			nw2->nodes[0]  = temp1->nodes[1];
	      			nw2->nodes[1]  = ngBC;
	      			nw2->nodes[2]  = num_nodes;
	      			set_nedgemap(emap,nw2->nodes);
    			
    				nw3 = new element();
    				nw3->ele_no  =  ++num_ele;
    				nw3->nodes[0]  = num_nodes;
    				nw3->nodes[1]  = ngBC;
    				nw3->nodes[2]  = temp1->nodes[2];
    				set_nedgemap(emap,nw3->nodes);
  	  		
  	  			temp1->nodes[2]  =  num_nodes;
  	  			nw->next  =  temp1->next;
  	  			temp1->next  =  nw2;//new triangles A,B,mid and mid,C,A are formed
  	  			nw2->next = nw3;
  	  			nw3->next = temp2->next;
  	  			temp2->next = nw3;
  	  			std::cout << "Inserted new elements " << nw->ele_no << " " << nw2->ele_no << " " << nw3->ele_no << std::endl;
  	  			temp1 = nw;
  	  			temp2 = nw3;
  	  		}
  	  		else{
  	  			temp1->nodes[2]  =  num_nodes;
  	  			nw->next  =  temp1->next;
  	  			temp1->next = nw;
  	  			temp1  =  nw;
  	  			std::cout << "Inserted new element " << nw->ele_no << " " << nw->nodes[0] 
        				<< " " << nw->nodes[1] << " " << nw->nodes[2] << std::endl;
  	  		}
  	  		process(temp1,node_map, emap, tol);
    		}
    		//std::cout << "Calling calc_length CA for element " << temp1->ele_no << std::endl;
    		length  =  calc_length(vertexC,vertexA);
    		std::cout << "Length CA for " << temp1->ele_no << " is " << length << std::endl;
    		if(length > tol){ //   printf("91\n");
    			midCA  =  new REAL[2];
			midCA  =  compute_mid(vertexC,vertexA); 
			num_nodes++;
			map_setnode(node_map, num_nodes, midCA);
  	
  			std::vector<int> ngb = edgemap_getnodes(emap, temp1->nodes[1], temp1->nodes[2]);
  			if(ngb[0] == temp1->nodes[0])
  				ngCA = ngb[1];
    			else
    				ngCA = ngb[0];
    		
    			nw  =  new element();
  			nw->ele_no  =  ++num_ele;
   			nw->nodes[0] = temp1->nodes[0];
   			nw->nodes[1] = temp1->nodes[1];
   			nw->nodes[2] = num_nodes;
   			set_nedgemap(emap,nw->nodes);
   			if (ngCA != -1){
				temp2 = find_element(el, temp1->nodes[2], temp1->nodes[0], ngCA);
   				nw2 = new element();
				nw2->ele_no  =  ++num_ele;
    				nw2->nodes[0]  = temp1->nodes[2];
    				nw2->nodes[1]  = ngCA;
    				nw2->nodes[2]  = num_nodes;
    				set_nedgemap(emap,nw2->nodes);
    	
    				nw3 = new element();
    				nw3->ele_no  =  ++num_ele;
    				nw3->nodes[0]  = ngBC;
    				nw3->nodes[1]  = temp1->nodes[0];
    				nw3->nodes[2]  = num_nodes;
    				set_nedgemap(emap,nw3->nodes);
    	
   				temp1->nodes[0] = num_nodes;
   				nw->next = temp1->next;
   				temp1->next = nw2;//new triangles A,B,mid and mid,B,C are formed
   				nw2->next = nw3;
   				nw3->next = temp2->next;
   				temp2->next = nw3;
   				std::cout << "Inserted new elements " << nw->ele_no << " " << nw2->ele_no << " " << nw3->ele_no << std::endl;
        			temp1 = nw;
        			temp2 = nw3;
        		}
        		else{
        			temp1->nodes[0] = num_nodes;
        			nw->next = temp1->next;
        			temp1->next = nw;
        			std::cout << "Inserted new element " << nw->ele_no << " " << nw->nodes[0] 
        			<< " " << nw->nodes[1] << " " << nw->nodes[2] << std::endl;
        			temp1 = nw;
        		}
        		process(temp1,node_map, emap, tol);
    		}
    		std::cout << "All lengths under limit, going for next" << std::endl;
    		//display_elements(el);
    		temp1 = temp1->next;
	}
//processing ends here
}

/************************************************************************/
int main(int argc, char **argv)
{   
	struct amrgeo *m;
	int i,j,nele,nnode,node_no;
	std::string filename;
	std::ifstream fnode, fele;
	std::vector<std::string> tok;
	std::string line;
	REAL vertices[2];
	int nbs[3];
	m = new amrgeo();
	struct element *head_ele = new element();
	struct node *head_node = new node();
	head_ele->ele_no = -1;
	struct edge_map *emap;
    	std::cout << "Please input the root of the file names without any extension" << std::endl;
 	std::cin >> filename;
 	m->elefilename = (filename + ".ele");
 	m->nodefilename = (filename + ".node");
    	//std::cout << "The file names are: " << m->elefilename << " and " << m->nodefilename << std::endl;
    	fele.open(m->elefilename,std::ios::in);
    	getline(fele, line);
	tokenize(line, tok);
	nele = std::stoi(tok[0]);
	//std::cout << "no of elements is " << nele << std::endl;
	emap = new edge_map();
	//emap = create_edgemap();
	//std::cout << "0" << std::endl;
	for( int i = 1; i <= nele; i++){
	     struct element *temp = new element();
	     getline(fele, line);
	     //std::cout << "line read is " << line << std::endl;
	     tok.erase(tok.begin(), tok.end());
	     tokenize(line, tok);
	     //std::cout << "Tokenized values " << tok[0] << " " << tok[1] << " " << tok[2] << " " << 
	     //tok[3] << std::endl;
	     temp->ele_no = std::stod(tok[0]);
	     temp->nodes[0] = std::stoi(tok[1]);
	     temp->nodes[1] = std::stoi(tok[2]);
	     temp->nodes[2] = std::stoi(tok[3]);
	     //fele >> temp->ele_no >> temp->nodes[0] >> temp->nodes[1] >> temp->nodes[2];
	     insert_element(head_ele, temp); 	
	     set_nedgemap(emap,temp->nodes);
	}
	fele.close();
	std::cout << "Reading elements completed" << std::endl;
	//display_elements(head_ele);
	tok.erase(tok.begin(), tok.end());
	/* following code reads node file and creates node map. */
	fnode.open(m->nodefilename,std::ios::in);
	//std::cout << "1" << std::endl;
	char dmy1[3];
	char dmy2[1];
	/* creating a map of nodes */
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
	std::cout << "Reading nodes is done" << std::endl;
	fnode.close(); 
	/*struct element *e = find_element(head_ele, 4,1,3);
	if ( e != NULL)
		std::cout << "Fetched the element " << e->ele_no << " with nodes " << e->nodes[0] << " " << e->nodes[1] << " " << e->nodes[2] << std::endl;
	else
		std::cout << "Could not find the element" << std::endl;*/
	process(head_ele, nodemap, emap, 12.0);
	std::cout << "Done processing" << std::endl;
	/*std::cout << "Printing node map" << std::endl;
	display_map(nodes);
	std::cout << " Process method is going to be called " << std::endl;
	
	std::cout << "Processing finished, # of elements are:" << num_ele << " # of nodes are" << num_nodes << std::endl;
     	std::cout << "Displaying elemets after processing" << std::endl;
     	display_elements(head_ele);
	std::cout << "5\n";
	//std::cout << "Displaying nodes after processing" << std::endl;
     	// display_map(nodes);
	std::cout << "Done the sorting" << std::endl;*/
     	//display_elements(head_ele);
     	
     	head_ele  =  sort_list(head_ele);
	write_elements(filename, head_ele, num_ele);
	write_nodes(filename, nodemap, num_nodes);
	std::cout << "Writing to files successful" << std::endl;
	return 0;
}  
