/* xsh_rmdir.c - xsh_rmdir */

#include <xinu.h>

/*------------------------------------------------------------------------
 * xhs_rm - Delete an existing file
 *------------------------------------------------------------------------
 */
shellcmd xsh_rm(int nargs, char *args[]) {

    int32 i;
    for (i = 0; i<nargs; i++) {
        printf("%s\n",args[i]);
    }
    if (nargs != 2) {
		fprintf(stderr, "%s: invalid arguments\n", args[0]);
		fprintf(stderr, "Usage: rm \"filePath\"\n");
		return SYSERR;
    }
    
}