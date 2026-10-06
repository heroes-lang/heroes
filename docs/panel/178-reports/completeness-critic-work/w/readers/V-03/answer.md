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
    path = "/tmp/app.sock"
    addr: SockaddrUn @ SockaddrUn(sun_family: 1, rest: zero)
    for i in range(from: 0, to: path.len())
        addr.sun_path[i] @ path[i]
    fd = app_connect(@addr)
    if fd == -1
        print("app_connect failed")
        exit(code: 1)
    print(f"connected, descriptor {fd}")
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
MUTEX: no. The cell starts as `pthread_mutex_t(rest: zero)` only because every binding has to be initialised, and `pthread_mutex_init(@m, nullptr)` runs on it before any lock or unlock.
GUESSED: C `char` arrays declared `u8[N]` rather than `i8[N]` (the spec says "a field of bytes" for `validated_bytes` but never says which sign `char` maps to); `Utsname(rest: zero)` with no field named before `rest`; `@name: Utsname` for `struct utsname *`; `@addr: SockaddrUn` with an `@` cell for the input-only `const struct sockaddr_un *` (the spec only names `@` for out-parameters and says nothing about a const pointer to a record); the `for` loop writing `sun_path` a byte at a time instead of a 104-element literal; leaving `sun_len` zero; `record pthread_mutex_t` with no `tag` for an anonymous typedef'd struct; `__sig` and `__opaque` accepted as identifiers; `long` as `i64`; `attr: ptr` plus `nullptr` for `const pthread_mutexattr_t *` (relying on "what a `ptr` points at" being exempt); no `link "pthread"`.