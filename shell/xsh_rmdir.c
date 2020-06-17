/* xsh_rmdir.c - xsh_rmdir */

#include <xinu.h>

/*------------------------------------------------------------------------
 * xhs_rmdir - Delete an existing directory provided it's empty
 *------------------------------------------------------------------------
 */
shellcmd xsh_rmdir(int nargs, char *args[]) {

    if (nargs != 2) {
		fprintf(stderr, "%s: invalid arguments\n", args[0]);
		fprintf(stderr, "Usage: rmdir \"filePath\"\n");
		return SYSERR;
	}

    char *name;     /* Directory path name */

    name = getmem(strlen(args[1]) + 1);
    strncpy(name, args[1], strlen(args[1]) + 1);

    if (strlen(args[1]) > 5) {
        if (args[1][strlen(args[1]) - 1] == '/') {
            strncpy(name, args[1], strlen(args[1]));
            name[strlen(args[1]) - 1] = NULLCH;
        }
    }

	int32 retval = control(FSYSTEM, FRMDIR, (int32)name, 0);
	
	if (retval == SYSERR) {
		fprintf(stderr, "Error occured, Please make sure the directory is empty\n");
        return SYSERR;
	}

	return 0;
}

    
