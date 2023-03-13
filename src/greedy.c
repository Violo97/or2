#include "tsp.h"
#include "greedy.h"
#include "utility.h"
#include <math.h>

void swap_element(int curr_pos , int next_node , int *sol);
double eucl_dist(instance *inst , int start_p , int final_p);
void optimal_sol(double dist , int *sol , instance *inst);
void nearest_neighbor_algo(instance *inst , int start_n);
void grasp_algorithm(instance *inst , int start_n);
double rand01();

void nearest_neighbor_algo(instance *inst , int start_n){

    //control if the starting node is valid
    //if(start_n > inst->nnodes) print_error("NODE NOT VALID");

    //initialize the vector of visited node 
    int *sol = malloc(inst->nnodes * sizeof(int));
    //total distance
    double obj = 0;


    //initi the sol array

    for(int i = 1 ; i <= inst->nnodes ; i++){
        sol[i-1] = i;
    }

    //move at the start postion the starting node
    if(start_n != 1){
        swap_element(0 , start_n , sol);    
    }

    double obj_update;
    double best_dist;
    //found the next element of the path 
    for(int i = 0 ; i < inst->path_length ; i++){
        int n = 0;
        best_dist = INFINITY;
        for(int j = i +1 ; j < inst->nnodes ; j++){
            obj_update = eucl_dist(inst , sol[i] , sol[j]);
            /////////////////////
            //debug piece of code
            /////////////////////
            //printf("Value of euclidean distance : %f \n" , obj_update );
            /////////////////////
            //end debug
            /////////////////////
            if ( obj_update < best_dist){
                best_dist = obj_update;
                n=j;
            }
        }

        swap_element( i+1 , n+1 , sol);
        obj += obj_update;
        

    }

    //control the solution
    optimal_sol(obj , sol , inst);
    free(sol);

}

void grasp_algorithm(instance *inst , int start_n){

    //control if the starting node is valid
    //if(start_n > inst->nnodes) print_error("NODE NOT VALID");

    //initialize the vector of visited node 
    int *sol = malloc(inst->nnodes * sizeof(int));
    //total distance
    double obj = 0;


    //initi the sol array

    for(int i = 1 ; i <= inst->nnodes ; i++){
        sol[i-1] = i;
    }

    //move at the start postion the starting node
    if(start_n != 1){
        swap_element(0 , start_n , sol);    
    }

    double obj_update;
    double best_dist;
    double second_dist;
    //found the next element of the path 
    for(int i = 0 ; i < inst->path_length ; i++){
        int n = 0;
        int n2 = 0;
        best_dist = INFINITY;
        for(int j = i +1 ; j < inst->nnodes ; j++){
            obj_update = eucl_dist(inst , sol[i] , sol[j]);
            /////////////////////
            //debug piece of code
            /////////////////////
            //printf("Value of euclidean distance : %f \n" , obj_update );
            /////////////////////
            //end debug
            /////////////////////
            if ( obj_update < best_dist){
                second_dist = best_dist;
                best_dist = obj_update;
                n2=n;
                n=j;
            }
        }
        if(rand01()<0.2){
            swap_element( i+1 , n2+1 , sol);
            obj += second_dist;
        }else{
            swap_element( i+1 , n+1 , sol);
            obj += best_dist;
        }
        
        

    }

    //control the solution
    optimal_sol(obj , sol , inst);
    free(sol);


}

void optimal_sol(double value , int *sol , instance *inst){
    /////////////////////
    //debug piece of code
    /////////////////////
    printf("Best solution found : %f  New possible solution : %f\n" , inst->zbest , value );
    /////////////////////
    //end debug
    /////////////////////
    if( value < inst->zbest){
        inst->zbest = value;
        for(int j = 0 ; j < inst->path_length+1 ; j++){
            inst->best_sol[j] = sol[j];
        }
    }
    /////////////////////
    //debug piece of code
    /////////////////////
    printf("Best solution path : [");
    for(int i = 0 ; i < inst->path_length ; i++){
        printf("%d " , inst->best_sol[i]);
    }
    printf("]\n");
    /////////////////////
    //end debug
    /////////////////////
    


}

double rand01(){
    return ((double) rand() / RAND_MAX);
}

void swap_element( int curr_pos , int next_node , int *sol){
    int temp = sol[curr_pos];
    sol[curr_pos] = sol[next_node-1];
    sol[next_node-1] = temp;
    /////////////////////
    //debug piece of code
    /////////////////////
    //printf("%d , %d \n" , curr_pos , next_node);
    for(int j = 0 ; j < sizeof(sol) ; j++){
            //printf("%d" , sol[j]);
    }
    //printf("\n");
    /////////////////////
    //end debug
    /////////////////////
    

}

double eucl_dist(instance *inst , int start_p , int final_p){
    double dx = inst->xcoord[start_p] - inst->xcoord[final_p];
	double dy = inst->ycoord[start_p] - inst->ycoord[final_p];
    return sqrt(dx*dx + dy*dy);
    
}