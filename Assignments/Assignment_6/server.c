#include <stdio.h>
#include "server.h"
#include <rpc/rpc.h>

int * decrementCookie();

// create total amount of cookies
int cookieCount = 20;
// if this is set to 0, we set it back to 2
int tinaCount = 2;

int * get_cookie_1_svc(struct input *argp, struct svc_req *rqstp) {
    // Make sure that Tina gets her priority
    if(argp->name == 'J') {
        if (tinaCount == 0 ) {
            return decrementCookie();
        }
        else if(tinaCount > 0) {
            // Judy has to wait for Tina to have at least 2
            return (int *)-1;
        }
    }
    else {
        return decrementCookie();
    }    

    // We should never reach this point
    return 0;
}

int * decrementCookie() {
    if(cookieCount == 0) {
        // We are all out of cookies
        return (int *)-2;
    }
    else {
        // Hand out a cookie
        printf("A cookie has been given out. There are now %i cookies", cookieCount);
        cookieCount--;
        return (int *)1;
    }
}