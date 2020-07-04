/* pipclose.c  -  pipclose */

#include <xinu.h>

/*--------------------------------------------------------------------------------
 *  pipclose  -  Close a pipe by placeing an EOF character or freeing device entry
 *--------------------------------------------------------------------------------
 */
devcall	pipclose (
	  struct dentry	*devptr	/* Entry in device switch table	*/
	)
{
	struct pipeblk *pipeptr;    /* Pointer to a pipe control block */
	intmask mask;			    /* Saved interrupt mask		*/
	int32 i;                    /* General loop index */

	pipeptr = &pipetab[devptr->dvminor];

	mask = disable();
	for (i = 0; i < NDESC; i++) {
		if (proctab[currpid].prdesc[i] == pipeptr->pdev) {
 			break;
 		}
	}

	if (i == NDESC) {
		restore(mask);
		return SYSERR;
	}

	proctab[currpid].prdesc[i] = -1;
	
	if ((pipeptr->pstate) == FREE) {
		restore(mask);
		return SYSERR;
	}
	
	if ((pipeptr->pmode) == wmode) { /* Writer process is done and will admit the reader process */
		pipeptr->pmode = rmode;
		if (!pipeptr->rdone) {
			signal(pipeptr->fsembuff);
		}
	}
	else if ((pipeptr->pmode) == pdmode) { /* Reader process is done and will free up the pipe control block */
		pipeptr->pstate = FREE;
	}

	restore(mask);
    return OK;
}