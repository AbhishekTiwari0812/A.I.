#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void _(char *s);		//for debugging purpose only
clock_t TIME_START,TIME_CURRENT;
int TIME_ALLOWED;		//in seconds
long double time_spent;
typedef struct bid_info{
	int bid_id;			//to identify repetitions
	int cid;			//company which bid for the blocks
	int bid_value;		//value a company is willing to pay
	int num_of_blocks;		//number of coal blocks company wants to bid for.
	int *block_id;		//block ids the company is bidding for.
	int bid_already_taken;		//boolean value to check if the current bid already exists in the solution or not.
	double goodness;		//how "good" is this current bid
	int nbd_size;			//number of compatible bids
	int *compatible_bids;	//id of all the compatible bids
}bid_info;
int num_of_bids;		//total number of bids
bid_info **bid_list;	//list of all bids
int BID_ID;
int ITERATION_NUMBER;
int MAX_VALUE;
double *total_goodness;
int *already_checked;
void scale_down();	//helps with making the CDF of the probability function
int bin_search(double *A,int left,int right,double key);	//returns index which is greater than or equal to key
int bin_search_int(int *A,int left,int right,int key);		//standard binary search.returns 1 if key is in A,0 if not.
void pb(bid_info *current_bid);								//print bid.
int check_compatibility(int index1,int index2){		//checks if two bids are in conflict with each other or not.returns 0 if not.1 if compatible
	int i;
	int index;
	for(i=0;i<bid_list[index1]->num_of_blocks;i++)
		if(bin_search_int(bid_list[index2]->block_id,0,bid_list[index2]->num_of_blocks-1,bid_list[index1]->block_id[i]))
			return 0;
	return 1;
}
int pick_next(int *existing_solution){			//if almost all of the bids are taken,this function comes to rescue
	int i=0;
	while(i<num_of_bids){
		if(existing_solution[i])
			i++;
		else return i;
	}
	//_("Nothing is left to pick from");
	return -1;
}
int pick_random(int *existing_solution){		//randomly pick the next state (kepping the probablity of a "good" bid being picked higher)
	double random_value;
	int random_index;
	int count=0;
	srand((int)(1e3+9)*time(NULL));
	do{
		if(count>2000)
			return pick_next(existing_solution);
		count++;
		random_value=(double)(abs((rand()*10009))%(int)ceil(total_goodness[num_of_bids-1]));
		random_index=bin_search(total_goodness,0,num_of_bids-1,random_value);
	}while(existing_solution[random_index]);
	return random_index;
}

int nbd_size;
int *nbd_list_answer;
void find_all_compatible_bids(int index){			//find all the bids which can be placed even if bid_id= index is placed
	int i;
	int size;
	nbd_size=0;
	int counter=0;
	for(i=0;i<num_of_bids;i++)
		if(check_compatibility(index,i))
			nbd_list_answer[nbd_size++]=i;
}

void set_compatibles(){								//Create a graph.set all 'compatible' bids.
	int i,j;
	nbd_list_answer=(int *)malloc(sizeof(int)*num_of_bids);
	for(i=0;i<num_of_bids;i++){
		find_all_compatible_bids(i);
		bid_list[i]->nbd_size=nbd_size;
		bid_list[i]->compatible_bids=(int *)malloc(nbd_size*sizeof(int));
		for(j=0;j<nbd_size;j++)
			bid_list[i]->compatible_bids[j]=nbd_list_answer[j];
	}
	free(nbd_list_answer);
}

void copy(int *from,int *to,int size){
	int i;
	for(i=0;i<size;i++)
		to[i]=from[i];
}

