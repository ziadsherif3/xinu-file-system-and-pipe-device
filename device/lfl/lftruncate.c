/* lftruncate.c - lftruncate */

#include <xinu.h>
#include <filesystem.h>

/*------------------------------------------------------------------------
* lftruncate - Truncate a file by freeing its data blocks
*               (assumes corresponding RAMDISK mutex is held)
*------------------------------------------------------------------------
*/
status lftruncate (
    struct lfcblk *lfptr    /* Ptr to file’s cntl blk entry */
)
{
    return OK;
}