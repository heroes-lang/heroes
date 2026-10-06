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
        .ok name => print(name)
        .err e => print(f"could not read sysname: {e.code}")
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
    path = "/tmp/app.sock"
    if path.len() >= 104
        print("socket path too long")
        exit(code: 1)
    addr: SockaddrUn @ SockaddrUn(sun_family: 1, rest: zero)
    for i in range(from: 0, to: path.len())
        addr.sun_path[i] @ path[i].to_i8().must()
    fd = app_connect(@addr)
    if fd == -1
        print("connect failed")
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
    m: pthread_mutex_t @ pthread_mutex_t(rest: zero)
    rc_init = pthread_mutex_init(@m, nullptr)
    if rc_init != 0
        print(f"pthread_mutex_init failed: {rc_init}")
        exit(code: 1)
    rc_lock = pthread_mutex_lock(@m)
    if rc_lock != 0
        print(f"pthread_mutex_lock failed: {rc_lock}")
        exit(code: 1)
    rc_unlock = pthread_mutex_unlock(@m)
    if rc_unlock != 0
        print(f"pthread_mutex_unlock failed: {rc_unlock}")
        exit(code: 1)
    print("locked and unlocked")
```
MUTEX: no. The cell starts out as zeros from `rest: zero`, because every binding has to be initialised, but `pthread_mutex_init` runs on it before any lock or unlock.
GUESSED: C `char` written as `i8[N]` (signed, as on macOS; the spec never says what sign `char` gets); `Utsname(rest: zero)` naming no fields at all; a `struct X *` parameter written as an `@` record parameter, both for the out-parameter in `uname` and for the const input in `app_connect(@addr: SockaddrUn)`, since the spec gives no other way to pass a record by pointer; writing into a fixed-array field one element at a time with `addr.sun_path[i] @ path[i].to_i8().must()`; `record pthread_mutex_t` with no `tag` for a typedef of an anonymous struct, and `__sig` / `__opaque` accepted as identifiers; the NULL-able `const pthread_mutexattr_t *` written as `attr: ptr` and passed `nullptr`; `-> i64` results against C `int`; no `link` for pthread; whether copy-in/copy-out through `@m` keeps the mutex at one stable address across the three calls.