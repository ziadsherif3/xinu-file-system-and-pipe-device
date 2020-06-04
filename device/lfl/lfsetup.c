/* lfsetup.c - lfsetup */

#include <xinu.h>

/*------------------------------------------------------------------------
* lfsetup - Set a file’s data block for the current file position
*                   (assumes file mutex held)
*------------------------------------------------------------------------
*/
status lfsetup (
    struct lfcblk *lfptr    /* Pointer to slave file device */
)
{
    return OK;
}