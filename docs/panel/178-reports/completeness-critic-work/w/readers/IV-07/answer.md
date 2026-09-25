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
    u: Utsname @ Utsname(sysname: [0; 256], nodename: [0; 256], release: [0; 256], version: [0; 256], machine: [0; 256])
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
        sun_path: i8[104]
    function app_connect(@addr: SockaddrUn) -> i64

function main()
    path = "/tmp/app.sock"
    if path.len() >= 104
        print("path too long: ", path)
        exit(code: 1)
    addr: SockaddrUn @ SockaddrUn(sun_len: 106, sun_family: 1, sun_path: [0; 104])
    for i in range(from: 0, to: path.len())
        addr.sun_path[i] @ to_i8(path[i]).must()
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
    m: pthread_mutex_t @ pthread_mutex_t(__sig: 0, __opaque: [0; 56])
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
MUTEX: no. The program zero-fills the storage only because every binding has to start with a value, and it calls pthread_mutex_init on it before any lock or unlock.
GUESSED: `i8` for plain C `char` in every `char[N]` field (the spec gives no rule for char's sign, and `u8` was the other candidate); `@addr: SockaddrUn` for the read-only `const struct sockaddr_un *` (the spec shows no form for passing a record by pointer other than `@`), which also forces `addr` to be an `@` cell; `attr: ptr` with `nullptr` for the NULL `const pthread_mutexattr_t *` (relying on "what a ptr points at" being exempt); `record pthread_mutex_t` with no `tag`, for a typedef of an anonymous struct; `__sig` and `__opaque` being accepted as identifiers; `to_i8(path[i]).must()` to copy str bytes into a char field; `sun_len: 106` (the header gives no value); whether `@m` hands C the cell's own address or a copied temporary, since copy-in/copy-out would move a live mutex; `-> i64` for C `int` results (going by "a result may be wider").