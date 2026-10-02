# Panel 186, the completeness critic's first pass: the briefs against the frozen tree

Written 2026-10-02 by the completeness critic, before any seat is launched.
Inputs: `docs/panel/186-briefs/00-shared.md`, `compiler-engineer.md`,
`ffi-pragmatist.md`, `completeness-critic.md` and the eight files of
`probes/`. Every command below ran in `<scratchpad>/186-critic/`, a
`git archive ae08ed93 | tar -x` copy (no `.claude/worktrees` in it, checked
with `ls .claude`), with a compiler built inside it from the seed
(`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, `heroes
0.2.0`), `HEROES_RUNTIME` set to the copy's `runtime/`, unless the line says
it was a read-only `git` or `gh` query. No paid run, at most three processes,
nothing timed. Case files I wrote are in `<scratchpad>/186-critic/work/`;
the ones a seat should get are quoted whole below, since the scratchpad goes
away with the session.

## The repairs the briefs need, most important first

1. **Q1 is framed as "two declared fields that overlap", and the same wrong
   answer is reachable with ONE declared field** (§ 1 below, measured): a
   struct holding an anonymous union bound by one arm answers `==` with
   `true` for two C values that differ, where the same union bound by one
   member is `ffi_union_field`. Neither (1a) nor (1b) can see it. Q1 must be
   reworded to *a declared field that lies in a union, at any depth*, and
   the seats asked whether `==`/`hash` on such a record follows panel 077's
   any-arity rule.
2. **The route defect 151 itself names is not in the brief** (§ 2):
   `docs/work/DEFECTS.md:886` says *the lane's route, the header's layout
   read from clang*; the compiler already asks clang for a header's AST in
   `build` (`selfhost/cli/header_types.hero`, panel 103), and clang's
   record layout dump shows `SA`'s anonymous union with its arms (measured,
   § 2). Of the routes listed or found here, it and a broad refusal of
   `==`/`hash` are the two that can see repair 1's case (searched: C
   expressions over `offsetof`, `sizeof`, `__builtin_classify_type` and
   initializer warnings). List it as (1e) with panel 103's constraint (the
   dump gives text, never the verdict).
3. **The platform list omits the two clangs the CI judges with** (§ 3,
   measured from run 36939966148's own job logs): Ubuntu clang **18.1.3** on
   both Linux legs and clang **20.1.8** on the CI's Windows leg. A route
   resting on a warning's meaning must be read on 18.1.3 above all, whose
   major is the floor itself. This Mac also has Homebrew clang 22.1.8, usable by a
   seat locally.
4. **Route (2a)'s premise is false on both clangs of this Mac** (§ 4,
   measured): under `-Weverything`, in C, neither Apple clang 21 nor Homebrew
   clang 22.1.8 reports a field a designated initializer leaves out. Q2 must
   ask whether any clang does, not *under whichever warning reports it*.
5. **Q2 attributes to panel 073 a sentence panel 073 does not contain, and
   whose opposite it records** (§ 5): *naming one member of a union is naming
   all of it* is `selfhost/emit/extern_record.hero:91-92`, a code comment;
   panel 073's § Two holes says such a record *claims completeness falsely*.
6. **The brief omits that panel 073's detector caught `SA`'s shape and panel
   077's replacement lost it** (§ 6): panel 073 line 76, *the anonymous union
   promoted into a struct (`8 >= 12`, fires)*. Defect 151 is, on the record,
   a coverage regression of 2026-08-16's swap (panel 077 sat three hours
   after 073; `grep -n -i anonymous` over its file finds no hit, so it never
   measured the shape it dropped), and the sum-of-sizes form
   (conjoined with the classify form) is a route nobody listed.
7. **The escape every route must keep does not exist for `SA`** (§ 7,
   measured): a second record over a typedef'd anonymous struct by `tag` is
   `ffi_unknown_tag`, and construction names every declared field
   (`missing_fields`). So a refusal of construction over `{kind, i, f, x}`
   leaves no way, in one program, to build an `SA` holding `f` and to read
   `i` from an `SA` that C returns. Hand it to the seats as a question, not
   as panel 073's escape.
8. **The soundness-lane premise should go out as a question** (§ 8): spec
   § 13's *all its fields* is the sentence behind the question (for `SA` it
   tells the reader to name all four, which the route refuses), the spec
   says `union` nowhere, and `ffi_union_field`'s text (*`T` is a `union` in
   ...*) is false for `SA`, so the route needs at least new message text.
   SKILL.md § Two lanes: *a question that looks internal but has a sentence
   in the spec behind it is a full panel*.
9. **(1b) refuses a correct read-only program unless it is gated** (§ 9,
   measured): `W v = {.i = 0, .n = 0};` under
   `-Werror=initializer-overrides` is an error, and `read.hero` (defect 150)
   is the program it would refuse. Say in Q3 that the probe's reach (every
   record, or only `used` ones) is part of the question.
10. **A bit-field member is exit 2 today, with clang's text** (§ 10,
    measured, unfiled): the brief lists the shape without saying so. File it
    as a defect beside 151, or hand it to the seats as today's behaviour.
11. **Smaller factual repairs** (§ 11): the `used` line range, the trigger
    set (map key, construction at two or more fields), the provenance of
    `clang-2100.1.1.101`, the *no clang text reaches the author* citation,
    the census comparing exit code and stderr as well as the C, the census's
    composition and its baseline (§ 12), the module sizes in the
    instrument's unit, the `(0)` rule, defect 062 as u19's precedent, and
    the probes directory carrying 4 of the lane's 18 cases.

Status: complete. First pass only; the second pass waits for the seats'
reports.

## 1. The one-arm shape that Q1's framing excludes

Q1 asks how `build` learns that **two fields a record declares** overlap.
The lane's own case u08 (`SA` naming `kind`, `i`, `x`) is complete under
today's probe and correct to read and build (`work/u08_mine.hero`, the same
record, `SA(kind: 1, i: 7, x: 3).i` and `make_sa().i`: silent build, prints
7 and 12, exit 0). But panel 077 refuses `==`,
`hash` and a map key on a union record **at any arity**, because *one
declared member still reads one arm's bytes out of a value that may hold
another* (`selfhost/emit/extern_union.hero:45-50`). The same reasoning
reaches a struct holding an anonymous union bound by one arm, and nothing
refuses it. Measured, `work/one_arm.h`:

```c
#include <stdint.h>
typedef struct { int32_t kind; union { int8_t b; int64_t q; }; } SB;
typedef union { int8_t b; int64_t q; } UB;
static inline SB make_a(void) { SB s; s.kind = 1; s.q = 0x100; return s; }
static inline SB make_b(void) { SB s; s.kind = 1; s.q = 0x200; return s; }
static inline UB make_ua(void) { UB u; u.q = 0x100; return u; }
static inline UB make_ub(void) { UB u; u.q = 0x200; return u; }
```

`one_arm.hero`, `record SB` naming `kind: i32` and `b: i8`, `print(make_a()
== make_b())`: `build` exit 0, **prints `true`**, exit 0, no warning.
`one_arm_union.hero`, `record UB` naming `b: i8`, `print(make_ua() ==
make_ub())`: exit 1, *error[ffi_union_field]: `UB` is a `union` in
`one_arm.h`, so comparing or hashing one asks about bytes that may hold
another member*. The two `SB` values differ in their union's byte 1 (`q` is
0x100 against 0x200; `b`, byte 0, is 0 in both); the struct form says
equal, the union form is refused.

No two declared fields of `SB` overlap, so (1a)'s ranges and (1b)'s
overrides are silent by construction. **What can see it** is a question for
the seats; the candidates found here: the header's layout read from clang
(§ 2), or a broad refusal of `==`/`hash` on group records (panel 077's
historian predicted zero programs compare two `extern` records; that is a
census the seats can run, not a fact). The shared brief should name this
shape beside u17 and u18, and Q1 should read *a declared field that lies in
a union, at any depth*.

## 2. The route the defect names, and the precedent for it

`docs/work/DEFECTS.md:886`, defect 151's body: *The lane's route, the
header's layout read from clang, changes the union rule of panels 060 to
077, so the repair is a sitting's (panel 186).* The brief's Q1 lists (1a) to
(1d) and not this. The compiler already does the same kind of thing in
`build`: `selfhost/cli/header_types.hero:7-16` writes a unit including the
program's headers and runs `clang -fsyntax-only -Xclang -ast-dump=json
-Xclang -ast-dump-filter=hero_ty_`, read *by its shape, not by a JSON
parser* (lines 18-23), and panel 103 (`docs/panel/103-the-ffi-pointer-verdict-goes-strict.md:142-146`)
fixed the constraint: *the verdict is not read from the JSON: it is asked
of clang in* C assertions. `selfhost/cli/clang_floor.hero:28-30` says the
dump shape is the same on 18.1.8, 21.0.0 and 22.1.8. Panel 077's
compiler-engineer ruled that union-ness *is not in the `.hero` source*, so
moving it into the checker means a C front end in the checker
(`docs/panel/077-the-builtin-that-was-already-there.md:43`): the route lives
in `build`, as the pointee check does.

What the route would answer that no C-expression route does: which declared
fields share an anonymous member (Q1, at any depth, including § 1's one-arm
case), the header's top-level members in order (Q2: the field left out, and
its NAME for u19), and whether a member is a bit-field (§ 10). Its unknowns,
for the seats: whether the AST dump carries the anonymous members in a shape
the reader can hold to (`RecordDecl` with `"isImplicit"` / an anonymous
`FieldDecl`), whether `-Xclang -fdump-record-layouts` is the better source
(it prints offsets), the process cost per build, and whether panel 103's
*text, never the verdict* can be kept (the verdict could still be a
positional probe written in C's grouping, so clang judges it). Unbuilt.

One measurement, so the route is not a hope: `work/c/lay.c` (`#include
"u.h"`, `#include "one_arm.h"`, `SA hero_l_SA; SB hero_l_SB;`) under
`/usr/bin/clang -std=gnu11 -c -o /dev/null -Xclang -fdump-record-layouts`
prints, among the others,

```
         0 | SA
         0 |   int32_t kind
         4 |   union SA::(anonymous at ./u.h:8:32)
         4 |     int32_t i
         4 |     float f
         8 |   int32_t x
           | [sizeof=12, align=4]
         0 | SB
         0 |   int32_t kind
         8 |   union SB::(anonymous at ./one_arm.h:2:32)
         8 |     int8_t b
         8 |     int64_t q
           | [sizeof=16, align=8]
```

so the anonymous member, its arms and their offsets are in clang's answer,
including § 1's one-arm `SB`. It is a `-Xclang` (cc1) option, so whether its
text is a stable interface across 18.1.3, 20.1.8, 21, 22 and 23 is a
question, unrun on any but Apple clang 21; the `-ast-dump=json` form the
compiler already reads is the measured-stable one (`clang_floor.hero:28-30`).

## 3. The clangs the CI judges with are not in the platform list

`00-shared.md` § What every route must keep lists four clangs and says *a
route that rests on a clang warning or builtin is measured on every clang
this project meets*. Read from the four job logs of run 36939966148
(`gh api --allow-escape-sequences repos/heroes-lang/heroes/actions/jobs/<id>/logs`,
read-only, saved in `work/job-*.log`, `grep -a -m3 "clang version"`):

| CI leg | job | clang |
|---|---|---|
| Linux x86-64 | 110628923032 | `Ubuntu clang version 18.1.3 (1ubuntu1)` |
| Linux arm64 | 110628923054 | `Ubuntu clang version 18.1.3 (1ubuntu1)` |
| Darwin arm64 | 110628923062 | `Apple clang version 21.0.0 (clang-2100.1.1.101)` |
| Windows x86-64 | 110628922616 | `clang version 20.1.8` |

So two clangs that judge every push are missing: **18.1.3**, whose major
IS the floor (`constant FLOOR: i64` 18, `selfhost/cli/clang_floor.hero:35`;
its comment at 27-28 calls 18.1.3 *the oldest clang this project builds
on*), and **20.1.8**, the CI's Windows leg, which is not the Windows box's
23.1.1. `docs/ref/environment/linux/LINUX-MACHINE.md:271-281` already says the local
Linux containers (Debian clang 22.1.8) are *not the same compiler* as the
CI's Linux leg, and that a fact about clang's verdict measured there is a
fact about clang 22. A route resting on `-Wmissing-field-initializers`,
`-Wmissing-designated-field-initializers`, `-Winitializer-overrides` or
`-Wexcess-initializers` must be read on 18.1.3 before it is adopted; the
brief should say who runs it and where (an Ubuntu 24.04 container's `apt`
clang is the obvious instrument; unrun).

Also on this Mac and not in the brief: `/opt/homebrew/opt/llvm@22/bin/clang
--version` reads `Homebrew clang version 22.1.8`, so a seat can read a clang
22 verdict without asking the coordinator. `which -a clang` names only
`/usr/bin/clang` (Apple clang 21.0.0, clang-2100.3.34.2, as the brief says),
and `heroes doctor` in the copy names that one.

## 4. Route (2a): no warning reports a designated omission, on two clangs

Q2's (2a) is *a designated probe naming each declared field, under
whichever clang warning reports a field a designated list leaves out*, with
*which versions do what* unmeasured. Measured on this Mac, `work/c/d1.c`:

```c
#include "s3.h"
#include "u.h"
void f1(void) { S3 v = {.a = 0, .c = 0}; (void)v; }
void f2(void) { SA v = {.kind = 0, .i = 0}; (void)v; }
void f3(void) { SA v = {.kind = 0, .i = 0, .f = 0, .x = 0}; (void)v; }
```

`clang -std=gnu11 -fsyntax-only <flag> d1.c` for each of
`-Wmissing-field-initializers`, `-Wmissing-designated-field-initializers`
and `-Weverything`, on `/usr/bin/clang` (Apple 21.0.0) and Homebrew clang
22.1.8: the only diagnostic either prints about an initializer is
`d1.c:5:44: warning: initializer overrides prior initialization of this
subobject [-Winitializer-overrides]` (f3). **f1 (S3 missing `b`) and f2 (SA
missing `f` and `x`) are silent on both, under every flag**, while both
accept `-Wmissing-designated-field-initializers` as a known option (`clang
-fsyntax-only -Werror=unknown-warning-option
-Wmissing-designated-field-initializers e.c`, exit 0 on both, where the
control `-Wno-such-warning-xyz` is an error on both). The brief's *`-Wmissing-field-initializers`
changed meaning across clang versions* is unrun on any clang named in it;
on 21 and 22, in C, the designated form is not reported at all. Whether
18.1.3 or 20.1.8 differ is unmeasured. Q2 should ask *does any clang this
project meets report a designated omission in C*, and the ffi-pragmatist's
list of shapes should be run on 18.1.3 and 20.1.8 too.

## 5. The sentence attributed to panel 073

`00-shared.md` Q2: *What does a union record get: panel 073 says naming one
member of a union is naming all of it.* `grep -rn -i "naming all"` over
`docs/panel/`, `selfhost/emit/` and `archive/bootstrap-rs/heroes/src/emit/`
finds the phrase once, at `selfhost/emit/extern_record.hero:92`, inside the
comment of lines 84-92. Panel 073 says something else and records its
opposite: its § Two holes, item 1 (`docs/panel/073-the-fields-that-share-one-address.md:162-165`),
*A non-`partial` record naming ONE field of a union compiles silently ... It
claims completeness falsely and keeps `==`/`hash`*. Panel 077 closed the
`==`/`hash` half (the classify form fires at one field) and the done record
`docs/records/done/2026-08-17-0322-fixed-2026-08-16-panel-077-and-the-brief-that-filed.md`
marks the hole fixed; no sitting found here ruled that one member is a
complete binding. Panel 077's ffi-pragmatist conditioned its approval on
*making the declaration honest* (`077...:118-120`) and predicted SDL_Event
would need `partial` (`077...:106`). So what a union record's completeness
probe should say is an open question with two readings on the record, and
the brief should put it that way.

## 6. The history the brief leaves out

`00-shared.md` says panel 077 replaced panel 073's sum-of-sizes form *because
a padded union (`SDL_Event`) has room*. True, and incomplete: panel 073's
coordinator measured that the sum form *also catches every union shape
tested, including the anonymous union promoted into a struct (`8 >= 12`,
fires)* (`073...:75-76`), and its ffi-pragmatist's eleven shapes for the
`offsetof` predicate included *anonymous inner union* (`073...:52`). For `SA`
the sum form reads `sizeof(SA) = 12 >= 4+4+4+4 = 16`, false, so it fires.
Panel 077 replaced the predicate *wholesale* (the done record above) and the
shape was lost the same day. Two consequences for the sitting:

- the seats should know defect 151 is a coverage regression introduced on
  2026-08-16, with a predicate on the record that caught it;
- **a route nobody listed**: the classify form OR the sum form, linear in
  the fields, which restores 073's coverage of `SA` and keeps 077's of a
  padded union. Its blind spot is a padded struct whose anonymous union is
  small, and it is blind to § 1's one-arm case. Measured, `work/c/sum.c`
  under `/usr/bin/clang -std=gnu11 -fsyntax-only`:
  `_Static_assert(sizeof(SA) >= sizeof(int32_t)*4, ...)` fails (*static
  assertion failed ... 'sizeof(SA) >= sizeof(int) * 4'*), and the same
  form over `typedef struct { int64_t k; union { int8_t a; int8_t b; }; }
  PADU;` (`sizeof(PADU) >= 8 + 1 + 1`) passes, though `a` and `b` share a
  byte. Not built into the compiler; the seats can judge it against (1a).

Panel 073 also recorded that the `offsetof` predicate meets bit-fields and
incomplete types *with clang's text, not ours* (`073...:52`) and resolved
that they be *filtered before the assertion* (item 3, `073...:98-100`). (1a)
inherits both; the brief cites only the zero-sized members.

## 7. The escape for `SA` does not exist

`00-shared.md` § What every route must keep: *one record per arm by `tag`*
stays legal. For `SA`, a typedef of an anonymous struct, measured
(`work/arm.hero`, `record SA` naming `kind`, `i`, `x` and `record SAf tag
SA` naming `kind`, `f`, `x`): exit 1, *error[ffi_unknown_tag]: `u.h` has no
`struct SA` ... a record takes a typedef by its own name, with no `tag`:
`record SA` (§ 13)*. And a construction names every declared field
(`work/omit.hero`, `SA(kind: 1, i: 7, x: 3)` over a record declaring four:
exit 1, *error[missing_fields]: `SA` is built with every field, named*). So
for a typedef'd anonymous struct, one Heroes record exists, and a route
that refuses constructing it while it declares `i` and `f` leaves a program
that builds an `SA` holding `f` and reads `i` from one C returns with no
expressible form. Panel 073's escape is a premise for tagged structs only;
for `SA`, what the author writes instead is a question for the seats, and
the answer bears on whether (1c) or a refusal of construction is a ceiling
(panel 073's historian's Go precedent, `073...:55`).

## 8. The lane

`00-shared.md` § The lane: *on the coordinator's reading it changes no
surface, no spec token and no diagnostic class*. Three facts bear on it,
each run:

- `sed -n '354,362p' spec/heroes-spec.md` (verified as quoted): *A group's
  `record` is the header's struct: all its fields*. For `SA`, C's members
  are `kind`, `i`, `f`, `x` (the anonymous union's members are members of
  `SA`), so the spec tells a reader to write u17's record, which the route
  refuses to construct or compare. `grep -n -i "union\|overlap\|anonymous"
  spec/heroes-spec.md` finds no C union anywhere (one hit, line 259, about
  anonymous functions); panel 073's blind seat said the same in 2026-08
  (`073...:53`).
- `selfhost/emit/ffi_record.hero:176` and `:194`: both `ffi_union_field`
  messages begin *`` `T` is a `union` in `<header>` ``*, false for `SA`,
  which is a struct. The route needs new message text under that code, or
  a new code; whether either is a *diagnostic class* change is CLAUDE.md
  § 4's question.
- `.claude/skills/panel/SKILL.md:29-34`: *When in doubt take the full panel
  ... A question that looks internal but has a sentence in the spec behind
  it is a full panel.*

The brief's escape clause (a seat that finds a spec sentence says so) is
there; the premise should still go out as a question with these three facts
attached, since the seats it goes to are the two who do not read the spec as
a reader.

## 9. (1b) and the read-only union

`work/c/d2.c`, `void g1(void) { W v = {.i = 0, .n = 0}; (void)v; }` under
`/usr/bin/clang -std=gnu11 -fsyntax-only -Werror=initializer-overrides`:
*d2.c:3:32: error: initializer overrides prior initialization of this
subobject*, exit 1. `W` naming `i` and `n` is `probes/read.hero`'s record,
a correct read-only program that panel 073 keeps legal. So a designated
probe emitted for every record, as the completeness probe is today
(`extern_record.hero:100`), refuses defect 150's program; gated to the
`used` set (`extern_union.hero:133-156`) it does not. Q3 should say that
which records get the probe is part of the question. Also measured, the
existing construction already provokes the signal: u17's `build` prints
*initializer overrides prior initialization of this subobject* at
`u17_anon_constructed.hero:9:36`, on `(SA){.kind = t1, .i = t2, .f = t3,
.x = t4}`; arming it there is a variant of (1b) with no new C for the
construction half (not for `==`/`hash`, where nothing is constructed).

## 10. A bit-field member is exit 2 today

`work/bf.h`, `typedef struct { int32_t kind; uint32_t flag : 1; uint32_t
rest : 31; } BF;`, and `work/bf.hero`, `record BF` naming `kind: i32`,
`flag: u32`, `rest: u32`, reading `make_bf().kind`: `build` exit 2,
*internal error: compiling the generated C failed: bf.hero:4:71: error:
invalid application of 'sizeof' to bit-field*, twice, then *bf.c:87:37:
error: address of bit-field requested*. clang's text reaches the author and
the compiler blames itself for a binding the author can be told about
(`.claude/rules/c-boundary.md`, *a clang failure is normally exit 2 and says
the compiler is wrong*). `grep -n -i "bit-field\|bitfield\|bit field"` over
`docs/work/DEFECTS.md`, `DECIDE.md`, `docs/work/milestones/`,
`docs/records/done/`, the spec and design.md: no hit. It is a shape beside
defect 151 and unfiled; both seats are told to test a bit-field without
being told this.

## 11. The smaller repairs, each with what was run

- **The detector's lines.** `00-shared.md`: *emitted only for a record the
  program constructs or compares (`extern_union.hero:42-60`, `used`)*. Lines
  42-60 are `UNION_ASSERTION`, `record Used` and the head of
  `union_assertions`; `used` is lines 133-156 and `reach` 170-217. The
  trigger is wider than *constructs or compares*: a map construction and a
  map lookup reach it (`:144`, `:149-151`), and construction counts only at
  two or more declared fields (`:68`), `==`/`hash`/a map key at any arity.
  For `SA` the assertion IS emitted and passes: u17's `--emit-c` holds
  `_Static_assert(__builtin_classify_type(*(SA *)0) != 13, "heroes-ffi-union
  SA kind i f x");` (`work/u17.c:31`). The engineer's task 1 asks *not
  emitted or not firing*; the answer is the second.
- **The probe's form.** `00-shared.md` says *one positional zero per
  DECLARED field (`{}` for an aggregate field)*. It omits that a one-scalar
  record's list is `(0)` (`extern_record.hero:195-196`), panel 077's repair
  of clang's `{0}` exemption (`077...:82-85`). A designated route must keep
  that lesson for a one-field record over a multi-field struct; say so.
- **`clang-2100.1.1.101`.** The brief says it is *from CI run 36939966148 in
  defect 155*. Defect 155's text (`docs/work/DEFECTS.md:917`) says *Apple
  clang 21.0.0, Xcode 26.6*, no build number. The string is in the run's
  Darwin job log (measured, § 3), and in the tree at
  `tests/golden/run/fixedbugs-140-variants-through-arrays-a-thousand-deep-build.hero:22`
  and `selfhost/cli/clang_floor.hero:39`. The fact is right; cite the log.
- **"No clang text reaches the author."** `00-shared.md` cites
  `.claude/rules/c-boundary.md` and `generated-c.md`. Neither carries that
  sentence (`grep -n -i "clang text\|clang's text\|warning"`); both say *a
  clang failure is exit 2 and says the compiler is wrong*, with one class of
  exception. The zero-warning bar is `tests/harness/suite_warnings.hero:1-8`
  (*The generated C compiles with NO warnings at all*), over
  `tests/golden/run`, `tests/golden/emit` and `examples`. Name that suite:
  it is defect 150's instrument, and a route that adds a probe owes it.
- **The census compares the C only.** `build --emit-c` runs clang and prints
  its warnings on stderr (measured: `read.hero` exit 0 with the excess
  warning twice, u19 exit 1 with `ffi_incomplete_record` and no C on
  stdout). A census that diffs stdout alone cannot see defect 150's class (a
  warning appearing or leaving) nor a newly refused program whose stdout was
  empty on both sides. It should record exit code, stdout and stderr per
  file, both compilers. Headers resolve relative to the `.hero` file
  (measured from the root on a `tests/golden/unsupported/` case), so running
  from the root is sound.
