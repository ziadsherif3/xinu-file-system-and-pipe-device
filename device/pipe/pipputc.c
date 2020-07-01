/* pipputc.c  -  pipputc */

#include <xinu.h>

/*---------------------------------------------------------------
 *  pipputc  -  Write a single byte to an open pipe
 *---------------------------------------------------------------
 */
devcall	pipputc (
	  struct dentry	*devptr,	/* Entry in device switch table	*/
      char ch   /* Character (byte) to write */
	)
{
    return OK;
}