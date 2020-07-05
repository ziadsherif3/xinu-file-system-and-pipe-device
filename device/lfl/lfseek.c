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
    struct lfcblk *lfptr; /* pointer to open file table entry */
    int32 j;

    if (offset < 0) {
        return SYSERR;
    }

    lfptr = &lftab[devptr->dvminor];

    for (j = 0; j < NDESC; j++) {
		if (proctab[currpid].prdesc[j] == lfptr->lfdev) {
 			break;
 		}
	}

	if (j == NDESC) {
		return SYSERR;
	}

    /* Record offset, and invalidate the byte pointer */
    
    lfptr->lfoffset = offset;
    lfptr->lfbyte = &lfptr->lfdblock[RM_BLKSIZ];
    
    return OK;
}