TASK 1
```
extern "sys/utsname.h"
    record Utsname tag utsname partial
        sysname: i8[256]
    function uname(@name: Utsname) -> i64

function main()
    u: Utsname @ Utsname(sysname: [
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    ])
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
extern "app.h" link "app"
    record SockaddrUn tag sockaddr_un
        sun_len: u8
        sun_family: u8
        sun_path: i8[104]
    function app_connect(@addr: SockaddrUn) -> i64

constant AF_UNIX: u8
    1

constant SOCKADDR_UN_SIZE: u8
    106

function main()
    addr: SockaddrUn @ SockaddrUn(sun_len: SOCKADDR_UN_SIZE, sun_family: AF_UNIX, sun_path: [
        '/', 't', 'm', 'p', '/', 'a', 'p', 'p', '.', 's', 'o', 'c', 'k', 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
    ])
    fd = app_connect(@addr)
    if fd == -1
        print("could not connect to /tmp/app.sock")
        exit(code: 1)
    print("connected, descriptor ", fd)
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
    m: pthread_mutex_t @ pthread_mutex_t(__sig: 0, __opaque: [
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
    ])
    rc_init = pthread_mutex_init(@m, nullptr)
    if rc_init != 0
        print("pthread_mutex_init failed: ", rc_init)
        exit(code: 1)
    rc_lock = pthread_mutex_lock(@m)
    if rc_lock != 0
        print("pthread_mutex_lock failed: ", rc_lock)
        exit(code: 1)
    print("locked")
    rc_unlock = pthread_mutex_unlock(@m)
    if rc_unlock != 0
        print("pthread_mutex_unlock failed: ", rc_unlock)
        exit(code: 1)
    print("unlocked")
```
MUTEX: no. The program starts the cell from zeros only because every binding has to be initialised, and pthread_mutex_init runs on it before any lock or unlock.
GUESSED: C `char` declared as `i8` rather than `u8` (the spec gives no rule for char's sign); `partial` on Utsname so only `sysname` is declared; `record pthread_mutex_t` with no `tag`, for a typedef of an anonymous struct; `long __sig` as `i64`, and `__sig`/`__opaque` accepted as identifiers; `@addr: SockaddrUn` for a `const struct sockaddr_un *` input (the spec only names `@` for out-parameters); `attr: ptr` plus `nullptr` for the NULL-able `const pthread_mutexattr_t *`; `link "app"` for app_connect's library; `sun_len` set to 106 (sizeof the struct); AF_UNIX written as my own `constant AF_UNIX: u8` with body `1`; long array literals written as comma-separated rows split by newlines, with no trailing comma and a newline right after `[`; `validated_bytes()` on an `i8[256]` field returning `str?`.