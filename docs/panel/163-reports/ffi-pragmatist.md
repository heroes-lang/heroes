# Panel 163 — ffi-pragmatist

**verdict: veto** — on **route 3** (an out-parameter that does not require an
initialised value), as a refusal and not a price. **object** to routes 1 and 2,
because they buy a capability the language already has. **approve route 4**, with
one amendment: it is not a refusal, because **question 1 answers YES** and the
program the refusal owed an answer for already builds and runs.

**section**: design.md **§4.19** — *"Macros and `inline` functions are reachable
because the C compiler sees the real header"* — with **§1.11**, **§1.12**
(robustness, CLAUDE.md § Precedence rank 3), `spec § 5` and `spec § 13`.

**Every command below was run on 2026-09-18 in a copy** of the tree
(`cp -r`, `rm -rf build`, `clang -I runtime seed/heroes.c runtime/runtime.c -o
heroes`). Nothing was rebuilt from `selfhost/`; `archive/bootstrap-rs/` was not
touched. Scratch root:
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/4cbbc5a6-a98e-4b44-80b5-b6957110cd39/scratchpad`.

---

## 1. The ten-minute experiment: it works, and the sitting costs nothing

**The real header first** (CL-062), `$(xcrun --show-sdk-path)/usr/include/sys/utsname.h`:
`#define _SYS_NAMELEN 256`, five `char[256]` fields, `int uname(struct utsname *);`.

The header I wrote, `e1/blank.h`:

```c
#include <string.h>
#include <sys/utsname.h>

static inline struct utsname hero_blank(void) {
    struct utsname u;
    memset(&u, 0, sizeof u);
    return u;
}
```

The binding, `e1/main.hero` — **one group, no second binary, no `.c` file, no
`heroes cc`, no `compile "shim.c"` clause**:

```
extern "blank.h"
    record Utsname tag utsname
        sysname: i8[256]
        nodename: i8[256]
        release: i8[256]
        version: i8[256]
        machine: i8[256]
    function hero_blank() -> Utsname
    function uname(@u: Utsname) -> i32

function main()
    u: Utsname @ hero_blank()
    r = uname(@u)
    print(r.to_i64().must())
    print(u.sysname.validated_bytes().must())
    print(u.machine.validated_bytes().must())
```

**What the compiler said, verbatim** (`./heroes run e1/main.hero`):

```
0
Darwin
arm64
EXIT=0
```

**Not one diagnostic.** The same program under `./heroes run --sanitize`
(ASan + UBSan, `.claude/rules/c-boundary.md`) prints the same three lines,
`EXIT=0`.

### The emitted C is what a C programmer would write

`heroes build --emit-c`, the body of `main`:

```c
    struct utsname h0_u;
    struct utsname t1;
    ...
    t1 = hero_blank();
    h0_u = t1;
    t2 = uname(&h0_u);
```

**No marshalling, no boxing, no copy through a runtime value** (the Lua lesson).
The return is C's own indirect-result convention for a 1280-byte struct, chosen
by clang from the real declaration; the out-parameter is `&h0_u`. The ABI is
untouched, which is why I have nothing to veto here.

### And `importc` verification survives on the producer

The emitter writes, at `e1/out.c:55`:

```c
_Static_assert(HERO_RET_RECORD(hero_blank(), struct utsname), "heroes-ffi-return hero_blank Utsname");
```

So the producer is verified against the header by exactly the mechanism §4.19
already uses for every other member. I attacked it three ways (`e6/`), and all
three are **compile errors**:

| what I wrote | what the compiler said |
|---|---|
| `function blank_mixed() -> Short` | `error[type_mismatch]: expected `Mixed`, found `Short`` |
| `function blank_nothing() -> Mixed` | `error[ffi_unknown_name]: `shapes.h` declares no `blank_nothing` — clang read the header and could not find it` |
| `name: i8[128]` against `char[256]` | `error[ffi_field_type]: `Mixed.name` is not `i8[128]` in `shapes.h` — clang read the header's struct and the field disagrees` |

**A wrong producer is a compile error. The thesis holds at the boundary.**

### The route is header-only, which is why §4.19's undecided shim question does not apply

design.md §4.19 defers **how a `.c` shim is compiled** (`heroes cc` versus a
`compile "shim.c"` clause, panel 036). That deferral does not reach this route:
`static inline` in a header needs no translation unit. The emitted C carries
`#include <blank.h>` and `heroes` already passes `-I <source_dir>`
(`selfhost/cli/units.hero:157`, `selfhost/cli/pointee.hero:283`), so a header
beside the `.hero` file is found with no new flag, no new clause and no new
decision. **This is landable today with zero compiler change.**

### This is not a new pattern — the repository already ships it, and tests it

