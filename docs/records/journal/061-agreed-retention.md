# 061 — M-agreed-retention: five words for what a C call does with a handle, a runtime that holds the program to them, and a line that ends where it says it does

## Goal

M-declared-extents closed with four open items and nowhere to keep them, so its
close wrote this milestone's file around them
(`docs/work/milestones/M-agreed-retention.md`, dated 2026-09-21). Each was a
shape the language admitted and said nothing about: a pointer C made, handed
twice to a C function that frees it, was `check` 0 and died saying nothing; two
modules could declare one C function with contradictory retention marks and the
compiler took both; one declaration could not reach all three of
`sqlite3_bind_text`'s retention modes; and `/panel` did not say that a seat's
copy of the tree is its own. Three of the four are one question asked at three
distances, in the file's own words: *whose word about a C function is the one
that counts*. The fourth is a process rule the closed file could not keep.

**The row opened with step 1 on 2026-09-23** (`ca150c06`, *Row 62 opens*), the
date `docs/ROADMAP.md` gives it. The two days before went to defect 074 and
panel 174, the Windows CI leg losing 106 cases to one shared file handle: the
first work on the trunk after the tag `m-declared-extents` (`316d69c4`,
2026-09-21 17:07), and counted here.

**It ended somewhere larger.** The four items were closed by 2026-09-25, and
most of the milestone went to the question step 1 found under the second: what
a declaration says a C call does with a handle. Panel 175 found that `consumes`
meant three things and no declaration said which; panel 176 split it; panel 177
answered what becomes of a handle after the call that ends it; and defect 098
showed that an address is not an identity. It closes with five words on a C
declaration's handle positions, a checker that follows a handle's life inside a
function, and a runtime that numbers every life C hands out.

**And then larger again.** From 2026-09-26 the milestone went to the formatter
and to the line: the four comment defects filed on 09-25 (lane g), the lexer
found quadratic in the size of a file (defect 103), a probe for the formatter
(panel 179), where a line may break inside brackets (panel 180) and where it
ends outside them (panel 181), the compiler's other passes found quadratic by
growing their input (lane 105), and what the emitted C zeroes (panel 182). Each
repair was attacked at the shapes beside it and each attack filed more, so the
milestone was not closed by its list: the author closed it at 16:40 on
2026-09-28, the eight defects open then repaired before the tag and any found
after carried to the next milestone
(`docs/records/log/2026-09-28-1640-m-agreed-retention-closes-over-the-defects-found-after-its-last-eight.md`).

## What surprised

**Two of the four items were not what their entries said, and a third was a
defect seen from two files.** Step 1 ran each on four platforms before any
sitting ruled (`ca150c06`). The double free of a pointer C made read as a
missing mechanism, and the same program written over a handle was already
stopped before C on all four: the item was about a `ptr` that could have been a
handle. The `sqlite3_bind_text` item said nothing was silent, which held for the
destructor's type and failed for the text's mark: `text: cstr lent` with a
`nullptr` destructor printed the bytes of a later allocation at exit 0, three of
three on Darwin and both Linux legs. And the second item's contradiction across
two modules was admitted inside one: `acquires pclose` on `popen`, then
`fclose`, `check` 0 and `run` 0 on all four platforms, defect 075. An item is
written as its finder saw it, like a defect (CL-078).

**A consuming call was three things, and no declaration said which.** It ends a
life, it hands a life into another value, or it drops one of several
references. Checking the named releaser alone, panel 175 measured, aborts three
correct programs over json-c, OpenSSL and cJSON's shape with no spelling that
runs, and a correct `sqlite3_close_v2`, since nine real pairs of releasers are
interchangeable for one acquisition. The producer side had been split at panel
148 with `borrows`; nobody had asked whether the consumer side needed the same
split before the runtime refused on it.

