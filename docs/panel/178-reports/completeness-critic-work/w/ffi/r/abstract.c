/* Panel 178 ffi-pragmatist: Linux abstract sockets name a socket by EVERY byte
   of sun_path up to addrlen, so the bytes after the name are compared by the
   kernel. Bind with the rest zero; connect with the rest zero, then with one
   stray byte at the end. */
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
int main(void) {
    struct sockaddr_un a; memset(&a, 0, sizeof a); a.sun_family = AF_UNIX; memcpy(a.sun_path + 1, "heroes-178", 10);
    int s = socket(AF_UNIX, SOCK_STREAM, 0); int b = bind(s, (struct sockaddr *)&a, sizeof a); listen(s, 1);
    int c1 = socket(AF_UNIX, SOCK_STREAM, 0); int k1 = connect(c1, (struct sockaddr *)&a, sizeof a); int e1 = errno;
    struct sockaddr_un d = a; d.sun_path[sizeof d.sun_path - 1] = (char)0xAA;
    int c2 = socket(AF_UNIX, SOCK_STREAM, 0); int k2 = connect(c2, (struct sockaddr *)&d, sizeof d); int e2 = errno;
    printf("abstract bind=%d; connect, rest zero=%d%s%s; connect, last byte 0xAA=%d%s%s\n", b, k1, k1 ? " " : "", k1 ? strerror(e1) : "", k2, k2 ? " " : "", k2 ? strerror(e2) : "");
    return 0;
}
