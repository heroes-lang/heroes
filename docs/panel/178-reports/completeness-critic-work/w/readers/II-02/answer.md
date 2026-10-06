TASK 1
```
extern "sys/utsname.h"
    record Utsname tag utsname zero
        sysname: i8[256]
        nodename: i8[256]
        release: i8[256]
        version: i8[256]
        machine: i8[256]
    function uname(@name: Utsname) -> i32

function main()
    u: Utsname @ Utsname(rest: zero)
    r = uname(@u)
    if r != 0
        print(f"uname failed: {r}")
        exit(code: 1)
    match u.sysname.validated_bytes()
        .ok name => print(name)
        .err e => print(e.code)
```
TASK 2
```
extern "app.h"
    record SockaddrUn tag sockaddr_un zero
        sun_len: u8
        sun_family: u8
        sun_path: i8[104]
    function app_connect(@addr: SockaddrUn) -> i32

function main()
    fixed: i8[104]? = "/tmp/app.sock".to_fixed()
    addr: SockaddrUn @ SockaddrUn(sun_family: 1, sun_path: fixed.must(), rest: zero)
    fd = app_connect(@addr)
    if fd == -1
        print("app_connect failed")
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
    m: pthread_mutex_t @ pthread_mutex_t(__sig: 0, __opaque: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
    init = pthread_mutex_init(@m, nullptr)
    if init != 0
        print(f"pthread_mutex_init failed: {init}")
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
MUTEX: no. The program fills every field with zero only to have storage to hand to pthread_mutex_init, which runs and is checked before any lock. I did not claim `zero` on the record, because all zeros is not a valid mutex on every platform.
GUESSED: where `zero` goes on the record line (after `tag`, in the same slot as `partial`, since the grammar leaves it out); `Utsname(rest: zero)` with no field named; C `char` written as `i8` rather than `u8` (`i8[256]`, `i8[104]`, `i8[56]`); a `@` parameter for every struct pointer, including the input-only `const struct sockaddr_un *`, with no `lent`; `to_fixed` getting its target type from the `i8[104]?` annotation and then `.must()` inside the constructor; `attr: ptr` with `nullptr` standing in for a NULL `const pthread_mutexattr_t *`; naming the typedef'd anonymous struct `record pthread_mutex_t` with no `tag`; `__sig` and `__opaque` being legal identifiers; `long` as `i64`; `int` results declared `i32`; no `link` needed for pthread; `sun_len` left at zero.