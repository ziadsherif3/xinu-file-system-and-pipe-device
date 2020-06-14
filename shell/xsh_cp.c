/* xsh_cp.c - xsh_cp */

#include <xinu.h>


/*------------------------------------------------------------------------
 * xhs_cp - Copies source file into destination
 *------------------------------------------------------------------------
 */
shellcmd xsh_cp(int nargs, char *args[]) {

    if (nargs != 3) {
        fprintf(stderr, "%s: invalid arguments\n", args[0]);
        fprintf(stderr, "Usage: cat \"filePath\" \"filePath\"\n");
        return SYSERR;
    }
}