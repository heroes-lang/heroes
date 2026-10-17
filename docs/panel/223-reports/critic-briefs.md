# Panel 223, completeness critic, first pass: the briefs' framing facts

Read 2026-10-17, before any seat started, on lane `lane-land215` at `3c478500b`
(`git rev-parse HEAD` printed `3c478500b959ed5dc32549d9d00d6e49746496df`). Briefs read:
`00-shared.md` (69 lines), `compiler-engineer.md` (12), `ffi-pragmatist.md` (13), `critic.md` (11),
by `wc -l docs/panel/223-briefs/*`. No route is judged here.

**Where I ran things.** Nothing was built and no file was written outside the lane but this one.
Every check below is a read of the tree, `gh issue view` (read-only; `gh auth status` showed a
logged-in account, no token printed), `git` reads, clang on this Mac reading stdin and writing
stdout (`-fsyntax-only` or `-S -o -`), and one `docker run --rm` whose files stayed in the
container's `/tmp`. The reason I did not make the scratchpad copy the coordinator offered is item
14 below.

## 1. Facts that hold

| brief | fact | command | result |
|---|---|---|---|
| shared 11 | HEAD `3c478500b` | `git rev-parse HEAD` | holds |
| shared 4 | issue 2099 exists, open, `adjacent` since 2026-10-16 | `gh issue view 2099 --repo heroes-lang/heroes --json ...` | holds: state `OPEN`, labels `kind:defect`, `class:adjacent`, `area:emit`; body `**adjacent**, 2026-10-16` |
| shared 16-18 | R1 ratified 2026-10-16; writes the bytes no C member holds, never a byte a member holds | `grep -n` in the 215 decision file and `.claude/rules/generated-c.md` | holds: decision file line 240 `**RATIFIED, 2026-10-16**`; generated-c.md lines 68 to 80, *never a byte a member holds* at line 72, *the gaps are found once, before `main`* at 75 to 76 |
| shared 25-27 | the compiler-engineer's sentence, `215-reports/compiler-engineer.md` 158 to 162 | `sed -n 150,170p` | the quoted words stand at lines 160 to 161 (but see item 6) |
| shared 30 | spec line 383, *A bit-field is none of these: leave it to `partial`* | `grep -n 'bit-field' spec/heroes-spec.md` | holds, verbatim, the one match |
| shared 48 | the probe path and its line 76 the `w_bf` lend | `cat -n .../probes/pn.hero` | holds: line 76 `n4 = w_bf(@b.h)`; the header `pn.h` line 16 is the `struct bf` the brief quotes |
| shared 49 | `heroes-linux-arm64:latest` on this Mac, MemorySanitizer usable | `docker images`; `docker run --rm heroes-linux-arm64:latest sh -c '... clang -fsanitize=memory ...'` | image `338a1044ac90`, 4 weeks old; Debian clang 22.1.8; a C program alone setting `a b c d` of `struct bf` and reading byte 2 printed `WARNING: MemorySanitizer: use-of-uninitialized-value` |
| shared 55 | `wc -l` of `padding.hero` 351, `padding_tables.hero` 141, `membership.hero` 100 | `wc -l selfhost/emit/{padding,padding_tables,membership}.hero` | holds: 351, 141, 100 |
| shared 36-37 | R1's tables are found before `main` | `grep -n hero_pad_gaps` | holds: the unit's constructor calls `hero_pad_gaps` (`emit/padding_tables.hero:93`, `runtime/parts/lend.c:490-519`) |
| ffi 12 | the four CI legs | `.github/workflows/ci.yml:301` | Linux x86-64, Linux arm64, Darwin arm64, Windows x86-64; Windows's target `x86_64-pc-windows-msvc` (`docs/platforms/windows/WINDOWS-MACHINE.md:346`, `:357`) |

## 2. Facts that do not hold, or hold only in part

1. **The `records/tagged` reading names a tag that does not exist** (shared 5 to 7). `git
   for-each-ref 'refs/tags/m-b*'` lists `m-binding-fidelity`, `m-bootstrap-archive` and
   `m-buildable-structs`, no `m-bounded-seed`; `git tag -l | grep -i -E 'bounded|seed'` exits 1;
   `git ls-remote --tags origin 'm-bounded*'` prints nothing; `git tag --points-at 3c478500b`
   prints nothing. `records/tagged` judges the newest tag of `git tag --list m-* --sort=-creatordate`
   (`tests/harness/suite_records.hero:1058-1063`), which today is `m-buildable-structs`
   (`6f83c0a37`, 2026-10-17 14:34). Read against the aging rule (`.claude/rules/records.md`
   § The lists: *blocking from the second tag placed since*), 2099, classed 2026-10-16, has one tag
   since it, so a run today would not count it: **an inference, unrun**. Either the coordinator ran
   it with a tag since removed, or in a tree I cannot see; the brief should say which, or say
   *2099 counts once `m-bounded-seed` is placed*.
