TASK 1
```
extern "sys/utsname.h"
    record Utsname tag utsname
        sysname: u8[256]
        nodename: u8[256]
        release: u8[256]
        version: u8[256]
        machine: u8[256]
    function uname(@name: Utsname) -> i32

function main()
    u: Utsname @ Utsname(sysname: [0; 256], nodename: [0; 256], release: [0; 256], version: [0; 256], machine: [0; 256])
    rc = uname(@u)
    if rc != 0
        print("uname failed")
        exit(code: 1)
    match u.sysname.validated_bytes()
        .ok name => print(name)
        .err e => print(e.code)
```
TASK 2
```
extern "app.h"
    record SockaddrUn tag sockaddr_un
        sun_len: u8
        sun_family: u8
        sun_path: u8[104]
    function app_connect(@addr: SockaddrUn) -> i32

constant AF_UNIX: u8
    1

function main()
    path = "/tmp/app.sock"
    addr: SockaddrUn @ SockaddrUn(sun_len: 106, sun_family: AF_UNIX, sun_path: [0; 104])
    for i in range(from: 0, to: path.len())
        addr.sun_path[i] @ path[i]
    fd = app_connect(@addr)
    if fd == -1
        print("app_connect failed")
        exit(code: 1)
    print(f"connected: descriptor {fd}")
```
TASK 3
```
extern "pthread.h"
    record pthread_mutex_t
        __sig: i64
        __opaque: u8[56]
    function pthread_mutex_init(@m: pthread_mutex_t, attr: ptr) -> i32
    function pthread_mutex_lock(@m: pthread_mutex_t) -> i32
    function pthread_mutex_unlock(@m: pthread_mutex_t) -> i32

function main()
    m: pthread_mutex_t @ pthread_mutex_t(__sig: 0, __opaque: [0; 56])
    if pthread_mutex_init(@m, nullptr) != 0
        print("pthread_mutex_init failed")
        exit(code: 1)
    if pthread_mutex_lock(@m) != 0
        print("pthread_mutex_lock failed")
        exit(code: 1)
    print("locked")
    if pthread_mutex_unlock(@m) != 0
        print("pthread_mutex_unlock failed")
        exit(code: 1)
    print("unlocked")
```
MUTEX: no. The cell is built from zeros only because the `@` out-parameter needs an initialised binding; pthread_mutex_init fills it and its result is checked before any lock.
GUESSED: C `char` declared as `u8` instead of `i8` (tasks 1, 2 and 3, since the spec never says char's sign); the `const struct sockaddr_un *` passed as `@addr: SockaddrUn` (the spec only names `@` for out-parameters); the NULL `const pthread_mutexattr_t *` declared as `attr: ptr` and passed `nullptr`; `__sig`/`__opaque` accepted as identifiers; the anonymous-struct typedef declared as `record pthread_mutex_t` with no `tag`; no `link` for pthread; `sun_len: 106` (the header never says what goes there); `validated_bytes()` working on a `u8[256]` field reached as `u.sysname`; results declared `-> i32`.