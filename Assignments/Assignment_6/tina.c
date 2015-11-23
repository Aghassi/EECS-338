#include <stdio.h>
#include "server.h"
#include <rpc/rpc.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
  struct timeval tv;
  CLIENT *client;

  char *server;
  input *parameters = (input *)malloc(sizeof(input));
  parameters->name = 'T';

  int totalCookies = -1;

  /*
   We must specify a host on which to run.  We will get the host name
   from the command line as argument 1.
  */
  if (argc != 2) {
     fprintf(stderr, "Usage: %s host_name\n", *argv);
     exit(1);
  }
  printf("%s\n", argv[1]);
  server = argv[1];

  /*
   Generate the client handle to call the server
  */
  printf("Creating client \n");
  client=clnt_create(server, ASSN6, DISPLAY_VER, "udp");
  if ( client == NULL) {
    clnt_pcreateerror(server);
    exit(2);
  }
  printf("Client created \n");

  // Call funcion

  // this is the value we get back from the server 
  printf("Creating return variable \n");
  int *ret;
  printf("Created return variable \n");

  // We loop and run at a 2 second interval until
  // there are no more cookies
  printf("entering while loop \n");
  while(*ret != -2) {
    sleep(2);
    gettimeofday(&tv, NULL);
    printf("Tina: Calling function. %03ld \n",  tv.tv_usec);
    ret = get_cookie_1(parameters, client);
    if (ret == NULL) {
        clnt_perror (client, "call failed");
    }
    else {
      totalCookies++;
      gettimeofday(&tv, NULL);
      printf("Tina: Got a cookie. \n");
    }
  }
  gettimeofday(&tv, NULL);
  printf("Tina: Mother says there are no more cookies. %03ld \n",  tv.tv_usec);
  gettimeofday(&tv, NULL);
  printf("Tina: I got %i cookies %03ld \n", totalCookies,  tv.tv_usec);

  free(parameters);
  clnt_destroy(client);
  return 0;
}
