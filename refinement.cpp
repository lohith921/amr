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
#include "mmap_lib_p.h"
#include "structures.h"

 static int num_ele  =  0;// to keep track of total no of elements
 static int num_nodes = 0;// to keep track of total no of nodes

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
void insert_element(struct element *t){
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
/*******************************************************************/
REAL * compute_mid(REAL *vA, REAL *vB){
	REAL *nw;
	nw = new REAL[2];
	nw[0] = (vA[0]+vB[0]) / 2;
	nw[1] = (vA[1]+vB[1]) / 2;
	return nw;
}
/*******************************************************************/
REAL calc_length(REAL *A, REAL *B){
	std::cout << "Calc_length is called " << std::endl;
	std::cout << "The vertices are " << A[0] << " " << A[1] << " " << B[0] << " " << B[1] << std::endl;
	REAL d1,d2,d;
	d1 = A[0]-B[0];
	d2 = A[1]-B[1];
	REAL temp = (d1*d1) + (d2*d2);
	d = std::sqrt(temp);
	std::cout << "Length from inside calc_length is " << d << std::endl;
	return d;
}
/***********************************************************************/
void print_elements(struct element *e){
	printf("#: %d, Nodes %d, %d, %d\n",e->ele_no,e->nodes[0],e->nodes[1],e->nodes[2]);
}
/*************************************************************************/
struct element * find_element(struct element *e, struct node n1, struct node n2, struct node n3){
	struct element *temp;
	 if(el->ele_no == -1){// for head element
                   temp = el->next;
     }
     else{
                   temp = el;
     }
	 while(temp != NULL){
		 if (temp->nodes[0]->node_no == n1.node_no && temp->nodes[1]->node_no == n2.node_no && temp->nodes[2]->node_no == n3.node_no)
			 return temp;
		 else if (temp->nodes[0]->node_no == n2.node_no && temp->nodes[1]->node_no == n3.node_no && temp->nodes[2]->node_no == n1.node_no)
			 return temp;
		 else if (temp->nodes[0]->node_no == n1.node_no && temp->nodes[1]->node_no == n1.node_no && temp->nodes[2]->node_no == n2.node_no)
			 return temp;
		 else
			 temp = temp->next;
	 }
	 if (temp == NULL)
		 return NULL;
}



/**********************************************************************/
void process(struct element *el,struct node_map *node_map, struct edge_map* emap, int tol){
     //printf("entered process for the element: %d\n",el->ele_no);
     struct element *temp1, *temp2;
     //struct node_map *temp2;
     int i, j, eleno, vertices[3]; 
     int ngAB, ngBC, ngCA;
     if(el->ele_no == -1){// for head element
                   temp1 = el->next;
     }
     else{
                   temp1 = el;
     }
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
                             //printf("The structures are not properly initiated\n");
                             exit(0);
                 }
     //length calculation begins
     std::cout << "Calling calc_length AB for element " << temp1->ele_no << std::endl;
     length = calc_length(vertexA, vertexB);
     std::cout << "Length AB is " << length << std::endl;
     if (length >= tol){
                std::cout << "Entered AB " << std::endl;
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
    	        	nw3->next = nw;
    	        	std::cout << "Inserted new elements " << nw->ele_no << " " << nw2->ele_no << " " << nw3->ele_no << std::endl;
                	temp1 = nw;
                }
                else{
                	temp1->nodes[1]  =  num_nodes;
                	nw->next  =  temp1->next;
                	std::cout << "Inserted new element " << nw->ele_no << std::endl;
                	temp1 = nw;
                }
                process(temp1,node_map, emap, tol);
     }
    
    std::cout << "Calling calc_length BC for element " << temp1->ele_no << std::endl;
    length = calc_length(vertexB,vertexC);
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
  	  		nw3->next = nw;
  	  		std::cout << "Inserted new elements " << nw->ele_no << " " << nw2->ele_no << " " << nw3->ele_no << std::endl;
  	  		temp1  =  nw;
  	  	}
  	  	else{
  	  		temp1->nodes[2]  =  num_nodes;
  	  		nw->next  =  temp1->next;
  	  		temp1  =  nw;
  	  	}
  	  	process(temp1,node_map, emap, tol);
    }
    std::cout << "Calling calc_length CA for element " << temp1->ele_no << std::endl;
    length  =  calc_length(vertexC,vertexA);
    if(length > tol){ //   printf("91\n");
    		midCA  =  new REAL[2];
		midCA  =  compute_mid(vertexC,vertexA); 
		num_nodes++;
		map_setnode(node_map,num_nodes,midCA);
  	
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
   			nw3->next = nw;
   			std::cout << "Inserted new elements " << nw->ele_no << " " << nw2->ele_no << " " << nw3->ele_no << std::endl;
        		temp1 = nw->next;
        	}
        	else{
        		temp1->nodes[0] = num_nodes;
        		nw->next = temp1->next;
        		std::cout << "Inserted new element " << nw->ele_no << std::endl;
        		temp1 = nw->next;
        	}
        	process(temp1,node_map, emap, tol);
    }
    std::cout << "All lengths under limit, going for next" << std::endl;
    temp1 = temp1->next;
 }
