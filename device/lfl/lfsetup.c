/* lfsetup.c - lfsetup */

#include <xinu.h>

local	status	getcputcsetup(struct lfcblk *);
local	status	wsetup(struct lfcblk *, int32);

/*------------------------------------------------------------------------
* lfsetup - Set a file’s data block for the current file position
*-------------------------------------------------------------------------
*/
status lfsetup (
    struct lfcblk *lfptr,   /* Pointer to slave file device */
    int32 count             /* Number of bytes to write */
)
{
    int32 retval;

    if (lfptr->lfmode == rmode) {
        retval = getcputcsetup(lfptr);
        if (retval == SYSERR) {
            return SYSERR;
        }
    }
    else {
        if (count == -1) {
            retval = getcputcsetup(lfptr);
            if (retval == SYSERR) {
                return SYSERR;
            }
        }
        else {
            retval = wsetup(lfptr, count);
            if (retval == SYSERR) {
                return SYSERR;
            }
        }
    }

    return OK;
}

/*------------------------------------------------------------------------
* getcputcsetup - Setup an opened file upon getc or putc
*-------------------------------------------------------------------------
*/
local	status	getcputcsetup(struct lfcblk *lfptr)
{
    int32 i;                                    /* Counter used to index the Datablocks array */
    int32 offset;                               /* Current file offset */
    did32 ramNo;                                /* ram disk that holds the file data */
    int32 singleBlockRange = 70656;             /* Max number of bytes held by the direct and single indirect blocks */
    int32 singleIndex[RM_BLKSIZ / 4];           /* Single indirect index block */
    int32 masterDoubleIndex[RM_BLKSIZ / 4];     /* primary Double indirect index block */
    int32 secondaryDoubleIndex[RM_BLKSIZ / 4];  /* single index within the double indirect index */
    int32 j;                                    /* secondary Counter to aid with double indirect indexing */

    ramNo = lfptr->lfram;
    offset = lfptr->lfoffset;

    if (ramNo == RAMDISK0) {
        wait(fsystem.lmf_mutex0);
    }
    else {
        wait(fsystem.lmf_mutex1);
    }

    if (lfptr->lfmode == wmode) {
        if (lfptr->lfdbdirty) {
            lfflush(lfptr);
        }
    }

    i = offset / RM_BLKSIZ;

    if (i < 10) { /* A direct Block Found */
        lfptr->lfdnum = lfptr->lfinode.datablcks[i];
        read(ramNo, lfptr->lfdblock, lfptr->lfdnum);
        lfptr->lfbyte = &lfptr->lfdblock[offset % RM_BLKSIZ];
    }
    else { /* Retrieve the correct indirect block */
        if (offset < singleBlockRange) { /* within the single indirect block range */
            read(ramNo, (char*)singleIndex, lfptr->lfinode.datablcks[10]);
            i = (offset / RM_BLKSIZ) - 10; // Holds the index in the single indirect block
            lfptr->lfdnum = singleIndex[i];
            read(ramNo, lfptr->lfdblock, lfptr->lfdnum);
            lfptr->lfbyte = &lfptr->lfdblock[offset % RM_BLKSIZ];
        }
        else { /* within the double indirect block range */
            read(ramNo, (char*)masterDoubleIndex, lfptr->lfinode.datablcks[11]);
            i = (offset / RM_BLKSIZ) - 138; // Bypass direct and single indirect indeces */
            i /= 128; // Holds the index of the master double indirect block
            read(ramNo, (char *)secondaryDoubleIndex, masterDoubleIndex[i]);
            j = ((offset / RM_BLKSIZ) - 138) % 128; // Direct block number in secondaryDoubleIndex
            lfptr->lfdnum = secondaryDoubleIndex[j];
            read(ramNo, lfptr->lfdblock, lfptr->lfdnum);
            lfptr->lfbyte = &lfptr->lfdblock[offset % RM_BLKSIZ];
        }
    }

    if (ramNo == RAMDISK0) {
        signal(fsystem.lmf_mutex0);
    }
    else {
        signal(fsystem.lmf_mutex1);
    }

    return OK;
}

