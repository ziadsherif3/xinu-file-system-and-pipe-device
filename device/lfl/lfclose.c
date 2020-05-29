/* lfclose.c  -  lfclose */

#include <xinu.h>
#include <filesystem.h>

/*------------------------------------------------------------------------
 *  lfclose  -  Close a file by flushing output and freeing device entry
 *------------------------------------------------------------------------
 */
devcall	lfclose (
	  struct dentry	*devptr	/* Entry in device switch table	*/
	)
{
    return OK;
}