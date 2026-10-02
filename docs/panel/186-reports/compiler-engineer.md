# Panel 186, the compiler-engineer's report

Written 2026-10-02 by the compiler-engineer seat, as it goes. Every command
below ran in `<scratchpad>/186-compiler-engineer/`, a `git archive 779139d0 |
tar -x` copy, with compilers built inside it (`heroes-trunk` from the seed,
`heroes` from my edits), `HEROES_RUNTIME` set to the copy's `runtime/`. No paid
run, at most three processes at once, nothing timed. Status: IN PROGRESS.

## 1. The causes, confirmed on my compiler

On `heroes-trunk` (the seed of `779139d0`, `heroes 0.2.0`), `heroes run` on
each probe copied from `docs/panel/186-briefs/probes/`:

| case | measured | cause, confirmed |
|---|---|---|
| `u17_anon_constructed` | exit 0, prints `1056964608`; stderr holds *excess elements in struct initializer* (the probe `SA v = {0,0,0,0};`) and *initializer overrides prior initialization of this subobject* at `u17:9:36` | the classify assertion IS emitted and passes: `--emit-c` line 31 is `_Static_assert(__builtin_classify_type(*(SA *)0) != 13, "heroes-ffi-union SA kind i f x");`. `SA` is a struct (12). The probe writes one zero per declared field and C has three top-level members |
| `u18_anon_compared` | exit 0, `true`, the excess warning | same assertion, passes; `==` walks `i` and `f`, the same bytes |
| `u07_anon_omits_x` | exit 0, `12`, silent | three zeros fill `kind`, the union (by brace elision, `-Wmissing-braces` is ignored) and `x`: C's count is met by the wrong fields |
| `u16_anon_omits_kind` | exit 0, `15`, silent | the same: `i`, `f`, `x` are three zeros for C's three members |
| `u19_struct_omits_middle` | exit 1, *`S3` does not name `c`* | two zeros fill `a` and `b`; clang names C's third member, which the record declares |
| `one_arm` (`SB` naming `kind`, `b`) | exit 0, `true` | the classify assertion passes (struct); nothing asks about the anonymous union |
| `one_arm_union` (`UB` naming `b`) | exit 1, `ffi_union_field` | classify answers 13 |
| `u08_anon_one_member`, `u08_mine` | exit 0, `15`; `7` and `12` | correct: one arm per union, three zeros for three members |
| `read.hero` (defect 150) | exit 0, `7`, *excess elements in union initializer* at `read.hero:2:81`, twice (once per clang run of the build) | the probe `W v = {0,0};` for a union |
| `critic/bf.hero` (defect 156) | exit 2, *invalid application of 'sizeof' to bit-field* at the two field assertions, *address of bit-field requested* at `bf.c:87` and `:88`, the record's generated hash | the field assertion's `sizeof(((BF *)0)->flag)`, and the descriptor's `hash(&v->flag)`, emitted although the program never hashes a `BF` |

So the brief's reading of every cause stands on my compiler. What each needs:

- `u17`: construction refused (two declared fields share bytes); no warning.
- `u18`, `one_arm`: `==` refused (a declared field lies in a union).
- `u07`, `u16`, `u19`: `ffi_incomplete_record` naming `x`, `kind` and `b`.
- `u08`, `u08_mine`, `read.hero`: unchanged verdicts, and no warning.
- `bf.hero`: exit 1 on the author's line, never exit 2.

## 2. The census's baseline, on the trunk's compiler

`git ls-tree -r --name-only 779139d0 | grep '\.hero$'` reads 1672 files, and
`xargs grep -l '^extern '` over them 447. Each ran as `heroes-trunk build
<file> --emit-c` from the copy's root, `xargs -P 3`, exit code, stdout and
stderr kept per file (`work/census.sh`, `work/census-trunk/`):

