/* lfflush.c - lfflush */

#include <xinu.h>

/*------------------------------------------------------------------------
* lfflush - Flush root, data block for an open file 
*               (assumes file mutex is held)
*------------------------------------------------------------------------
*/
status lfflush (
    struct lfcblk *lfptr    /* Ptr to file pseudo-device */
)
{
    int32 iNodeNumber; /* inode Block number */
    int32 ram;          /* RamDisk being Uaed */
    int32 dataBlockNumber; /* Current data block number */
    
    if (lfptr->lfstate == FREE) {
        return SYSERR;
    }

    iNodeNumber = lfptr->lfinode->filestat.ino;
    kprintf("lfflush: Inode block number%d\n", iNodeNumber);

    ram = lfptr->lfram;
    kprintf("lfflush: Ram Number = %d\n", ram);

    dataBlockNumber = lfptr->lfdnum;
    kprintf("lfflush: Data Block number = %d\n");

    if (lfptr->lfdbdirty) {
        write(ram,(char*)lfptr->lfinode, iNodeNumber );
        if (dataBlockNumber != LF_DNULL) {
            write(ram, lfptr->lfdblock, dataBlockNumber);
        }
        return OK; 
    }
    else {
        return OK;
    }
    
}