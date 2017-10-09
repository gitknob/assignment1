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
int contains(int array[], int x, int y);
void printValidOptions();
void openFile(char fileName[], bool print);
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
int contains(int array[], int x, int y){
	for(int p=0; p<sizeof(array)/sizeof(int); p+=2 )
		if(array[p]==x && array[p+1]==y)
			return true;
	return false;
}

// Prints out the valid options and paramters for the program
void printValidOptions(){
	printf("%s [-i inputfile [-o outputfile]]\n",PROGNAME);
}

//Prints the instance information
void printInstance(struct Instance instance){
	printf("%d %d\n", instance.x, instance.y);
	printf("%d\n", instance.num_pt);

	for(int point=0; point<instance.num_pt*2; point+=2){
		//why is this giving segmentation error?
		//works when reading an existing file,
		//why not when creating?	
		printf("%d %d\n", *(instance.points+point), *(instance.points+point+1));
	}

}

void openFile(char fileName[], bool print){
	
	FILE *file = fopen(fileName,"r");

	if(file){

		char str[128];
		const char delim[2]=" ";

		int mode=0, point=0;
		
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
				*(instance.points+point  ) = atoi(strtok(str, delim));
				*(instance.points+point+1) = atoi(strtok(NULL,delim));
				point+=2;
				//printf("%d %d\n", *(instance.points+point), *(instance.points+point+1));
			}
		}

		if(print) printInstance(instance);

		return instance;
	} else printf("Error in reading the instance file!\n");
		//Perhaps add proper error handlng later?
}

void saveFile(char fileName[]){
	FILE *file = fopen(fileName,"w");//double check	
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

		for(int j=0; j<inst_ammount; j++){

			for(int point=0, x_cord, y_cord; point<instance.num_pt; point+=2){
					do{
					x_cord = rand() % instance.x-1 + MIN_X;
					y_cord = rand() % instance.y-1 + MIN_Y;
				}while( contains(points, x_cord, y_cord) );
				// CHange containment check?
				//why exactly does this not work here?			
				*(instance.points+point  ) = x_cord;
				*(instance.points+point+1) = y_cord;
			
			}
			//saveFile(saveFileLocation);
			printInstance(instance);
			printf("instance%d_%d.txt generated", instance.num_pt, j+1 );;
			if(j!=inst_ammount-1) printf("\n");
			
		}
		printf(" ... done!\n");
	}
}
