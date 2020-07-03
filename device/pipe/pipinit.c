/* pipinit.c  -  pipinit */

#include <xinu.h>

struct pipeblk pipetab[NPIPE];

/*---------------------------------------------------------------
 *  pipinit  -  Initialize control blocks for pipe pseudo-devices
 *---------------------------------------------------------------
 */
devcall	pipinit (
	  struct dentry	*devptr	/* Entry in device switch table	*/
	)
{
	struct pipeblk *pipeptr = &pipetab[devptr->dvminor];

	pipeptr->pstate = FREE;
	pipeptr->pdev = devptr->dvnum;
	pipeptr->esembuff = semcreate(0);
	pipeptr->fsembuff = semcreate(0);
	pipeptr->pipsem = semcreate(1);

    return OK;
}