# Panel 186, the completeness critic's second pass: the reports

Written 2026-10-02 by the completeness critic, after the seats. Inputs read
whole: the briefs (`00-shared.md`, the four seat briefs, `blind/`; the
earlier shared brief is `00-shared-before-the-critic.md`), and every file of
`docs/panel/186-reports/`: `compiler-engineer.md`, `ffi-pragmatist.md`,
`spec-warden.md`, `historian.md`, `llm-ergonomist.md`,
`coordinator-blind-program-built.md`, `coordinator-platform-readings.md`.
Every command below ran in `<scratchpad>/186-critic/t2/`, a `git archive
779139d0 | tar -x` copy with its compiler built from the seed (`heroes
0.2.0`), `HEROES_RUNTIME` set to its `runtime/`, cases under `t2/w2/`; or it
is a read-only `git` query on the trunk. No seat's directory was read or
run: a seat's file is cited by the text its report quotes, and a route's
behaviour on a case I wrote is an INFERENCE from the rule the seat states,
marked so. No paid run, at most three processes, nothing timed.

The coordinator's fact, used below: lane ffi-macro's `6559facf`, on branches
`lane-ffi-macro` and `lane-round1002` (`git branch -a --contains`), not on
`779139d0`.

## 1. What the synthesis must settle, most important first

1. **Which (1e).** The engineer's layout dump behind a screen is the only
   (1e) that is built, run over the census, and read on five clangs. The
   pragmatist's JSON dump with an adjacency probe is unbuilt, and two of its
   parts fail on cases measured here (§ 2): **the adjacency probe refuses a
   correct one-arm binding of a REAL SDK struct** (`mach_port_options_t`
   bound by `work_interval_port` builds and runs today, prints `13`; the
   adjacency assertion fails on both clangs of this Mac), and **the
   member-access dump does not hold an anonymous union's other arm**, which
   the pragmatist's narrowed `==` rule needs. Against the engineer's route,
   measured here: **one flagged record over a by-value nest brings back the
   cubic dump**, 348,378,433 bytes from clang alone at depth 1000 and
   10,367,327 at depth 300, past the 10 MB the engineer's condition names
   (for a real header; this one is synthetic, as defect 140's tracked case
   is). SKILL.md § 3c: a route that does not build is not adopted.
2. **The `==` rule, one rule, and whether it is sound** (§ 3). The engineer
   built two rules (an anonymous union's field: refused at any arity; a
   field a macro reaches in a named union: refused only if it does not fill
   it); the pragmatist vetoes the any-arity rule on `in6.hero` and approves
   "refused where the field does not cover its union". **Covering by size is
   not sound, measured**: a padded struct arm that fills its union compares
   two C values whose bytes differ as `true`, and so does an `f32` arm that
   fills it (`-0.0` against `0`). A rule that keeps `in6.hero` and
   `sig_eq` and refuses both is *covering and compared bit for bit* (an
   integer, a pointer, an array of them). Whether it also changes panel
   077's any-arity rule for a union TYPE (two goldens) must be said.
3. **(1h) is unbuilt by every seat, and the adopted sentence depends on
   it** (§ 4). The engineer's objection holds on the record: `missing_fields`
   is a `check` refusal and the checker has no union knowledge (panel 077,
   `077...:43`), so (1h) needs `check` to stop refusing an omitted field for
   every group record, or a marker, or a C front end in `check` (refused at
   077). The pragmatist's (1h) is hand C (`work/sdl/h1.c`). Nobody built it.
4. **The spec sentence is false under the built route, and under both seats'
   `in6` handling** (§ 5). O_final's *is built naming exactly one* is (1h)'s
   clause; under the built route a record naming `i` and `f` is never built.
   O_final's *comparing such a record ... compile errors* names `in6`'s
   `s6_addr`, which lies in a named union, and both seats keep `in6.hero`
   legal. Priced here, vendored, for the routes as built and as narrowed:
   R_build +66, R_build_cover +88, O_cover +83 (O_final +61, reproduced).
