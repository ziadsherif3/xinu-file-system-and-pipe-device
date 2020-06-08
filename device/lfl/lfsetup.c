/* lfsetup.c - lfsetup */

#include <xinu.h>

/*------------------------------------------------------------------------
* lfsetup - Set a file’s data block for the current file position
*                   (assumes file mutex held)
*------------------------------------------------------------------------
*/
status lfsetup (
    struct lfcblk *lfptr    /* Pointer to slave file device */
)
{
    dbid32 dnum;    /* Data block to fetch */
    struct inode * fileInode;    /* pointer to the file's inode */
    int32 * datablocks;     /* pointer to datablocks array */
    did32 ramNo;    /* ram disk that holds the file data */
    /* lfflush if necessary */
    if (lfptr->lfdbdirty) {
        lfflush(lfptr);
    }
    ramNo = lfptr->lfram;
    dnum = lfptr->lfdnum;
    fileInode = lfptr->lfinode;
    datablocks = fileInode->datablcks;
    /* check of the file is empty */
    if (dnum == LF_DNULL) { /* if true, allocate a new block and adjust the related fields */
        dnum = dballoc(lfptr->lfdblock, ramNo);
        lfptr->lfdnum = dnum;
        *lfptr->lfinode->datablcks = dnum;
        lfptr->lfdbdirty = TRUE;
    }   else {
        
    }


    
    
    return OK;
}