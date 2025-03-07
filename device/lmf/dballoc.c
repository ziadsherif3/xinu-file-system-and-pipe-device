/* dballoc.c - dballoc */

#include <xinu.h>

/*------------------------------------------------------------------------
* dballoc - Allocate a new data block from free list on disk
*           (assumes corresponding RAMDISK mutex is held)
*------------------------------------------------------------------------
*/
dbid32 dballoc (
    struct lfdbfree *dbuff,     /* Addr. of buffer to hold data block */
    did32 disk                  /* Which ram disk RAMDISK0/RAMDISK1 to allocate from */
)
{
    dbid32 dnum;        /* ID of next d-block on the free list */
    int32 retval;       /* Return value */

    /* Get the block number of first data block on the free list */

    if (disk == RAMDISK0) {
        dnum = fsystem.r0freepos;
    }
    else {
        dnum = fsystem.r1freepos;
    }

    if (dnum == LF_DNULL) { /* Ran out of free data blocks */
        return SYSERR;
    }

    retval = read(disk, (char *)dbuff, dnum);
    if (retval == SYSERR) {
        return SYSERR;
    }

    /* Unlink d-block from free list */

    if (disk == RAMDISK0) {
        fsystem.r0freepos = dbuff->lf_nextdb;
        fsystem.r0freenum--;
    }
    else {
        fsystem.r1freepos = dbuff->lf_nextdb;
        fsystem.r1freenum--;
    }

    /* Fill data block to erase old data */
    
    memset((char *)dbuff, NULLCH, RM_BLKSIZ);
    return dnum;
}