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
	char * readBuff;	/* buffer to hold read characters */
	int32 numRead;	/* Number of Bytes read */

	if ( (fileDev = open(FSYSTEM, args[1], "r") )  == SYSERR) { /*Cannot open file */
		return SYSERR;
	}

	/* Read the file */
	numRead = read(fileDev, readBuff, 100);
	printf("%s\n", readBuff);
	close(fileDev);
	return 0;

}