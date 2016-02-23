#include <time.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
void _(char *s);			//for debugging purpose only
typedef struct bid_info{
	int bid_id;				//to identify repetitions
	int cid;				//company which bid for the blocks
	int bid_value;			//value a company is willing to pay
	int num_of_blocks;		//number of coal blocks company wants to bid for.
	int *block_id;			//block ids the company is bidding for.
	int bid_already_taken;	//boolean value to check if the current bid already exists in the solution or not.
	double goodness;		//how "good" is this current bid
	int nbd_size;			//number of compatible bids
	int *compatible_bids;	//id of all the compatible bids
}bid_info;
int num_of_bids;			//total number of bids
bid_info **bid_list;		//list of all bids
int BID_ID;
int bin_search(int *A,int left,int right,int key){
	int mid=(left+right)/2;
	if(right > left){
		if(A[mid]==key)
			return 1;
		else{
			if( key<A[mid])
				return bin_search(A,left,mid-1,key);

			else
				return bin_search(A,mid+1,right,key);
		}
	}
	return A[left]==key?1:0;
}
int check_compatibility(int index1,int index2){
	int i;
	int index;
	for(i=0;i<bid_list[index1]->num_of_blocks;i++)
		if(bin_search(bid_list[index2]->block_id,0,bid_list[index2]->num_of_blocks-1,bid_list[index1]->block_id[i]))
			return 0;
	return 1;
}
int nbd_size;
int *nbd_list_answer;
void find_all_compatible_bids(int index){
	int i;
	int size;
	nbd_size=0;
	int counter=0;
	for(i=0;i<num_of_bids;i++)
		if(check_compatibility(index,i))
			nbd_list_answer[nbd_size++]=i;
	if(nbd_size > 30)
		_("more than 30 neigbours!!");
	printf("%d\n",nbd_size );
}
void set_compatibles(){
	int i,j;
	nbd_list_answer=(int *)malloc(sizeof(int)*num_of_bids);
	int k=0;
	for(i=0;i<num_of_bids;i++){
		//printf("BID #%d is compatible with:",i);
		find_all_compatible_bids(i);
		bid_list[i]->nbd_size=nbd_size;
		bid_list[i]->compatible_bids=(int *)malloc(nbd_size*sizeof(int));
		for(j=0;j<nbd_size;j++){
			k++;
		//	printf("%d %d, ",i,nbd_list_answer[j]);
			bid_list[i]->compatible_bids[j]=nbd_list_answer[j];
		}
		//printf("\n");
	}
	printf("total:%d\n",k);
	free(nbd_list_answer);
}
int TEMP_GRAPH_SIZE;
int TEMP_MAX_VALUE;
int GRAPH_SIZE;
int MAX_VALUE;

int check_connectedness(bid_info *current,int *chosen,int size){
	int i;
	int j;
	int value=0;
	for(i=0;i<size;i++){
		for(j=i+1;j<size;j++){
			if(!check_compatibility(current->compatible_bids[chosen[i]], current->compatible_bids[chosen[j]])){
				return INT_MIN;
			}
		}
		value+=bid_list[current->compatible_bids[chosen[i]]]->bid_value;
	}
	return value;
}
int *getPowerSet(bid_info *current,int *set, int set_size){
	if(set_size>40)
		printf("Set size:%d\n", set_size);
	unsigned int pow_set_size = pow(2, set_size);
	int counter, j;
	int *temp;
	int *answer;
	temp=(int *)malloc(set_size*sizeof(int));
	int size;
	for(counter = 0; counter < pow_set_size; counter++){
		size=0;
		for(j = 0; j < set_size; j++){
			if(counter & (1<<j)){
				temp[j]=set[j];
				size++;
			}
		}
		int temp2=check_connectedness(current,temp,size)+current->bid_value;
		if(temp2>TEMP_MAX_VALUE){
			TEMP_MAX_VALUE=temp2;
			TEMP_GRAPH_SIZE=size;
			answer=temp;
		}
	}
	return answer;
}
int *seq(bid_info *current){
	int size=current->nbd_size;
	int *set = (int *)malloc(sizeof(int)*size);
	int i;
	for(i=0;i<size;i++){
		set[i]=i;
	}
	return getPowerSet(current, set, size);
}
void assign_answer(int *final_answer,int *temp_answer){
	int size=GRAPH_SIZE;
	int i;
	for(i=0;i<size;i++){
		final_answer[i]=temp_answer[i];
	}
}
void find_max(){
	set_compatibles();
	int i;
	bid_info *temp;
	int *final_answer=(int *)malloc(sizeof(int)*num_of_bids);
	for(i=0;i<num_of_bids;i++){
		temp=bid_list[i];
		int *temp_answer=seq(temp);
		if(TEMP_MAX_VALUE > MAX_VALUE){
			MAX_VALUE=TEMP_MAX_VALUE;
			GRAPH_SIZE=TEMP_GRAPH_SIZE+1;
			assign_answer(final_answer,temp_answer);
			final_answer[GRAPH_SIZE]=temp->bid_id;
		}
	}

	printf("%d ",MAX_VALUE);
	for(i=0;i<GRAPH_SIZE;i++){
        printf("%d ",final_answer[i]);
	}
	printf("\n");
}


int main(){

    char filename[100];
    scanf("%s",filename);
	FILE *in=fopen(filename,"r");
	FILE *out=fopen("output.txt","w");
	srand((int)(1e7+9)*time(NULL));
	BID_ID=0;
	long long int Time;
	int num_of_blocks, num_of_companies;
	int cid,ncid;
	int c_num_of_bids, c_num_of_blocks,bid_value,temp_block_id;
	int i,j,k;
	fscanf(in,"%lld",&Time);
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
	find_max();
	if(in)
        fclose(in);
	if(out)
        fclose(out);

	return 0;
}

void _(char *s){
	printf("%s\n",s );
}

