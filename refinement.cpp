/* This program takes input a .ele file or a .node files and a .neigh file with switches -e, -n and -p resp*/
/* There is a small bug, the name of the file without extension should be 3 or more characters length*/
#ifdef SINGLE
#define REAL float
#else
#define REAL double
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include "mmap_lib_p.h"
#include "structures.h"
/* Maximum number of characters in a file name (including the null).*/

 static int num_ele  =  0;// to keep track of total no of elements
 static int num_nodes = 0;// to keep track of total no of nodes

/*********************************************************************/ 
 void info(){
      printf("This program is used to refine the mesh using edge bisection.");
      printf("This program requires a root name of the .node and .ele files");
      printf("Usage shall be\n");
      printf("amrgeo -en <root name>\n");
 }
/*********************************************************************/
 void syntax(char *pname){
      printf("%s [-en] input_file\n",pname);
      printf("    -e  element file indication.\n");
      printf("    -n  node file indication\n");
      exit(0);
      }
/*********************************************************************/
void display_elements(struct element *t){
	struct element *temp;
	temp = t->next;
	if(temp == NULL){
		printf("there are no elements in the linked list\n");
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
	nw = (REAL *)malloc(2*sizeof(REAL));
	nw[0] = (vA[0]+vB[0])/2;
	nw[1] = (vA[1]+vB[1])/2;
	return nw;
}
/*******************************************************************/
REAL calc_length(REAL *A, REAL *B){
	//printf("calc_length is called\n");
	REAL d1,d2,d;
	//printf("The vertices are x1 = %f, y1 = %f, x2 = %f, y2 = %f\n",A[0],A[1],B[0],B[1]);
	d1 = fabs((A[0]-B[0]));
	d2 = fabs((A[1]-B[1]));
	REAL temp = (d1*d1+d2*d2);
	//printf("d1 =  %f, d2 =  %f, sum of squares: %f\n",d1,d2,temp);
	d = sqrt(temp);
	return d;
}
/***********************************************************************/
void print_elements(struct element *e){
	printf("#: %d, Nodes %d, %d, %d\n",e->ele_no,e->nodes[0],e->nodes[1],e->nodes[2]);
}
/**********************************************************************/
void process(struct element *el,struct node_map *node_map, struct edge_map* emap, int tol){
     //printf("entered process for the element: %d\n",el->ele_no);
     struct element *temp1;
     //struct node_map *temp2;
     int i, j, eleno, vertices[3]; 
     int ngAB, ngBC, ngCA;
     if(el->ele_no == -1){// for head element
                   temp1 = el->next;
     }
     else{
                   temp1 = el;
     }
  //temp2 = node_map;
  //eleno = temp1->ele_no;
     REAL *vertexA,*vertexB,*vertexC;
     REAL *midAB, *midBC, *midCA, length;
     vertexA = (REAL *)malloc(2*sizeof(REAL));
     vertexB = (REAL *)malloc(2*sizeof(REAL));
     vertexC = (REAL *)malloc(2*sizeof(REAL));
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
     //printf("calling calc_length AB for element %d\n",temp1->ele_no);
     length = calc_length(vertexA, vertexB);
     if (length >= tol){
                // printf("89\n");
                // ngb has the node numbers of nodes opposite to edge AB.
                std::vector<int> ngb = edgemap_getnodes(emap, temp1->nodes[0], temp1->nodes[1]);
                if(ngb[0] == temp1->nodes[2])
                          ngAB = ngb[1];
                else
    		              ngAB = ngb[0];
                midAB  =  new REAL[2]; //(REAL *)malloc(2*sizeof(REAL));
                midAB  =  compute_mid(vertexA, vertexB);
                num_nodes++;
                map_setnode(node_map,num_nodes,midAB);
    	        //nw = (struct element *)malloc(sizeof(struct element));
    	        nw = new element();
    	        nw->ele_no  =  ++num_ele;
    	        nw->nodes[0]  =  num_nodes;
    	        nw->nodes[1]  =  temp1->nodes[1];
    	        nw->nodes[2]  =  temp1->nodes[2];
    	        set_nedgemap(emap,nw->nodes);
    	
    	        nw2 = new element();
    	        nw2->ele_no  =  ++num_ele;
    	        nw2->nodes[0]  = temp1->nodes[0];
    	        nw2->nodes[1]  = ngAB;
    	        nw2->nodes[2]  = num_nodes;
    	        set_nedgemap(emap,nw2->nodes);
    	
    	        nw3 = new element();
    	        nw3->ele_no  =  ++num_ele;
    	        nw3->nodes[0]  = num_nodes;
    	        nw3->nodes[1]  = ngAB;
    	        nw3->nodes[2]  = temp1->nodes[1];
    	        set_nedgemap(emap,nw3->nodes);
    	    	
    	        temp1->nodes[1]  =  num_nodes;
    	        nw->next  =  temp1->next;
    	        temp1->next  =  nw2;//new triangles A,mid,C and mid,B,C are formed
    	        nw2->next = nw3;
    	        nw3->next = nw;
    	        std::cout << "Inserted new elements " << nw->ele_no << " " << nw2->ele_no << " " << nw3->ele_no << std::endl;
                //printf("inserted new elements %d %d %d\n",nw->ele_no, nw2->ele_no, nw3->ele_no);
                temp1 = nw;
                process(temp1,node_map, emap, tol);
    }
    
    //printf("calling calc_length BC for element %d \n",temp1->ele_no);
    length = calc_length(vertexB,vertexC);
    if(length > tol){
              // printf("90\n");
              midBC  =  new REAL[2];
              midBC  =  compute_mid(vertexB,vertexC);
              num_nodes++;
  	map_setnode(node_map, num_nodes, midBC);
  	
  	std::vector<int> ngb = edgemap_getnodes(emap, temp1->nodes[1], temp1->nodes[2]);
    	if(ngb[0] == temp1->nodes[0])
    		ngBC = ngb[1];
    	else
    		ngBC = ngb[0];
  	
  	nw  =  new element();
  	nw->ele_no  =  ++num_ele;
  	nw->nodes[0]  =  temp1->nodes[0];
  	nw->nodes[1]  =  num_nodes;
  	nw->nodes[2]  =  temp1->nodes[2];
  	set_nedgemap(emap,nw->nodes);
  	
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
      //printf("inserted new elements %d %d %d\n",nw->ele_no, nw2->ele_no, nw3->ele_no);
  	temp1  =  nw;
  	process(temp1,node_map, emap, tol);
   }
 // printf("calling calc_length CA for element %d\n",temp1->ele_no);
  length  =  calc_length(vertexC,vertexA);
  if(length > tol){
	//   printf("91\n");
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
       //printf("inserted new elements %d %d %d\n",nw->ele_no, nw2->ele_no, nw3->ele_no);
  	temp1 = nw->next;
  	process(temp1,node_map, emap, tol);
  }
 // printf("All lengths under limit, going for next\n");
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
void write_elements(char *rt,struct element *el){
	ofstream fp; //FILE *fp;
	char *t;
	t = (char *)malloc(sizeof(rt));
	strcpy(t,rt);
	fp.open(strcat(t,".1.ele", ios::out);//fp = fopen(strcat(t,".1.ele"),"w"); 
	struct element *temp;
	temp = el->next;
	// printf("write_elements is called\n");
	fp << num_ele << " " << 3 << std::endl; //fprintf(fp,"%d %d \n",num_ele,3);
	while(temp!=  NULL){
	 //printf("writing element %d\n",temp->ele_no);
	 fp << temp->ele_no << " " << temp->nodes[0] << " " << temp->nodes[1] << " " << temp->nodes[2] << std::endl;
     //fprintf(fp," %d %d %d %d\n",temp->ele_no,temp->nodes[0],temp->nodes[1],temp->nodes[2]);
	 temp = temp->next;
	}
	fp.close(); //fclose(fp);
	std::cout << "Finished writing elements" << std::endl; //printf("finished writing elements\n");
}
/***********************************************************************/
void write_nodes(char *rt,struct node_map *m){
	// printf("write_nodes is called\n");
	ofstream *fp;
	char *t;
	t = (char *)malloc(sizeof(rt));
	strcpy(t,rt);
	struct node_map *temp = m;
	fp.open(strcat(t,".1.node"),ios::out); 
	int i;
	//REAL *verts;
	//verts = (REAL *)malloc(2*sizeof(REAL));
	fp << num_nodes << 2 << std::endl; 
	// printf("printed first line\n");
	//for(i = 1;i <=  num_nodes;i++){
	 //verts = (REAL *)malloc(2*sizeof(REAL));
	 //verts = map_get(m,i);
	 //if(verts!=  NULL)
	 while(temp!=  NULL){
	  fp << temp->point << temp->xandy[0] << temp->xandy[1] << std::endl;
      //fprintf(fp," %d %f %f\n",temp->point,temp->xandy[0],temp->xandy[1]);//verts[1]);
	  temp = temp->nxt;  
	 }
	 //else{
	  //printf("map_get returned NULL\n");
	  //exit(0); 
	// }
	//}
	 fp.close();
	 printf("finished writing nodes\n");
}
/************************************************************************/
int main(int argc, char **argv)
{   // printf("0\n");
	struct amrgeo *m;
	int i,j,nele,nnode,node_no;
	char filename[INPUTLINESIZE];
	char *firstline,*token;
	char *delim;
	//char data[200];
	//char *root;
	//root[0] = '\0'; 
	data[0] = '\0';
	filename[0] = '\0';
	// printf("0.25\n");
	REAL vertices[2];
	int nbs[3];
	m = new amrgeo();
	//m = (struct amrgeo *)malloc(sizeof(struct amrgeo));
	m->nodefilename[0] = '\0';
	m->elefilename[0] = '\0';
	
	//printf("0.35\n");
	//FILE *fnode, *fele;
	ifstream fnode, fele;
	
	head_ele = new element();
	head_node = new node();
	head_ele->next = NULL;
	head_ele->ele_no = -1;
	head_node->next = NULL;
 	//print	f("0.5\n");
	struct	node_map *nodes;
	struct edge_map *emap;
    std::cout << "Please input the root of the file names without any extension" << std::endl;
 	std::cin >> filename;
 	m->elefilename = strcat(filename, ".ele");
 	m->nodefilename = strcat(filename, ".node");
    std::cout << "The file names are: " << m->elefilename " and " << m->nodefilename << std::endl;
    fele.open(m->elefilename,ios::in);
	fele >> nele; 
	emap = create_edgemap();
	// printf("2.75\n");
	for( int i = 1; i <= nele; i++){
	     struct element *temp;
	     temp  =  new element();//(struct element *)malloc(sizeof(struct element));
	     temp->next  =  NULL;
	     fele >> temp->eleno >> temp->nodes[0] >> temp->nodes[1] >> temp->nodes[2] >> std::endl;
	     insert_element(temp); 	
	     set_nedgemap(emap,temp->nodes);
	 }
	//printf("about to display the elements\n");
	// display_elements(head_ele);
	
	/* following code reads node file and creates node map. */
	 fnode.open(m->nodefilename,iso::in);
	// printf("3\n");
	 // creating a map of nodes
	 nodes  =  create_nodemap();
	 //firstline  =  readline(data, fnode, m->nodefilename);
	 fnode >> nnode; //  =  atoi(strtok(firstline,delim)); 
	 //printf("no of nodes are %d\n",nnode);
	 num_nodes  =  nnode;
	 for(int i = 1; i <= nnode; i++){
 	         fnode >> node_no >> vertices[0] >> vertices[1] >> vertices[2] >> std::endl;
             //printf("vertices array is %f %f\n",vertices[0],vertices[1]);
	         map_setnode(nodes,node_no,vertices);
	 }
	 //display_map(nodes);
	 // printf("going to call process method\n");
	 process(head_ele, nodes, emap, 0.9);
	 std::cout << "Processing finished, # of elements are:" << num_ele << " # of nodes are" << num_nodes << std::endl;
     //printf("processing finished no of elements are: %d no of nodes are %d\n",num_ele,num_nodes);
	 std::cout << "Displaying elemets after processing" << std::endl;
     //printf("Displaying elements after processing\n");
	 display_elements(head_ele);
	 // printf("5\n");
	 std::cout << "Displaying nodes after processing" << std::endl;
     //printf("displaying nodes after processing\n");
	// display_map(nodes);
	 fnode.close();
	 fele.close();
     //fclose(fnode);
	 //fclose(fele);
     //printf("files successfully opened\n");
	 head_ele  =  sort_list(head_ele);
	 std::cout << "Done the sorting" << std::endl;
     //printf("done the sorting:\n");
	 //display_elements(head_ele);
	 write_elements(root,head_ele);
	 write_nodes(root,nodes);
	 std::cout << "Writing to files successful" << std::endl;
	 return 0;
	}	
	
	
/* ********************************** Removed code *********************
//printf("insert_element\n");
 /* Write code for reading neigh file and create a neighbor map *
	 fneigh = fopen(m->neighborfilename,"r");
	 // creating a map of neighbors
	 neighbs  =  create_neighbormap();
	 firstline  =  readline(data,fnode,m->neighborfilename);
	 nele  =  atoi(strtok(firstline,delim)); 
	 //printf("no of nodes are %d\n",nnode);
	 //num_eles  =  nele;
	 for(i  =  1;i  <=   nele;i++){
	 	firstline  =  readline(data,fnode,m->neighborfilename);
	 	token  =  strtok(firstline,delim);
	       // printf("token extracted is %s\n",token);
	 	node_no  =  atoi(token);
	 	j  =  0;
	 	while(token !=   NULL){
	 		token  =  strtok(NULL,delim);
		        if(token !=   NULL){
	               nbs[j++]  =  atof(token);
	            }
	 	 }
	    map_setneighbor(neighbs,node_no,nbs);
	 }
	 */
/*firstline  =  readline(data,fele,m->elefilename);
	       // printf("first line is %s\n",firstline);
	 	token  =  strtok(firstline,delim);
	 	temp->ele_no  =  atoi(token);
	 	j = 0;
	 	while(token !=   NULL){
	 		token  =  strtok(NULL,delim);
			if(j !=   3)
	 		 temp->nodes[j++]  =  atoi(token);
	 	 }*/
/* following function is used to read a line from the file
 char *readline(char *string, FILE *infile, char *infilename){
      char *result;
      do {
         result  =  fgets(string, INPUTLINESIZE, infile);
         if (result == (char *) NULL) {
            printf("Error:  Unexpected end of file in %s.\n",
             infilename);
            exit(1);
            }
      while ((*result !=  '\0') && (*result !=  '#')
           && (*result !=  '.') && (*result !=  '+') && (*result !=   '-')
           && ((*result < '0') || (*result > '9'))) {
           result++;
           }
      } while ((*result  ==  '#') || (*result  ==  '\0'));
      return result;
      }*/
/*
 char *findfield(char *string){
      char *result;
      result  =  string;
      while ((*result != '\0') && (*result != '#')
         && (*result != ' ') && (*result != '\t')) {
            result++;
      }
      while ((*result != '\0') && (*result != '#')
         && (*result !=   '.') && (*result != '+') && (*result != '-')
         && ((*result < '0') || (*result > '9'))) {
            result++;
      }
      if (*result == '#') {
         *result = '\0';
         }
      return result;
      } */
//parsing the arguments//
 	//printf("1\n"); 
	/*for (i  =  1; i < argc; i++) {
	    if (argv[i][0]  ==  '-') {
	      for (j  =  1; argv[i][j] !=   '\0'; j++) {
	        if (argv[i][j]  ==  'e')
	          m->eleindi  =  1;
	        else if (argv[i][j]  ==  'n')
	          m->nodeindi  =  1;
	        else
	          info();
		}// inner for loop ends here
	    } 
	    else 
	      strcpy(filename, argv[i]);
	 }// for loop ends here
	 if (filename[0] == '\0')
	   syntax(argv[0]);
 
	 //printf("&filename[strlen(filename) - 5] is %s\n",&filename[strlen(filename) - 5]);
	 if(!strcmp(&filename[strlen(filename) - 5], ".node")) { //printf("2\n"); 
	 	strcpy(m->nodefilename,filename);
		memcpy(m->elefilename,&filename,(strlen(filename)-5)); //  strcpy(root,m->elefilename);  
	 	strcat(m->elefilename,".ele");
	 	m->elefilename[strlen(m->elefilename)+1] = '\0';	   
	 }
	 else if(!strcmp(&filename[strlen(filename) - 4], ".ele")) {
	  // printf("2.5\n");
	 	strcpy(m->elefilename,filename);
	 	memcpy(m->nodefilename,&filename,(strlen(filename)-4));
	 	//strcpy(root,m->nodefilename);
	 	strcat(m->nodefilename,".node");  
	 	m->nodefilename[strlen(m->nodefilename)+1] = '\0';	  
	 }
	 else
	 	info();
	 */
//while(fnode!=  NULL){
	 /*do{
	 	printf("%s\n",data);
	 // }while(fscanf(fnode,"%s",data)!=  EOF);
	 }while(fgets(data,200,fnode)!=  NULL);*/
	 
	 //following code is to read .ele file and create linked list of elements
	 //firstline  =  readline(data,fele,m->elefilename);
 /*root  = new char[sizeof(filename)];
    root[0]  =  '\0';
	 strcpy(root,filename); */
  /*firstline  =  readline(data,fnode,m->nodefilename);
 	         token  =  strtok(firstline,delim);
	         // printf("token extracted is %s\n",token);
	 	     node_no  =  atoi(token);
	 	     j  =  0;
	 	while(token !=   NULL){
	 		token  =  strtok(NULL,delim);
		        if(token !=   NULL){
	                 // printf("token value again is %s\n",token);
	 		  vertices[j++]  =  atof(token);
			  //printf("vertices of j-1 is %f\n",vertices[j-1]);
			}
	 	 }*/