- **229 exit 0, 217 exit 1, 1 exit 2** (`probes/critic/bf.hero`, defect 156).
- 30 files print a clang warning; the initializer warnings (20 *excess
  elements in union initializer*, 10 *excess elements in struct
  initializer*, and u17's *overrides*) are all in 15 files under
  `docs/panel/186-briefs/probes/`; the rest are `-Wunused-function` lines
  under `docs/panel/17[5-7]-briefs/`, as the critic counted on `ae08ed93`.

## 3. What clang can tell, measured on the two clangs of this Mac

Apple clang 21.0.0 (clang-2100.3.34.2) and Homebrew clang 22.1.8, files in
`work/c/`.

**Scoping the AST dump to one record does not work in one process.**
`-ast-dump=json -ast-dump-filter=SA` over `u.h` prints `SA`'s anonymous
`RecordDecl` with its anonymous union (`"tagUsed": "union"`, an implicit
`FieldDecl` with no name, `IndirectFieldDecl`s), on both clangs. But the filter
is a substring of the qualified name, and for `typedef struct other_tag {...}
TD;` the filter `TD` prints only the two `TypedefDecl`s and no record at all
(both clangs). A `hero_ty_`-named typedef of `SA` prints the type chain only,
`ownedTagDecl` by id, never the fields. So the JSON route costs one clang
process PER RECORD, or the unfiltered dump: `SDL3/SDL.h` alone is 84,186,095
bytes of JSON (`stdio.h` 880,547, `raylib.h` 2,519,025), measured with `wc -c`.

**`-Xclang -fdump-record-layouts` scopes itself.** A unit holding the
program's headers and, per record, `struct hero_ly_<k> { T v; };
_Static_assert(sizeof(struct hero_ly_<k>) > 0, "");` under `-fsyntax-only`
prints, on stdout, one block per laid-out record, and the block that opens
with `0 | struct hero_ly_<k>` holds `T`'s whole tree under the `v` line,
whatever `T`'s spelling (typedef of an anonymous struct, tagged, typedef of a
typedef). The text the reader needs, all on both clangs:

```
         0 | struct hero_ly_4
         0 |   DEEP v
         0 |     int32_t kind
         4 |     struct DEEP::(anonymous at ./bf.h:4:32)<SP>
         4 |       union DEEP::(anonymous at ./bf.h:4:41)<SP>
         4 |         int32_t i
         4 |         float f
         8 |       int32_t y
        12 |     int32_t x
           | [sizeof=16, align=4]
```

(`<SP>` marks a trailing space: an anonymous member is its type and an empty
name.) Bit-fields print `4:0-0 | uint32_t flag`, an unnamed bit-field
`68:0-2 | uint32_t<SP>` and a zero-width one `72:- | uint32_t<SP>`; an array
is `int32_t[4] a`, a flexible array member `int32_t[] fam`; a named member of
record type is followed by its own fields one level deeper; an empty struct
member has none. Size for the headers alone, no wrapper: `SDL3/SDL.h` 30,309
bytes, `raylib.h` 1,647, `stdio.h` 1,171.

**What differs between 21 and 22**: only the type text of a nested anonymous
record, `union DEEP::(anonymous at ./bf.h:4:41)` against `union
DEEP::(anonymous struct)::(anonymous at ./bf.h:4:41)` (`diff` of the two
outputs, four lines, all that one). The offsets, the `|`, the two-space
indentation, the empty name and the bit-field column are the same. So a reader
that never reads a TYPE's text, only offsets, indentation, the name and the
bit-field colon, holds on both. 18.1.3, 20.1.8 and 23.1.1 are unread
(requests below).

## 4. The route I built: (1e), the layout dump proposes names, C judges every verdict

In my copy, in `selfhost/`, building and running (all four changes in one
lane; the files are `<scratchpad>/186-compiler-engineer/selfhost/`):

- **`cli/layout.hero`, new**: the runner. One clang process on a cold build
  for every program holding a group record with fields: `clang -std=gnu11
  -fsyntax-only -Xclang -fdump-record-layouts` over a unit of the program's
  headers and one wrapper per record, `struct hero_ly_<decl> { T v; };
  _Static_assert(sizeof(struct hero_ly_<decl>) > 0, "");`. Cached under
  `build/layout-<key>/` with the headers it read recorded by digest, as
  `cli/pointee.hero` does (its `unit_path` and `with_search` reused, not
  copied). A second process only when the check unit holds an assertion; a
  complete record over a struct with no anonymous member needs none. A dump
  that does not compile returns nothing, as the pointee dump does: the build's
  own compile reports the header on the author's line.
- **`emit/layout_text.hero`, new**: the dump unit, the reader (five facts:
  two-space levels after `| `, the empty name of an anonymous member, a deeper
  next line for a record, `:` in a bit-field's offset, `[]` before a flexible
  member's name; a type's text is never read), and the check unit. Every
  verdict is a `_Static_assert` on the header's own offsets and sizes:
  - **a member the record leaves out** (non-`partial` records): `sizeof(m)
    == 0`, or `m` shares a byte with a declared field of the same anonymous
    member (`__builtin_offsetof` and `sizeof`, half-open ranges, both
    non-empty), or `__builtin_classify_type(*(T *)0) == 13` where `m` is a
    direct member, or one of an anonymous member no declared field is in.
    For a bit-field, which has no offset in C, the one number read from the
    text: whether a declared member's bytes hold the bit-field's first byte.
    Marker `heroes-ffi-missing T m`, read as today's `ffi_incomplete_record`
    with its message unchanged.
  - **a field in a union, compared** (records `extern_union.used` puts in
    `operated`, any arity): a declared field inside an anonymous member that
    shares a byte with any member beside it, declared or not. Marker
    `heroes-ffi-overlap T f`, `ffi_union_field`, new text.
  - **two fields of one union, built** (`constructed`, two or more declared):
    each pair of declared fields in one anonymous member that share a byte.
    Marker `heroes-ffi-shared T f g`, `ffi_union_field`, new text.
  - **a declared bit-field** (defect 156): refused before the unit whose
    `sizeof` and `&v->flag` are clang's hard errors. Marker
    `heroes-ffi-bitfield T f`, `ffi_field_type` on the field's line, new text.
  Whether a member is in a union is never read from the dump: an anonymous
  STRUCT's members do not share bytes, so its assertion passes (measured,
  `ans`, below), and the classify assertion of `extern_union.hero` stays the
  oracle for a union `T`.
- **`emit/ffi_record.hero`**: `layout_record` reads the four markers;
  `incomplete_record`, which read clang's prose *missing field 'c'* through
  the `#line` mapping, is deleted with `caret_of` and `record_at_line`.
  `emit/ffi.hero` calls the new reader where the old one was.
- **`emit/extern_record.hero`**: the positional probe `completeness_probes`
  and its three pragmas are deleted. The main unit no longer carries any
  completeness probe; that is what removes defect 150's and u17's warnings.
- **`cli/assemble.hero`**: `layout.check` runs in `round` right after the
  pointee check, before any unit compiles, so its failure rides the same
  mapping (`produce.blamed`, `ffi.explain`).

### The probes and the shapes beside them, trunk against mine

`heroes run` on each, the trunk's compiler and mine (`work/shapes/` holds
mine; `mine.h` is my own header for the padded, zero-sized, flexible,
anonymous-struct and bit-field shapes):