Counted today: **25** `.h` files sit beside `.hero` programs in `tests/` and
`examples/`, and **23** of them contain a `static inline`. **Seven already return
a struct BY VALUE** from a header producer, and they are the project's own
regression cases:

```
tests/golden/fixedbugs/ffi-const-pointer.h:8            static inline Handle get_handle(void)
tests/golden/run/fixedbugs-a-handle-in-a-record-field.h:21   static inline Slot slot_make(int64_t n)
tests/golden/run/fixedbugs-the-unacquired-sibling-leaks-loudly.h:33  static inline Pair pair_open(int64_t n)
tests/golden/run/fixedbugs-sibling-fields-of-one-record-type.h:33    static inline Pair pair_open(int64_t n)
tests/golden/check/fixedbugs-a-handle-reached-through-a-field.h:48   static inline Pair pair_make(int64_t n)
tests/golden/check/fixedbugs-a-handle-reached-through-a-field.h:55   static inline Outer outer_make(int64_t n)
tests/golden/check/fixedbugs-a-handle-reached-through-a-field.h:62   static inline Four four_make(int64_t n)
```

**So the sitting is not weighing a new mechanism against three language changes.
It is weighing a mechanism the suite already exercises against three language
changes.** What was missing is the sentence saying it applies to a long array
field, which is what `spec § 13` should gain.

### The shapes beside it (CL-061) — all five, under `--sanitize`

`e5/shapes.h` + `e5/main.hero`, one program, `./heroes run --sanitize`, `EXIT=0`:

```
short literal still works: ABC
short from header: []
mixed fill: 0 filled 7
onlylong: [][]
nested fill: 0 deep 9
by value: 7
```

- short fixed array (`char[4]`) — **literal still works**, nothing is taken away;
- short array from a header producer — works;
- one long array + one scalar, filled through `@` — works;
- only long arrays — works;
- **nested record holding one**, filled through `@` — works;
- the same record **by value** (`read_mixed(m) -> i32`) — works.

**And a seventh shape the brief did not list**, `e7/`: a producer returning a
**`partial`** record. `record Broken tag tm partial` over the real
`<time.h>` — `struct tm` there carries `long tm_gmtoff` and `char *tm_zone`,
which is why `examples/ctime/main.hero` marks it `partial` — with
`static inline struct tm hero_blank_tm(void)`. Runs under `--sanitize`, prints
`0 0 0`, exit 0. So the producer zeroes C's whole 56 bytes, including the two
fields the program never names.

**I nearly reported that as an argument against routes 1 and 2 and it is false**
(CL-018). I checked instead of inferring: `Broken(tm_year: 125, tm_mon: 8,
tm_mday: 18)` emits `(struct tm){.tm_year = t1, .tm_mon = t2, .tm_mday = t3}`,
and C11 6.7.9p21 zeroes the unnamed members of a brace-initialised aggregate.
The literal route is sound for `partial` too. The comparison between route 0 and
routes 1/2 is therefore about **cost**, not soundness — and route 3 is the only
one that is unsound, for the reason in §3.

**The header-side producer covers every shape in the brief's list, plus
`partial`.** Routes 1, 2 and 3 each cover a subset of it and cost a language
change.

---

## 2. What real out-parameter APIs require — the class that kills route 3

The brief asks whether a callee READS a field the caller set. **It does, in the
most common out-parameter idiom POSIX has, and the failure is silent.**

Read from the real headers on this SDK:

- `sys/socket.h:708-720` — **five** entry points take a non-`const`
  `socklen_t *`: `accept`, `getpeername`, `getsockname`, `getsockopt`,
  `recvfrom`. The length cell is **input then output**.
- `sys/poll.h` — `struct pollfd { int fd; short events; short revents; }`: two
  input fields and one output field **in one struct**.
- `net/if.h:294` — `char ifr_name[IFNAMSIZ]` is input; `ifr_addr` is output.
- `netdb.h` — `struct addrinfo` hints: **eight** fields, all caller-supplied.
- `/opt/homebrew/include/raylib.h` — **48** entry points take `Image *` as their
  first argument (`ImageCrop`, `ImageResize`, `ImageFormat`, …) and every one is
  read-modify-write: the callee dereferences `image->data` before writing.
  `Mesh *`/`Model *`/`Wave *` add **5** more.

**Compiled and run** (`e2/rbw.c`, `e2/rbw2.c`, clang clean at `-Wall -Wextra`),
each call once with the field set and once with the bytes filled `0xAB`:

