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
    dbid32 dnum;    /* Data block number */
    dbid32 inum;    /* index block number */
    dbid32 inum2;    /* secondary block number */
    int32 * datablocks;     /* pointer to datablocks array */
    did32 ramNo;    /* ram disk that holds the file data */
    int32 i;    /* Counter used to index the Datablocks array */
    int32 j; /* secondary Counter to aid with double indirect indexing */
    bool8 indirectFlag = TRUE; /* Flag to indicate a indirect datablock */
    int32 offset;   /* Current file offset */
    int32 singleBlockRange = 65576; /* Max number of bytes held by the direct and single indirect blocks */
    int32 singleIndex[RM_BLKSIZ/4]; /* Single indirect index block */
    int32 masterDoubleIndex[RM_BLKSIZ/4]; /* primary Double indirect index block */
    int32 secondaryDoubleIndex[RM_BLKSIZ/4]; /* single index within the double indirect index */
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
        dnum = dballoc( (struct lfdbfree *) lfptr->lfdblock, ramNo);
        lfptr->lfdnum = dnum;
        *lfptr->lfinode->datablcks = dnum;
        lfptr->lfbyte = lfptr->lfdblock;
        lfptr->lfdbdirty = TRUE;
    }

    else { /* File is Not empty */

        if ( offset  < lfptr->lfinode->filestat.size) { /* Case 1: setup is invoked by seek or getc*/
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
                    read(ramNo, (char*) singleIndex, lfptr->lfinode->datablcks[10]);
                    i = (offset/RM_BLKSIZ) - 10; // Holds the index in the single indirect block
                    lfptr->lfdnum = singleIndex[i];
                    read(ramNo, lfptr->lfdblock,lfptr->lfdnum);
                    lfptr->lfbyte = &lfptr->lfdblock[offset % RM_BLKSIZ];
                    lfptr->lfdbdirty = TRUE;
                }
                else { /* within the double indirect block range */
                    read(ramNo, (char*) masterDoubleIndex, lfptr->lfinode->datablcks[11]);
                    i = (offset/RM_BLKSIZ) - 138; //bypass direct and single indirect indeces */
                    i/=128; //Holds the index of the master double indirect block
                    read(ramNo, (char *) secondaryDoubleIndex, masterDoubleIndex[i]);
                    j = offset - singleBlockRange;// start with the start of the double indirect block
                    j = j - (i * 65536);
                    j/=RM_BLKSIZ;
                    lfptr->lfdnum = secondaryDoubleIndex[j];
                    read(ramNo, lfptr->lfdblock,lfptr->lfdnum);
                    lfptr->lfbyte = &lfptr->lfdblock[offset % RM_BLKSIZ];
                    lfptr->lfdbdirty = TRUE;   
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
                    dnum = dballoc( (struct lfdbfree *) lfptr->lfdblock, ramNo);
                    lfptr->lfdnum = dnum;
                    lfptr->lfinode->datablcks[i+1] = dnum;
                    lfptr->lfbyte = lfptr->lfdblock;
                    lfptr->lfdbdirty = TRUE;
                }

                else { /* Case 3: initialize and allocate a single indirect block */
                    inum = dballoc((struct ldbfree *) singleIndex, ramNo);
                    lfptr->lfinode->datablcks[10] = dnum;
                    dnum = dballoc((struct ldbfree *) lfptr->lfdblock, ramNo);
                    singleIndex[0] = dnum;
                    lfptr->lfdnum = dnum;
                    lfptr->lfbyte = lfptr->lfdblock;
                    lfptr->lfdbdirty = TRUE;
                    write(ramNo, (char*) singleIndex, inum );
                }
            }

            else { /* Direct blocks are full */
                /* Case 4: Check the if the single indirect blocks are full */
                

                if (lfptr->lfinode->datablcks[11] != LF_DNULL) { /* Case 4.1: allocate a new data block in the single indirect block */
                    read (ramNo, (char*) singleIndex, lfptr->lfinode->datablcks[10]);
                    for (i = 0; i < 128; i++) {
                        if ( singleIndex[i] == LF_DNULL) {
                            break;
                        }
                    }
                    if ( i < 128) { /* single indirect block is not full */
                        dnum = dballoc((struct ldbfree *) lfptr->lfdblock, ramNo);
                        singleIndex[i] = dnum;
                        lfptr->lfdnum = dnum;
                        lfptr->lfbyte = lfptr->lfdblock;
                        lfptr->lfdbdirty = TRUE;
                        write(ramNo, (char *) singleIndex , lfptr->lfinode->datablcks[10]);
                    }
                    else { /* Case 4.2: initialze and allocate a double indirect block */
                        inum = dballoc((struct ldbfree *) masterDoubleIndex, ramNo);
                        lfptr->lfinode->datablcks[11] = inum;
                        inum2 = dballoc((struct ldbfree *) secondaryDoubleIndex, ramNo);
                        masterDoubleIndex[0] = inum2;// to be wriiten
                        dnum = dballoc((struct ldbfree *) lfptr->lfdblock, ramNo);
                        secondaryDoubleIndex[0] = dnum; //to be written
                        lfptr->lfdnum = dnum;
                        lfptr->lfbyte = lfptr->lfdblock;
                        lfptr->lfdbdirty = TRUE;
                        write(ramNo, (char*) masterDoubleIndex, inum);
                        write(ramNo, (char*) secondaryDoubleIndex, inum2);
                    }

                if (lfptr->lfinode->datablcks[11] != LF_DNULL) { /* Case 4.1: Traverse the single indirect block */
                    

                }

                else { /* Case 4.3: Single indirect Block is Full, Traverse the double indirect Block */
                    read(ramNo, (char*) masterDoubleIndex, lfptr->lfinode->datablcks[11]);
                    for (i = 0 ; i < 15; i++) {
                        
                       if ( masterDoubleIndex[i] != LF_DNULL) {

                           read (ramNo, (char*) secondaryDoubleIndex, masterDoubleIndex[i]);

                           for (j = 0; j <128; j++) {
                               if (secondaryDoubleIndex[j] == LF_DNULL) {
                                   break;
                               }
                           }
                       }

                       else {
                           break;
                       }
                    }
                    if ( i == 15) { /* max file size*/
                    kprintf("Max file size\n");
                    return SYSERR;
                    }

                    if ( j < 128 ) { /* Single indirect is not empty */
                        dnum = dballoc((struct ldbfree *) lfptr->lfdblock, ramNo); 
                        secondaryDoubleIndex[j] = dnum;
                        lfptr->lfdnum = dnum;
                        lfptr->lfbyte = lfptr->lfdblock;
                        lfptr->lfdbdirty = TRUE;
                        write(ramNo, (char*) secondaryDoubleIndex, masterDoubleIndex[i]);
                    }
                    else { /* allocate a new single indirect block */
                        inum = dballoc((struct ldbfree *) secondaryDoubleIndex, ramNo);
                        dnum = dballoc((struct ldbfree *) lfptr->lfdblock, ramNo);
                        masterDoubleIndex[i] = inum;
                        secondaryDoubleIndex[0] = dnum;
                        lfptr->lfdnum = dnum;
                        lfptr->lfbyte = lfptr->lfdblock;
                        lfptr->lfdbdirty = TRUE;
                        write(ramNo, (char*) masterDoubleIndex, lfptr->lfinode->datablcks[11]);
                        write(ramNo, (char*) secondaryDoubleIndex, inum); 
                    }
                }
            }
        }
    }
    return OK;
}