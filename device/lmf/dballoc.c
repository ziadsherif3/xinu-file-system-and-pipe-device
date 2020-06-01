/* dballoc.c - dballoc */

#include <xinu.h>
#include <filesystem.h>

/*------------------------------------------------------------------------
* dballoc - Allocate a new data block from free list on disk
*           (assumes corresponding RAMDISK mutex is held)
*------------------------------------------------------------------------
*/
dbid32 dballoc (
    struct lfdbfree *dbuff      /* Addr. of buffer to hold data block */
)
{
    return OK;
}