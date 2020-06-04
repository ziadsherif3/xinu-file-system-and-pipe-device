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
	struct lfcblk *lfptr;
	int32 i;
	lfptr = &lftab[devptr->dvminor];
	
	/* Initialize control block entry */
	
	lfptr->lfstate = FREE; /* Device is currently unused */ 
	lfptr->lfdev = devptr->dvnum; /* Set device ID */ 
	lfptr->lfmutex = semcreate(1); /* Create the mutex semaphore */
	lfptr->lfinode = (struct inode *) NULL;
	lfptr->lfdbdirty = FALSE;
	lfptr->lfoffset = 0;
	lfptr->lfdnum = 0;
	lfptr->lfbyte = &lfptr->lfdblock[RM_BLKSIZ];

	for (i = 0; i < NAME_LEN; i++) {
		lfptr->lfname[i] = NULLCH;
	}

	memset((char *) &lfptr->lfdblock, NULLCH, RM_BLKSIZ);
    
	return OK;
}