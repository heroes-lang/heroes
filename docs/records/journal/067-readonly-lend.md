# 067 — M-readonly-lend: a function reads the caller's place without claiming to change it

## Goal

Panel 217's form, ratified by the author on 2026-10-17: `lent` on a parameter of
a Heroes function, the caller's place read and never written, the call
unmarked (R1); the refusals that are part of it (R2); the C half, a group's
record lent read-only crossing as `const T *` and refused where C may write it
(R3); the read-only mode carried by the IR with a verifier rule (R5); the
messages that named `@c` naming `lent` (R4); the walk a new surface form owes
every tool that reads the language; the specification's text; and defect 2128,
`adjacent` and counted `blocking` at this tag, filed between the two tags of
2026-10-17. One lane, `lane-readonly-lend`, opened and closed on 2026-10-18 in
one session, the author asleep from step 2 on, having asked for the steps to
be chained and for the site and the specification to be pushed.

## What surprised

**The compiler asked one question in three places.** Which slot owes a
copy-out was answered by the lowering, by the merge of a function's exits and
by the verifier's invariant, each with its own copy. Changing the first for a
read-only slot left the exit merge waiting for a copy-out the lowering no
longer wrote, and the verifier refused `depth`, a function of two returns: the
merge is total or loud, and it was loud. The rule has one home since,
`containers.owes_copy_out`. And the copy-out it removed moved emitted C where
nothing had promised it would not: the ownership pass held every value read
from an `@` parameter in an owned slot, the place being writable, and a
read-only place's read is borrowed, the returned value's net retain the same.

**The walk found one printer dropping the form, and it was not the
formatter.** The formatter, the IR dump, the tree dump, `mutate`, the probe and
both highlighters carried `lent`; `--dump-scopes` printed `param p` for `p: Pt
lent`, which reads as a copy to anyone reading the dump. And `heroes grammar`
held no copy of the productions to teach: it reads them from the
specification, so the new `Param` printed the moment the text landed.

**The refusal of a nested `@` is `lent`'s alone.** `read(p, bump(@p))` is
refused, the unmarked argument reading as a value taken where it stands, and
`both(@p, bump(@p))` builds and prints 4: both arguments marked, the nested
call returned before `both` runs, which is also what copy in at the call reads.
The specification says so in one clause, and design.md §4.8 records the
accepted shape beside the refused one.

**The specification's price was exact twice.** The spec-warden's draft, applied
alone and measured on the reader's tokeniser, read 10,246, its prediction to
the token; the two sentences the sitting owed and left unpriced cost 40 more,
10,286 in all.

**An instrument could not see a shape until a case needed it.** The
`annotations` suite read an expectation's diagnostics by line alone, so a case
of two modules, which defect 2128's repair needed, reported the other
module's line against the case's own marks. It reads each diagnostic with its
file now; no other case's verdict moved.

## What broke and why

- **The verifier refused `depth`**, above: two copies of one rule, one changed.
- **A message the form made false**: `lent` on a C parameter that is not a
  `cstr` or a `ptr` was told *only those two are ever lent*, which a group's
  record lent read-only now is; and a handle, the pointer itself, which C
  receives by value, needed words of its own. The message asks which it is
  now (`group_handle`).
- **A fix would have written `lent lent`** on a parameter already marked: the
  fix is skipped there.
- **`layout` went red twice on `cli/assemble.hero`**, which defect 2128's
  repair took past §11's 300 lines: what a build settles before any round
  moved to `cli/settled.hero`, the probe's verdict to `value_ask.judged`, and
  the refused unit's copy to `value_beside.again`.
- **The gate's `order` was red** on a `keys(...).len()` with no `# ORDER:` mark,
  a count no order changes; the mark says so.
- The language refused the rest at the moment of writing, as it should: a
  name shadowing a function, two arguments of one type unnamed, an import no
  longer read.

## What landed, and what carried forward

