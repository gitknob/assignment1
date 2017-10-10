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

int inRange(int val, int max);
int contains(struct Instance instance, int x, int y);
void printValidOptions();
int  openFile(char fileName[], bool print);
void saveFile(char fileName[]);
void generateInstances();
void printInstance(struct Instance instance);

int main(int argc, char *argv[]){

	if(argc==5){
		if(strcmp(argv[1],"-i")==0 && strcmp(argv[3], "-o")==0){
			if(openFile(argv[2], false))
				saveFile(argv[4]);
			else return 1;
		}else if(strcmp(argv[1],"-o")==0 && strcmp(argv[3],"-i")==0){
			if(openFile(argv[4], false))
				saveFile(argv[2]);
			else return 0;
		}else printValidOptions();

	}else if (argc==3 && strcmp(argv[1],"-i")==0){
		if(!openFile(argv[2], true))
			return 1;
	}else if(argc==1){
		 generateInstances();

	}else printValidOptions();
	
	return 0;
}
	
// Checks if a value is within a given range
int inRange(int val, int max){
		return val >= MIN_X && val <= max;
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
	
	FILE *file = fopen(fileName,"r");
	
	if(file){

		char str[128];
		char delim[2]="\t";
		int mode=0, point=0, num_pt=0, temp_x, temp_y;
		
		while(fgets(str, sizeof(str), file) != NULL){
			
			if(strncmp(str,"#",1) == 0)
				 continue;

			if(mode == 0){
				if(strchr(str,'\t')==NULL){
					delim[0] = ' ';
					delim[1] = ' ';
				}
				instance.x = atoi(strtok(str, delim));
				if(instance.x <=0) goto errorCall;
				instance.y = atoi(strtok(NULL,delim));
				if(instance.y <=0) goto errorCall;
				mode++;
			}else if(mode == 1){
				delim[0] = '\t';
				delim[1] = ' ';
				instance.num_pt = atoi(strtok(str,delim));
				mode++;
				instance.points = (int *)malloc(2*instance.num_pt*sizeof(int)); 
			}else {
				if(strchr(str,'\t')==NULL){
					delim[0] = ' ';
					delim[1] = ' ';
				}
				temp_x = atoi(strtok(str, delim));
				temp_y = atoi(strtok(NULL,delim));

				if(contains(instance, temp_x, temp_y))
					goto errorCall;

				*(instance.points+point  ) = temp_x;
				*(instance.points+point+1) = temp_y;

				if(!inRange(*(instance.points+point),instance.x))
					goto errorCall;
				if(!inRange(*(instance.points+point+0),instance.y))
					goto errorCall;

				num_pt++;
				if(num_pt>instance.num_pt)break;
				point+=2;
			}
		}
		if(num_pt != instance.num_pt)goto errorCall;
		if(print) printInstance(instance);

	} else goto errorCall;

	return true;

	errorCall:
		printf("Error in reading the instance file!\n");
		return false;	
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

	if(instance.x * instance.y < instance.num_pt
		|| instance.x<=MIN_X  || instance.y<=MIN_Y
		|| instance.num_pt<=0 || inst_ammount<=0)
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
