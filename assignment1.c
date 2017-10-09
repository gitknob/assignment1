#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define PROGNAME ("assignment1")
#define MIN_X 1
#define MIN_Y 1
#define MAX_X 1000
#define MAX_Y 1000

struct Instance{
	int x, y, num_pt, *points;
}instance;


int checkCorrectness(int inst_x, int inst_y);
int inRange(int val, int min, int max);
int contains(struct Instance instance, int x, int y);
void printValidOptions();
int  openFile(char fileName[], bool print);
void saveFile(char fileName[]);
void generateInstances();
void printInstance(struct Instance instance);


int main(int argc, char *argv[]){

	if(argc==5){
		if(strcmp(argv[1],"-i")==0 && strcmp(argv[3], "-o")==0){
			openFile(argv[2], false);
		}else if(strcmp(argv[1],"-o")==0 && strcmp(argv[3],"-i")==0){
			openFile(argv[4], false);
		}else printValidOptions();

	}else if (argc==3 && strcmp(argv[1],"-i")==0){
		openFile(argv[2], true);

	}else if(argc==1){
		 generateInstances();

	}else printValidOptions();
	

	return 0;
}

int checkCorrectness(int inst_x, int inst_y){
	
	return true;

}

// Checks if a value is within a given range
int inRange(int val, int min, int max){
	return val > min || val < max;
}

// Checks if given array contains a point
int contains(struct Instance instance, int x, int y){
	
	for(int point=0; point<instance.num_pt*2; point+=2 ){
		if(*(instance.points+point)==x && *(instance.points+point+1)==y)
			return true;
	}
	return false;
}

// Prints out the valid options and paramters for the program
void printValidOptions(){
	printf("%s [-i inputfile [-o outputfile]]\n",PROGNAME);
}

//Prints the instance information
void printInstance(struct Instance instance){
	printf("%d\t%d\n", instance.x, instance.y);
	printf("%d\n", instance.num_pt);

	for(int point=0; point<instance.num_pt*2; point+=2){	
		printf("%d\t%d\n", *(instance.points+point), *(instance.points+point+1));
	}

}

int openFile(char fileName[], bool print){

	bool error= false;	
	FILE *file = fopen(fileName,"r");
	
	if(file){

		char str[128];
		const char delim[2]="\t";
		int mode=0, point=0, num_pt=0;
		
		while(fgets(str, sizeof(str), file) != NULL){
			
			if(strncmp(str,"#",1) == 0)
				 continue;

			if(mode == 0){	
				instance.x = atoi(strtok(str, delim));
				instance.y = atoi(strtok(NULL,delim));
				mode++;
			}else if(mode == 1){
				instance.num_pt = atoi(strtok(str,delim));
				mode++;
				instance.points = (int *)malloc(2*instance.num_pt*sizeof(int)); 
			}else {
				//use contains method to check for duplicates!!!
				*(instance.points+point  ) = atoi(strtok(str, delim));
				*(instance.points+point+1) = atoi(strtok(NULL,delim));
				num_pt++;
				if(num_pt>instance.num_pt)break;
				point+=2;
			}
		}
		if(num_pt != instance.num_pt) error = true;
		if(!error && print) printInstance(instance);

	} else error = true;

	if(error) printf("Error in reading the instance file!\n");
	
	return !error;	
}

void saveFile(char fileName[]){
	FILE *file = fopen(fileName,"w");
	fprintf(file,"%d\t%d\n", instance.x, instance.y);
	fprintf(file,"%d\n",instance.num_pt);
	
	for(int point=0; point<instance.num_pt*2; point+=2){
		fprintf(file,"%d\t%d\n", *(instance.points+point), *(instance.points+point+1));
	}
	close(file);
}


/* Generates instances defined by the user
-asks user for dimentions, ammount of point, and ammount of instances
-checks if generation is possible, writes error if not
-generates each instace, checking for no duplicate points
-saves instances
-prints generation progress
*/
void generateInstances(){;
	
	int inst_ammount;
	char fileName[128];
	printf("Generating random instances\n");
	printf("Enter the circuit board size MAX_X MAX_Y: ");
	scanf("%d %d", &instance.x, &instance.y);
	printf("Enter the number of points NUM_PT: ");
	scanf("%d", &instance.num_pt);
	printf("Enter the number of random instances to be generated: ");
	scanf("%d", &inst_ammount);

	if(instance.x * instance.y < instance.num_pt)
		printf("Error in generating instances!\n");
	else {
		instance.points = (int *)malloc(2*instance.num_pt*sizeof(int));
		srand(time(NULL));
		for(int j=0; j<inst_ammount; j++){
			for(int point=0, x_cord, y_cord; point<instance.num_pt*2; point+=2){
				do{
					x_cord = rand() % instance.x-1 + MIN_X;
					y_cord = rand() % instance.y-1 + MIN_Y;
				}while( contains(instance, x_cord, y_cord) );	
				*(instance.points+point  ) = x_cord;
				*(instance.points+point+1) = y_cord;
			
			}

			sprintf(fileName, "instance%d_%03d.txt",instance.num_pt, j+1);
			saveFile(fileName);
			printf(fileName);
			printf(" generated");
			if(j!=inst_ammount-1) printf("\n");
			
		}
		printf(" ... done!\n");
	}
}
