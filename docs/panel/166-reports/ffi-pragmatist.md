# Panel 166 — ffi-pragmatist

Method: compile, don't opine. Every number below is from a command run on
2026-09-19 in a scratchpad copy of the tree, with the seed rebuilt there
(`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, `real 3.65`).
`archive/` was deleted from the copy before anything was built.

Apple clang version 21.0.0 (clang-2100.1.1.101), arm64-apple-darwin25.6.0.

---

## 0. Both defects reproduce, and the shapes beside them are worse

```
$ ./heroes run d063.hero --include .
7
1094795585        # 0x41414141, exit 0
$ ./heroes run d063.hero --include . --sanitize
AddressSanitizer: stack-buffer-overflow ... WRITE of size 1
  [32, 44) 'h0_s' <== Memory access at offset 44 overflows this variable
$ ./heroes run d065.hero --include .
72
65                # exit 0
```

**Five shapes beside the defect, all run, all exit 0:**

| shape | program | result |
|---|---|---|
| extent is a **variable** | `n = 4096` then `sum_n(p: b.u.ptr(), n: n)` | `222404`, exit 0 |
| field of a field | `sum_n(p: o.i.b.ptr(), n: 4)` | `360`, exit 0 — legal |
| same field twice in one call | `sum2(a: b.u.ptr(), na: 8, b: b.u.ptr(), nb: 8)` | `16`, exit 0 — legal |
| inside a loop | lend re-taken each iteration (`t14 = (void *)(h0_b.u)`) | `24`, exit 0 — legal |
| **field of a `partial` record** | `fill_n(p: b.u.ptr(), n: 24)` on `u8[8]` | `520`, exit 0 — **corrupts** |
| `u8[N]` vs `i8[N]` | both cross | shipped golden, exit 0 |

The variable-extent shape is the one that decides the repair: **no static route
can see it.** The `partial` shape is the one that decides the *mechanism*: Heroes
does not know that struct's size, and the check still has to be right.

## 1. What the lend actually emits — the ABI question, settled

```c
t19 = (void *)(h0_s.nsap);          /* the lend */
t20 = INT64_C(8);
t21 = slot_sum(t19, t20);           /* the call */
```

A bare array-to-pointer decay, cast to `void *`. No box, no copy, no
marshalling. **Routes A, B, C, D, E all preserve this line unchanged**, so my
veto does not fire on any of them. Route B's `&s.name` is the same address:
`(void*)&s.name == (void*)s.name` prints `1`.

## 2. `__counted_by` from the other side — two corrections and a refusal

**Correction to my own brief and to panel 165's historian.** The claim was that
`_LIBC_COUNT` expands to nothing on this Mac. That is true *without the flag*.
With it:

```
$ clang -std=c11 -fbounds-safety -dM -E -x c /dev/null | grep -i bounds
#define __LIBC_STAGED_BOUNDS_SAFETY_ATTRIBUTES 1

$ clang -std=c11 -fbounds-safety -E -P hdr.c
char *__attribute__((__terminated_by__(0))) getcwd(char *__attribute__((__counted_by_or_null__(__size))), size_t __size);
ssize_t read(int, void *__attribute__((__sized_by__(__nbyte))), size_t __nbyte) __asm("_" "read" );
char *__attribute__((__terminated_by__(0))) tmpnam(char *__attribute__((__counted_by__(1024))));
```

**The SDK already carries the extents Heroes wants, for exactly the functions
panel 164 opened the lend for.** And the enforcement is real, against
libSystem's own `getcwd`, with no recompilation of the callee:

```
$ ./gc          # honest n=8        -> before id=7 / after id=7 r=0x0, exit 0
$ ./gc x        # overstated n=1024 -> before id=7, exit 133 (SIGTRAP)
```

Heroes writing the attribute works too: `void *__sized_by(n) p` on a wrapper
traps on 64 into an 8-byte field and runs clean on 8, at exit 0 vs exit 133.

**And it is still refused, on a measurement.** `-fbounds-safety` is not a flag,
it is a C dialect, and Heroes' own generated C does not compile in it. On the
**shipped golden** `tests/golden/run/ffi-a-byte-field-crosses-to-c.hero`
(560 lines of emitted C):

```
$ clang -std=c11 -fbounds-safety -I runtime -I tests/golden/run -fsyntax-only g.c
exit 1 — 7 errors, 2 warnings
  static assertion failed: heroes-ffi-field Slot nsap
  static assertion failed: heroes-ffi-field Slot tag
  cannot initialize indexable pointer with type 'const Slot *__bidi_indexable'
    from __single pointer to incomplete type 'const void *__single'   (x4)
  passing 'const char *__bidi_indexable' to ... '__terminated_by(0)' is unsafe
```

Two of the seven are **§4.19's own assertions going red**: the `_Generic` over
`&((Slot *)0)->nsap` finds no arm, because every pointer type in the dialect
carries a `__bidi_indexable` qualifier, and falls to `default: 0`. The flag
breaks the mechanism that makes *"clang verifies the declared signature against
the real header"* a true sentence. Four more are the runtime's own
`const void *elem` hash callback, which every emitted unit carries.

It also refuses ordinary C outside Heroes: `char *r = getcwd(b.path, n);` is a
hard error, and it warns inside third-party header bodies Heroes does not own.

**Verdict on the attribute route: the data exists and the dialect cannot be
adopted.** Not "unavailable" — *unreachable at an acceptable price*, which is a
different sentence and the honest one.

## 3. The eighteen, read

The shared brief's two figures reproduce exactly (66 and 18). Reading them
changes what they mean. With `grep -rnoE` so each location carries its own
declaration:

| class | n | which |
|---|---|---|
| **not an `extern` declaration at all** | 4 | `cli/pointee.hero:5` and `ffi-pointee-width.hero:3` are **prose in comments**; `mutate/handles.hero:167` and `check/type_labels.hero:171` are **Heroes** functions, and their `count`/`n` relate to no pointer |
| single named sibling counts that pointer | 7 | `lending.hero:298,301,304,309` (`f(p,n)`, a fabricated `extern "s.h"` inside a unit-test string) and `slot_sum`/`slot_fill` x3, from **one header written for panel 164** |
| extent is a **product** `n * size` | 3 | `qsort` x2, `fwrite` — a single-name `counted_by n` is wrong for all three, by `sizeof(element)` |
| one count, **two** pointers | 2 | `memcpy(@dst, src, n)` — and both sites are defect cases clang already refuses |
| indirect: pointer-to-count | 2 | `getline(@line: ptr, @n: i32, stream: ptr)` — `*n` counts `*line`, two levels down, and `getline` *allocates* |

**Real C library functions whose extent is a single named sibling: zero.**

And the denominator is worse than 66 suggests. The 66 split `selfhost` 25 /
`examples` 4 / `tests` 37, and the four in `examples/` — the ladder bindings
this project actually ships — are:

```
examples/ledger/db/sqlite.hero:95   sqlite3_exec(db, sql: cstr, callback: ptr, context: ptr, @error: ...)
examples/ledger/db/sqlite.hero:98   sqlite3_free(p: ptr)
examples/ledger/db/sqlite.hero:107  sqlite3_bind_text(stmt, column, text: cstr, length: i32, destructor: ptr)
examples/sqlite/main.hero:63        sqlite3_exec(db, sql: cstr, callback: ptr, context: ptr, error: ptr)
```

**Not one takes a counted buffer.** The `ptr`s are a callback, an opaque user
context, an error slot and a destructor; `sqlite3_bind_text`'s `length` counts a
`cstr`, not a `ptr`. `grep -c '\.ptr()' examples/sqlite/main.hero` is **0**.

## 4. The C that route C would emit — compiled, against the real header

Standard C11, no flag, header untouched, one file-scope declaration per lend
call site:

```c
_Static_assert((int64_t)64 <= (int64_t)sizeof(((struct sl *)0)->name),
  "heroes-ffi-lend d063.hero:12 sl_fill p: the call states 64 bytes and the field `name` is 8");
```

```
$ clang -std=c11 -I. -fsyntax-only rc_lit.c
error: static assertion failed due to requirement '(long long)64 <= (long long)sizeof (((struct sl *)0)->name)':
  heroes-ffi-lend d063.hero:12 sl_fill p: the call states 64 bytes and the field `name` is 8
note: expression evaluates to '64 <= 8'
```

The honest call (`8`) compiles and runs: prints `7`, exit 0, `id` untouched.

**This is defect 063 turned into a compile error, at zero runtime cost, with the
call-site C byte-identical to today's.** It rests on §4.19's own sentence —
*"Every `extern` carries a `_Static_assert`"* — and the reader that turns such a
failure back into a `.hero` diagnostic at exit 1 **already exists**:
`selfhost/emit/ffi_pointee.hero:31` writes the `heroes-ffi-pointee ` marker and
`selfhost/cli/pointee.hero` parses clang's line back. Measured live:

```
$ ./heroes run out_n.hero --include .     # @n: i32 against size_t *
error[ffi_parameter_type]: `n` of `pick` is declared `i32`, and the header's
  `size_t *` points at a different width or sign
  fix (guess): declare `n` as `@n: u64`
```

Three properties I checked because route C would be wrong without them:

- **It is right on a `partial` record**, where Heroes does not know the struct.
  `_Static_assert(24 <= sizeof(((struct both *)0)->u))` → `expression evaluates
  to '24 <= 8'`. The number comes from C, so the field list Heroes holds is not
  load-bearing. This is the exact program that ran at exit 0 in §0.
- **It generalises to the product extent** with no new mechanism:
  `_Static_assert(8 * sizeof(int) <= sizeof(...->key))` → `'32 <= 16'`. But the
  **grammar must allow the product**, or `qsort` and `fwrite` get a check that
  passes while being wrong by `sizeof(element)` — three of the eighteen.
- **It cannot see a variable extent.** The `n = 4096` program read 4096 bytes at
  exit 0 and no `_Static_assert` can be written for it. So route C needs a
  runtime half, and it is one compare in standard C11:

```
panic: d063.hero:12 sl_fill p lends 4096 bytes of a 8-byte field
exit 134
```

Same shape as `guard_arguments`' null-`cstr` panic (`.claude/rules/c-boundary.md`).
Cost, 200 million lends, `-O2`, `noinline` callee, `volatile` extent so the loop
is not folded (the first attempt read `user 0.00` and was discarded as folded,
per CLAUDE.md § Verification's ratio rule):

```
unchecked   real 1.28  user 1.00  sys 0.00
checked     real 1.30  user 1.01  sys 0.00
```

**1% over 200M lends**, ratio clean. Robustness is rank 3 and speed is rank 5
(CL-012); this does not even test the ordering.

## 5. Route F is far cheaper than the brief prices it, and the brief is wrong

The shared brief says F *"leaves `getcwd` unwritable — the gap panel 164 opened
it for."* **Run, that sentence is false.** `getcwd` binds today with no lend at
all:

```
extern "unistd.h"
    function getcwd(buf: ptr, size: u64) -> cstr owned free
extern "stdlib.h"
    function free(p: ptr)
...
    p = getcwd(buf: nullptr, size: 0)
```
```
$ ./heroes run f_getcwd.hero
112                # and `pwd | tr -d '\n' | wc -c` is 112. exit 0.
```

`getcwd(NULL, 0)` allocates on macOS and glibc, `owned free` hands Heroes a
`str?`, the compiler frees it. This route predates panel 164.

F's **real** cost is `read(2)`, which has no allocating form. With the lend it
works (`12` bytes from stdin, exit 0). And there is an escape that works today
and is **better than the lend** — a `static inline` shim in the author's own
header:

```c
static inline int64_t buf_read(int fd, struct rdbuf *x) { return read(fd, x->b, sizeof x->b); }
```
```
    function buf_read(fd: i32, @x: Buf) -> i64
...
$ printf 'hello world\n' | ./heroes run f_shim.hero --include .
12
104                # 'h' — C wrote into the field. exit 0.
```

Emitted: `t3 = buf_read(t2, &h0_x);` with the probe
`hero_ffi_probe_h_fshim_buf_read(int32_t a0, struct rdbuf * a1)` — a **typed**
`struct rdbuf *`, which clang checks against the real header, where the lend
crosses as an untypeable `void *`. And **the extent is `sizeof x->b`, C's own,
which no Heroes call site can overstate.** This is the only route on the table
where defect 063 is impossible by construction rather than by a rule.

Two corrections it carries. The header is found with **no `--include`** (§4.19's
`-I<directory of the .hero source>`), and design.md §4.19's sentence that shim
compilation is *"undecided — panel 036 deferred both candidates, `heroes cc` and
a `compile "shim.c"` clause"* is true only for a separate `.c`: the header-only
`static inline` form needs **neither candidate** and ships.

## 6. What panel 164's resolution 2 does today — confirmed, twice

```
$ ./heroes check b_at.hero --brief
b_at.hero:10:17: error[type_mismatch]: expected `ptr`, found `i8[8]`
$ ./heroes check b_at2.hero --brief
b_at2.hero:10:17: error[not_a_place]: only a name, a field or an element can be passed as `@`
```

Both exactly as the shared brief states. And routes D and E:

```
d_len.hero:10:44: error[bad_operand]: `len` takes `str`, `[T]` or `{K: V}`, found `i8[8]`
e_named.hero:4:18: error[expected_array_length]: expected the array's length after `[`, found a name (`SL_NAME_LEN`)
```

---

# Verdicts

| route | verdict | ground |
|---|---|---|
| **A** refuse `.ptr()` from an immutable binding | **object** | closes half of 065 and **none** of 063; over-refuses a measured-safe read (`sum_n(p: s.name.ptr(), n: 8)` on `s = sl_make()` → `604`, correct), forcing `@` onto a value the program never mutates — which makes spec § 5 lie in the other direction. No ABI change. |
| **B** `@`-marked `ptr` takes `@field` | **approve, only shipped with C** | `&s.name == s.name` → `1`, so no ABI change and no veto. It puts the write mark where the **header** knows it, which is where §4.19 puts `consumes`, `acquires`, `owned`. Alone it closes 065 and leaves 063 wide: `sl_fill(p: @s.name, n: 64)` still writes 64 bytes. |
| **C** counted parameter | **approve — the only route that closes 063** | compiled above against the real header: `expression evaluates to '64 <= 8'`, standard C11, no flag, call-site C byte-identical, reader machinery already shipping. **Two conditions**, both measured: the grammar must take a **product** (3 of the 18 are `n * size`), and it needs a **runtime half** for the variable extent (1% over 200M lends). |
| **D** `len()` on a fixed field | **object as a repair, approve as ergonomics** | checks nothing — `n: s.name.len()` is a runtime `i64` no `_Static_assert` can see. Worth having only if C makes `<field>.len()` a *recognised* extent form, in which case it becomes the honest spelling and the assertion is trivially `sizeof(field) <= sizeof(field)`. |
| **E** named extent `i8[SL_NAME_LEN]` | **object on priority** | touches neither defect, and route C does not need it: `sizeof(((struct sl *)0)->name)` asks **C** for the number, so the name never has to reach the emitter. Route E's cost is a grammar change buying a legibility gain. |
| **F** refuse the lend entirely | **object, but it is cheap and the brief overprices it** | `getcwd` binds today with no lend (`112`, exit 0) — the brief's stated cost is false. Its real cost is `read(2)`, and the header-only `static inline` shim covers that **better** than the lend: typed `struct rdbuf *` probe, extent from C's own `sizeof`. If the panel will not pay for C, F beats G by a wide margin. |
| **G** write the hole into the specification | **VETO** | See below. |

## The veto, stated precisely

My veto is on *"anything that breaks the C ABI or makes bindings categorically
harder."* G does not touch the ABI. It makes bindings categorically harder in
the one sense that matters here, and the ground is **design.md §4.19** —
*"Clang verifies the declared signature against the real header is therefore a
true sentence, and this is the mechanism that makes it one"* — together with
**CLAUDE.md §12**, which holds a refusal to the same standard as a feature: a
Part 6 row must name the program or compiler fact that would make it wrong.

G asks the specification to declare that the extent is the call's word and
nothing checks it. I ran the check. It is one line of standard C11, it compiles
against the real header, it is right on a `partial` record where Heroes does not
know the struct, and it costs nothing at runtime. **A refusal cannot name the
fact that would make it wrong when the fact has already been compiled.** And the
ASan trace shows the corruption crossing into a *sibling field* — so under G a
binding author cannot reason about their own record locally, which is the
definition of categorically harder.

Secondary, and it is design.md §1.12 and CLAUDE.md § Precedence rank 3 rather
than my seat's ground: G leaves shipped code corrupting memory at exit 0.

**Adopt B + C together.** B marks the write at the declaration, which is where
the header knows it; C checks the extent, statically where the call states a
constant and at runtime where it does not. Neither changes a byte of the ABI.

## One falsifiable prediction

**Under route C as specified, none of the four real-library `ptr` externs in
`examples/` needs a `counted_by` clause, and the C emitted for
`examples/sqlite/main.hero` is byte-identical to today's.**

Measured basis, not hope: `examples/sqlite/main.hero` runs today at `rows: 3` /
`longest: 6`, exit 0, with **zero** `.ptr()` lends and three uncounted `ptr`
parameters (`callback`, `context`, `error`). So **step 3 of §4.19's ladder needs
no shim and no clause under route C.**

The falsifier: if any of those four requires a `counted_by` clause to keep
compiling, route C has been specified as a property of `ptr` parameters rather
than of the **lend's argument position**, and must be narrowed to the latter
before it lands. `heroes build examples/sqlite/main.hero --emit-c` before and
after is the instrument.

## What would change my verdicts

- **C → object** if the runtime half cannot be written without the emitter
  learning which argument is the extent at the *call* rather than at the
  *declaration*. I priced the declaration route; I did not price a call-site
  one, and `sum_n(p: ptr, n: i64)` genuinely relates nothing today
  (`selfhost/check/lend_types.hero:89-94` types the receiver only).
- **F → approve** if a seat shows that the `.ptr()` lend reaches a binding the
  header-only `static inline` shim cannot, on the §1.0 closure list or §4.19's
  ladder. I looked and did not find one; that is a claim about my vocabulary,
  not about the world.
- **G → object rather than veto** if someone compiles a real header where the
  `_Static_assert` in §4 above is *not* writable — a field whose size C itself
  cannot state. I could not construct one: `sizeof(((T*)0)->f)` is valid for
  every field of a complete type, and for a `partial` record C completes it.
