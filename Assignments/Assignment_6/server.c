#include <stdio.h>
#include "server.h"
#include <rpc/rpc.h>

// create total amount of cookies
int cookieCount = 20;
// if this is set to 0, we set it back to 2
int tinaCount = 2;

int * getCookie_1_svc(struct input *argp, struct svc_req *rqstp) {
    // Make sure that Tina gets her priority
    if(argp->name = 'Judy' ) {
        if (tinaCount == 0 ) {
            // Give Judy a cookie
            cookieCount--;
            return 1;
        }
        else if(tinaCount > 0) {
            // Judy has to wait for Tina to have at least 2
            return -1;
        }
    }
    
    if(cookieCount == 0) {
        // We are all out of cookies
        return -2;
    }
    else {
        // Hand out a cookie
        cookieCount--;
        return 1;
    }

    // We should never reach this point
    return 0;
}
