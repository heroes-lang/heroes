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
    u: Utsname @ Utsname(sysname: [0; 256], nodename: [0; 256], release: [0; 256], version: [0; 256], machine: [0; 256])
    if uname(@u) != 0
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
        sun_path: i8[104]
    function app_connect(@addr: SockaddrUn) -> i32

function main()
    path = "/tmp/app.sock"
    assert path.len() < 104
    addr: SockaddrUn @ SockaddrUn(sun_len: 0, sun_family: 1, sun_path: [0; 104])
    for i in range(from: 0, to: path.len())
        addr.sun_path[i] @ to_i8(path[i]).must()
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
        __opaque: i8[56]
    function pthread_mutex_init(@m: pthread_mutex_t, attr: ptr) -> i32
    function pthread_mutex_lock(@m: pthread_mutex_t) -> i32
    function pthread_mutex_unlock(@m: pthread_mutex_t) -> i32

function main()
    m: pthread_mutex_t @ pthread_mutex_t(__sig: 0, __opaque: [0; 56])
    if pthread_mutex_init(@m, nullptr) != 0
        print("init failed")
        exit(code: 1)
    if pthread_mutex_lock(@m) != 0
        print("lock failed")
        exit(code: 1)
    if pthread_mutex_unlock(@m) != 0
        print("unlock failed")
        exit(code: 1)
    print("locked and unlocked")
```
MUTEX: no. The program builds the cell from zeros, because every binding must be initialised before `@m` can be passed, but it calls pthread_mutex_init on it before any lock or unlock.
GUESSED: `char` as `i8` rather than `u8` (all three tasks) and `validated_bytes()` accepting an `i8[256]` field; `-> i32` for the int results; a `struct *` in-parameter (including `const struct sockaddr_un *`) written as `@addr: SockaddrUn`, with no `lent`; `record pthread_mutex_t` with no `tag` for a typedef'd anonymous struct, and `__sig` / `__opaque` being legal identifiers; `long` as `i64`; `attr: ptr` with `nullptr` for the NULL `const pthread_mutexattr_t *`; no `link "pthread"`; `to_i8(path[i]).must()` to turn a string byte into a `char` element, and indexing the fixed array with an `i64` from `range`; `sun_len: 0`; `fd == -1` with the literal taking `i32`; `record Utsname tag utsname` / `record SockaddrUn tag sockaddr_un` as the naming form.