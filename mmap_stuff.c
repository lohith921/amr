#include <stdio.h>
#include <stdlib.h>
#include "mmap_lib.h"


void display_both(struct map_t *m,int pt);

int main(int argc,char **argv) {
   struct map_t *test;
   REAL *temp;
   temp=(REAL *)malloc(2*sizeof(REAL));
   temp[0]=1.0;temp[1]=2.1;
   test=map_create();
   if(test==NULL){
    printf("Map couldn't be created\n");
    return 0;
   }
   map_set(test,1,temp);
   display_both(test,1);
   temp[0]=2.1;temp[1]=3.2;
   map_set(test,2,temp);
   display_both(test,2);
   temp[0]=4.0;temp[1]=5.1;
   map_set(test,4,temp);
   display_both(test,4);
   printf("\n");

  // reset an existing entry
  temp[0]=2.2;temp[1]=2.2; 
  map_set(test,2,temp);

   display_both(test,2);
   display_both(test,4);
   display_both(test,1);

   display_both(test,8);

   temp[0]=8.0;temp[1]=9.2;
   map_set(test,8,temp);

   display_both(test,8);
}

void display_both(struct map_t *m,int pt) {
   
   printf("displaying for %d: ",pt);
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
