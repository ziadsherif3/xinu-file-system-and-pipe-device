/* xsh_cat.c - xsh_cat */

#include <xinu.h>
#define	BUFF_SIZE	512

/*------------------------------------------------------------------------
 * xsh_cat - Display File Contents
 *------------------------------------------------------------------------
 */
shellcmd xsh_cat(int nargs, char *args[]) {

    if (nargs != 2) {
		fprintf(stderr, "%s: invalid arguments\n", args[0]);
		fprintf(stderr, "Usage: cat \"filePath\"\n");
		return SYSERR;
	}
	
	did32 fileDev;	/* File device descriptor */
	char  readBuff[BUFF_SIZE];	/* buffer to hold read characters */
	uint64 numRead;	/* Number of Bytes read */
	char *srcname = getmem(strlen(args[1]) + 1);
	strncpy(srcname, args[1], strlen(args[1]) + 1);

	if (((fileDev = open(FSYSTEM, srcname, "r") ) == SYSERR) || (fileDev == NOTFOUND)) { /*Cannot open file */
		fprintf(stderr, "Cannot open file\n");
		freemem(srcname, strlen(args[1]) + 1);
		return SYSERR;
	}

	freemem(srcname, strlen(args[1]) + 1);

	struct dentry *devptr = (struct dentry *) &devtab[fileDev];
	struct lfcblk *lfptr = &lftab[devptr->dvminor];

	if (lfptr->lfinode.filestat.size == 0) {
		close(fileDev);
		return 0;
	}

	/* Read the file and write in stdoutput */


	numRead = read(fileDev, readBuff, BUFF_SIZE);
	while (numRead == BUFF_SIZE) {
		write(proctab[currpid].prdesc[1], readBuff, numRead);
		numRead = read(fileDev, readBuff, BUFF_SIZE);
	}

	if (numRead != SYSERR) {
		write(proctab[currpid].prdesc[1], readBuff, numRead);
	}

	close(fileDev);
	
	return 0;
}