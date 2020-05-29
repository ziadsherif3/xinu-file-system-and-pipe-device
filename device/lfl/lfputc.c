/* lfputc.c  -  lfputc */

#include <xinu.h>
#include <filesystem.h>

/*------------------------------------------------------------------------
 *  lfputc  -  Write a single byte to an open local file
 *------------------------------------------------------------------------
 */
devcall	lfputc (
	  struct dentry	*devptr,	/* Entry in device switch table	*/
      char ch   /* Character (byte) to write */
	)
{
    return OK;
}