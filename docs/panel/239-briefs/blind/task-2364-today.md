# The task

A program of three modules, each file given whole below, built with `heroes build main.hero`. The
two headers are the program's own; you may change any file of this folder.

`main.hero`:

```
use m
use rs

function main()
    print(m.total(x: rs.make(v: 3)))
```

`m.hero`:

```
use rs

extern "m.h"
    record Mm
        w: i32
    function mm_make(v: i32) -> Mm

function total(x: rs.Rs) -> i64
    m = mm_make(v: 4)
    return to_i64(x.a + x.b + m.w).must()
```

`rs.hero`:

```
extern "rs.h"
    record Rs
        a: i32
        b: i32
    function rs_make(v: i32) -> Rs

function make(v: i32) -> Rs
    return rs_make(v: v)
```

`m.h`:

```
#define TWO_OTHERS_WIDE 1
typedef struct Mm { int w; } Mm;
static inline Mm mm_make(int v) { Mm x = { v }; return x; }
```

`rs.h`:

```
#ifdef TWO_OTHERS_WIDE
typedef struct Rs { long long a; int b; } Rs;
#else
typedef struct Rs { int a; int b; } Rs;
#endif
static inline Rs rs_make(int v) { Rs x = { v, 2 }; return x; }
```

The author meant the program to print `9`.

## What happened

`heroes build main.hero` exits 1 and prints:

```
error[ffi_field_type]: `Rs.a` is `long long` in `rs.h`, a signed 64-bit integer, not `i32` — clang read the header's struct and the field disagrees
  at rs.hero:3:9
    |
  3 |         a: i32
    |         ^
  note: a group's `record` IS the header's struct (§4.19), so every field is at the header's own width and sign: correct `a`, or name the header that spells `Rs` this way
  fix (guess): declare `a` with `i64`
```

Repair the program so that it builds and prints what its author meant.
