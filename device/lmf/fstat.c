/* fstat.c - fstat */

#include <xinu.h>

/*------------------------------------------------------------------------
* fstat - returns the statistics of file in the structure stat
*------------------------------------------------------------------------
*/
status fstat(
    char *filename,             /* Name of file */
    struct stat *filestat       /* Pointer to structure */
)
{
    struct inode *root;         /* Ptr to in-memory root */
    struct inode node1;         /* Temporary i-node */
    struct inode *pnode1;       /* Ptr to Temporary i-node */
    struct inode node2;         /* Temporary i-node */
    struct inode *pnode2;       /* Ptr to Temporary i-node */
    char *chp, *to;             /* Ptrs used during comparison and copy */
    char nam[NAME_LEN];         /* String to hold name */
    int32 i;                    /* General loop index */
    int32 retval;               /* Value returned from function */
    int32 retval1;              /* Value returned from function */
    int32 retval2;              /* Value returned from function */
    did32 disk;                 /* Device ID of disk */
    bool8 found;                /* Was the name found? */

    /* Check path name validity */

    retval1 = strncmp(filename, "rd0:/", 5);
    retval2 = strncmp(filename, "rd1:/", 5);

    if ((retval1 != 0) && (retval2 != 0)) {
        return SYSERR;
    }

    chp = filename + 5;
    
    if ((filename[strlen(filename) - 1] == '/') || (*chp == '/')){
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

    if (retval1 == 0) {
        disk = fsystem.ram0dskdev;
        root = &fsystem.rt0;
        wait(fsystem.lmf_mutex0);
        if (!(fsystem.rt0present)) {
            retval = read(disk, (char *)root, 0);
            if (retval == SYSERR) {
                signal(fsystem.lmf_mutex0);
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
    chp = filename + 5;
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
        return SYSERR;
    }

    /* Set the stat struct fields */

    filestat->ino = pnode2->filestat.ino;
    filestat->dev = pnode2->filestat.dev;
    filestat->size = pnode2->filestat.size;
    filestat->acctime = pnode2->filestat.acctime;
    filestat->ctime = pnode2->filestat.ctime;
    filestat->mtime = pnode2->filestat.mtime;

    if (retval1 == 0) {
        signal(fsystem.lmf_mutex0);
    }
    else {
        signal(fsystem.lmf_mutex1);
    }
    return OK;
}