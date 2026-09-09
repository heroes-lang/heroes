# 124 — The remedy was missing, not the refusal

**Full panel**, five seats, 2026-09-09, and **four rounds for the ffi seat**.
Convened on defect **024**, which panel 122 R5 split out of defect 022 by its own
design: the class that milestone was opened to close turned out to have a half no
rule about a Heroes POSITION can reach.

**Every candidate that went out was refused, including the coordinator's.** Three
went to the seats and a fourth was raised mid-sitting when all three had a veto
on them. The resolution is a fifth thing, and it is not a refusal at all.

## The proposal, verbatim

How a Heroes program says that a C function RETAINS a pointer past the call, and
what the compiler then refuses. This is `design.md` §4.19's reserved keyword,
case 2 of three, *"a borrowed pointer you must not touch. Reserve a keyword."*,
of which case 1 is `owned` and landed at panel 109 and case 3, a buffer C takes
ownership of, is still unreserved.

**A, OPT-IN at the declaration.**
```
`kept` after a `cstr` parameter says the C side holds that pointer after the
call returns, so a `.cstr()` lend into it is refused: a lend's bytes are freed
when its own line runs again. Hand it a pointer C owns instead.
```

**B, OPT-OUT at the declaration.**
```
A header does not say how long C keeps a pointer, so a `.cstr()` lend is
refused into every `cstr` parameter but one marked `transient`, which says C
is done with it when it returns. `sqlite3_bind_text` is the case: SQLITE_STATIC
keeps the bytes and SQLITE_TRANSIENT copies them, and both are `const char *`.
```

**C, at the CALL.**
```
A lend is refused at a call whose C side keeps the pointer, and `s.cstr() kept`
at that argument says you know: the compiler stops freeing those bytes and
frees nothing, so the program owns them for as long as C does.
```

**D, the coordinator's, raised in round two** when A, B and C each carried a
veto: delete the early release. Today a lend's owner slot is freed at the
earlier of the enclosing function's exit or its own site re-executing; under D
the site re-executing frees nothing and the bytes live to the frame's exit,
always. No keyword, no annotation, zero of the 60 declarations touched.

## The measurements the briefs carried

**The token cost, measured and not estimated.** From
`POST /v1/messages/count_tokens` on `claude-opus-5`, the pinned model, taken
2026-09-09; content is the raw figure less that model's own probed offset of 6.
The two columns are labelled because the warden caught that 5210 appears twice
meaning different things, and a number like that gets transposed later.

| document | raw | content | delta |
|---|---:|---:|---:|
| `spec/heroes-spec.md` as it stands | 5134 | **5128** | |
| plus candidate A | 5216 | 5210 | **+82** |
| plus candidate B | 5259 | 5253 | **+125** |
| plus candidate C | 5210 | 5204 | **+76** |
| plus the warden's shorter wording of A | 5199 | 5193 | **+65** |

The base matches the record in `selfhost/cli/measure.hero` and
`tests/harness/suite_spec.hero` exactly, which is a check on the record rather
than a coincidence. Against the 6144 ceiling what is judged is 5128 + 60 = 5188
and free is **956**, so A is 8.6% of it, B 13.1%, C 7.9%.

**The annotation surface, with the instrument named, and it was counted three
times.** **60 declarations** take a `cstr` parameter, across **36 files**, and
they carry **77 `cstr` parameters**, of which 6 are `@` out-parameters and **71
are plain in-parameters across 28 distinct C names**. The compiler seat and the
coordinator agree exactly on 60 and 36; the warden's tree-wide figure did not
add up to its own parts and its per-tree numbers for `selfhost/` and `examples/`
matched. The instrument tracks extern-group membership rather than grepping
lines: a naive grep reads **89** because 16 `selfhost/` hits are inside string
literals, in `library_source.hero`, `cli/syntax_cmds.hero`, `emit/ffi_call.hero`,
`print/dump.hero` and `check/lending.hero`. **A migration is priced per
parameter and a reading is done per declaration**, which is why the disagreement
dissolved the moment the unit was named. `.cstr()` call sites: 43 `selfhost/`,
50 `tests/`, 10 `examples/`.

**The brief itself was wrong once**: it said eight of them are the `hero_*` runtime
calls. It is **seven**, plus `getenv` in the same group and `atof` in
`selfhost/emit/literal.hero`. Corrected by the warden and confirmed by re-run.

