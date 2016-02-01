dimension_x=0;
dimension_y=0;
diamond_x=0;
diamond_y=0;

class node:
	def __init__(self,value,x,y):
		self.value=value  
		self.x=x
		self.parent=None		#initializing with Null value
		self.y=y;
		self.nbd=self.add_neighbours()
		self.visited=False
		self.h_value=0
		self.g_value=0
		self.f_value=0
	def add_neighbours(self):
		'''adds all the immediate neighbours to the current node in a list in the anticlockwise order
		starting with the left of the current node'''
		global dimension_x
		global dimension_y
		nbd=[]
		x=self.x
		y=self.y
		#left
		if(y-1>=0):
			nbd.append([x,y-1])
		#down
		if (x+1 < dimension_x):
			nbd.append([x+1,y])
		#right
		if(y+1 < dimension_y):
			nbd.append([x,y+1])
		#up
		if(x-1>=0):
			nbd.append([x-1,y])
		return nbd
def find_path(map,x,y):
	'''Once the goal is reached, this function 
	constructs the path and returns a sequence of nodes which
	are essentially the nodes in the path'''
	path=[]
	current=map[x][y]
	if(not(current.parent==None)):
		path+=find_path(map,current.parent.x,current.parent.y)
	path.append([current.x,current.y])
	return path
def getHvalue(current):
	#returning the manhattan distance
	global diamond_y
	global diamond_y
	return abs(diamond_x-current.x)+abs(diamond_y-current.y);


def DFS(map,x,y,currentThreshold):
	'''currentThreshold is the threshold.
	If the node has f-value less or equal to this,
	it's allowed to be explored otherwise not'''
	#the nodes which are encountered while searching the goal are stored in this list
	searchQueue=[]
	#helper queue maintains the nodes which are yet to be explored in the order they are encountered
	helper_queue=[]
	temp=[]
	exploredNodes=[]
	#reinitializing the whole map for each time the function is called
	for i in range(0,len(map)):
		for  j in map[i]:
			j.visited=False
			j.parent=None
			j.g_value=0
	#pushing the start node in the list
	map[x][y].h_value=getHvalue(map[x][y]);
	map[x][y].f_value=map[x][y].g_value+map[x][y].h_value;
	map[x][y].visited=True
	helper_queue.append(map[x][y])
	temp.append([x,y])
	#helps keeping track of the next threshold which we'll set 
	#in the next iteration if the destination is not found in the current iteration
	nextThreshold=100000000
	while len(helper_queue)!=0:
		current=helper_queue[-1]
		del helper_queue[-1]
		del temp[-1]
		exploredNodes.append([current.x,current.y])
		for i in current.nbd:
			if(not(map[i[0]][i[1]].visited)):
				map[i[0]][i[1]].parent=current;
				map[i[0]][i[1]].g_value=current.g_value+1;
				map[i[0]][i[1]].h_value=getHvalue(map[i[0]][i[1]]);
				map[i[0]][i[1]].f_value=map[i[0]][i[1]].g_value+map[i[0]][i[1]].h_value;
				if(map[i[0]][i[1]].value==2 or map[i[0]][i[1]].value==3 ):
					map[i[0]][i[1]].visited=True
					if(map[current.x][current.y].value!=3):
						#if current doesn't cross the threshold
						#don't append it
						if(map[i[0]][i[1]].f_value <= currentThreshold):							
							helper_queue.append(map[i[0]][i[1]])
							temp.append([i[0],i[1]])
						#keeping track of next min f-value which is greater than current threshold
						elif( nextThreshold > map[i[0]][i[1]].f_value):
							nextThreshold=map[i[0]][i[1]].f_value;
		searchQueue.append(temp[:])
		#construct the path from the start point to the destinatio
		#if goal is found
		if(map[current.x][current.y].value==3):
			shortestPath=find_path(map,current.x,current.y)
			return exploredNodes,searchQueue,shortestPath,0
	#no path found, returning the next threshold
	return (exploredNodes,searchQueue,[-1],nextThreshold)



def singleAgentSearch(board):
	global diamond_x
	global diamond_y
	exploredNodes = []
	searchQueue  = []
	shortestPath = [[]]*100000
	tempExploredNodes = []
	tempSearchQueue  = []
	tempShortestPath = []
	global dimension_x
	global dimension_y
	dimension_x=len(board)
	dimension_y=len(board[0])
	map=[]
	for i in range(0,len(board)):
		map.append([])
		for j in range(0,len(board[i])):
			temp=node(board[i][j],i,j)
			if(board[i][j]==3):
				diamond_x=i;
				diamond_y=j;
			map[i].append(temp)
	#set initial value of the threshold
	for i in range(0,len(board)):
		for j in range(0,len(board[i])):
			if(board[i][j]==1):
				nextThreshold=getHvalue(map[i][j]);
				tempExploredNodes = []
				tempSearchQueue  = []
				tempShortestPath = []
				while (True):
					temptempExploredNodes,temptempSearchQueue,tempShortestPath,nextThreshold = DFS(map,i,j,nextThreshold);
					tempExploredNodes+=temptempExploredNodes[:]
					tempSearchQueue+=temptempSearchQueue[:]
					if (not tempShortestPath == [-1]):
						break;
				if(tempShortestPath==None):
					something=2+3;
				elif(len(tempShortestPath) < len(shortestPath)):
					exploredNodes,searchQueue,shortestPath=	tempExploredNodes,tempSearchQueue,tempShortestPath

	print "exploredNodes = ",exploredNodes
	print "searchQueue=["
	for i in searchQueue:
		print "            ",i
	print "            ]\nShortestPath=",shortestPath
	return (exploredNodes,searchQueue,shortestPath)

#for testing purpose
#board=[[0,2,2,2,1],[2,2,2,0,2],[2,1,0,3,2],[2,2,0,2,2],[1,2,2,2,0]]
#board=[[2,2,0,0],[0,1,2,2],[0,2,3,2],[2,0,2,0]]
#a,b,c=singleAgentSearch(board)