**A diagnostic's own note was twice the road into a defect.** The first
`transfers` asked no releaser set, so `fclose(stream: File transfers)` on a
`popen` stream ran at 0 on three platforms: defect 075 again, reached through
the repair the new diagnostic's note suggests (panel 176's critic). And defect
083, found by the llm-ergonomist reading nothing but the specification, is what
following `unmarked_handle_producer`'s note writes: an `@` cell holding a `tag
void` handle against a header's `void *`, which C converts in silence before it
frees the program's own cell. A note proposes a program, and nothing had checked
the program it proposes.

**Not one of the twenty-five defects filed on the trunk in its first half
existed when the milestone opened.** Of 074 to 090 and 095 to 102 (091 to 094
were issued by a peer session and are not on the trunk, `517b8e25`), thirteen
came from the seats of the four sittings, two from step 1, one from the
coordinator at route E's landing (089), one from CI (074), and eight from a seat
set against a landing or a repair. Seven of those eight were measured older than
the work they were found beside; the eighth, 102, was unrelated to its lane. The
landings' own faults were found too and never reached a list: step 11's first
shape passed the compiler's tests and 546 blessed emissions with six wrong
shapes inside it, found by five finder seats and two refuters per finding before
its commit
(`docs/records/log/2026-09-25-1040-a-landing-is-reviewed-before-its-commit.md`).

**An address is not an identity.** The live set knew a handle by its address,
and C may free an address and hand it out again. In defect 077 a stale handle
equal to a live one was accepted as the live one, so the one correct release
after it was the call that aborted. In defect 098 two threads over one C pool
met the same fact inside a single call, and a correct program aborted ten times
of ten on three platforms. The repair gives every life a number no other life at
any address ever had, carried by the emitted C from a call's announcement to its
end. What no number reaches stays written down: a copy made before the end and
given back after C reused the address is the new life to the runtime, pinned at
exit 0 by `limit-a-copy-given-back-after-c-reused-its-address-is-not-caught`.

**The sanitiser can hide what it is trusted to find.** Defect 077's reproducer
is caught under `--sanitize` on both Linux legs only because ASan's quarantine
does not hand a freed address straight back, which the record calls hiding the
defect rather than finding it. And design.md Part 8 wart 20 named `--sanitize`
as the remedy for a copy that outlives its release, while ASan checks reads only
in instrumented code: four copies handed to real SQLite exit 0 under it (panel
177). The wart is rewritten.

**A comment in one lane promised what only the other lane delivered.** Panel
177 landed from two lanes, the checker's route M and the runtime's poison and
dead set. Route M's comments said the runtime stops what the rule cannot see,
which was false in its own lane, where its skeptic's probes ran at 0; with both
halves together they stop before C (`65be5c8e`).

**A defect was closed without its cause being found, on purpose.** Panel 174's
brief blamed the watchdog, and the failing job's own timestamps excluded it: the
whole `run` suite took 65 seconds against a watchdog of 120. Route B, one
redirect path per spawn, was made load-bearing because it closes 074 whoever
holds the handle, and the holder on the runner was never identified.

**And the document had said it already.** Defect 088 is the copy design.md Part
8 wart 20 describes, and panel 153 had declined a shape of the same class on
that ground; the filing searched the defect list and nothing else (`521c5e02`).
It stayed filed, with the reason under it: the live set gives the shape a guard
the wart could not name.

**The list did not converge, and the reason is the method that keeps it
honest.** Twenty-eight defects were filed after 2026-09-26, 103 to 130:
eighteen by a lane's agent at the shapes beside its own repair, seven by the
seats of panels 179 to 182, three by the coordinator. Lane 123 was handed one
and filed seven beside it, five it then repaired and two carried. CLAUDE.md § RUN IT asks every repair to be attacked at its
neighbours, and a neighbour that fails is a new entry; the rule that a milestone
is tagged only over a clean list assumes the attacks run dry, and on this
surface they did not. The author's waiver is the answer, and `records/tagged`
now holds it as a second table, `CARRIED_OVER`, which excuses a tag over a
carried defect only where the defect's own item says so (`a63cf9ed`).

**A formatter can be green while it moves a comment, and only a generator finds
where.** Lane g's guard makes `fmt` compare where each comment sits in its input
and its output and refuse at exit 2 when one moved; the lane was repaired four
times after its filings, each round attacked by a seat that had not written it
(defect 101's record). Panel 179 made the generator a verb, `heroes probe`, and
its first run over the formatter's fixtures found defect 121. Its full form,
every comment position and every bracket break over every file, was priced by
arithmetic at about 4.4 CPU hours, so the net holds a stride of it and the rest
runs by hand before a push that touches `selfhost/print/`
(`.claude/rules/verification.md`).

**The specification said a line inside brackets may break between any two
tokens, and one of those breaks was an exit-0 wrong answer.** Defect 104 was
the sentence; panel 180, measuring it, found `[a` over `- b]` running and
printing `2` (defect 106): a list of two elements where the author wrote one
subtraction, run by four seats independently. Panel 181 took the other side:
outside brackets a line ends its statement, in both directions. Its critic
found that the `certain` fix `_ = ` on `total = base` over `- fee` wrote a
program that compiles and prints 100 where the subtraction meant 93 (defect
120), at the one site `.claude/rules/diagnostics-and-goldens.md` named as
obeying the fix rule by construction; the reading of 2026-09-08 is corrected
beneath it as a reading and not a run (`efaf5564`).

**Quadratic passes hid under a small constant, and growing the input is what
showed them.** Eight of the nine paths that append a token to the lexer's array
copied it, where the spelling panel 144 ruled grows it in place and one path
already used it: a file of 500 functions with escaped strings took 91.05 s user
at eight copies and takes 0.18, and `check selfhost/main.hero` went from 7.1 to
5.5 s user (defect 103). Lane 105's ladders, one to thirty-two copies, found
`fmt` at 20.70 s for thirty-two copies of `walk.hero` where one took 0.28
(defect 105, now 0.185 s a copy), and six more: the checker reading every
declaration and parameter of the program at every call to a user function,
whose repair made the compiler check itself six times faster (111), the holes
report (113), the emitter and the lowering (112), an empty file (109), text cut
inside a character (110), and the zeroing (114). None of them showed at one
size.

**Four fifths of the zeroes were values.** The emitted C zeroed every local,
106,734 `= {0}` in the seed. Panel 182 ruled that a value's one definition
precedes every read on every path and a slot is read before its first write,
so only slots are zeroed: 21,566 remain, and with each one stripped clang's
uninitialised-read warnings flag all 21,566, every one a slot and none a value.
The sitting's critic found the one place the emitter wrote a value in parts,
`map_get`'s found path, and the new suite `wholes` pins zero partial writes.
The coordinator's own reading of defect 114 that morning, that the zeroing was
the release's precondition (`f159c023`), was true of the slots and false of
the four fifths that were values.

**A cache that did not know its compiler.** Defect 122 named two caches keyed
without the C compiler; the repair found four, and keys all four on clang's
`-###` answer for the flag list and on the list itself.

