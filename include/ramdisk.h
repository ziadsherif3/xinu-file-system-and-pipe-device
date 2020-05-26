/* ramdisk.h - definitions for a ram disk (for testing) */

/* Ram disk block size */

#ifndef RM_BLKSIZ
#define	RM_BLKSIZ	512		/* block size			*/
#endif

#define	RM_BLKS		2048		/* number of blocks		*/

struct	ramdisk	{
	char	disk[RM_BLKSIZ * RM_BLKS];
	};

extern	struct	ramdisk	Ram[];
