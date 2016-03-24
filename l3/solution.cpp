#include <cstring>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <set>
#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <climits>
using namespace std;
ofstream out;
void print_sudoku(int **s){
	if(s==NULL){
		out<<"Nothing to print"<<endl;
	}else
	for(int i=0;i<9;i++){
		for(int j=0;j<9;j++)
			out<<s[i][j];
	}
	out<<endl;
}

struct nbd{
	int x;
	int y;
};
nbd global_nbd[20];
void generate_nbd(int x,int y){
	int index=0;
	for(int i=0;i<9;i++){
		if(i!=x){
			global_nbd[index].x=i;
			global_nbd[index].y=y;
			index++;
		}
	}
	for(int i=0;i<9;i++){
		if(i!=y){
			global_nbd[index].x=x;
			global_nbd[index].y=i;
			index++;
		}
	}	
	int a=x-x%3;
	int b=y-y%3;
	for(int i=a;i<a+3;i++)
		for(int j=b;j<b+3;j++){
			if(!(i==x || j==y)){
				global_nbd[index].x=i;
				global_nbd[index].y=j;
				index++;
			}
		}
}

//check this. Not verified yet.
bool is_valid(int **sudoku,int x,int y,int value){
	/*for(int i=0;i<9;i++)
		if(i!=x && sudoku[i][y]==value){
			return false;
		}
	for(int j=0;j<9;j++)
		if(j!=y && sudoku[x][j]==value){
			return false;
		}
	int a=x-x%3;
	int b=y-y%3;
	for(int i=a;i<a+3;i++)
		for(int j=b;j<b+3;j++)
			if(i!=x && j!=y && sudoku[i][j]==value){
				return false;
			}
	*/
	generate_nbd(x,y);
	for(int i=0;i<20;i++){
		if(!(global_nbd[i].x==x && global_nbd[i].y==y) && sudoku[global_nbd[i].x][global_nbd[i].y]==value)
			return false;
	}
	return true;
}
//A[i]=1 => sudoku[x][y] != i
//A[0] doesn't matter here. (But added :(! )

int constraint_helper_array[10];
//pick the least constrained value.
//i.e, pick the variable whose nbd is mostly full
bool constraints(int **sudoku,int x,int y){
	if(sudoku[x][y]!=0)
		return false;
	generate_nbd(x,y);
	for(int i=0;i<10;i++)
		constraint_helper_array[i]=0;
	for(int i=0;i<20;i++){
		constraint_helper_array[sudoku[global_nbd[i].x][global_nbd[i].y]]=1;	
	}
	return true;
}

int **BS(int **sudoku,int count){
	if(count==81)
		return sudoku;
	int x=count/9;
	int y=count%9;
	int **answer=NULL;
	if(sudoku[x][y]==0){
		for(int i=1;i<=9;i++)
			if(is_valid(sudoku,x,y,i)){
				sudoku[x][y]=i;
				answer=BS(sudoku,count+1);
				if(answer!=NULL)
					return answer;
				sudoku[x][y]=0;
			}
	}
	else 
		return BS(sudoku,count+1);
	return NULL;
}

int **BS_1(int **sudoku,int count){
	if(count==81)
		return sudoku;
	int min=INT_MAX;
	int x=-1,y;
	int temp;
	
	for(int i=0;i<9;i++)
		for(int j=0;j<9;j++){
			bool flag=constraints(sudoku,i,j);
			if(flag==false)
				continue;
			temp=0;
			for(int k=1;k<=9;k++){
				if(constraint_helper_array[k]!=1)
					temp++;
			}
			if(temp < min){
				min=temp;
				x=i;
				y=j;
			}
		}

	int **answer=NULL;
	if(x!=-1){
		for(int i=1;i<=9;i++)
			if(is_valid(sudoku,x,y,i)){
				sudoku[x][y]=i;
				answer=BS_1(sudoku,count+1);
				if(answer!=NULL)
					return answer;
				sudoku[x][y]=0;
			}
	}
	else 
		return BS_1(sudoku,count+1);
	return NULL;
}