**A generator finds only what its seeds hold.** `fmt` exited 2 on every
negative pattern, `-1 =>` included, and no suite caught it, because no seed of
`heroes probe` held one and the probe's families only vary what a seed holds
(defect 125). The fixture that repaired it became a seed, and the probe's first
run over it found defect 124's fifteen variants at once (`d5e088d9`). And the
six repairs of lane 123 moved no program: over the tree's 1,202 `.hero` files
no `check` exit changed but that of 127's own new golden, so every shape they
refuse was a shape nobody here had written.

## What broke and why

**A stale seed on the trunk, from a lane taken over while its agent still
worked.** `d64da8ff`, step 15, carried the seed emitted before the lane's agent
split `selfhost/emit/container.hero` at its layout ceiling, beside the source
after the split: the agent had been resumed by the coordinator's own message an
hour before and was still working when the coordinator committed. From
`dadba73b` the trunk's seed was not its source's fixpoint until `3721222d`
regenerated it and `3b40c60d` merged it, and defect 097's record gained a dated
correction underneath. What it pins is the process: a lane taken over is a lane
whose agent is known to have stopped. The same lane's gate had read `emission`
and `layout` red on its first pass and green on the re-run, cause not reproduced
(`d64da8ff`); whether that was the same story is not measured.

**A merge that lost two hand resolutions.** In the integration lane of step 14,
`92b419b4` took its generated files wholesale while two files resolved by hand
were still unstaged, and the next merge, `95006aa2`, restored both before the
lane reached the trunk.

