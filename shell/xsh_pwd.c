/* xsh_pwd.c - xsh_pwd */

#include <xinu.h>

/*------------------------------------------------------------------------
 * xhs_pwd - Display Current Directory path
 *------------------------------------------------------------------------
 */
shellcmd xsh_pwd(int nargs, char *args[]) {

    if (nargs != 1) {
		fprintf(stderr, "%s: invalid argument\n", args[0]);
		fprintf(stderr, "Usage: pwd\n");
		return SYSERR;
	}

     printf("%s\n", cwd->name);

	return 0;


}