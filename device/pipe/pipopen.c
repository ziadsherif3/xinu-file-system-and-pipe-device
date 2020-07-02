/* pipopen.c  -  pipopen */

#include <xinu.h>

/*---------------------------------------------------------------
 *  pipopen  -  Open a free pipe pseudo-device
 *---------------------------------------------------------------
 */
devcall	pipopen (void)
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

    restore(mask);
    return pipeptr->pdev;
}