/* fstat.c - fstat */

#include <xinu.h>
#include <filesystem.h>

/*------------------------------------------------------------------------
* fstat - returns the statistics of file in the structure stat
*------------------------------------------------------------------------
*/
status fstat(
    char* filename,             /* Name of file */
    struct stat* filestat       /* Pointer to structure */
)
{
    return OK;
}