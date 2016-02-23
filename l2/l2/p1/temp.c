#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct bid_info{
	int bid_id;			//to identify repetitions
	int cid;			//company which bid for the blocks
	int bid_value;		//value a company is willing to pay
	int num_of_blocks;		//number of coal blocks company wants to bid for.
	int *block_id;		//block ids the company is bidding for.
	int bid_already_taken;		//boolean value to check if the current bid already exists in the solution or not.
}bid_info;
int num_of_bids;
bid_info ** bid_list;
int BID_ID;
int ITERATION_NUMBER;
int MAX_VALUE;
/*
 *accept the current state and change the bit mask
 *of currently bid blocks
 */
void change_bids_taken(int *bids_taken,bid_info *current_state){
	int i;
	for(i=0;i<current_state->num_of_blocks;i++)
		bids_taken[current_state->block_id[i]]=1;
	return;
}
/*
 *	@params bids_taken:bit mask of the bids that are already taken.
 *	checking if current state is compatible to be taken or not.
 */
int check_compatibility(int *bids_taken,bid_info *current_state){
	int i;
	/*if(current_state->bid_already_taken==ITERATION_NUMBER)
		return 0;
	else
	*/
	for(i=0;i<current_state->num_of_blocks;i++){
		if(bids_taken[current_state->block_id[i]]==1)
			return 0;
	}
	return 1;
}

//returns a random set of bids allocated.
int *pick_random_state(bid_info** list,int list_size, int num_of_blocks){
	int i,j,flag;
	//ITERATION_NUMBER++;
	int *bid_mask=(int *)malloc(sizeof(int)*num_of_blocks);
	for(i=0;i<num_of_blocks;i++)
		bid_mask[i]=0;

	srand(time(NULL));
	//we can change this value 100 to something else to pick random states faster...
	for(i=0;i<100;i++){
		j=rand()%list_size;
		flag=check_compatibility(bid_mask,list[j]);
		if(flag){
			change_bids_taken(bid_mask,list[j]);
			MAX_VALUE+=list[j]->bid_value;
			//list[j]->bid_already_taken=ITERATION_NUMBER;

		}
	}
	return bid_mask;
}

void find_next_state(int *bid_mask,int mask_size,int current_gain){

}

void find_max(bid_info **list,int size,int num_of_bids){
	int *bids_picked=pick_random_state(list,size,num_of_bids);
	int value_gained=MAX_VALUE;
	MAX_VALUE=0;
}


int main(){
	FILE *in=fopen("input.txt","r");
	FILE *out=fopen("output.txt","w");
/*
int main(int argc, char *argv[]){
	if(argc < 3 || argc > 3){
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
	MAX_VALUE=0;
	BID_ID=0;
	ITERATION_NUMBER=0;
	int Time, num_of_blocks, num_of_companies;
	int cid,ncid;
	int c_num_of_bids, c_num_of_blocks,bid_value,temp_block_id;
	int i,j,k;
	fscanf(in,"%d",&Time);
	fscanf(in,"%d",&num_of_blocks);
	fscanf(in,"%d",&num_of_bids);
	fscanf(in,"%d",&num_of_companies);
	bid_list=(bid_info **)malloc(sizeof(bid_info *)*num_of_bids);
	for(i=0;i<num_of_companies;i++){
		fscanf(in,"%d",&cid);
		fscanf(in,"%d",&c_num_of_bids);
		for(j=0;j<c_num_of_bids;j++){
			fscanf(in,"%d",&cid);
			fscanf(in,"%d",&c_num_of_blocks);
			fscanf(in,"%d",&bid_value);
			bid_list[BID_ID]=(bid_info *)malloc(sizeof(bid_info));
			bid_list[BID_ID]->bid_id=BID_ID;
			bid_list[BID_ID]->cid=cid;
			bid_list[BID_ID]->num_of_blocks=c_num_of_blocks;
			bid_list[BID_ID]->bid_value=bid_value;
			bid_list[BID_ID]->block_id=(int *)malloc(sizeof(int)*c_num_of_blocks);
			for(k=0;k<c_num_of_blocks;k++){
				fscanf(in,"%d",&temp_block_id);
				bid_list[BID_ID]->block_id[k]=temp_block_id;
			}
			bid_list[BID_ID]->bid_already_taken=-1;
			BID_ID++;
		}
	}
	for(i=0;i<num_of_bids;i++){
        printf("bid_id:%d\ncid:%d\nnumber of blocks:%d\nvalue:%d\n\n",bid_list[i]->bid_id,bid_list[i]->cid,bid_list[i]->num_of_blocks,bid_list[i]->bid_value);
	}
	find_max(bid_list,num_of_bids,num_of_bids);
	//it sometimes gives segmentation fault....
	//TODO: FIX THIS!
	if(in)
        fclose(in);
	if(out)
        fclose(out);
	return 0;
}
