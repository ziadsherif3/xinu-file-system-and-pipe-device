/* opendir.c  -  opendir */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  opendir  -  Open a directory through the local file system master device 
 *------------------------------------------------------------------------
 */
devcall	opendir (
      char *name       /* Path name for the directory */
	)
{
    struct inode *root;         /* Ptr to in-memory root */
    char *chp, *to;             /* Ptrs used during comparison and copy */
    char nam[NAME_LEN];         /* String to hold name */
    int32 i;                    /* General loop index */
    struct inode node1;         /* Temporary i-node */
    struct inode *pnode1;       /* Ptr to Temporary i-node */
    struct inode node2;         /* Temporary i-node */
    struct inode *pnode2;       /* Ptr to Temporary i-node */
    bool8 found;                /* Was the name found? */
    int32 retval;               /* Value returned from function */
    int32 retval1;              /* Value returned from function */
    int32 retval2;              /* Value returned from function */
    did32 disk;                 /* Device ID of disk */

    /* Check path name validity */

    retval1 = strncmp(name, "rd0:/", 5);
    retval2 = strncmp(name, "rd1:/", 5);

    if ((retval1 != 0) && (retval2 != 0)) {
        return SYSERR;
    }

    if ((retval1 == 0) && (strlen(name) == 5)) {
        memcpy((char *)&cwd, (char *)&fsystem.rt0, sizeof(struct inode));
        return OK;
    }
    else if ((retval2 == 0) && (strlen(name) == 5)) {
        memcpy((char *)&cwd, (char *)&fsystem.rt1, sizeof(struct inode));
        return OK;
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
        else { /* nam holds the desired directory name */
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
        return NOTFOUND;
    }

    if (retval1 == 0) {
        signal(fsystem.lmf_mutex0);
    }
    else {
        signal(fsystem.lmf_mutex1);
    }

    memcpy((char *)&cwd, (char *)pnode2, sizeof(struct inode));
    return OK;
}