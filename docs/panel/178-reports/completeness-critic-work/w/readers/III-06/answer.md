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
        .err e => print("sysname unreadable: ", e.code)
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

function main()
    m: pthread_mutex_t @ pthread_mutex_t(rest: zero)
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
MUTEX: no. The program fills the cell with zeros only because every binding has to start with a value, and pthread_mutex_init runs on it before any lock or unlock.
GUESSED: I guessed these lines. (1) Every C `char` field written as `u8[N]` rather than `i8[N]` (sysname..machine, sun_path, __opaque). (2) `Utsname(rest: zero)` and `pthread_mutex_t(rest: zero)`, which name no field at all before `rest: zero`. (3) `@addr: SockaddrUn` and `app_connect(@addr)` for a `const struct sockaddr_un *` that is an input and not an out-parameter; the spec shows no way to pass a record by pointer except `@`. (4) `sun_path: "/tmp/app.sock".to_fixed().must()`, assuming the fixed-array context carries through `.must()`. (5) Leaving `sun_len` at zero through `rest: zero`. (6) `record pthread_mutex_t` using the typedef name with no `tag`, and `__sig` / `__opaque` as legal field identifiers. (7) `attr: ptr` with `nullptr` passed for the NULL `const pthread_mutexattr_t *`, which means pthread_mutexattr_t is never declared. (8) No `link "pthread"` on the pthread group. (9) `-> i32` for the C `int` results.