# Panel 230 — a stop says where and what it held, a report cannot be forged, and `exit` takes a byte

2026-10-26, at M-panic-location (row 72), in the lane `panic-location`. **Full panel**: the five
seats and a completeness critic, which read the briefs before any seat started (40 findings,
`230-reports/critic-briefs.md`) and the reports after (19 findings, `230-reports/critic-reports.md`).
**Status: `provisional — author ratification pending`**, queued as decision
https://github.com/heroes-lang/heroes/issues/2227.

Convened on task 812, features 1001, 1155 and 2083, and on the defects the critic's passes found and
the coordinator reproduced and filed the same day: **2224** (`exit(code:)` with a multiple of 256
tells the shell the program succeeded), **2225** (a line feed in a program's own text forges lines
of an abort's report) and **2226** (the control arm's unmatched value written raw). By the author's
answers of that day: the full panel; issue 1001 in the same sitting with a resolution of its own;
issue writes for the session; the blind seat as 84 short sessions within 12 USD (2.9475 USD spent),
the API account's cap raised by the author when it stopped the first run. The briefs are
`230-briefs/` (committed before the blind seat's first session, `811e84d39`), the reports
`230-reports/`, the evidence `230-reports/evidence.tar.xz` (`evidence.md`).

## The proposal, as the seats received it

Three sentences of the specification, each a question the sitting rules: § 1, `main` *produces
nothing or a `()?`: a `fail` it returns is written on stderr, and the program exits 1*; § 6, an
abort *saying why and where: the file, line and function*; § 12, an `assert` failure shows *both
sides of a comparison, written as this document writes a value*. Beneath every abort's unchanged
first line, a place, `  at <file>:<line>:<column>, in <module>.<function>`, the file as the
program's modules name it and the column the start of the expression that stopped the program,
passed as a static record and never computed where nothing aborts (L1); an `assert`'s sides written
by renderers the compiler emits (S1). The routes beside it: L0 (`__FILE__` and `__LINE__`), L2 (the
line table read at the stop), L3 (a thread-local site), L4 (the frame walk), L5 (an address resolved
off-line); S2 (scalar records only), S3 (the first difference), S4 (today); M2 to M7 for `main`, and
`exit`'s range (`00-shared.md` § The routes).

## What was measured

- **The blind seat** (the coordinator's experiment, scored by `score.py` before the key was read;
  `230-reports/llm-ergonomist.md`, which is **the coordinator's reading of its own experiment**,
  finding 15). Family P, eight stopped programs on Haiku 5.5, three sessions a cell: **24 of 24
  fixed and 24 of 24 located under today's stderr, and the same under the proposal's**; 15 of 24 P
  answers cite the place, 9 reconcile it against `task.md`'s own line numbering, where the program
  stood under 28 to 31 lines of text, and 3 conclude it wrong: a flaw of the design, not a reading of
  the place. Family M, six programs to write on Sonnet 5.5: **0 silent of 30 default error cases in
  either variant**, but every task opened by saying the status is read, so **the default issue 1001
  names was not measured** (finding 1); writers took `main -> ()?` in **15 of 18** sessions where
  it was offered, and passed 13 against 14 of 18.
- **The clock** (`230-reports/timings.md`, one still window with both other sessions holding, every
  kept run's `real` within 1.10 of `user + sys`): the places a program carries cost its path that
  does not abort **+0.02% of instructions** (the compiler as the program); the compiler's own
  emission **+1.25%** (×1.0117 instructions), its whole cold build **+3.2%** (×1.0251); the seed
  **+3.29%** under literals on the failing branch, **+7.4%** under a record per site with 416 KiB
  rebased at load. The kind-2 table compared the prototypes' bodies, not the routes (finding 4), and
  the two record forms were compared on bytes and rebases alone (finding 5): both corrected beneath
  the record.
- **The real token counts** (`230-reports/spec-warden.md` § 4, `--refresh` applied and reverted,
  baseline 10,297): the proposal **+30**; § 12 as `12min`, *both sides of a comparison*, **−21**;
  `exit(code: u8)` **0**; § 6 as drafted +10, every narrowing +8 to +20.

## The verdict table

| seat | verdict | section | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| **compiler-engineer** | **object, no veto**: approve L1 kind 1, L1a kind 2 with a pointer, L4 kind 3, S1 under four conditions, M5, `exit`'s range refused or aborted; object to § 6 as drafted, M1, M7 | design.md §1.12, §1.1, Part 5, §4.20 | `selfhost/` +238 code lines of 122,243, runtime about +380 of 14,436, ABI 30 → 31, seed +3.29% | instructions ×1.00 to 1.04 (**hit**: 1.0117, 1.0251); seed at the close 27.80 to 28.05 MB | § 6 narrowed; every kind-2 entry point located, the store first; 2225 repaired first; S1's four conditions |
| **ffi-pragmatist** | approve L1, L4 and L5 for kind 3, S1, M1 (order A), M5; object L0, L2, L3, *the low eight bits*; **veto** L1 with signatures changed in place, any renderer that reads through a foreign pointer, M4's place in `HeroFailure` | design.md §4.20, §1.11, §1.12 | an ABI-31 runtime compiled clean with and without `-Werror` at both levels; SQLite byte for byte | `examples/sqlite` and `ledger` pass with zero edits under L1's new names | the in-place veto never lifts; the S1 veto lifts only if spec § 7 stops comparing a `cstr` by address |
| **spec-warden** | **veto § 1 (M1)** on Principle 0 until a measurement pays it; **veto § 6 as drafted**, approve zero tokens; approve § 12 as a removal (`12min`); approve `exit(code: u8)` on W5; § 13's `13either` (+5) only with M1 | design.md §1.6, §1.2; CLAUDE.md § 2 | proposal +30; recommended −21 (10,276) | W1 to W6 | any of: 2 of 18 T sessions write `main -> ()?`; family M's silent count 5 lower under P; 052's `main_returns` 5 of 480 |
| **historian** (advisory) | approve | — | every precedent verified, the pages fetched in the sitting | L1's bytes from line and column, not names; without one corpus the two notations drift; M1 extended toward M6 | — |
| **llm-ergonomist** (the blind seat; **the coordinator's reading**) | the tables above; the coordinator's reading objects to M1 on its measurement, corrected beneath to Principle 0 | — | 2.9475 USD, 84 sessions | correction turns after an abort within 2 points of 052's (unscorable until 052's base is counted) | a family P run off the ceiling; a family M run that measures the default |

## Disagreements, stated plainly

- **§ 6.** The compiler-engineer wants it narrowed to what lands (*an abort the program's own
  operation raises names the file, the line, the column and the function; a fault the runtime
  catches names the function it stopped in, where the platform can tell*); the spec-warden vetoes the
  drafted sentence and objects to every narrowing it priced, each keeping an exception (stack
  exhaustion, a call through an `extern` pointer, Windows), and approves zero tokens. **Taken: zero
  tokens.** The place names itself in the output, no program is written against it, and *saying why*
  stays true under every route; the narrowing goes on the milestone's page instead, where a promise
  about the tool belongs.
- **`main -> ()?`.** The ffi-pragmatist and the historian approve it, the compiler-engineer objects
  and the spec-warden vetoes on Principle 0. **Taken: not adopted**, on Principle 0, and not on family
  M's table, which did not measure the default (finding 1).
- **M5.** Two seats approve a stderr built-in; the spec-warden makes it wait on whether a program may
  bind `hero_os.h`. **Taken: the door goes to the author** (R6), M5 waits on it.
- **The kind-2 form and the record form.** The seats disagree on the store (the compiler-engineer's
  L1a everywhere; the ffi-pragmatist no ground) and on records against literals; the measurements that
  would decide them were of the prototypes' bodies (findings 4, 5). **Taken**: the store becomes kind 1
  (the read's shape), the rest of kind 2 a per-site pointer, literals for kind 1; each choice recorded
  with the axis it rests on and the measurement the landing owes.
- **A group record's side.** The compiler-engineer's prototype writes `Name(...)`, the ffi-pragmatist's
  writes the fields with pointers as addresses. **Taken: the fields**, under the address rule; only it
  makes § 12's sentence true for such a record (finding 9).

## The resolution adopted, provisionally

**Robust, and neither the cheapest route nor a compromise between the seats.** Seven parts and an
order.

**R1. Every abort a program's own operation raises names its place, and no sentence of the
specification promises it.** Beneath today's first line, `  at <file>:<line>:<column>, in
<function>`: the file as the program's modules name it (the root's file name, a `use`d module's
path from the root's directory), never the path the build was invoked with (finding 6); the column
the start of the expression whose evaluation stopped the program, an `assert`'s asserted expression
included (a departure from OCaml's keyword and Rust's macro start, taken because it is the expression
the head line quotes); the function `<module>.<name>` as the module is bound where it is used, a test
`test "<title>"`.
- **Kind 1**, written in the program's C (the index read, overflow, division, shift, `.must()`,
  `assert`, the C-boundary checks the emitter writes): four literals on the failing branch, passed to
  runtime entry points **added beside the old ones** (`_at`), never the old ones changed (the
  ffi-pragmatist's veto, design.md §4.20); `.must()`'s and `assert`'s `#line` stay the generated
  file's (`emit/abort.hero:8-10`). Literals rather than a record per site, **on seed bytes and load-time
  rebases** (+3.29% against +7.4%, and none against 416 KiB); `__text` grows faster under them, and
  clang over the literal seed is owed by the landing (finding 5).
- **The indexed store** (`xs[i] @ v`, 17,163,158 calls a self-emission, through `hero_array_set`)
  **becomes kind 1**: its bounds check written in the program's C as the read's checked form, the
  runtime's check kept behind it as an invariant; after a seam in `emit/container.hero` (300 of 300).
- **Kind 2**, inside a runtime call on the path that does not abort: every entry point the census
  lists (`compiler-engineer.md` § 2: the `str` and array slices, `sort`'s `nan`, a map key not equal to
  itself, `repeat`, the C-boundary checks the runtime makes, the lend checks, the handle reports,
  `end_lease` twice, `hero_dir_at`, the thread checks, `run()`, the callback guard) takes **a pointer
  to a per-site static record**, its body the original's with the place added, its per-call cost
  measured at the landing against the original (finding 4's command) and reported.
- **The library's own Heroes** (`args`, `range`, `exit`, `map`, `find`): its functions that can abort
  receive their caller's place, so an abort in them names the program's call into the library, never
  a line of `<heroes library>` nobody can open (finding 11): `#[track_caller]`'s shape, for the
  library alone, with no surface.
- **Kind 3** (the fault handlers, the exit check, the lease handlers, the past-the-buffer report):
  the walk as today (`hero_stack_names`), its names mapped to the program's spellings (`thread_deep`,
  not `threaddeep`, finding 12); on Windows the failure without the function, as today
  (`stack.c:850-857`); an unended lease names no acquisition. These limits go on the milestone's page
  (R7).
- **The first line and the place are written from a fixed buffer, before anything that allocates**
  (finding 6): an index, an overflow or a division abort allocates nothing today and must not start.
- **`HERO_RUNTIME_ABI` 30 → 31** by `abi-put-back`; the old names kept while the seed calls them and
  removed in a step after the release refresh whose seed knows the new ones (a removed function is
  self-guarding at link).
- **The cost, accepted on rank 3** (CLAUDE.md § Precedence; design.md §1.12, *what would make this
  wrong: an abort that stops a program at a place that tells the reader nothing*): +0.02% of
  instructions where nothing aborts, the compiler's emission +1.25%, its cold build +3.2%, the seed
  +3.29%. **§ 6 is unchanged**, zero tokens.
- Not taken: L0 (its three measured failures), L2 and L5 (a debug-information reader, the
  compiler-engineer's veto condition; no column; line 0 at `-O2`), L3 (a store on every path).

**R2. A failed `assert` on any value shows both sides, held to one notation, and § 12 says so in
fewer words.** Renderers the compiler emits for the types that stand as sides (a worklist from the
assert sites, not every record), called after the head line and the place are written. **Every type
`==` accepts**: a record as its construction, a variant `.case(field: v)`, `[T]` and a fixed array
`[e0, e1]`, `{K: V}` with its entries **in the order of their whole rendered text** (two equal texts
cannot show their order), `T?` as `ok(v)` or `fail(code: "...", msg: "...")`, a `str` quoted in the
specification's escapes, a number as `print` writes it; **a group record field by field**, and a
`ptr`, a `cstr`, a function value, a handle and a union's pointer member **as their address, never
read through** (the ffi-pragmatist's veto). Bounded as the lldb formatter (16 elements, 1,024 bytes
of text, 256 values, depth 6, each cut written `... N more`), and where the two sides render alike (a
`nan`, a difference past a bound) a line says so. **Held to one notation with
`editors/lldb/heroes_lldb.py` by one corpus of values both render, compared by a test**, with two
exemptions written into it: maps until the formatter orders them the same way (the landing changes
it, ending the exemption) and group records always (lldb reads a dangling `cstr`). § 12 becomes
*An `assert` failure shows the source expression, and both sides of a comparison.* (`12min`, −21
real) **in the commit that completes S1 and its test, never before** (finding 9);
`tests/harness/suite_special.hero:308-317`'s tripwire is unpinned there, with a `fixedbugs` case per
shape (defect 016, issue 991).

**R3. A program's text never forges a line of a report (defects 2225 and 2226).** A **value** in a
report, an `assert`'s side or the control arm's unmatched value, is written as the language writes
it: a `str` quoted, with `\n`, `\t`, `\r`, `\\`, `\"` and `\u{...}` for any other character a line
cannot show (`\u{0}` for a NUL, the formatter's spelling). A **message** in a report, `.must()`'s code
and message, an `assert`'s source text, a test's title, keeps panel 192's R7 notation, the
compiler's diagnostics' too (`<U+001B>`), and **loses R7's two exceptions inside a report line**: a
line feed is `<U+000A>`, a tab `<U+0009>`. That amends R7 on defect 2225's evidence, and the author
ratifies it with this sitting. So a value can be pasted back as a literal, a message reads as the
compiler writes program text, and neither can impersonate a line; `.must()`'s first line stays
today's for every printable text (`run/must-aborts` unmoved), and the one pinned side moves
(`run/fixedbugs-355-an-assert-writes-its-sides-whole-and-by-their-code`). `hero_panic_unmatched_value`
writes through the printer (defect 2226).

**R4. `exit(code: u8)` (defect 2224).** One word of § 11, 0 tokens: a literal outside 0 to 255 is
refused at compile time (`int_out_of_range`), a computed `i64` until converted (`to_u8(n)`, a `u8?`,
`.must()` or `.default(v)`). The ten computed calls convert (`examples/interpreter/main.hero:63`,
seven in `selfhost/main.hero`, `tests/harness/issues.hero:120`, `tests/harness/thesis.hero:108`), and
`heroes run`'s forwarding of its child's status leaves by an entry point of its own, so Windows'
`0xC0000409` stays faithful (finding 13). 128 to 255 stay admissible, a choice recorded: a shell reads
them as a signal's death, and Rust's `ExitCode` and Zig's `main` take the same byte. **Adopted ahead
of the spec-warden's W5**, which counts measurement 052's sealed programs and is the author's to
count; if 3 or more of its 480 turn-1 answers pass a computed code, the fallback is `11abort` (+15),
a literal refused and a computed code outside the range an abort. The landing owes `check` goldens
annotated `#~ int_out_of_range` and `#~ type_mismatch` (W6).

**R5. `main -> ()?` is not adopted (issue 1001).** On Principle 0: the compiler would not use it (its
statuses are 0, 2 and computed, and it writes stderr through a binding), and no thesis effect is
measured, family M having told every writer the status is read (finding 1). Panel 159's R3 stands,
re-read and not reversed, and its llm-ergonomist's prediction stays **unscored** (its condition, a
document without R3's sentence, was never run; finding 2). What was measured is a preference: 15 of
18 writers took the shape when offered, with no effect on correctness. **It reopens on** a
measurement of the default, a task that does not say the status is read, showing silent error paths;
or on 052's turn-1 answers refused `main_returns` in 5 or more of 480 (W4, the author's to count). **If
it is ever taken**: the order measured (the failure's line, its release, then § 13's check, whose
abort wins at 134), § 13's `13either` (+5) in the same commit, and no place in `HeroFailure` (the
ffi-pragmatist's veto on M4). **`main_returns` gains the golden it never had** (finding 40 of the
first pass), owed whatever the ruling.

**R6. Whether a program may bind `hero_os.h` is the author's, and M5 waits on it.** The tree runs on:
it may (10 programs of `examples/`, 30 of `tests/`, 12 files of `selfhost/` bind it), and the
specification names it 0 times. Ruled the library's, internal, M5 (a built-in writing a line on
stderr) becomes the compiler's need and enters; ruled the language's, the specification owes a
sentence naming it. A decision issue of its own,
https://github.com/heroes-lang/heroes/issues/2228.

**R7. The milestone says what it delivers.** Its page and the ROADMAP row say: every abort a program's
own operation raises names its file, line, column and function; a fault the runtime catches names
the function where the platform can tell, Windows not; an unended lease names none; an `assert` on
any value shows both sides; and no program's text can forge a report.

**The landing's order**, one lane: (1) R3 and R1's fixed buffer, defects 2225 and 2226; (2) R4, defect
2224, in the order the seed's rule asks (a library signature the compiler's own code uses changes
only after the refresh whose seed knows it); (3) R1's kind 1 with the ABI step, the new names and the
store moved; (4) kind 2, entry by entry, each measured; (5) the library's caller place; (6) kind 3's
names; (7) R2 complete with its notation test, § 12 and the tripwire in its commit; (8) `main_returns`'
golden; (9) the refresh by `abi-put-back`; (10) the old names' end after the release.

## What the conservative resolution would have been, so the author can choose it

L1 for kind 1 alone: no kind 2, no library place, kind 3 unchanged; S4, today's § 12, with R3's
repair alone; `exit`'s range by refusing a literal and aborting a computed code (`11abort`, +15); M1
not adopted; § 6 unchanged. It was not adopted because it leaves the commonest runtime aborts (the
slice, `sort`'s `nan`, the library's) saying nothing of where, an `assert` on a record saying nothing
of what, and `exit`'s silent success answered at run time where the type answers it at compile time.

## Prior art, per resolution

The historian's report (`230-reports/historian.md`), every page fetched by that seat and **re-read by
the coordinator on 2026-10-26**:

- **R1**, *taken from Rust*: RFC 2091 (start date 2017-07-31), a function *"which accepts the
  caller's location as an additional argument"*, and `caller_location()` *"returns the `Location`
  structure which encodes the file, line and column of the callsite"*; the library's caller place is
  that shape. *Taken from Swift*: SE-0274 took the full path out of `#file` because *"It can
  inadvertently reveal private or sensitive information"*, *"It bloats binaries"* and *"It introduces
  artificial differences between binaries built on different machines"*, the new form being
  *"`<module-name>/<file-name>`"*.
- **R2**, *taken from Rust*: `assert_eq!` *"will print the values of the expressions with their debug
  representations"* (since 1.0.0), and `escape_debug` *"will escape the characters similar to the
  `Debug` implementations of `str` or `char`"*, a line feed written `\n`. *Reached first by Node*:
  `util.inspect`'s `sorted`, *"all properties of an object, and `Set` and `Map` entries are sorted in
  the resulting string"* (v10.12.0).
- **R4**, *taken from Rust*: `ExitCode`, *"Constructs an `ExitCode` from an arbitrary u8 value"*, and
  `process::exit`, *"On most Unix-like platforms, only the eight least-significant bits are
  considered"*, `process::exit(0x0100)` being *"`0` on Linux, but `256` on Windows"*. *Reached first by
  POSIX*: *"only the least significant 8 bits (that is, status & 0377) shall be available from
  wait() and waitpid()"*. *Reached first by Zig*: `main`'s return type *"'void', '!void', 'noreturn',
  'u8', or '!u8'"*.
- **R3, R5, R6, R7**: none.

## Predictions to score

| seat | prediction | scored |
|---|---|---|
| compiler-engineer | the after compiler retires ×1.00 to 1.04 the before's instructions, emission and whole build | **hit**, 1.0117 and 1.0251 (`timings.md`) |
| compiler-engineer | `seed/heroes.c` at the close, L1 complete and S1 reachable-only, 27.80 to 28.05 MB | M-panic-location's close |
| ffi-pragmatist | `examples/sqlite` and `examples/ledger` pass with zero edits; their emitted C differs from today's only on lines holding an abort call (restated for literals: no records' line exists, finding 19) | the landing of R1 |
| ffi-pragmatist | an `assert` on two `struct passwd` records after `endpwent()` prints both `pw_name`s as addresses, no ASan report, 134 under Guard Malloc | the landing of R2 |
| spec-warden | W1: the landing reads 10,276 (`12min` and `11u8`) against 10,297 | the landing of § 12 |
| spec-warden | W2: at most 1 of 18 T sessions declare `main -> ()?` | **holds**, 0 of 18, read as R3's sentence working (finding 2) |
| spec-warden | W3: P's silent count within 4 of T's, passed within 2 | **holds** (0 and 0; 14 and 13), the default unmeasured (finding 1) |
| spec-warden | W4: 052's `main_returns` at most 4 of 480; W5: computed exit codes at most 2 of 480 | **owed to the author**, the run sealed |
| spec-warden | W6: `exit(code: 256)` refused `int_out_of_range`, `exit(code: n)` `type_mismatch`, the compiler's calls build | the landing of R4 |
| historian | L1's seed bytes come from line and column, not file names; M-bounded-seed asked to merge places within two milestones | two tags after the landing |
| historian | without one corpus the two notations drift within two milestones | **its condition not met** if R2's test lands; scored at two tags after it |
| historian | M1 extended toward M6 | **lapsed**, M1 not taken |
| coordinator (blind) | correction turns after an abort within 2 points of 052's | unscorable until the author counts 052's base |
| panel 159's llm-ergonomist | ≥1 in 3 first attempts at a fallible `main` write `-> ()?` | **unscored**: its condition never run (finding 2) |