long MAX;
int *answer;
int answer_size;
void find_max(){
	scale_down();
	set_compatibles();
	int i,j;
	answer_size;
	int *start_point=(int *)calloc(num_of_bids,sizeof(int));
	answer=(int *)malloc(num_of_bids*sizeof(int));
	MAX=-1;
	long temp=-1;
    int new_bid_index;
    int MAX_COUNT=0;
    int *my_solution=(int *)calloc(num_of_bids,sizeof(int));
	int *taken_bids=(int *)malloc(num_of_bids*sizeof(int));
	int *checked_bids=(int *)calloc(num_of_bids,sizeof(int));
    while(1){
	    new_bid_index=pick_random(start_point);
	    if(new_bid_index==-1){
            for(i=0;i<num_of_bids;i++)
            start_point[i]=0;
            continue;
	    }
		start_point[new_bid_index]=1;
		for(i=0;i<num_of_bids;i++){
			my_solution[i]=0;
			taken_bids[i]=0;
			checked_bids[i]=0;
		}
		int size_of_bids_taken=0;
		checked_bids[new_bid_index]=1;
		temp=bid_list[new_bid_index]->bid_value;
		my_solution[new_bid_index]=1;
		taken_bids[size_of_bids_taken++]=new_bid_index;
		for(i=0;i<4*num_of_bids;i++){
			new_bid_index=pick_random(my_solution);
			if(new_bid_index==-1){
				break;
			}
			if(bid_list[new_bid_index]->nbd_size < size_of_bids_taken||checked_bids[new_bid_index]==1)
				continue;
			checked_bids[new_bid_index]=1;
			int flag=1;
			for(j=0;j<size_of_bids_taken;j++){
				flag=check_compatibility(new_bid_index,taken_bids[j]);
				if(flag==0)
					break;
			}
			if(flag){
				taken_bids[size_of_bids_taken++]=new_bid_index;
				temp+=bid_list[new_bid_index]->bid_value;
			}

		}
		for(i=0;i<num_of_bids;i++){
			new_bid_index=i;
			if(bid_list[new_bid_index]->nbd_size < size_of_bids_taken||checked_bids[new_bid_index]==1)
				continue;
			checked_bids[new_bid_index]=1;
			int flag=1;
			for(j=0;j<size_of_bids_taken;j++){
				flag=check_compatibility(new_bid_index,taken_bids[j]);
				if(flag==0){
					break;
				}
			}
			if(flag){
				taken_bids[size_of_bids_taken++]=new_bid_index;
				temp+=bid_list[new_bid_index]->bid_value;
			}
		}
		if(temp > MAX){
			MAX=temp;
			copy(taken_bids,answer,size_of_bids_taken);
			answer_size=size_of_bids_taken;
            printf("max till now is%d\n",MAX);

		}
		TIME_CURRENT=clock();
		time_spent=(long double)(TIME_CURRENT- TIME_START)/(long double)CLOCKS_PER_SEC;
		//printf("This is time:%f\n",(double)time_spent);
		if(TIME_ALLOWED>time_spent+3)
            continue;
        else break;

	}
	/*//printf("Best answer is:%d\n",MAX);
	//printf("Bid ids are:\n");
	for(i=0;i<answer_size;i++){
		//printf("%d ",answer[i] );
	}
	//printf("\n");
	for(i=0;i<answer_size;i++){
		//printf("bid:%d\tvalue:%d\tcid:%d\n",bid_list[answer[i]]->bid_id,bid_list[answer[i]]->bid_value,bid_list[answer[i]]->cid);
	}

	_("\n");*/
}
int comparator (const void * a, const void * b){
   return ( *(int*)a - *(int*)b );
}

