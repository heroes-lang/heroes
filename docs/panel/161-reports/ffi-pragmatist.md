# Panel 161 — ffi-pragmatist

**verdict**: approve route 3 · **veto route 4** · object to routes 1 and 2

**section**: design.md §1.11 (everything comes from C) and §4.19, through
`.claude/rules/c-boundary.md` § Why this boundary outranks the rest, and
CLAUDE.md § Precedence rank 3.

Every number below was produced on 2026-09-17 by a command run in a copy of the
tree at
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/4cbbc5a6-a98e-4b44-80b5-b6957110cd39/scratchpad/ffi/t`,
seed built from C alone in **3.30 s** (`/usr/bin/time -p clang -I runtime
seed/heroes.c runtime/runtime.c -o heroes`). Three machines, named beside each
figure:

| machine | clang | plain `char` |
|---|---|---|
| Darwin arm64 (the author's Mac) | Apple clang 21.0.0 | `CHAR_MIN=-128 CHAR_MAX=127` SIGNED |
| Debian arm64, `heroes-linux-arm64` | Debian clang 22.1.8 | `CHAR_MIN=0 CHAR_MAX=255` **UNSIGNED** |
| Debian x86-64, `heroes-linux` | Debian clang 22.1.8 | `CHAR_MIN=-128 CHAR_MAX=127` SIGNED |

Windows is **UNRUN**: the box is off and only the author starts it.

---

## 1. The headline: route 3's soundness cost, in numbers

The brief says route 3 "permits a silent sign confusion: 200 read back as −56".
**Measured, that confusion belongs to the status quo and to route 1, and route 3
is the only listed route that removes it.**

`probe/pchar.h` fills a `char[4]` with 200, 255, 128 and 65. `probe/arr_i8.hero`
and `probe/arr_u8.hero` bind it both ways. Route 3 was built by taking the
compiler's own `--emit-c` output for the spelling each machine accepts and
rewriting it into the other one: the four temporaries change type and the field
`_Static_assert` goes, which is exactly and only what route 3 relaxes. What a
Heroes program **reads**:

| declared | Darwin arm64 | Debian x86-64 | Debian arm64 |
|---|---|---|---|
| `i8[4]` today | `-56 -1 -128 65` | `-56 -1 -128 65` | **refused** `ffi_field_type` |
| `u8[4]` today | **refused** | **refused** | `200 255 128 65` |
| `i8[4]` under route 3 | `-56 -1 -128 65` | `-56 -1 -128 65` | `-56 -1 -128 65` |
| `u8[4]` under route 3 | `200 255 128 65` | `200 255 128 65` | `200 255 128 65` |
| what **C itself** sees (`(int32_t)t.name[0]`) | `-56` | `-56` | **`200`** |

Read the last row against the third and fourth. **Under route 3 one source file
gives one number on every leg.** Today the only source that compiles on every
leg is a per-platform source, and it reads `-56` on two legs and `200` on the
third. Route 1 reproduces that split exactly, with one spelling instead of two.

### The same, on a real header rather than a probe

`struct elf_prpsinfo` (`/usr/include/aarch64-linux-gnu/sys/procfs.h:84`) has
**four plain-`char` scalar fields**: `pr_state`, `pr_sname`, `pr_zomb`,
`pr_nice`. `pr_nice` is a nice value, −20…19. `probe/r_nice_*.hero` binds it and
reads a value C produced (`p.pr_nice = (char)-20`):

| | Debian x86-64 | Debian arm64 |
|---|---|---|
| today, `pr_nice: i8` | `-20` | **refused** `ffi_field_type` |
| today, `pr_nice: u8` | **refused** | **`236`** |
| route 3, `pr_nice: i8` | `-20` | **`-20`** |
| route 3, `pr_nice: u8` | `236` | `236` |

**Today the sign is chosen by the machine. Under route 3 it is chosen by the
author. Under route 1 it goes back to being chosen by the machine.** A binding
author who wants −20 can get it on all four legs only under route 3.

This also corrects the shared brief: **plain-`char` scalar fields are not 0 in
the world**, they are 0 in those nine headers. § 4 below has the wider walk.

## 2. What route 3 stops catching — compiled, not argued

`probe/route3_assert.c` is `extern_field.hero:154`'s row list with the `char`
row's sign conjunct removed and every other row untouched, plus seven
assertions naming what must survive. `probe/route3_ret.c` is the same question
at the result and parameter positions, against `extern_assert.hero:72-78`'s
`HERO_RET_*` macros, twelve assertions.

```
clang -std=c11 -Wall probe/route3_assert.c   ACCEPTED on all three machines
clang -std=c11 -Wall probe/route3_ret.c      ACCEPTED on all three machines
```

So route 3 keeps, measured: `signed char[4]` still refuses `u8`; `unsigned
char[4]` still refuses `i8`; `short[4]` still refuses `i8[4]`; `char[8]` still
refuses `i8[4]` and `char[4]` still refuses `i8[8]`; a `signed char` result
still refuses `u8` and an `unsigned char` result still refuses `i8`. **The only
thing it stops asking is the sign of the one C type that has no fixed sign, and
that is not information the header contains.**

`_Generic` was the thing that could have made route 3 unwritable at the result
position, and it does not: `_Static_assert(_Generic(give_char(), char:1,
default:0))` holds on all three machines — the controlling expression is lvalue-,
array- and function-converted but **not integer-promoted**, so a `char` result is
still visible as `char`. (`a_char_lvalue + 0` is promoted, and the file asserts
that too, so the distinction is measured rather than assumed.)

## 3. Routes 1 and 2 — the objection, compiled

The brief asks whether the assertion becomes decorative if the ninth type emits
as `char`. It does not become decorative. It becomes **less precise, in a
platform-dependent way**, and that is worse.

`extern_field.hero:154` tests **width and sign, never type identity** (panel
064, and its comment says so). With route 1's element — `char` itself — a field
the header declared `signed char` reaches the `signed char` row, whose conjunct
compares its sign to `char`'s. `probe/route1_collapse.c` and
`probe/route1_collapse_u.c` assert each acceptance:

| | Darwin arm64 | Debian x86-64 | Debian arm64 |
|---|---|---|---|
| `signed char[4]` accepted by the ninth spelling | **clang ACCEPTED** | **clang ACCEPTED** | clang REFUSED |
| `unsigned char[4]` accepted by the ninth spelling | clang REFUSED | clang REFUSED | **clang ACCEPTED** |

verbatim, from Debian arm64:

```
static assertion failed due to requirement '((signed char)-1 < 0) == ((char)-1 < 0)':
signed char[4] is accepted by the ninth type on this machine
```

**Route 1 removes one platform-dependent refusal and adds a platform-dependent
ACCEPTANCE in its place**, on the two `char` types that bind portably today
(`probe/s_controls.hero` reads `-56 200` on all three machines, unchanged). A
Heroes program could bind `signed char flags` as the ninth type on the Mac and
have it refused on Debian arm64. Route 3 cannot do this: `_Generic` dispatches
on the field's **actual** C type, so relaxing the `char` row reaches a field the
header spells plain `char` and nothing else.

Route 2 (a spelling legal only inside `extern`) inherits all of the above and
adds the sub-question the brief already names — what the program READS — which
is the whole content of § 1. Neither route touches the ABI: see § 5.

## 4. Route 4 — the veto, with the headers walked

I walked clang's own JSON AST (`ffi/walk.py`, `FieldDecl` whose `qualType` is
exactly `char` or matches `char[N]`), widening the brief's nine. Debian arm64,
in the container; raylib and SDL3 on the Mac, where the headers are installed.
**Plain-`char` positions the brief's nine did not carry:**

| header | what becomes unbindable |
|---|---|
| `utmpx.h` | `ut_line[32]`, `ut_id[4]`, `ut_user[32]`, `ut_host[256]` — who is logged in |
| `net/if.h` | `struct ifreq`'s `ifrn_name[16]` — every interface-naming ioctl |
| `signal.h` → `sys/procfs.h` | `elf_prpsinfo`'s four **scalars** plus `pr_fname[16]`, `pr_psargs[80]` |
| `netdb.h` | `ip_opts char[40]` |
| `raylib.h` | `BoneInfo.name[32]`, `ModelAnimation.name[32]`; and a **plain-`char` parameter**, `TextSplit(…, char delimiter, …)` — a second one beside `sqlite3_str_appendchar` |
| `dirent.h` | `d_name[256]` (Debian; `char[1024]` on Darwin) |
| `sys/utsname.h` | six `char[65]` (Debian); five `char[256]` (Darwin) |

`zlib.h` has **0** user-facing plain-`char` fields (its nine are pthread
internals), and `sqlite3.h` has **0** fields and **1** parameter.

Three real headers that become unbindable under route 4, walked rather than
recalled: **`dirent.h`** (you cannot read a filename), **`sys/utsname.h`** (the
struct is nothing but plain-`char` arrays, so `partial` gives `empty_record`),
**`net/if.h`** (you cannot name an interface). Add `utmpx.h` and raylib's
skeletal-animation pair.

**And the workaround an author would reach for is a memory-safety defect that
the compiler accepts today.** `probe/t_loud_cstr.hero` binds `char buf[8]` of
`typedef struct { char buf[8]; int32_t tail; char stop; } Loud` — a member with
no NUL in it — as `cstr`, which the field path permits because `_Generic`
lvalue-converts the array to `char *` and `sizeof(char[8]) == sizeof(char *)`.
On **all three machines**:

```
t_loud_cstr   ACCEPTED   the char[8] member read as a cstr: [abcdefghAAAA]
```

Twelve bytes out of an eight-byte array: the `AAAA` is the adjacent `tail`
field, `0x41414141`, which the header sets on purpose so the over-read prints
its own proof. That is an out-of-bounds read the checker accepted, and it is the
escape hatch route 4 pushes binding authors onto. Route 4 is therefore a refusal
whose workaround is unsound and whose alternative is C code left lying around,
which `.claude/rules/c-boundary.md` names as the failure the FFI exists to
prevent. **I refuse it. It is not a price.**

## 5. The ABI question, so the panel need not wonder: no route touches it

A group's `record` IS the header's struct, and the emitted C uses the header's
own type. From `--emit-c` of the golden case:

```c
    Tag t8;
    t9 = tag_first(t8);