2. **`compiler-engineer-build.md` lines 300 to 318 are not the MemorySanitizer measurement** (shared
   22). `sed -n 300,318p` prints *What stays open*, items 1 to 5, the bit-field reading at 310 to
   312. The measurement the sentence describes is the x86-64 table at lines 213 to 227 (`pn`,
   `1/0/0/0/0, line 76`, row 227) and the arm64 table and reading at 249 to 272 (row 263, reading
   268 to 272). It was measured on the sitting's prototype route, not the landed compiler.
3. **A second witness on the landed compiler is not cited**: `docs/measurements/046-*.md` § 2,
   shape 7 (lines 54 to 61), on the trunk's compiler at `bf6a9c4eb`: *the one byte left unwritten
   anywhere was byte 1 of the bit-field record, the three bits past `mid : 10` inside it: defect
   2099*. Its probes were not kept (line 83 to 84).
4. **"R1 zeroes, before each lend to C"** (shared 16) **is no longer the whole of R1's writes.**
   Defect 2112's repair, `d459e8050`, writes the padding zero again after every store of a group
   record whole (`selfhost/emit/inst.hero:167-171`, a second caller of `padding.of_place`; the
   head of `emit/padding.hero`, lines 33 to 37). `.claude/rules/generated-c.md` still says *before
   each `extern` call* only (`grep -n '2112\|written whole' .claude/rules/generated-c.md` exits 1),
   which bears on the compiler-engineer's task *say what the sentence would read after the route*:
   it already lags one repair. Item 2 of *What is to be measured* (the race) asks about a lend only;
   the store site writes too.
5. **"215's open item 5"** (shared 53) was measured and repaired: measurement 046 shape 5 found it
   (`reported` at x86-64 `-O0`), filed as defect 2112 (`blocking`), repaired at `d459e8050`. What
   the brief needs from it, a C library that kept the address, still stands; the citation should
   say it is closed and name 2112.
6. **"Why R1 kept to whole bytes, in its own seat's words"** (shared 25): the seat's own label for
   that sentence is *Residual, reasoned and unrun* (`compiler-engineer.md:158`). It is a reading,
   not a measured reason, and the brief should say so. And one target contradicts it as far as I
   could run (item 8).
7. **"215's 5 ns"** (shared 56) is the prototype's inline helper at a load of 7.6 to 8.9, *noisy,
   reported, not a verdict* (`compiler-engineer.md:152-156`), and the build report says the
   route's per-lend cost was *unmeasured* that pass (`compiler-engineer-build.md:341`). The landing
   did not take the inline form: a lend calls the runtime's out-of-line `hero_pad_fill`
   (`emit/padding.hero:131`, `runtime/parts/lend.c:522-529`), and R1's landing commit measured it:
   *a lend of a two-member struct costs ... +10 ns with the gaps found once (20 million lends, 0.69
   s today, ... 0.89 s)* (`git show -s 8e4636161`). The baseline to set a route beside is that +10
   ns and its command, not 5 ns.
