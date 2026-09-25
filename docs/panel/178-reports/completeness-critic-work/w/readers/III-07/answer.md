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
        .err e => print("sysname is not text: ", e.code)
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
        print("app_connect failed")
        exit(code: 1)
    print("connected, descriptor ", fd)
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
    init_rc = pthread_mutex_init(@m, nullptr)
    check(init_rc, "pthread_mutex_init")
    lock_rc = pthread_mutex_lock(@m)
    check(lock_rc, "pthread_mutex_lock")
    unlock_rc = pthread_mutex_unlock(@m)
    check(unlock_rc, "pthread_mutex_unlock")
    print("locked and unlocked")
```
MUTEX: no. The program fills the storage with zeros (`rest: zero`) only to have a value to hand over, and it calls pthread_mutex_init on it before any lock or unlock.
GUESSED: C `char` arrays declared as `u8[N]` rather than `i8[N]` (all three tasks); `Type(rest: zero)` with no field named at all (tasks 1 and 3); `@addr: SockaddrUn` for a `const struct sockaddr_un *` input parameter, since the spec only says how to pass an out-parameter (task 2); `.to_fixed()` still getting its `u8[104]` target type when `.must()` wraps it (task 2); `attr: ptr` passed `nullptr` for a `const pthread_mutexattr_t *` that may be NULL (task 3); `record pthread_mutex_t` with no `tag` for a typedef'd anonymous struct (task 3); the field names `__sig` and `__opaque` being legal identifiers (task 3); `long` as `i64` (task 3); every result declared as `i32` (all three tasks).