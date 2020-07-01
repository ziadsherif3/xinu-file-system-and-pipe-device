/* pipclose.c  -  pipclose */

#include <xinu.h>

/*--------------------------------------------------------------------------------
 *  pipclose  -  Close a pipe by placeing an EOF character or freeing device entry
 *--------------------------------------------------------------------------------
 */
devcall	pipclose (
	  struct dentry	*devptr	/* Entry in device switch table	*/
	)
{
    return OK;
}