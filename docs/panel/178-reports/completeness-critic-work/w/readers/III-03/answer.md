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
    addr: SockaddrUn @ SockaddrUn(sun_family: 1, sun_path: "/tmp/app.sock".to_fixed().must(), rest: zero)
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
        __opaque: u8[56]
    function pthread_mutex_init(@m: pthread_mutex_t, attr: ptr) -> i64
    function pthread_mutex_lock(@m: pthread_mutex_t) -> i64
    function pthread_mutex_unlock(@m: pthread_mutex_t) -> i64

function main()
    m: pthread_mutex_t @ pthread_mutex_t(rest: zero)
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
MUTEX: no. It starts as zeros through `rest: zero`, only because I need some value in the cell, and `pthread_mutex_init` runs on it before any lock or unlock.
GUESSED: C `char` arrays written as `u8[N]` rather than `i8[N]` (all three tasks); C `long` as `i64`; C `int` results widened to `-> i64`; `Utsname(rest: zero)` and `pthread_mutex_t(rest: zero)` naming no field at all; passing the input-only `const struct sockaddr_un *` as an `@addr` parameter, since the spec shows only out-parameters reaching a struct pointer; `"/tmp/app.sock".to_fixed().must()` still taking its fixed-array type from the field position through `.must()`; naming the record `pthread_mutex_t` with no `tag`, because the typedef'd struct has no struct tag; `__sig` and `__opaque` being accepted as identifiers; the NULL `attr` declared as `attr: ptr` and passed `nullptr` instead of declaring a `pthread_mutexattr_t` record; using an `@` argument inside a comparison (`if pthread_mutex_init(@m, nullptr) != 0`).