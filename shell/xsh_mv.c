/* xsh_mv.c - xsh_mv */

#include <xinu.h>

/*------------------------------------------------------------------------
 * xhs_mv - Moves (Renames) an oldname to newname
 *------------------------------------------------------------------------
 */
shellcmd xsh_mv(int nargs, char *args[]) {


        if (nargs != 3) {
		fprintf(stderr, "%s: invalid arguments\n", args[0]);
		fprintf(stderr, "Usage: mv \"filePath\" \"filePath\"\n");
		return SYSERR;
	}

}