```

No marshalling, no boxing, no copy through a Heroes representation — the Lua
lesson does not apply here. The proof that the element's Heroes spelling is not
an ABI question is in § 1's last row: in the `i8` build and in the route-3 `u8`
build of the same program, **what C itself sees is identical** (`-56` on the two
signed machines, `200` on Debian arm64) while what Heroes reads differs. The
sign spelling is a READ, not a layout. **No veto on ABI grounds is available
against any of the four routes, and I do not raise one.**

Emission stays target-independent under route 3, because the relaxed row is a
constant string: the same source emits byte-identical C on the Mac and on
Debian x86-64 (`sha256` head `077797bc22cd4040` on both).

## 6. The shapes beside it (CL-061) — which invert, and the three that do not

`probe/gen.sh` writes one program per shape per spelling; `probe/all.sh` runs
them. Full outputs in the scratchpad (`out-arm64.txt`, `out-amd64.txt`).

**Invert** (accepted at `i8` on the two signed machines, at `u8` on Debian
arm64, refused at the other): scalar field · array field · field in a **nested**
record · field in a **`partial`** record · function parameter · function result
· **`const char` parameter**. Seven shapes, `ffi_field_type`,
`ffi_parameter_type` and `ffi_return_type` respectively. The brief's eight rows
reproduce exactly.

**Do NOT invert** — and these are the findings:

1. **`const char[N]` fields are refused at every spelling on every machine**,
   and it is the `const`, not the `char`: `const signed char s[4]` as `i8[4]`
   and `const int32_t n[4]` as `i32[4]` are both `ffi_field_type` on all three
   machines, while a `const int32_t` **scalar** binds. So a resolution that
   fixes only the sign does not make a `const char[N]` field bindable, and a
   route argued as "this makes headers portable" must not count them.
2. **A record with any `const` member cannot cross the boundary by value**, and
   the failure is `internal error: compiling the generated C failed`, **exit
   2** — the compiler taking the blame for the author's `extern`, which
   `.claude/rules/c-boundary.md` § A clang failure that the author's own extern
   caused says should be exit 1 on the `.hero` line. Verbatim, on all three
   machines: `error: cannot assign to variable 't1' with const-qualified data
   member 'c'`. It inverts only in which *spelling* provokes it.
3. **A `char **` out-parameter never asks the sign question at all**:
   `@out: cstr` is `ffi_writable_parameter` on all three machines (the guard is
   about writes, not sign) and `@out: ptr` is accepted on all three. This
   position is already portable and no route need touch it.
4. **A `partial` record that omits the plain-`char` field binds everywhere**
   (`s_partial_omitted` reads `1 3` on all three) — today's only portable
   escape, and it is exactly the one `struct utsname` cannot use.
5. **`char[8]` bound as `cstr` is accepted everywhere and reads out of bounds**
   (§ 4). Does not invert; is a defect.
6. A `char[N]` field bound as `cstr` inside a **non-partial** record is refused
   with a diagnostic that **names a field the program did name**:
   `error[ffi_incomplete_record]: SEight does not name id` for a record whose
   source reads `buf: cstr` then `id: i32`. Brace elision in the completeness
   probe, the same shape as panel 081's defect (1), one position over. Same on
   all three machines.

## 7. Defects 058 and 059, reproduced on the machine that has them

Debian arm64, verbatim:

```
error[ffi_parameter_type]: `c` of `take_char` is declared a different sign from
the header's `char` — clang read the header, and C would convert the value in silence
  2 |     function take_char(c: i8) -> i32
  note: … Declare it `i8`, and convert at the call where the value is known to fit
  fix (guess): declare it `i8`