- **The census population.** `git ls-files '*.hero' | xargs grep -l
  '^extern ' | wc -l` reads **417** of **1642** (verified, both numbers, by
  `git ls-tree -r --name-only ae08ed93`). Of the 417, by directory: 313 under
  `tests/golden/`, **75 under `docs/panel/*-briefs/`** (earlier sittings'
  probes, many meant to fail), 21 under `examples/`, 5 under `selfhost/`, 2
  under `tests/harness/`, 1 under `archive/`. 215 hold a group `record`
  (`grep -l '^    record '`). The baseline exit codes on the frozen
  compiler are § 12. Panel 073's *three SDL3 programs* are not in
  the tracked tree (`grep -l SDL_Event` over the 1642 finds one example,
  `examples/sdl/main.hero`, which binds no record, and five files of
  comments and goldens), so the census cannot show *panel 073's three
  readings stay legal*: the seats must write them. SDL3's headers are on
  this Mac (`/opt/homebrew/include/SDL3`), and `raylib.h`.
- **The existing union goldens.** `grep -rl "ffi_union_field"` over `tests/`
  finds two cases, both one-member `==` refusals
  (`tests/golden/unsupported/fixedbugs-140-a-union-compared-sixteen-deep-is-refused`
  and `...-a-union-in-a-variant-case-is-refused`); no golden holds the
  two-member *are the same bytes* message (`ffi_record.hero:194`). A route
  that changes that message has no golden to move and owes one.
