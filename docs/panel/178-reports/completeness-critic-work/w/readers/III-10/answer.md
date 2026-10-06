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
        print("uname failed: ", rc)
        exit(code: 1)
    match u.sysname.validated_bytes()
        .ok name => print(name)
        .err e   => print(e.code)
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
    path: i8[104] = "/tmp/app.sock".to_fixed().must()
    addr: SockaddrUn @ SockaddrUn(sun_family: 1, sun_path: path, rest: zero)
    fd = app_connect(@addr)
    if fd == -1
        print("app_connect failed")
        exit(code: 1)
    print("connected, descriptor ", fd)
```
TASK 3
```
extern "pthread.h"
    record pthread_mutex_t partial
    function pthread_mutex_init(@m: pthread_mutex_t, attr: ptr) -> i64
    function pthread_mutex_lock(@m: pthread_mutex_t) -> i64
    function pthread_mutex_unlock(@m: pthread_mutex_t) -> i64

function main()
    m: pthread_mutex_t @ pthread_mutex_t(rest: zero)
    rc = pthread_mutex_init(@m, nullptr)
    if rc != 0
        print("pthread_mutex_init failed: ", rc)
        exit(code: 1)
    locked = pthread_mutex_lock(@m)
    if locked != 0
        print("pthread_mutex_lock failed: ", locked)
        exit(code: 1)
    unlocked = pthread_mutex_unlock(@m)
    if unlocked != 0
        print("pthread_mutex_unlock failed: ", unlocked)
        exit(code: 1)
    print("locked and unlocked")
```
MUTEX: no. The program fills the storage with zeros, because every binding must start with a value and `rest: zero` is the only way the spec gives to make one, but it calls pthread_mutex_init on that storage before any lock or unlock.
GUESSED: C `char` declared as `i8[N]` (and not `u8[N]`); a constructor that names no field and has only `rest: zero` (`Utsname(rest: zero)`, `pthread_mutex_t(rest: zero)`); a `const struct sockaddr_un *` in-parameter written as `@addr: SockaddrUn`, because `@` is the only pointer-to-record form the spec shows; `path: i8[104] = "/tmp/app.sock".to_fixed().must()` getting its target type from the annotation; leaving `sun_len` at zero; no `link` for app.h or pthread.h; a typedef'd anonymous struct named with its typedef name as the record name (`record pthread_mutex_t`) and no `tag`; `partial` with no fields listed, to avoid writing `__sig`/`__opaque` as identifiers; `attr: ptr` given `nullptr` in place of declaring pthread_mutexattr_t; every `int` result declared `-> i64`.