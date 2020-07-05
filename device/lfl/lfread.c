/* lfread.c  -  lfread */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  lfread  -  Read from a previously opened local file
 *------------------------------------------------------------------------
 */
devcall	lfread (
	  struct dentry	*devptr,	/* Entry in device switch table	*/
      char *buff,   /* Buffer to hold bytes */
      int32 count   /* Max bytes to read */
	)
{
    struct lfcblk *lfptr; /* Ptr to an open file table entry */
    uint32 numRead; /* Number of Bytes read */
    int32 nextByte; /* Character or EOF */
    int32 j;

    lfptr = &lftab[devptr->dvminor];

    for (j = 0; j < NDESC; j++) {
		if (proctab[currpid].prdesc[j] == lfptr->lfdev) {
 			break;
 		}
	}

	if (j == NDESC) {
		return SYSERR;
	}

    if (lfptr->lfmode != rmode) {
        return SYSERR;
    }

    if ((count <= 0) || (lfptr->lfoffset >= lfptr->lfinode->filestat.size)) {
        return SYSERR;
    }

    /* Use lfgetc to read bytes */
    
    for (numRead = 0; numRead < count; numRead++) {
        nextByte = lfgetc(devptr);
        *buff++ = (char) (0xff & nextByte);
        if (nextByte == EOF) { /* unexpected EOF */
            break;
        }
    }
    
    return numRead;
}