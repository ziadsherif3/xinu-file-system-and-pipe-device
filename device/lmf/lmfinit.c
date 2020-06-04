/* lmfinit.c  -  lmfinit */

#include <xinu.h>

struct lmf fsystem;

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