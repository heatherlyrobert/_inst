/*===[[ START HDOC ]]=========================================================*/
/*===[[ HEADER ]]=============================================================#

 *   category      : (PG) programming support
 *   niche         : custom shared and static libraries
 *   application   : _lib, library installer
 *   purpose       : provide simple, consistent, clean library installation
 *   base_system   : gnu/linux
 *   lang_name     : pure ansi-c (right, just, best, and universal)
 *   created       : 2008-07
 *   author        : the_heatherlys
 *   dependencies  : none
 * 
 */
/*===[[ PURPOSE ]]=============================================================*

 *   _lib is a quick set of capabilities to move custom developed shared (.so)
 *   and static (.a) libraries into the right places for use by other programs
 *   as well as their associated header files.  it is intended as a very small
 *   helper application only.
 *
 *   _lib focuses only on providing the ability to...
 *      - install libraries into system folders
 *      - listing the installed libraries
 *      - deleting specific installed libraries
 *      - installing custom shared libraries and their headers
 *      - installing custom static libraries and their headers
 *
 *   many existing libraries and utilities have been built by better programmers
 *   which are likely superior in speed, size, capability, and reliability; BUT,
 *   i would not have learned nearly as much using them, so i follow the adage...
 *
 *   TO TRULY LEARN> do not seek to follow in the footsteps of the men of old;
 *   seek what they sought ~ Matsuo Basho
 *
 */
/*===[[ END HDOC ]]===========================================================*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define  HEADERS      "/usr/local/include"
#define  LIBRARIES    "/usr/local/lib"

#define   P_VERNUM      "2.0f"
#define   P_VERTXT      "new library versioning fix for permissions on install"

typedef struct stat      tSTAT;
#define   LEN_FULL       200
#define   LEN_TERSE       10
typedef   const char   cchar;