**A commit past a failing suite.** `f22c8baf` filed defect 101 citing a
formatter file that the repair of 099 creates and the trunk did not have;
`records/citations` refused it, and the filing's command chain committed anyway,
because it read the exit status of the pipe's last command. `884d7135` is the
repair, and its body names the cause.

**Four sentences about a gate that the gate did not say.** Lane 076 merged two
run goldens with no blessed emission (`239a0c7a`), gated by the map in
`.claude/rules/verification.md`, which did not name `emission` for
`tests/golden/run/**`; the trunk read 486 and 2 until panel 176's
compiler-engineer found it (`501221db`, `f1568043`). The seven `emit` goldens
were red from step 10 to step 11, behind a gate output cut at forty lines
(`9f813de2`). Step 5's body said `warnings 204/0` over a gate that read 204 and
1 (`01bd4acd`), and `bd33f30c` called `tests/` byte-identical to its lane when
four files differed; both are corrected in their defects' records. The map's
file says so in its first section: the table is an answer, and the command is
the instrument.

**Two premises about the world expired in silence.** `HERO_RUN_MAX_ARGS` was
256, commented as an order of magnitude above the longest command line, and the
compiler's link line is one object per module: the 234th module could not be
built, and the bound is 4096
(`docs/records/log/2026-09-25-1042-the-command-line-ceiling-follows-the-module-count.md`).
And `/panel` told every brief that a rebuild from `selfhost/` takes about twenty
minutes, which panel 177's compiler-engineer measured false: 60.97 seconds from
the plain seed (`dc67e399`).

**The first repairs of defect 098 settled the ends by the order they land, and
were refused before a commit.** The filing's own left the old life's place
unpoisoned; a count of the old life's ends in flight, built in the lane,
poisoned the wrong place in the same race, since a new life holding two
references whose own end landed first would lose its name while that name still
held a reference. The number per life replaced both (`bcfc342b`).

**A count raised in the wrong table.** Defect 102's lane reports *the net's own
167, after moving the count the first run raised in the wrong table*
(`b75b908f`). The test that caught it counts the tables of
`tests/harness/suite_surface.hero`, and its comment says it exists because a
first draft once asserted numbers somebody had counted on screen.

**Windows under the sanitiser had never been run.** Route E's first run on the
box read 134 and 2, a C `abort()` and a C trap its arm did not hear; the second
read 135 and 2, the coordinator's own SIGABRT line wrapped inside the sentence
the golden asks for and the arm under ASan compiled out. Both were repaired and
measured on the box (the record of defects 080, 081, 082 and 089).

**Two sittings on a machine that moved, and a brief that asserted what nobody
ran.** During panel 175 the coordinator switched its session into a worktree
and the running seats' writes to the trunk were refused; the critic reran every
seat's numbers and found none bent. During panel 177 the Mac slept from
10:27:48 to 13:57:59, four seats and the critic stalled on a stream watchdog and
were resumed, and Windows is unrun for every row. That sitting's shared brief
said route R catches defect 088's shape; two seats measured it false on three
legs, and the synthesis names the sentence as the coordinator's inference,
written unrun.

