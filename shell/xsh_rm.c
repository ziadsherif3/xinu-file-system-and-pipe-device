/* xsh_rmdir.c - xsh_rmdir */

#include <xinu.h>

/*------------------------------------------------------------------------
 * xsh_rm - Delete an existing file
 *------------------------------------------------------------------------
 */
shellcmd xsh_rm(int nargs, char *args[]) {
  
  if (nargs != 2) {
    fprintf(stderr, "%s: invalid arguments\n", args[0]);
    fprintf(stderr, "Usage: rm \"filePath\"\n");
		return SYSERR;
  }

  char *srcname = getmem(strlen(args[1]) + 1);
	strncpy(srcname, args[1], strlen(args[1]) + 1);

  if ((control(FSYSTEM, FDELETE, (int32)srcname, 0)) == SYSERR) {
    fprintf(stderr, "Error occured\n");
    freemem(srcname, strlen(args[1]) + 1);
    return SYSERR;
  }
  
  freemem(srcname, strlen(args[1]) + 1);

  return 0;
}