- **The module sizes.** The brief gives `wc -l` (verified: 260, 287, 373,
  274) and points at `.claude/rules/module-shape.md`, which says the unit
  is `tests/harness/suite_layout.hero`'s and *not `wc -l`*. In that unit
  (non-blank lines outside `test` blocks, `code_lines` at
  `suite_layout.hero:686-701`, recomputed by an `awk` of the same rule, so a
  reimplementation): `ffi_record.hero` **183**, `extern_record.hero`
  **207**, `extern_field.hero` **286**, `extern_union.hero` **215**, against
  `CEILING` 300 (`suite_layout.hero:44-45`), none in `DECIDED`. The
  harness's own `layout` filtered to `extern_` read *1 passed, 0 failed*.
  `extern_field.hero` has 14 lines of room, not minus 73.
- **The largest group record by field count**, Q3's measurement: 16,
  `Matrix` in `tests/golden/run/ffi-a-c-array-member.hero` (an `awk` over
  the 417 counting `name:` lines under a group `record`; next are 9, 9, 8),
  so 120 pairs under a per-pair form, the number panel 073's engineer gave
  (`073...:51`). Real headers bind larger structs; the seats should not stop
  at the tracked tree.
- **u19's falsehood has a precedent the brief does not cite.**
  `tests/golden/unsupported/ffi-a-cstr-field-blames-the-field.hero:1-28`,
  defect 062 (panel 162's compiler-engineer, 2026-09-18): *`ffi_incomplete_record:
  does not name nodename` with `nodename` declared on the line below ...
  design.md §4.17 inverted twice in one message*. That defect's cause was a
  marker, not the probe, and its close measured the probe right on *two of
  five fields declared* (a prefix, lines 24-28). u19 is the non-prefix shape
  that measurement did not reach: the positional zeros fill `a` and `b`, and
  clang names `c`, which the record declares. One cheap guard is in the
  mapper whatever the probe becomes: `incomplete_record`
  (`ffi_record.hero:38-60`) can ask whether the member clang names is one
  the record declares, and if so it knows the positional reading is
  misaligned and must not say *does not name*. Unbuilt.
- **The probes directory carries 4 of the lane's 18 cases.**
  `scratchpad/lane-literals/pass1/U150/` (read only, its `summary.txt` and
  file list) holds u01 to u19 (no u11). Two the shared brief relies on and
  does not carry: u16 (`SA` with `kind` left out, exit 0, silent, the
  sibling of u07) and u08 (`SA` naming one arm, exit 0, silent and correct).
  The scratchpad goes away with the session (SKILL.md § 2); copy them into
  `probes/` or say they are not inputs.

## 12. The census's baseline on the frozen compiler

So a seat's census has a reference to compare against, and so the brief can
say what the 417 are: `xargs -P 3 -n 1` over the 417, each `heroes build
<file> --emit-c` from the copy's root on the seed-built compiler, recording
exit code and the count of `warning:` lines on stderr (`work/census.sh`,
`work/census.tsv`, the C and stderr per file under `work/census/`):

- **207 exit 0, 210 exit 1, 0 exit 2.** Exit 1 is 82 `tests/golden/check`,
  30 `unsupported`, 30 `fixedbugs`, 29 `surface-fixtures`, 33 under
  `docs/panel/*-briefs/`, 2 `ir`, 3 of the 5 `selfhost/` modules built
  alone, 1 `archive/`. Exit 0 is 119 `tests/golden/run`, 42 under
  `docs/panel/*-briefs/`, **all 21 `examples/` files**, 12
  `surface-fixtures`, 7 `fixedbugs`, 2 `selfhost/`, 2 `tests/harness/`, 1
  `ir`, 1 `emit`.
- **15 files print a clang warning, every one under
  `docs/panel/17[5-7]-briefs/`**: 121 `-Wunused-function` and 2
  `-Wunneeded-internal-declaration` lines, no initializer warning. So no
  tracked program prints an excess-elements or overrides warning today, and
  a route whose census shows one has introduced it.

So *every program that builds and runs today* is at most the 207 that exit
0, and the 210 that exit 1 are cases whose refusal must stay the same
refusal (its stderr), which the brief's census, comparing the C only,
would not check.

## 13. Verified as written, and not verifiable here

Verified, each by running it in the copy: the five reproductions of
`00-shared.md`'s table and of defect 150 (u17 `1056964608` with the excess
warning twice and the overrides warning once; u18 `true`; u07 `12` silent;
u19 exit 1 *`S3` does not name `c`*; `read.hero` `7` with *excess elements
in union initializer* twice at `read.hero:2:81`); `make_sa()`'s values
(`probes/u.h:15`); `SN` and `W1` in `u.h` (lines 9 and 4); the classify
assertion at `extern_union.hero:103` and its header comment at 1-24;
`completeness_probes` at `extern_record.hero:93`, the pragmas at 119-120,
the comment at 91-92; `ffi_incomplete_record` at `ffi_record.hero:53` and
`ffi_union_field` at `:169`; panel 073's ratification date (`073...:172`)
and its resolution items as summarised; panel 077's predicate; panel 061
at 267-268, panel 063 at 51 (*anonymous struct, same members*, a row of a
result-type check table, `types_compatible_p` against `_Generic` and
`sizeof`, not a union finding; its relevance to this sitting is thin), panel 071
at 34-35 (*2 unreachable inside anonymous unions*); spec § 13 at 354-362;
defects 150 and 151 in `docs/work/DEFECTS.md` (lines 866 and 876); the
417 and 1642; Apple clang 21.0.0 (clang-2100.3.34.2) on this Mac.

Not verifiable from the frozen tree, and not material to a route: the
Windows box's *clang version 23.1.1* (`ssh win`, the coordinator's); the
Linux containers' *Debian clang version 22.1.8* from *the arm64 leg's log,
2026-10-02 09:01* (not in the tree; `LINUX-MACHINE.md` and
`clang_floor.hero:40` carry the same string from earlier days); defect 150
*queued to lane ffi-macro and withdrawn at 10:35* (a scheduling fact after
the freeze); the rebuild's *about a minute* (carried from SKILL.md's 60.97 s
of 2026-09-24; timing is forbidden here).

