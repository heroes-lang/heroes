TASK 1
```
extern "sys/utsname.h"
    record Utsname tag utsname zero
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
        print("uname failed: ", rc)
        exit(code: 1)
    match u.sysname.validated_bytes()
        .ok s => print(s)
        .err e => print("sysname is not text: ", e.code)
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
    path: i8[104] = "/tmp/app.sock".to_fixed().must()
    sa: SockaddrUn @ SockaddrUn(sun_len: 0, sun_family: 1, sun_path: path)
    fd = app_connect(@sa)
    if fd == -1
        print("app_connect failed")
        exit(code: 1)
    print("connected, descriptor ", fd)
```
TASK 3
```
extern "pthread.h" link "pthread"
    record pthread_mutex_t
        __sig: i64
        __opaque: i8[56]
    function pthread_mutex_init(@m: pthread_mutex_t, attr: ptr) -> i64
    function pthread_mutex_lock(@m: pthread_mutex_t) -> i64
    function pthread_mutex_unlock(@m: pthread_mutex_t) -> i64

function main()
    opaque: i8[56] = [
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
    ]
    m: pthread_mutex_t @ pthread_mutex_t(__sig: 0, __opaque: opaque)
    rc_init = pthread_mutex_init(@m, nullptr)
    if rc_init != 0
        print("pthread_mutex_init failed: ", rc_init)
        exit(code: 1)
    rc_lock = pthread_mutex_lock(@m)
    if rc_lock != 0
        print("pthread_mutex_lock failed: ", rc_lock)
        exit(code: 1)
    rc_unlock = pthread_mutex_unlock(@m)
    if rc_unlock != 0
        print("pthread_mutex_unlock failed: ", rc_unlock)
        exit(code: 1)
    print("locked and unlocked")
```
MUTEX: no. The program writes the starting value out as explicit zeros and does not mark the record `zero`, because all zeros is not a valid mutex. It always calls pthread_mutex_init on that value before lock or unlock.
GUESSED: C `char` declared as `i8` (every `i8[N]` field and binding); where `zero` goes, written as `record Utsname tag utsname zero` (the grammar has no slot for it); `Utsname(rest: zero)` with no field named; `@addr: SockaddrUn` / `@sa` for a `const struct *` input, since the spec only shows `@` for out-parameters; `.to_fixed().must()` into a typed `i8[104]` binding; `sun_len: 0`; `record pthread_mutex_t` with no `tag` for an anonymous typedef struct; `__sig` and `__opaque` accepted as identifiers; `attr: ptr` with `nullptr` for the nullable attr; `link "pthread"`; no `link` for app.h; no `lent` on any `@` record parameter; the multi-line layout of the 56-zero array literal, with rows split by newline and no trailing commas.