struct point{
	int index;
	int value;
};
bool comp(point a,point b){
	if(a.value!=b.value) 
		a.value<b.value;
	else a.index<b.index;
}

//returns the number of variables in the neighbourhood of (x,y) in whose domain value is available.
int possible(int **S,int x,int y,int value){
	generate_nbd(x,y);
	int count=0;
	for(int i=0;i<20;i++)
		if(is_valid(S,global_nbd[i].x,global_nbd[i].y,value))
			count++;
	return count;

}

//returns the order of values to be assigned to the variable (x,y) according to "Least constraining Heuristics"
vector<int> find_ordering(int **S,int x,int y){
	point A[9];
	for(int i=1;i<=9;i++){
		if(is_valid(S,x,y,i)){
			A[i-1].index=i;
			A[i-1].value=possible(S,x,y,i);
		}else{
			A[i-1].index=-1;
			A[i-1].value=INT_MAX;
		}
	}
	sort(A,A+9,comp);
	vector<int> answer;
	for(int i=0;i<9;i++){
		if(A[i].index!=-1)
			answer.push_back(A[i].index);
	}
	return answer;
}



int **BS_2(int **sudoku,int count){
	if(count==81)
		return sudoku;
	int min=INT_MAX;
	int x=-1,y;
	int temp;
	for(int i=0;i<9;i++)
		for(int j=0;j<9;j++){
			bool flag=constraints(sudoku,i,j);
			if(flag==false)
				continue;
			temp=0;
			for(int k=1;k<=9;k++){
				if(constraint_helper_array[k]!=1)
					temp++;
			}
			if(temp < min){
				min=temp;
				x=i;
				y=j;
			}
		}
	int **answer=NULL;
	if(x!=-1){
		vector<int> order=find_ordering(sudoku,x,y);
		for(int i=0;i<order.size();i++){
			//if(is_valid(sudoku,x,y,order[i])){
				sudoku[x][y]=order[i];
				answer=BS_2(sudoku,count+1);
				if(answer!=NULL)
					return answer;
				sudoku[x][y]=0;
			}
	}
	else 
		return BS_2(sudoku,count+1);
	return NULL;
}
struct node{
	set<int> domain;
	bool fixed;
};

//prints the domain of each variable 
//helpful for debugging
void print_container(node **domain_container){
	for(int i=0;i<9;i++)
		for(int j=0;j<9;j++){
			cout<<"for ("<<i<<","<<j<<")\n";
			for(set<int>::iterator it=domain_container[i][j].domain.begin();it!=domain_container[i][j].domain.end();it++)
				cout<<(*it)<<" ";
			cout<<endl;
		}
}
//checking whether the value is consistent for the variable (x,y) or not.
bool domain_check(node **domain_container,int value,int x,int y){
	for(int i=0;i<9;i++){
		if(!domain_container[i][y].fixed){
			if(i!=x)
				if(domain_container[i][y].domain.size()==1 && *(domain_container[i][y].domain.begin())==value){
					return false;
				}
		}
	}
	for(int i=0;i<9;i++){
		if(!domain_container[x][i].fixed){
			if(i!=y)
				if(domain_container[x][i].domain.size()==1 && *(domain_container[x][i].domain.begin())==value){
					return false;
				}
		}
	}
	int a=x-x%3;
	int b=y-y%3;
	for(int i=a;i<a+3;i++)
		for(int j=b;j<b+3;j++){
			if(!domain_container[i][j].fixed){
			if(i!=x || j!=y)
				if(domain_container[i][j].domain.size()==1 && *(domain_container[i][j].domain.begin())==value){
					return false;
				}
			}
		}
	return true;
}

//creates the domain of the variables in the initial sudoku board
node **int_to_node(int **sudoku){
	node **S=new node*[9];
	for(int i=0;i<9;i++)
		S[i]=new node[9];
	for(int i=0;i<9;i++)
		for(int j=0;j<9;j++){
			if(sudoku[i][j]!=0){
				S[i][j].domain.insert(sudoku[i][j]);
				S[i][j].fixed=true;
			}
			else{
				for(int k=1;k<=9;k++){
					if(is_valid(sudoku,i,j,k)){
						S[i][j].domain.insert(k);
						S[i][j].fixed=false;
					}

				}
			}
		}
	return S;
}

