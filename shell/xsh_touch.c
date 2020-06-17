/* xsh_touch.c - xsh_touch */

#include <xinu.h>


/*------------------------------------------------------------------------
 * xhs_touch - Updates access and modification time of a file
 *------------------------------------------------------------------------
 */
shellcmd xsh_touch(int nargs, char *args[]) {

    if (nargs != 2) {
		fprintf(stderr, "%s: invalid arguments\n", args[0]);
		fprintf(stderr, "Usage: touch \"filePath\"\n");
		return SYSERR;
	}
    char  *fname;

    fname = getmem(NAME_LEN + strlen(args[1]) + 1);
    strncpy(fname, args[1], strlen(args[1]) + 1);

    int32 retval = open(FSYSTEM, fname, "r");

    if (retval == SYSERR) {
        retval = control(FSYSTEM, FCREATE, (int32)fname, 0);
        if (retval == SYSERR) {
            fprintf(stderr, "File is in use\n");
            return SYSERR;
        }
    }
    else {
        close(retval);
    }

    return 0;
}