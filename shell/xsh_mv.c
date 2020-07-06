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
    uint64 count;    /* Characters written to destination */
    char buffer[MAXFILESIZE]; /* buffer to transfer */

    char *destname, *srcname; /* Destination and source file paths */ 

    /* Set up the source file path */
    srcname = getmem(strlen(args[1]) + 1);
	strncpy(srcname, args[1], strlen(args[1]) + 1);

    if (((src = open(FSYSTEM, srcname, "r")) == SYSERR) || (src == NOTFOUND)) { /* Source file is not found or in use */
        fprintf(stderr, "Cannot open source file\n");
        freemem(srcname, strlen(args[1]) + 1);
        return SYSERR;
    }

    numRead = read(src, buffer, MAXFILESIZE);
    close(src);

    if (numRead < 0) {
        fprintf(stderr, "Error occured\n");
        freemem(srcname, strlen(args[1]) + 1);
        return SYSERR;
    }

    /* Set up the destination file path */
    destname = getmem(strlen(args[2]) + 1);
    strncpy(destname, args[2], strlen(args[2]) + 1);

    if ((control(FSYSTEM, FCREATE, (int32)destname, 0)) == SYSERR) { /* Error while creating the destination file */
        fprintf(stderr, "Error while creating the destination file\n");
        freemem(srcname, strlen(args[1]) + 1);
        freemem(destname, strlen(args[2]) + 1);
        return SYSERR;
    }
    else { /* Destination file was just Created by control */
        if ((dest = open(FSYSTEM, destname, "w")) == SYSERR) {
            fprintf(stderr, "Error occured while opening the destination file, aborting\n");
            freemem(srcname, strlen(args[1]) + 1);
            freemem(destname, strlen(args[2]) + 1);
            return SYSERR;
        }
    }

    if ((control(FSYSTEM, FDELETE, (int32)srcname, 0)) == SYSERR) {
		fprintf(stderr, "Error occured\n");
        freemem(srcname, strlen(args[1]) + 1);
        freemem(destname, strlen(args[2]) + 1);
        return SYSERR;
	}

    freemem(srcname, strlen(args[1]) + 1);
    freemem(destname, strlen(args[2]) + 1);
 
    count = write(dest, buffer, numRead);
    close(dest);

    if (numRead != count) {
        fprintf(stderr, "Error occured\n");
        return SYSERR;
    }

    return 0;
}