/* Defect 2606: the program's own C for its case on Windows, the twin of
 * defect 2600's header for POSIX (`fixedbugs-2600-stderr-into-a-pipe-nobody-
 * reads.h`): it makes the process's stderr the write end of a pipe the
 * program keeps open and never reads, so a thread that writes there fills it
 * and then blocks inside its write, holding stderr's locks. Windows alone,
 * since it names `windows.h`: elsewhere the case is skipped for the header
 * the platform lacks, and the shape is judged by the POSIX case. Nothing
 * here allocates or prints. */
#include <stdint.h>
#include <fcntl.h>
#include <io.h>
#include <windows.h>

/* 1 once stderr is the unread pipe, its read end kept open, 0 where the
 * system refused a call. */
static inline int64_t unread_stderr(void) {
    HANDLE r, w;

    if (!CreatePipe(&r, &w, NULL, 4096)) return 0;
    int fd = _open_osfhandle((intptr_t)w, _O_WRONLY | _O_BINARY);
    if (fd < 0 || _dup2(fd, 2) != 0) return 0;
    (void)SetStdHandle(STD_ERROR_HANDLE, (HANDLE)_get_osfhandle(2));
    (void)_close(fd);
    return 1;
}

/* Waits `us` microseconds, to the millisecond. */
static inline int64_t pause_us(int64_t us) {
    Sleep((DWORD)(us / 1000));
    return 0;
}
