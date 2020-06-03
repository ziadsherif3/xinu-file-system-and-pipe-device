/* lfflush.c - lfflush */

#include <xinu.h>
#include <filesystem.h>

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
    int32 ram;
    if (lfptr->lfstate == FREE) {
        return SYSERR;
    }

    iNodeNumber = lfptr->lfinode->filestat.ino;
    kprintf("inode block number %d\n", iNodeNumber);

    ram = lfptr->lfram;
    kprintf("Ram Number = %d\n", ram);
    
    if (!lfptr->lfdbdirty) {
        return OK;
    }
    else
    {
        write(ram,lfptr->lfinode, iNodeNumber );
        return OK; 
    }
}