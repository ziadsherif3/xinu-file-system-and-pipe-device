/* lmfctrl.c  -  lmfctrl */

#include <xinu.h>

local	status	fcreate(char *);
local	status	dircreate(char *);
local	status	fdelete(char *);
local	status	dirdelete(char *);

/*------------------------------------------------------------------------
 *  lmfctrl  -  Control function for lfs device
 *------------------------------------------------------------------------
 */
devcall	lmfctrl (
	  struct dentry	*devptr,	/* Entry in device switch table	*/
	  int32 func,			/* Function to execute	*/
	  int32	arg1,			/* First argument to function func */
      int32 arg2            /* Second argument to function func */
	)
{
	char *name = (char *)arg1;
	int32 retval;

	if (func == FCREATE) {
		retval = fcreate(name);
	}
	else if (func == FMKDIR) {
		retval = dircreate(name);
	}
	else if (func == FDELETE) {
		retval = fdelete(name);
	}
	else if (func == FRMDIR) {
		retval = dirdelete(name);
	}
	else {
		return SYSERR;
	}

	if (retval == SYSERR) {
		return SYSERR;
	}

	return OK;
}

/*------------------------------------------------------------------------
 *  fcreate  -  Create a file
 *------------------------------------------------------------------------
 */
local	status	fcreate(char *name)
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
	bool8 error;				/* Error indication */

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
	error = FALSE;
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
				error = TRUE;
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

	if (found || error) {
		if (retval1 == 0) {
            signal(fsystem.lmf_mutex0);
        }
        else {
            signal(fsystem.lmf_mutex1);
        }
        return SYSERR;
	}

	/* Create the file */

	if (pnode1->filestat.ino == 0) { /* Parent directory is the root */
		pnode1 = root;
	}

	if (pnode1->filestat.size == ntuples) { /* Directory is full */
		if (retval1 == 0) {
            signal(fsystem.lmf_mutex0);
        }
        else {
            signal(fsystem.lmf_mutex1);
        }
        return SYSERR;
	}

	retval = dballoc((struct lfdbfree *)pnode2, disk);

	if (retval == SYSERR) {
		if (retval1 == 0) {
            signal(fsystem.lmf_mutex0);
        }
        else {
            signal(fsystem.lmf_mutex1);
        }
        return SYSERR;
	}

	pnode2->type = FILE;
	pnode2->filestat.ino = retval;
	pnode2->filestat.dev = -1;
	pnode2->filestat.size = 0;
	pnode2->filestat.acctime = pnode2->filestat.ctime = pnode2->filestat.mtime = clktime;
	strncpy(pnode2->name, nam, strlen(nam) + 1);
	strncpy(pnode1->contents[pnode1->filestat.size].name, nam, strlen(nam) + 1);
	pnode1->contents[pnode1->filestat.size].blcknum = retval;

	retval = write(disk, (char *)pnode2, pnode2->filestat.ino);

	if (retval == SYSERR) {
		if (retval1 == 0) {
            signal(fsystem.lmf_mutex0);
        }
        else {
            signal(fsystem.lmf_mutex1);
        }
        return SYSERR;
	}

	retval = write(disk, (char *)pnode1, pnode1->filestat.ino);

	if (retval == SYSERR) {
		if (retval1 == 0) {
            signal(fsystem.lmf_mutex0);
        }
        else {
            signal(fsystem.lmf_mutex1);
        }
        return SYSERR;
	}

	if (retval1 == 0) {
        signal(fsystem.lmf_mutex0);
    }
    else {
        signal(fsystem.lmf_mutex1);
    }
    return OK;
}

/*------------------------------------------------------------------------
 *  dircreate  -  Create a directory
 *------------------------------------------------------------------------
 */
local	status	dircreate(char *name)
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
}

/*------------------------------------------------------------------------
 *  fdelete  -  delete a file
 *------------------------------------------------------------------------
 */
local	status	fdelete(char *name)
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
}

/*------------------------------------------------------------------------
 *  dirdelete  -  delete a directory
 *------------------------------------------------------------------------
 */
local	status	dirdelete(char *name)
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
}