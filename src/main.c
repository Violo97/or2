#include "tsp.h"
#include "greedy.h"
#include <time.h>


void print_error(const char *err); 
//generate document for gnuplot
void document_generator(instance *inst);
//print the node generated or read  
void print_instance(instance *inst);
void gen_plot();
void read_input(instance *inst);
void generate_inst(instance *inst);
void parse_command_line(int argc, char** argv, instance *inst);
//call the input algorithm 
void call_algo(instance *inst); 
void point_file(instance *inst);
void solution_file(instance *inst);

void print_error(const char *err) { printf("\n\n ERROR: %s \n\n", err); fflush(NULL); exit(1); }   

void free_instance(instance *inst)
{     
	free(inst->xcoord);
	free(inst->ycoord);
}    

int main(int argc, char **argv) 
{ 
	if ( argc < 2 ) { printf("Usage: %s -help for help\n", argv[0]); exit(1); }       
	
	
	instance inst;

	inst.best_sol = malloc(inst.nnodes * sizeof(int)); 	
	parse_command_line(argc,argv, &inst);

	//set a default seed value 
	if(inst.randomseed == 0){
		clock_t c = clock();
		inst.randomseed = c;	
	}    

	//generate or read the coordinates of the points  
	if(inst.nnodes >= 0){
        generate_inst(&inst);
    }else{
        read_input(&inst);
    } 

	//execute the algorithm 
	clock_t start = clock();
	if(inst.timelimit == INFINITY){
		call_algo(&inst);
	}else{
		while(clock()-start < inst.timelimit){
			printf("%ld\n" , clock()-start);
			call_algo(&inst);
		}
	}
	 
	//print_instance(&inst);
	document_generator(&inst);
	//gen_plot();
	point_file(&inst);
	solution_file(&inst);
	gen_plot();
	free_instance(&inst);
	return 0; 
} 

void generate_inst(instance *inst){
 
    inst->xcoord = malloc(inst->nnodes * sizeof(double)); 
    inst->ycoord = malloc(inst->nnodes * sizeof(double));
	//generate a random graph 
	srand(inst->randomseed);
    for(int i = 0 ; i < inst->nnodes  ; i++){
        inst->xcoord[i] = ((double) rand() / RAND_MAX) * 1000;
        inst->ycoord[i] = ((double) rand() / RAND_MAX) * 1000;
    }

}

void document_generator(instance *inst){
	FILE *fin = fopen("./plot/data.dat", "w");
	//generate file for gnuplot
	for(int i=0 ; i<inst->nnodes - 1 ; i++){
		fprintf(fin , "%f %f\n%f %f\n\n" , inst->xcoord[i] , inst->ycoord[i] , inst->xcoord[i+1] , inst->ycoord[i+1]);
	}
	fclose(fin);

}

void point_file(instance *inst){

	FILE *fin = fopen("plot/points.dat", "w");
	//generate file for gnuplot
	for(int i=0 ; i<inst->nnodes - 1 ; i++){
		fprintf(fin , "%f %f\n" , inst->xcoord[i] , inst->ycoord[i]);
	}
	fclose(fin);

}

void solution_file(instance *inst){

	FILE *fin = fopen("plot/solution.dat", "w");
	//generate file for gnuplot
	for(int i=0 ; i<inst->path_length+1 ; i++){
		fprintf(fin , "%f %f\n" , inst->xcoord[inst->best_sol[i]] , inst->ycoord[inst->best_sol[i]]);
	}
	fclose(fin);

}

void gen_plot(){
	system("gnuplot ./plot/commands.txt");
}

void call_algo(instance *inst){
	//printf("%d" , inst->algo);
	switch(inst->algo){
		case 0 :
			nearest_neighbor_algo(inst , 2);
			break;
		case 1 :
			grasp_algorithm(inst , 2);
			break;	
		default :
			print_error(" algorithm not valid ");
			break;
	}

}

