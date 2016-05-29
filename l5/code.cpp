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