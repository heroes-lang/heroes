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
    function app_connect(@addr: SockaddrUn) -> i64

constant AF_UNIX: u8
    1

function unix_address(path: str) -> SockaddrUn?
    return ok(SockaddrUn(sun_len: 106, sun_family: AF_UNIX, sun_path: path.to_fixed()?))

function main()
    addr: SockaddrUn @ unix_address("/tmp/app.sock").must()
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
    function pthread_mutex_init(@m: pthread_mutex_t, attr: ptr) -> i64
    function pthread_mutex_lock(@m: pthread_mutex_t) -> i64
    function pthread_mutex_unlock(@m: pthread_mutex_t) -> i64

function main()
    m: pthread_mutex_t @ pthread_mutex_t(rest: zero)
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
MUTEX: no. The cell starts as `pthread_mutex_t(rest: zero)` only because every binding has to be initialised, and pthread_mutex_init runs on it before any lock.
GUESSED: I guessed these lines. `u8[N]` for every C `char` array (sysname..machine, sun_path, __opaque), where `i8[N]` may be right because char is signed. `Utsname(rest: zero)` and `pthread_mutex_t(rest: zero)`, which name no field at all before `rest: zero`. `@addr: SockaddrUn` for the input-only `const struct sockaddr_un *`, because the spec only says an OUT-parameter is `@`. `sun_len: 106`, which is the struct size; the header does not say what goes there. `sun_path: path.to_fixed()?`, trusting that the field position fixes `u8[104]`. `record pthread_mutex_t` with no `tag`, because it is a typedef of an anonymous struct. `__sig` and `__opaque` being legal identifiers. `attr: ptr` with `nullptr` for the NULL attr. No `link` on the pthread group.