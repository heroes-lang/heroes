/* Critic, panel 178 reader test: task 2's header, on Darwin's real struct sockaddr_un
   (sun_len u8, sun_family u8, sun_path char[104], the brief's layout). */
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
static inline int app_connect(const struct sockaddr_un *addr) {
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) return -1;
    if (connect(fd, (const struct sockaddr *)addr, (socklen_t)sizeof *addr) != 0) { close(fd); return -1; }
    return fd;
}
