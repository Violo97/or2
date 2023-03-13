#ifndef TSP_  

#define TSP_

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h> 
#include <stdio.h> 

//data structures  

typedef struct {   
	
	//input data
	int nnodes; 	   
	double *xcoord;
	double *ycoord;

	// parameters 
	int randomseed;
	double timelimit;						// overall time limit, in sec.s
	char input_file[1000];		  			// input file
	int algo;                             //type of algorithm 
	int path_length;

	//global data
	double tstart;								
	double zbest;							// best sol. available  
	double tbest;							// time for the best sol. available  
	int *best_sol;						// best sol. available   
	int debug; 
	
	// model;     
	int start_node;
	
	
} instance;        


#endif 