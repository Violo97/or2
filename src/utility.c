#include "tsp.h"
#include <math.h>
#include <stdio.h>

double euclidean_dist(instance *inst , int start_p , int final_p){
    double dx = inst->xcoord[start_p] - inst->xcoord[final_p];
	double dy = inst->ycoord[start_p] - inst->ycoord[final_p];
    //return sqrt(dx*dx + dy*dy);
    return 0.0;
    
}