**What keeps a lend alive**, measured at panel 122 from the emitted C: one
synthetic owner slot per expression SITE, released at function exit or when its
site re-executes, whichever comes first. In a loop the emitted C carries
`t17 = h4_own4; h4_own4 = t10; hero_str_decref(t17);` inside the block, so
iteration *n+1* frees iteration *n*'s bytes.

**The precedent this project set for itself.** `owned` is case 1, spelled as a
word before the freeing function and placed after the type
(`-> cstr owned free`), and `spec/heroes-spec.md:233` states its default in one
sentence: *"Unmarked pointers are never freed."* So case 1's unmarked direction
is safe and its annotation is opt-in, which was the argument for A and against
B, and the sitting had to follow it or overrule it with a reason.

## The verdict table

| seat | verdict | section | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| llm-ergonomist | **veto C**, object A, approve B | locality | B adds 5 words over the four reading functions; A and C add 0 words and C adds 4 undecidable calls | A leaves >=5 of 10 keeper tasks compile-clean and wrong; B leaves **0 of 10** | approves B only if the diff also amends the spec's own `sqlite3_open` example, which B leaves uncallable |
| spec-warden | object, no veto | §1.6, §1.5, §2, §1.12 | +82 / +125 / +76 verified; its own shorter wording measured **+65**, not the <=50 it predicted | 0 marks needed in `selfhost/`, 1 in `examples/`; both reproducers become compile-time refusals | veto above 6084 content tokens, or if 024 lands as an appended sentence duplicating panel 122 R6 |
| ffi-pragmatist | **veto A, veto B**, object C, **veto D**, **approve the held buffer** | §4.19, §4.20, §1.11, §13 | A: `x3` refused and every remedy blocked. B: 47 of 60 marked before the tree compiles. D: 1.44 MB to 81.9 MB. Held: **+1.1% over today** | the ledger ships TRANSIENT at +2 -1 with output byte-identical; `x10h` and `x12h` close | the held buffer must be a COPY and its release WRITTEN, or it becomes a veto |
| compiler-engineer | object A and B, **veto C** | §1.7, Part 5, §1.12 | A prototyped: **+164 lines, 7 files, frontend only**, 607 tests to 609, zero DECIDED breached; timing **+4.3%**, attribution unrun | any A or B without a relay clause leaves a compile-clean use-after-free; `lending.hero` lands above 260 lines | withdraws on A if a clause closes the relay and its cost is measured |
| historian | approve (advisory) | precedent | thirteen sourced rows | a one-value-per-parameter annotation will not bind `sqlite3_bind_text` in a single declaration | three findings named, each a source |

## Where the seats disagreed, unsmoothed

**On B, the ergonomist and the ffi seat are exactly opposed, and both are
right about different things.** The ergonomist wrote the SQLite program correctly
on the first attempt only under B, because the default refusal forced it to look
at which destructor it was passing, and its rule for choosing is that *"omission
is the plausible LLM mistake; commission is not."* The ffi seat vetoed B because
**47 of the 60 declarations must be marked before the tree compiles** and the
compiler seat measured that **0 of the 71 parameters are decidable from a
header**: they are all `const char *`, and `const` promises no write, not no
retention. So B makes the plausible mistake loud and asks 47 authors for a
verdict nobody can honestly give. Both halves are measured and they do not
reconcile.

**On A, the warden and the ffi seat rank it first and last.** The warden ranks A
first for §1.5 (verbose where you declare, terse where you use) and prices C at
103 use sites against 65 declarations. The ffi seat vetoes A because it
**refuses `x3`, the one sound program that scales**, and then leaves no route:
`free` cannot take a `cstr`, `heroes cc` does not exist, and two declarations
hit `declared_twice`.

**On the leak gate, the warden and the ffi seat reach opposite conclusions from
the same file.** The warden reads `selfhost/cli/syntax_cmds.hero:305-320` — where
`heroes fmt` once **silently deleted** `owned`, at a measured 12.8 MB over
200,000 strings — and concludes that a dropped `kept` reopens a use-after-free
at exit 0 while a dropped `transient` yields a refusal, so **§1.12 ranks B
first**; it says so while ranking A first anyway, with the fix named. The ffi
seat's later measurement of D showed the same gate firing six times out of seven
with *"this is a compiler bug"* about programs an author wrote on purpose, which
is the same instrument arguing the other way.

**And the coordinator's own candidate was refused by the seat whose sentence
suggested it.** D came from the ffi seat's closing line, *"for case 2 the
missing information is a DURATION rather than a mark"*, and that seat then
vetoed D on measurement.

