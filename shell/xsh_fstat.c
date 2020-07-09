/* xsh_fstat.c - xsh_fstat */

#include <xinu.h>

/*------------------------------------------------------------------------
 * xsh_fstat - Display the filestat of the file specified in args
 *------------------------------------------------------------------------
 */
shellcmd xsh_fstat(int nargs, char *args[]) {
    
    if (nargs != 2) {
		fprintf(stderr, "%s: invalid arguments\n", args[0]);
		fprintf(stderr, "Usage: fstat \"filePath\"\n");
		return SYSERR;
	}

    char *srcname = getmem(strlen(args[1]) + 1);
    strncpy(srcname, args[1], strlen(args[1]) + 1);
    struct stat *fs = NULL;
    fstat(srcname, fs);
    printf("%d\n%d\n%d\n%d\n", fs->size, fs->ctime, fs->acctime, fs->mtime);

    return 0;
}