**A commit past a failing suite, the second in a week.** `efaf5564` filed
defect 123 and left the ROADMAP's count at six while the list said seven;
`records` refused it and `e90b682e` repaired it. It is `f22c8baf`'s story
again, and since then a records commit runs only inside `if` the suite's own
line reads 0 failed.

**A resolution written past its measurement.** Panel 181's item 1 (ii) listed a
line beginning with `(` or `[` as one that can only continue the line before;
lane 181 measured a legal program where such a line begins a statement
(`surface-fixtures/comments101/parenplace.hero:10`), and the item was narrowed
before it landed (`d3fad66c`).

**The machine stopped under the work.** The Mac slept with its lid closed or
on idle during the loop, and every lane stalled for the gap; the Windows box
went offline on its tailnet until the author reconnected it. Neither is a
defect in the tree; both are why the platform runs of this half took hours.

**Lanes beside the project.** Every lane was a worktree in a sibling
directory, each needing its own grant in the session's settings, and lane 182
stalled on permission prompts until the author approved them. The author's
rule of 2026-09-28 puts lanes under `.claude/worktrees/`, measured with a trial
worktree before it was written (`6b81e993`).

**A clock that did not ring.** Twice on the evening of 2026-09-28 a scheduled
wakeup did not return the coordinator to the loop, and the author waited thirty
minutes and then eighteen for an update the contract says comes every three
(CLAUDE.md § 3). A background timer whose exit notifies became the second
clock, and a monitor over the platform logs the third.

## What landed, and what carried forward

**Fifty-one defects closed on the trunk**, of fifty-three filed during the
milestone (074 to 130; 091 to 094 were issued by a peer session and are not on
the trunk), and the other two carried. Up to 2026-09-26: 074, the net's shared
redirect files; 075, 079, 084 and 085, the releaser set, the reference, one call
at two consuming positions and the releaser read in its own module; 076, 080,
081, 082, 089 and 090, runtime sentences measured false or deaths it did not
hear; 077 and 088, a handle used after its end; 078, 083, 086, 087, 095, 097,
098 and 102, from `owned` on a `const char **` cell to JSON that was not JSON.

