# Panel 186, the shared brief: the fields an anonymous union shares, and the probe that counts a record's fields

Written 2026-10-02 by the coordinator, on the trunk at `ae08ed93`. Every
number below names the command that produced it, run while this brief was
written; what was not run says so.

## The lane, and why

**Soundness lane**: `compiler-engineer` and `ffi-pragmatist`, then the
completeness critic over the briefs first and over the reports after
(`.claude/skills/panel/SKILL.md` § Two lanes, § 3b, § 3c). The question
changes how `build` checks a group's `record` against its C header; on the
coordinator's reading it changes no surface, no spec token and no diagnostic
class (`ffi_union_field` and `ffi_incomplete_record` exist). What the lane
gives up: no blind reader and no spec-warden. **If a seat finds that a route
needs a spec sentence, a new code or a new surface form, it says so in its
report and the sitting becomes a full panel before the synthesis.**

## The two defects

**Defect 151** (`docs/work/DEFECTS.md`): *a C struct holding an anonymous
union is bound as separate fields, so a value built from Heroes reads back
wrong.* The header, `docs/panel/186-briefs/probes/u.h`:
`typedef struct { int32_t kind; union { int32_t i; float f; }; int32_t x; } SA;`.
Reproduced by the coordinator on `ae08ed93`'s compiler, 2026-10-02 at 10:33
(`heroes build <case> -o <bin>` then the binary, in a scratch folder, with
`HEROES_RUNTIME` set):

| case | the program | `build` | run |
|---|---|---|---|
| `u17_anon_constructed.hero` | `record SA` naming `kind`, `i`, `f`, `x`; `s = SA(kind: 1, i: 7, f: 0.5, x: 3)`, `print(s.i)` | exit 0, with two clang warnings at the author's lines: *excess elements in struct initializer* (the completeness probe `SA v = {0,0,0,0};`) and *initializer overrides prior initialization of this subobject* (the construction `(SA){.kind = t1, .i = t2, .f = t3, .x = t4}`) | prints `1056964608`, exit 0 |
| `u18_anon_compared.hero` | the same record, `print(make_sa() == make_sa())` | exit 0, the excess-elements warning | prints `true`, exit 0 |
| `u07_anon_omits_x.hero` | `record SA` naming `kind`, `i`, `f` (no `x`); `print(make_sa().i)` | exit 0, no warning | prints `12`, exit 0 |
| `u19_struct_omits_middle.hero` | `s3.h`'s `S3 {a, b, c}`, `record S3` naming `a` and `c` | exit 1, `ffi_incomplete_record`: *`S3` does not name `c`* | not built |

`make_sa()` returns `{kind 1, i 12, x 3}` (`u.h`). u17 is the wrong value at
the C boundary; u18 is panel 073's refused `==` reached where the detector
does not look; u07 is a field left out that nothing reports; u19 names the
wrong field (the record names `c` and omits `b`).

**Defect 150** (`docs/work/DEFECTS.md`): *a correct program that reads a C
union naming two members gets clang's warning on the author's line.*
`probes/read.hero` over `probes/w.h`'s `typedef union { int32_t i; uint32_t
n; } W;`, `record W` naming `i` and `n`, `print(make_w().i)`: reproduced on
`ae08ed93` at 10:35, `build` exit 0 printing *warning: excess elements in
union initializer* twice at `read.hero:2:81` (`W v = {0,0};`), and the
program prints `7`, exit 0. It was queued to lane ffi-macro and withdrawn to
this sitting at 10:35, because its cause is the same probe.

## What the compiler does today, read by the coordinator

- **The union detector** asks C what the TYPE is:
  `_Static_assert(__builtin_classify_type(*(T *)0) != 13, ...)`
  (`selfhost/emit/extern_union.hero:103`), 13 being a union and 12 a struct,
  emitted only for a record the program constructs or compares
  (`extern_union.hero:42-60`, `used`). Panel 073 shipped `sizeof(T) >=` the
  fields' sum; panel 077 replaced it the same day because a padded union
  (`SDL_Event`) has room (the file's header comment, lines 1 to 24). A
  struct holding an anonymous union classifies as a struct, so for `SA` no
  assertion fires: that is defect 151's u17 and u18, on the coordinator's
  reading of the code; the seats measure it.
- **The completeness probe** (`selfhost/emit/extern_record.hero:84-210`,
  `completeness_probes`): one never-called function per non-`partial` group
  record with fields, `T v = {0, ..., 0};` with one positional zero per
  DECLARED field (`{}` for an aggregate field), under a local
  `#pragma clang diagnostic error "-Wmissing-field-initializers"` and
  `-Wmissing-braces` ignored (lines 119-120). Its own comment, lines 91-92:
  *A union gives no signal, CORRECTLY: naming one member of a union is
  naming all of it, so SDL_Event passes.* For `SA`, C has three top-level
  members (`kind`, the anonymous union, `x`) and the record declares four
  fields, so the probe writes four zeros: excess elements (u17, u18). With
  `x` left out it writes three, which C reads as `kind`, the union, `x`:
  complete, so u07 is silent. For `S3` naming `a` and `c` it writes two
  zeros, C reads them as `a` and `b`, and clang names `c` missing: u19.
- **The message mapper** for both is `selfhost/emit/ffi_record.hero` (260
  lines; `ffi_incomplete_record` at :53, `ffi_union_field` at :169).
- File sizes, `wc -l` at 10:34: `ffi_record.hero` 260,
  `extern_record.hero` 287, `extern_field.hero` 373, `extern_union.hero`
  274. The module ceiling is `.claude/rules/module-shape.md`'s.

## The precedent the sitting must read