/*------------------------------------------------------------------------
* wsetup - Setup an opened file upon writing and allocating data blocks
*-------------------------------------------------------------------------
*/
local	status	wsetup(struct lfcblk *lfptr, int32 count)
{
    int32 i;                                    /* Counter used to index the Datablocks array */
    int32 offset;                               /* Current file offset */
    did32 ramNo;                                /* ram disk that holds the file data */
    int32 singleBlockRange = 70656;             /* Max number of bytes held by the direct and single indirect blocks */
    int32 singleIndex[RM_BLKSIZ / 4];           /* Single indirect index block */
    int32 masterDoubleIndex[RM_BLKSIZ / 4];     /* primary Double indirect index block */
    int32 secondaryDoubleIndex[RM_BLKSIZ / 4];  /* single index within the double indirect index */
    int32 j;                                    /* Temporary helping variable */
    dbid32 dnum;                                /* Data block number */
    int32 nblocks;                              /* Number of blocks to be allocated */
    struct lfdbfree freetemp;                   /* Free block to allocate in */


    if (lfptr->lfinode.filestat.size >= MAXFILESIZE) {
        return SYSERR;
    }

    ramNo = lfptr->lfram;
    offset = lfptr->lfoffset;

    if (ramNo == RAMDISK0) {
        wait(fsystem.lmf_mutex0);
    }
    else {
        wait(fsystem.lmf_mutex1);
    }

    i = offset / RM_BLKSIZ;

    if (offset >= lfptr->lfinode.filestat.size) {
        dnum = LF_DNULL;
        if (i < 10) { /* Direct block */
            dnum = lfptr->lfinode.datablcks[i];
        }
        else {
            if (offset < singleBlockRange) { /* within the single indirect block range */
                if (lfptr->lfinode.datablcks[10] != LF_DNULL) {
                    read(ramNo, (char*)singleIndex, lfptr->lfinode.datablcks[10]);
                    j = (offset / RM_BLKSIZ) - 10; // Holds the index in the single indirect block
                    dnum = singleIndex[j];
                }
            }
            else { /* within the double indirect block range */
                if (lfptr->lfinode.datablcks[11] != LF_DNULL) {
                    read(ramNo, (char*)masterDoubleIndex, lfptr->lfinode.datablcks[11]);
                    j = (offset / RM_BLKSIZ) - 138; // Bypass direct and single indirect indeces */
                    j /= 128; // Holds the index of the master double indirect block
                    if (masterDoubleIndex[j] != LF_DNULL) {
                        read(ramNo, (char *)secondaryDoubleIndex, masterDoubleIndex[j]);
                        j = ((offset / RM_BLKSIZ) - 138) % 128; // Direct block number in secondaryDoubleIndex
                        dnum = secondaryDoubleIndex[j];
                    }
                }
            }
        }

        if (dnum != LF_DNULL) { /* A few bytes don't need to be allocated */
            count -= (RM_BLKSIZ - (offset % RM_BLKSIZ));

            if (count <= 0) { /* No blocks are needed to be allocated */
                if (ramNo == RAMDISK0) {
                    signal(fsystem.lmf_mutex0);
                }
                else {
                    signal(fsystem.lmf_mutex1);
                }
                return OK;
            }
        }
        else if (offset > lfptr->lfinode.filestat.size) {
            if (lfptr->lfinode.filestat.size == 0) {
                count += RM_BLKSIZ;
                if (offset < RM_BLKSIZ) {
                    count -= (RM_BLKSIZ - offset);
                }
                else {
                    count += (offset - RM_BLKSIZ);
                }
            }
            else {
                if (((lfptr->lfinode.filestat.size) % RM_BLKSIZ) == 0) {
                    j = lfptr->lfinode.filestat.size - 1; // Get last allocated byte in file
                }
                else {
                    j = (((lfptr->lfinode.filestat.size / RM_BLKSIZ) + 1) * RM_BLKSIZ) - 1; // Get last allocated byte in file
                }
                
                count += (offset - j - 1);
            }
        }
    }
    else {
        if (((lfptr->lfinode.filestat.size) % RM_BLKSIZ) == 0) {
            j = lfptr->lfinode.filestat.size - 1; // Get last allocated byte in file
        }
        else {
            j = (((lfptr->lfinode.filestat.size / RM_BLKSIZ) + 1) * RM_BLKSIZ) - 1; // Get last allocated byte in file
        }

        count -= (j - offset + 1); // A few bytes don't need to be allocated

        if (count <= 0) { /* No blocks are needed to be allocated */
            if (ramNo == RAMDISK0) {
                signal(fsystem.lmf_mutex0);
            }
            else {
                signal(fsystem.lmf_mutex1);
            }
            return OK;
        }
    }

    if ((lfptr->lfinode.filestat.size + count) >= MAXFILESIZE) {
        if (ramNo == RAMDISK0) {
            signal(fsystem.lmf_mutex0);
        }
        else {
            signal(fsystem.lmf_mutex1);
        }
        return SYSERR;
    }

    if ((count % RM_BLKSIZ) == 0) {
        nblocks = count / RM_BLKSIZ;
    }
    else {
        nblocks = (count / RM_BLKSIZ) + 1;
    }

    if (ramNo == RAMDISK0) {
        if (nblocks > fsystem.r0freenum) {
            signal(fsystem.lmf_mutex0);
            return SYSERR;
        }
    }
    else {
        if (nblocks > fsystem.r1freenum) {
            signal(fsystem.lmf_mutex1);
            return SYSERR;
        }
    }

    /* Get the first place to start allocation */

    if (((lfptr->lfinode.filestat.size) % RM_BLKSIZ) == 0) {
        dnum = (lfptr->lfinode.filestat.size) / RM_BLKSIZ;
    }
    else {
        dnum = ((lfptr->lfinode.filestat.size) / RM_BLKSIZ) + 1;
    }

    while ((nblocks--) > 0) {
        if (dnum < 10) {
            lfptr->lfinode.datablcks[dnum] = dballoc(&freetemp, ramNo);
        }
        else {
            if (dnum == 10) {
                lfptr->lfinode.datablcks[10] = dballoc((struct lfdbfree *)singleIndex, ramNo);
                for (i = 0; i < (sizeof(singleIndex) / sizeof(int32)); i++) {
                    singleIndex[i] = LF_DNULL;
                }
                singleIndex[0] = dballoc(&freetemp, ramNo);
            }
            else if (dnum < 138) {
                singleIndex[dnum - 10] = dballoc(&freetemp, ramNo);
            }
            else {
                if (dnum == 138) {
                    write(ramNo, (char *)singleIndex, lfptr->lfinode.datablcks[10]);
                    lfptr->lfinode.datablcks[11] = dballoc((struct lfdbfree *)masterDoubleIndex, ramNo);
                    for (i = 0; i < (sizeof(masterDoubleIndex) / sizeof(int32)); i++) {
                        masterDoubleIndex[i] = LF_DNULL;
                    }
                    masterDoubleIndex[0] = dballoc((struct lfdbfree *)secondaryDoubleIndex, ramNo);
                    for (i = 0; i < (sizeof(secondaryDoubleIndex) / sizeof(int32)); i++) {
                        secondaryDoubleIndex[i] = LF_DNULL;
                    }
                    secondaryDoubleIndex[0] = dballoc(&freetemp, ramNo);
                }
                else {
                    j = dnum - 138;
                    if ((j % 128) == 0) {
                        i = j / 128;
                        write(ramNo, (char *)secondaryDoubleIndex, masterDoubleIndex[i - 1]);
                        masterDoubleIndex[i] = dballoc((struct lfdbfree *)secondaryDoubleIndex, ramNo);
                        for (i = 0; i < (sizeof(secondaryDoubleIndex) / sizeof(int32)); i++) {
                            secondaryDoubleIndex[i] = LF_DNULL;
                        }
                        secondaryDoubleIndex[0] = dballoc(&freetemp, ramNo);
                    }
                    else {
                        secondaryDoubleIndex[j % 128] = dballoc(&freetemp, ramNo);
                    }
                }
            }
        }
        dnum++;
    }

    if (lfptr->lfinode.datablcks[11] != LF_DNULL) {
        i = j / 128;
        write(ramNo, (char *)secondaryDoubleIndex, masterDoubleIndex[i]);
        write(ramNo, (char *)masterDoubleIndex, lfptr->lfinode.datablcks[11]);
    }
    else if (lfptr->lfinode.datablcks[10] != LF_DNULL){
        write(ramNo, (char *)singleIndex, lfptr->lfinode.datablcks[10]);
    }

    if (offset > lfptr->lfinode.filestat.size) { /* Update the file size if the offset is greater than the file size */
        lfptr->lfinode.filestat.size = offset;
    }

    if (ramNo == RAMDISK0) {
        signal(fsystem.lmf_mutex0);
    }
    else {
        signal(fsystem.lmf_mutex1);
    }

    return OK;
}