```
flock  initialised   r=0 errno=0 l_type=2
flock  garbage input r=-1 errno=22 (Invalid argument)
pollfd initialised   r=0 revents=0
pollfd garbage input r=0 errno=0 (Undefined error: 0)
ifreq  initialised   r=0 family=2
ifreq  garbage input r=-1 errno=6 (Device not configured)
hints  initialised   r=0 (ok)
hints  garbage input r=5 (ai_family not supported)

getsockname len=sizeof  r=0 len_out=16 family=2
getsockname len=0       r=0 len_out=16 family=0 errno=0
getsockname len=garbage r=0 len_out=16 errno=0 (no error)
getsockopt vl=sizeof    r=0 val=1 vl_out=4
getsockopt vl=0         r=0 val=-1 vl_out=0 errno=0
kevent     EV_SET       r=0
kevent     garbage      r=1 errno=0 (no error)
```

**Read the second block.** `getsockname` with the length cell at 0 returns
**success**, writes **nothing**, and leaves `sin_family` at 0. `getsockopt` the
same: `r=0`, and `val` keeps the caller's `-1`. `kevent` on garbage returns
`r=1` — **an event manufactured from uninitialised bytes**. `poll` on garbage
returns 0 with no error.

So the class is not merely large. **It fails by returning success with a wrong
answer**, which is the one failure mode this project exists to make impossible.
Route 3 would put that behind a form the language calls safe.

---

## 3. The finding that outranks everything else in this report

The brief says a route that compiles and reads uninitialised memory is the most
important thing I can return. Here it is, and it is why the veto is a refusal.

**`--sanitize` cannot see it.** `.claude/rules/c-boundary.md` defines `--sanitize`
as `-fsanitize=address,undefined`. Neither sanitizer instruments reads of
uninitialised stack memory. `e2/uninit.c`, compiled with exactly those flags:

```
read 1280 uninitialised bytes, 397 of them non-zero
first byte of sysname = 0
EXIT=0
```

**Silent. Exit 0.** And the sanitizer that would catch it does not exist here:

```
clang: error: unsupported option '-fsanitize=memory' for target 'arm64-apple-darwin25.6.0'
```

**The consequence, reproduced.** `e2/leak2.c`: a frame writes a secret, returns;
the next frame declares `struct utsname u;` with no initialiser and calls a C
function that fills **only `sysname`** — exactly what route 3 emits, and exactly
what `uname` would do on a kernel that filled fewer fields.

```
sysname  = Darwin
nodename = SSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
release  = SSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
EXIT=0
```

Identical output with and without `-fsanitize=address,undefined`. In Heroes,
`u.nodename.validated_bytes()` returns that as an ordinary `str` — the previous
frame's bytes, to the first zero or the whole field, with a `.ok` tag on them.
**Route 3 turns panel 162's landed feature into a stack-disclosure primitive**,
and the program has no way to know.

Add raylib: an uninitialised `Image` handed to `ImageCrop` dereferences a garbage
`void *data`. That is a segfault, and CLAUDE.md § Precedence rank 3 — *a Heroes
program must not segfault and must not corrupt memory* — is a goal of the
language, not a trade-off. **Refusal, not a price.**

---

## 4. The 803-character literal: it runs, and the emitted C is not what I expected

