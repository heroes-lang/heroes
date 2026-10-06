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
    u: Utsname @ Utsname(rest: zero)
    r = uname(@u)
    if r != 0
        print("uname failed: ", r)
        exit(code: 1)
    match u.sysname.validated_bytes()
        .ok name => print(name)
        .err e => print(e.code)
```
TASK 2
```
extern "app.h"
    record sockaddr_un
        sun_len: u8
        sun_family: u8
        sun_path: u8[104]
    function app_connect(@addr: sockaddr_un) -> i32

function main()
    path = "/tmp/app.sock"
    if path.len() >= 104
        print("path too long")
        exit(code: 1)
    addr: sockaddr_un @ sockaddr_un(sun_family: 1, rest: zero)
    for i in range(from: 0, to: path.len())
        addr.sun_path[i] @ path[i]
    fd = app_connect(@addr)
    if fd == -1
        print("connect failed")
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
    m: pthread_mutex_t @ pthread_mutex_t(rest: zero)
    r = pthread_mutex_init(@m, nullptr)
    if r != 0
        print("pthread_mutex_init failed: ", r)
        exit(code: 1)
    l = pthread_mutex_lock(@m)
    if l != 0
        print("pthread_mutex_lock failed: ", l)
        exit(code: 1)
    u = pthread_mutex_unlock(@m)
    if u != 0
        print("pthread_mutex_unlock failed: ", u)
        exit(code: 1)
    print("locked and unlocked")
```
MUTEX: no. The cell does start as `pthread_mutex_t(rest: zero)`, because every binding has to be initialised, but `pthread_mutex_init(@m, nullptr)` runs on it before any lock or unlock, and a non-zero result exits.
GUESSED: C `char` arrays written as `u8[N]` (the spec never says how `char` maps; I went on "field of bytes" and `s[i]` being `u8`); `tag utsname` for the struct names; task 2's `sockaddr_un` with no `tag`, since it is written after `struct` too, so that is inconsistent with task 1 and one of the two is wrong; `record pthread_mutex_t` with no `tag` for an anonymous typedef'd struct; `Utsname(rest: zero)` / `pthread_mutex_t(rest: zero)` naming no field at all; `@addr: sockaddr_un` for a `const struct sockaddr_un *` in-parameter (the spec only says an OUT-parameter is `@`); `attr: ptr` plus `nullptr` for the nullable `const pthread_mutexattr_t *`, relying on "what a `ptr` points at" being unchecked; `__sig` / `__opaque` being legal identifiers; `long` as `i64`; `addr.sun_path[i] @ path[i]` indexing a fixed-array field with an `i64` from `range`; `-> i32` results; no `link` for libc/pthread.