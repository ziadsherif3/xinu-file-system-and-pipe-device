/* lfclose.c  -  lfclose */

#include <xinu.h>
#include <filesystem.h>

/*------------------------------------------------------------------------
 *  lfclose  -  Close a file by flushing output and freeing device entry
 *------------------------------------------------------------------------
 */
devcall	lfclose (
	  struct dentry	*devptr	/* Entry in device switch table	*/
	)
{
	struct lfcblk *lfptr; /* pointer to the open filetable entry */
	lfptr = &lftab[devptr->dvminor];
	wait(lfptr->lfmutex);
	
	/* if the file is not open return error */

	if (lfptr->lfstate != USED) {
		signal(lfptr->lfmutex);
		return SYSERR;
	}

	/* Flush to disk if the data has been changed */
	
	if (lfptr->lfdbdirty) {
		lfflush(lfptr);
	}

	/* free up the device and return */
	lfptr->lfstate = FREE;
	signal(lfptr->lfmutex);
    return OK;
}