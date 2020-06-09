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
    int32 * datablocks;     /* pointer to datablocks array */
    did32 ramNo;    /* ram disk that holds the file data */
    int16 i;    /* Counter used to index the Datablocks array */
    bool8 indirectFlag = TRUE; /* Flag to indicate a indirect datablock */
    int32 offset;   /* Current file offset */
    int32 singleBlockRange = 65576;
    char singleIndex[RM_BLKSIZ]; /* Single indirect index block */
    char masterDoubleIndex[RM_BLKSIZ]; /* primary Double indirect index block */
    char secondaryDoubleIndex[RM_BLKSIZ]; /* single index within the double indirect index */
    /* lfflush if necessary */
    if (lfptr->lfdbdirty) {
        lfflush(lfptr);
    }

    ramNo = lfptr->lfram;
    dnum = lfptr->lfdnum;
    offset = lfptr->lfoffset;
    datablocks = lfptr->lfinode->datablcks;

    /* check of the file is empty */
    if (dnum == LF_DNULL) { /* Case 0: allocate a new block and adjust the related fields */
        dnum = dballoc(lfptr->lfdblock, ramNo);
        lfptr->lfdnum = dnum;
        *lfptr->lfinode->datablcks = dnum;
        lfptr->lfbyte = &lfptr->lfdblock;
        lfptr->lfdbdirty = TRUE;
    }

    else { /* File is Not empty */

        if ( offset  < lfptr->lfinode->filestat.size) { /* Case 1: setup is invoked by a backward seek or getc*/
            i = offset /RM_BLKSIZ;
            if ( i < 9) { /* A dirext Block Found */
                lfptr->lfdnum = lfptr->lfinode->datablcks[i];
                kprintf("lfsetup: dnum = %d",lfptr->lfdnum);
                read(ramNo, lfptr->lfdblock, lfptr->lfdnum);
                lfptr->lfbyte = &lfptr->lfdblock[offset % RM_BLKSIZ];
                lfptr->lfdbdirty = TRUE;
            }
            
            else { /* Retrieve the correct indirect block */
                if (offset < singleBlockRange) { /* with in the single indirect block range */
                    read(ramNo, singleIndex, lfptr->lfinode->datablcks[10]);
                    i = (offset/RM_BLKSIZ) - 10; // Holds the index in the single indirect block
                    lfptr->lfdnum = ;
                    read(ramNo, lfptr->lfdblock,lfptr->lfdnum);
                    lfptr->lfbyte = &lfptr->lfdblock[offset % RM_BLKSIZ];
                    lfptr->lfdbdirty = TRUE;
                }
                else { /* within the double indirect block range */
                    read(ramNo, masterDoubleIndex, lfptr->lfinode->datablcks[11]);
                    i = (offset/RM_BLKSIZ) - 138; //bypass direct and single indirect indeces */
                    i/=128; //Holds the index of the master double indirect block
                    
                }
            }
        }

        else { /* Setup is invoked by putc */

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

                else { /* Case 3: initialize and allocate a single indirect block */
                    
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
    }
    
    return OK;
}