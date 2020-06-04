/* lfgetc.c  -  lfgetc */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  lfgetc  -  Read the next byte from an open local file
 *------------------------------------------------------------------------
 */
devcall	lfgetc (
	  struct dentry	*devptr	/* Entry in device switch table	*/
	)
{
	struct lfcblk *lfptr; /* Ptr to open file table entry */

	/* Obtain Exclusive access to the file */

	lfptr = &lftab[devptr->dvminor];
	wait(lfptr->lfmutex);

	/* If file is not open, return an error */
	
	if (lfptr->lfstate != USED) { 
		signal(lfptr->lfmutex);
	}
	
	/* Return EOF for any attempt to read beyond the EOF */
	
	if (lfptr->lfoffset >= lfptr->lfinode->filestat.size) {
		signal(lfptr->lfmutex);
		return EOF;
	}
	
    return OK;
}