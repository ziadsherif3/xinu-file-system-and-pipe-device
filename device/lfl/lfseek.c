/* lfseek.c  -  lfseek */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  lfseek  -  Seek to a specified position in a file
 *------------------------------------------------------------------------
 */
devcall	lfseek (
	  struct dentry	*devptr,	/* Entry in device switch table	*/
      int32 offset      /* Byte position in the file */
	)
{
    struct lfcblk *lfptr; /*pointer to open file table entry */

    /* If file is not open, return an error */

    lfptr = &lftab[devptr->dvminor];
    wait(lfptr->lfmutex);
    if (lfptr->lfstate != USED) {
        signal(lfptr->lfmutex);
        return SYSERR;
    }

    /* Verify offset is within the current file size */
    if (offset > lfptr->lfinode->filestat.size)
    {
        signal(lfptr->lfmutex);
        return SYSERR;
    }

    /* Record offset, and invalidate the byte pointer */
    lfptr->lfoffset = offset;
    lfptr->lfbyte = &lfptr->lfdblock[RM_BLKSIZ];
    signal(lfptr->lfmutex);
    return OK;
}