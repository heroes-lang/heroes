# Panel 186, the shared brief: a field that lies in a C union, and the probe that counts a record's fields

Written 2026-10-02 by the coordinator; repaired the same morning on the
completeness critic's first pass (`docs/panel/186-reports/completeness-critic-briefs.md`,
eleven repairs, every one taken; the brief as it stood before is
`00-shared-before-the-critic.md`). The sitting's tree is the trunk at
`779139d0` (`ae08ed93`, lane recovery-b6 integrated, plus this sitting's
probes and defect 156's filing, docs only). Every number below names the
command that produced it, or the critic's section that measured it on the
same tree today; what was not run says so.

## The panel, and why it is full

**Full panel**: `compiler-engineer`, `ffi-pragmatist`, `spec-warden`,
`historian`, and `llm-ergonomist` run as a fresh blind session; the
completeness critic's second pass over the reports before the synthesis.
The brief first proposed the soundness lane; the critic's § 8 showed three
facts against it, each run: spec § 13 (`spec/heroes-spec.md:354-362`) says
*A group's `record` is the header's struct: all its fields*, which for `SA`
tells a reader to write the record every route must refuse to build or
compare; `grep -n -i "union\|overlap\|anonymous" spec/heroes-spec.md` finds
no C union anywhere in the spec; and both `ffi_union_field` messages begin
*`` `T` is a `union` in ``* (`selfhost/emit/ffi_record.hero:176`, `:194`),
false for `SA`, a struct. `.claude/skills/panel/SKILL.md` § Two lanes: *a
question that looks internal but has a sentence in the spec behind it is a
full panel*.

## The defects

**Defect 151** (`docs/work/DEFECTS.md`, widened today): *a C struct holding
an anonymous union is bound as separate fields, so a value built from Heroes
reads back wrong.* `probes/u.h`: `typedef struct { int32_t kind; union {
int32_t i; float f; }; int32_t x; } SA;` and `make_sa()` returning `{kind 1,
i 12, x 3}`. Reproduced on `ae08ed93`'s compiler at 10:33 and 11:02, and
again by the critic in its copy (its § 13):

| case | the program | `build` | run |
|---|---|---|---|
| `u17_anon_constructed.hero` | `record SA` naming `kind`, `i`, `f`, `x`; `s = SA(kind: 1, i: 7, f: 0.5, x: 3)`, `print(s.i)` | exit 0, two clang warnings at the author's lines: *excess elements in struct initializer* (the probe `SA v = {0,0,0,0};`) and *initializer overrides prior initialization of this subobject* (the construction `(SA){.kind = t1, .i = t2, .f = t3, .x = t4}`) | `1056964608`, exit 0 |
| `u18_anon_compared.hero` | the same record, `print(make_sa() == make_sa())` | exit 0, the excess-elements warning | `true`, exit 0 |
| `u07_anon_omits_x.hero` | `record SA` naming `kind`, `i`, `f` (no `x`) | exit 0, silent | `12`, exit 0 |
| `lane-literals/u16_anon_omits_kind.hero` | `kind` left out | exit 0, silent (the critic's § 11) | |
| `u19_struct_omits_middle.hero` | `s3.h`'s `S3 {a, b, c}`, `record S3` naming `a` and `c` | exit 1, *`S3` does not name `c`* | |
| `critic/one_arm.hero` | `one_arm.h`'s `SB {int32 kind; union {int8 b; int64 q;};}`, `record SB` naming `kind` and `b`, `print(make_a() == make_b())`, two values whose `q` differs | exit 0, silent | `true`, exit 0 |
| `critic/one_arm_union.hero` | the same union alone, `record UB` naming `b` | exit 1, `ffi_union_field` | |
| `lane-literals/u08_anon_one_member.hero`, `critic/u08_mine.hero` | `SA` naming `kind`, `i`, `x` | exit 0, silent, correct to read and build (7 and 12) | |

So the class is wider than two fields overlapping: **a declared field that
lies in a union, at any depth**, reaches a wrong `==` with one field
(`SB`) and a wrong value with two (`SA`).

**Defect 150**: `probes/read.hero` over `probes/w.h`'s `typedef union {
int32_t i; uint32_t n; } W;`, `record W` naming `i` and `n`, reading
`make_w().i`: `build` exit 0 printing *warning: excess elements in union
initializer* twice at `read.hero:2:81` (`W v = {0,0};`), the program prints
`7`, exit 0 (10:35 on `ae08ed93`). Queued to lane ffi-macro and withdrawn to
this sitting at 10:35: same probe.

**Defect 156**, filed at 11:05 from the critic's § 10: `probes/critic/bf.h`'s
`BF { int32 kind; uint32 flag : 1; uint32 rest : 31; }`, `record BF` naming
all three: `check` exit 0, `build` exit 2, *internal error: ... invalid
application of 'sizeof' to bit-field* at the field assertions and *address of
bit-field requested* in the generated hash (reproduced at 11:02). Panel 073's
item 3 resolved that bit-fields are filtered before the assertion.

## What the compiler does today

- **The union detector** asks C what the TYPE is:
  `_Static_assert(__builtin_classify_type(*(T *)0) != 13, "heroes-ffi-union
  T f1 f2 ...")` (`selfhost/emit/extern_union.hero:103`; 13 a union, 12 a
  struct). It is emitted for a record the program reaches by `used`
  (`:133-156`) and `reach` (`:170-217`): construction at two or more
  declared fields (`:68`), and `==`, `hash`, a map construction or a map key
  at any arity (`:144`, `:149-151`). **For `SA` it is emitted and passes**:
  u17's `--emit-c` holds `_Static_assert(__builtin_classify_type(*(SA *)0)
  != 13, "heroes-ffi-union SA kind i f x");` (the critic's § 11). A struct
  classifies as 12.
- **History**: panel 073 shipped `sizeof(T) >=` the fields' sum, and its
  coordinator measured that it caught *the anonymous union promoted into a
  struct (`8 >= 12`, fires)* (`docs/panel/073-the-fields-that-share-one-address.md:75-76`).
  Panel 077 replaced the predicate wholesale the same day, on a padded union
  (`SDL_Event`); `grep -n -i anonymous` over its file finds no hit. So
  defect 151 is, on the record, a coverage regression of that swap (the
  critic's § 6).
- **The completeness probe** (`selfhost/emit/extern_record.hero:84-210`,
  `completeness_probes`): one never-called function per non-`partial` group
  record with fields (`:100`), `T v = {0, ..., 0};`, one positional zero per
  DECLARED field, `{}` for an aggregate field, and `(0)` for a one-scalar
  record (`:195-196`, panel 077's repair of clang's `{0}` exemption,
  `077...:82-85`), under a local `#pragma clang diagnostic error
  "-Wmissing-field-initializers"` with `-Wmissing-braces` ignored
  (`:119-120`). For `SA`, C has three top-level members (`kind`, the
  anonymous union, `x`) and the record four fields: four zeros are excess
  (u17, u18); three are read as `kind`, the union, `x`, so u07 is silent;
  for `S3` naming `a` and `c`, two zeros fill `a` and `b` and clang names
  `c` (u19). The code comment at `:91-92`, *A union gives no signal,
  CORRECTLY: naming one member of a union is naming all of it*, is a
  comment and no sitting's ruling: panel 073's § Two holes, item 1
  (`073...:162-165`), says a non-`partial` record naming ONE field of a
  union *claims completeness falsely and keeps `==`/`hash`*; panel 077 closed
  the `==`/`hash` half; what a union record's completeness probe should say
  has two readings on the record (the critic's § 5).
- **The mappers**: `selfhost/emit/ffi_record.hero` (`ffi_incomplete_record`
  at `:53`, its `incomplete_record` at `:38-60`; `ffi_union_field` at `:169`,
  both messages *`T` is a `union` in ...* at `:176`, `:194`). Two goldens
  hold `ffi_union_field`, both one-member `==` refusals; none holds the
  two-member message (the critic's § 11). u19's false *does not name* has a
  precedent, defect 062 (`tests/golden/unsupported/ffi-a-cstr-field-blames-the-field.hero:1-28`).
- **The module sizes in `suite_layout.hero`'s unit** (the critic's § 11, an
  `awk` of `code_lines`, `suite_layout.hero:686-701`; `layout` filtered to
  `extern_` read 1 passed, 0 failed): `ffi_record.hero` 183,
  `extern_record.hero` 207, `extern_field.hero` 286, `extern_union.hero`
  215, against the ceiling 300.
- **The header's AST is already asked of clang in `build`**:
  `selfhost/cli/header_types.hero:7-16` runs `clang -fsyntax-only -Xclang
  -ast-dump=json -Xclang -ast-dump-filter=hero_ty_`, read by its shape
  (`:18-23`), under panel 103's constraint that the dump gives text and the
  verdict is asked of clang in C assertions
  (`docs/panel/103-the-ffi-pointer-verdict-goes-strict.md:142-146`);
  `selfhost/cli/clang_floor.hero:28-30` says that dump's shape is the same on
  18.1.8, 21.0.0 and 22.1.8.

## The questions

**Q1. A declared field that lies in a union, at any depth.** How does
`build` learn which declared fields lie in a C union (anonymous or named,
inside a struct, inside an anonymous struct, two unions in one struct), so
that panel 073 and 077's rule reaches them: construction naming two that
share bytes refused, and `==`, `hash` and a map key refused for a record
holding any field in a union (the any-arity reason of
`extern_union.hero:45-50`, which `SB` shows)? Routes found so far:
- **(1a)** per-pair range assertions from `offsetof` and `sizeof` (panel 073
  refused an `offsetof` EQUALITY predicate on GNU zero-sized members; a
  range predicate differs, and bit-fields and incomplete types meet it with
  clang's text, `073...:52`, `:98-100`). Blind to `SB`.
- **(1b)** a designated-initializer probe under a locally-armed
  `-Winitializer-overrides` as an error. Blind to `SB`; refuses `read.hero`,
  a correct read-only program, unless emitted only for the `used` records
  (the critic's § 9, measured); and u17's own construction already provokes
  the warning, so arming it at the construction is a variant with no new C.
- **(1c)** a refusal of any record over a struct holding an anonymous union
  (u08 reads and builds correctly today, so it refuses a program that runs).
- **(1d)** a nested group in the record's declaration (a surface form).
- **(1e)** **the header's layout read from clang**, the route defect 151's
  own entry names: the AST dump `build` already runs, or `-Xclang
  -fdump-record-layouts`, which on this Mac's clang prints `SA`'s anonymous
  union with its members and offsets, and `SB`'s (the critic's § 2, its
  output quoted there); under panel 103's constraint the dump gives text and
  the verdict stays a C assertion. Whether `-fdump-record-layouts`' text is
  stable across clangs (a cc1 option) is unrun beyond Apple clang 21; the
  JSON dump's stability is `clang_floor.hero`'s measurement. Unbuilt.
- **(1f)** the classify form OR the sum-of-sizes form, linear in the fields:
  restores 073's coverage of `SA` and keeps 077's of a padded union; blind to
  a padded struct whose union is small (`PADU {int64 k; union {int8 a; int8
  b;};}` passes, the critic's § 6, measured) and to `SB`.
- **(1g)** a refusal of `==`, `hash` and a map key on every group record
  that is not proven union-free (how many tracked programs compare two
  `extern` records is a census the seats can run; panel 077's historian
  predicted zero).
- a route nobody listed is the seats' to find.

**Q2. The completeness probe's form.** What form tells, for a struct holding
an anonymous union and for a union itself: a field left out (u07, u16), WHICH
field (u19), and no warning for a correct program (u17's excess elements,
defect 150)? **Measured by the critic on this Mac** (§ 4, `critic/c/d1.c`):
under `-Wmissing-field-initializers`, `-Wmissing-designated-field-initializers`
and `-Weverything`, in C, neither Apple clang 21 nor Homebrew clang 22.1.8
reports a field that a DESIGNATED initializer leaves out (both accept the
second flag as a known option). So the question is whether any clang this
project meets reports a designated omission in C, and if none does, what
does (positional zeros in C's own grouping, written from the layout of
(1e)? a mapper that asks whether the member clang names is one the record
declares, the critic's guard for u19?). And what a union record's probe
says: one member named, is the record complete (the code comment) or does it
claim completeness falsely (panel 073)?

**Q3. Where the routes meet, and their reach.** One probe answering Q1 and
Q2, or two; which records get it (every group record, as the completeness
probe today, or only the `used` ones, as the detector); the cost in emitted
C per record and per build (a per-pair form is quadratic: the largest group
record in the tracked tree has 16 fields, `Matrix` in
`tests/golden/run/ffi-a-c-array-member.hero`, 120 pairs, the critic's § 11;
real headers are larger).

**Q4. What a record over such a struct is, for the author.** For `SA`, a
typedef of an anonymous struct, panel 073's escape (one record per arm by
`tag`) does not exist: a second record by `tag SA` is `ffi_unknown_tag`, and
a construction must name every declared field (`missing_fields`) (the
critic's § 7, `critic/arm.hero`, `critic/omit.hero`). So under a refusal of
construction, a program that builds an `SA` holding `f` and reads `i` from
one C returns has no expressible form. What does the author write, what
does the spec say (§ 13's *all its fields*), what does the message say
(*`T` is a `union`* is false for a struct), and what does a bit-field member
bind to (defect 156)? One route on the record: **(1h)** panel 073's filed
*construction-arity form* (`073...:119-123`), *a record over a union may
declare every member and be constructed naming exactly one*, which waited for
the compiler to know a type is a union before clang runs: (1e) would give it
that knowledge. The blind seat reads it as candidate M, beside N (*a record
names exactly one member of each union*) and L (no sentence)
(`blind/brief.md`).

## What every route must keep

- **The census**, `build --emit-c` over every tracked `.hero` file holding an
  `extern` group, the trunk's compiler against the seat's, **recording exit
  code, stdout and stderr per file**: `git ls-files '*.hero' | xargs grep -l
  '^extern ' | wc -l` read 417 of 1642 at 10:34 on `ae08ed93`, and 447 of
  1672 at 11:12 on `779139d0`, the 30 more being this sitting's own probes
  (`git ls-files 'docs/panel/186-briefs/probes/*.hero' | xargs grep -l
  '^extern ' | wc -l`, 30). Of the 417, 75 are earlier panels'
  probes under `docs/panel/*-briefs/` (the critic's § 11). The critic's
  baseline on `ae08ed93` (§ 12): 207 exit 0, 210 exit 1, 0 exit 2, and no
  tracked program prints an initializer warning (15 files print clang
  warnings, all `-Wunused-function` or `-Wunneeded-internal-declaration`
  under `docs/panel/17[5-7]-briefs/`). A refusal that stays must stay the
  same refusal (its stderr).
- **Panel 073's three readings that work** (reading a union's members, one
  record per arm by `tag`, a one-member binding) stay legal, and the seats
  must write them: no SDL3 program binding a union record is in the tracked
  tree (the critic's § 11); SDL3's headers are on this Mac
  (`/opt/homebrew/include/SDL3`), and `raylib.h`.
- **The zero-warning bar** is `tests/harness/suite_warnings.hero:1-8`, *The
  generated C compiles with NO warnings at all*, over `tests/golden/run`,
  `tests/golden/emit` and `examples`: defect 150's instrument, owed by any
  route that adds a probe. And `.claude/rules/c-boundary.md`: a clang
  failure is exit 2 and says the compiler is wrong, with one class of
  exception.
- **Platforms**: a route resting on a clang warning, builtin or dump is read
  on every clang that judges this project:

  | where | clang | how measured |
  |---|---|---|
  | this Mac | Apple clang 21.0.0 (clang-2100.3.34.2) | `clang --version`, 10:34 |
  | this Mac, Homebrew | clang 22.1.8 | `/opt/homebrew/opt/llvm@22/bin/clang --version`, the critic's § 3; a seat may use it |
  | the CI's Linux legs, x86-64 and arm64 | Ubuntu clang 18.1.3, the floor's major (`clang_floor.hero:35`) | run 36939966148's job logs, the critic's § 3 |
  | the CI's Mac | Apple clang 21.0.0 (clang-2100.1.1.101) | the same run's Darwin job log |
  | the CI's Windows leg | clang 20.1.8 | the same run's Windows job log |
  | the local Linux containers | Debian clang 22.1.8 | the arm64 leg's log, 09:01 |
  | the Windows box | clang 23.1.1 | `ssh win "clang --version"`, 10:34 |

  The coordinator has a `silkeh/clang:18` image (`docker images`, 11:00; its
  exact version unread) and runs clang 18 and the Windows box on a seat's
  request, with the exact files and commands named in the report. One
  container at a time on this machine.

## The frozen tree and your copy

The trunk is frozen at `779139d0` from the seats' launch to the synthesis.
`<scratchpad>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`.
Your copy is `<scratchpad>/186-<seat>/`, made with `git archive 779139d0 |
tar -x -C <copy>` (no `.claude/worktrees`, no `build/`), where you build
your own compiler from the seed (`clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`, a few seconds; `./heroes build
selfhost/main.hero -o heroes` after an edit). Never build, run or read
inside another seat's copy (the critic's is `<scratchpad>/186-critic/`; its
cases are already copied into `probes/critic/`), the trunk, or a lane's
worktree. The probes are `docs/panel/186-briefs/probes/` (the five cases at
its top, `lane-literals/` with all 18 of the lane's cases and its
`summary.txt`, `critic/` with the critic's cases and `critic/c/` its C
files). **No paid run of any kind.** Three repair lanes share this machine:
at most three processes at once, no timing measurement. Your report is
`docs/panel/186-reports/<seat>.md` in the TRUNK, the one file there you
write, written as you go: a verdict per route (approve, object, veto), what
you built and ran with its output, your cost, a falsifiable prediction, and
the condition that would change your verdict. English, no em dashes.
