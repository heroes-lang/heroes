TASK 1
```
extern "sys/utsname.h"
    record Utsname tag utsname zero
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
        print("uname failed")
        exit(code: 1)
    match u.sysname.validated_bytes()
        .ok name => print(name)
        .err e => print(e.code)
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
    addr: SockaddrUn @ SockaddrUn(sun_len: 0, sun_family: 1, sun_path: "/tmp/app.sock".to_fixed().must())
    fd = app_connect(@addr)
    if fd == -1
        print("connect failed")
        exit(code: 1)
    print(f"connected: descriptor {fd}")
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
    m: pthread_mutex_t @ pthread_mutex_t(__sig: 0, __opaque: [0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0])
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
MUTEX: no. The cell starts as an all-zero literal written out in full, with no `zero` claim on the record, only because every binding needs a starting value; pthread_mutex_init fills it before any lock or unlock.
GUESSED: T1 where `zero` goes (I put it after `tag utsname`, since the grammar's record Member does not show it), `Utsname(rest: zero)` naming no fields, `u8[256]` for `char[256]` (the spec does not say whether C `char` is u8 or i8), `@name: Utsname` for `struct utsname *`, and `-> i64` for int; T2 `@addr: SockaddrUn` for the `const struct sockaddr_un *` input (the spec gives `@` only for out-parameters and shows no by-pointer input form for a record), `u8[104]` for `char[104]`, `sun_len: 0`, and whether `.to_fixed()` gets its u8[104] type from the field through `.must()`; T3 `record pthread_mutex_t` with no `tag` for an anonymous typedef struct, `__sig`/`__opaque` being legal identifiers, `u8[56]` for `char[56]`, `attr: ptr` plus `nullptr` for the NULL attr (so pthread_mutexattr_t is never declared), whether `@m` passes the cell's own address so the mutex does not move between calls, whether a list literal may break lines this way inside `[ ]`, and no `link` being needed for pthread.