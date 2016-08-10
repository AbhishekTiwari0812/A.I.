<<<<<<< HEAD
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

using namespace std;

struct node{
	int id;
	vector<node *>parents;
	float *values;	//contains only true. for false, take 1-p.
};
node **G;
int main(){
	ifstream in;
	ofstream out;
	in.open("in.txt");
	out.open("out.txt");
	int n;
	in>>n;
	G=new node*[n];
	for(int i=0;i<n;i++){
		G[i]=new node;
		G[i]->id=i;
	}
	int a,b,c;
	for(int i=0;i<n;i++){
			
	}
}
=======
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
using namespace std;

struct node{
	int id;
	vector<node *>parents;
	vector<node *>children;
	vector<float> values;	//contains only true. for false, take 1-p.
	int visited_time;
	bool visited;
	int rank;
};
node **G;
int size;		//0-based indexing is done here!

struct query{
	int * variable;
	int * evidence;
};

//string to int function
int stoi(string &s){
	//cout<<"stoi:"<<s<<endl;
	int temp=0;
	int p=1;
	for(int i=s.length()-1;i>=0;i--){
		temp+= (s[i]-'0')*p;
		p*=10;
	}
	//cout<<"returning:"<<temp<<endl;
	return temp;
}

float stof(string &s){ //returns the fractional part of first float; Input "2.23 7.34" output = .23 ;
	int i=0;
	while(s[i]!='.')
		i++;
	i++;
	float answer=0.0f;
	float p=0.1f;
	while(s[i]!=' '){
			answer+= (float)((float)(s[i]-'0'))*p;
			p*=0.1f;
		i++;
	}
	return answer;
}


node *parse(string &s){		//parsing each string of format current-node parent+
	int temp;
	node *current;
	bool is_first=true;
	string helper="";
	for(int i=0;i<s.length();i++){
		if(s[i]!=' ' && s[i]!='\n')
			helper+=s[i];
		else{
			temp=stoi(helper)-1;
			helper="";
			if(is_first){
				is_first=false;
				current=G[temp];
			}else{
				current->parents.push_back(G[temp]);
				G[temp]->children.push_back(current);
			}
		}
	
	}
	temp=stoi(helper)-1;
	if(is_first)
		current=G[temp];
	else{
		current->parents.push_back(G[temp]);
		G[temp]->children.push_back(current);
	}
	return current;
}

void print_graph(node **G){
	cout<<size<<endl;
	for(int i=0;i<size;i++){
		cout<<"id:"<<(G[i]->id+1)<<endl;
		cout<<"parent:"<<endl;
		for(int j=0;j<G[i]->parents.size();j++){
			cout<<(G[i]->parents[j]->id+1)<<"  ";
		}
		cout<<endl;
		cout<<"children of "<<(G[i]->id+1)<<" are:";
		for(int j=0;j<G[i]->children.size();j++){
			cout<<(G[i]->children[j]->id+1)<<" ";
		}
		cout<<endl;
	}
}

int COUNTER;
void dfs_visit(node *current){
	current->visited_time=COUNTER++;
	for(int i=0;i<current->children.size();i++){
		dfs_visit(current->children[i]);
	}
}
void toposort(node **G2){
	for(int i=0;i<size;i++){
		dfs_visit(G2[i]);
	}
}

bool cmp(node *a,node *b){
	return a->visited_time < b->visited_time;
}
node **create_copy(){
	node **bn=new node *[size];
	for(int i=0;i<size;i++){
		bn[i]=new node;
		bn[i]->id=i;
		bn[i]->visited=false;
		bn[i]->visited_time=100000;
		bn[i]->values=G[i]->values;
	}
	for(int i=0;i<size;i++){
		for(int j=0;j<G[i]->parents.size();j++)
			bn[i]->parents.push_back(bn[G[i]->parents[j]->id]);
		for(int j=0;j<G[i]->children.size();j++)
			bn[i]->children.push_back(bn[G[i]->children[j]->id]);
	}
	return bn;
}
void pa(int *A,int n){
	cout<<"array elements are:"<<endl;
	for(int i=0;i<n;i++)
		cout<<A[i]<< " ";
	cout<<endl;
}