/*
int main(){

    FILE *in=fopen("1.txt","r");
    FILE *out=fopen("output.txt","w");
*/
int main(int argc, char *argv[]){
	if(argc < 3 || argc > 3){
		printf("Something wrong with the CLI format.Check and run the script again.\n");
		exit(0);
	}
	//read file
	FILE *in=fopen(argv[1],"r");
	if(in==NULL){
		printf("Error while trying to read %s\n",argv[1]);
		exit(EXIT_FAILURE);
	}
	//write to file
	FILE *out=fopen(argv[2],"w");
	if(out==NULL){
		printf("Error while trying to write to %s\n",argv[1]);
		exit(EXIT_FAILURE);
	}


	TIME_START=clock();
	//to identify all the different bids
	BID_ID=0;
	int Time;
	//number of coal blocks in the market to bid for
	int num_of_blocks;
	int num_of_companies;		//number of companies
	int cid,ncid;
	int c_num_of_bids, c_num_of_blocks,bid_value,temp_block_id;
	int i,j,k;
	fscanf(in,"%d",&Time);
	fscanf(in,"%d",&num_of_blocks);
	fscanf(in,"%d",&num_of_bids);
	fscanf(in,"%d",&num_of_companies);
	TIME_ALLOWED=(int)Time*60;					//changing time to seconds
	bid_list=(bid_info **)malloc(sizeof(bid_info *)*num_of_bids);		//list of all the bids
	total_goodness=(double *)calloc(num_of_bids,sizeof(double));		//Goodness of blocks is defined as (bid value)/(number of blocks being bid)
	already_checked=(int *)calloc(num_of_bids,sizeof(int));
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
			bid_list[BID_ID]->goodness=(double)bid_value/(double)c_num_of_blocks;
			bid_list[BID_ID]->block_id=(int *)malloc(sizeof(int)*c_num_of_blocks);
			if(BID_ID!=0){
				total_goodness[BID_ID]=bid_list[BID_ID]->goodness+total_goodness[BID_ID-1];
			}
			else{
				total_goodness[BID_ID]=bid_list[BID_ID]->goodness;
			}
			for(k=0;k<c_num_of_blocks;k++){
				fscanf(in,"%d",&temp_block_id);
				bid_list[BID_ID]->block_id[k]=temp_block_id;
			}
			bid_list[BID_ID]->bid_already_taken=-1;
			BID_ID++;
		}
	}
	find_max();
    qsort(answer,answer_size,sizeof(int),comparator);
	fprintf(out, "%ld",MAX );
	for(i=0;i<answer_size;i++)
		fprintf(out, " %d",answer[i],bid_list[answer[i]]->cid,bid_list[answer[i]]->bid_value );
	fprintf(out, "\n");
	if(in)
        fclose(in);
	if(out)
        fclose(out);
	return 0;
}


void _(char *s){
	//printf("%s\n",s );
}

void scale_down(){
	int i;
	for(i=0;i<num_of_bids;i++){
		total_goodness[i]/=total_goodness[BID_ID-1];
		total_goodness[i]*=1000.0;
	}
}

int bin_search(double *A,int left,int right,double key){
	int mid=(left+right)/2;
	if(right > left){
		if(A[mid]==key)
			return mid;
		else{
			if( key<A[mid]){
				return bin_search(A,left,mid-1,key);
			}
			else{
				return bin_search(A,mid+1,right,key);
			}
		}
	}
	else{
		if(A[right] >= key)
			return right;
		else
			return right+1;
	}
}

int bin_search_int(int *A,int left,int right,int key){
	int mid=(left+right)/2;
	if(right > left){
		if(A[mid]==key)
			return 1;
		else{
			if( key<A[mid]){
				return bin_search_int(A,left,mid-1,key);
			}
			else{
				return bin_search_int(A,mid+1,right,key);
			}
		}
	}
	//maybe some error
	return A[left]==key?1:0;
}
void pb(bid_info *current_bid){
	//printf("id #%d.value:%d.Goodness:%f.#blocks:%d.nbd_size:#%d\n",current_bid->bid_id,current_bid->bid_value,current_bid->goodness,current_bid->num_of_blocks,current_bid->nbd_size );
    int i;
	/*for(i=0;i<current_bid->nbd_size;i++)
		//printf("%d ",current_bid->compatible_bids[i] );
	//printf("\n");
*/}
