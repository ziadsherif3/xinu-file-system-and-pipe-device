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
    int32 retval;

    lfptr = &lftab[devptr->dvminor];

    /* if the pointer is beyond the current block, setup a new block */

    if (lfptr->lfbyte >= &lfptr->lfdblock[RM_BLKSIZ]) {
        retval = lfsetup(lfptr, -1);
        if (retval == SYSERR) {
            return SYSERR;
        }
    }

    /* if apppending a byte to the file, increment file size */

    if (lfptr->lfoffset >= lfptr->lfinode->filestat.size) { /* re-check the comparison */
        lfptr->lfinode->filestat.size ++;
        /* dirty inode*/
    }
    *lfptr->lfbyte++ = ch;
    lfptr->lfoffset++;
    lfptr->lfdbdirty = TRUE;

    return OK;
}