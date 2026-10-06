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
    u: Utsname @ Utsname(rest: zero)
    rc = uname(@u)
    if rc != 0
        print(f"uname failed: {rc}")
        exit(code: 1)
    match u.sysname.validated_bytes()
        .ok s  => print(s)
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

function main()
    path = "/tmp/app.sock"
    addr: SockaddrUn @ SockaddrUn(sun_family: 1, rest: zero)
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
    m: pthread_mutex_t @ pthread_mutex_t(rest: zero)
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
MUTEX: no. The storage starts as zeros (`rest: zero`), but `pthread_mutex_init` runs on it before any lock or unlock.
GUESSED: `u8[N]` for every C `char[N]` field (the spec never says what sign plain `char` takes); `Utsname(rest: zero)` and `pthread_mutex_t(rest: zero)` naming no fields at all; `@addr: SockaddrUn` for a `const struct sockaddr_un *` input (the spec only names `@` for out-parameters); element writes into a fixed-array field (`addr.sun_path[i] @ path[i]`); leaving `sun_len` zero; `attr: ptr` given `nullptr` for the nullable `const pthread_mutexattr_t *`; the record name `pthread_mutex_t` (the typedef name, no `tag`, lowercase) with fields spelled `__sig` and `__opaque`; `i64` for `long`; `-> i32` for every `int` result.