/* lmfctrl.c  -  lmfctrl */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  lmfctrl  -  Control function for lfs device
 *------------------------------------------------------------------------
 */
devcall	lmfctrl (
	  struct dentry	*devptr,	/* Entry in device switch table	*/
	  int32 func,			/* Function to execute	*/
	  int32	arg1,			/* First argument to function func */
      int32 arg2            /* Second argument to function func */
	)
{
    return OK;
}