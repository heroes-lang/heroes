# 055 — Where a header is, when the machine cannot say

**Status**: `ratified — 2026-08-14, author decision`.
**Convened** 2026-08-14, by author decision in `/decide` (item 5, answer `a`:
*add search paths, with the shape decided in panel*).
**Lane**: full, five judges, all reporting, all building in copies.

**The sitting inverted twice before it reached a verdict**, and both inversions
were measurements rather than arguments. It was convened to decide whether to
**add** a way to name a search path. It found that one **already exists**, unnamed;
and then that the language's other door is **shut for reasons this project
built**.

## The proposal, verbatim

**A** a path in the group head · **B** nothing in the language; the machine is
asked · **C** `--include`/`--library` on the argv table · **D** wait, because
`package` may have met the need.

## The verdict table

| judge | A | B | C | D | rests on | prediction |
|---|---|---|---|---|---|---|
| llm-ergonomist | **veto** | adopt-with-condition | object | retained | § FFI line 202, and locality: one clang invocation means one `-I` set, so a path in group 1's head changes how group 2's header resolves | under the status quo ≥40% of first tries put a path inside the header or `link` string; with two sentences, <10% |
| spec-warden | **veto** (+29 floor, not the +28 assumed) | approve at **0** | **refuse** under §10 | **approve — the answer** | §1.0, §1.6, §10, §12 | `SPEC_TOKENS` after this sitting is **≤3200**; if it rises, the sitting bought grammar it did not need |
| ffi-pragmatist | **object**, on measurement | approve as complement, object as the only answer | **approve**, built | **object — its premise is falsified today** | §1.11, §4.19's ladder | `extern "SDL2/SDL.h" package "sdl2"` fails on all three CI legs, on a `-D` flag, with no Heroes change |
| compiler-engineer | **veto** — **+222/−34 across 14 files**, reaching the lexer | **approve — 1 line, 1 file** | object | approve | §1.7, Part 5, §10, §11 | if A lands, its diff shows ≥150 added lines across ≥6 modules, one of them `lexer/keywords.rs` |
| historian | object | approve, narrowed to *absolute* only | object | **no longer available** — "wait" now ships an unnamed capability | precedent | `link ":/abs/path/libfoo.a"` builds on the Linux leg and fails on Darwin |

## The first inversion: option A already exists

The blind reader inferred it from the spec alone — *"a group names its header"*
never says the string is a C **include spelling** rather than a filesystem path —
and it is true:

```
extern "/tmp/absh/foo.h"
    constant FOO_OK: i64
```

`heroes run` prints `7`, exit 0. So the queued item was never *add a path slot*;
it was *a path slot exists, unnamed and unspecified — keep it, name it, or refuse
it*.

**The relative form is good and must survive.** `extern "sub/bar.h"` resolves
against the `.hero` file's own directory (panel 036's `-I <source dir>`), verified
from two working directories. So the refusal is narrowed to **absolute** spellings
— leading `/`, leading `\`, a drive letter — and never to "a path", because
`curl/curl.h` is path-shaped and correct. *"This string begins with a filesystem
root"* is a fact about the value; *"nobody needs a relative path"* would be a
premise about the world (CLAUDE.md §11).

**It is inherited, not designed.** Nim's `header` pragma has exactly this
permissiveness, and CLAUDE.md §6 says copy Nim's surface. §6 working and biting in
the same act.

**And the same hole exists for `link`, where it is worse.** `toolchain.rs`
prefixes blindly with `-l{library}`, and GNU `ld` reads `-l:filename` as *a file*,
so `link ":/opt/foo/lib/libfoo.a"` names an arbitrary file on the legs whose
linker supports it. The driver's own comment — *"no `.hero` file can hand clang an
arbitrary argument"* — is true about **flags** and false about **files**: another
premise about the world, in a comment, going on reading as correct.

## The second inversion: the other door is shut, and this project shut it

The ffi-pragmatist bound **SDL2**, and reproduced on the repository:

```
extern "SDL2/SDL.h" package "sdl2"   → error[ffi_package]: … answered with `-D_THREAD_SAFE`,
                                        which this compiler does not pass on
extern "SDL2/SDL.h" link "SDL2"      → error[ffi_missing_header]: `SDL2/SDL.h` is not on
                                        this machine's include path