| case | trunk | mine |
|---|---|---|
| `u17` (`SA` built naming `i` and `f`) | exit 0, `1056964608`, two warnings | exit 1, *`i` and `f` are the same bytes of `SA`* |
| `u18` (`SA` compared) | exit 0, `true`, a warning | exit 1, *`SA` holds `i` inside a union* |
| `one_arm` (`SB` naming `kind`, `b`, compared) | exit 0, `true` | exit 1, *`SB` holds `b` inside a union* |
| `one_arm_union` | exit 1, *`UB` is a `union`* | the same |
| `u07` (`x` left out) | exit 0, `12` | exit 1, *does not name `x`* |
| `u16` (`kind` left out) | exit 0, `15` | exit 1, *does not name `kind`* |
| `u19` (`S3` naming `a`, `c`) | exit 1, *does not name `c`* | exit 1, *does not name `b`* |
| `u08`, `u08_mine` (one arm, built and read) | exit 0, `15`; `7` `12` | the same, silent |
| `read.hero` (defect 150) | exit 0, `7`, the excess warning twice | exit 0, `7`, silent |
| `bf.hero` (defect 156) | exit 2, clang's text | exit 1, *`flag` is a bit-field of `BF`*, on `flag`'s line, and the same for `rest` |
| `bf_omit` (`BF` naming `kind`) | exit 1, *does not name `flag`* | the same |
| `bf_partial` | exit 0, `1` | the same |
| `deep` (`DEEP`, `i` and `f` built) | exit 0, `1056964608`, excess warning | exit 1, *`i` and `f` are the same bytes* |
| `deep_eq` (`DEEP`, one arm, compared) | exit 0, `true` | exit 1, *holds `i` inside a union* |
| `deep_omit_y` (`y` left out) | exit 1, *does not name `x`*, which it declares | exit 1, *does not name `y`* |
| `deep_one` (one arm, built) | exit 0, `5` | the same |
| `s2u` (one arm of each of two unions, built) | exit 0, `5` | the same |
| `s2u_two` (`s` and `b` of the second union) | exit 0, `2`, excess warning | exit 1, *`s` and `b` are the same bytes* |
| `s2u_eq` | exit 0, `true` | exit 1, *holds `i` inside a union* |
| `sn` (`SN` holding a named `W2 u`, compared) | exit 1, with the excess warning | exit 1, *`W2` is a `union`*, no warning |
| `padun` (padded union, one member, built) | exit 0, `7` | the same |
| `padu` (`PADU`, small union in a padded struct, compared) | exit 0, `true` | exit 1, *holds `a` inside a union* |
| `padu_build` (`a` and `b` built) | exit 0, `3`, excess warning | exit 1, *`a` and `b` are the same bytes* |
| `one` (a union of one member, compared) | exit 0, `true` | the same: nothing beside it shares a byte |
| `ans` (anonymous STRUCT, compared) | exit 0, `true` | the same: C says its members are apart |
| `ans_omit` | exit 1, *does not name `y`* | the same |
| `uas_q` (`kind` and the 64-bit arm `q` of `union { struct { a; b; }; q; }`) | **exit 1, *does not name `b`*, a false refusal** | exit 0, `21474836484` |
| `uas_a` (`a` without `b`) | exit 1, *does not name `b`* | the same |
| `uas_ab` | exit 0, `9` | the same |
| `reg_all` (`union { uint8_t all; struct { lo : 4; hi : 4; }; }` named by `all`) | exit 0, `3` | the same |
| `ze` (GNU empty struct member left out) | **exit 1, *does not name `x`*, false** | exit 0, `true` |
| `zs` (GNU `int32_t z[0]` left out) | **exit 1, *does not name `x`*, false** | exit 0, `2` |
| `fam` (flexible array member left out) | exit 0, `3` | the same |
| `partial_sa` (`partial`, `i` and `f` built) | exit 0, `1056964608`, overrides warning | exit 1, *`i` and `f` are the same bytes* |
| `partial_sa_read` | exit 0, `1` | the same |

So the route answers every case the brief lists, and three refusals the
trunk makes of correct programs (`uas_q`, `ze`, `zs`) go away. `reg_all`
was refused by my first build (the bit-field branch had no C question to ask)
and is what added the one number read from the text.

`./heroes test selfhost/main.hero` with my compiler: **1007 tests, all
passed**, exit 0.

### The census on my first build, and the cost it found