## Round two: the sibling argument, and D

**The header is the veto, quoted verbatim** from
`$(xcrun --show-sdk-path)/usr/include/sqlite3.h:4888`: *"The fifth argument to
the BLOB and string binding interfaces **controls or indicates the lifetime of
the object referenced by the third parameter**. These three options exist: (1) A
destructor to dispose of the BLOB or string … (2) SQLITE_STATIC … the
application remains responsible … (3) SQLITE_TRANSIENT … the object is to be
copied prior to the return."*

**SQLite's three options ARE §4.19's three reserved cases**, on the same
parameter of the same declaration, chosen at runtime by argument 5. `man 3
CURLOPT_POSTFIELDS` repeats it inside one `curl_easy_setopt` declaration: *"The
data pointed to is NOT copied by the library: as a consequence, it must be
preserved by the calling application until the associated transfer finishes."*
So a declaration-site mark on such a parameter is **either a false refusal or an
empty comment**, and the ffi seat labelled those two readings as inferences from
the candidates' own rules while what it measured is that `x3` is the only
scalable sound program.

**And value-dependent retention appears to be unprecedented**, sourced as an
absence with the query written down. gi's `(destroy PARAM)` names a sibling
STATICALLY; MSVC's SAL has the conditional mechanism (`_When_`) and no retention
annotation to condition. **What Vala shipped for this exact function is two
declarations for one C symbol**, `bind_text` with `owned string` and
`_bind_text` without, fetched from GNOME/vala master. So a declaration-site
annotation is not disqualified by the fifth argument; it is disqualified only
from binding that function **once**.

**D was refused on two numbers and one impossibility.** `x9.hero`, one lend
site, `getenv` in a loop, `/usr/bin/time -l`:

| | peak RSS | live blocks |
|---|---:|---:|
| today, the SITE decides | **1,425,408** flat across a 10x change in trips | ~2 |
| D, the COMPILER decides | **81,903,616** at 1e6 trips, **805,093,376** at 1e7 | 1,999,998 |

Today is O(1), measured rather than extrapolated; D is O(n), so a `while true`
loop with a lend inside grows without bound. **And D as stated is
unimplementable**: there is one owner slot per SITE, so the frame's exit
releases the last value and the other N-1 are unreachable. Six of seven
programs exited 134 with *"panic: N heap blocks still live at exit … this is a
compiler bug"*. The honest lowering is a per-frame retain list of dynamic
length, which fixes the gate and changes nothing about what is live.

**D did fix the two use-after-frees it was aimed at**, both confirmed by running
them: `x2` (STATIC, bind every column in a loop, then step, the idiomatic shape)
and `x6` (an `@tail` out-parameter that aliases the lend) both became correct.
**But its residue is idiomatic.** `x10.hero`, a per-column bind helper that
builds its own label, is `heroes check` exit 0 and heap-use-after-free under D,
because the helper's frame is gone before the caller steps. **That is the
function `examples/ledger/db/sqlite.hero:305-309` already ships**, one edit away.

**The sentence that killed D, and it is about the coordinator's reasoning rather
than about the rule**: a rule that says *step in the same frame* swaps one
non-local reading for another. *"It has not deleted the comment, it has
re-typeset it."*

## Round three: the route nobody listed

The question was CL-057's, what would have to be true for a route nobody listed
to exist, and it was put to the seat that had measured the inexpressibility.

**That seat corrected itself first, and the correction is the finding.** Its
first report said A's remedy *"leaks by construction"* and was *"A's only
remedy"*. The first clause is true of the route it tried; **the second was an
inference from an incomplete option set**. It had tried `strdup`, which answers
`cstr`, and stopped. It had not tried **`malloc`, which answers `ptr`** — and
`void *` reaches `const char *` in C, so declaring the retaining parameter
`text: ptr` against the real `sqlite3.h` compiles, links and runs.

**Four spellings were then compiled, so the list is a measurement**: `malloc` +
`strcpy` + a `ptr` parameter + `free(pointer: ptr)` **works**; `[u8]` and its
address is `error[type_mismatch]`; an `@` cell of a `u8` is
`error[ffi_parameter_type]`, whose text notes that *"`const char` has no name in
this language yet"*; an `owned` round trip is the wrong direction entirely,
because `owned` copies out of C and frees C's block.

**Both residues close today, on the shipping compiler, with no hand-editing.**

