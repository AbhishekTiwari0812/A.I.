import Queue
import copy

#dimensions of the board
dimension_x=0;
dimension_y=0;
#position of the diamond
diamond_x=0;
diamond_y=0;

class node:
	def __init__(self,value,x,y):
		#value of the item at co-ordinate x,y
		self.value=value  
		self.x=x
		self.parent=None		#initializing with Null value
		self.y=y;
		#adds all the immediate neighbours to the current node in a list in the anticlockwise order
		self.nbd=self.add_neighbours()
		self.visited=False
		self.h_value=-1
		self.g_value=0
		self.f_value=-1
	def __cmp__(self,other):
		'''used as a comparator for the node type objects
		helpful while using the inbuilt MinHeap'''
		if(other==None):
			return True
		return cmp(self.f_value,other.f_value)
	def __str__(self):
		'''prints the current object'''
		return "("+str(self.x)+","+str(self.y)+")"
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
	'''Used as the heuristics
	returning the manhattan distance'''
	global diamond_y
	global diamond_y
	return abs(diamond_x-current.x)+abs(diamond_y-current.y);


def AStar(map,x,y):
	#the nodes which are encountered while searching the goal are stored in this list
	searchQueue=[]

	#helper queue maintains the nodes which are yet to be explored in the increasing order
	#of their f-value
	helper_queue=Queue.PriorityQueue()
	#list of explored nodes 
	exploredNodes=[]

	#reinitializing the whole map for each time the function is called
	for i in range(0,len(map)):
		for  j in map[i]:
			j.visited=False
			j.parent=None
			j.g_value=0
	#pushing the start node in the list 
	map[x][y].visited=True
	map[x][y].h_value=getHvalue(map[x][y]);
	map[x][y].f_value=map[x][y].g_value+map[x][y].h_value;
	helper_queue.put(map[x][y])
	#searching for the destination
	while not helper_queue.empty():
		current=helper_queue.get()
		exploredNodes.append([current.x,current.y])
		if(not current.value == 3):
			for i in current.nbd:
				if(not(map[i[0]][i[1]].visited)):
					map[i[0]][i[1]].parent=current;
					map[i[0]][i[1]].g_value=current.g_value+1;
					map[i[0]][i[1]].h_value=getHvalue(map[i[0]][i[1]]);
					map[i[0]][i[1]].f_value=map[i[0]][i[1]].g_value+map[i[0]][i[1]].h_value;
					if(map[i[0]][i[1]].value==2 or map[i[0]][i[1]].value==3):
						map[i[0]][i[1]].visited=True
						helper_queue.put(map[i[0]][i[1]])
		current_queue_elements=[]
		for i in helper_queue.queue:
			current_queue_elements.append([i.x,i.y])
		if(not len(current_queue_elements)==0):
			searchQueue.append(current_queue_elements)
		#if the current node is destination
		# construct the path from start to the destination
		if(map[current.x][current.y].value==3):
			shortestPath=find_path(map,current.x,current.y)
			return exploredNodes,searchQueue,shortestPath
	#if the path is not found, return none!
	return(None,None,None)

def singleAgentSearch(board):
	'''main driver'''
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
			'''searching the destination node in the graph'''
			'''helpful for defining the heuristic function'''
			if(board[i][j]==3):
				diamond_x=i;
				diamond_y=j;
			map[i].append(temp)
	for i in range(0,len(board)):
		for j in range(0,len(board[i])):
			if(board[i][j]==1):
				#search for a path to the diamond from current musketeer ...
				tempExploredNodes,tempSearchQueue,tempShortestPath = AStar(map,i,j)
				if(tempShortestPath==None):
					#there's no path
					#do nothing
					something_random=2+3
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