**The parts**, one commit each: the row opened and its page's reading of what
the seats' reports left to the landing (`b0065d0f9`); the form and R2's
refusals, from the compiler-engineer's final prototype: a write, an `@` lend
and a lock's copy of a `lent` parameter refused, an `@` of the same place in
one call refused, nested in another argument included, and a function with
one no value (`a9ada72ab`); R3, a group's record lent read-only crossing to C
as `const T *` through a probe that carries the `const`, refused where the
header may write it and past a variadic prototype, and an `@` of the same
place in a C call refused as in a Heroes one (`71bec9371`); R5 and R4, the
slot's `read_only`, which owes no copy-out, `ir/read_only.hero`'s verifier
rule, and the messages that named `@c` naming `lent` (`f28c8a0f2`); the walk
(`abfc77457`); the specification, design.md §4.8, §4.13 and §4.19 and the
rule of the generated C (`030cb26fc`); defect 2128 (`062f8c262`); the gate's
two findings (`191478fb6`); and at the close, the evidence of sittings 213
to 224 kept beside their reports (task 2135): eight archives, 9.8 MB in all,
every text file under 2 MiB of the folders their reports cite with the copies
of the repository left out, read back byte for byte and scanned for keys, and
for 213 to 215, whose folders were all gone, a note saying so; `/panel` says
where a sitting's evidence goes since, and the folders are the author's to
remove. One decision the landing took, read from how R2 was built: a `lent`
parameter returned or copied is refused where the lock rule refuses a copy,
and a copyable value copied out of one stays legal (2138).

**Predictions scored at the close**, the sitting's table. The spec-warden,
*the specification measures exactly 10,246 real*: **hit**, exactly, the draft
applied alone and measured on `claude-opus-5`; the landing reads 10,286, the
two sentences the sitting owed and left unpriced costing 40, as the page said
the count would be scored. The compiler-engineer, *the landing measures 450
to 700 lines*: **missed**, the form's steps 2 to 5 measure +1,033 and -115
over 38 files of `selfhost/` (`git diff --numstat`, tests and comments
counted), the prototype's +524 grown by R3's C half, R5's verifier rule and
the walk; *it leaves emit, ownership and the runtime untouched*: **missed**
for two of three, `emit/` +135 and -13 for R3, and the ownership pass's
output moved where a read-only slot's copy-out went, the runtime untouched;
*`parse/`'s budget +2*: **held**, 8737; *`ir/flatten.hero` +13*: **missed**,
0, the read-only argument entering where the file's two argument loops were
merged into one. The compiler-engineer's condition, *a cold `--emit-c` more
than 2% slower turns that seat to object*: **held**, +0.54% real and +0.28%
CPU over three alternating pairs on main's sources, nothing else running,
the two compilers emitting the same 26,352,018 bytes. The ffi-pragmatist,
*`counter_init(c: Counter lent)` refused at exit 1*: **held**, measured on
the closing tree, `ffi_writable_parameter` in the compiler's own words;
*`fixedbugs-2062` rewritten with `lent` builds on both platforms*: Darwin
**held**, `run/lent-a-record-holding-a-union-is-read-from-a-binding`
printing 2; Linux arm64 **held**, the `lent` cases' `run` 31 and 0 in the
`heroes-linux-arm64` container, this case among them.

**The gate, 2026-10-18, on `191478fb6`**, the compiler clang built from the
seed, stage 2 and stage 3 emitting the same 26,591,884 bytes by `cmp`, stage
2 judging (`sources 57d3d798678a73fc`), the seed not refreshed: the full net,
31 suites four at a time and `cache` alone after, every one 0 failed: run
539, emission 1311, check 703, warnings 618, annotations 1180, surface 425,
fixes 1003, unsupported 353, corpus 70, determinism 580, lines 540, lldb 14,
probe 27, ir 30, emit 13, permissive 22, full 40, records 26, spec 31,
canonical 2, descriptors 636, grammar 9, order 3, runtime 8, special 10,
unseen 3, wholes 636, seed 1, layout 6, units 3, cache 7; the compiler's own
tests 1730 and the net's own 410. Linux arm64, from the same tree, said the
same key and read `emission` 1283 and 0 before its bless and after it, its
635 blessed files byte for byte the Mac's, so nothing came back, and `run` 31
and 0 over the `lent` cases. The site's build, 38 claims over 20 pages, exit 0.

