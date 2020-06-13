/* dbfree.c - dbfree */

#include <xinu.h>

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
    struct lfdbfree buf;                    /* Buffer to hold data block */

    if (diskdev == RAMDISK0) {
        buf.lf_nextdb = fsystem.r0freepos;
        fsystem.r0freepos = dnum;
        fsystem.r0freenum++;
    }
    else {
        buf.lf_nextdb = fsystem.r1freepos;
        fsystem.r1freepos = dnum;
        fsystem.r1freenum++;
    }
    write(diskdev, (char *)&buf, dnum);

    return OK;
}