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
    u: Utsname @ Utsname(sysname: [0; 256], nodename: [0; 256], release: [0; 256],
        version: [0; 256], machine: [0; 256])
    rc = uname(@u)
    if rc != 0
        print(f"uname failed: {rc}")
        exit(code: 1)
    match u.sysname.validated_bytes()
        .ok name => print(name)
        .err e   => print(f"sysname unreadable: {e.code}")
```
TASK 2
```
extern "app.h" link "app"
    record SockaddrUn tag sockaddr_un
        sun_len: u8
        sun_family: u8
        sun_path: i8[104]
    function app_connect(@addr: SockaddrUn) -> i64

function main()
    path = "/tmp/app.sock"
    a: SockaddrUn @ SockaddrUn(sun_len: 106, sun_family: 1, sun_path: [0; 104])
    for i in range(from: 0, to: path.len())
        a.sun_path[i] @ to_i8(path[i]).must()
    fd = app_connect(@a)
    if fd == -1
        print("app_connect failed")
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
    init_rc = pthread_mutex_init(@m, nullptr)
    if init_rc != 0
        print(f"pthread_mutex_init failed: {init_rc}")
        exit(code: 1)
    lock_rc = pthread_mutex_lock(@m)
    if lock_rc != 0
        print(f"pthread_mutex_lock failed: {lock_rc}")
        exit(code: 1)
    print("locked")
    unlock_rc = pthread_mutex_unlock(@m)
    if unlock_rc != 0
        print(f"pthread_mutex_unlock failed: {unlock_rc}")
        exit(code: 1)
    print("unlocked")
```
MUTEX: no. The cell starts as zeros (`__sig: 0, __opaque: [0; 56]`) only because every field has to be written, and pthread_mutex_init runs on it before any lock or unlock.
GUESSED: `char` as `i8` rather than `u8` in all three records, and whether `validated_bytes()` accepts an `i8[256]` field; passing a `const struct *` input as an `@` parameter (`@addr: SockaddrUn`, `@m: pthread_mutex_t`); `link "app"` for app.h, and none for libc/pthread; `sun_len: 106`; `to_i8(path[i]).must()` to copy the path bytes; naming the anonymous typedef struct `record pthread_mutex_t` with no `tag`, and `__sig`/`__opaque` being legal field names; `attr: ptr` taking `nullptr` for the NULL attribute; the `-> i64` results against C's `int`.