int *full_mask;
void num_to_array(unsigned int current_mask){
	for(int i=0;i<size;i++){
		full_mask[i]=(current_mask&1);
		current_mask = (current_mask>>1);
	}
}
bool is_sane(int *A,int *B){
	for(int i=0;i<size;i++){
		if(A[i]!=2){
			if(A[i]!=B[i])
				return false;
		}
	}
	return true;
}


bool generate_next(unsigned int current_mask,int *original_mask){
	num_to_array(current_mask);
	return is_sane(original_mask,full_mask);

}

void set_mask(query q,node **g,int *mask,bool variable_included){
	int original_merged_mask;
	//merging varible and evidence.
	for(int i=0;i<size;i++){
		if(q.evidence[i]!=2){
			original_merged_mask=q.evidence[i];
		}
		else if(variable_included && q.variable[i]!=2)
			original_merged_mask=q.variable[i];
		else original_merged_mask=2;
		//search for the id 
		for(int j=0;j<size;j++){
			if(i==g[j]->id){
				mask[g[j]->rank]=original_merged_mask;
			}
		}		
	}
}

double evaluate(node **bn){
	double answer=1;
	//cout<<"mask:";
	//pa(full_mask,size);
	for(int i=0;i<size;i++){
		node *current=bn[i];
		int final_index=0;
		double probability;
		//cout<<"for "<<current->rank<<endl;
		//cout<<"parents are:";
		for(int j=0;j<current->parents.size();j++){
			//cout<<current->parents[j]->rank<<" ";
			final_index+=full_mask[current->parents[j]->rank];
			final_index=(final_index<<1);
		}
		//cout<<endl;
		//cout<<"index is:"<<final_index<<endl;
		final_index=(final_index>>1);
		probability=current->values[final_index];
		if(full_mask[current->rank]==1)
			answer*=probability;
		else 
			answer*=(1-probability);
	}
	return answer;
}

double variable_elimination(query &q){
	COUNTER=0;
	node **bn=create_copy();
	toposort(bn);
	sort(bn,bn+size,cmp);
	for(int i=0;i<size;i++)
		bn[i]->rank=i;
	full_mask=new int[size];
	unsigned int count=0;
	int *given_variable_mask=new int[size];
	set_mask(q,bn,given_variable_mask,true);
	double answer1=0;
	while(count<(1<<size)){
		if(generate_next(count,given_variable_mask)){
			answer1+= evaluate(bn);
		}
		count++;
	}
	set_mask(q,bn,given_variable_mask,false);
	double answer2=0;
	count=0;
	while(count<(1<<size)){
		if(generate_next(count,given_variable_mask)){
			answer2+= evaluate(bn);
		}
		count++;
	}
	delete bn;
	delete full_mask;
	return answer1/answer2;
}


