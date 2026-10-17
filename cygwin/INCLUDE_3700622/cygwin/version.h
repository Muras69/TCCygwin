/* version.h -- Cygwin version numbers and accompanying documentation.

This file is part of Cygwin.

This software is a copyrighted work licensed under the terms of the
Cygwin license.  Please consult the file "CYGWIN_LICENSE" for
details. */

/* The DLL major and minor numbers correspond to the "version of
   the Cygwin shared library".  This version is used to track important
   changes to the DLL and is mainly informative in nature. */

#define CYGWIN_VERSION_DLL_MAJOR 3007
#define CYGWIN_VERSION_DLL_MINOR 0

/* CYGWIN_VERSION_DLL_COMBINED gives us a single number representing the
   combined DLL major and minor numbers. */

/* WATCH OUT FOR OCTAL!  Don't use, say, "00020" for 0.20 */

#define CYGWIN_VERSION_DLL_MAKE_COMBINED(maj, min) (((maj) * 1000) + min)
#define CYGWIN_VERSION_DLL_COMBINED \
  CYGWIN_VERSION_DLL_MAKE_COMBINED (CYGWIN_VERSION_DLL_MAJOR, CYGWIN_VERSION_DLL_MINOR)

/* Every version of cygwin <= this uses an old, incorrect method to determine
   signal masks. */
#define CYGWIN_VERSION_PER_PROCESS_API_VERSION_COMBINED(u) \
  CYGWIN_VERSION_DLL_MAKE_COMBINED ((u)->api_major, (u)->api_minor)

#define CYGWIN_VERSION_USER_API_VERSION_COMBINED \
  CYGWIN_VERSION_PER_PROCESS_API_VERSION_COMBINED (user_data)

/* Please note that the first API_VERSION of the 64 bit build was 0.263.
   For macros checking for older versions, bisect the git repo. */

#define CYGWIN_VERSION_CHECK_FOR_EXTRA_TM_MEMBERS \
  (CYGWIN_VERSION_USER_API_VERSION_COMBINED >= 272)

#define CYGWIN_VERSION_API_MAJOR 0
#define CYGWIN_VERSION_API_MINOR 362

/* There is also a compatibity version number associated with the shared memory
   regions.  It is incremented when incompatible changes are made to the shared
   memory region *or* to any named shared mutexes, semaphores, etc. */

#define CYGWIN_VERSION_SHARED_DATA 5

/* An identifier used in the names used to create shared objects.  The full
   names include the CYGWIN_VERSION_SHARED_DATA version as well as this
   identifier. */

#define CYGWIN_VERSION_DLL_IDENTIFIER	"cygwin1"

/* The Cygwin mount table interface in the Win32 registry also has a version
   number associated with it in case that is changed in a non-backwards
   compatible fashion.  Increment this version number whenever incompatible
   changes in mount table registry usage are made.

   1: Original number version.
   2: New mount registry layout, system-wide mount accessibility.
   3: The mount table is not in the registry anymore, but in /etc/fstab.
*/

#define CYGWIN_VERSION_MOUNT_REGISTRY 3

/* Identifiers used in the Win32 registry. */

#define CYGWIN_INFO_CYGWIN_REGISTRY_NAME "Cygwin"
#define CYGWIN_INFO_INSTALLATIONS_NAME   "Installations"

/* The default cygdrive prefix. */

#define CYGWIN_INFO_CYGDRIVE_DEFAULT_PREFIX "/cygdrive"

/* In addition to the above version number strings, the build process adds some
   strings that may be useful in debugging/identifying a particular Cygwin DLL:

   The mkvers.sh script at the top level produces a .cc file which initializes
   a cygwin_version structure based on the above version information and
   creates a string table for grepping via "grep -F '%%%' cygwinwhatever.dll"
   if you are using GNU grep.  Otherwise you may want to do a
   "strings cygwinwhatever.dll | grep -F '%%%'" instead.

   This will produce output such as:

   %%% Cygwin dll_identifier: cygwin
   %%% Cygwin api_major: 0
   %%% Cygwin api_minor: 0
   %%% Cygwin dll_major: 19
   %%% Cygwin dll_minor: 6
   %%% Cygwin shared_data: 1
   %%% Cygwin registry: b15
   %%% Cygwin build date: Wed Oct 14 16:26:51 EDT 1998
   %%% Cygwin shared id: cygwinS1

   This information can also be obtained through a call to
   cygwin_internal (CW_GETVERSIONINFO). */

#define CYGWIN_VERSION_MAGIC(a, b) ((unsigned) ((((unsigned short) a) << 16) | (unsigned short) b))
#define CYGWIN_VERSION_MAGIC_VERSION(a) ((unsigned) ((unsigned)a & 0xffff))
