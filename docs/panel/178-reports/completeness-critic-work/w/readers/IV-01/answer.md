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
    u: Utsname @ Utsname(sysname: [0; 256], nodename: [0; 256], release: [0; 256], version: [0; 256], machine: [0; 256])
    r = uname(@u)
    if r != 0
        print(f"uname failed: {r}")
        exit(code: 1)
    match u.sysname.validated_bytes()
        .ok s => print(s)
        .err e => print(e.code)
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
    addr: SockaddrUn @ SockaddrUn(sun_len: 106, sun_family: 1, sun_path: [0; 104])
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
    m: pthread_mutex_t @ pthread_mutex_t(__sig: 0, __opaque: [0; 56])
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
MUTEX: no. The program builds a zero-filled value only because every binding has to start with a value, then calls pthread_mutex_init on it before any lock or unlock.
GUESSED: C `char` fields written as `u8[N]` rather than `i8[N]` (the spec never says what `char` maps to); every `struct *` parameter written as an `@` record parameter, including task 2's `const struct sockaddr_un *`, which is an input and not an out-parameter; the nullable `const pthread_mutexattr_t *` written as `attr: ptr` and passed `nullptr`; `record pthread_mutex_t` named with the typedef's own name and no `tag`, since the header has no struct tag; `__sig` and `__opaque` accepted as identifiers; `sun_len: 106` (the header does not say what goes there); `[0; N]` in a constructor taking `u8` from the field type; indexing a fixed-array field with an `i64` from `range` in `addr.sun_path[i] @ path[i]`; `-> i32` for C `int` results; copy-in/copy-out of the mutex through `@m` being acceptable for a mutex, whose address C may expect to stay the same.