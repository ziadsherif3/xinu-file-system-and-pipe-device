/* xsh_cp.c - xsh_cp */

#include <xinu.h>


/*------------------------------------------------------------------------
 * xhs_cp - Copies source file into destination
 *------------------------------------------------------------------------
 */
shellcmd xsh_cp(int nargs, char *args[]) {

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
    char *srcName;
    char *destName;
    char * holder;
    uint64 i ;

    srcName = getmem(NAME_LEN + strlen(args[1]) + 1);
    destName = getmem(NAME_LEN + strlen(args[2]) + strlen(args[1]));
    holder = getmem(NAME_LEN + strlen(args[1]) + 1);
    strncpy(srcName,args[1], strlen(args[1])+1);
    strncpy(destName, args[2], strlen(args[2])+1);


     if (destName[strlen(args[2]) - 1] == '/') { /* Copy with the same name in src */
        for ( i = strlen(args[1]) - 1; i >=0; i--) {
            if (srcName[i] == '/') {
                break;
            }
        }
        strncpy(holder, srcName + i+1, strlen(args[1]) - i );
        strncat(destName, holder, strlen(holder) + 1);
    }

    if ( ( src = open(FSYSTEM, srcName, "r") ) == SYSERR) { /* Source file is not found or in use */
        fprintf(stderr, "%s:Cannot open source file %s\n",args[0], args[1]);
        return SYSERR;
    }

    numRead = read(src, buffer, MAXFILESIZE);
    close(src);

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