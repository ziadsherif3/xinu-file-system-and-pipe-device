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
	struct lflcblk *lfptr; /* Ptr to open file table entry */

	/* Obtain Exclusive access to the file */
	lfptr = &lftab[devptr->dvminor];
	/* return an error if the file is not open */
    return OK;
}