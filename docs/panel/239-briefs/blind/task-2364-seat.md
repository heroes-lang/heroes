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
error[ffi_record_two_layouts]: `Rs` is laid out two ways: 8 bytes aligned to 4 in `rs.hero`'s unit, 16 bytes aligned to 8 in `m.hero`'s, which reads `rs.h` after `m.h`
  at rs.hero:2:12
  note: `rs.hero`'s unit reads `rs.h` after no other header: `a` at 0 (`int`), `b` at 4 (`int`)
  note: `m.hero`'s unit reads it after `m.h`: `a` at 0 (`long long`), `b` at 8 (`int`)
  note: a value of `Rs` crosses between the two units, so both must read one layout; no order of the other modules' headers in `m.hero`'s unit, nor each with its own module's earlier headers before it, gives every record it spells its own module's layout
  note: spell `Rs` only in modules whose units read it as `rs.hero` does, or have `rs.hero` bind first the header that configures it, so both units read one layout
```

Repair the program so that it builds and prints what its author meant.
