#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "mmap_lib.h"

#ifdef SINGLE
#define REAL float
#else
#define REAL double
#endif
 
#define maxlength 1.0
#define minlength 0.5
void display_map(struct map_t *first){
 printf("displaying the map\n");
 struct map_t *temp;
 temp=first;
 if(temp!=NULL){
  while(temp!=NULL){
   printf("Node : %d x= %f y= %f \n",temp->point,temp->xandy[0],temp->xandy[1]);
   temp=temp->nxt;
  }
 }
 else{
  printf("Invalid map\n");
  return; 
 }
}

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

struct vertex{
 int no;
 REAL *values;
}; 

REAL calc_length(REAL *A, REAL *B){
REAL d1,d2;
printf("The vertices are x1=%f, y1=%f, x2=%f, y2=%f\n",A[0],A[1],B[0],B[1]);
d1=fabs((A[0]-B[0]));
d2=fabs((A[1]-B[1]));
/*if(A[0]>B[0]){
 //printf("subracting %f from %f result:%f\n",A[0],B[0],A[0]-B[0]);
 d1=A[0]-B[0];
}
else{
 //printf("subracting %f from %f result:%f\n",B[0],A[0],B[0]-A[0]);
 d1=B[0]-A[0];
}
if(A[1]>B[1]){
// printf("subracting %f from %f result:%f\n",A[1],B[1],A[1]-B[1]);
 d2=A[1]-B[1];
}
else{
 //printf("subracting %f from %f result:%f\n",B[1],A[1],B[1]-A[1]);
 d2=B[1]-A[1];
}*/
REAL temp=(d1*d1+d2*d2);
printf("d1= %f, d2= %f, sum of squares: %f\n",d1,d2,temp);
return sqrt(temp);
}

void compute_lengths(int ele[][3], struct map_t *node_map){
 //int segmentA[2], segmentB[2], segmentC[2], 
 int i=3, j, k;
 REAL *vertexA, *vertexB, *vertexC;
 int vertexes[3];
 struct vertex vertices[3];
 int x1,y1,x2,y2;
 int elements=sizeof(ele)/(3*sizeof(int));
 printf("no of elements are: %d\n",elements);
 for(i=0;i<elements;i++){
 vertexes[0]=ele[i][0];
 vertexes[1]=ele[i][1];
 vertexes[2]=ele[i][2];
 vertexA=map_get(node_map, vertexes[0]);
 vertexB=map_get(node_map, vertexes[1]);
 vertexC=map_get(node_map, vertexes[2]);
 printf("length between %d and %d is %f\n\n",vertexes[0],vertexes[1],calc_length(vertexA,vertexB));
 printf("length between %d and %d is %f\n\n",vertexes[1],vertexes[2],calc_length(vertexB,vertexC));
 printf("length between %d and %d is %f\n",vertexes[2],vertexes[0],calc_length(vertexC,vertexA));
 }
// for(i=0;i<elements;i++)
// {
 /*  segmentA[0]=ele[i][0];
   segmentA[1]=ele[i][1];
   
   segmentB[0]=ele[i][1];
   segmentB[1]=ele[i][2];

   segmentC[0]=ele[i][2];
   segmentC[1]=ele[i][0];*/
/*for(j=0;j<3;j++){
   vertices[j].no=ele[i][j];
   vertices[j].values=(REAL *)malloc(2*sizeof(REAL));
   vertices[j].values=map_get(node_map,ele[i][j]);
  
   vertices[(j+1)%3].no=ele[i][(j+1)%3];
   vertices[(j+1)%3].values=(REAL *)malloc(2*sizeof(REAL));
   vertices[(j+1)%3].values=map_get(node_map,ele[i][(j+1)%3]);
   
/*   vertices[2].no=ele[i][1];
   vertices[2].values=(REAL *)malloc(2*sizeof(REAL));
   vertices[2].values=map_get(node_map,ele[i][2]);
   REAL length=calc_length(vertices[j].values,vertices[(j+1)%3].values);
   if(length > maxlength){

   }
   printf("length between %d and %d is %f\n",ele[i][j],ele[i][(j+1)%3],calc_length(vertices[j].values,vertices[(j+1)%3].values));
}*/

}

int main(){
 int i,j,k,elelength,total_elements;
 total_elements=10;
 int elements[10][3]={{1,2,3}, {1,12,3}, {3,4,16}, {16,4,15}, {4,5,15}, {5,14,15}, {5,6,14}, {14,6,8}, {6,7,8}, {13,8,9}};

 elelength=sizeof(elements)/(3*sizeof(int));
/* printf("length is %d\n",elelength);
 for(i=0;i<elelength;i++){
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
 compute_lengths(elements,nodes);
 return 0;
}
