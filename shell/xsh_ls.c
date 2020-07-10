/* xsh_ls.c - xsh_ls */

#include <xinu.h>

/*-----------------------------------------------------------------------------
 * xsh_ls - Displays information about files in the current working directory
 *-----------------------------------------------------------------------------
 */
shellcmd xsh_ls(int nargs, char *args[]) {

    if (nargs != 1) {
		fprintf(stderr, "%s: invalid argument\n", args[0]);
		fprintf(stderr, "Usage: ls\n");
		return SYSERR;
	}

	if (cwd.filestat.size == 0) {
		return 0;
	}

	int32 i;

	for (i = 0; i < (cwd.filestat.size); i++) {
		write(proctab[currpid].prdesc[1], cwd.contents[i].name, strlen(cwd.contents[i].name));
		write(proctab[currpid].prdesc[1], "\n", 1);
	}

    return 0;
}