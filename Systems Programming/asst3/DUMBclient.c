/* Nathan Glikman	nmg116
 * Andrew Cater		ajc398
 * Asst3: DUMB
 *
 * DUMBclient.c
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#define PORT_NUM 23490
#define BUFFER_SIZE 1024

char buffer[BUFFER_SIZE];

char* translateInput(char* str)
{
	char* retStr;

	if (strcmp(str, "quit") == 0)
	{
		retStr = "GDBYE";
	}
	else if (strcmp(str, "create") == 0)
	{
		retStr = "CREAT";
	}
	else if (strcmp(str, "delete") == 0)
	{
		retStr = "DELBX";
	}
	else if (strcmp(str, "open") == 0)
	{
		retStr = "OPNBX";
	}
	else if (strcmp(str, "close") == 0)
	{
		retStr = "CLSBX";
	}
	else if (strcmp(str, "next") == 0)
	{
		retStr = "NXTMG";
	}
	else if (strcmp(str, "put") == 0)
	{
		retStr = "PUTMG";
	}	
	else if (strcmp(str, "help") == 0)
	{
		retStr = "HELP!";
	}
	else 
	{
		retStr = "WHAT?";
	}

	return retStr;
}
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
char** tokenizeString(char* str)
{
	int whiteSpaces = getWhiteSpaces(str);
	char* retStr[whiteSpaces + 1];

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
	if (argc != 3)
	{
		printf("Error: Please enter an IP Address and the Port to connect\n");
		return -1;
	}
	int clientSocket;

	struct sockaddr_in serverAddr;

	socklen_t addr_size;

	// Create socket
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (clientSocket < 0)
	{
		perror("Socket failed to create\n");
		return -1;
	}

	// Configure settings of the server address
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[2]));

	if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0)
	{
		perror("Invalid Address: Address Not Supported\n");
		return -1;
	}

	struct hostent* host;
	host = gethostbyname(argv[1]);
	char ip[25];
	snprintf(ip, 25, "%s", inet_ntoa(*(struct in_addr *)host->h_addr_list[0]));

	serverAddr.sin_addr.s_addr = inet_addr(ip);
	memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

	addr_size = sizeof(serverAddr);

	// Connect?
	if (connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size) < 0)
	{
		perror("Connection failed\n");
		return -1;
	}
	
	char inputstr[50];
	char* str; // Might be a problem later with the arg0

	int helloFlag = 0;

	while (1)
	{
		// Start by sending the hello message to the server
		// Check to see if we got the right response back
		// TODO: Do something if we didnt get the right response (The wait 3 times thing)
		if (helloFlag == 0)
		{
			send(clientSocket, "HELLO", 5, 0);
			read(clientSocket, buffer, BUFFER_SIZE);

			if (strcmp(buffer, "HELLO DUMBv0 ready") == 0)
			{
				printf(buffer);
				printf("\n");
			}

			helloFlag = 1;
			memset(buffer, '\0', BUFFER_SIZE);
		}

		// Wait for input from the user
		printf("> ");
		gets(inputstr);

		// Tokenize the string to translate for the server
		int whiteSpaces = getWhiteSpaces(inputstr);

		char** tokens = tokenizeString2(inputstr);
		str = translateInput(tokens[0]);
		
		char* sendStr = malloc(sizeof(char)*50);
		strcpy(sendStr, "");
		for (int i = 0; i < whiteSpaces + 1; i++)
		{
			if(i == 0)
				strcat(sendStr, str);
			else
			{
				strcat(sendStr, " ");
				strcat(sendStr, tokens[i]);
			}
		}

		// Send the clients message to the server
		// Dont send if client entered "help"
		if(strcasecmp(str, "help") != 0)
			send(clientSocket, sendStr, strlen(sendStr), 0);

		// Help
		// TODO: Make the server handle the help, since thats what it should be doing
		//working on this Andrew <3
		if (strcmp(str, "GDBYE") == 0)
		{
			printf("Closing connection to server: Goodbye!\n");
			exit(0);
		}

		// Read from the server
		// Dont read if client entered "help"
		if (strcasecmp(str, "help") != 0)
			read(clientSocket, buffer, BUFFER_SIZE);

		// Server will send back an appropriate response to what the client just asked, and will print out to the user
		printf("%s\n", buffer);

		memset(buffer, '\0', BUFFER_SIZE);
		memset(inputstr, '\0', 50);
	}

	return 0;
}