`work/cmp.sh` compares exit code, stdout and stderr per file, trunk against
mine. On 446 of the 447 files: **397 identical on all three**; 27 differ in
stdout only, and every differing line is a deletion of the probe's own lines
(`hero_ffi_complete_*`, its three pragmas, its `#line`s) or a generated-file
`#line N "x.c"` that moved up by the lines deleted (checked by filtering the
`diff`: nothing else); the other 22 are all under `docs/panel/186-briefs/probes/`
and are exactly the moves the table above names (8 exit 0 to 1, `bf.hero` 2
to 1, the excess warnings gone from 11, u19's *`c`* now *`b`*). **No tracked
program outside this sitting's probes changed its exit code or its stderr.**

The 447th, `tests/golden/run/fixedbugs-140-extern-records-a-thousand-deep-build.hero`
(defect 140's case: records nested by value a thousand deep in one group),
did not finish, and that is the finding of this section. **The record-layout
dump is cubic in the depth of by-value nesting**: clang prints a block for
every record it lays out, and expands every NAMED record member inline,
indented two spaces per level, so the dump for that one program is
**693,681,863 bytes** (`ls -la` of `build/layout-01699b281dcc4c91/dump-*.txt`),
and my compiler sat at 1.5 GB resident (`ps -o rss`, 1,512,144 KB) inside a
reader that scanned every row once per record. I stopped it (exit 143). A
real header does not nest like that (`SDL3/SDL.h`'s whole dump is 30 KB), but
the tracked tree holds a program that does, and the route must not turn a
program that builds into one that exhausts a CI runner's memory: robustness,
CLAUDE.md § Precedence rank 3, above speed.

What the repair is, below: a C-judged screen first, so the dump is read only
for a record whose binding C cannot vouch for; a reader linear in the dump;
and the condensed check text cached, never the dump.

## 5. The route as it stands: a C-judged screen first, the dump only behind it

Section 4 describes the first build; this is the second, the one I adopt.
What changed, and why each change was measured into it:

- **`emit/layout_screen.hero`, new: the screen.** One never-called function
  per group record with fields, `#line <decl + 1> "hero_ly_screen"`, its body
  a positional initialiser in the record's declared order under three
  locally-armed errors (`-Wmissing-field-initializers`,
  `-Wexcess-initializers`, `-Wmissing-braces`), an aggregate's slot
  designating inside itself (`{.x = 0}` for a record, `{[0] = 0}` for an
  array), and `(void)sizeof(v.f);` per field. A record whose function
  compiles clean names exactly the struct's members, none in an anonymous
  member, none a bit-field, and needs nothing more; every line clang prints
  at `hero_ly_screen:<n>` sends record `n - 1` to the dump. One clang
  process, `-ferror-limit=0`. A `partial` record is screened for bit-fields
  only, and dumped only when the program builds it from two or more fields.
  The designator is what closes the screen's one hole I found by reasoning
  and then measured: a declared AGGREGATE at the slot of an anonymous union
  whose first arm it is (`AGG { int32_t kind; union { PT p; int64_t raw[2];
  }; }` named by `kind` and `p: PT`, and `ARR` with `bytes: u8[4]` over
  `union { uint8_t bytes[4]; uint32_t word; }`): `{}` there is silent, `{.x =
  0}` is *field designator 'x' does not refer to any field*, `{[0] = 0}` is
  an array designator into a union. Measured: `agg_eq` and `arr_eq` exit 1,
  *holds `p` / `bytes` inside a union*; the trunk prints `true` for both;
  `agg_read` and `arr_read` exit 0, `0` and `7`, as on the trunk.
- **`cli/layout.hero`**: screen, then the dump for the flagged records only,
  then the check unit only when it holds an assertion; what is cached under
  `build/layout-<key>/` is the check unit's TEXT (`check-<sum>.c`, empty for
  none) and its verdict, never the dump. A dump that does not compile is left
  to the build's own compile, which names every record's type in its field
  assertions; a screen that flags nothing and fails is the headers', left the
  same way.
- **`emit/layout_text.hero` split** (it measured 373 in the layout unit, over
  the ceiling): the reader stays (232), the assertions move to
  **`emit/layout_check.hero`** (158). The reader now indexes every wrapper's
  head in one pass and reads each block once (the first build scanned every
  row once per record: that, not only the dump's size, is what sat at 1.5 GB).

**Every probe and shape, again, on this build**: the table in section 4 holds
row for row (re-run after each change, the last on the build that also holds
the split: identical exit codes, outputs and first diagnostics), plus `agg_eq`,
`agg_read`, `arr_eq`, `arr_read` above.

**The census, again, on this build** (`work/cmp2.txt`): all 447 complete.
Trunk 229 exit 0, 217 exit 1, 1 exit 2; mine **221 exit 0, 226 exit 1, 0 exit
2**, the 9 moves all under `docs/panel/186-briefs/probes/` (the 8 of section 4
and `bf.hero`). Files printing any clang warning: 30 on the trunk, **15** on
mine (the 15 left are the `-Wunused-function` ones under
`docs/panel/17[5-7]-briefs/`); files printing an initializer warning: **15 to
0**. Stdout differs in 45 files: in every one of the 37 that still emit C, the
only lines are the deleted probe, its pragmas, its `#line`s, and the
generated-file `#line N "x.c"` that moved up by them (a filtered `diff`: zero
other lines added or removed); the other 8 are the refusals that now emit no
C. **No tracked program outside this sitting's probes changes its exit code
or its stderr**, the thousand-deep case included (exit 0, stderr identical:
the screen vouches for all thousand records and no dump is made).

**`suite_warnings`** (`./heroes run tests/harness/main.hero -- ./heroes
warnings`, output to a file and read whole): **280 passed, 0 failed**, on the
build before the split; re-run on the final build below.

**`./heroes test selfhost/main.hero`**: **1008 tests, all passed** on the
build before the split (1007 on the first build; the screen added one).

## 6. Requests to the coordinator (clang 18.1.3, and the Windows box)

The route rests on three clang behaviours, all measured identical on Apple
clang 21.0.0 and Homebrew clang 22.1.8 and unread anywhere else. The files are
self-contained (headers copied beside them), in
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/186-compiler-engineer/work/request/`:
`lay.c`, `screen.c`, `check.c`, and the headers `u.h`, `s3.h`, `one_arm.h`,
`bf.h`, `mine.h`, `agg.h`, `shapes2.h`. The expected outputs from this Mac are
beside them: `lay.usr.out`, `lay.llvm@22.out`, `screen.usr.err`,
`check.usr.err`.

In the `silkeh/clang:18` container (and, if the coordinator can, an Ubuntu
24.04 container with `apt install clang`, which is 18.1.3 exactly), and on
the Windows box (`clang` 23.1.1; the CI's 20.1.8 if reachable), from inside
that directory:

```sh
clang --version
clang -std=gnu11 -fsyntax-only -Xclang -fdump-record-layouts lay.c > lay.out 2> lay.err; echo "lay exit $?"
clang -std=gnu11 -fsyntax-only -ferror-limit=0 screen.c 2> screen.err; echo "screen exit $?"
grep -o 'hero_ly_screen:[0-9]*' screen.err | sort -t: -k2 -n -u | tr '\n' ' '; echo
clang -std=gnu11 -fsyntax-only -ferror-limit=0 check.c 2> check.err; echo "check exit $?"
grep -o 'EXPECT-[A-Z]*-[0-9]*' check.err | sort -u | tr '\n' ' '; echo
```

and hand back `lay.out` whole with the four printed lines. What each must
read for the route to stand, as it reads here:

1. **The dump**: `lay exit 0`, empty `lay.err`, and `lay.out` equal to
   `lay.usr.out` once every parenthesised span is masked (`sed 's/(.*)/(X)/'`
   on both, then `cmp`): the offset column, the `|`, the two-space levels,
   the empty name after an anonymous member, `N:a-b` for a bit-field, `[]`
   before a flexible member's name, `| [sizeof=` closing each block, `|
   struct hero_ly_<n>` opening each wrapper's. On Windows, also whether the
   lines end `\r\n` (the reader drops a `\r`, unmeasured there) and whether
   the MSVC target prints C records the same way.
2. **The screen**: `screen exit 1` and exactly `hero_ly_screen:1 2 3 5 6 7 9
   11 12` (here: 1, 2, 3, 5, 6, 7, 9, 11 and 12 flagged; 4, 8, 10, 13 and 14
   clean). Above all whether 18.1.3 accepts `#pragma clang diagnostic error
   "-Wexcess-initializers"` and still says *missing field* for `{0, 0}` over
   a three-member struct.
3. **The verdicts**: `check exit 1` and exactly `EXPECT-FAIL-1 EXPECT-FAIL-11
   EXPECT-FAIL-6 EXPECT-FAIL-7 EXPECT-FAIL-9` (every `EXPECT-PASS` passing):
   `__builtin_offsetof` through two anonymous levels, `sizeof` of a
   zero-length array and of an empty struct member being 0, `_Generic` over a
   bit-field, and the classify split.

I am not waiting on these to give my verdicts; each is a condition on them
(section 8).

## 7. Every route, judged

| route | verdict | what I built or measured for it |
|---|---|---|
| **(1a)** per-pair `offsetof`/`sizeof` ranges over declared fields | **object** | Not built as a route; its predicate is my `shares` (half-open, both non-empty, so a GNU zero-sized member never matches: `zs`, `ze` exit 0). Alone it is blind to one declared arm (`one_arm`, `padu`, `deep_eq`, `s2u_eq`, `agg_eq`, `arr_eq` all exit 0 `true` on the trunk), and over every pair it is 120 conjuncts for `Matrix`. Asked only for pairs the dump puts in one anonymous member, it is what (1e) uses |
| **(1b)** designated probe under `-Winitializer-overrides` | **object** | Unbuilt. Blind to one arm by construction (critic § 1), reads clang's prose, says nothing about a field left out, and refuses `read.hero` unless gated (critic § 9). My `heroes-ffi-shared` assertion answers its construction half with offsets and a marker |
| **(1c)** refuse any record over a struct holding an anonymous union | **object** | Unbuilt. Knowing the struct holds one costs at least my screen (141 lines), so it is not cheaper; and it refuses seven programs that build and print the right answer on my compiler (`u08`, `u08_mine`, `deep_one`, `s2u`, `uas_q`, `agg_read`, `arr_read`) |
| **(1d)** a nested group in the record's declaration | **veto** | design.md §1.7: a surface form the lexer and parser, the checker (a nested field namespace and its construction), the formatter and every re-printer (CLAUDE.md § 9's walk), the spec grammar and the lowering would all carry, to restate a fact the header already holds and clang prints for free. Not one case of this sitting needs it: every one is answered above with no surface form |
| **(1e)** the header's layout read from clang | **approve, built** | Sections 4 and 5: the census moves nothing outside this sitting's probes, `suite_warnings` 280 and 0, the compiler's tests all passed. Conditioned on the requests of section 6 |
| **(1f)** classify OR sum of sizes | **object** | Unbuilt; the critic measured `PADU` passing it, and it is blind to one arm (`one_arm`). Linear but incomplete, and (1e) gives the same coverage and more |
| **(1g)** refuse `==`, `hash`, a map key on every group record not proven union-free | **object** | The census refutes panel 077's historian's zero: `tests/golden/run/fixedbugs-a-group-record-in-every-container.hero` compares `Color` four times and keys a map by it (lines 47-59), and 24 tracked programs outside `docs/panel/` carry the classify assertion (construction at two fields or a comparison; how many compare is unsplit, unrun). With my screen as its proof it would be cheap (no dump), but it answers nothing about Q2 (`u07`, `u16`, `u19`) |
| **(1h)** construction-arity form (declare every member, build naming one) | **object, filed** | Unbuilt. It is the only route that gives Q4's program a form, and it is NOT what (1e) unlocks: the layout is known in `build`, after the checker, so `missing_fields` for a group record would move from `check` to `build`, and `heroes check` would accept a construction `build` refuses. Principle 0: no tracked program needs it (the census holds none) |
| **Q2** the probe's form | **approve the layout form** | Per member left out: `sizeof(m) == 0`, or a byte shared with a declared field of its anonymous member, or `__builtin_classify_type(T) == 13` for a direct member or an untouched arm. Names `x`, `kind` and `b` for `u07`, `u16`, `u19`. A union named by one member is complete (the code comment's reading, now asked of C), and panel 073's *claims completeness falsely* was the `==`/`hash` half, which stays refused, now for a struct's union too. **(2a)**, a designated probe under a warning: object, the critic measured no clang reporting a designated omission |
| **Q3** where they meet | **one screen, one dump, one check unit** | Completeness for every non-`partial` group record; the union assertions only for `extern_union.used`'s records (compared at any arity, built from two or more fields), so `read.hero` is never asked. Costs in section 8 |
| **Q4** what the author writes | the record names ONE member of each union to build it, any members to read it | `SA` naming `kind`, `i` (or `f`), `x` builds and reads; naming both is read-only; comparing either is refused, `partial` or not. No form builds an `SA` holding `f` and reads `i` from one C returns ((1h) is the only route). A bit-field: refused on its own line, *leave it out and mark the record `partial`* |

**What the route needs outside `selfhost/`.** No surface form, no new
diagnostic code: `ffi_union_field`, `ffi_incomplete_record` and
`ffi_field_type` carry three new message texts (*`i` and `f` are the same bytes
of `SA`*, *`SA` holds `i` inside a union*, *`flag` is a bit-field of `BF`*),
each owing a golden under `tests/golden/unsupported/` with its annotation.
`ffi_union_field`'s old text *`T` is a `union` in* stays only where `T` is a
union. **A spec sentence**, for the spec-warden to price (unmeasured here): §
13's *A group's `record` is the header's struct: all its fields* is false for
`SA` as written; something of the shape *a field may lie in a union of the
header's struct: building a record that names two members of one union, and
comparing one that names any, are compile errors; a bit-field is never a
field*. And `tests/emission/` re-blessed: 24 blessed files hold
`hero_ffi_complete_` (`grep -rl`), every one losing the probe's lines and
nothing else, as the census shows.

*Correction, 2026-10-02, same session: (1c)'s row counts seven; it is eight,
`reg_all` (a struct holding `union { uint8_t all; struct { lo : 4; hi : 4;
}; }`) being the eighth that builds and prints the right answer on mine.*

### Panel 073's three readings, on real SDL3

Written against `/opt/homebrew/include/SDL3/SDL.h` through a header of mine,
`work/sdl/sdlx.h`, which only adds three `static inline` makers of an
`SDL_Event` (a key, a quit, a mouse motion), `package "sdl3"`, run on both
compilers:

| program | trunk | mine |
|---|---|---|
| `read_two` (`SDL_Event` naming `type` and `key`) | exit 0, `768 97`, *excess elements in union initializer* | exit 0, `768 97`, silent |
| `one_member` (`SDL_Event` naming `type`) | exit 0, `256` | the same |
| `four_arm` (`type`, `key`, `quit`, `motion`, a `describe` over the arms) | exit 0, `key 97 quit 42 motion 3.5`, the excess warning | the same output, silent |
| `compared` (`==` on `SDL_Event`) | exit 1, *`SDL_Event` is a `union`* | the same |
| `by_tag` (`record KeyArm tag SDL_Event` beside `record SDL_Event`) | exit 1, `ffi_tag_is_a_union` | the same |

So the read-only binding, the one-member binding and the four-arm loop stay
legal, and lose defect 150's warning. **The second reading, one record per
arm by `tag`, is refused on the trunk already for a union** (`SDL_Event` is
`typedef union SDL_Event`), as the critic found it refused for a typedef'd
anonymous struct (`ffi_unknown_tag`): it is a premise the brief carries from
panel 073 and no compiler of this tree honours for either shape. Not this
route's to change; it bears on Q4.

## 8. Cost, prediction, condition

**Lines, in `suite_layout.hero`'s unit** (non-blank, outside `test` blocks,
comments counted; `work/codelines.awk`, a reimplementation of `code_lines` at
`suite_layout.hero:686-701`, the harness's own `layout` run in the gate below):

| file | trunk | mine |
|---|---|---|
| `selfhost/emit/layout_text.hero` (new: dump unit, reader) | 0 | 232 |
| `selfhost/emit/layout_check.hero` (new: the assertions) | 0 | 158 |
| `selfhost/emit/layout_screen.hero` (new: the screen) | 0 | 141 |
| `selfhost/cli/layout.hero` (new: the runner and its cache) | 0 | 180 |
| `selfhost/emit/ffi_record.hero` | 183 | 180 |
| `selfhost/emit/ffi.hero` | 194 | 188 |
| `selfhost/emit/extern_record.hero` | 207 | 90 |
| `selfhost/cli/assemble.hero` | 215 | 231 |
| **total** | **799** | **1400 (+601; +513 without comment lines)** |

Against the whole compiler, 67,789 lines in the same unit across 359 modules
(`work/trunk-src/selfhost`, the same awk): **+0.9 %**. No module crosses the
ceiling 300; `extern_field.hero`, the brief's tight one at 286, is untouched;
nothing lands in the lexer, the parser, the checker, the IR or the
descriptors. All of it is the C boundary in `build`.

**Emitted C per record.** The program's own unit SHRINKS: one probe line per
non-`partial` group record and four lines per unit gone. The screen unit: one
line per group record with fields (plus a comment and a `#line`). The check
unit exists only for a record the screen flags, and holds: one assertion per
member left out that has bytes; one per declared field inside an anonymous
member of a compared record, with one `shares` conjunct (about 200
characters) per member beside it in that anonymous member; one per declared
pair in one anonymous member of a built record; one per declared bit-field. A
complete record over a struct with no anonymous member costs nothing there.

**Clang processes per `build`.** Cold, for a program holding a group record
with fields: **+1** (the screen) always, **+1** (the dump) when the screen
flags a record, **+1** (the check unit) when the check holds an assertion.
Warm: **0**, reading `screen.deps.txt` and digesting the headers it names
(its own memo, not yet shared with the pointee check's or the units'), then
`check-<sum>.c` and, when that is not empty, the verdict. Not timed (the
sitting forbids it).

**The strongest reason against my own route**, which is what this seat is
for: it adds a THIRD reader of clang's text to the C boundary (after the
pointee check's JSON and the tag round's prose), and this one reads a
`-Xclang` option, which is clang's debugging output with no stability
promise. What I measured makes its failure LOUD rather than silent: a block
the reader cannot find is *the record-layout dump has no block for `T`*, an
internal error at exit 2, and a line inside a block without `| ` is the same.
What I did not build and would require before it lands: a premise-death
control in the style of `extern_union.hero:111-122`, one fixed wrapper the
dump unit always carries and whose tree the reader must find exactly, so that
a clang that changes the format fails every build that needs the dump, on its
first one, naming the dump, instead of failing only the program that meets
the changed line.

**Prediction**: at the batch that lands this route, (a) the harness's `layout`
reads the four new modules each at or under 300 and their sum at or under
760 in its unit (711 here); (b) `warnings` reads 0 failed; (c) a census of
`build --emit-c` over the tracked `.hero` files, trunk against the batch,
moves no exit code and no stderr outside `docs/panel/186-briefs/probes/` and
the batch's own new goldens; (d) on clang 18.1.3 the three files of section 6
read as section 6 says. Any one of the four false falsifies it.

**Condition**: I move from approve to object if clang 18.1.3, 20.1.8 or
23.1.1 reads differently on the dump's five facts, the screen's flagged set or
the `EXPECT` set (section 6); if a header is found where the screen is clean
and the layout check would refuse (a hole in the screen's argument, which I
reasoned and measured on `AGG` and `ARR` but did not prove); or if the dump
for the flagged records of a real header passes 10 MB. I move to veto only if
the route is taken with a surface form ((1d)) or without the premise-death
control above.

*Correction, 2026-10-02, same session, to section 8's last sentence: a veto
is this seat's only for a core construct or a breach of the ceiling (its
mandate, design.md §1.1 and §1.7), and a missing premise-death control is
neither. So: veto only if the route is taken with a surface form ((1d));
object if it is taken without the control. And the control is no longer a
condition: I built it, below.*

## 9. The premise-death control, built

`emit/layout_text.hero`'s dump unit now always carries one record of every
shape the reader knows, `struct hero_ly_control { struct { int a; union { int
b; float c; }; unsigned d : 2; unsigned : 1; int e[0]; } v; }`, and
`emit/layout_check.hero` refuses to judge anything unless its tree comes back
exactly as written (`control_holds`: depth, name, nested, bit-field for each
of its seven lines). A clang that prints the dump another way fails the first
build that needs the dump, at exit 2, with *this clang's record-layout dump is
not the shape this compiler reads: its control record ... did not come back as
written*, instead of failing only the program that meets the changed line.
Measured: the control's dump on Apple clang 21 and Homebrew clang 22.1.8 is
the seven lines the reader expects (`work/request/control.c`), the compiler's
own tests carry a case where a dump without the control and one with only the
control are both refused, and on the build that holds it: **1009 tests, all
passed**; every probe, shape and SDL3 program of sections 4, 5 and 7 gives the
same exit code, output and first diagnostic as before it (`diff` of the two
result files, whole lines, identical).

Its cost: `layout_text.hero` 232 to **257**, `layout_check.hero` 158 to
**163**, so the route's total is **+631** in the layout unit (1430 against
799), still no module over 300. The prediction's sum, (a) in section 8, is
then 741 against its bound of 760.

**For section 6**: add `work/request/control.c` to the files, and `clang
-std=gnu11 -fsyntax-only -Xclang -fdump-record-layouts control.c` to the
commands; it must print, under `0 | struct hero_ly_control`, the `v` line and
then exactly `int a`, an anonymous union line ending in a space, `int b`,
`float c` one level deeper, `8:0-1 | unsigned int d`, `8:2-2 | unsigned int `
and `12 | int[0] e`.

## 10. One more defect in my own route, found and repaired: a kept reading

Counting the census's cache after section 9 showed that build replaying
`check-<sum>.c` files the build before it had written: the key is
`t.fingerprint` (the release `VERSION` plus the cache layout,
`cli/toolchain.hero:89`, a constant between releases) and the screen's text,
and the screen's text had not changed, so a compiler with a new reader read
the old reader's answer. It is the class `cli/pointee.hero:111-119` records
paying for on 2026-09-04. Repaired: `cli/layout.hero` now keeps only what
clang said (the screen's stderr, `screen-<sum>.txt`, and the dump,
`dump-<sum>-<digest of the dump unit>.txt`) and re-derives the flagged set
and the check unit on every build; the verdict stays keyed by the check
unit's own text. Measured cold and warm on `u17` (exit 1, *`i` and `f` are
the same bytes*), `read.hero` (exit 0), `uas_q` (exit 0) and `u19` (exit 1,
*does not name `b`*): the same verdict both times. `cli/layout.hero` 180 to
**194**.

