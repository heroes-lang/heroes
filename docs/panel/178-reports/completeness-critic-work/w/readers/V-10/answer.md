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
    r = uname(@u)
    if r != 0
        print(f"uname failed: {r}")
        exit(code: 1)
    match u.sysname.validated_bytes()
        .ok s  => print(s)
        .err e => print(e.code)
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
    addr: SockaddrUn @ SockaddrUn(
        sun_family: 1,
        sun_path: [
            '/', 't', 'm', 'p', '/', 'a', 'p', 'p', '.', 's', 'o', 'c', 'k'
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        ],
        rest: zero
    )
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
    function pthread_mutex_init(@m: pthread_mutex_t, attr: ptr) -> i64
    function pthread_mutex_lock(@m: pthread_mutex_t) -> i64
    function pthread_mutex_unlock(@m: pthread_mutex_t) -> i64

function main()
    m: pthread_mutex_t @ pthread_mutex_t(rest: zero)
    init_rc = pthread_mutex_init(@m, nullptr)
    if init_rc != 0
        print(f"pthread_mutex_init failed: {init_rc}")
        exit(code: 1)
    lock_rc = pthread_mutex_lock(@m)
    if lock_rc != 0
        print(f"pthread_mutex_lock failed: {lock_rc}")
        exit(code: 1)
    unlock_rc = pthread_mutex_unlock(@m)
    if unlock_rc != 0
        print(f"pthread_mutex_unlock failed: {unlock_rc}")
        exit(code: 1)
    print("locked and unlocked")
```
MUTEX: no. The record starts from `rest: zero` only as storage, and pthread_mutex_init runs on it before any lock or unlock.
GUESSED: `char` as `i8` (C leaves its sign to the platform; I assumed signed, as on macOS); `Utsname(rest: zero)` and `pthread_mutex_t(rest: zero)` naming no field at all; `@` for the read-only `const struct sockaddr_un *` in app_connect (the spec shows `@` only for out-parameters and names no by-pointer input form); `sun_len` left at zero through `rest: zero`; the untagged record named exactly `pthread_mutex_t` for an anonymous typedef struct, and fields spelled `__sig`/`__opaque`; `attr: ptr` with `nullptr` standing in for the NULL `pthread_mutexattr_t *`; `-> i64` for every `int` result (the spec lets a result be wider); the multi-line fixed-array literal with commas inside a row and newlines between rows; whether copy-in/copy-out on `@m` moves the mutex's address between the init, lock and unlock calls (the spec does not say).