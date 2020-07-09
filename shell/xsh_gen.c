/* xsh_gen.c - xsh_gen */

#include <xinu.h>
#include <stdlib.h>

/*------------------------------------------------------------------------
 * xsh_gen - Generate the specified or a random number of bytes
 *------------------------------------------------------------------------
 */
shellcmd xsh_gen(int nargs, char *args[]) {

    if (nargs > 2) {
		fprintf(stderr, "%s: invalid arguments\n", args[0]);
		fprintf(stderr, "Usage: gen [Number of bytes]\n");
		return SYSERR;
	}

    char buff = 'z';
    int32 i;

    if (nargs == 1) {
        srand(clktime);
        int32 randn = rand();
        for (i = 0; i < randn; i++) {
            write(proctab[currpid].prdesc[1], &buff, 1);
        }
    }
    else {
        int32 nbytes = atoi(args[1]);
        for (i = 0; i < nbytes; i++) {
            write(proctab[currpid].prdesc[1], &buff, 1);
        }
    }

    return 0;
}