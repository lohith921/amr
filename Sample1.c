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
#include "mmap_lib.h"
/* Maximum number of characters in a file name (including the null).*/
#define FILENAMESIZE 2048
#define INPUTLINESIZE 1024

/* Structure to hold properties */
struct amrgeo {
int nodeindi; // nodefile indicator
int eleindi; // element file indicator
int neighbourindi;// neighbor file indicator
char elefilename[FILENAMESIZE];
char nodefilename[FILENAMESIZE];
char nieghborfilename[FILENAMESIZE];
REAL minlength;// min length could be passed from command line
};

static int num_ele=0;// to keep track of total no of elements
static int num_nodes=0;// to keep track of total no of nodes
/* structure for elements*/
struct element{
	int ele_no;
	int nodes[3];
	struct element *next;
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

/*********************************************************************/ 
void info(){
printf("This program is used to refine the mesh using edge bisection.");
printf("This program uses an .ele, a .node and a .neigh file.");
printf("Usage shall be\n");
printf("amrgeo -enp <filename>\n");
}
/*********************************************************************/
void syntax(char *pname){
  printf("%s [-en] input_file\n",pname);
  printf("    -e  element file indication.\n");
  printf("    -n  node file indication\n");
  exit(0);
}
/*********************************************************************/
// following function is used to read a line from the file
char *readline(char *string, FILE *infile, char *infilename){
  char *result;
  do {
    result = fgets(string, INPUTLINESIZE, infile);
    if (result == (char *) NULL) {
      printf("Error:  Unexpected end of file in %s.\n",
             infilename);
      exit(1);
    }
    while ((*result != '\0') && (*result != '#')
           && (*result != '.') && (*result != '+') && (*result != '-')
           && ((*result < '0') || (*result > '9'))) {
      result++;
    }
  } while ((*result == '#') || (*result == '\0'));
  return result;
}
/*******************************************************************/
char *findfield(char *string){
  char *result;
  result = string;
  while ((*result != '\0') && (*result != '#')
         && (*result != ' ') && (*result != '\t')) {
    result++;
  }
  while ((*result != '\0') && (*result != '#')
         && (*result != '.') && (*result != '+') && (*result != '-')
         && ((*result < '0') || (*result > '9'))) {
    result++;
  }
  if (*result == '#') {
    *result = '\0';
  }
  return result;
}
/*********************************************************************/
void display_elements(struct element *t){
	struct element *temp;
	temp=t->next;
	if(temp==NULL){
		printf("there are no elements in the linked list\n");
		exit(0);	
	}
	else{
		while(temp!=NULL){
			printf("Element #: %d, nodes are: %d %d %d\n",temp->ele_no,temp->nodes[0],temp->nodes[1],temp->nodes[2]);
			temp=temp->next;
		}
	}
}
/********************************************************************/
void insert_element(struct element *t){
	struct element *temp;
	temp=(struct element *)malloc(sizeof(struct element *));
	temp->next=NULL;
	if(head_ele->next==NULL){
		head_ele->next=t;
	}
	else{
		temp=head_ele->next;
		while(temp->next!=NULL){
			temp=temp->next;
		}
		temp->next=t;
	}
	num_ele++;
}
/*******************************************************************/
REAL * compute_mid(REAL *vA, REAL *vB){
 REAL *new;
 new=(REAL *)malloc(2*sizeof(REAL));
 new[0]=(vA[0]+vB[0])/2;
 new[1]=(vA[1]+vB[1])/2;
 return new;
}
/*******************************************************************/
REAL calc_length(REAL *A, REAL *B){
//printf("calc_length is called\n");
REAL d1,d2,d;
//printf("The vertices are x1=%f, y1=%f, x2=%f, y2=%f\n",A[0],A[1],B[0],B[1]);
d1=fabs((A[0]-B[0]));
d2=fabs((A[1]-B[1]));
REAL temp=(d1*d1+d2*d2);
//printf("d1= %f, d2= %f, sum of squares: %f\n",d1,d2,temp);
d=sqrt(temp);
return d;
}
/***********************************************************************/
void print_elements(struct element *e){
printf("#: %d, Nodes %d, %d, %d\n",e->ele_no,e->nodes[0],e->nodes[1],e->nodes[2]);
}
/**********************************************************************/
void process(struct element *el,struct map_t *node_map, int tol){
 //printf("entered process for the element: %d\n",el->ele_no);
  struct element *temp1;
  //struct map_t *temp2;
  int i,j,eleno,vertexes[3]; 
  if(el->ele_no==-1){// for head element
   temp1=el->next;
  }
  else{
   temp1=el;
  }
  //temp2=node_map;
  //eleno=temp1->ele_no;
  REAL *vertexA,*vertexB,*vertexC;
  REAL *midAB, *midBC, *midCA, length;
  vertexA=(REAL *)malloc(2*sizeof(REAL));
  vertexB=(REAL *)malloc(2*sizeof(REAL));
  vertexC=(REAL *)malloc(2*sizeof(REAL));
  while(temp1!=NULL){
  
  if(node_map!=NULL){
   vertexA=map_get(node_map,temp1->nodes[0]);
   vertexB=map_get(node_map,temp1->nodes[1]);
   vertexC=map_get(node_map,temp1->nodes[2]);
  }
  else{
   printf("The structures are not properly initiated\n");
   exit(0);
  }
  //length calculation begins
  //printf("calling calc_length AB for element %d\n",temp1->ele_no);
  struct element *new;
  length=calc_length(vertexA,vertexB);
   if(length >= tol){
    // printf("89\n");
    new=(struct element *)malloc(sizeof(struct element));
    new->next = NULL;
    new->ele_no = 0;
    for(i=0;i<3;i++){
      new->nodes[i] = 0;
    }
    midAB = (REAL *)malloc(2*sizeof(REAL));
    midAB = compute_mid(vertexA,vertexB);
    num_nodes++;
    map_set(node_map,num_nodes,midAB);
    new->ele_no = ++num_ele;
    new->nodes[0] = num_nodes;
    new->nodes[1] = temp1->nodes[1];
    new->nodes[2] = temp1->nodes[2];
    temp1->nodes[1] = num_nodes;
    new->next = temp1->next;
    temp1->next = new;//new triangles A,mid,C and mid,B,C are formed
    printf("inserted new element %d\n",new->ele_no);
    // We have inserted a node, but we have to check for conformity also
    
    temp1 = new;
   }
  //printf("calling calc_length BC for element %d \n",temp1->ele_no);
  length = calc_length(vertexB,vertexC);
  if(length>tol){
  // printf("90\n");
   midBC = (REAL *)malloc(2*sizeof(REAL));
   new = (struct element *)malloc(sizeof(struct element));
   new->next=NULL;
   new->ele_no=0;
   for(i=0;i<3;i++){
     new->nodes[i]=0;
   }
   midBC=compute_mid(vertexB,vertexC);
   num_nodes++;
   map_set(node_map,num_nodes,midBC);
   new->ele_no=++num_ele;
   new->nodes[0]=temp1->nodes[0];
   new->nodes[1]=num_nodes;
   new->nodes[2]=temp1->nodes[2];
   temp1->nodes[2]=num_nodes;
   new->next=temp1->next;
   temp1->next=new;//new triangles A,B,mid and mid,C,A are formed
   printf("inserted new element %d\n",new->ele_no);
   temp1=temp1->next;
  }
 // printf("calling calc_length CA for element %d\n",temp1->ele_no);
  length=calc_length(vertexC,vertexA);
  if(length>tol){
//   printf("91\n");
   midCA=(REAL *)malloc(2*sizeof(REAL));
   new=(struct element *)malloc(sizeof(struct element));
   new->next=NULL;
   new->ele_no=0;
   for(i=0;i<3;i++){
     new->nodes[i]=0;
   }
   midCA=compute_mid(vertexC,vertexA); 
   num_nodes++;
   map_set(node_map,num_nodes,midCA);
   new->ele_no=++num_ele;
   new->nodes[0]=num_nodes;
   new->nodes[1]=temp1->nodes[1];
   new->nodes[2]=temp1->nodes[2];
   temp1->nodes[2]=num_nodes;
   new->next=temp1->next;
   temp1->next=new;//new triangles A,B,mid and mid,B,C are formed
   printf("inserted new element %d \n",new->ele_no);
   temp1=temp1->next;
  }
 // printf("All lengths under limit, going for next\n");
  temp1=temp1->next;
 }
//processing ends here
}
/*********************************************************************/
void copy_nodes(int src[3],int dest[3])
{
 int i;
 for(i=0;i<3;i++){
  dest[i]=src[i];
 }
}
/*********************************************************************/
// This is very basic sorting, it requires improvement later
struct element *sort_list(struct element *head) {
    struct element *tmpPtr = head->next;
    struct element *tmpNxt = tmpPtr->next;
    int tmp;
    int tmp_nodes[3];
    while(tmpNxt != NULL){
           while(tmpNxt != tmpPtr){
                    if(tmpNxt->ele_no < tmpPtr->ele_no){
                            tmp = tmpPtr->ele_no;
                            copy_nodes(tmpPtr->nodes,tmp_nodes);
                           // tmp_nodes=tmpPtr->nodes;
                            tmpPtr->ele_no = tmpNxt->ele_no;
			    copy_nodes(tmpNxt->nodes,tmpPtr->nodes);
			    //tmpPtr->nodes=tmpNxt->nodes;
                            tmpNxt->ele_no = tmp;
			    copy_nodes(tmp_nodes,tmpNxt->nodes);
			   // tmpNxt->nodes=tmp_nodes;
                    }
                    tmpPtr = tmpPtr->next;
            }
            tmpPtr = head;
            tmpNxt = tmpNxt->next;
    }
         return tmpPtr ; // Place holder
}  

/**********************************************************************/
void write_elements(char *rt,struct element *el){
 FILE *fp;
 char *t;
 t=(char *)malloc(sizeof(rt));
 strcpy(t,rt);
 fp=fopen(strcat(t,".1.ele"),"w"); 
 struct element *temp;
 temp=el->next;
// printf("write_elements is called\n");
 fprintf(fp,"%d %d \n",num_ele,3);
 while(temp!=NULL){
  //printf("writing element %d\n",temp->ele_no);
  fprintf(fp," %d %d %d %d\n",temp->ele_no,temp->nodes[0],temp->nodes[1],temp->nodes[2]);
  temp=temp->next;
 }
 fclose(fp);
 printf("finished writing elements\n");
}
/***********************************************************************/
void write_nodes(char *rt,struct map_t *m){
// printf("write_nodes is called\n");
 FILE *fp;
 char *t;
 t=(char *)malloc(sizeof(rt));
 strcpy(t,rt);
 struct map_t *temp=m;
 fp=fopen(strcat(t,".1.node"),"w"); 
 int i;
 //REAL *verts;
 //verts=(REAL *)malloc(2*sizeof(REAL));
 fprintf(fp,"%d %d \n",num_nodes,2); 
// printf("printed first line\n");
 //for(i=1;i<=num_nodes;i++){
  //verts=(REAL *)malloc(2*sizeof(REAL));
  //verts=map_get(m,i);
  //if(verts!=NULL)
  while(temp!=NULL){
   fprintf(fp," %d %f %f\n",temp->point,temp->xandy[0],temp->xandy[1]);//verts[1]);
   temp=temp->nxt;  
  }
  //else{
   //printf("map_get returned NULL\n");
   //exit(0); 
 // }
 //}
  fclose(fp);
  printf("finished writing nodes\n");
}
/************************************************************************/
int main(int argc, char **argv)
{
// printf("0\n");
 struct amrgeo *m;
 int i,j,nele,nnode,node_no;
 char filename[INPUTLINESIZE];
 char *firstline,*token;
 char *delim=" ";
 char data[200];
 char *root;
 //root[0]='\0'; 
 data[0]='\0';
 filename[0]='\0';
// printf("0.25\n");
 REAL vertices[2];
 m=(struct amrgeo *)malloc(sizeof(struct amrgeo));
 m->nodefilename[0]='\0';
 m->elefilename[0]='\0';
 m->neighborfilename[0]='\0';
 //printf("0.35\n");
 FILE *fnode, *fele, *fneigh;
 
 head_ele=(struct element *)malloc(sizeof(struct element));
 head_node=(struct node *)malloc(sizeof(struct node));

 head_ele->next=NULL;
 head_ele->ele_no=-1;
 head_node->next=NULL;
 //printf("0.5\n");
 struct map_t *nodes;
 
 //parsing the arguments//
 //printf("1\n"); 
 for (i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      for (j = 1; argv[i][j] != '\0'; j++) {
        if (argv[i][j] == 'e') {
          m->eleindi = 1;
	 	}
        else if (argv[i][j] == 'n') {
          m->nodeindi = 1;
	 	}
	 	else if (argv[i][j] == 'p') {
          m->neighborindi = 1;
	 	}
        else {
          info();
	 	}
      }// inner for loop ends here
    } 
    else {
      strcpy(filename, argv[i]);
    }
 }// for loop ends here
 if (filename[0] == '\0') {
   syntax(argv[0]);
 }
 