void print_instance(instance *inst){
	printf("nnodes: %d\n" , inst->nnodes);
	for(int i = 0 ; i < inst->nnodes ; i++){
		printf("node %d : x->%f y->%f\n\n" , i , inst->xcoord[i] , inst->ycoord[i]);
	}
}

void read_input(instance *inst) {
                            
	//FILE *fin = fopen(inst->input_file, "r");
	FILE *fin = fopen(inst->input_file, "r");
	if ( fin == NULL ) print_error(" input file not found!");
	
	inst->nnodes = -1;

	char line[128];          // 1 line of the file
    char *sec_name;          // name of the parameter in the readed line
    char *token;            // value of the parameter in the readed line
    char sep[] = " :\n\t\r"; // separator for parsing

    // Read the file line by line
    while(fgets(line, sizeof(line), fin) != NULL) {
        if (strlen(line) <= 1 ) continue; // skip empty lines
        sec_name = strtok(line, sep);

        if(strncmp(sec_name, "NAME", 4) == 0){
            token = strtok(NULL, sep);
            //for future implement
			continue;
		}

		if(strncmp(sec_name, "COMMENT", 7) == 0){
			//for future implement 
			continue;
		}   

        if(strncmp(sec_name, "TYPE", 4) == 0){
            token = strtok(NULL, sep);  
            //for future implement
            continue;
		}

        if(strncmp(sec_name, "DIMENSION", 9) == 0 ){
            token = strtok(NULL, sep);
            inst->nnodes = atoi(token);
            inst->xcoord = malloc(inst->nnodes * sizeof(double));
			inst->ycoord = malloc(inst->nnodes * sizeof(double));
            continue;
		}

        if(strncmp(sec_name, "EOF", 3) == 0 ){
			break;
		}

        if(strncmp(sec_name, "EDGE_WEIGHT_TYPE", 16) == 0 ){
            token = strtok(NULL, sep);
			//for future implement
            continue;
		}

        if (strncmp(sec_name, "NODE_COORD_SECTION", 18) == 0){
			int i=0;
			int x,y;
			while (fscanf(fin, "%*d %d %d\n", &x, &y) == 2){
				inst->xcoord[i] = x;
				inst->ycoord[i] = y;
				i++;
			}
            continue;
        }

    }

	fclose(fin);    
	
}

void parse_command_line(int argc, char** argv, instance *inst) 
{ 
		
	// default   
	strcpy(inst->input_file, "NULL");
	inst->randomseed = 0; 
	inst->timelimit = INFINITY;
	inst->zbest = INFINITY;

	for ( int i = 1; i < argc; i++ ) 
	{ 
		if ( strcmp(argv[i],"-file") == 0 ) { strcpy(inst->input_file,argv[++i]); continue; } 			// input file
		if ( strcmp(argv[i],"-input") == 0 ) { strcpy(inst->input_file,argv[++i]); continue; } 			// input file
		if ( strcmp(argv[i],"-f") == 0 ) { strcpy(inst->input_file,argv[++i]); continue; } 				// input file
		if ( strcmp(argv[i],"-time_limit") == 0 ) { inst->timelimit = atof(argv[++i]); continue; }		// total time limit
		if ( strcmp(argv[i],"-seed") == 0 ) { inst->randomseed = abs(atoi(argv[++i])); continue; } 		// random seed
		if ( strcmp(argv[i],"-n") == 0 ) { inst->nnodes = atoi(argv[++i]); continue; }                  //number of nodes
		if ( strcmp(argv[i],"-l") == 0 ) { inst->path_length = atoi(argv[++i]); continue; } 			// path length of sol
		if ( strcmp(argv[i],"-d") == 0 ) { inst->debug = 1; continue; }
		if ( strcmp(argv[i],"-s") == 0 ) { inst->start_node = atoi(argv[++i]); continue; } 			   // start node 
		//type of algorithm input
		if ( strcmp(argv[i],"-g") == 0 ) { inst->algo = 0; continue;}                                 //nearest neighbor algortihtm
		if ( strcmp(argv[i],"-gg") == 0 ) { inst->algo = 1; continue;}                                //nearest neighbor with grasp	
    }      

}    