**Four more were filed in the formatter's comment printing**, each beside the
repair before it: 096 by the skeptic seat over 095's repair; 099 by the parser
seat that repaired 096; 100, and 099's shape without its blank line, by the
skeptic seat over 096's repair, which also found one shape that repair's first
form broke; and 101 by the skeptic seat over the repair of 096, 099 and 100.
**Lane g closed the four** (`316d974f`, 2026-09-27): `fmt` keeps every comment
where it was written, and refuses output that moves one, the guard in
`print/anchors.hero` comparing each comment's block and the tokens `fmt` keeps
on either side of it (defect 101's record, which holds the four).

**The milestone's four items, all closed.** The pointer C made and gave back
twice now dies saying so on four legs, by panel 175's route E (step 4), and
§ 13's `tag void` sentence says how a `void *` C hands out to be given back is
declared. Two declarations of one C function in two files must agree at every
position they share at one type, `contract_differs` (step 12). One declaration
still cannot reach all three of `sqlite3_bind_text`'s modes, and the record says
why one module per mode is the route (step 12). And a seat's copy is its own, on
the author's instruction (step 2).

**What the language gained**: five words for what a C call does with a handle,
`consumes`, `transfers`, `acquires`, `retains` and `borrows`, the three that
name releasers taking a set, `acquires sqlite3_close | sqlite3_close_v2`; `when
0` after a result, naming the result that means the call did what it was handed;
a transfer's receiver that must outlive the call (`transfer_without_receiver`);
one contract per C name (`contract_differs`); `ffi_owned_const_cell`; and
`handle_used_after_end`, the checker's one flow analysis, decided inside a
function. § 13 spent four ledger rows, 204 to 207 of
`docs/measurements/010-spec-budget-ledger.md`: +91, +82, +362 and +56 on the
reader's instrument, 8270 to 8861 real and 6212 to 6693 vendored, three of the
four over `DELTA_GATE` on the author's word of 2026-09-23, *pay all the tokens,
without economising*.

**What the runtime gained**, its ABI moving 22 to 26 in four steps (10, 11, 14
and 16): a crash handler that calls the handler it found before it speaks, says
*under* the Heroes function, hears SIGILL, and leaves an ignored signal alone; a
live set that keeps the releasers beside each address and counts references;
the ends a call announces, pending until it returns; a dead handle overwritten
where it lay with an address on a page mapped with no access, and the last 2^20
ended addresses remembered; and a number for every life. The cost is stated:
+3.9 ns a handle argument, and the dead set at most 16 MiB held (`1aa69f08`).

**What the net and the process gained**: from panel 174, one redirect path per
spawn, clang no longer inheriting the harness's capture, `FILE_SHARE_DELETE` and
never `FILE_SHARE_WRITE`, the process tree killed, and `Ran` saying why a process
did not start; a process that never started refused rather than given an exit
code it does not have (`497a048f`); a silent ending that refuses a word on
stderr; the contextual words a table `suite_grammar` holds to the parser; a seat
building in a copy of its own; and a landing reviewed by finder seats and
refuters before its commit. Over the milestone, before the formatter's lane,
`git diff --shortstat m-declared-extents..HEAD` at `bdf430f1` reads `selfhost/`
+5256 and -894 lines, `runtime/` +1425 and -187, the spec +27 and -7; at the
close, at `687c54f3`, `selfhost/` +16,457 and -2,617 over 165 files, `runtime/`
+1,470 and -210, the spec +38 and -11.

**After 2026-09-26**, closed on the trunk: 103 and 105, the lexer and every
printed artifact linear; 096, 099, 100, 101, 107, 108 and 121, comments the
formatter moved or dropped; 104 and 106, the line inside brackets; 116, 118, 119
and 120, the line outside them; 109, 110, 111, 112, 113 and 117, from an empty
file to `mutate`'s walk; 114, the zeroing; 115, Windows naming where C read a
dead handle; 122, the cache; and 123 to 128, a `match` arm's pattern (lane 123,
`eed23b19`). **Carried**, by the author's waiver: 129, a line holding `-` alone
that draws two certain joins, and 130, a statement skipped after a failed arm.

**Eight sittings on the trunk**, 174 to 177 and 179 to 182; 178 sat on
2026-09-25 on a peer session's branch, `lane-panel-178`, for
M-buildable-structs, and is not on the trunk. The ratifications of 175, 176,
177, 179, 180, 181 and 182 are the seven open items of `docs/work/DECIDE.md`,
the first three declined by the author for now on 2026-09-24. The trunk runs on
their provisional resolutions, and on one decision the landing took that no
sitting ruled: a transfer that was made ends the program's own name for the
value.

**What the language gained in its second half**: inside brackets a line breaks
by how it ends, and a list whose elements a line end separates refuses a
subtraction it would split (panel 180); outside brackets a line ends its
statement, and a line ending in an operator or beginning with a spaced `-` is
refused, its join `certain` only where the next line cannot stand alone (panel
181); and a `match` arm's pattern is one literal after at most one `-`, as spec §
8 writes it, takes the width of what it matches, and is refused where an earlier
arm covers it (defects 124, 127 and 128). Two more ledger rows, 6794 and 6838,
panels 180 and 181's sentences: the spec reads **9060** on the reader's own
instrument and 6838 vendored at the close, 1180 free against 10240.

**What the tools gained**: `heroes probe`; `fmt`'s guard; the verifier's
`in_order` check within a block; the `wholes` suite; the build caches keyed on
the C compiler and its flags.

**What the process gained**: the author's instructions of 2026-09-27 and
2026-09-28 in CLAUDE.md § Precedence (never economise on tokens, the spec's own
included, and where robustness is not at stake the route fastest at run time);
a borrowing credited in the session that makes it (`2b1a1f24`); lanes under
`.claude/worktrees/`; `CARRIED_OVER`.

