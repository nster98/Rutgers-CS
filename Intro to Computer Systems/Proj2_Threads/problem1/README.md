## Problem 1

To generate a list of n numbers to a file "arrayNums.txt":

```make generate```

```./generate n```

To run any question, please follow the instructions below.

To send the output to an output text file, please add

```> output.txt```

to the command that runs the program.
This will redirect the standard output from the program into a text file of the given name.


### Part 1:
After generating a list of size n, to run the search with X threads: 

```make q1```

```./search n X```


### Part 2:
After generating a list of size n, to run the recursive divide-and-conquer search: 

```make q2```

```./search n```

Note: the value of the minimum size of the list before searching will begin can be specified in the Definition at the top of the search_2.c file:
MIN_LIST_SIZE, by default set to 1000.


### Variation on the Problem:
After generating a list of size n, to run the variation on the problem with (at most) X threads: 

```make qvar```

```./search n X```