## 11. The final build, gated

On the build that holds sections 5, 9 and 10, with `build/layout-*` cleared
first so every answer is asked cold:

- `./heroes test selfhost/main.hero`: **1009 tests, all passed**.
- `./heroes run tests/harness/main.hero -- ./heroes layout`: **4 passed, 0
  failed** (the harness's own line count over every `selfhost/` module).
- `./heroes run tests/harness/main.hero -- ./heroes warnings`: **280 passed,
  0 failed**.
- Every probe, shape and SDL3 program: identical, whole lines, to the build
  before it (`work/shapes-final4.txt` against `work/shapes-final5.txt`).
- **The census**, 447 of 447 complete: trunk 229 / 217 / 1 (exit 0 / 1 / 2),
  mine **221 / 226 / 0**; 397 files identical on all three streams; every
  exit-code or stderr move is under `docs/panel/186-briefs/probes/` (8 exit 0
  to 1, `bf.hero` 2 to 1, 11 losing their initializer warnings, `u19`'s
  `c` to `b`); 24 tracked files outside it differ in stdout only, every line
  of it the deleted probe and the `#line`s it moved; files printing a
  warning 30 to **15**, an initializer warning 15 to **0**. The comparison is
  line for line the one of section 5 (`diff` of `cmp2.txt` and `cmp5.txt`,
  sorted: empty). Of the 72 distinct screens the census asked, 24 led to a
  dump and 10 to a check unit run (`ls` of `build/layout-*`).

**The cost, final** (the table of section 8, corrected): `layout_text.hero`
257, `layout_check.hero` 163, `layout_screen.hero` 141, `cli/layout.hero`
194, `ffi_record.hero` 180, `ffi.hero` 188, `extern_record.hero` 90,
`assemble.hero` 231: **799 to 1444, +645** in the layout unit (+541 without
comment lines), +0.95 % of the compiler; no module over 300. *Correction to
section 8's prediction (a)*: the four new modules sum to 755, so its bound of
760 is restated as **800**, set now, before the batch.

## 12. The seat's verdict, in the panel's structure

- `verdict`: **approve** (1e), the route built here: screen, dump behind it,
  every verdict a C assertion; **veto** (1d); **object** (1a), (1b), (1c),
  (1f), (1g), (1h); Q2 approve the layout form, object (2a).
- `section`: design.md §1.7 (core plus elaboration: nothing here is core;
  (1d) would be) and §1.1 (simplicity sets the ceiling), with §1.12 and §4.19
  for what the route must do; CLAUDE.md § Precedence rank 3 for the screen.
- `implementation_cost`: +645 lines in `suite_layout`'s unit, all at the C
  boundary in `build`: `selfhost/emit/layout_text.hero` 257,
  `selfhost/emit/layout_check.hero` 163, `selfhost/emit/layout_screen.hero`
  141, `selfhost/cli/layout.hero` 194 (new), `selfhost/emit/extern_record.hero`
  207 to 90, `selfhost/cli/assemble.hero` 215 to 231, `selfhost/emit/ffi_record.hero`
  183 to 180, `selfhost/emit/ffi.hero` 194 to 188. Zero lines in the lexer,
  parser, checker, IR, descriptors or ownership. +1 clang process per cold
  build holding a group record, +2 more only behind a flagged record, 0 warm.
- `needed_for_self_hosting`: **no** (`selfhost/` binds no group record; it is
  §1.12 robustness at the C boundary, not compiler need).
- `argument`: The defect is a fact the compiler cannot know before clang runs
  (panel 077's engineer), so the answer is clang's: a C-judged screen vouches
  for every record that names exactly its struct's members, and only the rest
  are laid out, the dump proposing names and C judging bytes. It fixes all
  nine brief cases, three false refusals the trunk makes, defect 150's
  warnings and defect 156's exit 2, moves no tracked program, and adds no
  construct. (1d) adds a surface form for information the header already
  holds; the others are blind to one declared arm or to Q2.
- `prediction`: at the batch that lands it, (a) `layout` reads the four new
  modules each at or under 300 and together at or under 800 (755 today);
  (b) `warnings` 0 failed; (c) a census of `build --emit-c` over the tracked
  `.hero` files moves no exit code or stderr outside panel 186's probes and the
  batch's new goldens; (d) clang 18.1.3 reads section 6's files as stated.
- `condition`: object if 18.1.3, 20.1.8 or 23.1.1 differs on the dump's
  facts, the screen's flagged set, the `EXPECT` set or the control (section 6
  and 9); if a header is found where the screen is clean and the check would
  refuse; or if a real header's flagged dump passes 10 MB. Veto if the
  sitting adopts (1d).

Status: complete on this Mac, 2026-10-02. Waiting on the coordinator for
section 6's measurements (clang 18.1.3 in the `silkeh/clang:18` or an Ubuntu
24.04 container, and the Windows box), with `control.c` added by section 9;
every verdict above stands on them as its condition. The copy holding the
route is `<scratchpad>/186-compiler-engineer/selfhost/` (eight files: four new,
four changed), its census `work/census-mine/`, its comparison `work/cmp5.txt`.

