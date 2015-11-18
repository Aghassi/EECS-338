#include <stdio.h>
#include "server.h"
#include <rpc/rpc.h>

// create total amount of cookies
int cookieCount = 20;

int * getCookie_1_svc(struct input *argp, struct svc_req *rqstp)
{
        // all return values must be type static
	printf("Hello world");
}