```

**Each diagnostic sends the author to the other, and there is no third door.**
Counted on this machine: **28 of 285 `.pc` files (10%)** answer with a flag panel
050's allow-list rejects — `sdl2`, `sdl3`, `ncurses`, `readline`, `x264`,
`simdjson`, and all twelve `Qt6*`. And **38% of `/opt/homebrew/lib`'s link names
have no `.pc` at all**, among them snappy, harfbuzz-icu and freebl.

**Panel 050 cited Go's allow-list as its precedent and then adopted a narrower
list than Go's.** Go documents *"only a limited set of flags are allowed, notably
`-D`, `-U`, `-I`, and `-l`"*. `-D` and `-U` define and undefine a preprocessor
macro; neither can load a plugin, write a file or name one. Their absence is what
shuts the door on SDL2, and it is the cheapest repair in the sitting.

## The composition, measured — and why it decides C

Three of them, all live because `Command` inherits the environment:

```
CPATH=…/include heroes run x.hero              → 42, exit 0
C_INCLUDE_PATH=…/include heroes run x.hero     → 42, exit 0
CPATH=… LIBRARY_PATH=…/lib heroes run x.hero   → 42, exit 0   (a real .a)
PKG_CONFIG_PATH=…/pkgconfig heroes run x.hero  → 42, exit 0   (a hand-written .pc)
```

CLAUDE.md §10's third clause — *nothing if two existing invocations already
compose to it* — yields **nothing**, and this repository proved the channel live
by accident a sitting ago: panel 049 records `LIBRARY_PATH` invisibly supplying a
`-L` and contaminating a measurement, caught only under `env -i`.

**The ffi-pragmatist dissents, and the dissent is not smoothed away.** It built C
— **+105/−6, 75 code lines, zero spec tokens, 549 tests green** — found and fixed
a defect in its own prototype (search paths absent from the cache key, so two
configurations shared one artifact), and argued that an environment variable is
not an answer a *program* can carry: asked for "a program that binds SDL2", a
model produces an artifact that cannot be built, and the missing half is an
unversioned shell line. Three judges answer that the machine's configuration is
where a machine's answer belongs — the same argument panel 049 made about the
platform axis and panel 050 made about `package`.

## What §1.12 decides, and it decides against both A and C

The engineer measured the failure mode this class of feature actually has:

```
CPATH=v1/include LIBRARY_PATH=v2/lib heroes run skew.hero
12
exit=0
```

Header v1 declares `int64_t skew_value(int64_t)`; the library at v2 defines
`double skew_value(double)`. **Compiles clean, links clean, exit 0, wrong number,
no diagnostic.** §4.19's `#include` checks the declaration against the *header*;
nothing checks that the library is the same build.

**And that is the discriminator.** `package` asks one question and gets `-I` and
`-L` from one `.pc`, written by one install — it is **the shape that cannot
skew**. A and C both let the two be named independently, which is the only way to
produce the skew. design.md §1.12: *where two admissible forms disagree and one of
them can be made to crash, the other wins.* Lying is worse than crashing, and this
one lies.

## A third thing, found while costing A, and it is a defect in design.md

design.md:2100 says both paths would be *"validated not to begin with `-`, so the
form stays closed rather than becoming an arbitrary-flag hole."* **A `-` check is
not the whole check.** clang expands `@file` response files *before* it parses a
single option:

```
$ cat two.rsp
/tmp -DSMUGGLED=1
$ clang -I "@two.rsp" s.c -o s.out     # s.c is `#error` unless SMUGGLED is defined
$ ls s.out                              # it exists
```

`-###` shows the splice. That is CVE-2018-6574's class arriving through the one
door a `-` check cannot see. **Panel 050's allow-list holds against it** — a `.pc`
answering `@/…/two.rsp` is `ffi_package` at exit 1 — but the *rule design.md
states* does not, and the document is wrong rather than silent.

## Resolution — provisional, author ratification pending

**A is refused**, four judges to one objection, and on four independent grounds:
it costs **+222/−34 across 14 files** and reaches the **lexer**, where it must
evict `include`'s reserved-word diagnostic and its `certain` fix; `heroes fmt`
would **merge two groups with different paths**, which is the defect class
M-program-corpus already paid for once; **219 of 273** `.pc` include answers embed
a version number, so a path in a source file is stale at the next `brew upgrade`;
and it is the shape that can skew.

