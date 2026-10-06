TASK 1
```
extern "sys/utsname.h"
    record Utsname tag utsname
        sysname: u8[256]
        nodename: u8[256]
        release: u8[256]
        version: u8[256]
        machine: u8[256]
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
extern "app.h"
    record SockaddrUn tag sockaddr_un
        sun_len: u8
        sun_family: u8
        sun_path: u8[104]
    function app_connect(@addr: SockaddrUn) -> i64

function main()
    path = "/tmp/app.sock"
    addr: SockaddrUn @ SockaddrUn(sun_family: 1, rest: zero)
    for i in range(from: 0, to: path.len())
        addr.sun_path[i] @ path[i]
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
    function pthread_mutex_init(@m: pthread_mutex_t, attr: ptr) -> i64
    function pthread_mutex_lock(@m: pthread_mutex_t) -> i64
    function pthread_mutex_unlock(@m: pthread_mutex_t) -> i64

function check(what: str, rc: i64)
    if rc != 0
        print(f"{what} failed: {rc}")
        exit(code: 1)

function main()
    m: pthread_mutex_t @ pthread_mutex_t(rest: zero)
    check("init", pthread_mutex_init(@m, nullptr))
    check("lock", pthread_mutex_lock(@m))
    check("unlock", pthread_mutex_unlock(@m))
    print("locked and unlocked")
```
MUTEX: no. The cell starts as `pthread_mutex_t(rest: zero)`, but only as storage for init's out-parameter. `pthread_mutex_init` runs on it before any lock or unlock.
GUESSED: C `char` arrays written as `u8[N]` rather than `i8[N]` (tasks 1-3). `Utsname(rest: zero)` and `pthread_mutex_t(rest: zero)` naming no field at all before `rest: zero`. `@name: Utsname` for a `struct *` out-parameter. `@addr: SockaddrUn` for a `const struct *` input parameter; the spec says nothing about passing a record by pointer as input. `addr.sun_path[i] @ path[i]`, indexing and mutating one element of a fixed-array field. `record pthread_mutex_t` with no `tag`, because the header's struct is an anonymous typedef. `__sig` and `__opaque` accepted as identifiers. `attr: ptr` with `nullptr` standing for the NULL `const pthread_mutexattr_t *`, and no `pthread_mutexattr_t` record declared. Results `-> i64` against C `int`. No `link` for pthread.