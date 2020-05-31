/* lmfopen.c  -  lmfopen */

#include <xinu.h>
#include <filesystem.h>

/*------------------------------------------------------------------------
 *  lmfopen  -  Open a file through the local file system master device 
 *------------------------------------------------------------------------
 */
devcall	lmfopen (
	  struct dentry	*devptr,	/* Entry in device switch table	*/
      char *name,       /* Path name for the file */
      char *mode        /* Mode used in opening the file */
	)
{
    return OK;
}