/* lfread.c  -  lfread */

#include <xinu.h>
#include <filesystem.h>

/*------------------------------------------------------------------------
 *  lfread  -  Read from a previously opened local file
 *------------------------------------------------------------------------
 */
devcall	lfread (
	  struct dentry	*devptr,	/* Entry in device switch table	*/
      char *buff,   /* Buffer to hold bytes */
      int32 count   /* Max bytes to read */
	)
{
    return OK;
}