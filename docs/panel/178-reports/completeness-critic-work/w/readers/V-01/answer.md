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
    u: Utsname @ Utsname(rest: zero)
    if uname(@u) != 0
        print("uname failed")
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
    function app_connect(@addr: SockaddrUn) -> i32

function main()
    addr: SockaddrUn @ SockaddrUn(sun_family: 1, sun_path: ['/', 't', 'm', 'p', '/', 'a', 'p', 'p', '.', 's', 'o', 'c', 'k'
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], rest: zero)
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
    function pthread_mutex_init(@m: pthread_mutex_t, attr: ptr) -> i32
    function pthread_mutex_lock(@m: pthread_mutex_t) -> i32
    function pthread_mutex_unlock(@m: pthread_mutex_t) -> i32

function main()
    m: pthread_mutex_t @ pthread_mutex_t(rest: zero)
    if pthread_mutex_init(@m, nullptr) != 0
        print("pthread_mutex_init failed")
        exit(code: 1)
    if pthread_mutex_lock(@m) != 0
        print("pthread_mutex_lock failed")
        exit(code: 1)
    print("locked")
    if pthread_mutex_unlock(@m) != 0
        print("pthread_mutex_unlock failed")
        exit(code: 1)
    print("unlocked")
```
MUTEX: no. The cell starts as zeros (`pthread_mutex_t(rest: zero)`), but pthread_mutex_init runs on it before any lock or unlock.
GUESSED: C `char` as `i8` (the spec never says which sign `char` gets), and that `validated_bytes()` accepts an `i8[256]` field as "a field of bytes"; `Utsname(rest: zero)` and `pthread_mutex_t(rest: zero)` naming no fields at all (and `rest: zero` being allowed on a `partial` record); `@addr: SockaddrUn` for a `const struct sockaddr_un *` input, since the spec only describes `@` for out-parameters; leaving `link` off `extern "app.h"` because no library is named; `record pthread_mutex_t partial` with no fields and no `tag`, named after the typedef because the struct has no tag, which also sidesteps whether `__sig`/`__opaque` are legal identifiers; `attr: ptr` plus passing `nullptr` for the nullable `const pthread_mutexattr_t *`; `i32` results; and splitting the 104-element array literal across lines with commas inside each line and newlines between lines.