# 066 — M-bounded-seed: the seed bounded in shape and in rhythm

## Goal

The row opened 2026-10-14 by the author's instruction, row 67, the first of the chain re-ordered
that day, consolidation first. The seed was 70% of the repository's history: 1,222 versions of
`seed/heroes.c`, 478 of the 677 MiB of blobs, a fresh clone of 580 MiB, regenerated at every
batch's close and on every lane that touched `selfhost/`, and 48% of its bytes `#line`
directives nobody reads (measurement 045). Three parts: **the shape**, a sitting, its target a seed
under 10 MB; **the rhythm**, the compiler always built in two stages and the seed refreshed only
when that build goes red and at each release (task 2087); **the history**, rewritten once and
announced.

## What surprised

- **The floor of the shape was measured, not argued.** Panel 219's compiler-engineer took out every
  copy, inline bounds check, `#line`, indentation and dead function and stopped at 18,831,319
  bytes, identifier text 69.7% of what remained: *under 10 MB* was no emission profile's to reach,
  and the author amended the target. s4, a re-anchoring `#line` joined to its anchor and a file
  name written where it changes, halved the seed with the line table the same at every address.
- **A file's age is not what a binary was built from.** Panel 222's compiler-engineer measured the
  hook passing a copy built before an edit and refusing a compiler whose sources were rewritten
  with the same bytes. On the day the digest landed, the trunk's hook, still asking the age,
  refused this lane's compiler for a seed the coordinator had restored byte for byte after a
  falsification run: the sitting's second case, met in the landing itself.
- **The rhythm's constraint is an invariant, not a count of commits** (panel 222's critic): a tree
  holding the reader of what the emitter writes while its seed's compiler does not write it builds
  no stage 2, and no documented refresh rescues it, a refresh being emitted by that stage 2. One
  commit carrying the writer, the reader and a seed a writer-only scaffold emitted is its own
  fixpoint.
- **A compiler is one stage behind its own library by construction.** The library a program gets is
  the compiling compiler's text, so the digest's `library` row says which stage a compiler is,
  rather than hiding it.
- **The language held its own author to its rule.** Two `str` parameters side by side, `version`
  and `sources`, and the compiler refused the positional call until both were named.

## What broke and why

- **Defect 2125**: the 2109 case pinned an unnamed enumeration's sign, `unsigned int` on Darwin and
  Linux and `int` for the MSVC target, and the Windows box read it red after the push of
  `f94a8cddb`; the box had read C2's diagnostics at `e92f33a2f`, before the reshape that added the
  value, which journal 065 now says beneath its sentence.
- **Defect 2126**: the project page said the seed was more than 800,000 lines, which s4 had made
  263,383, and gave `cmp` as a proof at any commit, which the rhythm makes a release's. Its own
  repair met a third falsehood: *under a minute* for the emission on the release the page now
  names, measured at 76.6 s on the MacBook Air it names.
- **The prototype's mirror misread `use x  # comment`** (the critic); the landed mirror is held to
  the compiler by a test whose fixture holds that line, and that test, given the prototype's
  reading, is red.
- **The first build with the reader failed the macro guard's own test**: a word `heroes_runtime.h`
  writes is owed to the guard's list and its two files.
- **The coordinator's own slips**: a comment added to a case's `.hero` moved every line its
  expected output names; a citation broken across two lines read as a spec citation to `spec`;
  and a commit body that counted lines opening `ok ` and wrote 1720 for the 1719 the count line
  says, corrected in the next commit's body.

## What landed, and what carried forward

**The three parts.** The shape: panel 219, ratified 2026-10-17 with its target amended, s4 landed
as step 8 (`7a64c307b`), the seed from 51,514,006 bytes to 26,229,018 at the batch of
`03b6cd829`. The rhythm: panel 222, ratified the same day with the coverage the author chose,
landed as step 9 (`d1f95674b`, its gate `12532e838`): every program carries the key of the
modules its root reaches, the library compiled into it and the runtime it links; `heroes
--version` prints the compiler's on its second line; the net's `seed` suite holds R3; the hook in
front of the net asks a compiler what it was built from; the CI judges with stage 2 and checks the
seed in two branches; the rules say the rhythm. The history: rewritten once on 2026-10-16
(variant A), every older commit under `refs/archive/*`. **Closed on GitHub**: 2087, 2118
(panel 219), 2124 (panel 222), 2125, 2126.

