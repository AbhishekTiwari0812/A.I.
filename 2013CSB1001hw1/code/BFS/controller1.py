# This function takes board as an input
# returns three lists as described in README file
# In board, following convention is followed
#         1 -> musketeer
#         2 -> soldier
#         0 -> empty location
#         3 -> Soldier With Diamond (Goal State)
dimension_x=0;
dimension_y=0;
class node:
	def __init__(self,value,x,y):
		#denotes what it is, 0 or 1 or 2 or 3
		self.value=value  
		#position
		self.x=x
		self.y=y;
		self.parent=None		#initializing with Null value
		#add neighbours
		self.nbd=self.add_neighbours()
		self.visited=False
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

def BFS(map,x,y):
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
	#pushing the start node in the list
	map[x][y].visited=True
	helper_queue.append(map[x][y])
	temp.append([x,y])
	#searching for the destination
	while len(helper_queue)!=0:
		current=helper_queue[0]
		#inefficient, takes O(n). 
		#but doesn't matter here because, copying and other steps also take O(n)
		#so it's not a bottleneck here
		del helper_queue[0]
		del temp[0]
		exploredNodes.append([current.x,current.y])
		for i in current.nbd:
			if(not(map[i[0]][i[1]].visited)):
				map[i[0]][i[1]].parent=current
				if(map[i[0]][i[1]].value==2 or map[i[0]][i[1]].value==3):
					map[i[0]][i[1]].visited=True
					if(map[current.x][current.y].value!=3):
						helper_queue.append(map[i[0]][i[1]])
						temp.append([i[0],i[1]])
		##print "current queue:",temp
		searchQueue.append(temp[:])
		if(map[current.x][current.y].value==3):
			shortestPath=find_path(map,current.x,current.y)
			return exploredNodes,searchQueue,shortestPath
	#no path found
	return(None,None,None)



def singleAgentSearch(board):
	#main driver
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
	#creating a graph...
	map=[]
	for i in range(0,len(board)):
		map.append([])
		for j in range(0,len(board[i])):
			temp=node(board[i][j],i,j)
			map[i].append(temp)
	#finding the musketeers..
	for i in range(0,len(board)):
		for j in range(0,len(board[i])):
			if(board[i][j]==1):
				#search for a path to the diamond...
				tempExploredNodes,tempSearchQueue,tempShortestPath = BFS(map,i,j)
				if(tempShortestPath==None):
					#there's no path
					#do nothing
					something = 2+3;
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
