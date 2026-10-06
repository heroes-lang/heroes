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
    r = uname(@u)
    if r != 0
        print(f"uname failed: {r}")
        exit(code: 1)
    name = u.sysname.validated_bytes().must()
    print(name)
```
TASK 2
```
extern "app.h" link "app"
    record SockaddrUn tag sockaddr_un
        sun_len: u8
        sun_family: u8
        sun_path: u8[104]
    function app_connect(@addr: SockaddrUn) -> i32

function main()
    path: u8[104] = "/tmp/app.sock".to_fixed().must()
    addr: SockaddrUn @ SockaddrUn(sun_family: 1, sun_path: path, rest: zero)
    fd = app_connect(@addr)
    if fd == -1
        print("app_connect failed")
        exit(code: 1)
    print(f"connected, descriptor {fd}")
```
TASK 3
```
extern "pthread.h" link "pthread"
    record pthread_mutex_t
        __sig: i64
        __opaque: u8[56]
    function pthread_mutex_init(@m: pthread_mutex_t, attr: ptr) -> i32
    function pthread_mutex_lock(@m: pthread_mutex_t) -> i32
    function pthread_mutex_unlock(@m: pthread_mutex_t) -> i32

function main()
    m: pthread_mutex_t @ pthread_mutex_t(rest: zero)
    r = pthread_mutex_init(@m, nullptr)
    if r != 0
        print(f"pthread_mutex_init failed: {r}")
        exit(code: 1)
    locked = pthread_mutex_lock(@m)
    if locked != 0
        print(f"pthread_mutex_lock failed: {locked}")
        exit(code: 1)
    unlocked = pthread_mutex_unlock(@m)
    if unlocked != 0
        print(f"pthread_mutex_unlock failed: {unlocked}")
        exit(code: 1)
    print("locked and unlocked")
```
MUTEX: no. The storage starts as `pthread_mutex_t(rest: zero)`, but pthread_mutex_init runs on it before any lock or unlock, and the program never uses a zeroed value that skipped init.
GUESSED: I mapped `char` to `u8` (not `i8`) in `u8[256]`, `u8[104]` and `__opaque: u8[56]`. I mapped `long` to `i64`. I assumed `Utsname(rest: zero)` and `pthread_mutex_t(rest: zero)` are legal with no named field. I declared `const struct sockaddr_un *` as `@addr: SockaddrUn`, because the spec has no rule for a const in-pointer. I assumed `to_fixed().must()` takes `u8[104]` from the annotation on `path`. I guessed `link "app"` and `link "pthread"`. I declared `record pthread_mutex_t` with no `tag` (the typedef names an anonymous struct) and guessed that `__sig`/`__opaque` are valid identifiers. I declared `attr: ptr` and passed `nullptr` for NULL. I used `-> i32` for every int result.