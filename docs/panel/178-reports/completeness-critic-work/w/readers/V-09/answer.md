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
    info: Utsname @ Utsname(rest: zero)
    rc = uname(@info)
    if rc != 0
        print(f"uname failed: {rc}")
        exit(code: 1)
    match info.sysname.validated_bytes()
        .ok s  => print(s)
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
    addr: SockaddrUn @ SockaddrUn(sun_family: 1, rest: zero)
    path = "/tmp/app.sock"
    for i in range(from: 0, to: path.len())
        addr.sun_path[i] @ path[i]
    fd = app_connect(@addr)
    if fd < 0
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
    m: pthread_mutex_t @ pthread_mutex_t(rest: zero)
    init_rc = pthread_mutex_init(@m, nullptr)
    if init_rc != 0
        print(f"pthread_mutex_init failed: {init_rc}")
        exit(code: 1)
    lock_rc = pthread_mutex_lock(@m)
    if lock_rc != 0
        print(f"pthread_mutex_lock failed: {lock_rc}")
        exit(code: 1)
    print("locked")
    unlock_rc = pthread_mutex_unlock(@m)
    if unlock_rc != 0
        print(f"pthread_mutex_unlock failed: {unlock_rc}")
        exit(code: 1)
    print("unlocked")
```
MUTEX: no. The cell starts from zeros (`rest: zero`) only because every binding must be initialised, and it goes through pthread_mutex_init before any lock or unlock.
GUESSED: C `char` fields declared as `u8[N]` (sysname..machine, sun_path, __opaque), since the spec does not say whether char counts as i8 or u8; every pointer-to-struct parameter written as `@` (uname, app_connect even though C says `const`, the three pthread functions), including whether `@`'s copy-in/copy-out keeps the mutex at one address across lock and unlock; `Record(rest: zero)` naming no fields at all; `attr: ptr` passed `nullptr` for a NULL-able `const pthread_mutexattr_t *`; `record pthread_mutex_t` with no `tag` for a typedef of an anonymous struct, and `__sig`/`__opaque` being legal identifiers; element assignment `addr.sun_path[i] @ path[i]` through a record field; leaving `sun_len` at zero.