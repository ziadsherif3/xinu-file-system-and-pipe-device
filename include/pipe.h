/* pipe.h */

/* Constants used by the pipes */

#ifndef NPIPE
#define	NPIPE        6		/* Number of pipes pseudo devices */
#endif

#define PIPEBUFFSIZE 256    /* Size of pipe buffer */
#define pdmode 2            /* Pipe done mode for a pipe before closing */

/* Structure for the pipe control block */

struct pipeblk {
    byte pstate;                        /* Is entry free or used */
    did32 pdev;                         /* Device ID of this device */
    int32 pmode;                        /* Mode (read/write) */
    int32 pbuff[PIPEBUFFSIZE];          /* Pipe buffer */
    sid32 esembuff;                     /* Binary semaphore for empty places used in synchronization */
    sid32 fsembuff;                     /* Binary semaphore for full places used in synchronization */
    sid32 pipsem;                       /* Global semaphore on pipe */
    byte wpointer;                      /* The next position in pipe buffer to be written in before calling close as a writer */
    byte rpointer;                      /* The next position in pipe buffer to be read from before calling close as a reader */
};

extern struct pipeblk pipetab[];