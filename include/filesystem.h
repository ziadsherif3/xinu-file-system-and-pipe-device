/* filesystem.h */

/* Constants used by the the local file system*/

#ifndef RM_BLKSIZ
#define	RM_BLKSIZ	512		/* block size			*/
#endif

#define DIR     0               /* Type directory of i-node */
#define FILE    1               /* Type file of i-node */
#define FREE    0               /* Pseudo-device control block is free */
#define USED    1               /* Pseudo-device control block is used */
#define NAME_LEN 12             /* Length of file/directory name */
#define LF_DNULL (dbid32) -1    /* Data block null pointer */


/* Structure for the tuple used in directory i-node */

struct tuple {
    char name[12];      /* Name of file/directory */
    int32 blcknum;      /* Disk block number */
};

/* Structure for stat information used in the file i-node */

struct stat {
    int32 ino;          /* inode number */
    int32 dev;          /* device number */
    int32 size;         /* file size */
    uint64 acctime;     /* time of last access in seconds (since the system has started)*/
    uint64 ctime;       /* time of creation in seconds */
    uint64 mtime;       /* time of last modification in seconds */
};

/* Structure for the file/directory i-node */

struct inode {
    int32 type;                 /* Type file or directory */
    struct tuple contents[25];  /* Tuples in case of an i-node of type DIR */
    struct stat filestat;       /* stat information for an i-node of type FILE */
    int32 datablcks[12];        /* Data blocks for an i-node of type FILE */
    char name[12];              /* Name of file/directory of this i-node */
    char padding[12];           /* Padding data */
};

/* Structure used by the the local file system (initialized in lmfinit) */

struct lmf {
    sid32 lmf_mutex0;   /* Mutex for the root of RAMDISK0 */
    sid32 lmf_mutex1;   /* Mutex for the root of RAMDISK1 */
    struct inode rt0;   /* Root for RAMDISK0 */
    struct inode rt1;   /* Root for RAMDISK1 */
    bool8 rt0present;   /* True when root0 is in memory */
    bool8 rt0dirty;     /* Has root0 changed? */
    bool8 rt1present;   /* True when root1 is in memory */
    bool8 rt1dirty;     /* Has root1 changed? */
};

/* Structure for control block of local file pseudo-device */

struct lfcblk {
    byte lfstate;               /* Is entry free or used */
    did32 lfdev;                /* Device ID of this device */
    sid32 lfmutex;              /* Mutex for this file */
    struct inode *lfinode;      /* Ptr to file’s i-node */
    int32 lfmode;               /* Mode (read/write) */
    uint32 lfpos;               /* Byte position of next byte to read or write */
    char lfname[NAME_LEN];      /* Name of the file */
    dbid32 lfdnum;              /* Number of current data block in lfdblock or LF_DNULL */
    char lfdblock[RM_BLKSIZ];   /* In-mem copy of current data block */
    char *lfbyte;               /* Ptr to byte in lfdblock or address one beyond lfdblock if current file pos lies outside lfdblock */
    bool8 lfdbdirty;            /* Has lfdblock changed? */
};