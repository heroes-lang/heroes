TASK 1
```
extern "sys/utsname.h"
    record Utsname tag utsname zero
        sysname: i8[256]
        nodename: i8[256]
        release: i8[256]
        version: i8[256]
        machine: i8[256]
    function uname(@name: Utsname) -> i64

function main()
    u: Utsname @ Utsname(rest: zero)
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
        sun_path: i8[104]
    function app_connect(@addr: SockaddrUn) -> i64

function main()
    path: i8[104] = "/tmp/app.sock".to_fixed().must()
    addr: SockaddrUn @ SockaddrUn(sun_len: 0, sun_family: 1, sun_path: path)
    fd = app_connect(@addr)
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
    opaque: i8[56] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    m: pthread_mutex_t @ pthread_mutex_t(__sig: 0, __opaque: opaque)
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
MUTEX: no. The cell has to start with some value, so it starts as an all-zero literal. pthread_mutex_init runs on it before any lock. I did not mark the record `zero`, because an all-zero pthread_mutex_t is not a valid mutex on every platform.
GUESSED: C `char` as `i8[N]` rather than `u8[N]` (the spec does not say what sign a plain char gets). Where `zero` goes in `record Utsname tag utsname zero` (the prose says "after its name" and the grammar leaves it out). That `Utsname(rest: zero)` may name no fields at all. That `@addr: SockaddrUn` works for a `const struct sockaddr_un *` (the spec gives `@` only for out-parameters). `sun_len: 0`. That `i8[104]` and `i8[56]` work as ordinary binding types, and that `to_fixed()` gets its size through `.must()`. The record name `pthread_mutex_t` with no `tag` for an anonymous typedef struct. `__sig` and `__opaque` as identifiers, and `long` as `i64`. `attr: ptr` with `nullptr` for a nullable `const pthread_mutexattr_t *`. `-> i64` for the int results. No `link` for pthread.