/* pipread.c  -  pipread */

#include <xinu.h>

/*---------------------------------------------------------------
 *  pipread  -  Read from a previously opened pipe
 *---------------------------------------------------------------
 */
devcall	pipread (
	  struct dentry	*devptr	/* Entry in device switch table	*/
	)
{
    return OK;
}