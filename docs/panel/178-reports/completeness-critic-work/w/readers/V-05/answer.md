TASK 1
```
extern "sys/utsname.h"
    record Utsname tag utsname
        sysname: u8[256]
        nodename: u8[256]
        release: u8[256]
        version: u8[256]
        machine: u8[256]
    function uname(@name: Utsname) -> i64

function main()
    u: Utsname @ Utsname(rest: zero)
    rc = uname(@u)
    if rc != 0
        print("uname failed: ", rc)
        exit(code: 1)
    match u.sysname.validated_bytes()
        .ok s  => print(s)
        .err e => print("sysname is not text: ", e.code)
```
TASK 2
```
extern "app.h"
    record SockaddrUn tag sockaddr_un
        sun_len: u8
        sun_family: u8
        sun_path: u8[104]
    function app_connect(@addr: SockaddrUn) -> i64

function main()
    sa: SockaddrUn @ SockaddrUn(
        sun_family: 1,
        sun_path: ['/', 't', 'm', 'p', '/', 'a', 'p', 'p', '.', 's', 'o', 'c', 'k', 0, 0, 0
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            0, 0, 0, 0, 0, 0, 0, 0],
        rest: zero)
    fd = app_connect(@sa)
    if fd == -1
        print("app_connect failed")
        exit(code: 1)
    print("connected, descriptor ", fd)
```
TASK 3
```
extern "pthread.h"
    record pthread_mutex_t
        __sig: i64
        __opaque: u8[56]
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
MUTEX: no. The program starts the cell from zeros with `rest: zero`, because every binding must be initialised, but it calls pthread_mutex_init on it before any lock or unlock.
GUESSED: (1) C `char` fields written as `u8[N]` rather than `i8[N]`: the spec says "header's own width and sign" and "a field of bytes" but never says what `char` maps to. (2) Passing a `const struct *` input parameter as an `@` parameter in task 2: the spec only names `@` for out-parameters. (3) `Type(rest: zero)` with no fields named at all. (4) `record pthread_mutex_t` with no `tag` for an anonymous typedef'd struct, since `tag` only covers the name written after `struct`. (5) `__sig` and `__opaque` accepted as identifiers. (6) `attr: ptr` given `nullptr` for a NULL-able `const pthread_mutexattr_t *`. (7) `-> i64` results for C `int`, using "a result may be wider". (8) No `link` for libc or pthread. (9) How newlines and indentation work inside the multi-line argument list and array literal, with no trailing commas at line ends. (10) Whether `@m`'s copy in and copy out moves an initialised mutex, which pthread forbids; the spec says copy in, copy out and does not say whether the cell's address stays the same. (11) Leaving `sun_len` at zero.