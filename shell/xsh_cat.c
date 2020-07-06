/* xsh_cat.c - xsh_cat */

#include <xinu.h>


/*------------------------------------------------------------------------
 * xhs_cat - Display File Contents
 *------------------------------------------------------------------------
 */
shellcmd xsh_cat(int nargs, char *args[]) {

    if (nargs != 2) {
		fprintf(stderr, "%s: invalid arguments\n", args[0]);
		fprintf(stderr, "Usage: cat \"filePath\"\n");
		return SYSERR;
	}
	did32 fileDev;	/* File device descriptor */
	char  readBuff[MAXFILESIZE];	/* buffer to hold read characters */
	uint64 numRead;	/* Number of Bytes read */
	uint64 i;
	char *srcname = getmem(strlen(args[1]) + 1);
	strncpy(srcname, args[1], strlen(args[1]) + 1);

	if (((fileDev = open(FSYSTEM, srcname, "r") ) == SYSERR) || (fileDev == NOTFOUND)) { /*Cannot open file */
		fprintf(stderr, "Cannot open file\n");
		freemem(srcname, strlen(args[1]) + 1);
		return SYSERR;
	}

	freemem(srcname, strlen(args[1]) + 1);

	/* Read the file */
	numRead = read(fileDev, readBuff, MAXFILESIZE);
	close(fileDev);
	
	for (i = 0; i < numRead; i++) {
		printf("%c", readBuff[i]);
	}
	
	return 0;
}