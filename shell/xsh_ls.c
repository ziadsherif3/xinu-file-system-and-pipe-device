/* xsh_ls.c - xsh_ls */

#include <xinu.h>

/*-----------------------------------------------------------------------------
 * xhs_ls - Displays information about files in the current working directory
 *-----------------------------------------------------------------------------
 */
shellcmd xsh_ls(int nargs, char *args[]) {

	if (nargs != 1) {
		fprintf(stderr, "%s: invalid argument\n", args[0]);
		fprintf(stderr, "Usage: ls\n");
		return SYSERR;
	}


	

    return 0;
    
}