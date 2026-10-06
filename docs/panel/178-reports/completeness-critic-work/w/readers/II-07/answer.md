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
    r = uname(@u)
    if r != 0
        print("uname failed: ", r)
        exit(code: 1)
    match u.sysname.validated_bytes()
        .ok name => print(name)
        .err e => print("sysname is not text: ", e.code)
```
TASK 2
```
extern "app.h" link "app"
    record SockaddrUn tag sockaddr_un zero
        sun_len: u8
        sun_family: u8
        sun_path: u8[104]
    function app_connect(@addr: SockaddrUn) -> i32

function main()
    addr: SockaddrUn @ SockaddrUn(sun_family: 1, sun_path: "/tmp/app.sock".to_fixed().must(), rest: zero)
    fd = app_connect(@addr)
    if fd < 0
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
    m: pthread_mutex_t @ pthread_mutex_t(__sig: 0, __opaque: [
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
    ])
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
MUTEX: no. The zeros are only the storage the binding needs to be initialised; pthread_mutex_init runs on it before any lock or unlock, and the record never claims `zero`.
GUESSED: C `char` fields and arrays written as `u8` (the spec never says which sign `char` gets); where `zero` goes in the record line (`tag utsname zero`, copied from how `partial` is placed; the Member grammar does not show `zero`); a `const struct sockaddr_un *` input passed as an `@` parameter (the spec only names `@` for out-parameters); `link "app"` for app.h; `"...".to_fixed().must()` getting its fixed-array type from the constructor field; naming the record `pthread_mutex_t` with no `tag`, because the typedef is of an anonymous struct; `__sig`/`__opaque` being legal identifiers; `long` as `i64`; the nullable `const pthread_mutexattr_t *` declared as `attr: ptr` and passed `nullptr`; `i32` for every result; copy-in/copy-out `@m` being acceptable for a mutex; mixing commas within a line and newlines between lines in the 56-element array literal.