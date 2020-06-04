/* dballoc.c - dballoc */

#include <xinu.h>

#define DFILL   '+'     /* character used to fill a disk block */

/*------------------------------------------------------------------------
* dballoc - Allocate a new data block from free list on disk
*           (assumes corresponding RAMDISK mutex is held)
*------------------------------------------------------------------------
*/
dbid32 dballoc (
    struct lfdbfree *dbuff,     /* Addr. of buffer to hold data block */
    int32 fram                  /* Which ram disk RAMDISK0/RAMDISK1 to allocate from */
)
{
    dbid32 dnum;        /* ID of next d-block on the free list */
    int32 retval;       /* Return value */
    did32 dskdev;       /* Device ID for disk to allocate from */

    /* Get the block number of first data block on the free list */

    if (fram == RAMDISK0) {
        dnum = fsystem.r0freepos;
        dskdev = fsystem.ram0dskdev;
    }
    else {
        dnum = fsystem.r1freepos;
        dskdev = fsystem.ram1dskdev;
    }

    if (dnum == LF_DNULL) { /* Ran out of free data blocks */
        panic("out of data blocks");
    }

    retval = read(dskdev, (char *)dbuff, dnum);
    if (retval == SYSERR) {
        panic("dballoc cannot read disk block\n\r");
    }

    /* Unlink d-block from free list */

    if (fram == RAMDISK0) {
        fsystem.r0freepos = dbuff->lf_nextdb;
    }
    else {
        fsystem.r1freepos = dbuff->lf_nextdb;
    }

    /* Fill data block to erase old data */
    
    memset((char *)dbuff, DFILL, RM_BLKSIZ);
    return dnum;
}