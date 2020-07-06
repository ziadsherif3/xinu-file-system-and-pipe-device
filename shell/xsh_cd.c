/* xsh_cd.c - xsh_cd */

#include <xinu.h>

/*------------------------------------------------------------------------
 * xhs_cd - Change the working directory
 *------------------------------------------------------------------------
 */
shellcmd xsh_cd(int nargs, char *args[]) {

    if (nargs != 2) {
		fprintf(stderr, "%s: invalid arguments\n", args[0]);
		fprintf(stderr, "Usage: cd \"filePath\"\n");
		return SYSERR;
	}

    char *name;     /* Directory path name */

    name = getmem(strlen(args[1]) + 1);
    strncpy(name, args[1], strlen(args[1]) + 1);

    if (strlen(args[1]) > 6) {
        if (args[1][strlen(args[1]) - 1] == '/') {
            strncpy(name, args[1], strlen(args[1]));
            name[strlen(args[1]) - 1] = NULLCH;
        }
    }

    int32 retval = control(FSYSTEM, DIROPEN, (int32)name, 0);
    freemem(name, strlen(args[1]) + 1);

    if ((retval == SYSERR) || (retval == NOTFOUND)) {
        fprintf(stderr, "Error occured\n");
        return SYSERR;
    }

    return 0;
}