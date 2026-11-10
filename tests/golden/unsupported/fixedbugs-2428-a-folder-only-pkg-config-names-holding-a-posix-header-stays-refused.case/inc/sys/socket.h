/* The library's folder holds a sys/socket.h that reads the platform's
   and changes it: the ffi-pragmatist's `pcsocket` (panel 243). */
#include_next <sys/socket.h>
#undef AF_INET
#define AF_INET 99
