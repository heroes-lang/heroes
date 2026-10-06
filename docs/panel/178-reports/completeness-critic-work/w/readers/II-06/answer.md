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
    path: u8[104]? = "/tmp/app.sock".to_fixed()
    match path
        .ok p =>
            addr: SockaddrUn @ SockaddrUn(sun_len: 106, sun_family: 1, sun_path: p)
            fd = app_connect(@addr)
            if fd < 0
                print("could not connect to /tmp/app.sock")
            else
                print(f"connected, descriptor {fd}")
        .err e => print(e.code)
```
TASK 3
```
extern "pthread.h"
    record pthread_mutex_t
        __sig: i64
        __opaque: u8[56]
    function pthread_mutex_init(@m: pthread_mutex_t, attr: ptr lent) -> i64
    function pthread_mutex_lock(@m: pthread_mutex_t) -> i64
    function pthread_mutex_unlock(@m: pthread_mutex_t) -> i64

function main()
    opaque: u8[56] = [
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
    ]
    m: pthread_mutex_t @ pthread_mutex_t(__sig: 0, __opaque: opaque)
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
MUTEX: no. The language makes every binding start with a value, so the program writes out zero bytes as the starting storage (by hand, with no `zero` claim, because all zeros is not a valid mutex everywhere). pthread_mutex_init runs on that storage before any lock or unlock.
GUESSED: C `char` arrays declared as `u8[N]` rather than `i8[N]`; the spot for the `zero` marker (`record Utsname tag utsname zero`, after `tag` the way `partial` is placed), since the grammar leaves it out; `Utsname(rest: zero)` with no fields named; `.validated_bytes()` working on a `u8[256]` field; a struct pointer declared as an `@` parameter, including the input-only `const struct sockaddr_un *`; `to_fixed()` taking its array size from the annotated `u8[104]?` binding; `sun_len: 106` (sizeof the struct); no `link` for app.h; the record named `pthread_mutex_t` after the typedef, with no `tag`, because the header gives no struct tag; `__sig` and `__opaque` accepted as identifiers; the NULL attr declared as `attr: ptr lent` and passed as `nullptr`; commas inside a row and newlines between rows in the 56-element literal; C `int` results declared `-> i64` (the spec allows a wider result).