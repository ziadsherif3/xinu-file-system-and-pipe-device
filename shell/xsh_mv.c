/* xsh_mv.c - xsh_mv */

#include <xinu.h>

/*------------------------------------------------------------------------
 * xhs_mv - Moves (Renames) an oldname to newname
 *------------------------------------------------------------------------
 */
shellcmd xsh_mv(int nargs, char *args[]) {

	if (nargs != 3) {
        fprintf(stderr, "%s: invalid arguments\n", args[0]);
        fprintf(stderr, "Usage: cat \"filePath\" \"filePath\"\n");
        return SYSERR;
    }

    did32 src;    /* First File Desc */
    did32 dest;    /* Second File Desc */
    uint64 numRead; /* Characters read from source */
    uint64 count;    /*Characters written to destination */
    char buffer[MAXFILESIZE]; /*buffer to transfer */
    char srcName[12];
    char destName[12];

    strcpy(srcName,args[1]);
    strcpy(destName, args[2]);

    if ((src = open(FSYSTEM, srcName, "r")) == SYSERR) { /* Source file is not found or in use */
        fprintf(stderr, "Cannot open source file\n");
        return SYSERR;
    }

    numRead = read(src, buffer, MAXFILESIZE);
    close(src);

	if ((control(FSYSTEM, FDELETE, (int32) srcName, 0)) == SYSERR) {
		fprintf(stderr, "%s Error occured in Deletion\n");
        return SYSERR;
	}

    if (numRead < 0) {
        fprintf(stderr, "Error occured\n");
        return SYSERR;
    }

    if ((dest = open(FSYSTEM, destName, "w")) == SYSERR) { /* Destination File is not found or in use */
        if ((control(FSYSTEM, FCREATE, (int32) destName, 0)) == SYSERR) { /* Destination File is in use */
            fprintf(stderr, "Cannot open destination file\n");
            return SYSERR;
        }
        else { /* Destination File was just Created by cp */
            if ((dest = open(FSYSTEM, destName, "w")) == SYSERR) {
                fprintf(stderr, "Error occured, aborting\n");
                return SYSERR;
            }
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