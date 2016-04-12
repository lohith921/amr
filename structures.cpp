/* This is used to store all structure definitions */
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <iostream>
#include "structures.h"
#include "mmap_lib_p.h"

#ifdef SINGLE
#define REAL float
#else
#define REAL double
#endif

/**************String tokenizer for reading files*********************/
void tokenize(std::string str, std::vector<std::string> &token_v){
	//std::cout << "Tokenizer called " << std::endl;
	size_t start = str.find_first_not_of(" "), end = start;
	while (start != std::string::npos){
        // Find next occurence of delimiter
        	end = str.find(" ", start);
        // Push back the token found into vector
                token_v.push_back(str.substr(start, end-start));
        // Skip all occurences of the delimiter to find new start
        	start = str.find_first_not_of(" ", end);
    	}
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
	//std::cout << "Calc_length is called " << std::endl;
	std::cout << "The vertices are " << A[0] << " " << A[1] << " " << B[0] << " " << B[1] << std::endl;
	REAL d1,d2,d;
	d1 = A[0]-B[0];
	d2 = A[1]-B[1];
	REAL temp = (d1*d1) + (d2*d2);
	d = std::sqrt(temp);
	//std::cout << "Length from inside calc_length is " << d << std::endl;
	return d;
}
/**********************************************************************/
void write_elements(std::string rt, struct element *el, int num_ele){
	std::ofstream fp; 
	fp.open((rt + ".r.ele"), std::ios::out);
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
void write_nodes(std::string rt,struct node_map *m, int num_nodes){
	std::cout << "write_nodes is called" << std::endl;
	std::ofstream fp;
	struct node_map *temp = m;
	fp.open((rt + ".r.node"),std::ios::out); 
	int i;
	fp << num_nodes << " " << 2 << std::endl; 
	while(temp!=  NULL){
	 	fp << temp->point << " " << temp->xandy[0] << " " << temp->xandy[1] << std::endl;
		//fprintf(fp," %d %f %f\n",temp->point,temp->xandy[0],temp->xandy[1]);//verts[1]);
	 	temp = temp->nxt;  
	}
	fp.close();
	std::cout <<  "Finished writing nodes" << std::endl;
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

/*************************************************************************/
struct element* find_element(struct element *el, int n1, int n2, int n3){
	std::cout << "Find element is called for nodes " << n1 << " " << n2 << " " << n3 << std::endl;
	struct element *temp;
	int nd1, nd2, nd3, c = 0;
	if(el->ele_no == -1)// for head element
                 temp = el->next;
     	else
                 temp = el;
     	//pv = temp;
	while(temp != NULL){
		c = 0;
		nd1 = temp->nodes[0]; nd2 = temp->nodes[1]; nd3 = temp->nodes[2];
		std::cout << " Checking against following elements " << nd1 << " " << nd2 << " " << nd3 << std::endl;
		if ((nd1 == n1) || (nd1 == n2) || (nd1 == n3))
			c++;
		if ((nd2 == n1) || (nd2 == n2) || (nd2 == n3))
			c++;
		if ((nd3 == n1) || (nd3 == n2) || (nd3 == n3))
			c++;
		std::cout << "Computed c value is " << c << std::endl;	
		if (c == 3)
			return temp;
		else
			temp = temp->next;
	}
	if (temp == NULL)
		return NULL;
}

/************************** Removed code *************************/
/*if ((nd1 == n1) && (nd2 == n2) && (nd3 == n3))
			return temp;
		else if ((nd1 == n2) && (nd2 == n3) && (nd3 == n1))
			return temp;
		else if ((nd1 == n3) && (nd2 == n1) && (nd3 == n2))
			return temp;
		else
		 	//pv = temp;
			temp = temp->next;
			*/
