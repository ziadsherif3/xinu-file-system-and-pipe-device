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
    int32 inodeNumber; /* inode Block number */
    if (lfptr->lfstate == FREE) {
        return SYSERR;
    }
    inodeNumber = lfptr->lfinode.filestat.ino;
    kprintf("%d\n", inodeNumber);
    

    return OK;
}