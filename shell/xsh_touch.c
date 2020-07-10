/* xsh_touch.c - xsh_touch */

#include <xinu.h>


/*------------------------------------------------------------------------
 * xsh_touch - Updates access and modification time of a file
 *------------------------------------------------------------------------
 */
shellcmd xsh_touch(int nargs, char *args[]) {

    if (nargs != 2) {
		fprintf(stderr, "%s: invalid arguments\n", args[0]);
		fprintf(stderr, "Usage: touch \"filePath\"\n");
		return SYSERR;
	}

    char *srcname = getmem(strlen(args[1]) + 1);
	strncpy(srcname, args[1], strlen(args[1]) + 1);

    int32 retval = open(FSYSTEM, srcname, "w");

    if (retval == NOTFOUND) {
        retval = control(FSYSTEM, FCREATE, (int32)srcname, 0);
        if (retval == SYSERR) {
            fprintf(stderr, "Error occured\n");
            freemem(srcname, strlen(args[1]) + 1);
            return SYSERR;
        }
    }
    else if (retval == SYSERR){
        fprintf(stderr, "Error occured\n");
        freemem(srcname, strlen(args[1]) + 1);
        return SYSERR;
    }
    else {
        close(retval);
    }
    
    freemem(srcname, strlen(args[1]) + 1);

    return 0;
}