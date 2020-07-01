/* pipinit.c  -  pipinit */

#include <xinu.h>

/*---------------------------------------------------------------
 *  pipinit  -  Initialize control blocks for pipe pseudo-devices
 *---------------------------------------------------------------
 */
devcall	pipinit (
	  struct dentry	*devptr	/* Entry in device switch table	*/
	)
{
    return OK;
}