#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define PROGNAME ("assignment1")
#define MIN_X 1
#define MIN_Y 1
#define MAX_X 1000
#define MAX_Y 1000

int checkCorrectness(int inst_x, int inst_y);
int inRange(int val, int min, int max);
int contains(int array[][2], int x, int y);
void printValidOptions();
void openFile(char fileName[]);
void saveFile(char fileName[]);
void generateInstances();

int main(int argc, char *argv[]){

	if(argc==3 || argc==5){

		if(strcmp(argv[1],"-i") == 0){
			openFile(argv[2]);
			if(argc==5 && strcmp(argv[3],"-o")==0){
				saveFile(argv[4]);
			}

		}else if(strcmp(argv[1],"-o") == 0){
			if(argc==3)
				generateInstances();
			else if(strcmp(argv[3],"-i") == 0){
				openFile(argv[2]);
			}
			saveFile(argv[4]);

		}else printValidOptions();

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
int contains(int array[][2], int x, int y){
	for(int p=0; p<sizeof(array); p++ )
		if(array[p][0]==x && array[p][1]==y)
			return true;
	return false;
}

// Prints out the valid options and paramters for the program
void printValidOptions(){
	printf("%s [-i inputfile [-o outputfile]]\n",PROGNAME);
}

void openFile(char fileName[]){
	
	FILE *file = fopen(fileName,"r");

	if(file){

		char *token;
		char str[128], delim=" ";

		//int points[][2];
		int num_pt, max_x, max_y, mode=0;

		while(fgets(str, sizeof(str), file) != NULL){
			if(strncmp(str,"#",1) == 0)
				 continue;

			printf("%s", str);

			if(mode==0){
				printf("test2");
				token = strtok(str, delim);
				printf("test3");
				printf("x: %s  ", token);
				token = strtok(NULL, delim);
				printf("y: %s\n", token);
				mode++;
			}else if(mode == 1){
				mode++;
			}else{}
		}
	} else printf("Error in reading the instance file!\n");

}

void saveFile(char fileName[]){
	
}


/* Generates instances defined by the user
-asks user for dimentions, ammount of point, and ammount of instances
-checks if generation is possible, writes error if not
-generates each instace, checking for no duplicate points
-saves instances
-prints generation progress
*/
void generateInstances(){
	int max_x, max_y, num_pt, inst_ammount;;	

	printf("Generating random instances\n");
	printf("Enter the circuit board size MAX_X MAX_Y: ");
	scanf("%i %i", &max_x, &max_y );
	printf("Enter the number of points NUM_PT: ");
	scanf("%i", &num_pt);
	printf("Enter the number of random instances to be generated: ");
	scanf("%i", &inst_ammount);

	if(max_x * max_y <num_pt)
		printf("Error in generating instances!\n");
	else {

		for(int j=0; j<inst_ammount; j++){

			int points[num_pt][2];	

			for(int p=0, x_cord, y_cord; p<num_pt; p++){
				do{
					x_cord = rand() % max_x-1 + MIN_X;
					y_cord = rand() % max_y-1 + MIN_Y;
				}while( !contains(points, x_cord, y_cord) );

				points[p][0] = x_cord;
				points[p][1] = y_cord;
			
			}
			saveFile(points);
			printf("instance%i_%i.txt generated", num_pt, j+1 );;
			if(j!=inst_ammount-1) printf("\n");

		}
		printf(" ... done!\n");
	}
}