void rejection_sampling(query &q){ 
 
    node **bn=create_copy(); 
    toposort(bn); 
    sort(bn,bn+size,cmp); 
    int total_samples=0; 
    int favorable_samples=0; 
 
 
 
    while(total_samples<4000){ 
 
 
     
    int curr_status[size]; 
     
    int status=1; 
 
     
 
    for(int i=0;i<size;i++) 
    { 
 
        double r = ((double) rand() / (RAND_MAX)); 
        //cout<<r<<endl; 
        int index=0; 
        int k; 
 
        //cout<<"AT ID:"<<bn[i]->id<<endl; 
        //cout<<"Parents size:"<<bn[i]->parents.size()<<" Parents:"; 
 
         
 
        for(int p=0;p<bn[i]->parents.size();p++) 
        { 
            k=(bn[i]->parents.size()-1)-p; 
            index+=(((int)pow(2,k))*curr_status[(bn[i]->parents[p]->id)]); 
          //  cout<<bn[i]->parents[p]->id<<" "; 
        } 
 
        //cout<<endl; 
        //cout<<"index:"<<index<<endl; 
 
 
        double val=(double)bn[i]->values[index]; 
        curr_status[bn[i]->id]=((val>=r)?1:0); 
 
        //cout<<"Current status: "<<curr_status[bn[i]->id]<<endl<<endl; 
 
        if((q.evidence[bn[i]->id])!=2) 
        { 
            if(curr_status[bn[i]->id]!=q.evidence[bn[i]->id]) 
            { 
                //cout<<curr_status[G[i]->id]<<" "<<q.evidence[G[i]->id]<<endl; 
                //cout<<r<<endl; 
                status=0; 
                break; 
            } 
        } 
 
     
 
    } 
 
 
     
     if(status) 
    { 
        total_samples++; 
        for(int z=0;z<size;z++) 
        { 
            if(q.variable[z]!=2) 
            { 
               if(q.variable[z]!=curr_status[z]) 
                    status=0; 
            } 
        } 
 
        if(status) 
            favorable_samples++; 
    } 
 
     
    }    
 
    if(total_samples!=0 ) 
        cout<<(double)favorable_samples/total_samples<<endl; 
    else 
    {
    	cout<<"Problem"; 
 		cout<<"total_samples"<<total_samples<<" favorable_samples"<<favorable_samples<<endl;
 	}
     
}


void solve (string &s){
	query q;
	s+=' ';
	q.evidence=new int[size];
	q.variable=new int[size];
	for(int i=0;i<size;i++){
		q.evidence[i]=2;
		q.variable[i]=2;
	}
	int method;
	int temp2;
	string temp="";
	int sign=1;
	bool is_evidence=false;
	for(int i=5;i<s.length();i++){
		if(s[i]==' ' && temp.length() > 0){
			if(is_evidence){
				temp2=stoi(temp)-1;
				q.evidence[temp2]=sign;
				sign=1;
				temp="";
			}else{
				temp2=stoi(temp)-1;
				q.variable[temp2]=sign;
				temp="";
				sign=1;
			}
		}
		else if(s[i]=='e'){
			temp="";
			is_evidence=true;
		}
		else if(s[i]=='~'){
			sign=0;
			temp="";
		}
		else {
			if(s[i]!=' ')
			temp+=s[i];
		}
	}
	if(s[0]=='v' ){
		//cout<<"this is the probability=";
		cout<<(variable_elimination(q))<<endl;
	}
	else {
		//cout<<"calling rejection sampling"<<endl;
		rejection_sampling(q);
	}
}


int main(){
	srand(time(NULL));
	ifstream in;
	ifstream query_file;
	string bn_file_name;
	cin>>bn_file_name;
	in.open(bn_file_name.c_str());
	if(in==NULL){
		cout<<"Input file is not found"<<endl;
		return 0;
	}
	string query_file_name;
	cin>>query_file_name;
	query_file.open(query_file_name.c_str());
	if(query_file==NULL){
		cout<<"Input file is not found"<<endl;
		return 0;
	}
	int n;
	in>>n;
	size=n;
	G=new node*[size];
	for(int i=0;i<size;i++){
		G[i]=new node;
		G[i]->id=i;
	}
	int a,b,c;
	string s;
	int table_size;
	node *current;
	//file read
	for(int i=0;i<n;i++){
		getline(in,s);
		if(s.length()!=0){
			current=parse(s);
			table_size=(1<<current->parents.size());
			float true_value;
			while(table_size--){
				getline(in,s);
				true_value=stof(s);
				current->values.push_back(true_value);
			}
			reverse(current->values.begin(),current->values.end());	//reverse the vector;
		}
		else i--;
	}
	//print_graph(G);
	while(getline(query_file,s)){
		solve(s);
		//break;
	}
}
>>>>>>> 43fc73f3fea4690d276094f1d5a56ef28a36a785
