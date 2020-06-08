/* filesystem.h */

/* Constants used by the the local file system*/

#ifndef RM_BLKSIZ
#define	RM_BLKSIZ	512		/* block size */
#endif

#ifndef Nlfl
#define	Nlfl        16		/* Number of Pseudo devices */
#endif

#define DIR     0               /* Type directory of i-node */
#define FILE    1               /* Type file of i-node */
#define FREE    0               /* Pseudo-device control block is free */
#define USED    1               /* Pseudo-device control block is used */
#define NAME_LEN 12             /* Length of file/directory name */
#define ntuples 25              /* Number of tuples */
#define LF_DNULL (dbid32) -1    /* Data block null pointer */
#define rmode   0               /* Read mode for a file */
#define wmode   1               /* write mode for a file */

#define FCREATE     0           /* File create */
#define FDELETE     1           /* File delete */
#define FMKDIR      2           /* directory create */
#define FRMDIR      3           /* directory delete */

/* Structure for the tuple used in directory i-node */

struct tuple {
    char name[NAME_LEN];    /* Name of file/directory */
    int32 blcknum;          /* Disk block number */
};

/* Structure for stat information used in the file i-node */

struct stat {
    int32 ino;          /* inode number */
    int32 dev;          /* device number */
    int32 size;         /* file size/number of directory contents */
    uint64 acctime;     /* time of last access in seconds (since the system has started)*/
    uint64 ctime;       /* time of creation in seconds */
    uint64 mtime;       /* time of last modification in seconds */
};

/* Structure for the file/directory i-node */

struct inode {
    int32 type;                         /* Type file or directory */
    struct tuple contents[ntuples];     /* Tuples in case of an i-node of type DIR */
    struct stat filestat;               /* stat information for an i-node of type FILE */
    int32 datablcks[12];                /* Data blocks for an i-node of type FILE */
    char name[NAME_LEN];                /* Name of file/directory of this i-node */
    char padding[12];                   /* Padding data */
};

/* Structure used by the the local file system (initialized in lmfinit) */

struct lmf {
    did32 ram0dskdev;   /* Device ID of RAMDISK0 */
    did32 ram1dskdev;   /* Device ID of RAMDISK1 */
    sid32 lmf_mutex0;   /* Mutex for the root of RAMDISK0 */
    sid32 lmf_mutex1;   /* Mutex for the root of RAMDISK1 */
    sid32 lftabmutex;   /* Mutex for lftab */
    struct inode rt0;   /* Root for RAMDISK0 */
    struct inode rt1;   /* Root for RAMDISK1 */
    bool8 rt0present;   /* True when root0 is in memory */
    dbid32 r0freepos;   /* Current position (start of free list) for RAMDISK0 */
    bool8 rt1present;   /* True when root1 is in memory */
    dbid32 r1freepos;   /* Current position (start of free list) for RAMDISK1 */
};

/* Structure for control block of local file pseudo-device */

struct lfcblk {
    byte lfstate;               /* Is entry free or used */
    did32 lfdev;                /* Device ID of this device */
    did32 lfram;                /* Which ram disk RAMDISK0/RAMDISK1 the file is found in? */
    sid32 lfmutex;              /* Mutex for this file */
    struct inode *lfinode;      /* In-memory i-node of file */
    int32 lfmode;               /* Mode (read/write) */
    uint32 lfoffset;            /* Byte position of next byte to read or write */
    char lfname[NAME_LEN];      /* Name of the file */
    dbid32 lfdnum;              /* Number of current data block in lfdblock or LF_DNULL */
    char lfdblock[RM_BLKSIZ];   /* In-mem copy of current data block */
    char *lfbyte;               /* Ptr to byte in lfdblock or address one beyond lfdblock if current file pos lies outside lfdblock */
    bool8 lfdbdirty;            /* Has lfdblock changed? */
};

/* Structure to keep track of free disk blocks linked list */

struct lfdbfree {
    dbid32 lf_nextdb;                           /* Next data block on the list */
    char padding[RM_BLKSIZ - sizeof(dbid32)];   /* Padding to take the whole disk block */
};


extern struct lfcblk lftab[];
extern struct lmf fsystem;

/* Helper functions declerations */

dbid32 dballoc (struct lfdbfree *, int32);
void dbclear(char *);
status dbfree(did32, dbid32);
status fstat(char *, struct stat *);
status lfflush (struct lfcblk *);
status lfsetup (struct lfcblk *);
status lftruncate (struct lfcblk *);