**Status at the close**, for `docs/ROADMAP.md` § Where we are: the compiler
674 modules and 175,346 lines of Heroes; the seed unchanged, 263,383 lines,
26,240,524 bytes, 56,190 `#line`s; the spec 7846 vendored and 10,286 real
against 16384, 238 of the consolidation block's 1,500 spent; the contract
8167 real, unchanged; sittings 222, journals 68 numbered files, milestone
pages 61, measurements 47, beats 135, questions in `docs/learn/` 486 of which
461 open.

**Carried**: the census of `check` over the tracked `.hero` files and panel
187's R2, owed after the push by the optimistic chain; the formatter's probe
M-inferred-cell's and this milestone's changes to `selfhost/print/` owe; the
two shapes beside defect 2128, filed apart as defects 2136 and 2137; the
Windows box, for
the `lent` cases that bind no pthread lock; a pointer C hands back into a
read-only record lend with its `const` cast away, which the sitting recorded
and no check reaches (issue 1292 says so of strings); the prediction the
ledger's row 114 registered for the two owed sentences, scored at
M-thesis-harness; and the working folders of sittings 213 to 224, kept now
in their reports, for the author to remove.

**The chain entry**: row 68, `m-readonly-lend`, closed 2026-10-18, this
journal.

## Added after the push, 2026-10-18

**The CI's four legs on `4a24be59a`, every one green**: Linux x86-64, Linux
arm64, Darwin arm64 and Windows x86-64, with CodeQL and the site's deploy
beside them. The Windows leg read the `lent` cases that bind no pthread lock,
which this journal carried to the box, so the box, paid by the hour, was not
needed.

**The census of `check`**, owed after the push by the optimistic chain: the
compiler before the milestone, stage 2 of `b0065d0f9` (`sources
0acfc2fe242765b8`), against the milestone's, stage 2 of `4a24be59a` (`sources
57d3d798678a73fc`), each judging `4a24be59a`'s files from a worktree of its
own: 2,293 roots, `selfhost/main.hero` and every tracked `.hero` of
`examples/` and `tests/`, `check --brief`, eight at a time. Before 1,331 exit 0
and 962 exit 1, after 1,339 and 954. 14 roots moved: 13 of them this
milestone's own cases, eight from 1 to 0, the earlier compiler reading no
`lent` on a function of the program, and five in their codes; the fourteenth
in its words alone, `check/ffi-lent-is-read-on-a-lend-and-nowhere-else`,
`lent_shape`'s message reworded at step 2. **And of `build`**, since R3's
refusals and defect 2128's values are made at build, where `check` cannot see
them: the 987 tracked `.hero` outside `docs/` holding an `extern`, built by
each. Before 358 exit 0 and 629 exit 1, after 361 and 626, none exit 2; 12
moved, every one this milestone's case, each as its repair says.

**Panel 187's R2**: the frozen plan, 1,356 programs, 14,209 singles and
15,737 pairs, replayed by each compiler with `heroes mutate --recovery` and
read with `--compare`: moved 0, findings 0, in both arms. The frozen
`examples/ledger` is told one message by both, panel 210's rule being later
than the freeze, and is subtracted as the instrument subtracts a program's own.

**The formatter's probe**, owed where `selfhost/print/` moved, M-inferred-cell's
changes and this milestone's: the compiler built at `-O2`, seven invocations
side by side. The fixtures with no stride and `examples` in both families read
exit 0, `examples` 120 seeds and 69,047 variants of `single` every one held;
`selfhost` and `tests` in both families were still running at this writing,
an hour of CPU each, and their verdict is added below when they end.

**What broke after the gate, and why**: the first Linux arm64 run read 31
false failures and an `emission` of 1,904 where the tree holds 1,311 cases,
because the tree's archive was made by macOS's `tar` without
`COPYFILE_DISABLE=1 --no-mac-metadata --no-xattrs`, and its `._*` AppleDouble
entries ran as programs. `.claude/rules/platforms.md` has said so since
2026-09-03, and the archive was made before that section was read. Remade, the
run read as above, and that run alone is the one this journal cites.
