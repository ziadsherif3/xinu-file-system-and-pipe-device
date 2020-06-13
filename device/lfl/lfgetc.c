/* lfgetc.c  -  lfgetc */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  lfgetc  -  Read the next byte from an open local file
 *------------------------------------------------------------------------
 */
devcall	lfgetc (
	  struct dentry	*devptr	/* Entry in device switch table	*/
	)
{
	struct lfcblk *lfptr; /* Ptr to an open file table entry */
	int32 singleByte; /* Next data byte */

	lfptr = &lftab[devptr->dvminor];
	
	/* Return EOF for any attempt to read beyond the EOF */
	
	if (lfptr->lfoffset >= lfptr->lfinode->filestat.size) {
		return EOF;
	}

	/* if the byte pointer is beyond the current the block call setup */

	if (lfptr->lfbyte >= &lfptr->lfdblock[RM_BLKSIZ]) {
		lfsetup(lfptr, -1);
	}

	/* Extract the next byte and update the related variables */

	singleByte = 0xff & *lfptr->lfbyte++;
	lfptr->lfoffset++;
	
	return singleByte;
}