/* pipwrite.c  -  pipwrite */

#include <xinu.h>

local devcall pipputc (struct dentry *);

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

/*---------------------------------------------------------------
 *  pipputc  -  Write a single byte to an open pipe
 *---------------------------------------------------------------
 */
local devcall	pipputc (
	  struct dentry	*devptr,	/* Entry in device switch table	*/
      char ch   /* Character (byte) to write */
	)
{
    return OK;
}