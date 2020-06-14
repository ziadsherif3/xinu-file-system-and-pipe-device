/* xsh_rmdir.c - xsh_rmdir */

#include <xinu.h>

/*------------------------------------------------------------------------
 * xhs_rm - Delete an existing file
 *------------------------------------------------------------------------
 */
shellcmd xsh_rm(int nargs, char *args[]) {

    if (nargs != 2) {
		fprintf(stderr, "%s: invalid arguments\n", args[0]);
		fprintf(stderr, "Usage: rm \"filePath\"\n");
		return SYSERR;
    }

    if ( (control(FSYSTEM, FDELETE,(int32) args[1],0)) == SYSERR) {
      return SYSERR;
    }
    return 0;
    
}