**Carried forward**, each where it was written, since the milestone's file holds
no open item: the limits § 13 and design.md Part 8 wart 20 state, pinned at exit
0 by the five `limit-` cases in `tests/golden/run/` (a copy read or given back
after C handed its address out again, a copy read after a million later ends, a
handle C keeps across the release and hands a callback afterwards); the one
interleaving defect 098's repair does not make exact, argued and not run;
`sqlite3_bind_text`'s single declaration, an expressiveness gap at design.md
§1.11's boundary; a success range, `> 0`, a question and not a form (panel 177's
item 6); what panel 174's yes did not settle, the holder on the runner, route H,
the 457 `is_err()` branches that discard an error value, and whether a process
that never started should produce a verdict; a Part 6 row of design.md that
still says *the six contextual words*; defects 129 and 130, carried by the
author's waiver and written in `docs/work/DEFECTS.md` with that waiver named in
each; design.md §4.15's panel 180 bullet, which names a literal's elements where
the compiler now refuses the same shape opening an arm, and whether a line
holding an arm stands alone under panel 181's item 3, both written under those
sittings' ratifications in `docs/work/DECIDE.md`; `grammar_expr.hero`'s ceiling
of 1085, left above the 1013 lines it now holds; and panel 179's historian,
carried to 2027-09-27, the horizon the seat registered.

**The gates at the close**: on the trunk at `c7b03a12`, whose code equals lane
123's merged tree, the fixpoint by `cmp`, the compiler's 828 tests, the net's
own 179, and the whole net at **3,144 passed, 0 failed** over 26 suites: check
167, ir 24, emit 8, unsupported 15, run 211, annotations 216, determinism 241,
emission 638, descriptors 303, wholes 303, cache 6, units 3, fixes 32, lines
212, corpus 55, warnings 272, records 24, surface 334, special 10, spec 20,
grammar 9, layout 4, canonical 2, probe 24, order 3, runtime 8. Linux arm64 and
the Windows box on lane 123's merged tree, the compiler's 828 tests and 19
suites each at 0 failed, EXIT 0 on both. And the by-hand probe
`.claude/rules/verification.md` owes before a push that touches
`selfhost/print/`: `single` and `bracket` over the 1,046 seeds of `selfhost`,
`tests` and `examples`, and every family over the fixtures, unstrided, 1,287,836
variants that parse, every one held by both judges and none refused, run as
2,273 jobs seven at a time in 57 minutes on a compiler built at `-O2`; the 90
files the parser refuses, 180 of the jobs at exit 1, are the ones the probe's
own walk of `tests` names.

**The predictions** are scored in
`docs/records/done/2026-09-28-2138-the-predictions-of-m-agreed-retention-scored.md`,
93 rows over the eight sittings and the six ledger rows. Of panels 174 to 177
and their four rows, 57: 27 held, one with its Windows clause lapsed because the
box has no `sqlite3.h`; 7 were falsified, three inside the sittings; 14 are
void, resting on a route or a text that never landed; 1 lapsed on its sitting's
own ruling; 5 rows carry seven predictions, five to M-thesis-harness and two to
the first milestone whose `examples/` gains a program of their shape, since
`examples/` did not change in the milestone; and 2 rows point to others. The
four falsified at a landing are all counts a prototype priced, lines added or
emissions moved, and each came out larger. Of panels 179 to 182, 36: 22 held, 9
falsified, 3 lapsed on the one instrument with no arm, the metric 2 task suite,
1 void and 1 carried to 2027-09-27; the falsified are again counts, lines and
variants and the help text, each larger than priced, and one is a landing's own
score overturned: panel 179's fixture count, which the landing read as held on
the seats' generators and the landed probe does not reproduce. One verdict is
the close's push to write: panel 174's compiler-engineer named the pre-push
Windows leg and the two after it, and nothing has been pushed since `d02b8bf4`.