/* lfclose.c  -  lfclose */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  lfclose  -  Close a file by flushing output and freeing device entry
 *------------------------------------------------------------------------
 */
devcall	lfclose (
	  struct dentry	*devptr	/* Entry in device switch table	*/
	)
{
	struct lfcblk *lfptr; /* pointer to the open filetable entry */
	int32 i, j;
	
	lfptr = &lftab[devptr->dvminor];

    for (j = 0; j < NDESC; j++) {
		if (proctab[currpid].prdesc[j] == lfptr->lfdev) {
 			break;
 		}
	}

	if (j == NDESC) {
		return SYSERR;
	}

	wait(fsystem.lftabmutex);

	if ((lfptr->lfstate) == FREE) {
		signal(fsystem.lftabmutex);
		return SYSERR;
	}

	if (lfptr->lfram == RAMDISK0) {
		wait(fsystem.lmf_mutex0);
	}
	else {
		wait(fsystem.lmf_mutex1);
	}

	/* Flush data block to disk if it has been changed */
	
	lfflush(lfptr);

	/* Write i-node of file to disk */

	lfptr->lfinode->sdev = -1;
    write(lfptr->lfram, (char*)lfptr->lfinode, lfptr->lfinode->filestat.ino);

	/* free up the device and return */

	lfptr->lfstate = FREE;
	lfptr->lfinode = (struct inode *) NULL;
	lfptr->lfdbdirty = FALSE;
	lfptr->lfoffset = 0;
	lfptr->lfdnum = LF_DNULL;
	lfptr->lfbyte = &lfptr->lfdblock[RM_BLKSIZ];

	for (i = 0; i < NAME_LEN; i++) {
		lfptr->lfname[i] = NULLCH;
	}

	memset((char *)&lfptr->lfdblock, NULLCH, RM_BLKSIZ);

	proctab[currpid].prdesc[j] = -1;
	proctab[currpid].nfprdesc--;

	if (lfptr->lfram == RAMDISK0) {
		signal(fsystem.lmf_mutex0);
	}
	else {
		signal(fsystem.lmf_mutex1);
	}

	signal(fsystem.lftabmutex);
    return OK;
}