/* lfinit.c  -  lfinit */

#include <xinu.h>
#include <filesystem.h>
struct lfcblk lftab[Nlfl];
/*------------------------------------------------------------------------
 *  lfinit  -  Initialize control blocks for local file pseudo-devices
 *------------------------------------------------------------------------
 */
devcall	lfinit (
	  struct dentry	*devptr	/* Entry in device switch table	*/
	)
{
	struct lfcblk *lfptr; int32 i;
	lfptr = &lftab[devptr->dvminor];
	/* Initialize control block entry */
	lfptr->lfstate = FREE; /* Device is currently unused */ 
	lfptr->lfdev = devptr->dvnum; /* Set device ID */ 
	lfptr->lfmutex = semcreate(1); /* Create the mutex semaphore */

    return OK;
}