| program | today, lent | held |
|---|---|---|
| `x10h`, the per-column bind helper the ledger ships | check 0, **heap-use-after-free** in `sqlite3_step` | **exit 0**, correct, ASan and UBSan clean, leak gate green |
| `x12h`, `@tail` aliasing across a frame | check 0, **heap-use-after-free** in `puts` | **exit 0**, correct, clean, gate green |

**So defect 024 can close without escape analysis**, which is the question the
coordinator had said no record could be written over.

**And the memory difference is entirely about who decides.** Same program, same
instrument: today 1,425,408 bytes; D 81,903,616; **held and released inside the
loop 1,441,792, which is +1.1% over today**. Holding is O(1) and D is O(n), so
the 57x is not the cost of holding bytes, it is the cost of the compiler
deciding.

**The `ptr` spelling gives up four guarantees and one of them is a segfault
class.** `puts(s: cstr)` with `nullptr` is **exit 134** and *"panic: a null
`cstr` was passed to a C function"*; `puts(s: ptr)` with `nullptr` is exit 0
printing `(null)`; **`atoi(s: ptr)` with `nullptr` is exit 139, SIGSEGV, no
message.** The other three: §4.20's `len+1` NUL-termination becomes the author's
`strcpy`, the leak gate stops counting the block, and the pointer cannot travel
through `T?` (`unsupported[pointer_element]`). §1.12 says a Heroes program must
not segfault, so the route that fixes the use-after-free opens a worse class
unless the buffer is a value the compiler recognises. **That is why it needs the
word**, and the word does not carry the retention decision: argument 5 does.

**A forgotten release is not caught, and the runtime says why in its own
words.** 33,570,816 bytes retained at **exit 0, in silence**, because
`runtime/parts/alloc.c:29` counts *"what the language owns: a `str`, an array, a
map"* and a raw `malloc` reached through an `extern` is invisible by design. The
instrument that would catch it is already written down for exactly this class:
`.claude/rules/c-boundary.md`, *a program that declares an `extern` runs its
Linux leg under `--sanitize`*, because LeakSanitizer exists there and nowhere
else.

**The ledger's repair, both ways, compiled and run against the real example.**

| repair | delta | result |
|---|---|---|
| **TRANSIENT and a lend** | **+2 -1, one file** | exit 0, ASan and UBSan clean, output **byte-identical to `examples/ledger/main.expected`** |
| STATIC and a held buffer | +34 -9, two files | exit 0, clean, identical output, **and the `Failure` message is gone** |

The held repair hit `unsupported[pointer_element]`, so failure became a
`nullptr` the caller must remember to test and the diagnostic
`"sqlite refused text at " + at.to_str()` was deleted. **The held route traded a
diagnostic for a silent sentinel, which is the class this language exists to
kill.** So the ledger ships TRANSIENT, and it deletes the eleven-line comment
with a fact about the **call** that clang checks against the header, rather than
with a fact about the whole program in a shorter sentence.

**And the shipped comment has been subtly wrong the whole time.** It says *"a
caller must step before it drops the string"*. The ledger is actually sound
because every `text:` argument is a **record field the loop variable owns**
(`a.name`, `a.kind`, `e.account`, `e.memo`), so the lend increfs a string with a
live owner. **The comment names the frame; the invariant is the refcount.** That
is why it must be deleted rather than shortened.

## The relay hole, found by prototyping A

The compiler seat built A — **+164 lines, 7 files, frontend only, 607 tests to
609, zero DECIDED ceilings breached** — and found that **A and B both leak the
class**: a Heroes function taking `c: cstr` relays a lend into a `kept`
parameter with **no `.cstr()` at that site**, which is `check` exit 0 and
heap-use-after-free READ of size 14 under `--sanitize`. Panel 122's R2, which
generalised the rule from *an extern call* to *any call*, is what opened it. So
B's 71 annotations buy A's 7 and neither closes the class.

Three routes close it and **none is free**: bringing back the extern
discriminator, which panel 122 dropped on the ergonomist's veto and which 024's
hop is new evidence against dropping; *no `cstr` parameter outside a group*,
which is 4 parameters in 3 golden fixtures **but refuses
`function validated(c: cstr) -> str?` at `selfhost/library_source.hero:178`,
whose own comment says it is the only way in from C text**; or lend-to-kept
dataflow, which stops being frontend-cheap.

## The resolution, provisional — author ratification pending

**The most robust and complete resolution, not the cheapest and not a
compromise** (CLAUDE.md § 4). Where robust and conservative disagree this takes
robust and records the conservative alternative, so the author can choose it.