//to maintain the consistency
vector<point> change_domain(node ** domain_container,int x,int y,int value,vector<point> A,bool flag){
	if(flag){		//remove values
		for(int i=0;i<9;i++){
			if(domain_container[x][i].domain.find(value)!=domain_container[x][i].domain.end()){
				domain_container[x][i].domain.erase(value);
				point temp;
				temp.index=x;
				temp.value=i;
				A.push_back(temp);
			}
		}
		for(int i=0;i<9;i++){
			if(domain_container[i][y].domain.find(value)!=domain_container[i][y].domain.end()){
				domain_container[i][y].domain.erase(value);
				point temp;
				temp.index=i;		//index=x
				temp.value=y;		//value=y
				A.push_back(temp);
			}
		}
		int a=x-x%3;
		int b=y-y%3;
		for(int i=a;i<a+3;i++)
			for(int j=b;j<b+3;j++)
				if(domain_container[i][j].domain.find(value)!=domain_container[i][j].domain.end()){
					domain_container[i][j].domain.erase(value);
					point temp;
					temp.index=i;		//index=x
					temp.value=j;		//value=y
					A.push_back(temp);
			}
	}
	else{			//add the values back in the domain
		for(int i=0;i<A.size();i++)
			domain_container[A[i].index][A[i].value].domain.insert(value);
	}
	return A;
}


int **BS_FC(int **sudoku,int count,node **domain_container){
	if(count==81)
		return sudoku;
	int min=INT_MAX;
	int x=-1,y;
	int temp;
	//print_sudoku(sudoku);
	for(int i=0;i<9;i++)
		for(int j=0;j<9;j++){
			bool flag=constraints(sudoku,i,j);
			if(flag==false)
				continue;
			temp=0;
			for(int k=1;k<=9;k++){
				if(constraint_helper_array[k]!=1)
					temp++;
			}
			if(temp < min){
				min=temp;
				x=i;
				y=j;
			}
		}
	int **answer=NULL;
	if(x!=-1){
		vector<int> order=find_ordering(sudoku,x,y);
		for(int i=0;i<order.size();i++){
			if(domain_container[x][y].domain.find(order[i])!=domain_container[x][y].domain.end())
			if(domain_check(domain_container,order[i],x,y)){
				sudoku[x][y]=order[i];
				vector<point> temp_helper_2;
				domain_container[x][y].fixed=true;
				temp_helper_2=change_domain(domain_container,x,y,order[i],temp_helper_2,true);
				answer=BS_FC(sudoku,count+1,domain_container);
				if(answer!=NULL)
					return answer;
				else{
					domain_container[x][y].fixed=false;
					change_domain(domain_container,x,y,order[i],temp_helper_2,false);
					sudoku[x][y]=0;
				
				}

			}
		}
	}
	else 
		return BS_FC(sudoku,count+1,domain_container);
	return NULL;
}

//helper for AC function
bool remove_values(node **domain_container,nbd current,nbd neighbour){
	if(domain_container[current.x][current.y].domain.size()==1){
		int temp=*(domain_container[current.x][current.y].domain.begin());
		if(domain_container[neighbour.x][neighbour.y].domain.find(temp)!=domain_container[neighbour.x][neighbour.y].domain.end()){
			domain_container[neighbour.x][neighbour.y].domain.erase(temp);
			return true;
		}
	}
	else return false;
}
//AC-3 function (as defined in the book)
bool change_consistency(node **domain_container,int x,int y){
	queue<nbd> q;
	nbd temp;
	temp.x=x;
	temp.y=y;
	int t=0;
	q.push(temp);
	while(!q.empty()){
		temp=q.front();
		q.pop();
		generate_nbd(temp.x,temp.y);
		bool flag;
		for(int i=0;i<20;i++){
			flag=remove_values(domain_container,temp,global_nbd[i]);
			if(flag){
				if(domain_container[global_nbd[i].x][global_nbd[i].y].domain.size()==0)
					return false;
				q.push(global_nbd[i]);
			}
		}
	}
	return true;
}

