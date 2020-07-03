/* pipread.c  -  pipread */

#include <xinu.h>

/*---------------------------------------------------------------
 *  pipread  -  Read from a previously opened pipe
 *---------------------------------------------------------------
 */
devcall	pipread (
	  struct dentry	*devptr,	/* Entry in device switch table	*/
	  char *buff,			    /* Buffer to hold bytes */
	  int32 count   			/* Maximum number of bytes to read */
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

	if (pipeptr->pstate == FREE) {
        signal(pipeptr->pipsem);
        return SYSERR;
    }

	if (pipeptr->pmode == pdmode) {
		signal(pipeptr->pipsem);
		return EOF;
	}

	signal(pipeptr->pipsem);

	wait(pipeptr->fsembuff);

	i = 0;

	while ((pipeptr->pbuff[i] != EOF) && ((count--) > 0)) {
		if (i == PIPEBUFFSIZE) {
			i = 0;
			signal(pipeptr->esembuff);
			wait(pipeptr->fsembuff);
		}
		*buff++ = (char) (0xff & pipeptr->pbuff[i++]);
	}

	pipeptr->rdone = TRUE;
	
	if (pipeptr->wblocked) {
		signal(pipeptr->esembuff);
	}

	pipeptr->pmode = pdmode;

    return OK;
}