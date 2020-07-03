/* pipread.c  -  pipread */

#include <xinu.h>

local devcall pipgetc (struct dentry *);

/*---------------------------------------------------------------
 *  pipread  -  Read from a previously opened pipe
 *---------------------------------------------------------------
 */
devcall	pipread (
	  struct dentry	*devptr,	/* Entry in device switch table	*/
	  char *buff			    /* Buffer to hold bytes */
	)
{
    return OK;
}

/*--------------------------------------------------
 *  pipgetc  -  Read the next byte from an open pipe
 *--------------------------------------------------
 */
local devcall	pipgetc (
	  struct dentry	*devptr	/* Entry in device switch table	*/
	)
{
    return OK;
}