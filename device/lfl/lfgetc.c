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

    return OK;
}