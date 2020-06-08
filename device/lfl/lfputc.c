/* lfputc.c  -  lfputc */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  lfputc  -  Write a single byte to an open local file
 *------------------------------------------------------------------------
 */
devcall	lfputc (
	  struct dentry	*devptr,	/* Entry in device switch table	*/
      char ch   /* Character (byte) to write */
	)
{
    struct lfcblk *lfptr;

    lfptr = &lftab[devptr->dvminor];
    wait(lfptr->lfmutex);

    /* If file is not open, return an error */
    if (lfptr->lfstate != USED) { 
        signal(lfptr->lfmutex);
        return SYSERR;
    }
    /* Return SYSERR for an attempt to skip bytes beyond EOF */
    if (lfptr->lfoffset >= lfptr->lfinode->filestat.size) {
        signal(lfptr->lfmutex); 
        return SYSERR;
    }

    /* if the pointer is beyond the current block, setup a new block */
    if (lfptr->lfbyte >= &lfptr->lfdblock[RM_BLKSIZ]) {
        lfsetup(lfptr);
    }

    /* if apppending a byte to the file, increment file size */
    if (lfptr->lfoffset >= lfptr->lfinode->filestat.size) { /* re-check the comparison */
        lfptr->lfinode->filestat.size ++;
        /* dirty inode*/
    }
    *lfptr->lfbyte++ = ch; lfptr->lfoffset++;
    lfptr->lfdbdirty = TRUE;
    /* dirty data block */
    signal(lfptr->lfmutex);

    return OK;
}