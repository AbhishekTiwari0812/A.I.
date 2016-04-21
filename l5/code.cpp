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
};
node **G;
int size;		//0-based indexing is done here!

struct query{
	set<int> variable;
	set<int> evidence;
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
	else
		current->parents.push_back(G[temp]);
	return current;
}
void print_graph(node **G){
	cout<<size<<endl;
	for(int i=0;i<size;i++){
		cout<<"id:"<<G[i]->id<<endl;
		cout<<"parent:"<<endl;
		for(int j=0;j<G[i]->parents.size();j++){
			cout<<G[i]->parents[j]->id<<"  ";
		}

		cout<<endl;
	}
}

void rejection_sampling(query &q){
	//implement the function here.
}

int COUNTER;
void dfs_visit(node *current){
	//TODO: make a copy of the BN
	current->visited_time=COUNTER++;
	for(int i=0;i<current->children.size();i++){
		current->children[i]->visited=true;
		dfs_visit(current->children[i]);
	}
}

void toposort(node **G2){
	for(int i=0;i<size;i++){
		if(!G2[i]->visited)
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
void variable_elimination(query &q){
	COUNTER=0;
	node **bn=create_copy();
	toposort(bn);
	sort(bn,bn+size,cmp);
	print_graph(bn);
}

void solve (string &s){
	query q;
	s+=' ';
	int method;
	int temp2;
	string temp="";
	int sign=1;
	bool is_evidence=false;
	for(int i=5;i<s.length();i++){
		if(s[i]==' ' && temp.length() > 0){
			if(is_evidence){
				temp2=stoi(temp)-1;
				q.evidence.insert(sign*temp2);
				sign=1;
				temp="";
			}else{
				temp2=stoi(temp)-1;
				q.variable.insert(sign*temp2);
				temp="";
				sign=1;
			}
		}
		else if(s[i]=='e'){
			temp="";
			is_evidence=true;
		}
		else if(s[i]=='~'){
			sign=-1;
			temp="";
		}
		else {
			if(s[i]!=' ')
			temp+=s[i];
		}
	}
	if(s[0]=='v')
		variable_elimination(q);
	else rejection_sampling(q);
}


int main(){
	ifstream in;
	ifstream query_file;
	ofstream out;
	in.open("b1.txt");
	query_file.open("q1.txt");
	out.open("out.txt");
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
		break;
	}
}
