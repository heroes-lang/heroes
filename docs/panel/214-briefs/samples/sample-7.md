# Sample 7

## The header, `seven.h`

```c
#include <stdint.h>
static inline int32_t seven(void) { return 7; }
```

## The program, `shelf.hero`

```
use parts/one

extern "seven.h"
    function seven() -> i32

record Held
    b: str

function mine() -> Held?
    return fail(code: "x", msg: "mine")

function main()
    print(one.failure(one.made()) + one.failure(mine()))
    print(seven())
```

## Its module `parts/one.hero`

```
record Held
    a: i64

function failure<T>(result: T?) -> str
    return match result
        .ok _  => ""
        .err e => e.msg

function count<T>(xs: [T]) -> i64
    return xs.len()

function keys_of<T>(m: {str: T}) -> i64
    return m.len()

function both<A, B>(a: [A], b: [B]) -> i64
    return a.len() + b.len()

function apply<T>(f: (function(T) -> i64), x: T) -> i64
    return f(x)

function made() -> Held?
    return ok(Held(a: 1))

function weigh(h: Held) -> i64
    return h.a
```

## Message X (the only one)

```
internal error: compiling the generated C failed:
shelf.c:165:23: error: conflicting types for 'h_partsone_failure_9c07dcd'
  165 | HERO_TU_LOCAL HeroStr h_partsone_failure_9c07dcd(h_0opt_ca5589c h0_result);
      |                       ^
shelf.c:164:23: note: previous declaration is here
  164 | HERO_TU_LOCAL HeroStr h_partsone_failure_9c07dcd(h_0opt_731f54b6 h0_result);
      |                       ^
shelf.hero:13:37: error: passing 'h_0opt_ca5589c' (aka 'struct h_0opt_ca5589c') to parameter of incompatible type 'h_0opt_731f54b6' (aka 'struct h_0opt_731f54b6')
   13 |     t4 = h_partsone_failure_9c07dcd(t3);
      |                                     ^~
shelf.c:164:66: note: passing argument to parameter 'h0_result' here
  164 | HERO_TU_LOCAL HeroStr h_partsone_failure_9c07dcd(h_0opt_731f54b6 h0_result);
      |                                                                  ^
parts/one.hero:7:23: error: conflicting types for 'h_partsone_failure_9c07dcd'
    7 | HERO_TU_LOCAL HeroStr h_partsone_failure_9c07dcd(h_0opt_ca5589c h0_result) {
      |                       ^
parts/one.hero:7:23: note: previous definition is here
    7 | HERO_TU_LOCAL HeroStr h_partsone_failure_9c07dcd(h_0opt_731f54b6 h0_result) {
      |                       ^
3 errors generated.

the generated C is at build/tu-1c549a3203922877/shelf.c
error: clang refused the generated C
```
