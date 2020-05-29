/* lfseek.c  -  lfseek */

#include <xinu.h>
#include <filesystem.h>

/*------------------------------------------------------------------------
 *  lfseek  -  Seek to a specified position in a file
 *------------------------------------------------------------------------
 */
devcall	lfseek (
	  struct dentry	*devptr,	/* Entry in device switch table	*/
      int32 offset      /* Byte position in the file */
	)
{
    return OK;
}