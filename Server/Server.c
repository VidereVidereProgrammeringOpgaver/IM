
#include "Server.h"

// Table 25.4			Echo  client program using TCP

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
//#include <arpa/innet.h> // inet -> innet !Error!
#include <arpa/inet.h>
#include <sys/wait.h>

int main (int argc, char* argv[  ])		// Three arguments to be checked later
{
	// Declare and define
	int s;						// Socket descriptor
	int  n;						// Number of bytes in each recv call
	char* servName;				// Server name
	int servPort;					// Server port number
	char* string;					// String to be echoed
	int len;						// Length of string to be echoed
	char buffer [256 + 1];			// Buffer
	char* ptr = buffer;    		// Pointer to move along the buffer
	struct sockaddr_in serverAddr;	// Server socket address
	int maxLen = 256; // !ADD!

	// Check and set arguments
	//if (argc != 3)
	if (argc != 4)
	{
		printf ("Error: three arguments are needed!");
		exit (1);
	}
	//servName = arg[1]; // !Error!
	servName = argv[1];
	//servPort = atoi(arg [2]); // !Error!
	servPort = atoi(argv[2]);
	//string = arg [3]; // !Error!
	string = argv[3];

	struct sockaddr_in servAddr; // !ADD!


	/*c
	// Create socket
	//if((s = socket (PF_INET, SOCK_STREAM, 0) < 0);
	s = socket(AF_INET, SOCK_STREAM, 0);
	if(s < 0)
	{
		perror ("Error: socket creation failed!");
		exit (1);
	}
	bd/*

	// Create remote (server) socket address
	//memset (&servAddr, 0, sizeof(servAddr));
	memset (&servAddr, '0', sizeof(servAddr));
	serverAddr.sin_family = AF_INET;
	inet_pton (AF_INET, servName, &serverAddr.sin_addr); // Server IP address
	serverAddr.sin_port = htons (servPort);		// Server port number
*/
	// Create socket
    if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }
    /**/
    // Create remote (server) socket address
    memset(&servAddr, '0', sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(servPort);

    if(inet_pton(AF_INET, argv[1], &servAddr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }

    // Connect  to the server
    if( connect(s, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
		perror ("Error: connection failed!");
		exit (1);
    }

	// Connect  to the server
	//if (connect (sd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0);
/*	if (connect (s, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0)
	{

		perror ("Error: connection failed!");
		exit (1);
	}
	*/
	printf("connect\n");
	int dataSend=0;
	// Data transfer section
	//send (s, string, strlen(string), 0);
	dataSend = send (s, string, strlen(string), 0);
	//dataSend = send (s, "123", 4, 0);
	if(dataSend==-1)
	{
		printf("dataSend==-1 \n");
		perror("send");
	}
	else
	{
		printf("else \n");
	}
	printf("while \n");

	if(( n = recv( s, buffer, sizeof( buffer ), 0 ) ) <= 0 )
	{
		perror("recv");
	}
	else
	{
		printf("Recv %d byte \n", n);
	}

	/*
	while ((n = recv (s, ptr, maxLen, 0)) > 0)
	{
		//ptr + = n;		// Move pointer along the buffer
		ptr += n;		// Move pointer along the buffer
		//maxLen - = n;	// Adjust the maximum number of bytes
		maxLen -= n;	// Adjust the maximum number of bytes
		len += n;		// Update the length of string received
		printf("while1 \n");
	} // End of while loop
	*/

	// Print and verify the echoed string
	//buffer [len] = Â’\0Â’;
	buffer [n] = '\0';
	printf ("Echoed string received: \n");
	fputs (buffer, stdout);
	printf ("\n");

	// Close socket
	close (s);

	// Stop program
	exit (0);

} // End of echo client program
