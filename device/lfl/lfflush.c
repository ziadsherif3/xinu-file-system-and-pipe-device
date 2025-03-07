/* lfflush.c - lfflush */

#include <xinu.h>

/*------------------------------------------------------------------------
* lfflush - Flush root, data block for an open file
*          (assumes corresponding disk mutex is held)
*------------------------------------------------------------------------
*/
status lfflush (
    struct lfcblk *lfptr    /* Ptr to file pseudo-device */
)
{
    int32 ram;          /* RamDisk being Uaed */
    int32 dataBlockNumber; /* Current data block number */

    ram = lfptr->lfram;
    //kprintf("lfflush: Ram Number = %d\n", ram);

    dataBlockNumber = lfptr->lfdnum;
    //kprintf("lfflush: Data Block number = %d\n");

    /* Write dirty data block to disk */

    if (lfptr->lfdbdirty) {
        write(ram, lfptr->lfdblock, dataBlockNumber);
    }

    return OK;
}