**C is queued, not refused**, and the split is recorded because it is real: §10's
composition test says *nothing*, and the judge who built it says an environment
variable is not an artifact. That is the author's call.

**Landed, all at or near zero cost:**

1. **`ffi_missing_header`'s note names the composition.** It said *"point the
   compiler at them"* and named nothing — one line, and it is the whole of what a
   reader needed.
2. **`-D` and `-U` join the allow-list**, on Go's own precedent, which panel 050
   cited for the list's existence and then under-copied. It opens SDL2, ncurses,
   readline and 25 more.
3. **An absolute header string is refused**, in `syntax/externs.rs` where the
   string is already in hand, narrowed to absolute spellings only.
4. **`link` gets the same refusal in the same commit**, because the historian's
   measurement says a path there is worse.
5. **`ld: library 'X' not found` becomes the sixth named exception** — exit 1 on
   the author's line where the group did write `link "X"`, instead of exit 2 and
   the compiler blaming itself for the author's machine. Two judges found it
   independently.
6. **design.md:2100's validation rule is repaired** with the `@file` finding, and
   the premise it rests on is written as a falsifiable claim with a test.

**Funding**: the spec-warden's removal, and it is a §12 repair rather than a
saving. `spec:149`'s operator fence still reads *"(i64 with i64, f64 with f64 —
never mixed)"* — false since panel 042 landed the eight widths, and false in the
**silent** direction: `u8 200 + u8 55` prints `255` and `i32 7 * i32 6` prints
`42`, both at exit 0. **−17**, against the sitting's **+6**.

## Predictions to score

| # | judge | prediction | scored at |
|---|---|---|---|
| 1 | historian | `link ":/abs/path/libfoo.a"` builds on the **Linux** leg and fails on Darwin, proving the driver's *"no `.hero` file can hand clang an arbitrary argument"* false on one of three legs | next CI run with the case |
| 2 | ffi-pragmatist | `extern "SDL2/SDL.h" package "sdl2"` fails on all three legs on a `-D` flag **without** any Heroes change — `-D_THREAD_SAFE` on macOS, `-D_REENTRANT` on Linux, both from SDL's own `.pc` | next corpus run |
| 3 | compiler-engineer | if A ever lands, its diff shows ≥150 added code lines across ≥6 modules of `crates/heroes/src`, one of them `lexer/keywords.rs` | A's landing, or M-selfhost-probe |
| 4 | spec-warden | at M-ffi-ladder rung 5's close, `grep` over every `.hero` returns **0** header strings beginning with `/` and **≥1** containing `/` (today: 0 and 1) | M-ffi-ladder rung 5 |
| 5 | llm-ergonomist | under the status quo ≥40% of first-try bindings of an off-path library put a filesystem path inside the header or `link` string; with the refusal and the named composition, <10% | next harness run |

## Ratification — 2026-08-14, by author decision

**RATIFIED as it stands.** A stays refused, C stays queued with its split intact,
the six landed items stay, and design.md:2100 stays repaired.

What the yes settles: **a sitting may discover that its own question was the wrong
one, twice, and that is a result rather than a failure.** It was convened to add a
way to name a search path. It found one already present and unnamed — inherited
from Nim's `header` pragma, which §6 says to copy, so §6 worked and bit in one act
— and then found the language's other door shut by this project's own allow-list,
which cited Go's precedent and adopted a narrower list than Go's. SDL2, one of the
most-bound C libraries there is, was reachable through neither clause.

It settles the general form of the refusal, which will outlive this feature:
**`package` is the shape that cannot skew.** It asks one question of one `.pc`
written by one install, so `-I` and `-L` cannot disagree; A and C both let them be
named independently, and independently named paths compile clean, link clean, exit
0 and print the wrong number. Under §1.12 a form that lies loses to one that
cannot, whatever it costs in tokens or convenience.

**The `--include` split is not resolved by this ratification and is not meant to
be.** Three judges hold that §10's composition test yields nothing; the judge who
built it holds that an environment variable is not an artifact a program can
carry. It stays in `DECIDE.md` as one question with two defensible answers.