**The spec-warden's two counts, scored 2026-10-29** (decision 2241,
https://github.com/heroes-lang/heroes/issues/2241): the author unsealed the primary's transcript,
`tests/thesis/held-out/runs/c24a5b81.enc`, on their Mac and counted it with commands printing
numbers alone, no line of a program on the screen (decision 4a of M-thesis-harness); the folder
`build/thesis-runs/c24a5b81` this table's issue named had been cleared with the scratch on
2026-10-25, measurement 052's own note. Of the 480 turn-1 answers: **W4 hit**, `main_returns`
refused in **0** (at most 4 held; the strict arm's turn-1 codes read `needs_label` 74,
`expected_arm_arrow` 25, `bad_operand` 24, `discarded_value` 10 at the top, so the path counted is
the verdicts' own); **W5 missed**, a computed exit code passed in **3** (at most 2 held): 473
answers pass a literal, 7 call `exit` nowhere, 2 pass a name and 1 another expression, and each of
the three is an `i64` the compiler of `sources 5e0ca4c634086a7c` refuses at the call with
`type_mismatch`, *expected `u8`*, none a name already of type `u8`. So R4's own condition is met and
its fallback lands, `11abort`, a literal outside 0 to 255 refused and a computed code outside it an
abort, +15 real: task 2250 on M-compatibility-promise
(https://github.com/heroes-lang/heroes/issues/2250). R5 stands.

## What the seats could not run

Linux, no leg in this sitting; the Windows box (`0xC0000409` again, `exit(code: 256)` there, dbghelp);
measurement 052's sealed programs (W4, W5, the `abort` and `crash` base); a memory exhaustion at an
abort (read in the code, finding 6); clang over the literal seed and the routes' own kind-2 cost
(findings 4, 5); the default of a writer told nothing about the status (finding 1). Each is written
where it is owed.

## Found beside the sitting

- **Defects 2224, 2225 and 2226**, filed and reproduced, closed by R4 and R3.
- **The brief carried a stale premise**: *M-bounded-seed's target is a seed under 10 MB*, from its
  page's line 34; the same page says beneath it that the author amended that target on 2026-10-17
  (panel 219: no emission profile reaches 10 MB, the floor measured at 18,831,319 bytes). Read
  forward, as CLAUDE.md § RUN IT says, it was no target here; the compiler-engineer's own reading, the
  `#line`s 947,574 bytes of the seed, agrees.
- **The consolidation block's lapse**: the spec-warden reads the 1,500 as having bound only *before
  the thesis harness has run*, which happened on 2026-10-24, and asks the author to write the lapse
  down; it changes nothing here (§ 12 is a removal).
- **For the author: files written outside the repository's root by the system**: macOS's crash
  reporter wrote 400 reports dated 2026-10-26 into `~/Library/Logs/DiagnosticReports/` for the aborting
  binaries the seats, the critic and the goldens ran (finding 17), and the historian's fetch tool saved
  two PDFs into the session's own folder under `~/.claude/projects/`. Nothing was removed, a deletion
  outside the tree being asked for. Whether a file the OS writes about a process a session ran falls
  under CLAUDE.md's hard stop is the author's question.

## Process, recorded against this sitting

The critic's first pass wrote nothing until a rate limit stopped it, and was resumed from its
transcript; every brief since asks a seat to write from its first finding. The API account's cap
stopped 42 blind sessions and the real token count at 12:33, and the author raised it. Sonnet 5.5's
safeguards refused 10 sessions on the brief's `choice_points` section (`reasoning_extraction`), the
shape panel 184 met first; family M's second brief dropped it. The blind design carried three flaws
the critic and the answers found after the run (the task's preamble priming the status, the program
numbered inside `task.md`, `shipping`'s comment naming its key), each recorded beneath the README and
the report rather than repaired. A hook refused a listing that named the held-out folder, rightly. Two
other sessions worked on this Mac the whole day, and every outward write, every timed run and every
merge was agreed with them first.

## Author's verdict

*Pending*: https://github.com/heroes-lang/heroes/issues/2227 (the sitting) and
https://github.com/heroes-lang/heroes/issues/2228 (R6's door).

**RATIFIED, 2026-10-26**, R1 to R7 as written above, with step 9's lift of R7's limit on an unended
lease, which the author's instruction of the same day asked for and
https://github.com/heroes-lang/heroes/issues/2227 records: the author answering in conversation
that evening, after the batch's push, meant as *I have read the panel; what it decided is fine by
me*. R4 and R5 stand as written; the spec-warden's W5 and W4, the counts of measurement 052's
sealed turn-1 answers, stay the author's to count, and reopen R4's fallback and R5 only where they
cross the thresholds written there. Recorded as a reading (CLAUDE.md § 4). The author may
overturn it. R6's door is https://github.com/heroes-lang/heroes/issues/2228's own question.
