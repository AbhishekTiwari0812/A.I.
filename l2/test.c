#include <stdio.h>
#include <stdlib.h>
int i,j,k;	//iterators;

typedef struct bid{
	int bid_id;
	int value;
	int *block_id_list;
}bid;


int main(){
	FILE *in=fopen("input.txt","r");
	FILE *out=fopen("output.txt","w");
	int Time, num_of_blocks, num_of_companies, num_of_bids;
	fscanf(in,"%d",&Time);
	fscanf(in,"%d",&num_of_blocks);
	fscanf(in,"%d",&num_of_bids);
	fscanf(in,"%d",&num_of_companies);
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
