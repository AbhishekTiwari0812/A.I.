#include <climits>
#include <ctime>
#include <set>
#include <fstream>
#include <queue>
#include <vector>
#include <iostream>
using namespace std;
int num_of_blocks;

struct block{
	int id;
	int base;				//== 0, block is on the table;==num_of_blocks+1, block is held by the arm.
	bool is_clear;
};


struct Action{
	int id;			//==1 pick; ==2 unsatck; ==3 release ==4 stack
	int block1;
	int block2;
	Action(int id,int block1,int block2){
		this->id=id;
		this->block1=block1;
		this->block2=block2;
	}
	Action(){
		this->id=-1;
		this->block1=-1;
		this->block2=-1;	
	}
	void print();
};
void Action::print(){
	switch(this->id){
		case 1:
			cout<<"action (pick "<<block1<<")"<<endl;
		break;
		case 2:
			cout<<"action (unstack "<<block1<<" "<<block2<<")"<<endl;
		break;
		case 3:
			cout<<"action (release "<<block1<<")"<<endl;
		break;
		case 4:
			cout<<"action (stack "<<block1<<" "<<block2<<")"<<endl; 
		break;
	}
}

struct state{
	int h_val;
	Action action;			//type of the action which resulted in the state, helpful for tracing back the path.
	vector<block> blocks;
	state *parent;
	bool arm_busy;
	void h();
};
state *GOAL;
state *START;

void state::h(){
	this->h_val=0;
	int helper[num_of_blocks+2];
	for(int i=0;i<num_of_blocks+2;i++){
		helper[i]=1;
	}
	for(int i=0;i<num_of_blocks;i++){
		helper[this->blocks[i].base]++;
	}
	int h_val=0;
	for(int i=0;i<num_of_blocks;i++){
		if(this->blocks[i].base!=GOAL->blocks[i].base){
			this->h_val+= (1<<helper[this->blocks[i].id]);
		}
	}
}

//checks if block a is on block b
bool on(block &a,block &b){
	return (a.base==b.id);
}

//pick a block
bool pick_block(block &current){
	if(current.is_clear && current.base==0){
		current.base=num_of_blocks+1;
		current.is_clear=false;
		return true;
	}
	return false;
}

//remove block a from the top of block b
bool unstack(block &a,block &b){
	if(a.is_clear && on(a,b)){
		b.is_clear=true;
		a.base=num_of_blocks+1;
		a.is_clear=false;
		return true;
	}
	return false;
}

//put block a on the table
bool release(block &a){
	if(a.base==num_of_blocks+1){
		a.base=0;
		a.is_clear=true;
		return true;
	}
	return false;
}

//put block a on the top of block b.
bool stack(block &a,block &b){
	if(b.is_clear && (a.base==num_of_blocks+1)){
		a.base=b.id;
		b.is_clear=false;
		a.is_clear=true;
		return true;
	}
	return false;
}

state *create_copy(state *current){
	state *copy=new state;
	copy->blocks=current->blocks;
	copy->action=current->action;
	copy->parent=current->parent;
	copy->h_val=current->h_val;
	copy->arm_busy=current->arm_busy;
	copy->h_val=current->h_val;
	return copy;
}

vector<state *> child_generator(state *current){
	vector<state *> children;
	bool is_possible;
	state *temp=create_copy(current);
	for(int i=0;i<num_of_blocks;i++){
		is_possible=false;
		if(!temp->arm_busy)
			is_possible=pick_block(temp->blocks[i]);
		if(is_possible){
			temp->action=Action(1,temp->blocks[i].id,temp->blocks[i].id);
			temp->arm_busy=true;
			children.push_back(temp);
			temp=create_copy(current);
		}
		for(int j=0;j<num_of_blocks;j++)
			if(i!=j){
				is_possible=false;
				if(!temp->arm_busy)
					is_possible=unstack(temp->blocks[i],temp->blocks[j]);
				if(is_possible){
					temp->action=Action(2,temp->blocks[i].id,temp->blocks[j].id);
					temp->arm_busy=true;
					children.push_back(temp);
					temp=create_copy(current);
				}
			}

		is_possible=release(temp->blocks[i]);
		if(is_possible){
			temp->action=Action(3,temp->blocks[i].id,temp->blocks[i].id);
			temp->arm_busy=false;
			children.push_back(temp);
			temp=create_copy(current);
		}
		for(int j=0;j<num_of_blocks;j++)
			if(i!=j){
				is_possible=stack(temp->blocks[i],temp->blocks[j]);
				if(is_possible){
					temp->action=Action(4,temp->blocks[i].id,temp->blocks[j].id);
					temp->arm_busy=false;
					children.push_back(temp);
					temp=create_copy(current);
				}
			}
		
	}
	return children;	
}

