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
    u: Utsname @ Utsname(rest: zero)
    rc = uname(@u)
    if rc != 0
        print("uname failed: ", rc)
        exit(code: 1)
    match u.sysname.validated_bytes()
        .ok name => print(name)
        .err e => print("sysname unreadable: ", e.code, " ", e.msg)
```
TASK 2
```
extern "app.h"
    record SockaddrUn tag sockaddr_un
        sun_len: u8
        sun_family: u8
        sun_path: i8[104]
    function app_connect(@addr: SockaddrUn) -> i32

function unix_address(path: str) -> SockaddrUn?
    return ok(SockaddrUn(sun_family: 1, sun_path: path.to_fixed()?, rest: zero))

function main()
    match unix_address("/tmp/app.sock")
        .ok a =>
            addr: SockaddrUn @ a
            fd = app_connect(@addr)
            if fd == -1
                print("app_connect failed")
                exit(code: 1)
            print("connected: descriptor ", fd)
        .err e => print("bad socket path: ", e.code, " ", e.msg)
```
TASK 3
```
extern "pthread.h"
    record pthread_mutex_t partial
    function pthread_mutex_init(@m: pthread_mutex_t, attr: ptr) -> i32
    function pthread_mutex_lock(@m: pthread_mutex_t) -> i32
    function pthread_mutex_unlock(@m: pthread_mutex_t) -> i32

function main()
    m: pthread_mutex_t @ pthread_mutex_t(rest: zero)
    init_rc = pthread_mutex_init(@m, nullptr)
    if init_rc != 0
        print("pthread_mutex_init failed: ", init_rc)
        exit(code: 1)
    lock_rc = pthread_mutex_lock(@m)
    if lock_rc != 0
        print("pthread_mutex_lock failed: ", lock_rc)
        exit(code: 1)
    print("locked")
    unlock_rc = pthread_mutex_unlock(@m)
    if unlock_rc != 0
        print("pthread_mutex_unlock failed: ", unlock_rc)
        exit(code: 1)
    print("unlocked")
```
MUTEX: no. The cell starts as `pthread_mutex_t(rest: zero)`, but `pthread_mutex_init(@m, nullptr)` always runs on it before any lock or unlock.
GUESSED: `i8[256]`/`i8[104]` for C `char` arrays (I picked signed char over `u8`); whether `Utsname(rest: zero)` and `pthread_mutex_t(rest: zero)` are legal when they name no field at all; `@addr: SockaddrUn` for a `const struct sockaddr_un *` input (the spec only shows `@` for out-parameters); `@name`/`@m` as the way to pass a pointer to a record; `record pthread_mutex_t partial` with no tag and no fields for an anonymous typedef struct (the other option was listing `__sig: i64` and `__opaque: i8[56]`, but I wasn't sure a leading `__` is a legal ident); `attr: ptr` plus `nullptr` for the NULL-able `const pthread_mutexattr_t *`; whether `path.to_fixed()?` inside the constructor takes `i8[104]` from its position; `-> i32` rather than `-> i64` results; no `link` on `app.h` or `pthread.h`; leaving `sun_len` zero through `rest: zero`; and whether copy-in/copy-out of `@m` across the three calls moves the mutex, which POSIX does not allow.