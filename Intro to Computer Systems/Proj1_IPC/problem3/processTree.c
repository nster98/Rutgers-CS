//Alex Malynovsky
//Problem 3

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

struct tree_node {
	unsigned 			nr_children;
	char 				name[1];
	struct tree_node 	*children;
};

void printStatus(char *, int, int);
void childFunction(char *);
static void __printTree(struct tree_node*, int);
void printTree(struct tree_node*);

static void __printTree(struct tree_node *root, int level) {
	int i;
	for (i = 0; i < level; i++)
		printf("\t");
	printf("%s\n", root->name);
	
	for (i=0; i < root->nr_children; i++)
		__printTree(root->children + i, level + 1);
}

void printTree(struct tree_node *root) {
	__printTree(root,0);
}

//aux function takes status 0,1,2 respectively as spawned, waiting, terminated
void printStatus(char * letterId, int status, int pid)
{
	int returnCode;
	if (letterId == "A") {
		returnCode = 2;
	} else if (letterId == "B") {
		returnCode = 4;
	} else if (letterId == "C") {
		returnCode = 6;
	} else if (letterId == "D") {
		returnCode = 10;
	}
	switch(status)
	{
		case 0: 
			fprintf(stderr,"Process %s spawned: PID = %d\n", letterId, pid);
			sleep(2);
			break;
		case 1:
			fprintf(stderr,"Process %s waiting: PID = %d\n", letterId, pid);
			sleep(2);
			break;
		case 2:
			fprintf(stderr,"Process %s terminated: PID = %d , RC = %d\n", letterId, pid,returnCode);
			sleep(2);
			break;
			
	}
}

void childFunction(char * letterId) {
	
	printf("Hi, I am process %s performing my child function!\n", letterId);
	sleep(2);
	
}

struct tree_node* tree_node_new(char * letter, struct tree_node* parent) { 
  struct tree_node* p = malloc(sizeof(struct tree_node));
  p->nr_children = 0;
  memcpy(p->name, letter, 1);
  if (letter != "A") {
	  parent->nr_children++;
	  parent->children = p;
  }
  return p;
}

void deleteChild(struct tree_node * parent) {
	parent->nr_children--;
	free(parent->children);
}

int workflow() {
	pid_t pid, rootpid;
	
	//Let current process be root process A.
	rootpid = getpid();
	printStatus("A", 0, getpid());
	struct tree_node * A;
	printTree(A = tree_node_new("A",NULL));
	
	//spawn B
	pid = fork();
	
	if (pid < 0) {
		
		//Error
		fprintf(stderr, "Fork Failed.\n");
		exit(1);
		
	} else if (pid > 0) {
		
		//Parent A
		printStatus("A", 1, getpid());
		waitpid(-1,NULL,0);
		//printf("Now I can create process C\n");
		
		//spawn C
		pid = fork();
		
		if (pid < 0) {
			
			//Error
			fprintf(stderr, "Fork Failed.\n");
			return EXIT_FAILURE;
			
		} else if (pid > 0) {
			
			//Parent A
			printStatus("A", 1, getpid());
			waitpid(-1,NULL,0);
			childFunction("A");
			
			//Kill A
			printStatus("A", 2, getpid());
			exit(0);
			
		} else {
			//Child C
			printStatus("C",0, getpid());
			struct tree_node * C = tree_node_new("C",A);
			printTree(A);
			childFunction("C");
			
			//Kill C
			printStatus("C", 2, getpid());
			deleteChild(A);
			printTree(A);
			exit(0);
		}
	} else {
		
		//Child B
		printStatus("B", 0, getpid());
		struct tree_node * B = tree_node_new("B",A);
		printTree(A);
		
		//spawn D
		pid = fork();
		
		if (pid < 0) {
			
			//Error
			fprintf(stderr, "Fork Failed.\n");
			return EXIT_FAILURE;
			
		} else if (pid > 0) {
			
			//Parent B
			printStatus("B", 1, getpid());
			waitpid(-1, NULL, 0);
			childFunction("B");
			
			//Kill B
			printStatus("B", 2, getpid());
			deleteChild(A);
			printTree(A);
			exit(0);
			
		} else {
			
			//Child D
			printStatus("D", 0, getpid());
			childFunction("D");
			struct tree_node * D = tree_node_new("D",B);
			printTree(A);
			
			//kill(rootpid,SIGKILL);
			//Adding a kill to the root process in the middle of the workflow
			//causes the leftover child processes to become orphans under init and terminate.
			
			//Kill D
			printStatus("D", 2, getpid());
			deleteChild(B);
			printTree(A);
			exit(0);
			
		}
	}
}
int main() {
	int exitcode = workflow();
 }  
