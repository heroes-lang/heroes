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
    if uname(@u) != 0
        print("uname failed")
        exit(code: 1)
    match u.sysname.validated_bytes()
        .ok name => print(name)
        .err e => print(f"sysname is not text: {e.code}")
```
TASK 2
```
extern "app.h"
    record SockaddrUn tag sockaddr_un
        sun_len: u8
        sun_family: u8
        sun_path: u8[104]
    function app_connect(@addr: SockaddrUn) -> i64

function unix_address(path: str) -> SockaddrUn?
    return ok(SockaddrUn(sun_len: 0, sun_family: 1, sun_path: path.to_fixed()?))

function main()
    match unix_address(path: "/tmp/app.sock")
        .ok a =>
            addr: SockaddrUn @ a
            fd = app_connect(@addr)
            if fd == -1
                print("could not connect to /tmp/app.sock")
                exit(code: 1)
            print(f"connected, descriptor {fd}")
        .err e => print(f"bad path: {e.code} {e.msg}")
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
    m: pthread_mutex_t @ pthread_mutex_t(__sig: 0, __opaque: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
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
MUTEX: no. The storage starts as zeros written out field by field (the record is deliberately not marked `zero`, because all zeros is not a valid mutex on every platform), and pthread_mutex_init always runs on it before any lock.
GUESSED: T1 `u8[256]` for `char[256]` (the spec says "header's own width and sign" but char's sign depends on the platform), where `zero` goes in `record Utsname tag utsname zero` (the grammar doesn't show it; I put it in the `partial` slot), `Utsname(rest: zero)` naming no field at all, `@name: Utsname` for `struct utsname *`, `-> i64` as a wider result; T2 `@addr: SockaddrUn` for a `const struct sockaddr_un *` in-parameter (no `lent`, no read-only form found), `sun_len: 0` (the header gives no value), `u8[104]` for `char[104]`, `path.to_fixed()?` taking its target type from the constructor field; T3 `record pthread_mutex_t` with no `tag` for an anonymous typedef'd struct (a record name in C's lowercase spelling), `__sig: i64` for `long`, `__opaque: u8[56]` for `char[56]`, identifiers beginning with `__`, `attr: ptr` passed `nullptr` for the NULL-able `const pthread_mutexattr_t *`, `@m` as the mutex pointer, and the 56-element literal on one comma-separated line (whether commas and newlines may mix is unclear).