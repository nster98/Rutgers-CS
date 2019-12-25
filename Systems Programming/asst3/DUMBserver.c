/* Nathan Glikman	nmg116
 * Andrew Cater		ajc398
 * Asst3: DUMB
 *
 *
 * DUMBserver.c
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <netdb.h>

#define BUFFER_SIZE 1024

#define true 1
#define false 0

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

char buffer[BUFFER_SIZE];
char client_message[2000];

struct sockaddr_in serverAddr;

struct socketArgs 
{
	int socket;
	struct sockaddr_in addr;
};

typedef struct message
{
	char* msg;
	struct message *next;
} Message;

typedef struct box 
{
	char name[25];
	Message *firstMsg;
	int isOpen;
	struct box *next;
} Box;

Box* firstBox;

// Format and print to stdout and stderr for each client command
// TODO: Print to stderr
void printMessage(char* msg, struct sockaddr_in client)
{
	time_t now;
	time(&now);

	if (msg[0] == 'E' && msg[1] == 'R' && msg[2] == ':')
	{		
		fprintf(stderr, "%s ", strtok(ctime(&now), "\n"));
		fprintf(stderr, "%s ", inet_ntoa(client.sin_addr));
		fprintf(stderr, "%s\n", msg);
	}
	else
	{
		fprintf(stdout, "%s ", strtok(ctime(&now), "\n"));
		fprintf(stdout, "%s ", inet_ntoa(client.sin_addr));
		fprintf(stdout, "%s\n", msg);
	}
}
void * handleClient(void* args_);//Thread Handler

// Helper function to see how many arguments there are
int getWhiteSpaces(char* str)
{
	int whiteSpaces = 0;
	for (int i = 0; i < strlen(str); i++)
	{
		if (str[i] == ' ')
			whiteSpaces++;
	}
	return whiteSpaces;
}

// Make each argument a token for easy usage
char** tokenizeString(char* str)
{
	int whiteSpaces = getWhiteSpaces(str);
	char* retStr[whiteSpaces + 1];

	char* ptr = strtok(str, " ");

	int i = 0;
	while (ptr != NULL)
	{
		retStr[i] = ptr;
		ptr = strtok(NULL, " ");
		i++;
	}

	return retStr;
}
char** tokenizeString2(char* str)
{
	int whiteSpaces = getWhiteSpaces(str) + 1;
	char** retStr = malloc(sizeof(char*)*whiteSpaces);
	for(int i = 0; i < whiteSpaces; i++)
	{
		retStr[i] = malloc(sizeof(char)*50);
	}
	char* ptr = strtok(str, " ");
	int i = 0;
	while(ptr != NULL)
	{
		retStr[i] = ptr;
		ptr = strtok(NULL, " ");
		i++;
	}

	return retStr;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Error: Missing PORT argument\n");
		return -1;
	}
	//TODO: Make the empty struct of the message box
	//			Should be probably a linked list of linked lists where the first layer is each box and the second layer is the messages
	pthread_t threadNumber =0;	
	int serverSocket;
	int newSocket;

	//Changed Address to Global Variable
	struct sockaddr_storage serverStorage;

	socklen_t addr_size;
	
	// Create socket
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (serverSocket == 0)
	{
		perror("Socket failed to create\n");
		exit(EXIT_FAILURE);
	}

	// Let it resuse address
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0)
	{
		perror("setsockopt(SO_REUSEADDR) failed");
	}

	char ip[25];
	char newIp[25];
	ip[24] = '\0';
	gethostname(ip, 25);
	struct hostent* host;
	host = gethostbyname(ip);
	snprintf(newIp, 25, "%s", inet_ntoa(*(struct in_addr *)host->h_addr_list[0]));
	
	// Configure settings of server address struct
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[1]));
	serverAddr.sin_addr.s_addr = inet_addr(newIp);
	memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

	// Bind the address struct to the socket
	if (bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
	{
		perror("Socket failed to bind");
		exit(EXIT_FAILURE);
	}


	// Listen on the socket with max amount of connections to be 10
	if (listen(serverSocket, 10) < 0)
	{
		perror("Socket failed to listen");
		exit(EXIT_FAILURE);
	}

	//TODO: The function to pass to the thread should be doing it only when we open a new message box? That way only one person can fuck with it
	addr_size = sizeof(serverStorage);


	firstBox = (Box*) malloc(sizeof(Box));
	firstBox->next = NULL;
	firstBox->isOpen = false;

	while(1)
	{
		while((newSocket = accept(serverSocket, (struct sockaddr *) &serverAddr, &addr_size)) != -1)
		{
			if (newSocket > 0)
			{
				struct socketArgs *passingArgs = (struct socketArgs *) malloc(sizeof(struct socketArgs));
				passingArgs->socket = newSocket;
				passingArgs->addr = serverAddr;
					
				pthread_create(&threadNumber,NULL,handleClient,(void*)passingArgs);
				threadNumber++;
			}

		}

		// newSocket is the client socket
		// TODO: Once accepted, make a new thread to handle the client commands, will probably mean turning everything below into a function
	}
}

void * handleClient(void* args_)
{
	int newSocket = ((struct socketArgs*)args_)->socket;
	struct sockaddr_in serverAddr = ((struct socketArgs*)args_)->addr;
	int whiteSpaces;
	char** tokens = malloc(sizeof(char*)*(1+whiteSpaces));
	char* boxName;
	Box* openBox;
	int isABoxOpen = false;
	// Initialize the head of the message box to help point to every other box that will be created
	// Probably put this above accepting the new client so we can have this for every thread to access
	while (1)
	{
		// Accept calls and create new sockets
		
		read(newSocket, buffer, BUFFER_SIZE);
		//printf("%s\n", buffer);

		// MESSAGES:
		// 1. HELLO
		// 2. GDBYE
		// 3. CREAT arg0
		// 4. OPNBX arg0
		// 5. NXTMG
		// 6. PUTMG!arg0!bytes
		// 7. DELBX arg0
		// 8. CLSBX arg0
		whiteSpaces = getWhiteSpaces(buffer);
		tokens = tokenizeString2(buffer);

		char str[1000];
		
		// 1. HELLO
		// Should get this from the client immediately after it connects
		// Send back the correct message to let the client know the server is ready to accept commands
		if (strcmp(buffer, "HELLO") == 0)
		{
			// Send back "HELLO DUMBv0 READY" to the client
			
			printMessage("connected", serverAddr);
			printMessage(buffer, serverAddr);

			char *sendBack = "HELLO DUMBv0 ready";
			send(newSocket, sendBack, strlen(sendBack), 0);
		}
		// 2. GDBYE
		// Should close connection to the client, but the server is still running after that
		else if (strcmp(tokens[0], "GDBYE") == 0)
		{
			// Do goodbye things
		
			// TODO: CLOSE BOX IF OPEN
			
			if (isABoxOpen == true)
			{
				openBox->isOpen = false;
			}

			printMessage(buffer, serverAddr);
			printMessage("disconnected", serverAddr);
			close(newSocket);
			pthread_exit(NULL);
			//exit(0);
		}
		// 3. CREAT arg0
		else if (strcmp(tokens[0], "CREAT") == 0)
		{
			// Need to check for arg0, possibly check first 5 chars and then the next until the end

			// First go through each box starting from firstBox to see if theres a name that already exists
			// If so, send back ER:EXIST
			// If not, create new box
			
			int existFlag = 0;	
			if (whiteSpaces == 1)
			{
				if ((tokens[1][0] >= 'a' && tokens[1][0] <= 'z') || (tokens[1][0] >= 'A' && tokens[1][0] <= 'Z'))
				{
					// Loop through every box that exists to see if one with the same name already exists
					for (Box* errorPtr = firstBox; errorPtr != NULL; errorPtr = errorPtr->next)
					{
						if (strcmp(tokens[1], errorPtr->name) == 0)
						{
							snprintf(str, 1000, "Error: Message box '%s' already exists", tokens[1]);
							send(newSocket, str, strlen(str), 0);
							printMessage("ER:EXIST", serverAddr);
							existFlag = 1;
							break;
						}
					}
				}
				else
				{
					snprintf(str, 1000, "Error: Message boxes must start with an alphabetical letter");
					send(newSocket, str, strlen(str), 0);
					printMessage("ER:WHAT?", serverAddr);
					existFlag = 1;
				}
			}
			// If we didn't find one that exists, we can create the new box
			if (whiteSpaces == 1 && existFlag == 0)
			{
				for (Box* ptr = firstBox; ptr != NULL; ptr = ptr->next)
				{
					if (ptr->next == NULL)
					{
						// Set up the new box with the requested name
						Box* newBox = (Box*) malloc(sizeof(Box));
						strcpy(newBox->name, tokens[1]);
						newBox->next = NULL;
						newBox->firstMsg = NULL;

						ptr->next = newBox;

						snprintf(str, 1000, "Success: Created box '%s'", newBox->name);
						send(newSocket, str, strlen(str), 0);
						printMessage(buffer, serverAddr);

						break;
					}
				}
				
			}

			// If there wasn't only one argument then we send an error of WHAT?
			if (whiteSpaces != 1)
			{
				snprintf(str, 1000, "Error: Command was unsuccessful. Please try again");
				send(newSocket, str, strlen(str), 0);
				printMessage("ER:WHAT?", serverAddr);
			}
		}
		// 4. OPNBX
		// When we open a box, this is where we should use a mutex, since only that thread gets to work with that box
		// Not sure how the mutex interacts with the linked list
		// If a thread is already working with a box and a different thread tries to access it, its ok just send an error to the client
		else if (strcmp(tokens[0], "OPNBX") == 0)
		{
			// MUTEX only if found

			int foundFlag = false;
			for (Box* ptr = firstBox; ptr != NULL; ptr = ptr->next)
			{
				// This is the EXTRA CREDIT (i think)
				// ER:BXALO -> Error: Box already open
				if (isABoxOpen == true)
				{
					
					snprintf(str, 1000, "Error: A message box is currently open");	
					send(newSocket, str, strlen(str), 0);
					printMessage("ER:BXALO", serverAddr);
					foundFlag = true;
					break;
				}
				if (strcmp(ptr->name, tokens[1]) == 0 && ptr->isOpen == false)
				{
					openBox = ptr;
					foundFlag = true;
					ptr->isOpen = true;

					isABoxOpen = true;
					boxName = ptr->name;

					snprintf(str, 1000, "Success: Opened box '%s'", ptr->name);
					send(newSocket, str, strlen(str), 0);
					printMessage(tokens[0], serverAddr);

					break;
				}
				else if (strcmp(ptr->name, tokens[1]) == 0 && ptr->isOpen == true)
				{
					foundFlag = true;
					
					snprintf(str, 1000, "Error: Message box '%s' is already opened", ptr->name);
					send(newSocket, str, strlen(str), 0);
					memset(str, '\0', 100);
					printMessage("ER:OPEND", serverAddr);
					break;
				}
			}
			
			if (foundFlag == false)
			{
				snprintf(str, 1000, "Error: Message box '%s' does not exist", tokens[1]);
				send(newSocket, str, strlen(str), 0);
				printMessage("ER:NEXST", serverAddr);
			}	
		}
		// 5. NXTMG
		// When the client asks for the next message, the server sends it then deletes it from the linked list
		// Basically, delete from the front and make the next message the head if there is one
		else if (strcmp(tokens[0], "NXTMG") == 0)
		{
			// If this is the first time using next message on the open box, make the firstmessage pointer
			if (openBox->firstMsg == NULL)
			{
				openBox->firstMsg = (Message *) malloc(sizeof(Message));
				openBox->firstMsg->next = NULL;
			}
			// If the first message is NULL, send error
			if (openBox->firstMsg->next == NULL)
			{
				snprintf(str, 1000, "Error: No messages found");
				send(newSocket, str, strlen(str), 0);
				printMessage("ER:EMPTY", serverAddr);
			}
			// Send error if box is already open
			else if (openBox->isOpen == false)
			{
				snprintf(str, 1000, "Error: No message box is open, can not get next message");
				send(newSocket, str, strlen(str), 0);
				printMessage("ER:NOOPN", serverAddr);
			}
			else
			{
				char* sendMsg = openBox->firstMsg->next->msg;
				send(newSocket, sendMsg, strlen(sendMsg), 0);
				printMessage(tokens[0], serverAddr);

				Message* ptr = openBox->firstMsg->next;
				openBox->firstMsg->next = openBox->firstMsg->next->next;
				free(ptr);
					
				//for (Message* ptr = openBox->firstMsg; ptr != NULL; ptr = ptr->next)
				//{
					
				//}
			}

		}	
		// 6. PUTMG
		// The client will put a new message into the back of the linked list
		// Loop through the message list until the next is null and make that the new message
		else if (strcmp(tokens[0], "PUTMG") == 0)
		{
			char** tempStr = malloc(sizeof(char*)*(whiteSpaces));
			for(int i = 0; i < whiteSpaces +1; i++)
			{
				//malloc is dumb stupid and I dislike it.
				//Don't question this plz
				tempStr[i] = malloc(sizeof(char)*50);
				tempStr[i] = tokens[i];
			}
			if (openBox->firstMsg == NULL)
			{
				openBox->firstMsg = (Message *) malloc(sizeof(Message));
				openBox->firstMsg->next = NULL;
			}
			if (isABoxOpen == false || openBox->isOpen == false)
			{
				snprintf(str, 1000, "Error: No message box is open, can not put message");
				send(newSocket, str, strlen(str), 0);
				printMessage("ER:NOOPN", serverAddr);
			}
			else
			{
				for (Message* ptr = openBox->firstMsg; ptr != NULL; ptr = ptr->next)
				{
					if (ptr->next == NULL)
					{
						Message* newMsg = (Message *) malloc(sizeof(Message));

						char* newStr = (char *)malloc(1024);

						if (whiteSpaces == 1)
						{
							strcat(newStr, tempStr[1]);
						}
						else
						{
							for (int i = 1; i <= whiteSpaces; i++)
							{
								strcat(newStr, tempStr[i]);
								if (i < whiteSpaces)
								{
									strcat(newStr, " ");
								}
							}
						}

						newMsg->msg = newStr;
						newMsg->next = NULL;

						ptr->next = newMsg;

						char* sendStr[1024];
						snprintf(sendStr, 1024, "OK!%d", strlen(newStr));
						//itoa(strlen(tokens[1]), sendStr, 10);
						//strcat(sendStr, itoa(strlen(tokens[1]))); // Get length of entire message from above

						send(newSocket, sendStr, strlen(sendStr), 0);
						printMessage("PUTMG", serverAddr);

						break;
					}
				}
			}
		}
		// 7. DELBX
		// Delete the box
		// Instructions shown below	
		else if (strcmp(tokens[0], "DELBX") == 0)
		{
			// How to delete:
			// Check that we only have one argument, if not say WHAT? (whitespaces)
			// Go through each box and check for name equality
			// If firstMsg = NULL then we can delete
			// If theres something there then ER:NOTMT
			// If name equals then delete
			// After going through if we didnt find anything ER:NEXST
			// Send OK! if we successfully delete
			
			int existFlag = 0;
			if (whiteSpaces == 1)
			{
				// Get prevPtr for possible deletion
				Box* prevPtr = firstBox;
				for (Box* errorPtr = firstBox->next; errorPtr != NULL; errorPtr = errorPtr->next)
				{
					// Check if the name exists
					if (strcmp(tokens[1], errorPtr->name) == 0)
					{
						existFlag = 1;
						//checks to see if its your box
						if(errorPtr->isOpen == 0)
						{
							// Is the message box empty?
							if (errorPtr->firstMsg == NULL || errorPtr->firstMsg->next == NULL)
							{
								// Its ok to delete now
								if (errorPtr == NULL) // Dont know if this block is irrelavant or not (it probably is)
								{
									firstBox->next = NULL;
									free(errorPtr);
								}
								else
								{
									prevPtr->next = errorPtr->next;
									free(errorPtr);
								}

								send(newSocket, "OK!", 3, 0);
								printMessage(buffer, serverAddr);

								break;
							}
							// Message box is not empty, send error and dont delete
							else
							{
								snprintf(str, 1000, "Error: Message box is not empty, can not delete");
								send(newSocket, str, strlen(str), 0);
								printMessage("ER:NOTMT", serverAddr);
								break;
							}
						}
						else
						{
							snprintf(str, 1000, "Error: Message box '%s' is in use and can not be deleted", errorPtr->name);
							send(newSocket, str, strlen(str), 0);
							printMessage("ER:INUSE", serverAddr);
							break;
						}
					}
						
					// Get the next prevPtr for the next loop
					prevPtr = prevPtr->next;
				}

				// The name does not exist so we can't delete it, send a message
				if (existFlag == 0)
				{
					snprintf(str, 1000, "Error: Message box '%s' does not exist", tokens[1]);
					send(newSocket, str, strlen(str), 0);
					printMessage("ER:NEXST", serverAddr);
				}
			}
			// Wtf did you write you dummy
			else
			{
				snprintf(str, 1000, "Error: Command was unsuccessful, please try again");
				send(newSocket, str, strlen(str), 0);
				printMessage("ER:WHAT?", serverAddr);
			}

		}
		// 8. CLSBX
		// Unlock the mutex	from OPNBX, allowing other threads to now open the box and use it
		else if (strcmp(tokens[0], "CLSBX") == 0)
		{
			// UNLOCK HERE

			int foundFlag = false;

			if (whiteSpaces == 0)
			{
				snprintf(str, 1000, "Error: Command was unsuccessful, please try again");
				send(newSocket, str, strlen(str), 0);
				printMessage("ER:WHAT?", serverAddr);
				foundFlag = true;
			}
			else
			{
				for (Box* ptr = firstBox; ptr != NULL; ptr = ptr->next)
				{	
					if (strcmp(ptr->name, tokens[1]) == 0)
					{
						foundFlag = true;
						//checks to see if its your box
						if(boxName != NULL&&strcmp(boxName, ptr->name)==0)
						{
							if (ptr->isOpen == true)
							{
								ptr->isOpen = false;
								isABoxOpen = false;
							}
							else
							{
								snprintf(str, 1000, "Error: Message box '%s' is not opened and can't be closed", boxName);
								send(newSocket, str, strlen(str), 0);
								printMessage("ER:NOOPN", serverAddr);
								break;	
							}
	
							snprintf(str, 1000, "Success: Message box '%s' has been closed", boxName);
							send(newSocket, str, strlen(str), 0);
							printMessage(tokens[0], serverAddr);
							break;
						}
						else
						{	
							snprintf(str, 1000, "Error: Message box '%s' is in use and can't be closed", tokens[1]);
							send(newSocket, str, strlen(str), 0);
							printMessage("ER:INUSE", serverAddr);
							break;
						}
					}
				}
			}
			if (foundFlag == false)
			{
				snprintf(str, 1000, "Error: Message box '%s' could not be found", tokens[1]);
				send(newSocket, str, strlen(str), 0);
				printMessage("ER:NOOPN", serverAddr);
			}
		}
		else if (strcmp(tokens[0], "HELP!") == 0)
		{
			send(newSocket, "quit\t\tClose connection to the server\ncreate\t\tCreate a new message box\ndelete\t\tDelete a message box\nopen\t\tOpen a message bonx\nclose\t\tClose the currently open box\nnext\t\tGo to next message\nput\t\tPut a new message into the open box\n",239,0);
			printMessage("HELP!", serverAddr);
		}
		// Any other thing sent that the client decided to type gets back sent a WHAT?
		else
		{
			// Send back "WHAT?" to the client
			snprintf(str, 1000, "Error: Command was unsuccessful, please try again");
			send(newSocket, str, strlen(str), 0);
			printMessage("ER:WHAT?", serverAddr);
		}

		// Reset the buffer for new input
		memset(buffer, '\0', BUFFER_SIZE);
		memset(str, '\0', 100);

	}
}

