TASK 1
```
extern "sys/utsname.h"
    record Utsname tag utsname
        sysname: i8[256]
        nodename: i8[256]
        release: i8[256]
        version: i8[256]
        machine: i8[256]
    function uname(@name: Utsname) -> i32

function main()
    u: Utsname @ Utsname(
        sysname: [0; 256],
        nodename: [0; 256],
        release: [0; 256],
        version: [0; 256],
        machine: [0; 256])
    rc = uname(@u)
    if rc != 0
        print("uname failed: ", rc)
        exit(code: 1)
    match u.sysname.validated_bytes()
        .ok sysname => print(sysname)
        .err e => print("sysname is not text: ", e.code)
```
TASK 2
```
extern "app.h"
    record SockaddrUn tag sockaddr_un
        sun_len: u8
        sun_family: u8
        sun_path: i8[104]
    function app_connect(@addr: SockaddrUn) -> i32

function main()
    addr: SockaddrUn @ SockaddrUn(sun_len: 106, sun_family: 1, sun_path: [0; 104])
    path = "/tmp/app.sock"
    for i in range(from: 0, to: path.len())
        addr.sun_path[i] @ path[i].to_i8().must()
    fd = app_connect(@addr)
    if fd == -1
        print("app_connect failed")
        exit(code: 1)
    print("connected, descriptor ", fd)
```
TASK 3
```
extern "pthread.h"
    record pthread_mutex_t
        __sig: i64
        __opaque: i8[56]
    function pthread_mutex_init(@m: pthread_mutex_t, attr: ptr) -> i32
    function pthread_mutex_lock(@m: pthread_mutex_t) -> i32
    function pthread_mutex_unlock(@m: pthread_mutex_t) -> i32

function main()
    m: pthread_mutex_t @ pthread_mutex_t(__sig: 0, __opaque: [0; 56])
    init_rc = pthread_mutex_init(@m, nullptr)
    if init_rc != 0
        print("pthread_mutex_init failed: ", init_rc)
        exit(code: 1)
    lock_rc = pthread_mutex_lock(@m)
    if lock_rc != 0
        print("pthread_mutex_lock failed: ", lock_rc)
        exit(code: 1)
    unlock_rc = pthread_mutex_unlock(@m)
    if unlock_rc != 0
        print("pthread_mutex_unlock failed: ", unlock_rc)
        exit(code: 1)
    print("locked and unlocked")
```
MUTEX: no. The program fills the record with zeros only as the starting value of the `@` cell, then calls pthread_mutex_init on it before any lock or unlock.
GUESSED: C `char` declared as `i8` (the spec gives no sign for plain `char`) in `i8[256]`/`i8[104]`/`i8[56]`, and whether `validated_bytes()` accepts an `i8` field; `@addr: SockaddrUn` for an input `const struct sockaddr_un *` (the spec only covers `@` for out-parameters); `sun_len: 106` (sizeof the struct; the header leaves it unspecified); `path[i].to_i8().must()` to convert a `u8` byte into the `i8` element; `record pthread_mutex_t` named after the typedef with no `tag`, since the struct has no tag; `__sig`/`__opaque` accepted as identifiers; `long` as `i64`; `attr: ptr` given `nullptr` in place of declaring `pthread_mutexattr_t`; results declared `i32`; no `link` needed for pthread or uname.