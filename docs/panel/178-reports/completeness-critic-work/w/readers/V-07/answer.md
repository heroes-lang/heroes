TASK 1
```
extern "sys/utsname.h"
    record Utsname tag utsname
        sysname: i8[256]
        nodename: i8[256]
        release: i8[256]
        version: i8[256]
        machine: i8[256]
    function uname(@name: Utsname) -> i64

function main()
    u: Utsname @ Utsname(rest: zero)
    rc = uname(@u)
    if rc != 0
        print(f"uname failed: {rc}")
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
        sun_path: i8[104]
    function app_connect(@addr: SockaddrUn) -> i64

function main()
    # sun_len is left zero by rest: zero; sun_family is AF_UNIX
    addr: SockaddrUn @ SockaddrUn(
        sun_family: 1,
        sun_path: ['/', 't', 'm', 'p', '/', 'a', 'p', 'p', '.', 's', 'o', 'c', 'k'
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        rest: zero)
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
        __opaque: i8[56]
    function pthread_mutex_init(@m: pthread_mutex_t, attr: ptr) -> i64
    function pthread_mutex_lock(@m: pthread_mutex_t) -> i64
    function pthread_mutex_unlock(@m: pthread_mutex_t) -> i64

function main()
    # zeroed storage only; pthread_mutex_init makes it a mutex before any use
    m: pthread_mutex_t @ pthread_mutex_t(rest: zero)
    rc = pthread_mutex_init(@m, nullptr)
    if rc != 0
        print(f"pthread_mutex_init failed: {rc}")
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
MUTEX: no (the mutex starts as zeroed storage via rest: zero, but pthread_mutex_init runs on it before lock or unlock touches it)
GUESSED: i8[N] for C char[N] (the spec says "header's own width and sign" but never says what plain char is; u8 was the other candidate); Utsname(rest: zero) and pthread_mutex_t(rest: zero) naming no field at all; @addr: SockaddrUn for a const struct sockaddr_un * input (the spec only covers out-parameters, and a by-value record would not match the pointer); u.sysname.validated_bytes() working on an i8 field as "a field of bytes"; record pthread_mutex_t with no tag for a typedef of an anonymous struct; __sig and __opaque as legal identifiers; long as i64; attr: ptr with nullptr for a NULL const pthread_mutexattr_t * (relying on the "what a ptr points at" exception); no link clause for app.h (no library named) or for pthread.h; sun_len left zero; the multi-line array layout (commas inside a row, newlines between rows, ] on the last row); character literals taking i8 from context; whether an @ record argument needs lent (I followed the sqlite example and left it out).