**Predictions scored at the close.** Panel 219's compiler-engineer: *the seed at M-bounded-seed's
close 24,000,000 to 27,000,000 bytes and under 60,000 `#line`s*, **held**, 26,240,524 and 56,190;
*`lldb` and `lines` 0 failed*, **held**, 14 and 535 at the gate; *no seed under 10,000,000 bytes
unless a change to the C's content lands first*, **held**; *a regeneration costs at most 65% of an
s0 one by the method of his § 7*, **unrun at the close**: the method asks three consecutive seeds
in both shapes from one emitter, and the landed compiler writes s4 alone, with two committed s4
regenerations, not three. Panel 222's compiler-engineer: *(a)'s commits touch no front-end file*,
**held**, no file under the lexer, the parser, the resolver or the checker in `d1f95674b`;
*every blessed emission moves by exactly one line*, **held for every emission of a program with a
`main`**, 612 of 612 at +1 -0 (610 on this Mac, 2 on Linux arm64), and not of the 17 `ir-*`
emissions, which have no `main` and did not move; his condition *to object if the cost passes 1%*,
a warm build of the net's program at 5.70 s user before and 5.73 after, three runs each, the two
compilers built two ways, so a ceiling rather than the cost.

**The gate, 2026-10-17, on `12532e838`**, the compiler clang built from the seed of
`d1f95674b`, whose fixpoint holds over two generations by `cmp` (`seed/heroes.c` 263,383 lines,
26,240,524 bytes): the whole net 31 suites, 0 failed but `emit` and `spec`, repaired in that
commit and run again alone, 13/0 and 31/0: run 534, emission 1299, check 699, warnings 613,
annotations 1166, surface 425, fixes 999, unsupported 344, corpus 70, determinism 574, lines 535,
lldb 14, probe 27, ir 29, permissive 22, full 39, records 26, canonical 2, descriptors 630, grammar
9, order 3, runtime 8, special 10, unseen 3, wholes 630, seed 1, layout 6, units 3, cache 7; the
compiler's own tests 1719 and the net's own 409; the hooks' own 187. Linux arm64 said the same key,
`5f45d6514b22d650`, and read `emission` 1277 and 0 after blessing its two. The Windows box, on
`12db9b318`'s tree, read the repaired 2109 case green in both its forms, and `run` 511 and 0 on
`f94a8cddb`. The site's build, 38 claims over 20 pages, exit 0.

**Status at the close**, for `docs/ROADMAP.md` § Where we are: the compiler 666 modules and
173,440 lines of Heroes, the seed 263,383 lines of C, 26,240,524 bytes, 56,190 `#line`s; the spec
7703 vendored and 10,108 real against 16384; the contract 8167 real, unchanged; sittings 220,
journals 67 numbered files, milestone pages 60, measurements 47, beats 134, questions in
`docs/learn/` 478 of which 453 open.

**Carried**: the author's readings of panels 217, 220 and 221 (2105, 2122, 2123); the census of
`check` over the tracked `.hero` files and panel 187's R2, owed after the push by the optimistic
chain; the formatter's probe M-inferred-cell's changes still owe; the write hook still asks a
compiler's age, which tells how a refusal is told, a digest of the tree with the written file in
it differing on every write; defect 2120, `adjacent`.

**The chain entry**: row 67, `m-bounded-seed`, closed 2026-10-17, this journal.

## Added before the tag, 2026-10-17 evening

