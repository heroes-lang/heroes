TASK 1
```
# Print the operating system's name, as uname reports it.

extern "sys/utsname.h"
    record Utsname tag utsname
        sysname: u8[256]
        nodename: u8[256]
        release: u8[256]
        version: u8[256]
        machine: u8[256]
    function uname(@name: Utsname) -> i64

function main()
    zeros: u8[256] = [
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
    ]
    u: Utsname @ Utsname(
        sysname: zeros,
        nodename: zeros,
        release: zeros,
        version: zeros,
        machine: zeros
    )
    rc = uname(@u)
    if rc != 0
        print("uname failed: ", rc)
        exit(1)
    match u.sysname.validated_bytes()
        .ok name => print(name)
        .err e   => print("sysname is not text: ", e.code)
```
TASK 2
```
# Connect to the local socket at /tmp/app.sock.

extern "app.h"
    record SockaddrUn tag sockaddr_un
        sun_len: u8
        sun_family: u8
        sun_path: u8[104]
    function app_connect(@addr: SockaddrUn) -> i64

function main()
    addr: SockaddrUn @ SockaddrUn(
        sun_len: 106,
        sun_family: 1,
        sun_path: [
            '/', 't', 'm', 'p', '/', 'a', 'p', 'p', '.', 's', 'o', 'c', 'k'
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        ]
    )
    fd = app_connect(@addr)
    if fd == -1
        print("could not connect to /tmp/app.sock")
        exit(1)
    print("connected, descriptor ", fd)
```
TASK 3
```
# Make a mutex, lock it, unlock it.

extern "pthread.h"
    record pthread_mutex_t
        __sig: i64
        __opaque: u8[56]
    function pthread_mutex_init(@m: pthread_mutex_t, attr: ptr) -> i64
    function pthread_mutex_lock(@m: pthread_mutex_t) -> i64
    function pthread_mutex_unlock(@m: pthread_mutex_t) -> i64

function main()
    # Every binding must be initialised, so the cell starts as zeros;
    # pthread_mutex_init fills it before anything else touches it.
    m: pthread_mutex_t @ pthread_mutex_t(
        __sig: 0,
        __opaque: [
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            0, 0, 0, 0, 0, 0, 0, 0
        ]
    )
    rc_init = pthread_mutex_init(@m, nullptr)
    if rc_init != 0
        print("pthread_mutex_init failed: ", rc_init)
        exit(1)
    rc_lock = pthread_mutex_lock(@m)
    if rc_lock != 0
        print("pthread_mutex_lock failed: ", rc_lock)
        exit(1)
    rc_unlock = pthread_mutex_unlock(@m)
    if rc_unlock != 0
        print("pthread_mutex_unlock failed: ", rc_unlock)
        exit(1)
    print("locked and unlocked")
```
MUTEX: no. The program does build a zero-filled pthread_mutex_t, because the spec requires every binding to be initialised, but that cell goes to pthread_mutex_init before lock or unlock ever see it.
GUESSED: C `char` declared as `u8` rather than `i8` (the spec says "header's own width and sign" but not what `char` maps to); a `const struct sockaddr_un *` in-parameter declared and passed as `@addr` (the spec only names `@` for out-parameters); the NULL-able `const pthread_mutexattr_t *` declared as `attr: ptr` and passed `nullptr`; `long` as `i64`; `__sig` and `__opaque` accepted as identifiers; a typedef'd anonymous struct declared as `record pthread_mutex_t` with no `tag`; no `link` needed for pthread; `int` results declared `-> i64` (relying on "a result may be wider"); a fixed-array type on a local binding (`zeros: u8[256] = [...]`); mixing comma and newline separators in one multi-line literal; `sun_len: 106` (sizeof the struct); calling `validated_bytes()` on a field reached through a binding (`u.sysname`); and whether `@` copy-in/copy-out keeps a mutex's address stable across calls, which the spec does not say.