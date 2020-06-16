/* xsh_mv.c - xsh_mv */

#include <xinu.h>

/*------------------------------------------------------------------------
 * xhs_mv - Moves (Renames) an oldname to newname
 *------------------------------------------------------------------------
 */
shellcmd xsh_mv(int nargs, char *args[]) {

	if (nargs != 3) {
        fprintf(stderr, "%s: invalid arguments\n", args[0]);
        fprintf(stderr, "Usage: mv \"filePath\" \"filePath\"\n");
        return SYSERR;
    }

    did32 src;    /* First File Desc */
    did32 dest;    /* Second File Desc */
    uint64 numRead; /* Characters read from source */
    uint64 count;    /*Characters written to destination */
    char buffer[MAXFILESIZE]; /*buffer to transfer */

    char *schp, *dchp, *to;
    char newname[NAME_LEN];
    int32 s, d;

    schp = args[1];
    dchp = args[2];
    s = d = 0;

    if ((*(schp + 4) != '/') || (*(dchp + 4) != '/')) {
        fprintf(stderr, "Error in file path names\n");
        return SYSERR;
    }

    while (*schp++ != NULLCH){
        s++;
    }
    while ((*--schp) != '/') {
        s--;
    }
    s--;

    while (*dchp++ != NULLCH){
        d++;
    }
    while ((*--dchp) != '/'){
        d--;
    }
    d--;
    dchp++;

    to = newname;
    while ((*to++ = *dchp++) != NULLCH);

    if(s == d) {
        int32 retval = strncmp(args[1], args[2], s);
        if (!retval) { /* Rename src file */
            if (((src = open(FSYSTEM, args[1], "r")) == SYSERR) || (src == NOTFOUND)) { /* Source file is not found or in use */
                fprintf(stderr, "Cannot open source file\n");
                return SYSERR;
            }
            struct dentry *devptr = (struct dentry *) &devtab[src];
            struct lfcblk *lfptr = &lftab[devptr->dvminor];

            to = lfptr->lfname;
            schp = newname;
            while ((*to++ = *schp++) != NULLCH);

            close(src);
            return 0;
        }
    }

    if (((src = open(FSYSTEM, args[1], "r")) == SYSERR) || (src == NOTFOUND)) { /* Source file is not found or in use */
        fprintf(stderr, "Cannot open source file\n");
        return SYSERR;
    }
    numRead = read(src, buffer, MAXFILESIZE);
    close(src);
	if ((control(FSYSTEM, FDELETE, (int32)args[1], 0)) == SYSERR) {
		fprintf(stderr, "Error occured\n");
        return SYSERR;
	}

    if (numRead < 0) {
        fprintf(stderr, "Error occured\n");
        return SYSERR;
    }

    if ((control(FSYSTEM, FCREATE, (int32) args[2], 0)) == SYSERR) { /* Error while creating the destination file */
        fprintf(stderr, "Error while creating the destination file\n");
        return SYSERR;
    }
    else { /* Destination file was just Created by control */
        if ((dest = open(FSYSTEM, args[2], "w")) == SYSERR) {
            fprintf(stderr, "Error occured while opening the destination file, aborting\n");
            return SYSERR;
        }
    }
 
    count = write(dest, buffer, numRead);
    close(dest);

    if ( numRead != count) {
        fprintf(stderr, "Error occured\n");
        return SYSERR;
    }

    return 0;
}