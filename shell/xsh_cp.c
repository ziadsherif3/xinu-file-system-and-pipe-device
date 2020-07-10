/* xsh_cp.c - xsh_cp */

#include <xinu.h>


/*------------------------------------------------------------------------
 * xsh_cp - Copies source file into destination
 *------------------------------------------------------------------------
 */
shellcmd xsh_cp(int nargs, char *args[]) {

    if (nargs != 3) {
        fprintf(stderr, "%s: invalid arguments\n", args[0]);
        fprintf(stderr, "Usage: cp \"filePath\" \"filePath\"\n");
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

    freemem(srcname, strlen(args[1]) + 1);

    numRead = read(src, buffer, MAXFILESIZE);
    close(src);

    if (numRead < 0) {
        fprintf(stderr, "Error occured\n");
        return SYSERR;
    }

    /* Set up the destination file path */
    destname = getmem(NAME_LEN + strlen(args[2]) + 1);
    strncpy(destname, args[2], strlen(args[2]) + 1);

    if (args[2][strlen(args[2]) - 1] == '/') { /* Copy with the same name in src */
        char srcname[NAME_LEN];
        char *chp, *to;
        chp = args[1];
        while (*chp++ != NULLCH);
        while ((*--chp) != '/');
        chp++;
        to = srcname;
        while ((*to++ = *chp++) != NULLCH);
        strncpy(destname + strlen(destname), srcname, strlen(srcname) + 1);
    }

    if (((dest = open(FSYSTEM, destname, "w")) == NOTFOUND)) { /* Destination file is not found */
        if ((control(FSYSTEM, FCREATE, (int32)destname, 0)) == SYSERR) {
            fprintf(stderr, "Error while creation, cannot open destination file\n");
            freemem(destname, NAME_LEN + strlen(args[2]) + 1);
            return SYSERR;
        }
        else { /* Destination file was just Created by control */
            if ((dest = open(FSYSTEM, destname, "w")) == SYSERR) {
                fprintf(stderr, "Error occured, aborting\n");
                freemem(destname, NAME_LEN + strlen(args[2]) + 1);
                return SYSERR;
            }
        }
    }
    else if (dest == SYSERR) {
        fprintf(stderr, "Cannot open destination file, aborting\n");
        freemem(destname, NAME_LEN + strlen(args[2]) + 1);
        return SYSERR;
    }

    freemem(destname, NAME_LEN + strlen(args[2]) + 1);
 
    count = write(dest, buffer, numRead);
    close(dest);

    if (numRead != count) {
        fprintf(stderr, "Error occured\n");
        return SYSERR;
    }

    return 0;
}