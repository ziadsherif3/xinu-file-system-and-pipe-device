/* pipgetc.c  -  pipgetc */

#include <xinu.h>

/*--------------------------------------------------
 *  pipgetc  -  Read the next byte from an open pipe
 *--------------------------------------------------
 */
devcall	pipgetc (
	  struct dentry	*devptr	/* Entry in device switch table	*/
	)
{
    return OK;
}