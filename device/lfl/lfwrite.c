/* lfwrite.c  -  lfwrite */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  lfwrite  -  Write data to a previously opened local disk file
 *------------------------------------------------------------------------
 */
devcall	lfwrite (
	  struct dentry	*devptr,	/* Entry in device switch table	*/
      char *buff,   /* Buffer holding data to write */
      int32 count   /* Number of bytes to write */
	)
{
    struct lfcblk *lfptr;
    int32 i; /* Number of Bytes written */
    int32 retval;
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

    if (lfptr->lfmode != wmode) {
        return SYSERR;
    }
    
    if (count <= 0) {
        return SYSERR;
    }

    lfptr = &lftab[devptr->dvminor];

    retval = lfsetup(lfptr, count);
    if (retval == SYSERR) {
        return SYSERR;
    }

    /* Iterate and write */
    
    for (i = 0; i < count; i++) {
        if (lfputc(devptr, *buff++) == SYSERR) {
            return SYSERR;
        }
    }
    
    return count;
}