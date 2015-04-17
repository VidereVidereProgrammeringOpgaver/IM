// Table 25.3			Echo server program using TCP

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
//#include <arpa/innet.h> // inet -> innet !error!
#include <arpa/inet.h>
#include <sys/wait.h>
#include <pthread.h>

// compile with -lpthread
// gcc -o Table-25.3_new_th_v2 Table-25-3_new_th_v2.c -lpthread


struct arg
{
         int new_sd;
};


void* client_thread(void* newClient)
{
	int n;
	char buffer [256];				// Data buffer
	int new_sd;
	struct arg *my_arg;
	my_arg = (struct arg *)newClient;
	new_sd = my_arg->new_sd;
	printf("Client thread. New socket descriptor: = %d \n", new_sd);

	while(1)
	{
		// Data transfer section
		if(( n = recv( new_sd, buffer, sizeof( buffer ), 0 ) ) <= 0 )
		{
			//printf("Socket descriptor:%d, recv", new_sd);
            if( n == 0 )
            {
              printf( "Socket descriptor: %d hung up\n", new_sd );
              break;
            }
            else
            {
            	printf( "Socket descriptor: %d\n", new_sd );
            	perror("recv");
            }
		}
		else
		{
			printf("Socket descriptor:%d, Recv %d byte \n", new_sd, n);
		}
		/*
		while ((n = recv (s, ptr, maxLen, 0)) > 0)
		{
			//ptr + = n;			// Move pointer along the buffer
			ptr += n;			// Move pointer along the buffer
			//maxLen - = n;		// Adjust maximum number of bytes to receive
			maxLen -= n;		// Adjust maximum number of bytes to receive
			//len + = n;			// Update number of bytes received
			len += n;			// Update number of bytes received
		}
		*/
		int dataSend=0;
		//dataSend = send (s, buffer, len, 0);		// Send back (echo) all bytes received
		dataSend = send (new_sd, buffer, n, 0);		// Send back (echo) all bytes received
		if(dataSend==-1)
		{
			printf("Socket descriptor:%d, dataSend==-1 \n", new_sd);
			perror("send");
		}
		else
		{
			printf("Socket descriptor:%d, Send %d byte \n", new_sd, dataSend);
		}
	}


	// Close the socket
	printf("Close the socket:%d\n", new_sd);
	close (new_sd);
	pthread_exit; // end thread
}


int main (void)
{
	// Declare and define
	int ls;							// Listen socket descriptor (reference)
	int s;							// socket descriptor (reference)
	char buffer [256];				// Data buffer
	char* ptr = buffer;				// Data buffer
	int len = 0;						// Number of bytes to send or receive
	int maxLen = sizeof (buffer);		// Maximum number of bytes to receive
	int n = 0;						// Number of bytes for each recv call
	int waitSize = 16;					// Size of waiting clients
	struct sockaddr_in serverAddr;		// Server address
	struct sockaddr_in clientAddr;		// Client address
	//int clntAddrLen; 					// Length of client address
	pthread_t id_thread;
	struct arg my_arg;

	struct sockaddr_in servAddr; // !ADD!
	int SERV_PORT = 49999; // !ADD!



	// Create listen socket
	//if (ls = socket (PF_INET, SOCK_STREAM, 0) < 0);
	ls = socket (AF_INET, SOCK_STREAM, 0);
	if (ls < 0)
	{
		perror ("Error: Listen socket failed!");
		exit (1);
	}

	// Create local (server) socket address
	//memset (&servAddr, 0, sizeof (servAddr));
	memset(&servAddr, '0', sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	//servAddr.sin_addr.s_addr = htonl (INADDR_ANY);		// Default IP address
	servAddr.sin_addr.s_addr = INADDR_ANY;		// Default IP address
	servAddr.sin_port = htons (SERV_PORT); 			// Default port
	memset( &( servAddr.sin_zero ), '\0', 8 );

	// Bind listen socket to the local socket address
	//if (bind (ls, &servAddr, sizeof (servAddr)) < 0);
	if (bind (ls, ( struct sockaddr * ) &servAddr, sizeof (servAddr)) < 0)
	{
		perror ("Error: binding failed!");
		exit (1);
	}

	// Listen to connection requests
	//if (listen (ls, waitSize) < 0);
	if (listen (ls, waitSize) < 0)
	{
		perror ("Error: listening failed!");
		exit (1);
	}

	struct sockaddr_in clntAddr; // !ADD!
    socklen_t clntAddrLen = sizeof(clntAddr); // !ADD!
	//int clntAddrLen = sizeof(clntAddr); // !ADD!


	// Handle the connection
	for ( ; ; )		// Run forever
	{
		printf("\n\n-------------------------------\n");
		// Accept connections from client
		//if (s = accept (ls, &clntAddr, &clntAddrLen) < 0);
		if ((s = accept (ls, (struct sockaddr *)&clntAddr, &clntAddrLen)) < 0)
		{
			//perror ("Error: accepting failed!);
			perror ("Error: accepting failed!");
			exit (1);
		}
		my_arg.new_sd = s;
		printf("s = accept = %d \n", s);
		printf("Accept\n");
        // create a thread to handle request
        errno = pthread_create(&id_thread, NULL, client_thread, (void*)&my_arg);


	} // End of for loop

} // End of echo server program
