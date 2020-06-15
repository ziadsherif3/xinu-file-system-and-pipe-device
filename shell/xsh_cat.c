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
	char  readBuff[100];	/* buffer to hold read characters */
	uint64 numRead;	/* Number of Bytes read */
	uint64 i;

	if ( ((fileDev = open(FSYSTEM, args[1], "r") )  == SYSERR) || (fileDev == NOTFOUND)) { /*Cannot open file */
		return SYSERR;
	}

	/* Read the file */
	numRead = read(fileDev, readBuff, MAXFILESIZE);
	for (i = 0; i < numRead; i++) {
		printf("%c",readBuff[i]);
	}
	close(fileDev);
	return 0;

}