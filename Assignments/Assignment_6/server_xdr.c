/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "server.h"

bool_t
xdr_input (XDR *xdrs, input *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->a))
		 return FALSE;
	 if (!xdr_char (xdrs, &objp->b))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_result (XDR *xdrs, result *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->status))
		 return FALSE;
	 if (!xdr_char (xdrs, &objp->name))
		 return FALSE;
	return TRUE;
}
