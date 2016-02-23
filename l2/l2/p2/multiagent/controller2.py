
##Takes the input currentBoardState and
##return a list of two objects initial position and Final Position
##Board is in form of 2-D list where left top is [0,0] and right Bottom is [4,4]

##In Board
##0 -> Empty Location
##1 -> Musketeer
##2 -> soldier
import math
def copy(state):
	answer=[]
	for i in state:
		answer.append(i[:]);
	return answer;

def generate_child(state,flag):
	answer=[];
	if(flag==1):
		for i in range(5):
			for j in range(5):
				if(state[i][j]==1):
					#left
					if(j-1>=0):
						if(state[i][j-1]==2):
							something=copy(state);
							something[i][j]=0;
							something[i][j-1]=1;
							answer.append(copy(something));
					#down
					if(i+1<5):
						if(state[i+1][j]==2):
							something=copy(state);
							something[i][j]=0;
							something[i+1][j]=1;
							answer.append(copy(something));
					#right
					if(j+1<5):
						if(state[i][j+1]==2):
							something=copy(state);
							something[i][j]=0;
							something[i][j+1]=1;
							answer.append(copy(something));
					#up
					if(i-1>=0):
						if(state[i-1][j]==2):
							something=copy(state);
							something[i][j]=0;
							something[i-1][j]=1;
							answer.append(copy(something));
					
		
	else:
		for i in range(5):
			for j in range(5):
				if(state[i][j]==2):
					#left
					if(j-1>=0):
						if(state[i][j-1]==0):
							something=copy(state);
							something[i][j]=0;
							something[i][j-1]=2;
							answer.append(copy(something));
					#down
					if(i+1<5):
						if(state[i+1][j]==0):
							something=copy(state);
							something[i][j]=0;
							something[i+1][j]=2;
							answer.append(copy(something));
					#up
					if(i-1>=0):
						if(state[i-1][j]==0):
							something=copy(state);
							something[i][j]=0;
							something[i-1][j]=2;
							answer.append(copy(something));
					#right
					if(j+1<5):
						if(state[i][j+1]==0):
							something=copy(state);
							something[i][j]=0;
							something[i][j+1]=2;
							answer.append(copy(something));
	return answer
def evaluate_state(current_state):
	pos=[];
	soilder_count=0;
	for i in range(5):
		for j in range(5):
			if(current_state[i][j]==1):
				pos.append([i,j]);
				if(i-1>=0):
					if(current_state[i-1][j]==2):
						soilder_count-=3;
				if(i+1<5):
					if(current_state[i+1][j]==2):
						soilder_count-=3;
				if(j-1>=0):
					if(current_state[i][j-1]==2):
						soilder_count-=3;
				if(j+1<5):
					if(current_state[i][j+1]==2):
						soilder_count-=3;
	col=abs(pos[0][0]-pos[1][0])+abs(pos[0][0]-pos[2][0])+ abs(pos[1][0]-pos[2][0]);
	row=abs(pos[0][1]-pos[1][1])+abs(pos[0][1]-pos[2][1])+ abs(pos[1][1]-pos[2][1]);
	final=min(col,row);
	if (final==0):
		return -10;
	return final*10+soilder_count;

def next_move(current,next):
	for i in range(5):
		for j in range(5):
			if(current[i][j]==1 and next[i][j]==0):
				if(i-1>=0 ):
					if(current[i-1][j]==2 and next[i-1][j]==1):
						return [[i,j],[i-1,j]];
				if(j-1>=0 ):
					if(current[i][j-1]==2 and next[i][j-1]==1):
						return [[i,j],[i,j-1]];
				if(i+1<5):
					if(current[i+1][j]==2 and next[i+1][j]==1):
						return [[i,j],[i+1,j]];
				if(j+1<5):
					if(current[i][j+1]==2 and next[i][j+1]==1):
						return [[i,j],[i,j+1]];

def DFS(current_state,flag,depth):
	if(depth==0):
		return evaluate_state(current_state);
	else:
		if(flag==1):
			children=generate_child(current_state,1);
			if(len(children)==0):
				return 10;
			max_value=-10;
			for i in children:
				child_val=DFS(i,1-flag,depth-1);
				if(child_val> max_value):
					max_value=child_val;
			return max_value;
		else:
			children=generate_child(current_state,2);
			min_value=10;
			for i in children:
				child_val=DFS(i,1-flag,depth-1);
				if(child_val < min_value):
					min_value=child_val;
					if (min_value==-10):
						return min_value;
			return min_value;


def IDDFS(current_state,num_of_iterations):
	max=-10;
	index=0;
	children=generate_child(current_state,1);
	for max_depth in (num_of_iterations,num_of_iterations+1):
		for i in range(len(children)):
			k=DFS(children[i],0,max_depth);
			if (k > max):
				max=k;
				index=i;
				if(max==10):
					return children[index];
	return children[index];

def musketeerMove(board):
    num_of_iterations=4;
    next_state=IDDFS(board,num_of_iterations);	
    return next_move(board,next_state);

