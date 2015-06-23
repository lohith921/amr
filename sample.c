/* This program takes input a .ele file and a .node files with switches -e and -n resp*/
/* #define SINGLE */
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
/* Maximum number of characters in a file name (including the null).         */
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
  exit(0);
}
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
int main(int argc, char **argv)
{
 struct amrgo *m;
 int i,j,nele,nnode;
 char filename[INPUTLINESIZE];
 char *firstline;
 char *delim=" ";
 char data[200];
 data[0]='\0';
 filename[0]='\0';
 m->nodefilename[0]='\0';
 m->elefilename[0]='\0';
 FILE *fnode, *fele;
 //parsing the arguments//
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
 printf("&filename[strlen(filename) - 5] is %s\n",&filename[strlen(filename) - 5]);
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
 fnode=fopen(m->nodefilename,"r");
 fele=fopen(m->elefilename,"r");
 //while(fnode!=NULL){
 /*do{
 	printf("%s\n",data);
 // }while(fscanf(fnode,"%s",data)!=EOF);
 }while(fgets(data,200,fnode)!=NULL);*/
 firstline=readline(data,fnode,m->nodefilename);
 nnode=atoi(strtok(firstline,delim));
 printf("# of nodes are: %d\n",nnode);
 firstline=readline(data,fele,m->elefilename);
 nele=atoi(strtok(firstline,delim)); 
 printf("# of elements are: %d\n",nele);
// printf("findfiels output\n %s",findfield(firstline+1));
 fclose(fnode);
 fclose(fele);
 return 0;
}
