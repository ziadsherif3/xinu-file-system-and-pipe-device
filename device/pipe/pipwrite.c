/* pipwrite.c  -  pipwrite */

#include <xinu.h>

/*---------------------------------------------------------------
 *  pipwrite  -  Write data to a previously opened pipe
 *---------------------------------------------------------------
 */
devcall	pipwrite (
	  struct dentry	*devptr,	/* Entry in device switch table	*/
      char *buff,   /* Buffer holding data to write */
      int32 count   /* Number of bytes to write */
	)
{
    return OK;
}