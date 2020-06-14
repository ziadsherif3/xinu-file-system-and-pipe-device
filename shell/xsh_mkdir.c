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


}