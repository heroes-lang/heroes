# Panel 223, shared brief: the padding bits inside a byte a named bit-field holds

**Convened 2026-10-17** by the author's answer that afternoon (*convene the two sittings now*), on
defect 2099 (https://github.com/heroes-lang/heroes/issues/2099), `adjacent` since 2026-10-16 and
counted `blocking` at M-bounded-seed's tag: the coordinator placed the tag locally on
`3c478500b` and ran `./heroes run tests/harness/main.hero -- ./heroes records` (2026-10-17),
`records/tagged` reading *m-bounded-seed was placed over 2 open defect(s) that count against a
tag*, 2099 and 2120, each `adjacent` and open at two tags; the local tag was then removed (`git tag
-d`), so no `m-bounded-seed` tag exists, and against today's newest, `m-buildable-structs`, neither
counts (the critic's first pass). The sitting is convened so that the tag can be placed. **Lane: soundness** (compiler-engineer, ffi-pragmatist, a
completeness critic): the question changes no surface, no diagnostic and no spec token, by
`.claude/skills/panel/SKILL.md`'s rule; the critic says if that is wrong.

**The tree**: lane `lane-land215`, HEAD `3c478500b` (`git log -1`), working tree clean but
the briefs of this sitting and of panel 224, and the critic's first pass
(`docs/panel/223-reports/critic-briefs.md`, which every seat reads after this brief). Copy it, never work in it.

## The question

Panel 215's R1 (ratified 2026-10-16) zeroes, before each lend to C, and since defect 2112's
repair (`d459e8050`, `selfhost/emit/inst.hero` lines 167 to 171) after every store of a whole group
record too, the bytes of a group record that no C member holds, and **never a byte a member holds**
(the rule file says *before each `extern` call* alone and lags that repair)
(`.claude/rules/generated-c.md` § Arithmetic and memory; `docs/panel/215-a-group-*.md` R1). So the
bits of a byte a named bit-field holds that no named bit-field holds (the unnamed `:2`'s included)
reach C as they were: panel 215's compiler-engineer measured MemorySanitizer reports at `-O0` on
both Linux legs, on the sitting's prototype, over
`struct bf { char a; unsigned b:3; unsigned :2; unsigned c:4; double d; }` lent from a field after C
returned it by value (`docs/panel/215-reports/compiler-engineer-build.md` lines 213 to 227 and 249
to 272; its open item is lines 300 to 318), and measurement 046 § 2's shape 7 witnesses it on the
landed compiler (`docs/measurements/046-padding-c-reads-under-memorysanitizer-five-shapes-two-reach-it-and-one-is-the-x86-64-abi.md`).
**The bit positions are one ABI's**: on `x86_64-pc-windows-msvc` the critic reads the bit-fields in
bytes 4 and 5 and clang's write to `c` as a 32-bit read-modify-write of bytes 4 to 7, so R1 may
already write inside a bit-field's access unit on one CI leg; reasoned and unrun, item 2 asks it. **Should those bits be zero where C receives them, and how?**

Why R1 kept to whole bytes, in its own seat's words, **a reasoning its seat marked unrun** (`docs/panel/215-reports/compiler-engineer.md`
lines 158 to 162): *G never writes a byte a member holds, so a mutex another thread is locking keeps
its bytes; a padding byte written while another thread copies the whole struct is a race on bytes
no member holds.* Panel 215's critic (`docs/panel/215-reports/critic-reports.md` line 174): an
unnamed bit-field run stays outside `offsetof`, a record holding one is `partial`
(`spec/heroes-spec.md` line 383, *A bit-field is none of these: leave it to `partial`*), and
leaving its storage unit alone writes no member's byte.

## Routes the coordinator listed (a seat adds any it finds; the list is not the option set)

- **A, a mask per shared byte, read-modify-write**: for each byte a named bit-field shares with bits
  no named bit-field holds, `byte &= mask` before the lend, the masks found once before `main`
  (where R1's tables are found today) by assigning all-ones to each named bit-field of a zeroed
  object and reading its bytes. Writes only those bytes, and writes a member's byte.
- **B, rebuilt through a zeroed temporary**: a zeroed object, each named member assigned from the
  place, the object's bytes copied back. Writes every byte of the record.
- **C, left open and said**: R1 as it stands. No rule lets a defect left open stop counting
  against a tag (panel 187's R1 pins a recovery item in `tests/golden/check/`, which cannot hold what
  MemorySanitizer reads), so C leaves M-bounded-seed untagged until 2099 closes otherwise; a seat
  that recommends it says how the class closes.
- **D, refused**: a lend of a record whose header holds such a byte refused at build time. **Outside
  this lane**: a refusal is a diagnostic class, and spec line 383 tells a reader to bind such a
  struct `partial`, so a sitting that finds D the only sound route is convened again in full.
- The critic's first pass lists five more (masks read at build time from the dump's `byte:lo-hi`
  column, which `layout_rows.last_byte` parses; masks from a static object with every named
  bit-field all ones; B narrowed to the bit-field run's bytes; zeroing where a value enters a Heroes
  place from C; one route at the lend and another at 2112's store). Judge them as listed.

## What is to be measured, not argued

1. The defect reproduced on the landed compiler: a Heroes program over `struct bf` (the 215
   probe is `docs/panel/215-reports/compiler-engineer-work/build/probes/pn.hero`, its line 76 the
   `w_bf` lend), MemorySanitizer in `heroes-linux-arm64:latest` (Docker on this Mac). The
   MSan run is the instrument; say the exact command.
2. The race, per C access unit and not per byte: which Heroes program, if any, can reach a lend or
   a whole store of a record while another thread, Heroes' or C's through an address it kept,
   writes a bit-field of the same access unit, given Heroes' own rules on `@`
   (`spec/heroes-spec.md`); and whether R1 as landed already writes inside an access unit on the
   MSVC target (the critic's reading above), measured on this Mac with
   `--target=x86_64-pc-windows-msvc` or on the Windows box (on, the author said 2026-10-17).
3. The cost of the route built: lines in `selfhost/emit/` (`wc -l` on `3c478500b`:
   `padding.hero` 351, `padding_tables.hero` 141, `membership.hero` 100; `layout` counts code
   lines, ask it), and in `runtime/` (`hero_pad_gaps`, `hero_pad_fill`), `cli/layout.hero` and
   `emit/inst.hero` (its `layout` ceiling 350) where they move; per-lend run time beside the
   landed +10 ns of `8e4636161`'s body (a two-member struct, 20 million lends); and the suites the
   map in `.claude/rules/verification.md` names for `selfhost/emit/**` (`emission`, `emit`,
   `determinism`, `wholes`, `descriptors`, `lldb`) and `runtime` if the runtime moves.

## Rules for every seat

Work in your own copy **inside the repository's root**, `.claude/worktrees/223-<seat>/` under
`/Users/joseph/Temp/heroes/heroes-lang/` (git ignores it; CLAUDE.md's hard stop forbids a file
outside the root): `git -C <lane> archive 3c478500b | tar -x -C <dir>`,
then `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` there (stage 1), and
`./heroes build selfhost/main.hero -o heroes-next` for stage 2 if you edit `selfhost/` (74.37 s
real by `/usr/bin/time -p` on this Mac, 2026-10-17, the coordinator's build of the lane). Never build or run in another seat's directory or in the lane. Write your report to
`docs/panel/223-reports/<seat>.md` in the lane as you go, not only at the end. **No paid run** (no
`claude -p`, no API call), **no credential printed** (`gh auth status` only, never `gh auth
token`). A program that may not end runs as a built binary under `timeout`, its output to
`/dev/null` or through `head -c`. Every number in your report carries the command that made it;
a claim you could not run says *unrun*.
