#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mmap_lib.h"

#ifdef SINGLE
#define REAL float
#else
#define REAL double
#endif
 
#define maxlength 1.0
#define minlength 0.5

#define tol 1.0
/*******************************************************************/
void display_node(struct map_t *m,int pt) {
   printf("displaying node %d: ",pt);
   REAL *result;
   result=(REAL *)malloc(2*sizeof(REAL));
   result=map_get(m,pt);
   if(result==NULL){
    printf("Entry not found in the map\n");
   }
   else{
    printf("%f, %f\n",result[0],result[1]);
   }
}
/*******************************************************************/
struct vertex{
 int no;
 REAL *values;
}; 
/*******************************************************************/
REAL calc_length(REAL *A, REAL *B){
//printf("calc_length is called\n");
REAL d1,d2,d;
printf("The vertices are x1=%f, y1=%f, x2=%f, y2=%f\n",A[0],A[1],B[0],B[1]);
d1=fabs((A[0]-B[0]));
d2=fabs((A[1]-B[1]));
REAL temp=(d1*d1+d2*d2);
//printf("d1= %f, d2= %f, sum of squares: %f\n",d1,d2,temp);
d=sqrt(temp);
return d;
}
/*******************************************************************/
void check_length(REAL *vA, REAL *vB, REAL len, REAL *new){
//printf("check length is called\n");
 new=(REAL *)malloc(2*sizeof(REAL));
 if(len>tol){
  new[0]=(vA[0]+vB[0])/2;
  new[1]=(vA[1]+vB[1])/2;
  printf("The mid point is %f, %f\n\n",new[0],new[1]);
 }
 else{
  printf("mid point need not be calculated\n\n");
 }
}


/*******************************************************************/
void compute_lengths(int ele[][3], struct map_t *node_map, int m){
 /*int segmentA[2], segmentB[2], segmentC[2] */
// printf("compute lengths is called\n");
 int i, j, k;
 REAL *vertexA, *vertexB, *vertexC;
 REAL *midAB, *midBC, *midCA, length;
 int vertexes[3];
FILE *nfp;
nfp=fopen("newNodes.node","w+");
 struct vertex vertices[3];
//printf("size of elements sent is %d\n",sizeof(ele));
 int elements=sizeof(ele)/(3*sizeof(int));
 int x1,y1,x2,y2;
 //printf("1 no of elements are %d\n",elements);
 midAB=(REAL *)malloc(2*sizeof(REAL));
 midBC=(REAL *)malloc(2*sizeof(REAL));
 midCA=(REAL *)malloc(2*sizeof(REAL));
 //printf("2\n");
 for(i=0;i<m;i++){
 vertexes[0]=ele[i][0];
 vertexes[1]=ele[i][1];
 vertexes[2]=ele[i][2];
 vertexA=map_get(node_map, vertexes[0]);
 vertexB=map_get(node_map, vertexes[1]);
 vertexC=map_get(node_map, vertexes[2]);
// printf("3\n");
 length=calc_length(vertexA,vertexB);
 printf("length between %d and %d is %f\n",vertexes[0],vertexes[1],length);
 check_length(vertexA,vertexB,length,midAB);
 fprintf(nfp,"%d %d \n",midAB[0],midAB[1]);
 length=calc_length(vertexB,vertexC);
 printf("length between %d and %d is %f\n",vertexes[1],vertexes[2],length);
 check_length(vertexB,vertexC,length,midBC);
 fprintf(nfp,"%d %d \n",midBC[0],midBC[1]);

 length=calc_length(vertexC,vertexA);
 printf("length between %d and %d is %f\n",vertexes[2],vertexes[0],length);
 check_length(vertexC,vertexB,length,midCA);
 fprintf(nfp,"%d %d \n",midCA[0],midCA[1]);
 }
 //printf("4\n");
}
/*******************************************************************/
int main(){
 int i,j,k,elelength,total_elements;
 total_elements=10;
 int elements[][3]={{1,2,3}, {1,12,3}, {3,4,16}, {16,4,15}, {4,5,15}, {5,14,15}, {5,6,14}, {14,6,8}, {6,7,8}, {13,8,9}};;
 //elements=
// printf("program execution begins\n");
 elelength=sizeof(elements)/(3*sizeof(int));

 //printf("length is %d\n",elelength);
 /*for(i=0;i<elelength;i++){
  printf("For element %d, vertices are",i+1);
  for(j=0;j<3;j++){
   printf(" %d",elements[i][j]);
  }
  printf("\n");
 }*/
 //creating map of node values.
 struct map_t *nodes;
 nodes=map_create();
 REAL temp[2]={2.5,1.5};//values for node 1
 map_set(nodes,1,temp);
 //display_node(nodes,1);
 // values for node 2
 temp[0]=0.5; temp[1]=0.5;
 map_set(nodes,2,temp);
 // values for node 3
 temp[0]=1.0; temp[1]=0.5;
 map_set(nodes,3,temp);
 // values for node 4
 temp[0]=2.0; temp[1]=0.5;
 map_set(nodes,4,temp);
 // values for node 5
 temp[0]=3.0; temp[1]=0.5;
 map_set(nodes,5,temp);
 // values for node 6
 temp[0]=4.0; temp[1]=0.5;
 map_set(nodes,6,temp);
 // values for node 7
 temp[0]=4.5; temp[1]=1.3;
 map_set(nodes,7,temp);
 // values for node 8
 temp[0]=4.1; temp[1]=2.0;
 map_set(nodes,8,temp);
 // values for node 9
 temp[0]=3.0; temp[1]=3.22;
 map_set(nodes,9,temp);
 // values for node 10
 temp[0]=0.78; temp[1]=2.67;
 map_set(nodes,10,temp);
 // values for node 12
 temp[0]=2.0; temp[1]=2.22;
 map_set(nodes,12,temp);
 // values for node 13
 temp[0]=2.87; temp[1]=1.32;
 map_set(nodes,13,temp);
  // values for node 14
 temp[0]=3.0; temp[1]=1.42;
 map_set(nodes,14,temp);
 // values for node 15
 temp[0]=2.33; temp[1]=1.42;
 map_set(nodes,15,temp);
 // values for node 16
 temp[0]=1.5; temp[1]=1.42;
 map_set(nodes,16,temp);

 //display_map(nodes);
//printf("going to call compute_lengths:\n");
 compute_lengths(elements,nodes,elelength);
 return 0;
}
