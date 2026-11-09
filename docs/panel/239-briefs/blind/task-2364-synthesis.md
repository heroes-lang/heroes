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
error[ffi_record_two_layouts]: `Rs` means two things: where module `rs` reads `rs.h`, `a` is an `int` at byte 0 and `b` an `int` at byte 4, 8 bytes in all; where module `m` reads `rs.h` after `m.h`, `a` is a `long long` at byte 0 and `b` an `int` at byte 8, 16 bytes in all
  at rs.hero:2:12
  note: a value of `Rs` passes between modules `rs` and `m`, so both must read `rs.h` one way; module `m` reads its own headers first (spec § 13), so what `m.h` defines reaches `rs.h` there
  note: spell `Rs` in a module that does not read `m.h`, or keep what `m.h` defines from reaching the headers read after it (an `#undef` at the end of `m.h`)
```

Repair the program so that it builds and prints what its author meant.
