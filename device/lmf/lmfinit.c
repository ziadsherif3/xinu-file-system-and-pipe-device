/* lmfinit.c  -  lmfinit */

#include <xinu.h>
#include <filesystem.h>

/*------------------------------------------------------------------------
 *  lmfinit  -  Initialize the local file system master device
 *------------------------------------------------------------------------
 */
devcall	lmfinit (
	  struct dentry	*devptr	/* Entry in device switch table	*/
	)
{
    return OK;
}