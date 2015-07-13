/* This program takes input a .ele file or a .node files with switches -e and -n resp*/
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
struct amrgo {
int nodeindi;//nodefile indicator
int eleindi;//element file indicator
char elefilename[FILENAMESIZE];
char nodefilename[FILENAMESIZE];
REAL minlength;
};
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
printf("This program uses an .ele file and a .node file.");
printf("Usage shall be\n");
printf("amrgeo -en <filename>\n");
}
/*********************************************************************/
void syntax(){
  printf("amrgeo [-en] input_file\n");
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
}
/***********************************************************************/
void process(struct element *el,struct map_t *node_map, int tol){
 struct element *temp;
 int i,j,ele_no,vertexes[3];
 REAL *vertexA,*vertexB,*vertexC;
 REAL *midAB, *midBC, *midCA, length;

 temp=el->next;
 if(temp!=NULL){
  

}
/*********************************************************************/
int main(int argc, char **argv)
{
 printf("0\n");
 struct amrgo *m;
 int i,j,nele,nnode,node_no;
 char filename[INPUTLINESIZE];
 char *firstline,*token;
 char *delim=" ";
 char data[200];
 data[0]='\0';
 filename[0]='\0';
 printf("0.25\n");
 REAL vertices[2];
 m=(struct amrgo *)malloc(sizeof(struct amrgo));
 m->nodefilename[0]='\0';
 m->elefilename[0]='\0';
 printf("0.35\n");
 FILE *fnode, *fele;
 
 head_ele=(struct element *)malloc(sizeof(struct element));
 head_node=(struct node *)malloc(sizeof(struct node));

 head_ele->next=NULL;
 head_node->next=NULL;
 printf("0.5\n");
 struct map_t *nodes;
 
 //parsing the arguments//
 printf("1\n"); 
 for (i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      for (j = 1; argv[i][j] != '\0'; j++) {
        if (argv[i][j] == 'e') {
          m->eleindi = 1;
	 	}
        else if (argv[i][j] == 'n') {
          m->nodeindi = 1;
	 	}
        else {
          info();
	 	}
      }
    } 
    else {
      strcpy(filename, argv[i]);
    }
 }
 if (filename[0] == '\0') {
   syntax();
 }
 printf("2\n");
 //printf("&filename[strlen(filename) - 5] is %s\n",&filename[strlen(filename) - 5]);
 if(!strcmp(&filename[strlen(filename) - 5], ".node")) {
   strcpy(m->nodefilename,filename);
   memcpy(m->elefilename,&filename,(strlen(filename)-5));
   strcat(m->elefilename,".ele");
   m->elefilename[strlen(m->elefilename)+1]='\0';
   filename[strlen(filename) - 5] = '\0';
 }
 else if(!strcmp(&filename[strlen(filename) - 4], ".ele")) {
   strcpy(m->elefilename,filename);
   memcpy(m->nodefilename,&filename,(strlen(filename)-4));
   strcat(m->nodefilename,".node");  
   m->nodefilename[strlen(m->nodefilename)+1]='\0';
   filename[strlen(filename) - 4] = '\0';
 }
 else{
 	info();
 }
 printf("The file names are: %s and %s\n", m->elefilename, m->nodefilename);

 /*fele=fopen(m->elefilename,"r");
 //while(fnode!=NULL){
 /*do{
 	printf("%s\n",data);
 // }while(fscanf(fnode,"%s",data)!=EOF);
 }while(fgets(data,200,fnode)!=NULL);*/
 
 //following code is to read .ele file and create linked list of elements
 
/* firstline=readline(data,fele,m->elefilename);
 nele=atoi(strtok(firstline,delim)); 
 for(i=1;i<=nele;i++){
 	struct element *temp;
 	temp=(struct element *)malloc(sizeof(struct element));
 	temp->next=NULL;
  	firstline=readline(data,fele,m->elefilename);
 	token=strtok(firstline,delim);
 	temp->ele_no=atoi(token);
 	j=0;
 	while(token!=NULL){
 		token=strtok(NULL,delim);
 		temp->nodes[j++]=atoi(token);
 	 }
	 insert_element(temp); 	
 }
 display_elements(head_ele);*/

 fnode=fopen(m->nodefilename,"r");
// printf("3\n");
 // creating a map of nodes
 nodes=map_create();

 firstline=readline(data,fnode,m->nodefilename);
 nnode=atoi(strtok(firstline,delim)); 
 printf("no of nodes are %d\n",nnode);
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
 printf("4\n");
 display_map(nodes);
 fclose(fnode);
 fclose(fele);
 
 return 0;
}
