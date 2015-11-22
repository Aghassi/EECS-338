#include <stdio.h>
#include "server.h"
#include <rpc/rpc.h>

int * decrementCookie();

// create total amount of cookies
int cookieCount = 20;
// if this is set to 0, we set it back to 2
int tinaCount = 2;

int outOfCookies = -2;
int judyWaits = -1;
int giveCookie = 1;

int * get_cookie_1_svc(input *argp, struct svc_req *rqstp) {
    if (tinaCount == 0) {
 	tinaCount = 2;
    }
    if(cookieCount == 0) {
        // We are all out of cookies
        return &outOfCookies;
    }

    // Make sure that Tina gets her priority
    if(argp->name == 'J') {
        if (tinaCount == 0 ) {
            return decrementCookie(argp);
        }
        else if(tinaCount > 0) {
            printf("Judy must wait for Tina to get 2 cookies \n");
            // Judy has to wait for Tina to have at least 2
            return &judyWaits;
        }
    }
    else {
    	// Reset the Tina's count if she hit 0
        return decrementCookie(argp);
    }    

    // We should never reach this point
    return 0;
}

/**
* A function for handing out cookies
**/
int * decrementCookie(input *argp) {
        cookieCount--;
        // Hand out a cookie
        printf("A cookie has been given out. There are now %i cookies \n", cookieCount);
	if (argp->name == 'T') {
		tinaCount--;
	}
        return &giveCookie;
}
