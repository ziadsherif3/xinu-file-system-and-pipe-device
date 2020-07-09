/* pipwrite.c  -  pipwrite */

#include <xinu.h>

/*---------------------------------------------------------------
 *  pipwrite  -  Write data to a previously opened pipe
 *---------------------------------------------------------------
 */
devcall	pipwrite (
	  struct dentry	*devptr,	/* Entry in device switch table	*/
      char *buff,               /* Buffer holding data to write */
      int32 count               /* Number of bytes to write */
	)
{
    struct pipeblk *pipeptr;    /* Pointer to a pipe control block */
    int32 i;                    /* General loop index */

    pipeptr = &pipetab[devptr->dvminor];

    for (i = 0; i < NDESC; i++) {
		if (proctab[currpid].prdesc[i] == pipeptr->pdev) {
 			break;
 		}
	}

	if (i == NDESC) {
		return SYSERR;
	}

    wait(pipeptr->pipsem);

    if ((pipeptr->pstate == FREE) || (pipeptr->pmode != wmode)) {
        signal(pipeptr->pipsem);
        return SYSERR;
    }

    signal(pipeptr->pipsem);

    i = pipeptr->wpointer;

    while ((count--) > 0) {
        if (i == PIPEBUFFSIZE) {
            i = 0;
            signal(pipeptr->fsembuff);
            wait(pipeptr->esembuff);
        }
        pipeptr->pbuff[i++] = *buff++;
    }

    if (i == PIPEBUFFSIZE) {
        i = 0;
        signal(pipeptr->fsembuff);
        wait(pipeptr->esembuff);
    }

    pipeptr->pbuff[i] = EOF;
    pipeptr->wpointer = i;

    return OK;
}