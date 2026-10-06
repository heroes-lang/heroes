TASK 1
```
# Print the operating system's name, as uname reports it.

extern "sys/utsname.h"
    record Utsname tag utsname
        sysname: i8[256]
        nodename: i8[256]
        release: i8[256]
        version: i8[256]
        machine: i8[256]
    function uname(@name: Utsname) -> i32

function main()
    zeros: i8[256] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    name: Utsname @ Utsname(sysname: zeros, nodename: zeros, release: zeros, version: zeros, machine: zeros)
    rc = uname(@name)
    if rc != 0
        print("uname failed")
        exit(code: 1)
    match name.sysname.validated_bytes()
        .ok s  => print(s)
        .err e => print(e.code)
```
TASK 2
```
# Connect to the local socket at /tmp/app.sock.

extern "app.h"
    record SockaddrUn tag sockaddr_un
        sun_len: u8
        sun_family: u8
        sun_path: i8[104]
    function app_connect(@addr: SockaddrUn) -> i32

function main()
    addr: SockaddrUn @ SockaddrUn(sun_len: 106, sun_family: 1, sun_path: ['/', 't', 'm', 'p', '/', 'a', 'p', 'p', '.', 's', 'o', 'c', 'k', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
    fd = app_connect(@addr)
    if fd < 0
        print("connect failed")
        exit(code: 1)
    print("connected, descriptor ", fd)
```
TASK 3
```
# Make a mutex, lock it, unlock it.

extern "pthread.h"
    record pthread_mutex_t
        __sig: i64
        __opaque: i8[56]
    function pthread_mutex_init(@m: pthread_mutex_t, attr: ptr) -> i32
    function pthread_mutex_lock(@m: pthread_mutex_t) -> i32
    function pthread_mutex_unlock(@m: pthread_mutex_t) -> i32

function main()
    m: pthread_mutex_t @ pthread_mutex_t(__sig: 0, __opaque: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
    init_rc = pthread_mutex_init(@m, nullptr)
    if init_rc != 0
        print("pthread_mutex_init failed: ", init_rc)
        exit(code: 1)
    lock_rc = pthread_mutex_lock(@m)
    if lock_rc != 0
        print("pthread_mutex_lock failed: ", lock_rc)
        exit(code: 1)
    print("locked")
    unlock_rc = pthread_mutex_unlock(@m)
    if unlock_rc != 0
        print("pthread_mutex_unlock failed: ", unlock_rc)
        exit(code: 1)
    print("unlocked")
```
MUTEX: no. The zero-filled record is only the starting value handed to pthread_mutex_init, and lock/unlock run only after init returns 0. It is still built from zeros first, because the spec gives no other way to make a record value.
GUESSED: `char` fields as `i8[N]` (the spec says "header's own width and sign", but C leaves char's sign to the platform, and "a field of bytes" hints at u8); calling `validated_bytes()` on an `i8[256]` field; a fixed-array binding `zeros: i8[256] = [...]` outside an extern group; `-> i32` results; `@addr: SockaddrUn` for a `const struct sockaddr_un *` input (the spec only names `@` for out-parameters and has no form for a read-only struct pointer); `sun_len: 106` (sizeof the struct; the header does not say); `record pthread_mutex_t` with no `tag`, named after the typedef because the struct has no tag; `__sig`/`__opaque` being legal identifiers; `attr: ptr` with `nullptr` for the NULL attr pointer (resting on "what a ptr points at" being exempt); whether `@` copy-in/copy-out moves the mutex's address between init, lock and unlock (the spec says "copy in, copy out", and moving a pthread mutex is not safe).