## 13. Against the ffi-pragmatist's objections, on the coordinator's readings

**The readings** (`coordinator-platform-readings.md` § The compiler-engineer's
requests, outputs in `<scratchpad>/p186/ce-out/`): Debian clang 18.1.8,
Debian clang 20.1.8 and **Ubuntu clang 18.1.3, the CI's own**, each read
`lay.c`, `screen.c`, `check.c` and `control.c` exactly as Apple 21 and
Homebrew 22 do. The Windows box (23.1.1) is unrun for my files; the
pragmatist's `plat_lay.c` on it printed Homebrew 22's spelling inside the
parenthesis.

**Objection 1: two texts across clangs.** My reader never reads the text the
two spellings differ in. It reads the offset column, the `|`, the two-space
levels, whether a member line ends in a space (the empty name), a `[]` just
before the name, and its own wrapper's head `| struct hero_ly_<n>`, which is
not a parenthesised type. On five clangs, both spellings among them, the
dumps are byte-equal once every parenthesised span is masked. If a clang
changed one of the facts the reader does use (say, trimmed the trailing space
of an anonymous member), the control record of section 9 would come back
wrong and every build that needs the dump would stop at exit 2, naming the
dump. So the objection holds against reading the type text, and my reader
does not read it.

**Objection 2: under `-fsyntax-only` the dump prints only what clang laid
out.** True, and the reason my unit forces the layout of exactly what it asks
about: every wrapper carries `_Static_assert(sizeof(struct hero_ly_<n>) > 0,
"")`, and so does the control. The reader reads only those blocks, and a
missing one is the internal error *the record-layout dump has no block for
`T`*, never a silent pass. `plat_lay.c` declared tentative definitions
(`SA hero_l_SA;`), which `-fsyntax-only` does not lay out. That is why it
printed one record where `lay.c` prints all eleven wrappers on every clang
read.

