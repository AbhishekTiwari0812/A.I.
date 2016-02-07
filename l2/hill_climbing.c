#include <stdio.h>
#include <stdlib.h>


int main(){
//int main(int argc, char *argv[]){
	/*if(argc < 3 || argc > 3){
		printf("Something wrong with the input format.Check and run the script again.\n");
		exit(0);
	}
	//read file
	FILE *in=fopen(argv[1],"r");
	if(in==NULL){
		perror("Error while trying to read %s\n",argv[1]);
		exit(EXIT_FAILURE);
	}
	//write to file
	FILE *out=fopen(argv[2],"w");
	if(out==NULL){
		perror("Error while trying to write to %s\n",argv[1]);
		exit(EXIT_FAILURE);
	}
	*/

	FILE *in=fopen("input.txt","r");
	FILE *out=fopen("output.txt","w");

	//scanning the file to take input....
	int Time, num_of_blocks, num_of_companies, num_of_bids;
	fscanf(in,"%d",&Time);
	fscanf(in,"%d",&num_of_blocks);
	fscanf(in,"%d",&num_of_bids);
	fscanf(in,"%d",&num_of_companies);
	int i;
	//read all the bids from the file.
	//current line is stored in line
	//assumption: line size is not more than 5000 bytes
	const size_t line_size=5000;
	char line[line_size];
	for(i=0;i<=num_of_bids;i++){
		fgets(line,line_size,in);
		if(i!=0){

            printf("bid no.%d is:%s\n",i,line);

		}
	}



	if(line)
		free(line);
	fclose(in);
	fclose(out);
	return 0;
}
