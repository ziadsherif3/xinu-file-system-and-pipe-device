/* xsh_count.c - xsh_count */

#include <xinu.h>
#define BUFF_SIZE   512
#define OBUFF_SIZE  30

/*------------------------------------------------------------------------
 * xsh_count - Count the bytes in stdinput
 *------------------------------------------------------------------------
 */
shellcmd xsh_count(int nargs, char *args[]) {

    if (nargs != 1) {
		fprintf(stderr, "%s: invalid arguments\n", args[0]);
		return SYSERR;
	}

    char buff[BUFF_SIZE];
    char outbuff[OBUFF_SIZE];
    int32 rdcount;
    int32 retval = read(proctab[currpid].prdesc[0], buff, BUFF_SIZE);
    if (retval == SYSERR){
        fprintf(stderr, "Error occurred\n");
        return SYSERR;
    }
    while (retval == BUFF_SIZE) {
        rdcount += retval;
        retval = read(proctab[currpid].prdesc[0], buff, BUFF_SIZE);
    }
    if (retval != EOF) {
        rdcount += retval;
    }
    sprintf(outbuff, "%d total bytes received\n\0", rdcount);
    write(proctab[currpid].prdesc[1], outbuff, strlen(outbuff));

    return 0;
}