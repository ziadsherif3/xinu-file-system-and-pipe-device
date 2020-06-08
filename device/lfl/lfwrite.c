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
    int32 i; /* Number of Bytes written */
    if (count < 0) {
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