```

Defect 059 is present at the **parameter** position with the fix text, and the
**result** position gives `ffi_return_type` with no spelling advice at all. The
shipped golden case is `ffi_field_type` at `:35:9`.

**Under route 3 the shipped golden case passes on Debian arm64 unchanged.**
Measured: its `u8` twin emitted, rewritten back to `i8` with the field assertion
dropped, compiled and run there — output **byte-identical to the committed
`.expected`** (`68` / `68 7`).

## 8. A fifth route, named because naming four is not a claim that four is the set

The walk says **what the plain-`char` arrays actually hold is text**: `d_name`,
`utsname`'s six, `ut_user`, `ut_host`, `ifrn_name`, `pr_fname`, `pr_psargs`,
raylib's two `name[32]`. A handful are bytes rather than text (`sa_data[14]`,
`ip_opts[40]`, `fs_cdhash[20]`, the `__ss_padding` family). Reading a text one
in Heroes today means 65 or 256 element reads, and the one-token shortcut —
bind it as `cstr` — is § 4's out-of-bounds read.

So: **a fixed-length text field spelling**, `char[N]` bound as a bounded text
field that copies at most N bytes and stops at the first NUL. It is sign-free,
because text is bytes; it closes the over-read; and it removes the loop. **It
does not replace the sitting's answer** — `sa_data`, the four `elf_prpsinfo`
scalars and the two plain-`char` parameters are numbers, not text, and still
need a sign rule. I name it as the follow-on route 3 leaves open, with its
measured basis, not as a substitute.

And on portability, honestly: `struct utsname` is `char[65]` on Debian and
`char[256]` on Darwin (measured on both), so **no route in this sitting makes it
portable**. What route 3 makes portable, measured: `dirent.d_name` (256 on both
Debian legs), `ifreq.ifrn_name`, `utmpx`'s four, `elf_prpsinfo`'s four scalars,
raylib's two, `sqlite3_str_appendchar`, `TextSplit`, and this repository's own
golden case.

## 9. Prediction, falsifiable, with its milestone

At the close of **M-arm-platform**, with route 3 implemented at all four
positions:

1. `tests/golden/run/ffi-a-char-array-member.hero` passes on Debian arm64
   **unchanged**, and the net there reads **1828 passed, 0 failed** — the 1825
   the brief measured plus the three that suite, `determinism` and `emission`
   currently fail. *(Already measured at the C level: § 7.)*
2. `probe/s_controls.hero`'s equivalent — any binding of a `signed char` or
   `unsigned char` field — is **unaffected on all four legs**. Under route 1
   this prediction is FALSE on Debian arm64 for `signed char`, and § 3 is the
   compile that says so.
3. **SQLite step 3 of §4.19's ladder needs no shim under route 3**:
   `sqlite3_str_appendchar`'s `char C` parameter binds as `i8` or `u8` on all
   four legs from one source file. Under route 4 it needs a C shim.
4. A binding author who wants `elf_prpsinfo.pr_nice` to read `-20` gets it on
   every leg under route 3, and on no unsigned-`char` leg under routes 1, 2
   or the status quo.

Falsified if any of the four fails to hold on the arm64 leg of CI.

## 10. Condition

My approval of route 3 is conditional on all three positions moving together —
the array row (`extern_field.hero:154`), the scalar sign test
(`unsignedness_of`, `:261`) and the `HERO_RET_*` macros
(`extern_assert.hero:72-78`) — plus `c_spellings.hero:59`, which is defect 059
and must stop tabling `i8` and start naming both. `probe/route3_ret.c` is the
compiled proof that the macro form works; a repair that lands the array row
alone ships half of it, and § 1's `elf_prpsinfo` measurement is the header that
proves scalars are not hypothetical.

I withdraw the objection to route 1 if its row is rewritten to dispatch on
**type identity** (`char (*)[N]: 1` and nothing else) rather than on width and
sign — that removes § 3's collapse. It would then differ from route 3 only in
what the program READS, and § 1 is the measurement of that difference: one
number on four legs, or two.

I withdraw the veto on route 4 if somebody measures a workaround for
`dirent.d_name` that is neither a C shim nor § 4's out-of-bounds `cstr`.
