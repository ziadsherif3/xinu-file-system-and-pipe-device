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
	struct lfcblk *lfptr; /* Ptr to open file table entry */
	int32 singleByte; /* Next data byte */
	/* Obtain Exclusive access to the file */

	lfptr = &lftab[devptr->dvminor];
	wait(lfptr->lfmutex);

	/* If file is not open, return an error */
	
	if (lfptr->lfstate != USED) { 
		signal(lfptr->lfmutex);
	}
	
	/* Return EOF for any attempt to read beyond the EOF */
	
	if (lfptr->lfoffset >= lfptr->lfinode->filestat.size) {
		signal(lfptr->lfmutex);
		return EOF;
	}


	/* if the byte pointer is beyond the current the block call setup */

	if ( lfptr->lfbyte >= lfptr->lfdblock[RM_BLKSIZ]) { /* possible error */
		lfsetup(lfptr);
	}
	/* Extract the next byte and update the related variables */

	singleByte = 0xff & *lfptr->lfbyte++;
	lfptr->lfoffset++;
	signal(lfptr->lfmutex);
	return singleByte;


	
    return OK;
}