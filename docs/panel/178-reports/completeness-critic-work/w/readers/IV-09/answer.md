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
    u: Utsname @ Utsname(
        sysname: [0; 256],
        nodename: [0; 256],
        release: [0; 256],
        version: [0; 256],
        machine: [0; 256])
    rc = uname(@u)
    if rc != 0
        print(f"uname failed: {rc}")
        exit(code: 1)
    match u.sysname.validated_bytes()
        .ok name => print(name)
        .err e => print(f"sysname is not text: {e.code}")
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
    addr: SockaddrUn @ SockaddrUn(sun_len: 0, sun_family: 1, sun_path: [0; 104])
    addr.sun_path[0] @ '/'
    addr.sun_path[1] @ 't'
    addr.sun_path[2] @ 'm'
    addr.sun_path[3] @ 'p'
    addr.sun_path[4] @ '/'
    addr.sun_path[5] @ 'a'
    addr.sun_path[6] @ 'p'
    addr.sun_path[7] @ 'p'
    addr.sun_path[8] @ '.'
    addr.sun_path[9] @ 's'
    addr.sun_path[10] @ 'o'
    addr.sun_path[11] @ 'c'
    addr.sun_path[12] @ 'k'
    fd = app_connect(@addr)
    if fd == -1
        print("could not connect to /tmp/app.sock")
        exit(code: 1)
    print(f"connected, descriptor {fd}")
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

function main()
    m: pthread_mutex_t @ pthread_mutex_t(__sig: 0, __opaque: [0; 56])
    rc_init = pthread_mutex_init(@m, nullptr)
    if rc_init != 0
        print(f"pthread_mutex_init failed: {rc_init}")
        exit(code: 1)
    rc_lock = pthread_mutex_lock(@m)
    if rc_lock != 0
        print(f"pthread_mutex_lock failed: {rc_lock}")
        exit(code: 1)
    print("locked")
    rc_unlock = pthread_mutex_unlock(@m)
    if rc_unlock != 0
        print(f"pthread_mutex_unlock failed: {rc_unlock}")
        exit(code: 1)
    print("unlocked")
```
MUTEX: no. The cell starts as zeros only because every binding has to be initialised, and pthread_mutex_init runs on it before any lock or unlock.
GUESSED: `char` fields written as `i8[N]` (I assumed char is signed) and `validated_bytes()` accepting an `i8` field; `@addr: SockaddrUn` for the `const struct sockaddr_un *` input parameter (the spec only shows `@` for out-parameters); `attr: ptr` passed `nullptr` for the NULL-able `const pthread_mutexattr_t *`; `record pthread_mutex_t` with no `tag` for a typedef'd anonymous struct, and `__sig` / `__opaque` being accepted as identifiers; `long` as `i64`; `int` results as `-> i64`; no `link` on `extern "app.h"`; `sun_len: 0`; that `@` passes the cell's own address, so the mutex is not copied to a temporary between calls; multi-line record construction inside parentheses.