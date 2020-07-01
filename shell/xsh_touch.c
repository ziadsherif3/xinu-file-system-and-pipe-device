/* xsh_touch.c - xsh_touch */

#include <xinu.h>


/*------------------------------------------------------------------------
 * xhs_touch - Updates access and modification time of a file
 *------------------------------------------------------------------------
 */
shellcmd xsh_touch(int nargs, char *args[]) {

    if (nargs != 2) {
		fprintf(stderr, "%s: invalid arguments\n", args[0]);
		fprintf(stderr, "Usage: touch \"filePath\"\n");
		return SYSERR;
	}
    char  *fname;
    



    fname = getmem(NAME_LEN + strlen(args[1]) + 1);
    strncpy(fname, args[1], strlen(args[1]) + 1);
    did32 fileDesc;
    
    if ( (fileDesc = open(FSYSTEM, fname, "w")) == SYSERR) {
        
        if ( (fileDesc = control(FSYSTEM, FCREATE, (int32)fname, 0)) == SYSERR) {
            fprintf(stderr, "An Error occured, Recheck the arguments\n");
            return SYSERR;
        }
         if ( (fileDesc = open(FSYSTEM, fname, "w")) == SYSERR) {
            fprintf(stderr, "An Error occured, please mak sure the directory exists\n");
            fprintf(stderr, "Aborting...");
            return SYSERR;
        }
    }
    close(fileDesc);

    return 0;
}