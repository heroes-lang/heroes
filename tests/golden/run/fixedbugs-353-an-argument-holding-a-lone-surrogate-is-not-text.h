/* Defect 353's fixture: the program runs itself with one argument holding a
 * lone surrogate, `x`, U+D800 alone, `y`, as each platform hands one over:
 * Windows a UTF-16 unit that no UTF-8 holds, POSIX the three bytes that unit
 * would take (WTF-8), which are not UTF-8 either. `words` 2 adds a second
 * argument, `extra`. The child's stdout is this process's own, flushed
 * first so the lines keep their order; its stderr is discarded, since the
 * child that asks `args()` stops with a panic there. Returns the child's exit
 * status, or -1 where it could not be started. */
#include <stdio.h>
#if defined(_WIN32)
#include <windows.h>
#include <wchar.h>

static inline int64_t run_self(int64_t words) {
    static wchar_t self[32768];
    static wchar_t line[32768 + 64];
    DWORD n = GetModuleFileNameW(NULL, self, 32768);
    if (n == 0 || n >= 32768) return -1;
    wcscpy(line, L"\"");
    wcscat(line, self);
    wcscat(line, L"\" x\xD800y");
    if (words == 2) wcscat(line, L" extra");
    SECURITY_ATTRIBUTES inherit = {sizeof inherit, NULL, TRUE};
    HANDLE nul = CreateFileW(L"NUL", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, &inherit,
                             OPEN_EXISTING, 0, NULL);
    if (nul == INVALID_HANDLE_VALUE) return -1;
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    SetHandleInformation(out, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
    STARTUPINFOW start;
    ZeroMemory(&start, sizeof start);
    start.cb = sizeof start;
    start.dwFlags = STARTF_USESTDHANDLES;
    start.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    start.hStdOutput = out;
    start.hStdError = nul;
    PROCESS_INFORMATION child;
    fflush(stdout);
    if (!CreateProcessW(self, line, NULL, NULL, TRUE, 0, NULL, NULL, &start, &child)) {
        CloseHandle(nul);
        return -1;
    }
    WaitForSingleObject(child.hProcess, INFINITE);
    DWORD code = 0;
    GetExitCodeProcess(child.hProcess, &code);
    CloseHandle(child.hThread);
    CloseHandle(child.hProcess);
    CloseHandle(nul);
    return (int64_t)code;
}
#else
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#if defined(__APPLE__)
#include <mach-o/dyld.h>
#endif

static inline int64_t run_self(int64_t words) {
    static char self[4096];
#if defined(__APPLE__)
    uint32_t size = sizeof self;
    if (_NSGetExecutablePath(self, &size) != 0) return -1;
#else
    ssize_t n = readlink("/proc/self/exe", self, sizeof self - 1);
    if (n < 0) return -1;
    self[n] = '\0';
#endif
    static char word[] = "x\xED\xA0\x80y";
    static char extra[] = "extra";
    char *argv[] = {self, word, words == 2 ? extra : NULL, NULL};
    fflush(stdout);
    pid_t pid = fork();
    if (pid < 0) return -1;
    if (pid == 0) {
        int nul = open("/dev/null", O_WRONLY);
        if (nul >= 0) dup2(nul, 2);
        execv(self, argv);
        _exit(127);
    }
    int status = 0;
    if (waitpid(pid, &status, 0) < 0) return -1;
    return WIFEXITED(status) ? (int64_t)WEXITSTATUS(status) : (int64_t)(128 + WTERMSIG(status));
}
#endif
