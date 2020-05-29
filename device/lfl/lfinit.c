/* lfinit.c  -  lfinit */

#include <xinu.h>
#include <filesystem.h>

/*------------------------------------------------------------------------
 *  lfinit  -  Initialize control blocks for local file pseudo-devices
 *------------------------------------------------------------------------
 */
devcall	lfinit (
	  struct dentry	*devptr	/* Entry in device switch table	*/
	)
{
    return OK;
}