 //printf("&filename[strlen(filename) - 5] is %s\n",&filename[strlen(filename) - 5]);
 if(!strcmp(&filename[strlen(filename) - 5], ".node")) { //printf("2\n"); 
   strcpy(m->nodefilename,filename);
   memcpy(m->elefilename,&filename,(strlen(filename)-5)); //  strcpy(root,m->elefilename);  
   strcat(m->elefilename,".ele");
   m->elefilename[strlen(m->elefilename)+1]='\0';
   memcpy(m->neighborfilename, &filename, (strlen(filename)-5));
   strcat(m->neighborfilename,".neigh");
   m->neighborfilename[strlen(m->neighborfilename)+1]='\0';
   filename[strlen(filename) - 5] = '\0';
   
 }
 else if(!strcmp(&filename[strlen(filename) - 4], ".ele")) {
  // printf("2.5\n");
   strcpy(m->elefilename,filename);
   memcpy(m->nodefilename,&filename,(strlen(filename)-4));
   //strcpy(root,m->nodefilename);
   strcat(m->nodefilename,".node");  
   m->nodefilename[strlen(m->nodefilename)+1]='\0';
   memcpy(m->neighborfilename, &filename,(strlen((filename)-4));
   strcat(m->neighborfilename,".neigh");
   m->neighborfilename[strlen(m->neighbourfilename)+1] = '\0';
   filename[strlen(filename) - 4] = '\0';
 }
 else if(!strcmp(&filename[strlen(filename) - 6], ".neigh")) {
  // printf("2.5\n");
   strcpy(m->neighborfilename,filename);
   memcpy(m->nodefilename,&filename,(strlen(filename)-6));
   //strcpy(root,m->nodefilename);
   strcat(m->nodefilename,".node");  
   m->nodefilename[strlen(m->nodefilename)+1]='\0';
   memcpy(m->elefilename, &filename,(strlen((filename)-6));
   strcat(m->elefilename,".neigh");
   m->elefilename[strlen(m->nelefilename)+1] = '\0';
   filename[strlen(filename) - 4] = '\0';
 }
 else{
 	info();
 }
 printf("The file names are: %s and %s\n", m->elefilename, m->nodefilename.m->neighborfile);
 root=(char *)malloc(sizeof(filename));
 root[0]='\0';
 strcpy(root,filename);
 fele=fopen(m->elefilename,"r");
 //while(fnode!=NULL){
 /*do{
 	printf("%s\n",data);
 // }while(fscanf(fnode,"%s",data)!=EOF);
 }while(fgets(data,200,fnode)!=NULL);*/
 
 //following code is to read .ele file and create linked list of elements
 firstline=readline(data,fele,m->elefilename);
 nele = atoi(strtok(firstline,delim)); 
// printf("2.75\n");
 for(i=1;i <= nele;i++){
 	struct element *temp;
 	temp=(struct element *)malloc(sizeof(struct element));
 	temp->next=NULL;
  	firstline = readline(data,fele,m->elefilename);
       // printf("first line is %s\n",firstline);
 	token = strtok(firstline,delim);
 	temp->ele_no = atoi(token);
 	j=0;
 	while(token!=NULL){
 		token = strtok(NULL,delim);
		if(j!=3)
 		 temp->nodes[j++] = atoi(token);
 	 }
	 insert_element(temp); 	
//printf("insert_element\n");
 }
//printf("about to display the elements\n");
// display_elements(head_ele);

 fnode=fopen(m->nodefilename,"r");
// printf("3\n");
 // creating a map of nodes
 nodes=map_create();

 firstline=readline(data,fnode,m->nodefilename);
 nnode=atoi(strtok(firstline,delim)); 
 //printf("no of nodes are %d\n",nnode);
 num_nodes=nnode;
 for(i=1;i<=nnode;i++){
 	firstline=readline(data,fnode,m->nodefilename);
 	token=strtok(firstline,delim);
       // printf("token extracted is %s\n",token);
 	node_no=atoi(token);
 	j=0;
 	while(token!=NULL){
 		token=strtok(NULL,delim);
	        if(token!=NULL){
                 // printf("token value again is %s\n",token);
 		  vertices[j++]=atof(token);
		  //printf("vertices of j-1 is %f\n",vertices[j-1]);
		}
 	 }
 //printf("vertices array is %f %f\n",vertices[0],vertices[1]);
 map_set(nodes,node_no,vertices);
 }
 /* Write code for reading neigh file and create a neighbor map
 
 */
 //display_map(nodes);
 // printf("going to call process method\n");
 process(head_ele,nodes,0.9);
 printf("processing finished no of elements are: %d no of nodes are %d\n",num_ele,num_nodes);
 printf("Displaying elements after processing\n");
 display_elements(head_ele);
 // printf("5\n");
 printf("displaying nodes after processing\n");
 display_map(nodes);
 fclose(fnode);
 fclose(fele);
 printf("files successfully opened\n");
 head_ele=sort_list(head_ele);
 printf("done the sorting:\n");
 //display_elements(head_ele);
 write_elements(root,head_ele);
 write_nodes(root,nodes);
 printf("writing to files successful\n");
 return 0;
}
