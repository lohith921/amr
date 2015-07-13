#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include"map_lib.h"

#ifdef single
#define REAL float
#else
#define REAL double
#endif

#define FILENAMESIZE 2048
#define INPUTLINESIZE 1024

void info(){
printf("This program is used to refine the mesh using edge bisection.");
printf("This program uses an .ele file and a .node file.");
printf("Usage shall be\n");
printf("amrgeo -en <filename>\n");
}

void syntax(){
  printf("amrgeo [-en] input_file\n");
  printf("    -e  element file indication.\n");
  printf("    -n  node file indication\n");
  exit(EXIT_SUCCESS);
}
// the following code reads a line from file
char* readline(char *string, FILE* infile, char infilename){
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

int main(int argc, char **argv){
 int eleindi=0,nodeindi=0;
 int i=0,j=0;
 //printf("arguments size is %d\n",argc);
 char nodefile[FILENAMESIZE];
 char elefile[FILENAMESIZE];
 char filename[FILENAMESIZE];
 filename[0]='\0';
 nodefile[0]='\0';
 elefile[0]='\0';
 char filetext[INPUTLINESIZE];
 filetext[0]='\0';
 FILE* nfp=NULL;
 for(i=1;i<argc;i++){
 	printf("i value is %d\n",i);
   if(argv[i][0]=='-'){
    for(j=1;argv[i][j]!='\0';j++){
	   // printf("argv[%d][%d]=%c \n",i,j,argv[i][j]);    
 	 if(argv[i][j]=='n'){
	    nodeindi=1;
     }
     else if(argv[i][j]=='e'){
        eleindi=1;
     }
     else{
        info();
     }
    }
   }
   else{
    strcpy(filename,argv[i]);
   	//printf("copying filename %s\n",argv[i]);
    //printf("copying value is %s \n",strcpy(filename,argv[i]));
    //printf("copying successful %s length is %d\n",filename,strlen(filename));
   }
 }
 printf("exited the for loop\n");
 //printf("Comparision %d\n",strcmp(&filename[(strlen(filename)-5)],".node"));
 if(!strcmp(&filename[(strlen(filename)-5)],".node")){
  //printf("comparing with .node %s",&filename[strlen(filename)-5]);
  strcpy(nodefile,filename);
  memcpy(elefile,&filename,strlen(filename)-5);
  strcat(elefile,".ele");
  elefile[strlen(elefile)+1]='\0';
  //strncpy(filename, filename, strlen(filename)-5);
  }
 else if(!strcmp(&filename[(strlen(filename)-4)],".ele")){
  //printf("comparing with .ele %s",&filename[strlen(filename)-4]);
  strcpy(elefile,filename);
  memcpy(nodefile,&filename,strlen(filename)-4);
  strcat(nodefile,".node");  
  nodefile[strlen(nodefile)+1]='\0';
  //strncpy(filename, filename, strlen(filename)-4);
  }
 else{
  syntax();
  }
 printf("The files are %s, %s\n",nodefile, elefile);
 // file names are read, now open the files and do the processing
 nfp=fopen(nodefile,'r');
 strcpy(filetext,readline(filetext,nfp,nodefile));
 printf("first line of node file is %s",filetext);
 return 0;
}
