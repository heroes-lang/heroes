# Defect 092: the routes, built and measured, for panel 194

Lane ffi13 of batch 12, 2026-10-06, from 12:27 to 13:56 by the clock. This is
evidence for a sitting and not a ruling: nothing here is landed, and defect 092
stays open (`issues/2026-09/24/2026-09-24-0001-defect-092-a-whole-group-record-lent-to-a-void-parameter-is-handed-to-c.md`).
Every program cited is in `092/` beside this file; the two prototype compilers
are `092/prototype.diff` applied to `lane-b12-ffi13` at `e531fac3` (eight
files of `selfhost/`, each hunk marked `PROTOTYPE 092`). Every number below was
run that day: on this Mac (Apple clang 21.0.0, arm64), and on Linux arm64 in
the `heroes-linux-arm64` container (Debian clang 22.1.8). Durations are not
given anywhere: three lanes ran beside this one, so only instruction counts are
written (`/usr/bin/time -l`, *instructions retired*). Linux x86-64 and the
Windows box were not run for 092.

## The defect, and the shapes beside it

A group record lent whole through `@` reaches C as the address of the
program's cell, and when the C function is told how far to reach by ANOTHER
argument, nothing relates that argument to the record. The reproducer
(`092/today-read-into-addrinfo.hero`, panel 178's ffi-pragmatist) is `read`
into a 48-byte `struct addrinfo` with `n: 4096`. The shapes were attacked at
depth one in one pass, each on a header of the program's own (`092/s092.h`)
except the reproducer. **What the defect's title says, `void *`, is one of two
causes**: the same lend to the record's own pointer type, with a count, writes
past it too.

| program (`092/today-*`) | C parameter, and what the count counts | this Mac, today | Linux arm64, today |
|---|---|---|---|
| `read-into-addrinfo` | `void *`, bytes (`read`) | 138, after printing `4096` and `1094795585` | 135 (bus error), after printing `3568` and `1094795585` |
| `void-count-past` | `void *`, bytes, a count read at run time | 133 (SIGTRAP) | 139 |
| `void-count-fits` | `void *`, bytes, the record's own size: a correct program | 0 | 0, printing `4` and `1094795585` |
| `void-padded` | `void *`, bytes, a padded record | 133 | **0, silent**, printing `64` and `4702111234474983745` |
| `void-a-field-lent-whole` | `void *`, bytes, `@o.inner`, a record field lent whole | 133 | **0, silent**, printing `4096` and `1094795585` |
| `typed-pointer-byte-count` | `struct one *`, bytes | 133 | **0, silent**, printing `4096` and `1094795585` |
| `typed-pointer-record-count` | `struct pfd *`, RECORDS (`poll`, `epoll_wait`) | **0, silent**, printing `512` and `1`; ASan at `-O0`: *stack-buffer-overflow, WRITE of size 2* | **143, did not end**, stopped after 30 s; ASan at `-O0`: *WRITE of size 2* |
| `count-through-a-cell` | `void *`, bytes read through `uint32_t *` (`getsockopt`'s `optlen`) | 133 | **0, silent**, printing `4096` and `1094795585` |
| `const-void-read` | `const void *`, bytes (`write`, `send`) | **0, silent**, printing the sum of 4096 bytes of the stack; ASan at `-O0`: *stack-buffer-overflow, READ of size 1* | **143, did not end**, stopped after about 120 s; ASan at `-O0`: *READ of size 1* |
| `const-void-read-by-value` | `const void *` handed the record by value | 1, `ffi_parameter_type`: clang refuses a struct for a pointer | 1, `ffi_parameter_type` |

**The platform decides whether the overwrite is seen at all.** Four shapes
that die of SIGTRAP on this Mac (`void-padded`, `void-a-field-lent-whole`,
`typed-pointer-byte-count`, `count-through-a-cell`) run to exit 0 on Linux
arm64 and print a corrupted value, and two (`typed-pointer-record-count`,
`const-void-read`) do not end there. AddressSanitizer, at the `-O0` that
`build --sanitize` uses, was run on those two on both platforms and reports
both, as the cells say; the other shapes were not run under it for this file.

Programs that read standard input were given 4096 bytes of `A`
(`head -c 4096 /dev/zero | tr '\0' A`). The issue's own reruns hold
`--sanitize` on the reproducer: *stack-buffer-overflow, WRITE of size 4096*.

## Route A: the record declares its count, and an undeclared one to `void *` is refused

What the prototype does (`092/prototype.diff`, the hunks in
`check/lend_extent.hero`, `emit/lend_extent.hero`, `cli/pointee_wants.hero`,
`cli/header_types.hero`, `emit/ffi_pointee.hero`, `emit/ffi.hero`):

- `counted_by n` is accepted on an `@` parameter whose type is a group record,
  where today it is `counted_by_shape` (*`buf` is not* a `ptr`);
- before such a call the emitter writes the compare the field lend already
  writes for a count read at run time: `if ((uint64_t)n > sizeof(<the record>))
  hero_panic(...)`, so C never runs with a count past the record. The
  prototype writes the run-time compare for a constant count too; the field
  lend's `_Static_assert` form, which refuses a constant at build time, is
  the landing's to add;
- the pointee check (panel 103's unit) asks each `@` whole-record parameter
  with no `counted_by` whether the header's pointee is `void`, with defect
  010's guard, `!__builtin_types_compatible_p(T, void)`, which ignores
  qualifiers, so `const void *` is caught by the same line; where it is, the
  build stops at exit 1 with `ffi_parameter_type` on the declaration. No new
  code was needed in the prototype; whether the class wants its own is the
  sitting's.

| program | route A, this Mac (the `today-*` rows: route A's compiler, and the same under A and B) | route A, Linux arm64 (the compiler with A and B) |
|---|---|---|
| `today-read-into-addrinfo` (no `counted_by`) | 1, *`buf` of `read` lends a whole record to C as `void *`, and nothing says how many bytes C may reach through it* | 1, `ffi_parameter_type` |
| `today-void-count-fits` (correct, no `counted_by`) | **1, refused**: the price, every such lend must declare its count | 1, `ffi_parameter_type` |
| `today-const-void-read` | 1, refused | 1, `ffi_parameter_type` |
| `today-count-through-a-cell` | 1, refused | 1, `ffi_parameter_type` |
| `today-typed-pointer-byte-count` | **133, unchanged**: a typed pointer is not asked | **0, silent**, printing `4096` and `1094795585` |
| `today-typed-pointer-record-count` | **0, silent, unchanged** | **143, did not end**, stopped after 30 s |
| `a-read-into-addrinfo` (`counted_by n`, `n: 4096`) | 134 before C runs: *`read` was told a count in `n` past the bytes of the record lent to `buf`* | 134 before C runs |
| `a-read-into-addrinfo-fits` (`n: 8`) | 0 | 0, printing `8` and `1094795585` |
| `a-void-count-past` (a count read at run time) | 134 | 134 before C runs |
| `a-void-count-fits` | 0 | 0, printing `4` and `1094795585` |
| `a-void-a-field-lent-whole` | 134 | 134 before C runs |
| `a-typed-pointer-byte-count` | 134 | 134 before C runs |
| `a-typed-pointer-record-count` (`nfds: 2`) | **0, silent**: 2 is under the record's 8 BYTES, and C writes 2 RECORDS; ASan at `-O0`: *WRITE of size 2* | **0, silent**, printing `2` and `1`; ASan at `-O0`: *WRITE of size 2* |
| `a-count-through-a-cell` (`counted_by n` on `@n: u32`) | **1, `counted_by_shape`**: the sibling must be an integer C takes by value, so `getsockopt` into a record cannot be bound | 1, `counted_by_shape` |
| `a-const-void-read` | 134 | 134 before C runs |

## Route B: `x.ptr()` lends a whole record, held to `counted_by` as a field is

Spec § 13 already says *`f.ptr()` lends a binding's field to a `ptr`
parameter declared `counted_by n` ... and one past the field is refused*, and
the compiler already checks it: at build time where the count is a constant
(`emit/lend_extent.hero`'s `_Static_assert`, read back as
`field_lend_extent`), and at run time where it is not. Route B widens `ptr()`
from a fixed run of bytes to a whole group record that is not a handle
(`check/lend_types.hero`), and takes the record's address where a field's
array decays (`emit/field_lend.hero`). The binding is the spec's own example,
`read(fd: i32, buf: ptr counted_by n lent, n: u64)`, and the call
`read(fd: 0, buf: h.ptr(), n: 4096)`. A lend from a `=` binding crosses as
`const void *`, as a field's does (panel 166). Route B alone does not touch
`@buf: Hints`: it is the remedy route A's refusal can name, and the two were
measured in one compiler.

| program | routes A and B, this Mac | Linux arm64 |
|---|---|---|
| `b-read-into-addrinfo` (`n: 4096`, a constant) | **1 at build**, `field_lend_extent` on the number | 1, `field_lend_extent` |
| `b-read-into-addrinfo-fits` (`n: 8`) | 0 | 0, printing `8` and `1094795585` |
| `b-void-count-past` (read at run time) | 134 before C runs | 134 before C runs |
| `b-void-count-fits` | 0 | 0, printing `4` and `1094795585` |
| `b-void-a-field-lent-whole` (`o.inner.ptr()`) | 1 at build | 1, `field_lend_extent` |
| `b-typed-pointer-byte-count` | 1 at build | 1, `field_lend_extent` |
| `b-typed-pointer-record-count` (`nfds: 2`) | **0, silent**, the same unit gap as route A; ASan at `-O0`: *WRITE of size 2* | **0, silent**, printing `2` and `1`; ASan at `-O0`: *WRITE of size 2* |
| `b-count-through-a-cell` | **1, `counted_by_shape`**, as route A | 1, `counted_by_shape` |
| `b-const-void-read` (`o = One(...)`) | 1 at build | 1, `field_lend_extent` |

The prototype's message names the field's length as `0 bytes`, because the
emitter's length is a fixed array's and a record's size is C's
(`emit/lend_extent.hero`'s `fixed_length`); the comparison itself is C's own
`sizeof` and is right. A landing words it from clang's answer.

## What neither route closes, measured

1. **A count of RECORDS against a record's own pointer type** (`poll`,
   `epoll_wait`, `kevent`). `counted_by` counts what the C function counts,
   and both prototypes compare in bytes, as the field lend does; `nfds: 2`
   passes against an 8-byte record and C writes a second record past it. The
   unit is the C function's, and C cannot be asked it at the call site: no
   C11 expression yields a parameter's pointee type. Two routes were found and
   not built: **C**, the pointee check, which already knows each parameter's
   pointee text (`cli/header_types.hero`), hands the round a `-D` per counted
   record lend naming the unit, records where the pointee is the record's own
   type and bytes where it is `void`; and **C'**, refusing `counted_by` on a
   lend whose header pointee is not `void`, so a typed pointer is one record
   and nothing counts it, which leaves `poll` with one descriptor bindable
   only where `nfds` is written as 1 by the program, unchecked.
2. **The same lend with NO declaration, to the record's own pointer type**
   (`today-typed-pointer-*`): neither route asks it, because C's `struct pfd *`
   is the right type for one record and for an array alike. A rule that
   reached it would have to refuse every typed `@` record lend beside an
   integer parameter, or ask every one to say *one*; neither was built.
3. **A count C reads through a pointer** (`getsockopt`'s `socklen_t *`):
   `counted_by` refuses an `@` sibling (`counted_by_shape`) under both routes,
   so route A's refusal makes this binding impossible rather than checked. The
   compare would read the cell before the call (`*n <= sizeof`), and C writes
   the length back; not built.

## Two routes found and not built

- **D, the compiler passes the size.** A mark on the count, for instance
  `n: u64 sizes buf`, and the call names no `n`: the compiler writes
  `sizeof` of the record lent to `buf`. Nothing can be wrong at the call, and
  it fits `read`, `recv`, `setsockopt`, and `getsockopt` with the cell
  initialised to the size; it needs the unit ruling of (1) for `poll`, a
  grammar change in `CParam`, and a word in the spec.
- **E, refusal alone.** Route A's refusal with no `counted_by` to answer it:
  measured as route A on `today-*`, it refuses the correct
  `today-void-count-fits` and leaves no way to `read` into a record.

## What each route would change in the tracked programs

Enumerated by `092/census/scan.py` over `git ls-files -- examples
tests/golden`, and each lend's C parameter asked of clang by
`092/census/ask.py` (the `__typeof__` redeclaration `cli/pointee.hero` uses):

- **12 whole-record `@` lends, in 9 programs, all under `tests/golden/`;
  none in `examples/`.** clang answered 10 of them, and each is the record's
  own pointer type (`regex_t *`, `const struct addrinfo *`, `struct slot *`,
  ...); the 2 in `run/ffi-a-construction-polls-an-sdl3-event.hero` were not
  answered, because the census gave clang no package flags and it found no
  `SDL3/SDL.h`; that program is among the 9 the compilers below built. **None
  of the 10 answered goes to `void *`.**
- **Route A changes none of the 9**: each builds with the same exit and the
  same stderr, compared by `cmp`, under the prototype as under the lane's
  compiler at `e531fac3`.
- **Route B changes none of the 9, and none of the 15 programs holding the 24
  `ptr counted_by` parameters** under `tests/golden/` (none in `examples/`):
  15 of 15 the same exit and the same stderr.
- Neither route changes a program of the compiler's own; `selfhost/` was not
  enumerated beyond building the prototypes, which compiled it.

## What each costs, in instructions retired, on this Mac

- **The compare before the call** (both routes; route B's is the field lend's
  existing one): `092/cost/guarded.hero`, 10 million calls with a count that
  changes each iteration so it is not hoisted, its emitted C compiled with the
  build's flags with and without the one line, three runs each.
  `-O2`: 375.8 to 376.5 million against 355.8 to 356.4 million, about **2.0
  instructions per call**, 5.6% of a loop doing nothing else. `-O0`: 4,556.4
  to 4,557.3 million against 4,526.3 to 4,527.3 million, about **3.0 per
  call**, 0.66%.
- **Route A's question at build time**, one more line in the pointee check's
  verdict unit per lend, cold builds in fresh directories, three runs each:
  `run/fixedbugs-173-...` (regcomp) 2,916.9 to 2,917.5 million against
  2,898.7 to 2,899.3 million, **+0.63%**; a program whose one lend is a
  record, 2,810.5 to 2,810.8 million against 2,805.9 to 2,806.1 million,
  **+0.16%**. Route B asks nothing at build time beyond what the field lend
  asks.

## What the specification would have to say

Counted offline on the vendored tables (`heroes measure`), so each is a lower
bound and not the reader's count (`.claude/rules/spec-shape.md`); no paid run
was made.

| route | sentence | tokens, vendored maximum |
|---|---|---|
| A | *A record lent whole through `@` to a parameter C reads as `void *` declares `counted_by n` too, and a count past the record is refused.* | 38 |
| A with the unit of (1) | *A record lent whole through `@` declares `counted_by n` where C is told its extent: in bytes where C reads `void *`, in records where it reads the record's own type, and a count past the record is refused.* | 53 |
| B with A's refusal | § 13's sentence becomes *`f.ptr()` lends a binding, or its field, to a `ptr` parameter declared `counted_by n`, naming the sibling that gives the extent, and one past it is refused. A record lent whole through `@` to C's `void *` is refused.* | 61, from 41 (+20) |
| E | *A record lent whole through `@` to C's `void *` is refused: lend it with `ptr()`.* | 26 |
| D | *`n: u64 sizes buf` makes the compiler pass the size of the record lent to `buf`, and the call names no `n`.* | 31, and a `CParam` production |

## Found beside, while measuring

`heroes run --sanitize` builds at `-O2`, and there AddressSanitizer did not
report a 4096-byte write past a 4-byte stack record made by `memset` inside a
header's `static inline` function: exit 0, the program printing `4096`. The
same program built with `build --sanitize` (`-O0`) reports
*stack-buffer-overflow, WRITE of size 4096*, exit 134, and so does plain C at
`-O0`; plain C at `-O2` misses it as well, so it is the toolchain's and not
this compiler's, and it means the `run` suite's sanitized leg can be blind to
this shape. Reported to the coordinator, not filed.

## How to run it again

```
# the prototypes: lane-b12-ffi13 at e531fac3, with 092/prototype.diff applied
patch -p1 < docs/panel/194-evidence/092/prototype.diff
./heroes build selfhost/main.hero -o heroes-routes

# a shape, its standard input 4096 bytes of `A`
head -c 4096 /dev/zero | tr '\0' A > a4096.txt
./heroes-routes run docs/panel/194-evidence/092/a-read-into-addrinfo.hero < a4096.txt

# the tracked lends and the C parameter each reaches
python3 docs/panel/194-evidence/092/census/scan.py . > rows.json
python3 docs/panel/194-evidence/092/census/ask.py . rows.json
```

The two result tables, every program under each compiler with its exit and
its first words, are `092/results-darwin-arm64.tsv` and
`092/results-linux-arm64.tsv`.
