/* pipe.h */

/* Constants used by the pipes */

#ifndef NPIPE
#define	NPIPE        6		/* Number of pipes pseudo devices */
#endif

#define PIPEBUFFSIZE 256    /* Size of pipe buffer */

/* Structure for the pipe control block */

struct pipeblk {
    byte pstate;                       /* Is entry free or used */
    did32 pdev;                        /* Device ID of this device */
    int32 pmode;                       /* Mode (read/write) */
    int32 pbuff[PIPEBUFFSIZE];         /* Pipe buffer */
    sid32 esembuff;                    /* Binary semaphore for empty places used in synchronization */
    sid32 fsembuff;                    /* Binary semaphore for full places used in synchronization */
};

extern struct pipeblk pipetab[];