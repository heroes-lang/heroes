TASK 1
```
extern "sys/utsname.h"
    record Utsname tag utsname
        sysname: i8[256]
        nodename: i8[256]
        release: i8[256]
        version: i8[256]
        machine: i8[256]
    function uname(@name: Utsname) -> i64

function main()
    u: Utsname @ Utsname(sysname: [0; 256], nodename: [0; 256], release: [0; 256], version: [0; 256], machine: [0; 256])
    rc = uname(@u)
    if rc != 0
        print("uname failed: ", rc)
        exit(code: 1)
    match u.sysname.validated_bytes()
        .ok name => print(name)
        .err e   => print(e.code)
```
TASK 2
```
extern "app.h"
    record SockaddrUn tag sockaddr_un
        sun_len: u8
        sun_family: u8
        sun_path: i8[104]
    function app_connect(@addr: SockaddrUn) -> i64

function main()
    path = "/tmp/app.sock"
    assert path.len() < 104
    addr: SockaddrUn @ SockaddrUn(sun_len: 0, sun_family: 1, sun_path: [0; 104])
    for i in range(from: 0, to: path.len())
        addr.sun_path[i] @ to_i8(path[i]).must()
    fd = app_connect(@addr)
    if fd == -1
        print("app_connect failed")
        exit(code: 1)
    print("connected: ", fd)
```
TASK 3
```
extern "pthread.h"
    record pthread_mutex_t
        __sig: i64
        __opaque: i8[56]
    function pthread_mutex_init(@m: pthread_mutex_t, attr: ptr) -> i64
    function pthread_mutex_lock(@m: pthread_mutex_t) -> i64
    function pthread_mutex_unlock(@m: pthread_mutex_t) -> i64

function check(rc: i64, what: str)
    if rc != 0
        print(what, " failed: ", rc)
        exit(code: 1)

function main()
    m: pthread_mutex_t @ pthread_mutex_t(__sig: 0, __opaque: [0; 56])
    check(pthread_mutex_init(@m, nullptr), "pthread_mutex_init")
    check(pthread_mutex_lock(@m), "pthread_mutex_lock")
    print("locked")
    check(pthread_mutex_unlock(@m), "pthread_mutex_unlock")
    print("unlocked")
```
MUTEX: no. The cell starts as zeros only because every binding has to be initialised, and pthread_mutex_init(@m, nullptr) runs on it before any lock or unlock.
GUESSED: C `char` declared as `i8` (the `sysname`..`machine`, `sun_path` and `__opaque` fields) and the `to_i8(path[i]).must()` that follows from that; `u.sysname.validated_bytes()` counting an `i8[256]` as "a field of bytes"; `@addr: SockaddrUn` for the read-only `const struct sockaddr_un *` (the spec only says an OUT-parameter is `@`); `record pthread_mutex_t` taking the typedef name with no `tag`; `__sig` and `__opaque` being legal identifiers; `long` as `i64`; `attr: ptr` with `nullptr` standing for the NULL `const pthread_mutexattr_t *`; leaving `link` off all three groups (no library named for `app.h`, pthread assumed to be in libc); `sun_len: 0` (the header does not say what goes there).