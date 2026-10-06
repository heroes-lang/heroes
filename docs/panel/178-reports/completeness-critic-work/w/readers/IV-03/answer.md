TASK 1
```
extern "sys/utsname.h"
    record Utsname tag utsname
        sysname: u8[256]
        nodename: u8[256]
        release: u8[256]
        version: u8[256]
        machine: u8[256]
    function uname(@name: Utsname) -> i64

function main()
    u: Utsname @ Utsname(sysname: [0; 256], nodename: [0; 256], release: [0; 256], version: [0; 256], machine: [0; 256])
    rc = uname(@u)
    if rc != 0
        print(f"uname failed: {rc}")
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
    function app_connect(@addr: SockaddrUn) -> i64

function main()
    path = "/tmp/app.sock"
    assert path.len() < 104
    addr: SockaddrUn @ SockaddrUn(sun_len: 0, sun_family: 1, sun_path: [0; 104])
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
    function pthread_mutex_init(@m: pthread_mutex_t, attr: ptr) -> i64
    function pthread_mutex_lock(@m: pthread_mutex_t) -> i64
    function pthread_mutex_unlock(@m: pthread_mutex_t) -> i64

function main()
    m: pthread_mutex_t @ pthread_mutex_t(__sig: 0, __opaque: [0; 56])
    rc = pthread_mutex_init(@m, nullptr)
    if rc != 0
        print(f"pthread_mutex_init failed: {rc}")
        exit(code: 1)
    locked = pthread_mutex_lock(@m)
    if locked != 0
        print(f"pthread_mutex_lock failed: {locked}")
        exit(code: 1)
    unlocked = pthread_mutex_unlock(@m)
    if unlocked != 0
        print(f"pthread_mutex_unlock failed: {unlocked}")
        exit(code: 1)
    print("locked and unlocked")
```
MUTEX: no. The cell starts as zeros only because every binding has to be initialised, and pthread_mutex_init(@m, nullptr) runs on it before any lock or unlock.
GUESSED: C `char` fields as `u8` (not `i8`) in all three tasks; `long __sig` as `i64`; the input `const struct sockaddr_un *` declared as `@addr: SockaddrUn`, since the spec only shows `@` for out-parameters; the anonymous typedef struct written `record pthread_mutex_t` with no `tag`; `__sig` and `__opaque` accepted as identifiers; the nullable `const pthread_mutexattr_t *attr` declared as `attr: ptr` and passed `nullptr`; `sun_len: 0`; no `link` clause on any group; `int` results declared `-> i64`.