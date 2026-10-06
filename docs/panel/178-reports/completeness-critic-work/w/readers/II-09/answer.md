TASK 1
```
extern "sys/utsname.h"
    record Utsname tag utsname zero
        sysname: u8[256]
        nodename: u8[256]
        release: u8[256]
        version: u8[256]
        machine: u8[256]
    function uname(@name: Utsname) -> i32

function main()
    u: Utsname @ Utsname(rest: zero)
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
        sun_path: u8[104]
    function app_connect(@addr: SockaddrUn) -> i32

function main()
    addr: SockaddrUn @ SockaddrUn(sun_len: 106, sun_family: 1, sun_path: "/tmp/app.sock".to_fixed().must())
    fd = app_connect(@addr)
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
    function pthread_mutex_init(@m: pthread_mutex_t, attr: ptr) -> i32
    function pthread_mutex_lock(@m: pthread_mutex_t) -> i32
    function pthread_mutex_unlock(@m: pthread_mutex_t) -> i32

function main()
    m: pthread_mutex_t @ pthread_mutex_t(__sig: 0, __opaque: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
    rc_init = pthread_mutex_init(@m, nullptr)
    if rc_init != 0
        print("pthread_mutex_init failed: ", rc_init)
        exit(code: 1)
    rc_lock = pthread_mutex_lock(@m)
    if rc_lock != 0
        print("pthread_mutex_lock failed: ", rc_lock)
        exit(code: 1)
    print("locked")
    rc_unlock = pthread_mutex_unlock(@m)
    if rc_unlock != 0
        print("pthread_mutex_unlock failed: ", rc_unlock)
        exit(code: 1)
    print("unlocked")
```
MUTEX: no. The program writes a placeholder of zeros field by field (there is no `zero` claim and no `rest: zero`, because all zeros is not a valid mutex on every platform), but pthread_mutex_init fills it before any lock or unlock.
GUESSED: `char` as `u8[N]` in all three (the spec never says how to spell C `char` or its sign); where `zero` goes in `record Utsname tag utsname zero` (the grammar leaves it out, and the prose only says "after its name"); `@addr: SockaddrUn` for a `const struct sockaddr_un *` input (the spec only says that a C out-parameter is `@`); `sun_len: 106` (sizeof, since the header gives no value); `"/tmp/app.sock".to_fixed().must()` taking `u8[104]` from its constructor position through `.must()`; naming the record `pthread_mutex_t` after an anonymous typedef struct, with `__`-prefixed field names allowed; `attr: ptr` with `nullptr` for the NULL record pointer; `-> i32` on every result; and whether `@m` copy-in/copy-out hands C the cell's own address, since a pthread mutex must not be moved between init, lock and unlock.