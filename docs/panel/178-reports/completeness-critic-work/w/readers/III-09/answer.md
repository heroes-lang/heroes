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
        print("uname failed: ", rc)
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

function main()
    addr: SockaddrUn @ SockaddrUn(sun_family: 1, sun_path: "/tmp/app.sock".to_fixed().must(), rest: zero)
    fd = app_connect(@addr)
    if fd == -1
        print("connect failed")
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

function check(rc: i32, what: str)
    if rc != 0
        print(what, " failed: ", rc)
        exit(code: 1)

function main()
    m: pthread_mutex_t @ pthread_mutex_t(rest: zero)
    rc_init = pthread_mutex_init(@m, nullptr)
    check(rc_init, "pthread_mutex_init")
    rc_lock = pthread_mutex_lock(@m)
    check(rc_lock, "pthread_mutex_lock")
    rc_unlock = pthread_mutex_unlock(@m)
    check(rc_unlock, "pthread_mutex_unlock")
    print("locked and unlocked")
```
MUTEX: no. The mutex starts as all zeros (`pthread_mutex_t(rest: zero)`), but `pthread_mutex_init` runs on it before it is locked or unlocked.
GUESSED: `u8[N]` for every C `char` array, since the spec never says what sign plain `char` takes (`sysname`...`machine: u8[256]`, `sun_path: u8[104]`, `__opaque: u8[56]`); `Utsname(rest: zero)` and `pthread_mutex_t(rest: zero)` with no named field before `rest: zero`; `@addr: SockaddrUn` for a `const struct sockaddr_un *` that is input only, because the spec only names `@` for out-parameters; `"/tmp/app.sock".to_fixed().must()` inline as a field argument, assuming the expected `u8[104]` type still reaches through `.must()`; `record pthread_mutex_t` taking the typedef name with no `tag`, for a struct that has no name after `struct`; `attr: ptr` passed `nullptr` for the NULL attr, with no `pthread_mutexattr_t` record declared; `__sig: i64` for `long` (LP64 assumed); no `link` on the pthread group.