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
    u: Utsname @ Utsname(
        sysname: [0; 256],
        nodename: [0; 256],
        release: [0; 256],
        version: [0; 256],
        machine: [0; 256]
    )
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
    function app_connect(@addr: SockaddrUn) -> i32

function main()
    path = "/tmp/app.sock"
    a: SockaddrUn @ SockaddrUn(sun_len: 106, sun_family: 1, sun_path: [0; 104])
    for i in range(from: 0, to: path.len())
        a.sun_path[i] @ path[i].to_i8().must()
    fd = app_connect(@a)
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
    function pthread_mutex_init(@m: pthread_mutex_t, attr: ptr) -> i32
    function pthread_mutex_lock(@m: pthread_mutex_t) -> i32
    function pthread_mutex_unlock(@m: pthread_mutex_t) -> i32

function check(rc: i32, what: str)
    if rc != 0
        print(what, " failed: ", rc)
        exit(code: 1)

function main()
    m: pthread_mutex_t @ pthread_mutex_t(__sig: 0, __opaque: [0; 56])
    check(pthread_mutex_init(@m, nullptr), "pthread_mutex_init")
    check(pthread_mutex_lock(@m), "pthread_mutex_lock")
    check(pthread_mutex_unlock(@m), "pthread_mutex_unlock")
    print("locked and unlocked")
```
MUTEX: no. The program fills the cell with zeros only because every binding has to start with a value, and it calls pthread_mutex_init on that cell before any lock or unlock.
GUESSED: `char` declared as `i8` in all three tasks (the header's sign for char is not given); `validated_bytes()` accepting an `i8[256]` field as "a field of bytes"; `@addr: SockaddrUn` for a `const struct sockaddr_un *` input (the spec only names `@` for out-parameters); `link "app"` (unknown whether app_connect needs a library); `sun_len: 106` (sizeof the struct; the header does not say what it should be); `path[i].to_i8().must()` converting u8 to i8, and indexing a fixed array with an i64 from `range`; `record pthread_mutex_t` with no `tag`, named straight after the typedef of an anonymous struct, and `__sig`/`__opaque` accepted as identifiers; `long` as `i64`; `attr: ptr` passed `nullptr` for a `const pthread_mutexattr_t *`; no `link` for pthread; whether copy-in/copy-out through `@m` is acceptable for a pthread mutex (the spec says `@` copies, and POSIX does not allow a mutex to be copied).