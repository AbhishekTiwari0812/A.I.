To compile the code use
g++ solution.cpp -o output
And to run the code...
./output i
where
 	i = 1 for running BS
	i = 2 for running BS-i
	i = 3 for running BS-ii
	i = 4 for running BS-FC
	i = 5 for running BS-MAC
=================================================================================================
method BS:The brute force backtracking method takes almost 3-4 minutes to find the solutions to 150 sudokus.
So, on an average it takes 3*60/150=1.2 seconds to solve one sudoku.
Note that, using optimization option, the time duration can be reduced to 75 seconds !!!!
To run the code with optimization option on:
	compile using: 	g++ solution.cpp -O3 -o output
	and run:		./output
(And almost no memnory at all)
One can check the time and memory used by using the command: top | grep output
=================================================================================================
method:BS_1 implements the Minimum remaining value heuristic.
	The function "constraints" returns the best variable to pick (Fail first heuristics).
	This method also doesn't use much memory because there's no allocation except the stack allocation.
	Works faster than BS.
	Time taken to solve 150 sudokus is almost 15 seconds.
	Memory used is almost similar to BS. (because there's no need of extra memory)
=================================================================================================
method:BS_2 implements the Least constraining Value heuristics in addition to the MRV.
	The function find_ordering creates the order of the values in which the variable is assigned.
	Time taken is almost 12 seconds.
	Memory used is equal to the amount used by BS_1
=================================================================================================
method: BS_FC implements the forward checking method.Helper functions are change_domain and check_domain.
change_domain is used in assigning the value at each state and "undoing" the assignment if the current assignment leads to failure.
check_domain is the method used to check the domain of the neighbours of the current variable for validation of the assignment.
	Time taken to solve 150 sudokus is 22 seconds.
	This is slightly more than the time taken by BS_1. The reason is that the Search space is not that big and there are a lot more checks going on in this function than in BS_1 or BS_2
	Memory used is slightly more than that of BS_1 and BS_2. This is because, in forward checking one needs to keep track of the domain of the variables as well which requires some memory.
==================================================================================================
Methos: BS_MAC implements the maintaining arc consistency concept.
Helper functions are change_consistency and remove_values. Change consistency maintains the arc consistency if a value is assigned to a variable. This uses remove_value to remove the values from the domains of neighbouring variables of current variable. Returns failure if nothing is left after consistency check in the domain of any of the neighbouring variable.
For some reason, there's  a memory leak happening in this function. Because of this, it is taking more time to find the valid assignments. Time taken is almost 1 minute in this case.
==================================================================================================
