/* lmfopen.c  -  lmfopen */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  lmfopen  -  Open a file through the local file system master device 
 *------------------------------------------------------------------------
 */
devcall	lmfopen (
	  struct dentry	*devptr,	/* Entry in device switch table	*/
      char *name,       /* Path name for the file */
      char *mode        /* Mode used in opening the file */
	)
{
    struct inode *root;         /* Ptr to in-memory root */
    char *chp, *to;             /* Ptrs used during comparison and copy */
    char nam[NAME_LEN];         /* String to hold name */
    int32 i;                    /* General loop index */
    did32 lfnext;               /* Minor number of an unused file pseudo-device */
    struct inode node1;         /* Temporary i-node */
    struct inode *pnode1;       /* Ptr to Temporary i-node */
    struct inode node2;         /* Temporary i-node */
    struct inode *pnode2;       /* Ptr to Temporary i-node */
    struct lfcblk *lfptr;       /* Ptr to open file table entry */
    bool8 found;                /* Was the name found? */
    int32 retval;               /* Value returned from function */
    int32 retval1;              /* Value returned from function */
    int32 retval2;              /* Value returned from function */
    int32 mod;                  /* Open Mode (rmode/wmode) */
    did32 disk;                 /* Device ID of disk */

    /* Check path name validity */

    retval1 = strncmp(name, "rd0:/", 5);
    retval2 = strncmp(name, "rd1:/", 5);

    if ((retval1 != 0) && (retval2 != 0)) {
        return SYSERR;
    }

    chp = name + 5;
    
    if ((name[strlen(name) - 1] == '/') || (*chp == '/')){
        return SYSERR;
    }

    while (*chp != NULLCH) {
        if (*chp == '/'){ /* 2 or more forward slashes were found following each other */
            return SYSERR;
        }
        for (i = 0; i < NAME_LEN; i++) {
            if ((*chp == '/') || (*chp == NULLCH)) {
                break;
            }
            chp++;
        }
        if (i >= NAME_LEN) { /* Name is too long */
            return SYSERR;
        }

        chp++;
    }

    while ((*--chp) != '/');
    chp++;

    to = nam;
    while ((*to++ = *chp++) != NULLCH);
    
    /* Check mode validity */

    if (!strncmp(mode, "r", 1)) {
        mod = rmode;
    }
    else if (!strncmp(mode, "w", 1)) {
        mod = wmode;
    }
    else {
        return SYSERR;
    }

    wait(fsystem.lftabmutex);

    /* If named file is already open, return SYSERR */

    lfnext = SYSERR;
    for (i = 0; i < Nlfl; i++){
        lfptr = &lftab[i];
        if ((lfptr->lfstate) == FREE) {
            if (lfnext == SYSERR){
                lfnext = i;
            }
            continue;
        }
        if (!(strncmp(nam, lfptr->lfname, strlen(nam))) && (lfptr->lfname[strlen(nam)] == NULLCH)) { /* File is already open */
            signal(fsystem.lftabmutex);
            return SYSERR;
        }
    }

    if (lfnext == SYSERR) { /* No slave file devices are available */
        signal(fsystem.lftabmutex);
        return SYSERR;
    }

    if (retval1 == 0) {
        disk = fsystem.ram0dskdev;
        root = &fsystem.rt0;
        wait(fsystem.lmf_mutex0);
        if (!(fsystem.rt0present)) {
            retval = read(disk, (char *)root, 0);
            if (retval == SYSERR) {
                signal(fsystem.lmf_mutex0);
                signal(fsystem.lftabmutex);
                return SYSERR;
            }
            fsystem.rt0present = TRUE;
        }
    }
    else {
        disk = fsystem.ram1dskdev;
        root = &fsystem.rt1;
        wait(fsystem.lmf_mutex1);
        if (!(fsystem.rt1present)) {
            retval = read(disk, (char *)root, 0);
            if (retval == SYSERR) {
                signal(fsystem.lmf_mutex1);
                signal(fsystem.lftabmutex);
                return SYSERR;
            }
            fsystem.rt1present = TRUE;
        }
    }

    /* Search disk to see if file exists */

    found = FALSE;
    pnode1 = &node1;
    pnode2 = &node2;
    memcpy((char *)pnode1, (char *)root, sizeof(struct inode));
    chp = name + 5;
    while (*chp != NULLCH) {
        to = nam;
        while ((*chp != NULLCH) && (*chp != '/')) {
            *to++ = *chp++;
        }
        *to = NULLCH;
        if (*chp == '/') { /* nam holds a directory name */
            for (i = 0; i < pnode1->filestat.size; i++) {
                if (!(strncmp(nam, pnode1->contents[i].name, strlen(nam))) && (pnode1->contents[i].name[strlen(nam)] == NULLCH)) {
                    retval = read(disk, (char *)pnode2, pnode1->contents[i].blcknum);
                    if (retval == SYSERR) {
                        if (retval1 == 0) {
                            signal(fsystem.lmf_mutex0);
                        }
                        else {
                            signal(fsystem.lmf_mutex1);
                        }
                        signal(fsystem.lftabmutex);
                        return SYSERR;
                    }
                    if (pnode2->type == DIR) {
                        break;
                    }
                }
            }
            if (i == pnode1->filestat.size) { /* No directory was found by the name in nam */
                break;
            }            
            memcpy((char *)pnode1, (char *)pnode2, sizeof(struct inode));
        }
        else { /* nam holds a file name */
            for (i = 0; i < pnode1->filestat.size; i++) {
                if (!(strncmp(nam, pnode1->contents[i].name, strlen(nam))) && (pnode1->contents[i].name[strlen(nam)] == NULLCH)) {
                    retval = read(disk, (char *)pnode2, pnode1->contents[i].blcknum);
                    if (retval == SYSERR) {
                        if (retval1 == 0) {
                            signal(fsystem.lmf_mutex0);
                        }
                        else {
                            signal(fsystem.lmf_mutex1);
                        }
                        signal(fsystem.lftabmutex);
                        return SYSERR;
                    }
                    if (pnode2->type == FILE) {
                        break;
                    }
                }
            }
            if (i == pnode1->filestat.size) { /* No file was found by the name in nam */
                break;
            }
            found = TRUE;
            chp--;
        }

        chp++;
    }

    if (!found) {
        if (retval1 == 0) {
            signal(fsystem.lmf_mutex0);
        }
        else {
            signal(fsystem.lmf_mutex1);
        }
        signal(fsystem.lftabmutex);
        return SYSERR;
    }

    /* Initialize the local file pseudo-device */

    lfptr = &lftab[lfnext];
    lfptr->lfstate = USED;
    lfptr->lfram = disk;
    lfptr->lfinode = pnode2;
    lfptr->lfmode = mod;
    lfptr->lfoffset = 0;
    to = lfptr->lfname;
    chp = pnode2->name;
    while ((*to++ = *chp++) != NULLCH);
    lfptr->lfdnum = LF_DNULL;
    lfptr->lfbyte = &lfptr->lfdblock[RM_BLKSIZ];
    lfptr->lfdbdirty = FALSE;

    /* Update the i-node on disk */

    pnode2->filestat.dev = lfptr->lfdev;
    pnode2->filestat.acctime = clktime;
    retval = write(disk, (char *)pnode2, pnode2->filestat.ino);

    if (retval == SYSERR) {
		if (retval1 == 0) {
            signal(fsystem.lmf_mutex0);
        }
        else {
            signal(fsystem.lmf_mutex1);
        }
        signal(fsystem.lftabmutex);
        return SYSERR;
	}

    if (retval1 == 0) {
        signal(fsystem.lmf_mutex0);
    }
    else {
        signal(fsystem.lmf_mutex1);
    }
    signal(fsystem.lftabmutex);

    return lfptr->lfdev;

}