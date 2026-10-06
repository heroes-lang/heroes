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
    u: Utsname @ Utsname(rest: zero)
    rc = uname(@u)
    if rc != 0
        print("uname failed")
        exit(code: 1)
    match u.sysname.validated_bytes()
        .ok s => print(s)
        .err e => print(e.code)
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
    path: i8[104] = "/tmp/app.sock".to_fixed().must()
    addr: SockaddrUn @ SockaddrUn(sun_len: 106, sun_family: 1, sun_path: path)
    fd = app_connect(@addr)
    if fd == -1
        print("connect failed")
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
    m: pthread_mutex_t @ pthread_mutex_t(rest: zero)
    init_rc = pthread_mutex_init(@m, nullptr)
    if init_rc != 0
        print("init failed: ", init_rc)
        exit(code: 1)
    lock_rc = pthread_mutex_lock(@m)
    if lock_rc != 0
        print("lock failed: ", lock_rc)
        exit(code: 1)
    unlock_rc = pthread_mutex_unlock(@m)
    if unlock_rc != 0
        print("unlock failed: ", unlock_rc)
        exit(code: 1)
    print("locked and unlocked")
```
MUTEX: no. It zero-fills the cell only because every binding has to start with a value, then calls pthread_mutex_init on it before any lock or unlock.
GUESSED: C `char` arrays declared as `i8[N]` rather than `u8[N]` (the spec says "same width and sign" but never says how plain `char` maps); `Utsname(rest: zero)` and `pthread_mutex_t(rest: zero)` with no fields named; the `const struct sockaddr_un *` parameter written as `@addr: SockaddrUn` (the spec only shows `@` for out-parameters, so a read-only pointer to a record is a guess); `.must()` carrying the `i8[104]` type from the annotation into `to_fixed()`; `sun_len: 106` as the struct size; `link "app"` for app.h; `attr: ptr` with `nullptr` for the nullable `const pthread_mutexattr_t *`; the record named `pthread_mutex_t` with no `tag` because the header's struct is an anonymous typedef; `__sig`/`__opaque` accepted as identifiers; `long` as `i64`.