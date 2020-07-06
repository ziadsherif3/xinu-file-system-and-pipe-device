/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xhs_echo - write argument strings to stdout
 *------------------------------------------------------------------------
 */
shellcmd xsh_echo(int nargs, char *args[])
{
	int32	i;			/* walks through args array	*/
	if (nargs > 1) {
		write(proctab[currpid].prdesc[1], args[1], strlen(args[1]));

		for (i = 2; i < nargs; i++) {
			write(proctab[currpid].prdesc[1], args[i], strlen(args[i]));
		}
	}
	
	write(proctab[currpid].prdesc[1], "\n", 1);

	return 0;
}
