/* pipopen.c  -  pipopen */

#include <xinu.h>

/*---------------------------------------------------------------
 *  pipopen  -  Open a free pipe pseudo-device
 *---------------------------------------------------------------
 */
devcall	pipopen (
	  did32		descrp,		/* Descriptor for device	*/
	  char		*name,		/* Name to use, if any		*/
	  char		*mode		/* Mode for device, if any	*/
	)
{
    struct pipeblk *pipeptr;    /* Pointer to a pipe control block */
    intmask mask;			    /* Saved interrupt mask		*/
    int32 i;                    /* General loop index */

    mask = disable();
    for (i = 0; i < NPIPE; i++) {
        pipeptr = &pipetab[i];
        if ((pipeptr->pstate) == FREE) {
            break;
        }
    }

    if (i == NPIPE) {
        restore(mask);
        return SYSERR;
    }

    pipeptr->pstate = USED;
    pipeptr->pmode = wmode;
    pipeptr->wpointer = 0;
    pipeptr->rpointer = 0;

    for (i = 5; i < NDESC; i++) { /* Find first free location for files in prdesc */
        if (proctab[currpid].prdesc[i] == -1) {
            break;
        }
    }
    proctab[currpid].pprdesc = i;
    proctab[currpid].prdesc[i] = pipeptr->pdev;

    restore(mask);
    return pipeptr->pdev;
}