5. **Lane ffi-macro and this route meet in one message.** Five of
   `6559facf`'s seven new goldens are records over a two-member struct
   `{a, b}` naming `a` and a misspelt `c`; under the engineer's route the
   layout check runs before any unit compiles and says *does not name `b`*
   where the lane's golden expects `ffi_unknown_field` on `c`'s line (§ 6;
   inference from the engineer's `typo2`/`typo3` rows, unrun). The files are
   disjoint, the behaviour is not: merge order and which diagnostic leads.
6. **Bit-fields: refuse or bind** (§ 7). Built: the refusal (engineer). Measured
   in C and unbuilt in the compiler: binding (pragmatist, historian, the
   warden's B3). The warden's premise that B3 needs the width from (1e) is
   contradicted by the pragmatist's run-time fit check.
7. **Panel 073's *one record per arm by `tag`* does not exist on the tree**,
   for a union (both compiling seats measured it) as for a typedef'd
   anonymous struct (pass one), and the warden's approval of O_final says it
   *stays legal*.
   The refusal's note, *A union has no `record` spelling in this language*,
   is false (`read.hero` binds a typedef'd union) and contradicts panel 077
   item 4; it has no golden (§ 8). Unfiled.
8. **The historian's (1g) approval meets its own reversal condition** (§ 9).
9. **The blind seat never read the sentence the sitting would adopt** (§ 10):
   it read L, M and N; O_final and the built route's sentence are unread.

## 2. The two (1e)s, and which side of each objection is checkable

**The engineer's layout dump, against the pragmatist's three objections.**

- *Two texts across clangs*: checkable and checked. The engineer's reader
  reads no parenthesised type text, and the coordinator's readings found
  `lay.c` equal to Apple 21's output with every parenthesised span masked on
  Debian 18.1.8, Debian 20.1.8 and Ubuntu 18.1.3, the CI's own
  (`coordinator-platform-readings.md`, second table). 23.1.1 is unrun for the
  engineer's files. **Measured here for the Windows TARGET** (not 20.1.8 or
  23.1.1): `t2/w2/ms/ms.c`, wrappers over `SA`, `DEEP`, `BF` and a
  mixed-width bit-field struct `MIX { uint8_t a : 3; uint32_t b : 5; int32_t
  c; }`, under Homebrew clang 22.1.8 with and without `--target=x86_64-pc-windows-msvc
  -ffreestanding`: 140 lines each, and `diff` shows only `MIX`'s values
  (`0:3-7 | uint32_t b` and `[sizeof=8` against `4:0-4 | uint32_t b` and
  `[sizeof=12`), the format identical. So the MSVC layout is the same text
  carrying a different layout, which is what the reader is for. Line endings
  on the Windows box stay unmeasured.
- *Only what Sema laid out*: checkable and answered by the engineer's
  wrappers (`_Static_assert(sizeof(struct hero_ly_<n>) > 0, "")`); the
  pragmatist's own `plat_lay.c` used tentative definitions, which is why it
  printed one record (both reports agree on the mechanism).
- *Blind to `sa_handler` and `s6_addr`*: conceded and repaired by the
  engineer (a field the dump does not show is asked by name under `#ifdef`),
  measured on its `sig_read`, `sig_eq`, `in6_eq`, `nuw_*` rows. Not checkable
  from here beyond the report.

**What neither seat measured: a flagged record over a by-value nest.** The
screen exists because the thousand-deep case's dump was 693,681,863 bytes
(engineer § 4); it vouches for a record that names exactly its members, so
that case makes no dump. Add one anonymous union at the top of the same
nest and the record is flagged. `t2/w2/deep/`, headers `G0 ... G<N-1>` as in
`tests/golden/run/fixedbugs-140-extern-records-a-thousand-deep-build.h`, then
`typedef struct { G<N-1> inner; union { int32_t i; float f; }; } TOP;`, and
the engineer's wrapper for `TOP` alone, under `/usr/bin/clang -std=gnu11
-fsyntax-only -Xclang -fdump-record-layouts lay<N>.c | wc -c`:

| depth | bytes |
|---|---|
| 100 | 492,727 |
| 300 | 10,367,327 |
| 1000 | **348,378,433** (exit 0) |

Near-cubic, as the engineer found for the unflagged case. The engineer's
route caches the dump on disk (`dump-<sum>-<digest>.txt`, its § 10), so this
program would write about a third of a gigabyte into `build/` and read it,
where the trunk builds it. The engineer's own condition reads *object if a
real header's flagged dump passes 10 MB*; this header is synthetic, as
defect 140's tracked one is, and depth 300 already passes 10 MB. A
mitigation nobody listed, measured only for size: `-Xclang
-fdump-record-layouts-simple` prints 38,736 bytes at depth 300 and 128,338 at
1000 (linear), but its blocks carry `FieldOffsets: [0, 64]` and no member
names (`lay100.c`, the `TOP` and anonymous-union blocks read), so it is not a
drop-in. Whether the JSON member-access dump grows on this nest is
unmeasured; it dumps only `hero_lay_` functions, so linear growth is an
inference.

**The pragmatist's JSON dump with the adjacency probe.**

- **The adjacency probe refuses a correct binding**, which the pragmatist's
  own veto standard forbids. Its § 7 approves it *as Q2's completeness probe
  for STRUCTS with a complete type*, and its table shows it firing on `SB`
  bound by `b` (P14, five clangs), which the pragmatist reads as a virtue
  (*sees `SB` by one arm*). As a completeness probe that is a refusal of a
  record every other reading calls complete (one arm per union: the
  engineer's rule, O_final's *names one or more of each union*). On a real
  header, measured: `t2/w2/mpo.hero`, `extern "mach/port.h"`, `record
  mach_port_options_t` naming `flags`, `mpl` (a record over
  `mach_port_limits_t`) and `work_interval_port`, one arm of the SDK's
  anonymous union `{ uint64_t reserved[2]; mach_port_name_t
  work_interval_port; ... }` (`port.h:477-486`): `build` exit 0, silent,
  prints `13`. `t2/w2/mpo_adj.c`, the pragmatist's `ADJ` and `END` macros
  over the same declared list, on Apple 21 and Homebrew 22.1.8: `A0`, `A1`,
  `A2` pass, **`A3 after work_interval_port` fails**, a message saying a
  member is missing after a field where the union's longer arms are. The
  pragmatist's census saw no such shape in the tracked tree (`(B)`), and its
  real-header sweep's `glob_t` arms are both pointers, so the gap never
  showed. Adjacency could still serve as the `==` detector at `used`
  records; as written it is the completeness probe.
- **The member-access dump does not hold the other arms.** `t2/w2/json/td.c`,
  `typedef struct other_tag { int32_t kind; union { int8_t b; int64_t q; }; }
  TD;` with `hero_lay_TD_b`, Apple 21, `-ast-dump=json -ast-dump-filter=<f>`:
  filter `hero_lay_` gives 2 `MemberExpr`s and `"name": "q"` 0 times; filter
  `other_tag` gives the record's tree (4 `FieldDecl`, 2 `IndirectFieldDecl`);
  filter `TD` gives the typedef and no `FieldDecl`. So the pragmatist's
  narrowed `==` rule (*`sizeof` of the arm against its siblings', names from
  (1e)*) needs the record tree, which is one filtered process per record,
  keyed by the tag or the typedef name depending on the header's spelling,
  or the unfiltered dump (84,186,095 bytes for `SDL3/SDL.h`, the engineer's
  § 3). An anonymous union has no name C can take `sizeof` of, so the
  siblings' names are the only C route to its size. Unstated in the report,
  unbuilt.
- **It cannot name u19's `b`**: adjacency says *between `a` `c`* (the
  pragmatist's table); the brief's Q2 asks WHICH field. The engineer's route
  says *does not name `b`* (its table).

So of the two, the checkable record is: the built one answers every
objection measured against it except the flagged deep nest; the unbuilt one
has a false refusal on a real header and an information gap in its `==`
rule. Neither is a verdict of mine.

## 3. The `==` rule, the `in6.hero` veto, and covering

**What is on the table.** The brief's Q1: refused *for a record holding any
field in a union*. The engineer's final build: any arity for a field of an
anonymous union (`SA` naming `kind`, `i`, `x`, compared: refused), *fills its
named union* for a field a macro reaches (`in6_eq`, `sig_eq` legal, `nuw_eq`
refused). The pragmatist: veto on any-arity, approve *refused where a
declared field lies in a union it does not cover*. The engineer: *two rules
where the pragmatist asks for one ... which rule is the panel's*.

**Covering by size admits a wrong `true`, measured** (`t2/w2/cover.h`,
`zero.h`, on the trunk's compiler, where every one is legal today because
each type is a struct):

| case | the arm, and how it fills the union | prints | the C values |
|---|---|---|---|
| `cov_sp.hero`: `SP { kind; union { Inner s; int64_t q; } u; }`, `#define sp_s u.s`, `record SP` naming `kind`, `sp_s: Inner`; `Inner { int8_t a; int32_t b; }` | `s` is 8 bytes, the union 8 | `p1 == p2` **`true`**, `sp_q(p1) == sp_q(p2)` `false` | `q` 0x0000000500000001 against 0x0000000500FFFF01: they differ in `Inner`'s padding |
| `cov_zero.hero`: `SZ { kind; union { int32_t i; float f; }; }`, `record SZ` naming `kind`, `f: f32` | `f` 4 bytes, the union 4 | `make_z1() == make_z2()` **`true`**, `sz_i(...) == sz_i(...)` `false` | `i` is `INT32_MIN` against `0`: `f` reads `-0.0` and `0.0` |
| `cov_sf.hero`, `cov_sm.hero`: the same `f32` arm, anonymous and through `#define sm_val u.val`, `i` holding 0x7fc00000 | covering | `a == a` `false` | the same value |
| `plain_nan.hero`: `PF { kind; float f; }`, no union, `f` = `NAN` from C | | `a == a` `false` | |

The last row is the control: `a == a` is `false` for a plain record holding
a NaN, so the reflexivity rows are Heroes' float equality, not a union
defect by themselves. The first two are not: two C values that differ
compare `true`, the class `SB` was added to defect 151 for. **Under the
pragmatist's rule both are legal** (the arm covers). **Under the engineer's
final build**: `cov_zero`'s anonymous arm is refused (any arity); `cov_sp`'s
macro-reached arm fills its union, so by its stated rule it is legal and
answers `true` (inference from the rule, not run on its compiler).

**A rule that keeps both vetoed programs and refuses both rows**: refused
unless the declared field fills its union AND its equality is bit identity,
which in Heroes' field kinds is an integer, `bool`, a `ptr`/handle, or a
fixed array of them, never a float and never a record. `in6.hero`
(`s6_addr: u8[16]` over a 16-byte union) and `sig_eq` (`sa_handler`, a
function pointer filling `__sigaction_u`) stay legal; `cov_sp` (a record
arm) and `cov_zero` (a float arm) are refused; `SA` naming `i` becomes legal
for `==`, which the engineer's any-arity build refuses today. Unbuilt by
anyone; its sentence is priced in § 5.

**What the synthesis must say beside it.** Panel 077's rule refuses `==` on
a union TYPE at any arity, covering or not: the two `ffi_union_field`
goldens (`tests/golden/unsupported/fixedbugs-140-a-union-compared-sixteen-deep-is-refused`,
`...-in-a-variant-case-...`) bind `U` naming `i: i32` over `union { int32_t
i; float f; }` and `W` naming `i: i32` over `union { int32_t i; uint32_t n;
}` (their `.h` files, line 4), each a covering bitwise arm. One rule for a union's field
wherever the union sits flips both to legal (and they would need a
non-covering member to keep witnessing defect 140's transitivity); two rules
leave the union TYPE stricter than the union inside a struct. And the
warden objected to F_sum because *its rule lives in the header's layout*
(design.md §1.3); a covering rule lives there too, though the blind seat's
criterion admits *the named header*. The warden has not judged a covering
rule.

## 4. (1h): unbuilt, and where it would live

- **Approve**: the pragmatist (the shim-free SDL3 event loop; hand C
  `h1.c`, zero warnings on two clangs), the historian (Rust, D, Zig, Swift
  for a union type; only D flat for a struct, its § 2), the warden (as
  O_final's clause). **Object**: the engineer (*`missing_fields` would move
  from `check` to `build`*; Principle 0, no tracked program needs it).
- **The engineer's premise, checked**: `probes/critic/omit.hero`
  (`SA(kind: 1, i: 7, x: 3)` over a record declaring four), `heroes check`
  in `t2/w2`: exit 1, *error[missing_fields]: `SA` is built with every
  field, named*. The checker has no union
  knowledge (`077...:43`). So the brief's *(1e) would give it that knowledge*
  (`00-shared.md` Q4) is true of `build` only. (1h) then needs one of: `check`
  accepting an omitted field of EVERY group record (so `S3(a: 1)` over
  `{a, b, c}` passes `check` and only `build` refuses it); **a marker on the
  record that `check` reads and `build` verifies against the layout** (a
  route nobody listed this sitting; panel 073 filed (1h) as *waiting for a
  marker*, and panel 077 vetoed an UNVERIFIED marker because *item C is the
  author who did not write it*; a marker `build` refuses to be missing over
  a union answers that; a surface form, unpriced, unbuilt); or `check`
  running clang (refused at 077).
- **Not a contradiction of fact**: both compiling seats measured that no
  SDL3 key or mouse event is readable today without hand-written C (the
  engineer's `sdlx.h` makers, the pragmatist's `sdl_ev2.h`). They differ on
  the weight: Principle 0 (CLAUDE.md § 2) against design.md §1.11 and §1.12.
  The blind seat's M prediction (≥75 % one-turn success against N's ≤30 %)
  is an argument, not a trial; the warden notes metric 2 has zero tasks.
- **SKILL.md § 3c**: no seat built (1h). If the synthesis adopts it, it is
  adopted unbuilt, or built first.

## 5. The sentence against the routes

**O_final** (the warden's approve) says a record *is built naming exactly
one* member of each union and that *comparing such a record ... [is a]
compile error*. Under the built route without (1h), a record declaring `i`
and `f` cannot be built at all, and construction names every declared
field (`missing_fields`), so *built naming exactly one* is false. Under both
seats' handling of `in6.hero` (legal), *comparing such a record* is false
for `In6`, whose `s6_addr` lies in a named union. The warden's own condition
reads *my approval is void if [(1e)] cannot see one of those, because a word
of O_final then becomes false*: the word that becomes false is the
construction clause, and it is (1h)'s, not (1e)'s.

**Priced here, vendored only**, spliced in place of the three lines from
*`record Font partial` names only some* to *not the field list's.*
(`t2/w2/spec/splice.py`, the anchor asserted once; `./heroes measure
w2/spec/<id>.md` from the copy's root), against 6716 / 6838:

| id | what it says, beyond O_final's first sentence | legacy | cl100k | Δ |
|---|---|---|---|---|
| O_final | the warden's, verbatim: built naming exactly one; comparing refused | 6777 | 6899 | **+61** (the warden's figure, reproduced) |
| R_build | *a record names one or more of each union and reads any, and one naming two of a union, an anonymous struct's fields counting as one, cannot be built*; comparing refused as O_final | 6782 | 6904 | **+66** |
| R_build_cover | R_build's construction, and *Comparing a `partial` record, or one holding a union's field that is not an integer, pointer or array of them as wide as the union, and using it as a map key are compile errors, for it and for any value holding it.* | 6804 | 6926 | **+88** |
| O_cover | O_final's construction ((1h)), R_build_cover's comparison | 6799 | 6921 | **+83** |

R_build is true under the engineer's build only if `in6.hero` and `sig_eq`
are refused, which the pragmatist vetoes; R_build_cover is true under the
built route with § 3's rule in place of its two; O_cover is true only with
(1h) built. The real counts are unmeasured (paid); on the warden's
calibration of 1.16 to 1.42 they read +77 to +94, +102 to +125, +96 to +118,
an inference. Every one is inside the 1120 spendable. The bit-field clause
(the warden's B1_merged +15 or B3_abort +40) adds to any of them. None of
these texts was read by a blind seat (§ 10).

## 6. Lane ffi-macro's `6559facf` and the engineer's route

`git show --stat 6559facf`: `selfhost/emit/ffi_field.hero` (+113, -20) and
seven `tests/golden/unsupported/fixedbugs-152-a-field-the-header-lacks-*`
cases over `fixedbugs-152-fields.h`. The engineer's route changes eight other
files, so no textual conflict. The cases, read with `git show
6559facf:<path>`: five are non-`partial` records over a two-member struct
`{a, b}` (`anon_s`, `tagged_t`, `named_s`, `T tag tagged_s`, `O tag
only_tag`) naming `a` and `c`, annotated `#~ ffi_unknown_field` on `c`; one
is `anon_u` naming `x` alone; one is `partial`. The engineer's route runs its
layout check *right after the pointee check, before any unit compiles*
(its § 4), and its `typo2`/`typo3` rows (*a misspelt field in a typedef'd
record, nothing else wrong*) read *does not name `b`*. So after both merge,
the five would lead with `ffi_incomplete_record` on the record's line where
the golden expects the field's (inference from the two reports and the case
texts; unrun, since the commit is not on the frozen tree). The lane's
message names the cause on the line the author fixes; the route's names
the effect. The engineer itself says its message *is not the repair*.

## 7. Bit-fields

Refuse, built: the engineer (*`flag` is a bit-field of `BF`*, exit 1 on the
field's line) and the warden's B1_merged (+15). Bind, measured in C and
unbuilt: the pragmatist's § 4.3 (`_Generic` width, the full-width test
P18/P19 on five clangs, a run-time fit check `((T){.f = v}).f == v`, a hash
by value), the historian (*those who let the C compiler do the bits bound
bit-fields as fields; those who refused left users writing C*; curl's
`curl_hstsentry`, the pragmatist's `hsts.hero`, is rung 4's header), the
warden's B3_abort (+40, *production-ready*). **Contradiction with a
checkable side**: the warden says B3 *is only true if the route gives the
emitter the bit width (the AST or the layout of (1e))*; the pragmatist
measured the run-time fit check, which needs no width (*`fits(1)=1
fits(5)=0`*, its § 4.3, two clangs). The fit check's reading on 18.1.3 and
20.1.8 is not in the coordinator's runs (P17 to P20 cover width, full-width
and hash; the fit check is `bf3.c`/`bf4.c`, not `plat.c`).

## 8. The *one record per arm* reading, and the refusal behind it

The engineer (`by_tag`, `record KeyArm tag SDL_Event`: exit 1,
`ffi_tag_is_a_union`) and the pragmatist (`r2_tag.hero`, and a minimal
`union utag`) both measured it refused; pass one measured `ffi_unknown_tag`
for a typedef'd anonymous struct. The warden's report says O_final *keeps
all three of panel 073's readings: `read.hero`, u15 and one record per arm
stay legal*, and objects to M because it would turn *one record per arm by
`tag` (the SDL pattern)* into an incomplete record: asserted, not run, false
on the tree. Read here: the refusal is `selfhost/emit/ffi_tag.hero:200-218`,
landed in `8fdd2a3c` (2026-08-17, *M-selfhost-port: the tag classes
cross*, `git log -S'ffi_tag_is_a_union'`); its note says *a group's `record`
is the header's STRUCT (§4.19). A union has no `record` spelling in this
language*, which `read.hero` falsifies (a typedef'd union bound by `record
W`, exit 0), and panel 077's ratified item 4 asked that the emitter *spell
`union T` where the header says union*. `grep -rln ffi_tag_is_a_union`
over `tests/` and `docs/panel/*.md`: no hit. A false note on a code no
golden holds, against a ratified item: a defect to file, and Q4's escape
for a TAGGED union is a question this sitting did not settle either.

## 9. The rest: contradictions and claims not settled by a command

| what | who | the checkable side |
|---|---|---|
| (1g) *approve as the floor* | historian | its own § 5: *to object: a tracked program, outside `docs/panel/*-briefs/`, that compares two group records and is refused by it*. `tests/golden/run/fixedbugs-a-group-record-in-every-container.hero` compares raylib's `Color` four times and keys `{Color: i64}` (read here, lines 47-61), found by the engineer and the pragmatist. The condition is met |
| (1a) approve (at `used` records) | pragmatist, against object by engineer, historian, warden | not a contradiction of fact: all four measured it blind to `SB`; the pragmatist pairs it with the JSON names |
| (1b) approve | pragmatist, against object by the other three | the same five-clang verdict is undisputed (engineer: *which I do not dispute*); the dispute is that it says nothing of a field left out |
| *the briefs' probes are untracked in the trunk* | warden § 0 | `git show --stat 779139d0` lists `docs/panel/186-briefs/probes/**`: tracked. The briefs and `blind/` are untracked (`git status`). It copied them either way, so nothing it measured moves |
| *C zeroes the rest* (`extern_union.hero:47-48`) | warden § 8, asked of the two compiling seats | neither answered. Measured here on Apple 21: `t2/w2/rest.hero`, `SB(kind: 1, b: 5)` after a C function writes 0xAB over 256 bytes of stack, passed to C reading `q`: **`5`** at the default level and at `-O2` (the emitted `(SB){.kind = t1, .b = t2}`). So clang 21 zeroes the union's remaining seven bytes here; C11 leaves them unspecified (the warden's citation); other clangs unmeasured. Every route keeps this construction legal |
| suites owed by the engineer's route | engineer | it ran the compiler's tests (1009), `layout`, `warnings` and the `--emit-c` census. Not run: `emission` (its own count: 24 blessed files lose the probe), `determinism`, `run`, `unsupported`, `check`, `corpus`, `descriptors`, `wholes`, owed at the batch by `.claude/rules/verification.md`'s map for `selfhost/emit/**` and its rule for a change in what is refused |
| the blind program under L | coordinator | re-run here (`t2/w2/l_prog.hero`, `l_rev.hero` over `sa.h` and its twin with `union { float f; int32_t i; }`): exit 0 with 3 warnings each, `12 1.5 4 true` and `12 0.0 4 true`. As recorded |

## 10. The question the sitting should have asked

**What single sentence, in the spec's words, is true of the route the
sitting builds, for an anonymous union, a named union a macro reaches, and a
union type alike, and which two `==` programs does it keep?** The brief put
the comparison rule as a premise (*any field in a union*); the seats split it
into two rules and a veto, the warden priced sentences against a third
reading ((1h) built, every union refused at compare), and no seat measured a
covering arm that is not compared bit for bit (§ 3). Two smaller ones
followed from it and were not asked: *does (1h) live in `check` or in
`build`* (§ 4; the brief's Q4 assumed (1e) settles it), and *what does a
flagged record cost when the layout is deep* (§ 2).

**A paid run worth running** (not run; the coordinator's to decide): the
blind seat as run at 11:09 (one `claude -p` session, SKILL.md § 2's command,
150.9 s and 0.479 USD by its `run.json`, `llm-ergonomist.md`'s header), on
the same task with the sentence the synthesis adopts (O_final, R_build_cover
or O_cover) in place of L, M and N: the adopted text has no blind reading,
and the M reading it inherits presupposes (1h).

## 11. What ran here

`t2/w2/`: `cover.h`, `cov_sf.hero`, `cov_sm.hero`, `cov_sp.hero`,
`zero.h`, `cov_zero.hero`, `plain.h`, `plain_nan.hero`, `rest.h`,
`rest.hero`, `mpo.hero`, `mpo_adj.c`, `l_prog.hero`, `l_rev.hero`,
`sa_rev.h`; `deep/` (the three depths), `ms/ms.c`, `json/td.c`, `spec/`
(the splicer and four spliced specs). The scratchpad goes with the session;
the headers and sources of § 3 are quoted in its table in enough detail to
rebuild, and the rest are one-paragraph files.

Status: complete. Second pass; nothing further unless the coordinator
resumes me.
