TASK 1
```
extern "sys/utsname.h"
    record Utsname tag utsname partial
        sysname: i8[256]
    function uname(@name: Utsname) -> i64

function main()
    u: Utsname @ Utsname(sysname: [
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    ])
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

constant AF_UNIX: u8
    1

function main()
    addr: SockaddrUn @ SockaddrUn(sun_len: 106, sun_family: AF_UNIX, sun_path: [
        '/', 't', 'm', 'p', '/', 'a', 'p', 'p', '.', 's', 'o', 'c', 'k'
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    ])
    fd = app_connect(@addr)
    if fd < 0
        print("app_connect failed: ", fd)
        exit(code: 1)
    print("connected on descriptor ", fd)
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
    m: pthread_mutex_t @ pthread_mutex_t(__sig: 0, __opaque: [
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
    ])
    init_rc = pthread_mutex_init(@m, nullptr)
    if init_rc != 0
        print("pthread_mutex_init failed: ", init_rc)
        exit(code: 1)
    lock_rc = pthread_mutex_lock(@m)
    if lock_rc != 0
        print("pthread_mutex_lock failed: ", lock_rc)
        exit(code: 1)
    unlock_rc = pthread_mutex_unlock(@m)
    if unlock_rc != 0
        print("pthread_mutex_unlock failed: ", unlock_rc)
        exit(code: 1)
    print("locked and unlocked")
```
MUTEX: no. The mutex value starts as zeros only because a record has to be built before it can be passed as `@m`. It always goes through pthread_mutex_init before lock or unlock.
GUESSED: `i8` (not `u8`) for C `char` arrays (`sysname`, `sun_path`, `__opaque`); `partial` with only `sysname` declared, and whether a partial record can be constructed; `.validated_bytes()` on an `i8[256]` field; `@addr: SockaddrUn` for `const struct sockaddr_un *` (the spec only names `@` for out-parameters and never says how a const struct pointer is declared); `attr: ptr` with `nullptr` for the NULL-able `const pthread_mutexattr_t *`; naming the record `pthread_mutex_t` with no `tag` for an anonymous typedef'd struct; `__sig`/`__opaque` being valid identifiers; `sun_len: 106` (sizeof, as BSD expects; the header does not say); leaving out `link` for `app.h` and `pthread.h`; the multi-line array layout (newline after `[`, rows split by NEWLINE, no trailing comma, `])` on its own line); `-> i64` for C `int` results; and whether copy-in/copy-out of `@m` moves the mutex in memory between calls.