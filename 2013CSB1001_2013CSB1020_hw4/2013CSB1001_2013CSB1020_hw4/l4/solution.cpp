#include <climits>
#include <ctime>
#include <set>
#include <fstream>
#include <queue>
#include <vector>
#include <iostream>
#include <stack>
#include <string>
#include <sstream>


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
	int arm_busy;      //arm containing the value of the block picked,,Is 0 if the arm is not busy
	void h();
};
state *GOAL;
state *START;

//these are used to avoid repetitions of the same states
long long int HELPER;
long long int HELPER_2;

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

//returns the heuristic value of the current state


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
bool Stack(block &a,block &b){
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
			temp->arm_busy=temp->blocks[i].id;
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
					temp->arm_busy=temp->blocks[i].id;     ////Edited
					children.push_back(temp);
					temp=create_copy(current);
				}
			}

		is_possible=release(temp->blocks[i]);
		if(is_possible){
			temp->action=Action(3,temp->blocks[i].id,temp->blocks[i].id);
			temp->arm_busy=0;
			children.push_back(temp);
			temp=create_copy(current);
		}
		for(int j=0;j<num_of_blocks;j++)
			if(i!=j){
				is_possible=Stack(temp->blocks[i],temp->blocks[j]);
				if(is_possible){
					temp->action=Action(4,temp->blocks[i].id,temp->blocks[j].id);
					temp->arm_busy=0;
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
	//cout<<"h:"<<a.h_val<<endl;
    //cout<<"Busy Arm:"<<a.arm_busy<<endl;
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



void init_state(state *current){
	current->parent=NULL;
	current->arm_busy=0;

	for(int i=0;i<num_of_blocks;i++){
		block block_temp;
		block_temp.id=(i+1);
		block_temp.is_clear=true;
		block_temp.base=0;
		current->blocks.push_back(block_temp);
	}
}

////




struct selement{
	vector<string> literals;
	string literal;
	int id;		//1 for literals, 2 for group of literals , 3 for action
};



//Functions to check the Literals
//checks if block a is on block b
bool check_on(block &a,block &b){
	return (a.base==b.id);
}

bool check_ontable(block &a)
{
	return a.base==0;
}

bool check_clear(block &a)
{
	return a.is_clear;
}

bool check_hold(block &a)
{
	return a.base==(num_of_blocks+1);
}

//Function to return which block is on the block id1
int getOn(state * currstate,int id1)
{
    //cout<<"getOn:";
	for(int i=0;i<num_of_blocks;i++)
	{
		if(currstate->blocks[i].base==id1)
        {
            //cout<<i+1<<endl;
            return i+1;
        }
	}
	return -1;
}


//get the block pointer from id
block * getBlock(state * currstate,int num)
{

	if(currstate->blocks[num-1].id==num)
		return & (currstate->blocks[num-1]);
	else
	{
		for(int i=0;i<num_of_blocks;i++)
		{
			if(currstate->blocks[i].id==num)
				return &(currstate->blocks[i]);
		}
	}
}

//defining to_string function for work
string to_string(int num)
{
    stringstream ss;
    ss<<num;
    return ss.str();

}

vector<string> read_input_gsp(ifstream &in,state *current)
{
	string temp;
	int id1,id2;
	in>>temp;

	vector<string> literals;

	while(true)
		{
        in>>temp;
		//cout<<temp;
		if(temp[0]=='e')
            return literals;
		else if(temp[0]=='c'){			//clear

			in>>id1;
			temp+=" ";
			temp+=to_string(id1);
			literals.push_back(temp);
			//taken care of while initializing
		}else if(temp=="on"){
			in>>id1>>id2;
			id1--;
			id2--;
			current->blocks[id1].base=id2+1;
			current->blocks[id2].is_clear=false;
			temp+=" ";
			temp+=to_string(id1+1);
			temp+=" ";
			temp+=to_string(id2+1);
			literals.push_back(temp);

		}else{				//ontable
			in>>id1;
			temp+=" ";
			temp+=to_string(id1);
			literals.push_back(temp);
			//taken care of while initializing
		}
	}
	return literals;
}

//vector containing the preconditions of an action
vector<string> preconditions(string action)
{
	stringstream actionstream(action);
	string type;
	actionstream>>type;
	vector<string> precondition;

	if(type=="pick")
	{
		int id1;
		actionstream>>id1;
		string prec="ontable ";
		prec+=to_string(id1);
		precondition.push_back(prec);
		prec="clear ";
		prec+=to_string(id1);
		precondition.push_back(prec);
	}
	else if(type=="unstack")
	{
		int id1,id2;
		actionstream>>id1>>id2;
		string prec="on ";
		prec+=to_string(id1);
		prec+=" ";
		prec+=to_string(id2);
		precondition.push_back(prec);
		prec="clear ";
		prec+=to_string(id1);
		precondition.push_back(prec);
	}
	else if(type=="stack")
	{
		int id1,id2;
		actionstream>>id1>>id2;
		string prec="hold ";
		prec+=to_string(id1);
		precondition.push_back(prec);
		prec="clear ";
		prec+=to_string(id2);
		precondition.push_back(prec);
	}
	else if(type=="release")
	{
		int id1;
		actionstream>>id1;
		string prec="hold ";
		prec+=to_string(id1);
		precondition.push_back(prec);
	}

	return precondition;
}

//Function to get the object for the stack based on type
selement * getselement1(string _element,int _id)
{
	selement * t=new selement;
	t->literal=_element;
	t->id=_id;
	return t;
}

//Function to get the object for the stack based on type
selement * getselement2(vector<string> _element,int _id)
{
	selement * t=new selement;
	t->literals=_element;
	t->id=_id;
	return t;
}


//Function to check if the Literal is true or false
bool checkLiteral(state* current,string literal)
{
	stringstream literalstream (literal);
	string type;
	literalstream>>type;

   // cout<<type;

	if(type=="on")
	{
		int id1,id2;
		literalstream>>id1>>id2;
		block * b1=getBlock(current,id1);
		block * b2=getBlock(current,id2);
		//cout<<b1->id<<b2->id<<endl;
		return check_on(*b1,*b2);
	}
	else if(type=="clear")
	{
		int id1;
		literalstream>>id1;
		block  * b1=getBlock(current,id1);
		//cout<<b1->id<<endl;
		return check_clear(*b1);
	}
	else if(type=="hold")
	{
		int id1;
		literalstream>>id1;
		block * b1=getBlock(current,id1);
		//cout<<b1->id<<endl;
		return check_hold(*b1);
	}
	else if(type=="ontable")
	{
		int id1;
		literalstream>>id1;
		block * b1=getBlock(current,id1);
		//cout<<b1->id<<endl;
		return check_ontable(*b1);
	}
}

//Preferred action based on the literal seen which is false
string prefAction(state * currstate,string literal)
{
	stringstream literalstream (literal);
	string type;
	literalstream>>type;
    string action="";
	if(type=="on")
	{
		int id1,id2;
		literalstream>>id1>>id2;
		action+="stack ";
		action+=to_string(id1);
		action+=" ";
		action+=to_string(id2);
		return action;
	}
	else if(type=="clear")			//implement it
	{
		int id1;
		literalstream>>id1;
		int id2=getOn(currstate,id1);		//Block which is on id1 block

        //cout<<"Arm:"<<currstate->arm_busy<<endl;

        //Sleep(1000);
		if(currstate->arm_busy)
        {
            action+="release ";
            action+=to_string(currstate->arm_busy);
           // cout<<"-_____-_______-______-_________-_______-"<<action<<endl;
            return action;
        }
        else
        {
            if(id2==-1)
            {
                cout<<"Cant find the base of the block";
                return "Error";
            }
            else
            {
                action+="unstack ";
                action+=to_string(id2);
                action+=" ";
                action+=to_string(id1);
            }
			return action;
        }


	}
	else if(type=="hold")
	{
		int id1;
		literalstream>>id1;
		block *b=getBlock(currstate,id1);
		//cout<<"Here"<<endl;
		if(b->base==0)
        {
            action+="pick ";
            action+=to_string(id1);
            //cout<<"pickpos"<<endl;
            return action;
        }
        else if(b->base!=(num_of_blocks+1))
        {
            //cout<<"unstackpos"<<endl;
            action+="unstack ";
            action+=to_string(id1);
            action+=" ";
            action+=to_string(b->base);
            return action;
        }
        else if(b->base==(num_of_blocks+1))
        {
            action+="release ";
            action+=to_string(b->id);
            return action;
        }

	}
	else if(type=="ontable")
	{
		int id1;
		literalstream>>id1;
		action+="release ";
		action+=to_string(id1);
		return action;
	}
}

bool isActionUnstack(string action)
{
    stringstream actionstream(action);
	string type;
	actionstream>>type;
	if(type=="unstack")
        return true;
    else
        return false;
}


void executeAction(state* start,string action)
{
    stringstream actionstream(action);
	string type;
	actionstream>>type;
	vector<string> precondition;

	if(type=="pick")
	{
		int id1;
		actionstream>>id1;
		block * b1=getBlock(start,id1);

		//print_state(*start);
		pick_block(*b1);
        //print_state(*start);

    }
	else if(type=="unstack")
	{
		int id1,id2;
		actionstream>>id1>>id2;
		block *b1=getBlock(start,id1);
		block *b2=getBlock(start,id2);
		//print_state(*start);
		unstack(*b1,*b2);

        start->arm_busy=b1->id;         //Setting up the Busy Arm
        //cout<<start->arm_busy;

		//print_state(*start);
	}
	else if(type=="stack")
	{
		int id1,id2;
		actionstream>>id1>>id2;
		block *b1=getBlock(start,id1);
		block *b2=getBlock(start,id2);
		//print_state(*start);
		Stack(*b1,*b2);
		//print_state(*start);
	}
	else if(type=="release")
	{
		int id1;
		actionstream>>id1;
		block *b1=getBlock(start,id1);
		//print_state(*start);
		release(*b1);
		start->arm_busy=0;
		//cout<<"Arm Cleared";
		//print_state(*start);
	}
}

void goal_stack_planning(state *start, state *goal,vector<string> goals)
{
    stack<selement*> gsp;
	selement * s=new selement;
	s->literals=goals;
	s->id=2;
	gsp.push(s);

	vector<string> actionsDone;
	for(int i=0;i<goals.size();i++)
	{
		s=new selement;
		s->literal=goals[i];
		s->id=1;
		gsp.push(s);
	}

	while(!gsp.empty())
	{
		s=gsp.top();

		//cout<<"id"<<s->id<<endl;
		//Sleep(2000);

		if(s->id==1)
		{
			string lit=s->literal;
			//cout<<checkLiteral(start,lit);
            //cout<<lit<<start->arm_busy<<endl;

			if(checkLiteral(start,lit) && (!start->arm_busy))
            {
                gsp.pop();
                //cout<<"true here"<<endl;
                //Sleep(2000);
            }
			else if(!checkLiteral(start,lit))
			{
				string action_req=prefAction(start,lit);
                /*
                cout<<"IS ACtion Unstack"<<isActionUnstack(action_req);

				if(isActionUnstack(action_req))
                {
                    string s2=prefAction(start,lit);
                    cout<<"____------------------------------_______________________";
                    cout<<s2<<endl;
                    s=new selement;
                    s->literal=s2;
                    s->id=3;
                    gsp.push(s);

                }
                */

				//cout<<action_req<<endl;
				s=new selement;
				s->literal=action_req;
				s->id=3;
				gsp.push(s);

				vector<string> precond=preconditions(action_req);
				gsp.push(getselement2(precond,2));
				for(int i=0;i<precond.size();i++)
                {
                  //  cout<<"preconditions: "<<precond[i]<<endl;
                    gsp.push(getselement1(precond[i],1));

                }
			}
			else
            {
                //cout<<"Is it here? ";
                string action_req=prefAction(start,lit);
                //cout<<action_req<<endl;
				s=new selement;
				s->literal=action_req;
				s->id=3;
				gsp.push(s);
            }

		}
		else if(s->id==2)
		{
			vector<string> literals=s->literals;
			bool check=true;
			//was Leaving this case for now
			for(int i=0;i<literals.size();i++)
			{
				if(checkLiteral(start,literals[i])==false)
					{
					    check=false;
                        break;
                    }
			}
			if(check==false)
            {
                gsp.push(getselement2(literals,2));
                for(int i=0;i<literals.size();i++)
                {
                    gsp.push(getselement1(literals[i],1));

                }

            }
            else
            {
                gsp.pop();
            }
		}
		else if(s->id==3)
		{

			executeAction(start,s->literal);
			//cout<<"Action Taken:"<<s->literal<<endl;
			actionsDone.push_back(s->literal);
			gsp.pop();

		}
	}

	//cout<<"------------------------------------------------------------------------"<<endl;
	//cout<<"Actions Are"<<endl;

	cout<<actionsDone.size()<<endl;

	for(int i=0;i<actionsDone.size();i++)
        cout<<actionsDone[i]<<endl;

    //cout<<"Total:"<<actionsDone.size();
}

/////

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
	HELPER=0;
	HELPER_2=0;
    
	if(argc==2)
		in.open(argv[1]);
	else{
		cout<<"wrong arguments"<<endl;
		return 0;
	}
    
	//in.open("3.txt");
	in>>num_of_blocks;
	char type;
	in>>type;
	state *start=new state;
	state *end=new state;
	init_state(start);
	init_state(end);

	clock_t start_time,end_time;
	start_time=clock();

	switch(type){
		case 'a':
		    read_input(in,start);
            read_input(in,end);
            START=start;
            GOAL=end;

			AStar(start,end);
		break;
		case 'f':
			//forward planner with BFS.
			read_input(in,start);
            read_input(in,end);
            START=start;
            GOAL=end;

			BFS(start,end);
		break;
		case 'g':
			//Goal Stack planner.
			{
			    vector<string> st=read_input_gsp(in,start);
                vector<string> goals= read_input_gsp(in,end);
                START=start;
                GOAL=end;
                goal_stack_planning(start,end,goals);

			}



		break;
		default:
		cout<<"There's something wrong with the input file"<<endl;
	}
	end_time=clock();
	cout<<"Time taken:"<<((double)((end_time-start_time)*1000)/CLOCKS_PER_SEC)<<" miliseconds"<<endl;
	return 0;

}

/*
int main()
{
    ifstream in;
	HELPER=0;
	HELPER_2=0;

	in.open("3.txt");

	in>>num_of_blocks;
	char type;
	in>>type;
	state *start=new state;
	state *end=new state;
	init_state(start);
	init_state(end);
    vector<string> st=read_input_gsp(in,start);
    vector<string> goals= read_input_gsp(in,end);
    /*
    for(int i=0;i<goals.size();i++)
        cout<<goals[i]<<endl;
    */
    /*
    START=start;
    GOAL=end;
    goal_stack_planning(start,end,goals);

}
*/
