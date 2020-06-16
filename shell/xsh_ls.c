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

	if (cwd->filestat.size == 0) {
		return 0;
	}

	int32 i;

	for (i = 0; i < (cwd->filestat.size); i++) {
		printf("%s\n", cwd->contents[i].name);
	}

    return 0;
}