//checks if c==g
bool goal_test(state *c,state *g){
	for(int i=0;i<num_of_blocks;i++){
		if(c->blocks[i].base!=g->blocks[i].base)
			return false;
	}
	return true;
}


void print_state(state &a){
	cout<<"h:"<<a.h_val<<endl;
	for(int i=0;i<num_of_blocks;i++){
		if(a.blocks[i].base==0){
			cout<<"block #"<<a.blocks[i].id<<" is on table"<<endl;
		}
		else if(a.blocks[i].base==num_of_blocks+1){
			cout<<"block #"<<a.blocks[i].id<<" is in the air"<<endl;
		}
		else{
			cout<<"block #"<<(a.blocks[i].id)<<" is on"<<"block #"<<(a.blocks[i].base)<<endl;
		}
	}
	cout<<"===================="<<endl;
}
void trace_path(state *a){
//	print_state(*a);
	if(a!=NULL){
		trace_path(a->parent);
		a->action.print();
	}
	return;
}
/**/
void BFS(state *start,state *goal){
	queue<state *> q;
	q.push(start);
	start->parent=NULL;
	state *current;
	while(!q.empty()){
		current=q.front();
		q.pop();
		//print_state(current);
		if(goal_test(current,goal)){
			trace_path(current);
			break;
		}else{
			vector<state *>child=child_generator(current); 
			for(int i=0;i<child.size();i++){
				child[i]->parent=current;
				q.push(child[i]);
				//TODO: to make the code run faster, check for visited states
			}
		}
		//TODO:add current in the visited list
	}

}

//min heap
class comp{
public:
	bool operator()(state *a,state *b){
		return a->h_val > b->h_val;
	}
};

void AStar(state *start,state *goal){
	priority_queue<state *,vector<state * >,comp> q;
	q.push(start);
	start->parent=NULL;
	state *current;
	while(!q.empty()){
		current=q.top();
		q.pop();
		//print_state(*current);
		if(goal_test(current,goal)){
			trace_path(current);
			break;
		}else{
			vector<state *>child=child_generator(current); 
			for(int i=0;i<child.size();i++){
					child[i]->parent=current;
					child[i]->h();
					q.push(child[i]);
			}
		}
	}

}


void goal_stack_planning(state *start, state *goal){
		
}

void init_state(state *current){
	current->parent=NULL;
	current->arm_busy=false;
	for(int i=0;i<num_of_blocks;i++){
		block block_temp;
		block_temp.id=(i+1);
		block_temp.is_clear=true;
		block_temp.base=0;
		current->blocks.push_back(block_temp);
	}
}

void read_input(ifstream &in,state *current){
	string temp;
	int id1,id2;
	in>>temp;
	while(true){
		in>>temp;
		if(temp[0]=='e')
			return;
		else if(temp[0]=='c'){			//clear
			in>>id1;
			//taken care of while initializing
		}else if(temp=="on"){
			in>>id1>>id2;
			id1--;
			id2--;
			current->blocks[id1].base=id2+1;
			current->blocks[id2].is_clear=false;
		}else{				//ontable
			in>>id1;
			//taken care of while initializing
		}
	}
}

int main(int argc,char *argv[]){
	ifstream in;
	if(argc==2)
		in.open(argv[1]);
	else{
		cout<<"wrong arguments"<<endl;
		return 0;
	}
	in>>num_of_blocks;
	char type;
	in>>type;
	state *start=new state;
	state *end=new state;
	init_state(start);
	init_state(end);
	read_input(in,start);
	read_input(in,end);
	START=start;
	GOAL=end;
	clock_t start_time,end_time;
	start_time=clock();
	switch(type){
		case 'a':
			AStar(start,end);
		break;
		case 'f':
			//forward planner with BFS.
			BFS(start,end);
		break;
		case 'g':
			//Goal Stack planner.	
		break;
		default:
		cout<<"There's something wrong with the input file"<<endl;
	}
	end_time=clock();
	cout<<"Time taken:"<<((double)((end_time-start_time)*1000)/CLOCKS_PER_SEC)<<" miliseconds"<<endl;
	return 0;
}
