/* raminit.c  -  raminit */

#include <xinu.h>
#include <ramdisk.h>

struct	ramdisk	Ram[Nram];

/*------------------------------------------------------------------------
 *  raminit  -  Initialize the remote disk system device
 *------------------------------------------------------------------------
 */
devcall	raminit (
	  struct dentry	*devptr		/* Entry in device switch table	*/
	)
{
	memcpy(Ram[devptr->dvminor].disk, "hopeless", 8);
	memcpy( &Ram[devptr->dvminor].disk[8], Ram[devptr->dvminor].disk, RM_BLKSIZ * RM_BLKS - 8);
	return OK;
}
