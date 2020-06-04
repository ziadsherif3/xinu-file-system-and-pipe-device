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
    int32 disk = devtab[diskdev].dvminor;   /* Identify which RAMDISK */
    struct lfdbfree buf;                    /* Buffer to hold data block */

    if (disk == RAMDISK0) {
        buf.lf_nextdb = fsystem.r0freepos;
        fsystem.r0freepos = dnum;
    }
    else {
        buf.lf_nextdb = fsystem.r1freepos;
        fsystem.r1freepos = dnum;
    }
    write(diskdev, (char *)&buf, dnum);

    return OK;
}