- **Panel 073** (`docs/panel/073-the-fields-that-share-one-address.md`,
  ratified 2026-08-24): construction naming two or more overlapping fields is
  exit 1, a generated `==` or `hash` over such a record is exit 1, **reading
  is untouched** (three SDL3 programs read unions correctly), one Heroes
  record per arm over the same C type is the escape (panel 074's `tag`), and
  the construction-arity form (*declare every member, construct naming
  exactly one*) is filed, waiting for a marker. Its § Two holes: a
  non-`partial` record naming ONE field of a union compiles silently.
- **Panel 077** (`docs/panel/077-the-builtin-that-was-already-there.md`):
  the detector became `__builtin_classify_type`.
- **Panel 061** (`docs/panel/061-the-fields-nobody-named.md`, around its line
  268): *all its fields* cannot be written for a header with an array member
  or an anonymous union, which is why `partial` exists.
- **Panel 063** (around its line 51) and **panel 071** (around its line 35:
  SDL3's *2 unreachable inside anonymous unions*).
- Spec § 13, `spec/heroes-spec.md:354-362` (`sed -n` at 10:34): *A group's
  `record` is the header's struct: all its fields ...*; `partial` *names
  only some, and then comparing it and using it as a map key are compile
  errors ... Its size stays C's, not the field list's.*

## The questions

**Q1. Two declared fields that share bytes inside a struct.** How does
`build` learn that two fields a record declares overlap in C, so that panel
073's rule (construction naming both refused, `==` and `hash` refused,
reading untouched) reaches a struct holding an anonymous union, at every
depth (an anonymous union inside an anonymous struct inside a struct), and
what does the author read? Routes the coordinator can name, none built:
**(1a)** per-pair assertions from C's own layout, `offsetof` and `sizeof`,
two ranges intersecting (panel 073 refused an `offsetof` EQUALITY predicate
on GNU zero-sized members; a range predicate is a different predicate, and
whether zero-sized members still break it is unmeasured); **(1b)** a
designated-initializer probe, `T v = {.kind = 0, .i = 0, .f = 0, .x = 0};`,
under a locally-armed `-Winitializer-overrides` made an error, clang naming
the overriding field; **(1c)** a refusal of any record over a struct holding
an anonymous union (u07 reads `i` correctly today, so this would refuse a
program that runs); **(1d)** a nested group in the record's declaration (a
surface form, which would make this a full panel). A route nobody listed is
the seats' to find.

**Q2. The completeness probe's form.** The positional probe counts declared
fields against C's top-level members, and an anonymous union is one member
holding several declared fields. What form tells, for a struct holding an
anonymous union and for a union itself, a field left out (u07), the field
that is left out (u19), and no warning for a correct program (u17's excess
elements, defect 150)? Routes: **(2a)** a designated probe naming each
declared field, under whichever clang warning reports a field a designated
list leaves out (`-Wmissing-field-initializers` changed meaning across clang
versions and `-Wmissing-designated-field-initializers` is newer; which
versions do what is unmeasured); **(2b)** a coverage check from `offsetof`
and `sizeof` (padding makes byte coverage incomplete by design); **(2c)**
anything else. What does a union record get: panel 073 says naming one
member of a union is naming all of it.

**Q3. Where the two routes meet.** One probe answering both questions, or
two, and the cost in emitted C per record (a per-pair form is quadratic in a
record's fields; the largest group record in the tracked tree is a
measurement the seats make).

## What every route must keep

- **Every program that builds and runs today and is correct keeps building
  and its emitted C stays byte-identical unless the route is about it**: the
  census is `build --emit-c` over every tracked `.hero` file holding an
  `extern` group, the trunk's compiler against the seat's; `git ls-files
  '*.hero' | xargs grep -l '^extern ' | wc -l` read **417** of **1642**
  tracked `.hero` files at 10:34.
- Panel 073's three readings that work (reading a union's members, one
  record per arm by `tag`, a one-member binding) stay legal.
- **Platforms**: a route that rests on a clang warning or builtin is measured
  on every clang this project meets: this Mac, `clang --version` *Apple
  clang version 21.0.0 (clang-2100.3.34.2)*; the CI's Mac, *Apple clang
  21.0.0* (clang-2100.1.1.101, from CI run 36939966148 in defect 155, not run
  here); the Linux containers, *Debian clang version 22.1.8* (the arm64
  leg's log, 2026-10-02 09:01); the Windows box, `ssh win "clang --version"`
  *clang version 23.1.1* at 10:34. The Linux and Windows measurements are
  the coordinator's to run on a seat's request, with the exact files: name
  them in your report.
- `.claude/rules/c-boundary.md` and `.claude/rules/generated-c.md`: no clang
  text reaches the author; exit 2 is the compiler blaming itself.

## The frozen tree and your copy

The trunk is frozen at `ae08ed93` from the seats' launch to the synthesis.
`<scratchpad>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`.
Your copy is `<scratchpad>/186-<seat>/`, made with `git archive ae08ed93`
(no `.claude/worktrees`, no `build/`), where you build your own compiler from
the seed (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, a
few seconds; `./heroes build selfhost/main.hero -o heroes` after an edit,
about a minute). Never build, run or read inside another seat's copy, the
trunk or a lane's worktree. The probes are in
`docs/panel/186-briefs/probes/`: copy them into your copy. No paid run of any
kind. Your report is `docs/panel/186-reports/<seat>.md` in the TRUNK, the one
file there you write, written as you go: verdict per route (approve, object,
veto), what you built and ran with its output, your cost in lines and files,
a falsifiable prediction, the condition that would change your verdict.
English, no em dashes.
