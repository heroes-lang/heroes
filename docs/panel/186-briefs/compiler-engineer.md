# Panel 186, the compiler-engineer's brief

Read `00-shared.md` in this directory first, whole: the defects, what the
compiler does today, the precedent, the four questions and what every route
must keep. It was repaired on the critic's first pass
(`docs/panel/186-reports/completeness-critic-briefs.md`); read that too.

## Your input

The live compiler, in your own copy `<scratchpad>/186-compiler-engineer/`
(`git archive 779139d0 | tar -x -C <copy>`, then `clang -I runtime
seed/heroes.c runtime/runtime.c -o heroes` inside it). The files the
question lives in, with their size in `suite_layout.hero`'s unit (the
critic's § 11): `selfhost/emit/extern_union.hero` (215; the classify-type
assertion at :103, `used` :133-156, `reach` :170-217),
`selfhost/emit/extern_record.hero` (207; `completeness_probes` from :84,
the pragmas at :119-120, the `(0)` rule at :195-196),
`selfhost/emit/ffi_record.hero` (183; `incomplete_record` :38-60,
`ffi_union_field` :169, its two messages :176 and :194),
`selfhost/emit/extern_field.hero` (286, 14 lines of room under the ceiling
300), `selfhost/cli/header_types.hero` (the AST dump `build` already runs)
and `selfhost/cli/clang_floor.hero`, and the construction the emitter
writes for a group record (u17's emitted C holds `(SA){.kind = t1, .i = t2,
.f = t3, .x = t4}`). Never `archive/bootstrap-rs/`.

## Your task

1. Confirm `00-shared.md`'s reading of each case's cause on your compiler
   (the critic measured the classify assertion emitted and passing for
   `SA`; the positional probe's count), and say what each of `u17`, `u18`,
   `u07`, `u16`, `u19`, `one_arm`, `u08`, `read.hero` and `bf.hero` needs.
2. **Build** the route you would adopt for Q1 and Q2 in your copy, in
   `selfhost/`, and run it: every probe of `docs/panel/186-briefs/probes/`
   (all three directories), the shapes beside them (an anonymous union inside
   an anonymous struct inside a struct, `critic/bf.h`'s `DEEP`; two unions in
   one struct, its `S2U`; a named union member, `u.h`'s `SN`; a union of one
   member; a padded union, a header of your own; `PADU`; a GNU zero-sized
   member; a bit-field, defect 156; a flexible array member; a `partial`
   record), the compiler's own tests (`./heroes test selfhost/main.hero`),
   `suite_warnings` (`./heroes run tests/harness/main.hero -- ./heroes
   warnings`), and the census of `00-shared.md` (exit, stdout and stderr per
   file, the trunk's compiler against yours, `xargs -P 3`, every moved file
   explained). If your route is (1e), build it far enough that its verdict is
   a C assertion clang judges, and say which clang text it reads and how that
   text varies by version. A route that does not build is not adopted
   (`.claude/skills/panel/SKILL.md` § 3c).
3. Its cost: lines added per file in the layout unit, whether a module
   crosses the ceiling, the emitted C's growth per record, and the extra
   clang processes per `build`, if any.
4. Every route of Q1 to Q4 judged: approve, object or veto, with what you
   built or measured for each; a route you did not build is said to be
   unbuilt. If your route needs a spec sentence, a new code or a new surface
   form, say which, exactly.

What must be read on clang 18.1.3 or the Windows box goes in your report as
a request, with the exact files and commands, before your verdict; the
coordinator runs it and hands you the output. Write your report into
`docs/panel/186-reports/compiler-engineer.md` in the trunk as you go.