8. **The bit positions are one ABI's.** `struct bf` dumped by `clang -target <t> -fsyntax-only
   -Xclang -fdump-record-layouts -x c -` (Apple clang 21.0.0): on `x86_64-unknown-linux-gnu`,
   `aarch64-unknown-linux-gnu` and `arm64-apple-macos14`, `b` is `1:0-2`, the unnamed `1:3-4`, `c`
   `1:5-8`; on **`x86_64-pc-windows-msvc`, `b` is `4:0-2`, the unnamed `4:3-4`, `c` `4:5-8`**, so the
   shared bytes are 4 and 5 and bytes 1 to 3 are whole padding. No MemorySanitizer exists there
   (046 § 5), so the shape on the Windows leg is reached by reading bytes, not by MSan.
9. **Item 2's race is asked per BYTE, and C writes a bit-field per access unit.** `clang -target <t>
   -O0/-O2 -S -emit-llvm` of `void set_c(struct bf *p) { p->c = 2; }`: Linux x86-64, Linux arm64
   and Darwin arm64 load and store an `i16` at offset 1 (bytes 1 and 2, the `n` span R1 leaves
   alone); **the MSVC target loads and stores an `i32` at offset 4 (bytes 4 to 7)**. By
   `layout_rows.last_byte` (`selfhost/emit/layout_rows.hero:187-201`, `byte + hi / 8`), the dump row
   `4:5-8` gives the span bytes 4 and 5, so R1 today writes bytes 6 and 7 whole, inside C's own
   read-modify-write of `c` on that target. **A reading of the code over a dump I measured; the
   landed compiler was not run on Windows, and the CI box's clang (22.1.8, then 23.1.1) was not
   asked.** If it holds, *the race R1 avoided* is not avoided on one CI leg today, before any route.
10. **"no bit-field holds" against "no named bit-field holds"** (shared 19 against 35). Bits 3 and 4
    of byte 1 are held by the unnamed `unsigned :2`. `membership.hero:28-29` and `:37-39` treat an
    unnamed bit-field as padding (*C11 6.7.2.1p12*), and route A's wording matches that; line 19's
    does not. The question should use one of the two.
11. **Panel 215's critic sentence** (shared 28 to 31) stands at `critic-reports.md:173-174`, not 174
    alone, and cites *spec line 385* (the spec has moved since; 383 holds today). It is about an
    **unnamed** bit-field run, which sits outside `offsetof`; 2099 is the bits beside **named**
    ones, the shape membership encodes as `n <byte> <bytes>` (`membership.hero:43`).
12. **Route C's citation reaches a different kind of item.** `.claude/rules/verification.md:717`
    and on: *A recovery item closes (panel 187's R1) when each of its rows is repaired, filed
    `adjacent` apart, or pinned as a known cost in a `tests/golden/check/` case*. 2099 is not a
    recovery item, and a `check/` case cannot hold what MemorySanitizer reads. Whether *left open
    and said* stops 2099 counting against a tag is therefore a question, not a premise; the brief
    should name the rule that would make it so, or say none does.
13. **The cost and the suites of item 3 name `selfhost/emit/` alone.** R1's mechanism spans
    `runtime/` (`hero_pad_gaps`, `hero_pad_fill`, `runtime/parts/lend.c:490-529`, declared
    `runtime/heroes_runtime.h:742-744`, guarded `runtime/heroes_guard_open.h:320-321`,
    `selfhost/emit/guarded_names.hero:86`), `selfhost/cli/layout.hero` (it writes the membership
    text, `membership.hero:1-4`), and `selfhost/emit/inst.hero` (the 2112 store site; `layout`
    decides its ceiling at 350, `tests/harness/suite_layout.hero:493`, against the default 300 at
    `:45-46`). The map's `selfhost/emit/**` row names `emission` `emit` `determinism` `wholes`
    `descriptors` `lldb` (`verification.md:93`), and `runtime/**` names `runtime` (`:114`); the brief
    names two. `grep -rn -c hero_pad_fill tests/emission/ | grep -v ':0' | wc -l` reads 89 emissions
    that call the fill today.
14. **The copies the briefs prescribe sit outside the repository's root.** Shared 61 puts each
    seat's copy in `/private/tmp/.../scratchpad/223-<seat>/`. CLAUDE.md § Hard stops (line 53):
    *no file is written outside this repository's root (author, 2026-10-08)*; and
    `verification.md:118` calls the hooks' temporary folders *the author's one exception* to it.
    The panel skill names *the scratchpad* (`SKILL.md:218-226`), written before 2026-10-08. A
    reading, for the coordinator to rule: `.claude/worktrees/223-<seat>/`, git-ignored and inside
    the root, would satisfy both.
15. **"working tree clean but these briefs"** (shared 11 to 12): `git status --porcelain` also lists
    `docs/panel/224-briefs/` untracked, a second sitting in the same lane (`docs/panel/223-reports/`
    and `224-reports/` exist, empty).
16. **The ffi-pragmatist's grep misses the spaced spelling.** Over the macOS SDK's `usr/include`
    (`xcrun --show-sdk-path`), `grep -rn ':[0-9]\+;'` reads **200** lines and `grep -rnE
    ':[[:space:]]*[0-9]+[[:space:]]*;'` **403**, the same with `/usr/bin/grep`; the probe's own
    `pn.h` writes `unsigned b : 3;`, which the brief's pattern does not match. The counts are lines,
    not bit-fields, and include whatever else matches.

## 3. Could not verify

- *Convened ... by the author's answer that afternoon* (shared 3): a conversation; no record in
  the tree to read.
- The `records/tagged` run itself: unrun (item 1: its tag is absent, and the check needs a built
  compiler and the repository's tags, which a `git archive` copy has not).
- *About 75 s on this Mac* for stage 2 (shared 63): no command given, unrun.
- `layout`'s code-line counts of the three modules: unrun; the brief rightly says ask it.
- C11 clause numbers beyond the brief's § 6.7.2.1: no copy of the standard in the tree
  (`find / -maxdepth 4 -iname '*n1570*'` printed nothing). Whether the race also rests on the
  standard's definition of a memory location for adjacent bit-fields, and on its data-race rule, is
  a question for the ffi seat to read, not a fact I can cite.

## 4. Asserted without its command

- Shared 5 to 7: the `records/tagged` invocation is quoted by its output, not its command.
- Shared 56: *215's 5 ns* carries no command (item 7 gives the one that measured the landed form).
- Shared 51 to 52: *given Heroes' own rules on `@` (`spec/heroes-spec.md`)* names no line.
- Shared 63: *about 75 s*.

## 5. Is the soundness lane right?

For A, B and C, yes by the skill's rule (`SKILL.md:18-23`): each changes emitted C and a rule file,
no surface and no diagnostic, and `grep -n -i padding spec/heroes-spec.md` matches nothing; 215's R1
ruled *No sentence of the specification* (decision file line 130 to 131).

**For D, no.** A build-time refusal is a diagnostic class, the full panel's trigger (`SKILL.md:25-27`,
CLAUDE.md § 4). And spec line 383 tells a reader to bind a struct holding a bit-field `partial`;
D refuses a lend of exactly such a record, so the spec's own instruction would lead to a refused
program unless the spec said so, and *a question that looks internal but has a sentence in the spec
behind it is a full panel* (`SKILL.md:33-34`). Either the brief says this lane cannot adopt D and
the synthesis records what a full panel would owe it, or the sitting is the full panel.

What the lane gives up that is cheap to put back: panel 215's prior art already names this class.
The decision file's line 207 quotes SEI CERT DCL39-C, *the padding bytes and bit-field storage unit
padding bits of such a structure*; the brief can cite it with no historian.

## 6. Routes the briefs did not list, seen from the code (not judged)

- **A from the dump**: the masks read at build time from the dump's bit column. The layout dump is
  `clang -fsyntax-only -Xclang -fdump-record-layouts` (`layout_text.hero:20`), whose bit-field row
  reads `byte:lo-hi` (its test, `layout_text.hero:334`, `12:0-0 | uint32_t flag`); `layout_rows.last_byte`
  parses `lo` and `hi` and keeps only the last byte, and `membership.hero:43` writes `n <byte>
  <bytes>`. The bits could ride in that entry and be written as constants of the unit's table, with
  no assignment before `main`. It rests on the dump numbering bits from each byte's low bit, a
  premise about little-endian targets; all four CI legs are, and no big-endian target was run.
- **A from a static object**: each named bit-field of a static-storage object initialised to all
  ones, its bytes the mask, computed by clang with no code before `main`. Rests on padding bits of
  a static object being zero; unrun, and so is whether clang warns on the initialiser.
- **B over the run alone**: read each named bit-field of the run through its C name, write the
  run's bytes (the `n` span) zero, assign them back: route B narrowed from every byte of the record
  to the bytes of the run.
- **At the source**: a Heroes program cannot name a bit-field (spec 383), and `rest: zero` zeroes
  what a construction leaves (spec 385 to 386), so the bits in such a byte arrive dirty only from C
  (a value C returned, a write through an address C kept) or from a whole copy of one; zeroing where
  a value enters a Heroes place from C instead of at each lend. A reading of the spec, unrun.
- **Per site**: the lend (`padding.zero`) and the 2112 store (`inst.hero:167-171`) both call
  `padding.of_place`; a route may differ between them, and the brief frames only the lend.
- Not a route: `__builtin_clear_padding`, which 215 measured unavailable on Apple clang and on
  Debian clang 22.1.8 (`compiler-engineer-build.md:61-64`).

**Shapes beside `struct bf` for the seats** (CL-061), each found in the code or the tree, none run:
the MSVC layout (item 8); a bit-field record nested in a group record, an element of a fixed array,
an element of a lent array's buffer (R1 recurses into all three, `padding.hero:22-26`); a bit-field
inside a union arm (R1 writes nothing in a union, generated-c.md line 73); `_Bool`, signed and
`char` bit-fields; a run under `packed`; a bit-field whose dump row the reader cannot read, where
`membership.encoded` fails and the whole record gets nothing written (`membership.hero:26-27`,
`:41-42`); and the tree's one bit-field golden, `tests/golden/run/fixedbugs-156-bit-fields.h`
(`BF` fills its 32-bit unit, `UNNAMED` has an unnamed `:3` and no named bit-field).
