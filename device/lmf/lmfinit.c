/* lmfinit.c  -  lmfinit */

#include <xinu.h>
#include <ramdisk.h>

struct lmf fsystem;
local	status	lmfcreate(void);

/*------------------------------------------------------------------------
 *  lmfinit  -  Initialize the local file system master device
 *------------------------------------------------------------------------
 */
devcall	lmfinit (
	  struct dentry	*devptr	/* Entry in device switch table	*/
	)
{
	int32 retval;

	/* Assign ID of both disk devices */

	fsystem.ram0dskdev = RAMDISK0;
	fsystem.ram1dskdev = RAMDISK1;

	/* Create mutual exclusion semaphores */

	fsystem.lmf_mutex0 = semcreate(1);
	fsystem.lmf_mutex1 = semcreate(1);
	fsystem.lftabmutex = semcreate(1);

	/* Prepare 2 roots */

	memset((char *)&fsystem.rt0, NULLCH, sizeof(struct inode));
	memset((char *)&fsystem.rt1, NULLCH, sizeof(struct inode));
	fsystem.r0freepos = fsystem.r1freepos = 1;		/* Indicate the block number of the first disk block in free list of both disks */

	fsystem.r0freenum = fsystem.r1freenum = RM_BLKS - 1;

	retval = lmfcreate();
	if(retval == SYSERR){
		return SYSERR;
	}

	read(RAMDISK0, (char *)&fsystem.rt0, 0);
	read(RAMDISK1, (char *)&fsystem.rt1, 0);
	fsystem.rt0present = TRUE;
	fsystem.rt1present = TRUE;

	return OK;
}

/*------------------------------------------------------------------------
 *  lmfcreate  -  Set up the file system for the specified disk
 *------------------------------------------------------------------------
 */
local	status	lmfcreate(void)
{
	struct inode rt;				/* Buffer to hold the root inode */
	uint32 dblks = RM_BLKS - 1;		/* Total free disk blocks */
	struct lfdbfree dblock; 		/* Data block on the free list */
	int32 retval; 					/* Return value from func call */
	int32 i; 						/* Loop index */
	char name[] = "root\0";			/* Name for the root */

	/* Create an initial root */

	memset((char *)&rt, NULLCH, sizeof(struct inode));
	rt.type = DIR;
	strncpy(rt.name, name, strlen(name) + 1);
	rt.filestat.ino = 0;
	rt.filestat.size = 0;
	rt.filestat.dev = RAMDISK0;
	retval = write(RAMDISK0, (char *)&rt, 0);
	rt.filestat.dev = RAMDISK1;
	retval = write(RAMDISK1, (char *)&rt, 0);
	if (retval == SYSERR) {
		return SYSERR;
	}

	/* Create list of free data blocks on both disks */

	memset((char*)&dblock, NULLCH, RM_BLKSIZ);
	for (i = 0; i < (dblks - 1); i++) {
		dblock.lf_nextdb = i + 2;
		write(RAMDISK0, (char *)&dblock, (i + 1));
		write(RAMDISK1, (char *)&dblock, (i + 1));
	}

	dblock.lf_nextdb = LF_DNULL;
	write(RAMDISK0, (char *)&dblock, (i + 1));
	write(RAMDISK1, (char *)&dblock, (i + 1));

	return OK;
}