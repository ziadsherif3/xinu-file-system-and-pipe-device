/* xsh_mkdir.c - xsh_mkdir */

#include <xinu.h>

/*------------------------------------------------------------------------
 * xhs_mkdir - Create a new Directory 
 *------------------------------------------------------------------------
 */
shellcmd xsh_mkdir(int nargs, char *args[]) {
    
        if (nargs != 2) {
		fprintf(stderr, "%s: invalid arguments\n", args[0]);
		fprintf(stderr, "Usage: mkdir \"filePath\"\n");
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

	int32 retval = control(FSYSTEM, FMKDIR, (int32)name, 0);
	
	if (retval == SYSERR) {
		fprintf(stderr, "Error occured\n");
		return SYSERR;
	}

	return 0;


}