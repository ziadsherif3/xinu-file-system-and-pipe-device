/* dbfree.c - dbfree */

#include <xinu.h>
#include <filesystem.h>

/*------------------------------------------------------------------------
* dbfree - Free a disk block given its block number (assumes
*                   corresponding RAMDISK mutex is held)
*------------------------------------------------------------------------
*/
status dbfree(
    did32 diskdev,  /* ID of disk device to use */
    dbid32 dnum     /* Number of data block to free */
)
{
    return OK;
}