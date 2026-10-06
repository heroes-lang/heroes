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

function main()
    addr: SockaddrUn @ SockaddrUn(sun_len: 106, sun_family: 1, sun_path: "/tmp/app.sock".to_fixed().must())
    fd = app_connect(@addr)
    if fd < 0
        print("app_connect failed")
    else
        print(f"connected, descriptor {fd}")
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
    rc_init = pthread_mutex_init(@m, nullptr)
    if rc_init != 0
        print(f"pthread_mutex_init failed: {rc_init}")
        exit(code: 1)
    rc_lock = pthread_mutex_lock(@m)
    if rc_lock != 0
        print(f"pthread_mutex_lock failed: {rc_lock}")
        exit(code: 1)
    rc_unlock = pthread_mutex_unlock(@m)
    if rc_unlock != 0
        print(f"pthread_mutex_unlock failed: {rc_unlock}")
        exit(code: 1)
    print("locked and unlocked")
```
MUTEX: no. The record is not marked `zero` and the program does not use `rest: zero`. The literal full of zeros is there only because every binding has to start with a value, and pthread_mutex_init fills the mutex before any lock or unlock.
GUESSED: `u8` for C `char` in the three char arrays (the spec never says how plain `char`, whose sign depends on the platform, maps); where `zero` goes in `record Utsname tag utsname zero` (the grammar leaves `zero` out, and the prose only says "after its name"); `Utsname(rest: zero)` with no fields named; `@addr: SockaddrUn` for a `const struct sockaddr_un *` input (the spec only shows `@` for out-parameters, and never shows how a pointer to a record goes in); `to_fixed().must()` inline, taking the size u8[104] from the field it fills; `sun_len: 106` (the header gives no value); `record pthread_mutex_t` with no `tag`, named after the typedef because the struct has no tag, with `__sig` and `__opaque` accepted as identifiers; `long` as `i64`; `attr: ptr` given `nullptr` for the attr pointer, on the strength of the "what a ptr points at" exception; no `link` for pthread; and whether copy-in/copy-out on `@m` keeps the mutex at one address between lock and unlock.