`e4/main.hero`, generated: 4290 bytes, longest line **787** characters (the
brief's 803 was one line; mine is five field lines). It builds and runs:

```
0
Darwin
EXIT=0
```

**The emitted C is the surprise.** Each of the 1280 elements becomes its own
stack temporary in `main`'s frame:

```c
    int8_t t43;
    int8_t t44;
    ...                      /* grep -c '^    int8_t t' → 1280 */
    t1286 = (struct utsname){.sysname = {t1, t2, t3, ..., t256}, ...};
```

Counted: **1280 `int8_t` temporaries** in the literal route, **0** in the
header-producer route. `out.c` is **411672 bytes / 5542 lines** against
**185218 / 1780**. That single compound-literal line is **7951 characters**.

**clang's time is not a cost**, and I say so because I measured it rather than
assumed it: `/usr/bin/time -p clang -c out.c -o /dev/null` reads `real 0.10`
for the literal route and `real 0.12` for the producer route. The cost is the
**shape**, not the clock: O(N) stack slots per fixed-array literal, in the
caller's frame, for a value that is semantically a constant.

---

## 5. What each route costs to IMPLEMENT, and where it lands

| route | compiler change | file | my verdict |
|---|---|---|---|
| 0. header-side `static inline` producer | **none** | none — a sentence in `spec § 13` | **take it** |
| 1. a call typed by context | a checking direction for `.call` (6 context sites today are all literals) + a third producer in `selfhost/emit/storageless.hero`, whose own doc refuses `.call` *on purpose* because UBSan's array-bounds check needs the C type to stay `T[N]` | `selfhost/check/walk.hero`, `selfhost/emit/storageless.hero` | object |
| 2. zero default for a fixed field | amends design.md §4.9 (*no default values*) for one field kind; does not reach `struct Image`, where zero is a null `data` pointer | `selfhost/check/` + §4.9 | object |
| 3. uninitialised out-parameter | amends `spec § 5`; **and §2 and §3 above** | — | **veto** |
| 4. refuse | none | §4.19 | approve, amended: it is not a refusal |

Route 0 is not a fifth route invented to win an argument. It is **the route
design.md §4.19 already names**: *"Macros and `inline` functions are reachable
because the C compiler sees the real header."* Nobody had tried it.

**Where the document does not cover my objection, explicitly**: §4.19 says a
shim is for *"C++ and awkward struct-passing"* and leaves shim **compilation**
undecided. It does not say a header-only `static inline` needs no such decision.
That sentence is missing and this sitting should write it.

---

## 6. The one honest cost of route 0, stated rather than buried

The author writes three lines of C. That is a real ergonomic cost and I am not
pretending otherwise. Three things make it the right price:

- §1.11 already says everything a real program needs comes from C. A producer
  for a C struct is C.
- It is **verified**: the `_Static_assert` above means a wrong producer is a
  compile error, which no language-side route improves on.
- The struct needs a per-platform source file anyway (`char[65]` Debian,
  `char[256]` Darwin — the shared brief's own measurement), and a header is
  exactly where `#if` lives. **Route 0 is the only route that makes the
  per-platform half easier instead of duplicating it in `.hero` source.**

---

## experiment (summary)

Wrote `static inline struct utsname hero_blank(void)` over the real
`<sys/utsname.h>`; bound it in one `extern` group with `uname`; ran
`u: Utsname @ hero_blank()` then `uname(@u)`. **clang accepted it and the
program printed `0 / Darwin / arm64`, exit 0, and the same under `--sanitize`.**
Emitted C: `t1 = hero_blank(); h0_u = t1; t2 = uname(&h0_u);` — no marshalling.
Also compiled `e2/rbw.c`, `e2/rbw2.c`, `e2/uninit.c`, `e2/leak2.c` (all clean at
`-Wall -Wextra`) and five shapes in `e5/`.

## argument (≤120 words)

Question 1 answers yes. A group function returning the record already compiles,
runs, sanitizes clean, emits the C a human would write, and keeps `importc`
verification — measured, not argued. So routes 1 and 2 buy a capability the
language has, and the sitting's whole premise is false. Route 3 is worse than a
language-rule violation: POSIX's commonest out-parameter idiom is value-result,
and I measured `getsockname` and `getsockopt` returning **success while writing
nothing**, `kevent` manufacturing an event from `0xAB`, and 48 raylib entry
points dereferencing `Image *` before writing it. Uninitialised bytes flow
straight into `validated_bytes()` as a `str`, and ASan+UBSan see none of it;
MSan does not build for arm64-apple-darwin. Refusal, not a price.

## prediction (falsifiable)

**SQLite, rung 3 of §4.19's ladder, needs no producer at all under this rule and
`examples/sqlite/main.hero` gains zero lines** — its caller-owned types are
opaque handles, not structs the caller builds. **Rung 5, raylib, needs no
producer either**: every caller-owned raylib struct is returned by a raylib
function (`LoadImage`, `LoadTexture`, `GetMousePosition`), so
`examples/raylib/main.hero` gains zero lines. Neither was run; both are cheap to
check and I am naming the file so they can be.

The third one I did run, so it is a measurement and not a prediction: the group
`function hero_blank_tm() -> Broken` beside
`function gmtime_r(t: ptr, @out: Broken) -> ptr`, over the real `<time.h>`,
**verifies against the header and the producer runs, exit 0 under `--sanitize`**
— the reentrant `struct tm` fill that `examples/ctime/main.hero` cannot write
today. I did not call `gmtime_r` itself, which needs a `time_t *`, and I say so
rather than let the sentence imply it.

Falsifier for all three: a library entry point on the ladder or in `examples/`
that fills a caller-owned struct which the library does not itself return and
which no `static inline` in a header can produce — for instance one taking a
struct by value to a **variadic** function, where `HERO_RET_RECORD`'s `_Generic`
probe cannot reach.

## condition

I withdraw the veto on route 3 if someone demonstrates, on this machine, an
instrument in the `heroes` toolchain that turns the `e2/leak2.c` output above
into a diagnostic or an abort. Given `-fsanitize=memory` does not build for
arm64-apple-darwin, that means a compiler-emitted initialisation — at which
point route 3 has become route 2 and should be argued as route 2.

I withdraw the objection to routes 1 and 2 if a named binding is produced that
route 0 cannot reach: a caller-owned struct with a long array field that no
header-side `static inline` can produce. I looked and did not find one, and that
is a claim about my vocabulary, not about the world (CL-018).