node **create_copy(node **A){
	node **copy=new node*[9];
	for(int i=0;i<9;i++){
		copy[i]=new node[9];
		for(int j=0;j<9;j++){
			copy[i][j].domain=A[i][j].domain;
			copy[i][j].fixed=A[i][j].fixed;
		}
	}
	return copy;
}
int **BS_MAC(int **sudoku,int count,node **domain_container){
	if(count==81)
		return sudoku;
	int min=INT_MAX;
	int x=-1,y;
	int temp;
	//print_sudoku(sudoku);
	for(int i=0;i<9;i++)
		for(int j=0;j<9;j++){
			bool flag=constraints(sudoku,i,j);
			if(flag==false)
				continue;
			temp=0;
			for(int k=1;k<=9;k++){
				if(constraint_helper_array[k]!=1)
					temp++;
			}
			if(temp < min){
				min=temp;
				x=i;
				y=j;
			}
		}
	int **answer=NULL;
	if(x!=-1){
		vector<int> order=find_ordering(sudoku,x,y);
		for(int i=0;i<order.size();i++){
			if(domain_container[x][y].domain.find(order[i])!=domain_container[x][y].domain.end())
			if(is_valid(sudoku,x,y,order[i])){
					sudoku[x][y]=order[i];
					node **temp_container=create_copy(domain_container);
					temp_container[x][y].fixed=true;
					temp_container[x][y].domain.clear();
					temp_container[x][y].domain.insert(order[i]);
					bool flag=change_consistency(temp_container,x,y);
					if(!flag){
						sudoku[x][y]=0;
						free(temp_container);
						//delete temp_container;
						temp_container=NULL;
						continue;
					}
					answer=BS_MAC(sudoku,count+1,temp_container);
					if(answer!=NULL)
						return answer;
					else{
						//undo
						free(temp_container);
						//delete temp_container;
						temp_container=NULL;
						sudoku[x][y]=0;
					}
			}
		}
		order.clear();
	}
	else 
		return BS_MAC(sudoku,count+1,domain_container);
	return NULL;
}
int main(int argc, char *argv[]){
	ifstream in;
	in.open("p.txt");
	int **sudoku=new int*[9];
	for(int i=0;i<9;i++)
		sudoku[i]=new int[9];
	char c;
	clock_t start,end;
	start=clock();
	int t;
	if(strcmp("1",argv[1])==0){
		out.open("answer_bs.txt");
		t=1;
	}
	else if(strcmp("2",argv[1])==0){
		out.open("answer_bs1.txt");
		t=2;
	}
	else if(strcmp("3",argv[1])==0){
		out.open("answer_bs2.txt");
		t=3;
	}
	else if(strcmp("4",argv[1])==0){
		out.open("answer_bs_fc.txt");
		t=4;
	}
	else if(strcmp("5",argv[1])==0){
		out.open("answer_bs_mac.txt");
		t=5;
	}
	while(!in.eof()){
		for(int i=0;i<9;i++)
			for(int j=0;j<9;j++){
				in>>c;	
				if(in.eof()){
					end=clock();
					cout<<"time taken:"<<(((end-start)*1000)/CLOCKS_PER_SEC)<<" miliseconds"<<endl;
					in.close();
					return 0;
				}
				sudoku[i][j]=(c=='.'?0:(c-'0'));
		}
		if(t==1){
			print_sudoku(BS(sudoku,0));
		}
		else if(t==2){
			print_sudoku(BS_1(sudoku,0));
		}
		else if(t==3){
			print_sudoku(BS_2(sudoku,0));
		}
		else if(t==4){
			node **temp=int_to_node(sudoku);
			print_sudoku(BS_FC(sudoku,0,temp));
			delete temp;
		}
		else if(t==5){
			node **temp=int_to_node(sudoku);
			print_sudoku(BS_MAC(sudoku,0,temp));
			delete temp;
		}
	}
	return 0;
}