**Objection 3, from the same section: blind to `sa_handler` and `s6_addr`.**
Conceded, and it found a real defect in my route. As of section 11 my build
refused libc's `struct sigaction` naming `sa_handler`, which runs today.
Measured on my own `work/libc/sig_read.hero`: *`SigAction` does not name
`__sigaction_u`*. It did the same to my own `nuw_read.hero` (a field reached
through a header macro into a named union). Neither the census nor my shapes
held that shape. Repaired: a declared field the dump does not show is asked
about BY NAME, since C's `__builtin_offsetof` follows the macro, and each such
term is wrapped in `#ifdef <name>`, so a misspelt field (no macro) is never
mentioned and stays the field assertion's to report. Measured, `work/libc/`,
both compilers (`lx.h` mine, over `<signal.h>`, `<netinet/in.h>` and two
shapes of my own):

| program | trunk | mine, final |
|---|---|---|
| `sig_read` (`record SigAction tag sigaction`, `sa_handler`, `sa_mask`, `sa_flags`, read) | exit 0, `2` | exit 0, `2` |
| `sig_eq` (the same record, `==`) | exit 0, `true` | exit 0, `true`: `sa_handler` fills `__sigaction_u` |
| `sig_both` (`sa_handler` and `sa_sigaction`, built) | exit 0, `2`, *excess elements* | exit 1, *`sa_handler` and `sa_sigaction` are the same bytes of `SigAction`* |
| `in6_eq` (`record In6 tag in6_addr`, `s6_addr: u8[16]`, `==` twice) | exit 0, `true false 1` | exit 0, `true false 1` |
| `nuw_read` (`kind`, and `nu_small` a macro for `u.small` in a named union of `uint8_t`/`uint64_t`) | exit 0, `2` | exit 0, `2` |
| `nuw_eq` (the same, `==` over values whose `u.big` differs) | **exit 0, `true`** | exit 1, *`NUW` holds `nu_small` inside a union* |
| `tsx_full` (two macros filling a named struct member) | exit 0, `11` | exit 0, `11` |
| `tsx_half` (one of the two) | exit 1, *does not name `nsec`* | exit 1, *does not name `ts`* (true, and less exact than the trunk's) |
| `typo` (`tsx_secs`, misspelt, beside a member left out) | exit 1, *does not name `nsec`* | exit 1, *does not name `ts`* |
| `typo2`, `typo3` (a misspelt field in a typedef'd record, nothing else wrong) | **exit 2**, *no member named 'bb' in 'S3'* | exit 1, *does not name `b`* |

`typo2` and `typo3` show a defect beside this one, **pre-existing and
unfiled**: on the trunk, a misspelt field of a typedef'd group record is exit
2, because `ffi_field.unknown_field` (`selfhost/emit/ffi_field.hero:93-124`)
needs the word `struct` in clang's *no member named 'x' in 'T'*. Searched for:
`no member named` and `unknown_field` in `docs/work/DEFECTS.md`, and
`unknown_field` under `tests/golden/`; no hit. My route moves it to exit 1,
but its message names the header's member, not the misspelling, so it is not
the repair. It is the coordinator's to file.

The repair's cost, measured: `layout_check.hero` 163 to **241**. The route's
total is **799 to 1522, +723** in the layout unit, and the four new modules
sum to **833**. **That falsifies the bound of 800 I restated in section 11**,
set before this repair. I withdraw it, and leave prediction (a) at what the
module-shape rule already requires: each new module at or under 300. On the
final build, `build/layout-*` cleared: **1009 tests, all passed**; `layout`
**4 passed, 0 failed**; `warnings` **280 passed, 0 failed**; every probe,
shape and SDL3 row identical to section 11's; the census of 447 **identical,
line for line, to section 11's** (`cmp8.txt` against `cmp5.txt`).

**Correction to section 3**: *the JSON route costs one clang process PER
RECORD, or the unfiltered dump* was true of filtering `RecordDecl`s and false
of the route: the pragmatist dumps `hero_lay_` functions whose bodies access
each declared field, which is one process and follows macros. What that dump
does not hold is a member the record leaves out (`u16`'s `kind`) or an arm it
does not declare (`SB`'s `q`); the pragmatist answers those with the
adjacency form, C arithmetic on the declared fields' offsets that names the
gap rather than the member. I have not built that route.

**And the pragmatist's veto on the any-arity rule (`in6.hero`).** My final
build keeps `in6_eq` and `sig_eq` (a field a macro reaches is refused for `==`
only when it does not fill its named union), and refuses `SA` naming `kind`,
`i`, `x` for `==` (an anonymous union's field, any arity, as the brief's Q1
states). That is two rules where the pragmatist asks for one. Making the
anonymous case the narrower one too is one predicate in `layout_check.hero`
(refuse only if some sibling arm extends past the declared field), unbuilt.
Which rule is the panel's to rule, not this seat's: neither costs the
ceiling.

**Settled.**

- **(1a)** ranges over declared fields: **object** (blind to one arm; in (1e) it is the predicate).
- **(1b)** designated probe under `-Winitializer-overrides`: **object** (blind to one arm, says nothing of a field left out; the same verdict on five clangs, which I do not dispute).
- **(1c)** refuse a struct holding an anonymous union: **object** (costs at least my screen, refuses eight programs that run).
- **(1d)** nested group in the declaration: **veto**, design.md §1.7.
- **(1e)** via `-fdump-record-layouts`, screen first, the route built here: **approve**. On 18.1.3, 18.1.8, 20.1.8, 21, 22; 23.1.1 owed.
- **(1e)** via the JSON member-access dump with adjacency: **approve as an alternative**, unbuilt by me. It adds no core construct and should cost the same order of lines; it must be built and censused before it is preferred.
- **(1f)** classify OR sum: **object**.
- **(1g)** refuse `==`/`hash`/map key unless proven union-free: **object** (`fixedbugs-a-group-record-in-every-container.hero`).
- **(1h)** construction-arity form: **object, filed** (it moves `missing_fields` from `check` to `build`; no tracked program needs it).
- **Q2**: **approve the layout form**; **(2a) object**.

Status: complete, 2026-10-02. The only reading still owed is 23.1.1 (the
Windows box, down since 13:16), which is the CI's Windows leg's to read when
the route runs there.
