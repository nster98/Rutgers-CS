## Problem 2

To generate a list of n numbers to a file "arrayNums.txt":

```make generate```

```./generate n```

To run any question, please follow the instructions below.

To send the output to an output text file, please add

```> output.txt```

to the command that runs the program.
This will redirect the standard output from the program into a text file of the given name.


### Part 1:
After generating a list of size n, to run the single-process search: 

```make q1```

```./search n```


### Part 2:
After generating a list of size n, to run the multi-process DFS search: 

```make q2```

```./search n```


### Part 3:
After generating a list of size n, to run the multi-process BFS search: 

```make q3```

```./search n```

Note: the value of X can be specified in the Definition at the top of the search_3.c file:
X_PROCS, by default set to 10.


### Variation on the Problem:
After generating a list of size n, to run the variation on the problem: 

```make qvar```

```./search n```