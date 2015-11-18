#include <stdio.h>
#include "server.h"

int main(int argc, char *argv[]) {
	CLIENT *client;

	int *return_value;
	char *server;
	input *parameters = (input *)malloc(sizeof(input));


	/*
           We must specify a host on which to run.  We will get the host name
           from the command line as argument 1.
        */
         if (argc != 2) {
           fprintf(stderr, "Usage: %s host_name\n", *argv);
           exit(1);
         }
         server = argv[1];

	/*
           Generate the client handle to call the server
        */
	if ((client=clnt_create(server, ASSN6, DISPLAY_VER, "udp")) == (CLIENT *) NULL) {
           clnt_pcreateerror(server);
           exit(2);
        }

	// Call funcion
	printf("Judy: Calling function. \n");

        clnt_destroy(client);
}