**R1. A, B, C and D are all refused, and each refusal is a measurement.** A
refuses the only sound scalable program and leaves no expressible remedy; B asks
47 authors for a verdict that 0 of 71 headers can give; C is unsound in its
literal reading (`hero_str_cstr`'s block gets no owner slot at all) and
non-local in every reading; D is O(n) where today is O(1) and its residue is the
function the ledger ships. **No declaration-site mark and no call-site mark
lands.**

**R2. §4.19 gains a FOURTH case, and it is reserved rather than spelled here:
Heroes' bytes with C's reach.** All three cases §4.19 reserved are about a
pointer **C** made. A buffer **Heroes** made, that **Heroes** frees, which C may
read for as long as the program says, is a fourth case that section does not
name. It enters on **robustness, precedence rank 3**, and not on Principle 0:
the warden measured that both branches of §2 are unmet, since `runtime/` holds
zero retained `char *`, all 9 `selfhost/` bindings are transient, the compiler
self-hosts with no marks, and the thesis branch names metric 2, which has never
run. Rank 3 licenses the narrowest thing that closes the class, and R3 to R6 are
that narrowing.

**R3. The buffer is a COPY, never a pin.** If its bytes were the same allocation
as the `str`'s, then `s.cstr()` and the held pointer alias and a copy-on-write
mutation through the `str` rewrites what C is reading. §4.20's *"they may be
shared with other values"* is the exact sentence `ffi_writable_parameter`
already cites, so a pin is a corruption class. The ffi seat will compile the
aliasing case before it signs.

**R4. The release is WRITTEN by the author and checked by the compiler, never
inferred.** An inferred release is escape analysis, which panel 122 already
refused as its candidate 3 and which D's measurement prices again. The mirror
already exists: `owned free` is C's pointer on Heroes' schedule, and this is
Heroes' bytes on C's reach.

**R5. The four cases share ONE variant-valued field, not a field each.** The
compiler seat measured case 2's `Param` field at exactly +6 in `ast.hero` and +8
in `fmt.hero`, against DECIDED ceilings of 475 and 1150 with the file at 472 and
1144 after A. **A third field of the same shape gives 478 and 1152, breaching
both**, and this sitting has just added a fourth case. So the vocabulary is one
field now or it breaches two ceilings later.

**R6. The forgotten release is caught on the Linux leg, and that is named rather
than assumed.** `runtime/parts/alloc.c:29` counts what the language owns, so a
raw buffer is invisible to the leak gate by design and a forgotten release is
exit 0 in silence at 33.6 MB. `.claude/rules/c-boundary.md` already requires a
program declaring an `extern` to run its Linux leg under `--sanitize`, and
LeakSanitizer exists only there. **A word that makes the buffer a value the
compiler recognises should make a forgotten release a diagnostic or a named
exit-time panic that accuses the PROGRAM**, which is the fail-safe direction and
the opposite of D, where the gate accused the compiler six times out of seven.

**R7. Two repairs land now, without the word, and both are measured.**
`examples/ledger/db/sqlite.hero` ships **TRANSIENT at +2 -1 in one file**, with
output byte-identical to `main.expected`, and its eleven-line comment is
**deleted**; the record says why, which is that the comment named the frame while
the invariant is the refcount. And `examples/curl/main.hero` is **not** a defect
and none is filed: curl copies since 7.17.0, verified by a probe that clobbered
and freed the string after setting it, and the argument is a literal with static
storage besides.

**R8. The relay hole is a rider to 024 with its three routes priced, and the
milestone picks with a measurement, not here.** The route that refuses
`validated(c: cstr) -> str?` is refused by that function's own comment, so the
live pair is the extern discriminator, which is new evidence against panel 122's
own drop, and lend-to-kept dataflow, which is not frontend-cheap. **This
sitting does not choose, because neither route has been prototyped and a
resolution over an unrun option set is the thing CL-057 exists to stop.**

**R9. The out-parameter aliasing is the same class and the same defect number.**
`x6` and `x12` are *C, or C's answer, outlives the frame that owns the bytes*,
which is `x10`'s class. One vocabulary covers both and no new number is issued.

**R10. Defect 024 SHRINKS now and CLOSES when the word lands. No record says
closed over `x10`.** What closes now is the ledger's shape, by the sound route,
and what the compiler can see. What no compiler can decide stays named, with the
Linux `--sanitize` leg as its instrument, which is what Go did in 1.6 with a
global rule and a runtime crash before shipping `runtime.Pinner` seven years
later, rather than weakening the rule.

**R11. The milestone is `M-held-bytes`**, naming the fourth case's deliverable in
two words and leaving §4.19's other areas free. Its first step is the spec
wording and its **REAL token cost, which this sitting has not measured**: the
four measured deltas above price A, B, C and the warden's shorter A, and none of
them prices R2. That measurement is owed before the wording is chosen, against
956 free.

**R12. Whatever word lands walks every tool that re-prints a program**, the
formatter first, with its own self-check. `heroes fmt` silently deleted `owned`
once (`selfhost/cli/syntax_cmds.hero:305`), and for this word a dropped mark is
`fmt --in-place` turning a sound program into a leak or a segfault. The
compiler seat prices it at about +12 lines in a 119-line file.

**What conservative would have been, recorded so the author can choose it**
(CL-040): ship nothing but R7, leave 024 open with the ledger repaired and the
comment deleted, and let the reserved word wait for a program that needs it.
That is cheaper, it is defensible under Principle 0 as the warden measured it,
and it leaves `x10` and `x12` accepted at exit 0 with a use-after-free.

## Author's verdict

*Pending.*

## Predictions to score

| seat | prediction | scored at |
|---|---|---|
| llm-ergonomist | on 10 keeper-shaped and 10 reader-shaped FFI tasks: A leaves >=5 of 10 keeper programs compile-clean and wrong, B leaves **0 of 10** with every failure a compile error and reader first-try down 10 to 30 points. Falsified if B yields one compile-clean silently-wrong keeper, or A's silent rate is 0 of 10, or B's reader first-try falls below 50% | M-thesis-harness, since it needs a task set |
| spec-warden | marks needed to hold `./heroes build selfhost/main.hero` at exit 0: **exactly 0** in `selfhost/`, **exactly 1** in `examples/` | M-held-bytes |
| spec-warden | both 024 reproducers move from exit 0 and silently wrong to a compile-time refusal, and `--sanitize` reports zero heap-use-after-free on the corpus | M-held-bytes |
| spec-warden | its own shorter wording measures <=50 content tokens. **ALREADY FALSIFIED at +65**, measured this sitting, which also falsifies its claim that the wording clears §1.6's queued +50 gate | scored here |
| ffi-pragmatist | `examples/ledger/db/sqlite.hero` ships TRANSIENT at +2 -1, comment deleted, `main.expected` unchanged, clean under `--sanitize` on this Mac and the Linux leg, with no shim and no `heroes cc`. **Round three ran the Mac half and it held: +2 -1, byte-identical output, ASan and UBSan clean.** The Linux leg is unrun | M-held-bytes |
| ffi-pragmatist | no declaration-site annotation lands on `sqlite3_bind_text`'s `text` parameter. Falsified by a committed `text: cstr kept` or `text: cstr transient` | M-held-bytes |
| ffi-pragmatist | any implementation extending a lend to the frame's exit gives `x9.hero` at 1e6 trips a peak RSS above 50 MB against 1.44 MB today. Falsifier reachable in exactly one way, named by the seat itself: a flow analysis that keeps the early release where the lend provably does not outlive the iteration, which is the escape analysis panel 122 refused | M-held-bytes |
| ffi-pragmatist | with the held buffer spelled as a reserved word, `x10h` and `x12h` build at exit 0 and are clean on this Mac and the Linux leg, **and a forgotten release is a diagnostic or a named panic accusing the program** rather than exit 0 in silence at 33,570,816 bytes | M-held-bytes |
| compiler-engineer | any A or B shipped without a clause closing the relay accepts `relay(c: cstr)` forwarding a lend at exit 0 with heap-use-after-free under `--sanitize`; and `selfhost/check/lending.hero` lands above 260 lines with `ast.hero` at >=472 | M-held-bytes |
| historian | a one-value-per-parameter annotation will not bind `sqlite3_bind_text` in a single declaration: Heroes will do what Vala did, two declarations for one C symbol, or ship a call-site override on top of a declaration word. Falsifier: one declaration in any shipped language whose vocabulary lets the CALLER choose keep or copy for the same declared function | M-held-bytes |

## Still running when this was written

The compiler seat's round-two answer on D, asked before the ffi seat vetoed D.
Its numbers are corroboration on a refused rule and an independent reading of
the memory question, and they are appended below when they arrive rather than
waited for, because the tree is frozen while this sitting is open and R7's
repair is owed.
