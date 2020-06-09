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
    int16 i;    /* Counter */
    bool8 indirectFlag = TRUE; /* Flag to indicate a indirect datablock */
    int32 offset;   /* Current file offset */
    /* lfflush if necessary */
    if (lfptr->lfdbdirty) {
        lfflush(lfptr);
    }

    ramNo = lfptr->lfram;
    dnum = lfptr->lfdnum;
    offset = lfptr->lfoffset;
    fileInode = lfptr->lfinode;
    datablocks = fileInode->datablcks;

    /* check of the file is empty */
    if (dnum == LF_DNULL) { /* Case 0: allocate a new block and adjust the related fields */
        dnum = dballoc(lfptr->lfdblock, ramNo);
        lfptr->lfdnum = dnum;
        *lfptr->lfinode->datablcks = dnum;
        lfptr->lfbyte = &lfptr->lfdblock;
        lfptr->lfdbdirty = TRUE;
    }

    else { /* File is Not empty */

        if ( (offset % RM_BLKSIZ) != 0 ) { /* Case 1: setup is invoked by a backward seek */

        }
        for (i = 0; i<10; i++) {
            if ( *datablocks == dnum) {
                indirectFlag = FALSE;
                break;
            }
            datablocks++;
        }

        if (!indirectFlag) { /* Current Block is a direct Data Block */
            if ( i < 9) { /* Case 2: There are still free data blocks */
                dnum = dballoc(lfptr->lfdblock, ramNo);
                lfptr->lfdnum = dnum;
                lfptr->lfinode->datablcks[i+1] = dnum;
                lfptr->lfbyte = &lfptr->lfdblock;
                lfptr->lfdbdirty = TRUE;
            }

            else { /* Case 3: initialize and allocate single indirect */


            }
        }

        else { /* Direct blocks are full */
            /* Case 4: Check the if the single indirect blocks are full */
            datablocks = datablocks +2; // points to the double indirect field
            if (*datablocks != LF_DNULL) { /* Case 4.1: Traverse the single indirect block */


            }

            else { /* Case 4.2: Single indirect Block is Full, Traverse the double indirect Block */


            }

        }
    }
    
    return OK;
}