//processing ends here
}
/*********************************************************************/
void copy_nodes(int src[3],int dest[3]){
 int i;
 for(i = 0;i<3;i++){
  dest[i] = src[i];
 }
}
/*********************************************************************/
// This is very basic sorting, it requires improvement later
struct element *sort_list(struct element *head) {
    struct element *tmpPtr  =  head->next;
    struct element *tmpNxt  =  tmpPtr->next;
    int tmp;
    int tmp_nodes[3];
    while(tmpNxt !=   NULL){
           while(tmpNxt !=   tmpPtr){
                    if(tmpNxt->ele_no < tmpPtr->ele_no){
                            tmp  =  tmpPtr->ele_no;
                            copy_nodes(tmpPtr->nodes,tmp_nodes);
                           // tmp_nodes = tmpPtr->nodes;
                            tmpPtr->ele_no  =  tmpNxt->ele_no;
			    copy_nodes(tmpNxt->nodes,tmpPtr->nodes);
			    //tmpPtr->nodes = tmpNxt->nodes;
                            tmpNxt->ele_no  =  tmp;
			    copy_nodes(tmp_nodes,tmpNxt->nodes);
			   // tmpNxt->nodes = tmp_nodes;
                    }
                    tmpPtr  =  tmpPtr->next;
            }
            tmpPtr  =  head;
            tmpNxt  =  tmpNxt->next;
    }
         return tmpPtr ; // Place holder
}  

/**********************************************************************/
void write_elements(std::string rt,struct element *el){
	std::ofstream fp; 
	fp.open((rt + ".1.ele"), std::ios::out);
	struct element *temp;
	temp = el->next;
	std::cout << "Write_elements is called" << std::endl;
	fp << num_ele << " " << 3; 
	while(temp!=  NULL){
		fp << temp->ele_no << " " << temp->nodes[0] << " " << temp->nodes[1] << " " << temp->nodes[2] << std::endl;
     		temp = temp->next;
	}
	fp.close(); //fclose(fp);
	std::cout << "Finished writing elements" << std::endl; //printf("finished writing elements\n");
}
/***********************************************************************/
void write_nodes(std::string rt,struct node_map *m){
	std::cout << "write_nodes is called" << std::endl;
	std::ofstream fp;
	struct node_map *temp = m;
	fp.open((rt + ".1.node"),std::ios::out); 
	int i;
	fp << num_nodes << 2 << std::endl; 
	while(temp!=  NULL){
	 	fp << temp->point << " " << temp->xandy[0] << " " << temp->xandy[1] << std::endl;
		//fprintf(fp," %d %f %f\n",temp->point,temp->xandy[0],temp->xandy[1]);//verts[1]);
	 	temp = temp->nxt;  
	}
	fp.close();
	std::cout <<  "Finished writing nodes" << std::endl;
}
/************************************************************************/
int main(int argc, char **argv)
{   
	struct amrgeo *m;
	int i,j,nele,nnode,node_no;
	std::string filename;
	std::ifstream fnode, fele;
	REAL vertices[2];
	int nbs[3];
	m = new amrgeo();
	head_ele = new element();
	head_node = new node();
	head_ele->next = NULL;
	head_ele->ele_no = -1;
	head_node->next = NULL;
	struct	node_map *nodes;
	struct edge_map *emap;
    	std::cout << "Please input the root of the file names without any extension" << std::endl;
 	std::cin >> filename;
 	m->elefilename = (filename + ".ele");
 	m->nodefilename = (filename + ".node");
    	std::cout << "The file names are: " << m->elefilename << " and " << m->nodefilename << std::endl;
    	fele.open(m->elefilename,std::ios::in);
	fele >> nele; 
	emap = new edge_map();
	//emap = create_edgemap();
	std::cout << "0" << std::endl;
	for( int i = 1; i <= nele; i++){
	     struct element *temp;
	     temp  =  new element();
	     temp->next  =  NULL;
	     fele >> temp->ele_no >> temp->nodes[0] >> temp->nodes[1] >> temp->nodes[2];
	     insert_element(temp); 	
	     set_nedgemap(emap,temp->nodes);
	}
	std::cout << "Reading elements completed" << std::endl;
	display_elements(head_ele);
	
	/* following code reads node file and creates node map. */
	fnode.open(m->nodefilename,std::ios::in);
	std::cout << "1" << std::endl;
	/* creating a map of nodes */
	nodes  =  create_nodemap();
	fnode >> nnode;
	std::cout << "# of nodes are " << nnode << std::endl;
	num_nodes  =  nnode;
	for(int i = 1; i <= nnode; i++){
 		fnode >> node_no >> vertices[0] >> vertices[1];
                map_setnode(nodes, node_no, vertices);
	}
	std::cout << "Reading nodes is done" << std::endl;
	process(head_ele, nodes, emap, 10.0);
	std::cout << "Done processing" << std::endl;
	fnode.close();
	fele.close();
	/*std::cout << "Printing node map" << std::endl;
	display_map(nodes);
	std::cout << " Process method is going to be called " << std::endl;
	
	std::cout << "Processing finished, # of elements are:" << num_ele << " # of nodes are" << num_nodes << std::endl;
     	std::cout << "Displaying elemets after processing" << std::endl;
     	display_elements(head_ele);
	std::cout << "5\n";
	//std::cout << "Displaying nodes after processing" << std::endl;
     	// display_map(nodes);
	
     	head_ele  =  sort_list(head_ele);
	std::cout << "Done the sorting" << std::endl;
     	//display_elements(head_ele);
	write_elements(filename,head_ele);
	write_nodes(filename,nodes);
	std::cout << "Writing to files successful" << std::endl;*/
	return 0;
}  