**The tag waited on two defects the close had not counted.** Placed locally on `3c478500b`, the
tag read `records/tagged` red: defects 2099 and 2120, each `adjacent` and open at two tags, count
`blocking` at the second. The tag was removed, and by the author's answer (*convene the two
sittings now*, then *by 22:30 close both steps*) panels 223 and 224 sat that evening in the
soundness lane, a completeness critic reading every brief before the seats started: twelve repairs
to 223's briefs and ten to 224's, among them a premise both 223 seats then measured false (*never a
member's byte keeps R1 safe*: on the MSVC target R1 already writes inside a bit-field's access
unit) and the finding that 2120 is the `-fwrapv` case of an order that holds on every platform
(filed apart, 2128, `adjacent`). The CI of `6f83c0a37` read the Windows leg red on two cases, 2109's
(2125, repaired before) and `examples/ctime`'s emission, which names `struct tm`'s members as each
machine's header declares them (2127). And panel 223's compiler-engineer found a `partial` record
leaving out a `const` member told *internal error* at exit 2 (2131).

**What landed**: 2127, `examples/ctime/main` compared on Darwin alone (`82892e4b7`); 2099 under
panel 223, an atomic `and` clearing the bits beside a named bit-field, masks from clang's own bits
(`2365c4787`); 2131, the record recovered through the program's IR at clang's line
(`52b54908a`), with `emit/ffi.hero` split along its field readers, since it stood at its ceiling;
2120 under panel 224, a front-end pass under `-fno-wrapv` whose verdict counts only where every
error it adds is the region's (`32e517b9d`). The seed was not regenerated: the two-stage build
stayed green, the rhythm's first batch. Closed on GitHub: 2099, 2120, 2127, 2131; panels 223 and 224
provisional (2129, 2130).

**Predictions scored.** Panel 223's compiler-engineer, *exactly one emission moves beyond its key
line*: **held**, `run-fixedbugs-156` +17 -3 and 609 files at their key line alone. Panel 223's
ffi-pragmatist, libcurl's `struct curl_hstsentry` MemorySanitizer-clean through Heroes: **unrun**,
measured in C alone. Panel 224's compiler-engineer, *the 2072 case green on the CI's Windows leg on
its first run with clang 23*: **unscored**, the CI's leg runs clang 20.1.8; *the patch under 80
lines by `wc -l`*: **missed, high**, 316 lines of diff and 265 inserted (`git show --format=
32e517b9d -- selfhost | wc -l`), the repair after the critic's second pass and its four tests
tripling it. Panel 224's ffi-pragmatist, a box program binding `VRL_ENABLE_KERNEL_BREAKS` beside
`MB_OK` refused in its first build: **unrun** at the tag.

**The gate, 2026-10-17, on `d1b031833`**, stage 2 judging (`sources 0acfc2fe242765b8`, the mirror's
key of the tree): run 535, emission 1301, check 699, warnings 614, annotations 1167, surface 425,
fixes 999, unsupported 345, corpus 70, determinism 575, lines 536, lldb 14, probe 27, ir 29, emit 13,
permissive 22, full 39, records 26, spec 31, canonical 2, descriptors 631, grammar 9, order 3,
runtime 8, special 10, unseen 3, wholes 631, seed 1, layout 6, units 3, cache 7, every one 0 failed;
the compiler's own tests 1724 and the net's own 409. Linux arm64 said the same key and read
`emission` 1277 and 0 after blessing its two. The Windows box (clang 23.1.1), its disk freed by the author's
yes (this session's own build directories, about 70 GB), built stage 2 of `32e517b9d`, said the
same key, and read `unsupported` 329 and 0, the 2072 case among them, and `emission` first 1230
and 1: the batch's own new case, whose padding table names each bit-field's byte where the MSVC
target lays it out, bytes 4 and 5 where Darwin's are 1 and 2. It joined `darwin_emissions`
(`cec88be5e`), and the box read `emission` 1229 and 0, the case and `examples/ctime` skipped with
their reasons.

**Status at the tag**, for `docs/ROADMAP.md`: the compiler 669 modules and 174,028 lines of Heroes;
the seed unchanged, 263,383 lines, 26,240,524 bytes, 56,190 `#line`s; sittings 222.
