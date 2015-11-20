#include <stdio.h>
#include "server.h"
#include <rpc/rpc.h>

int main(int argc, char *argv[]) {
  CLIENT *client;

  char *server;
  input *parameters = (input *)malloc(sizeof(input));
  parameters->name = 'J';


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

    // this is the value we get back from the server 
    int *ret = 0;
    // We loop and run at a 2 second interval until
    // there are no more cookies
    while(*ret != -2) {
      sleep(2);
      printf("Judy: Asking for cookie. \n");
      ret = (int *)get_cookie_1(parameters, client);
      if (ret == NULL) {
        clnt_perror (client, "call failed");
      }
      else if (*ret == -1) {
        printf("Judy: Waiting on Tina to get her second cookie.\n");
      }
      else {
        printf("Judy: Got a cookie. \n");
      }
    }
    printf("Tina: Mother says there are no more cookies. \n");

    clnt_destroy(client);
}