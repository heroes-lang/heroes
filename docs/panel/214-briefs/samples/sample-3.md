# Sample 3

## The header, `gauge.h`

```c
int fabs;
static inline int eight(void) { return 8; }
```

## The program

```
extern "gauge.h"
    function eight() -> i32

function main()
    print(eight())
```

## Message X

```
internal error: compiling the generated C failed:
In file included from build/tu-5bfa93040775c7f4/efabs.c:20:
./gauge.h:1:5: error: redefinition of 'fabs' as different kind of symbol
    1 | int fabs;
      |     ^
build/tu-5bfa93040775c7f4/efabs.c:20:10: note: './gauge.h' included multiple times, additional include site here
   20 | #include <gauge.h>
      |          ^
build/tu-5bfa93040775c7f4/efabs.c:20:10: note: './gauge.h' included multiple times, additional include site here
In file included from build/tu-5bfa93040775c7f4/efabs.c:20:
./gauge.h:1:5: note: unguarded header; consider using #ifdef guards or #pragma once
    1 | int fabs;
      |     ^
1 error generated.

the generated C is at build/tu-5bfa93040775c7f4/efabs.c
error: clang refused the generated C
```

## Message Y

```
error[ffi_header_refused]: `gauge.h`, the header this group names, does not compile: its line 1 declares `fabs`, which clang itself declares first, at that very line, as a name of C's own, and clang refuses the two: redefinition of 'fabs' as different kind of symbol
  at gauge.hero:1:8
    |
  1 | extern "gauge.h"
    |        ^^^^^^^^^^
  note: `gauge.h` line 1 reads `int fabs;`
  note: C's own `fabs` is declared by clang itself where a unit first names it, as clang declares each function of the C library it knows and each builtin of its own: its notes, the include line of this program's C named for each declaration and a guard asked of the header, place both at the header's own line, which a guard would not change
  note: a group's header is compiled as this program's own C includes it, before any declaration of the group is checked against it: repair the header, or name the one that declares this group's C
  fix (guess): rename `fabs` in `gauge.h`, a name of C's own, which clang declares itself
```
