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
    uint32 numRead; /* Number of Bytes read */
    int32 nextByte; /* character or EOF or SYSERR */

    if (count < 0) {
        return SYSERR;
    }
    /* Use lfgetc to read bytes */
    for ( numRead = 0; numRead < count; numRead++) {
        nextByte = lflgetc(devptr);
        if (nextByte == SYSERR) { 
            return SYSERR;
        } 
        else if (nextByte == EOF) { /* unexpected EOF */
            if (numRead == 0) {
                return EOF;
            }
            else {
                return numRead;
            }
        }
        else {
            *buff++ = (char) (0xff & nextByte);
        }
    }
    return numRead;
}