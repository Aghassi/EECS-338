/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _SERVER_H_RPCGEN
#define _SERVER_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct input {
	char name;
};
typedef struct input input;

#define ASSN6 0x20000101
#define DISPLAY_VER 1

#if defined(__STDC__) || defined(__cplusplus)
#define get_cookie 1
extern  int * get_cookie_1(input *, CLIENT *);
extern  int * get_cookie_1_svc(input *, struct svc_req *);
extern int assn6_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define get_cookie 1
extern  int * get_cookie_1();
extern  int * get_cookie_1_svc();
extern int assn6_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_input (XDR *, input*);

#else /* K&R C */
extern bool_t xdr_input ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_SERVER_H_RPCGEN */
