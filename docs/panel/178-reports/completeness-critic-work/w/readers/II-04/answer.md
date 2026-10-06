TASK 1
```
extern "sys/utsname.h"
    record Utsname tag utsname zero
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
        print(f"uname failed: {rc}")
    else
        match u.sysname.validated_bytes()
            .ok sys => print(sys)
            .err e => print(e.code)
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
    path: i8[104]? = "/tmp/app.sock".to_fixed()
    addr: SockaddrUn @ SockaddrUn(sun_len: 106, sun_family: 1, sun_path: path.must())
    fd = app_connect(@addr)
    if fd < 0
        print("connect to /tmp/app.sock failed")
    else
        print(f"connected, descriptor {fd}")
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
    opaque: i8[56] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    m: pthread_mutex_t @ pthread_mutex_t(__sig: 0, __opaque: opaque)
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
MUTEX: no. The program writes an all-zero placeholder only because a record has to be constructed before it can be passed as `@m`, and it goes through pthread_mutex_init before any lock or unlock. The record is deliberately not marked `zero`, so `rest: zero` is not used on it.
GUESSED: C `char` written as `i8` in every array (`i8[256]`, `i8[104]`, `i8[56]`), since the spec says "header's own width and sign" but never says how to spell `char`; where the `zero` claim goes (`record Utsname tag utsname zero`), because the grammar has no slot for it; `Utsname(rest: zero)` with no named field at all; `@addr: SockaddrUn` for a `const struct sockaddr_un *` input, since `@` is the only way the spec shows to pass a record by pointer; `sun_len: 106` (the struct's size), because the header gives no value for it; how `to_fixed()` learns its target, which I pinned with the annotated `i8[104]?` binding; `record pthread_mutex_t` with the typedef's own name and no `tag` for an anonymous typedef struct; `__sig` and `__opaque` being legal identifiers; `attr: ptr` with `nullptr` standing for the NULL `const pthread_mutexattr_t *`; `-> i32` results; no `link` for libc or pthread; whether copy-in/copy-out of `@m` moves the mutex's address between the init, lock and unlock calls, which the spec does not answer for extern calls.