/* lmfctrl.c  -  lmfctrl */

#include <xinu.h>

local	status	fcreate(char *);
local	status	dircreate(char *);
local	status	fdelete(char *);
local	status	dirdelete(char *);
local   status  truncate(struct inode *, did32);
local   status  dtruncate(struct inode *, did32);
local   status  dircheck(struct inode *, did32);
local   status  dirempty(struct inode *, did32);

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
    //kprintf("ARG name %s\n", name);
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
    else if (func == DIROPEN) {
        retval = opendir(name);
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
    pnode2->sdev = -1;
	pnode2->filestat.ino = retval;
	pnode2->filestat.dev = disk;
	pnode2->filestat.size = 0;
	pnode2->filestat.acctime = pnode2->filestat.ctime = pnode2->filestat.mtime = clktime;
    for (i = 0; i < (sizeof(pnode2->datablcks) / sizeof(int32)); i++) {
        pnode2->datablcks[i] = LF_DNULL;
    }
	strncpy(pnode2->name, nam, strlen(nam) + 1);
	strncpy(pnode1->contents[pnode1->filestat.size].name, nam, strlen(nam) + 1);
	pnode1->contents[pnode1->filestat.size].blcknum = retval;
    pnode1->filestat.size++;

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
    bool8 error;                /* Error indication */

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

	/* Search disk to see if directory exists */

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
        else { /* nam holds the new directory name */
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

    if (found || error) {
		if (retval1 == 0) {
            signal(fsystem.lmf_mutex0);
        }
        else {
            signal(fsystem.lmf_mutex1);
        }
        return SYSERR;
	}

	/* Create the new directory */

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

    pnode2->type = DIR;
	pnode2->filestat.ino = retval;
    pnode2->filestat.dev = disk;
	pnode2->filestat.size = 0;
	strncpy(pnode2->name, nam, strlen(nam) + 1);
	strncpy(pnode1->contents[pnode1->filestat.size].name, nam, strlen(nam) + 1);
	pnode1->contents[pnode1->filestat.size].blcknum = retval;
    pnode1->filestat.size++;

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

    if (pnode2->sdev != -1) { /* File is opened */
        if (retval1 == 0) {
            signal(fsystem.lmf_mutex0);
        }
        else {
            signal(fsystem.lmf_mutex1);
        }
        return SYSERR;
    }

    if (pnode1->filestat.ino == 0) { /* Parent directory is the root */
		pnode1 = root;
	}
    
    truncate(pnode2, disk);

    pnode1->filestat.size--;
    memset((char *)&pnode1->contents[i], NULLCH, sizeof(struct tuple));
    memcpy((char *)&pnode1->contents[i], (char *)&pnode1->contents[pnode1->filestat.size], sizeof(struct tuple));
    memset((char *)&pnode1->contents[pnode1->filestat.size], NULLCH, sizeof(struct tuple));

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

    if (pnode1->filestat.ino == 0) { /* Parent directory is the root */
		pnode1 = root;
	}

    retval = dtruncate(pnode2, disk);

    if (retval == SYSERR) {
		if (retval1 == 0) {
            signal(fsystem.lmf_mutex0);
        }
        else {
            signal(fsystem.lmf_mutex1);
        }
        return SYSERR;
	}

    pnode1->filestat.size--;
    memset((char *)&pnode1->contents[i], NULLCH, sizeof(struct tuple));
    memcpy((char *)&pnode1->contents[i], (char *)&pnode1->contents[pnode1->filestat.size], sizeof(struct tuple));
    memset((char *)&pnode1->contents[pnode1->filestat.size], NULLCH, sizeof(struct tuple));

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
 *  truncate  -  Truncate a file by freeing all of the fields of its i-node
 *                  (assumes corresponding RAMDISK mutex is held)
 *------------------------------------------------------------------------
 */
local   status  truncate(struct inode *pnode, did32 disk)
{
    int32 i;                                    /* General loop index */
    int32 j;                                    /* General loop index */
    int32 sblock[RM_BLKSIZ / sizeof(int32)];    /* Single indirect block buffer */
    int32 dblock[RM_BLKSIZ / sizeof(int32)];    /* Double indirect block buffer */

    for (i = 0; i < ((sizeof(pnode->datablcks) / sizeof(int32)) - 2); i++) {
        if (pnode->datablcks[i] == LF_DNULL) {
            break;
        }
        dbfree(disk, pnode->datablcks[i]);
    }

    if (pnode->datablcks[(sizeof(pnode->datablcks) / sizeof(int32)) - 2] != LF_DNULL) { /* Check for single indirect block */
        read(disk, (char *)sblock, pnode->datablcks[(sizeof(pnode->datablcks) / sizeof(int32)) - 2]);
        for (i = 0; i < (sizeof(sblock) / sizeof(int32)); i++) {
            if (sblock[i] == LF_DNULL) {
                break;
            }
            dbfree(disk, sblock[i]);
        }

        dbfree(disk, pnode->datablcks[(sizeof(pnode->datablcks) / sizeof(int32)) - 2]);
    }

    if (pnode->datablcks[(sizeof(pnode->datablcks) / sizeof(int32)) - 1] != LF_DNULL) { /* Check for double indirect block */
        read(disk, (char *)dblock, pnode->datablcks[(sizeof(pnode->datablcks) / sizeof(int32)) - 1]);
        for (i = 0; i < (sizeof(dblock) / sizeof(int32)); i++) {
            if (dblock[i] == LF_DNULL) {
                break;
            }
            read(disk, (char *)sblock, dblock[i]);
            for (j = 0; j < (sizeof(sblock) / sizeof(int32)); j++) {
                if (sblock[j] == LF_DNULL) {
                    break;
                }
                dbfree(disk, sblock[j]);
            }
            
            dbfree(disk, dblock[i]);
        }

        dbfree(disk, pnode->datablcks[(sizeof(pnode->datablcks) / sizeof(int32)) - 1]);
    }

    dbfree(disk, pnode->filestat.ino);

    return OK;
}

/*------------------------------------------------------------------------
 *  dtruncate  -  Truncate a directory by freeing all of the contents of its i-node
 *                  (assumes corresponding RAMDISK mutex is held)
 *------------------------------------------------------------------------
 */
local   status  dtruncate(struct inode *pnode, did32 disk)
{
    int32 retval;               /* Value returned from function */

    retval = dircheck(pnode, disk);

    if (retval == SYSERR) {
        return SYSERR;
    }
    
    dirempty(pnode, disk);

    dbfree(disk, pnode->filestat.ino);

    return OK;
}

/*------------------------------------------------------------------------
 *  dircheck  -  Checks the content of a directory
 *           (assumes corresponding RAMDISK mutex is held)
 *------------------------------------------------------------------------
 */
local   status  dircheck(struct inode *pnode, did32 disk)
{
    int32 i;                    /* General loop index */
    struct inode node1;         /* Temporary i-node */
    struct inode *pnode1;       /* Ptr to Temporary i-node */
    int32 retval;               /* Value returned from function */

    pnode1 = &node1;

    for (i = 0; i < pnode->filestat.size; i++) {
        retval = read(disk, (char *)pnode1, pnode->contents[i].blcknum);
        if (retval == SYSERR) {
            return SYSERR;
        }
        if (pnode1->type == DIR) {
            retval = dircheck(pnode1, disk);
            if (retval == SYSERR) {
                return SYSERR;
            }
        }
        else if (pnode1->type == FILE) {
            if (pnode1->sdev != -1) {
                return SYSERR;
            }
        }
        else {
            return SYSERR;
        }
    }

    return OK;
}

/*------------------------------------------------------------------------
 *  dirempty  -  Empty a directory after Checking its content
 *           (assumes corresponding RAMDISK mutex is held)
 *------------------------------------------------------------------------
 */
local   status  dirempty(struct inode *pnode, did32 disk)
{
    int32 i;                    /* General loop index */
    struct inode node1;         /* Temporary i-node */
    struct inode *pnode1;       /* Ptr to Temporary i-node */

    pnode1 = &node1;

    for (i = 0; i < pnode->filestat.size; i++) {
        read(disk, (char *)pnode1, pnode->contents[i].blcknum);
        if (pnode1->type == DIR) {
            dirempty(pnode1, disk);
            dbfree(disk, pnode1->filestat.ino);
        }
        else {
            truncate(pnode1, disk);
        }
    }

    return OK;
}