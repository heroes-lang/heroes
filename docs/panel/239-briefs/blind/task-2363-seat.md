# The task

A program of three modules, each file given whole below, built with `heroes build main.hero
--library lib`, where the folder `lib` holds two C libraries the program's groups name: `liba.a`
(built from `pick_a.c` and `a_only.c`) and `libw.a` (built from `pick_w.c` and `w_only.c`). Every C
file is given below too; you may change any file of this folder, the C ones included.

`main.hero`:

```
use aux
use picker

function main()
    print(picker.chosen())
    print(aux.only())
```

`picker.hero`:

```
extern "a.h" link "a"
    function pick() -> i32
    function a_only() -> i32

function chosen() -> i32
    return pick()

function only() -> i32
    return a_only()
```

`aux.hero`:

```
extern "w.h" link "w"
    function w_only() -> i32

function only() -> i32
    return w_only()
```

`a.h`: `int pick(void);` and `int a_only(void);`. `w.h`: `int w_only(void);`.
`pick_a.c`: `int pick(void) { return 1; }`. `a_only.c`: `int a_only(void) { return 10; }`.
`pick_w.c`: `int pick(int *out) { *out = 99; return 2; }`. `w_only.c`: `int w_only(void) { return 30; }`.

The author meant the program to print `1` and then `30`.

## What happened

`heroes build main.hero --library lib` exits 1 and prints:

```
error[ffi_defined_elsewhere]: `pick`, bound here from `a.h` with `link "a"`, is defined by two libraries this program links, and the order of the libraries decides which one a call reaches: `liba.a(pick_a.o)` in one order, `libw.a(pick_w.o)` in the other
  at picker.hero:2:5
    |
  2 |     function pick() -> i32
    |     ^^^^^^^^^^^^^^^^^^^^^^
  note: the linker found a definition in `lib/liba.a(pick_a.o)`
  note: the linker found a definition in `lib/libw.a(pick_w.o)`
  note: `-lw` is linked for the group of `w.h`, `link "w"`
  note: a linked program holds one definition of `pick`, taken from the first library on the link line that defines it, and the order of that line is not the program's to write: link one library that defines `pick`, or make the other's local where that library is built (over an archive `objcopy --localize-symbol=pick`, or `nmedit -R` on macOS; for a shared library `-fvisibility=hidden` or a version script)
```

Repair the program so that it builds and prints what its author meant.
