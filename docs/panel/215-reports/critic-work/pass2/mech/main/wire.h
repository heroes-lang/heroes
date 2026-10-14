/* ffi-pragmatist, panel 215: a padded wire record a program owns; the readers are libc's and OpenSSL's own. */
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
struct cd { char c; double d; };      /* 7 bytes of padding after c; 16 bytes */
