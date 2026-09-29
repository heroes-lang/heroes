# Panel 176 — a transfer names where the life goes, and a reference joins the life it finds

2026-09-23, M-agreed-retention, after step 3 (`a747e5a2`). **Full panel**, five
seats and a completeness critic. Briefs and reproducers: `docs/panel/176-briefs/`.
Reports: `docs/panel/176-reports/`, the spec-warden's, the historian's and the
llm-ergonomist's written out by the coordinator with the mechanical changes each
file's header names.

**The author's words that bind this sitting**, given in conversation on
2026-09-23 and recorded in English as meant: *proceed, always favouring the most
solid solutions*; and, in answer to the prices of the set grammar `acquires a |
b` — which is this sitting's text, not panel 175's — *pay all the tokens, without
economising*. The spec-warden searched the records for an author's word on price
for this sitting and found only panel 175's; this is where it is written down.
It does not lift design.md §1.6's payment rule, and every row below names what
pays it.

**Procedure.** The blind seat read copies of the specification and its brief
placed OUTSIDE the tree, the amendment panel 175 put to the author; the rule files
that load by path did not reach it, and `CLAUDE.md`, `MEMORY.md` and a git status
snapshot still did, injected by the session. The completeness critic lost its API
connection mid-run and was resumed from its transcript; its report says which
measurements were taken before the interruption and which after, and none was
retaken except one Linux run that matched line for line.

**Two errors in the shared brief, found by seats, corrected here.** The `+14` and
`+29` it attributed to panel 175 were the coordinator's own measurements that day
and are in no file of panel 175 (spec-warden). And panel 175's *json-c freed the
root* is false: json-c 0.19 leaks every object that had a child when it is put,
912 B per object, 158.8 MB over 200 000 rounds against 1.5 MB without the add
(ffi-pragmatist). The reproducers that print `put 1` are leaking programs.

## The question

What a declaration says a C call does with a handle — ends it, and with which
releasers; hands it into another value; drops or adds one reference — and whether
two declarations of one C function must agree (the milestone's second item); one
C function whose contract a call chooses (the third item, `sqlite3_bind_text`);
and defect 078, `owned` on a `const char **` cell stopping the build with
`internal error`.

## The verdict table

| | compiler-engineer | ffi-pragmatist | spec-warden | llm-ergonomist | historian |
|---|---|---|---|---|---|
| **verdict** | approve the set, V1, R1, a call-site rule, Q2's rule, Q4 option 2 · object V2, V3 · no veto | approve V1, R1 (parameter form, *first if not held*), A2, Q4 refusal · object V2, V3 · no veto | object each draft as briefed · approve S2a and the merged C1 · no veto | approve V1 (conditions), R1, A2 if *share* is defined · object V2, V3 · no veto | approve V1 **if the transfer stays checked (V1c)**, R1, A2 · object V2, V3 |
| **section** | §1.7, §1.1, Part 5, §1.12 | §1.11, §4.19, §1.12 | §1.6, §1.2, §1.3, §1.12 | the spec alone | precedent |
| **cost** | +377 −73 selfhost, +70 −17 runtime, ABI 22→23 | no C byte changes in 112 patched units | C1 +182 real merged, +206 appended | — | — |
| **prediction** | one of 510 files changes verdict; `selfhost/` +250 to +450 | a result-only R1 leaves `x509_upref` at 134; `ssl_same_bio` stays 134 | the three `xfer_*` written `consumes` are `check` 1 naming `transfers`; C1 reads 8543 ± 8 | task 1 first-try ≤60% under X, ≥90% under V1 | the first transfer `examples/` marks transfers only on success |
| **condition** | object A if two valid closers are common; object the call-site rule on a correct program it refuses | drop R1's parameter form only if `_up_ref` is reachable another way | approve C1 only if a missing transfer word is a compile error | V1 needs V2's abort clause; A2 needs *share* defined | drop V1c only on a system that leaves its transfer mark unchecked and was not relabelled into silence |

## What the sitting measured, and what changed its mind

**`consumes` must be split, and the split must stay checked.** Every seat
approved a second consuming word for a transfer (V1) over listing transfers inside
every creator's mark (V2: json-c goes from 15 names in marks to 90, one new adder
is 16 edits, libcrypto's marks must name libssl's calls) and over naming the
receiver (V3: it changes only a message, and on a failed transfer that message is
false). **The critic then ran the one shape the approval rested on and it
failed**: the prototype's `transfers` asks no releaser set, so `fclose(stream:
File transfers)` on a `popen` stream is `check` 0 and `run` 0 on three platforms —
defect 075 again, reached through the first repair the new diagnostic's own note
suggests. A real transfer does the same: OpenSSL's `BIO_new_fp` over a `popen`
stream with `BIO_CLOSE` runs at 0 and leaves a child nobody waits for, measured in
C. **V2 catches that one and V1 does not**, which falsifies the compiler-engineer's
*"no program where V2's extra check catches something true"*. The historian's
V1c — the transfer names the releaser it hands the life towards, checked against
the handle's set before C runs, Clang's `ownership_holds` beside
`ownership_takes` — catches both and keeps V1's cost of one name per transfer
function: the critic emulated it by editing one emitted line per program, 0 on
all five correct programs and 134 before C on all three wrong ones.

**A transfer mostly happens only on success.** json-c's five adders, OpenSSL's
`add0`/`push0` family and cJSON leave the life with the caller when they fail;
SQLite's destructor argument transfers even on failure. Every route and the
prototype end the obligation before the call, so a correct failure path is refused
(134) and a leaking one runs silent (0). An emulated spelling that ends the
obligation only when the result equals the success value runs all four json-c
shapes correctly (critic, § 2). design.md has no sentence about it (ffi-pragmatist
grepped §4.19).

**A reference joins the life it finds, and must not replace it.** R1, a producer
mark for an added reference, was approved by every seat that judged it, **on a
parameter as well as a result**: 35 of 120 real reference-adders return a status
or nothing, all 27 AST-visible OpenSSL `_up_ref` among them. The prototype's
`retains` begins a life on an address not held (json-c's documented
borrow-then-`json_object_get` runs), and **replaces the live entry's releaser
set** when the address is held, so a wrong release through a reference runs silent
and the correct program aborts; the critic's 12-line change keeps the live set and
requires the reference to share a name with it, and catches both before C. The
reading that would also refuse a reference taken on a FREED address (`rc_after_release`,
0 plain and a use-after-free under ASan) is the spec-warden's *live or borrowed*,
and nothing in the runtime records a borrowed handle.

**The call-site rule is not local.** The compiler-engineer's route nobody listed,
`unadmitted_release`, refuses at check a consuming call that no mark in the
program names. The critic measured its verdict moving between 1 and 0 when a
declaration neither line mentions is added, moved into an imported module, or is
never called, and a correct module refused when checked alone — defect 085's
shape in a second rule. The llm-ergonomist's *no veto* covered five texts and not
this rule, and no priced sentence states it. The run-time check refuses the same
programs before C runs.

**Question 2's rule holds, and its one "false refusal" was a right one.**
`contract_differs` — two declarations of one C function agree on every position
they share at one type, sets compared as sets — refuses the three `xmod-*`
programs, keeps `twoarity` legal, and fires on 0 of 510 files. The engineer's
counterexample, `realpath` in two modes across two modules, is refused by the
engineer's own test for `permode`: its `owned free` declaration handed a static
buffer is 134 and an ASan *free of an address which was not malloc'd*. No
declaration rule can refuse `xmod-lent` and admit the `sqlite3_bind_text` pair,
because their declarations have the same shape.

**Question 3 needs no new form.** All three modes stay reachable under the rule:
the pointer modes through one declaration with `text` unmarked and a lease, the
function mode in its own module where the types differ; and a TRANSIENT call with a
true `lent` through a header-only shim that fixes the fifth argument, which design.md
§1.11 already sanctions and which runs clean under `--sanitize`. `lent_static.hero`
stays a wrong answer at exit 0 under every route: one module, one declaration,
and the fault is which constant the call passes. That limit is written down
rather than papered over.

**Question 4 is a refusal.** Of 589 parameters that are a pointer to a pointer to
const character data, one header comment says the caller frees, and following it
is a double free (Tesseract, run). The repair is `ffi_owned_const_cell` at exit 1,
14 lines, holding on the typedef spellings (`const gchar **`, `cstring_t *`,
`char const **`) and on real `sqlite3.h`'s `pzTail`.

**`SSL_set_bio(s, b, b)` is a message and a shim, not a word.** A general *one
handle in two consuming positions is one reference* is wrong: `ECDSA_SIG_set0(sig,
r, r)` crashes in C, and the refusal that stops it is the one that refuses
`SSL_set_bio`. The exception is per function, on its own page; a one-line shim
runs clean. What stays wrong is the message calling a correct program a double
release, which is defect 084.

## Found in what ships, each reproduced by the coordinator or the critic

083 an `@` cell holding a pointer accepted against a `void *` parameter
(llm-ergonomist, from the spec alone); 084 `SSL_set_bio(s, b, b)` refused with a
false message (ffi-pragmatist); 085 `unread_releaser` answering 1 alone and 0
inside a program (ffi-pragmatist); 086 a double release before any acquisition
dying at 133 with nothing (compiler-engineer); 087 `heroes grammar`'s *six
contextual words* against eleven (compiler-engineer); and 088 **a handle used after
the call that consumed it is `check` 0 and a use-after-free** (critic), the class
defect 077 belongs to. And the `emission` suite on the trunk is red by the two
goldens lane 076 added without a blessed emission (compiler-engineer) — the
coordinator's fault, from trusting `.claude/rules/verification.md`'s map instead of
rerunning its command.

## The resolution — `provisional — author ratification pending`

1. **The releaser mark takes a set**, `acquires sqlite3_close | sqlite3_close_v2`,
   compared by content, checked before C runs, the newest mark winning at
   re-acquisition — panel 175's route A, landed with this sitting's vocabulary.
   **Defect 075 closes with it.**
2. **A transfer is its own word, and it names where the life goes (V1c)**:
   `val: Json transfers json_object_put`. The runtime checks the named releaser
   against the handle's set before C runs, so a release relabelled as a transfer,
   and a real transfer of a life its receiver would end wrongly, are refused like a
   crossed release. V1 unchecked is refused (it restores 075); V2 and V3 are refused
   for the reasons above.
3. **A transfer that happens only on success says so**, naming the result that
   means success, and the obligation ends only then: a correct failure path runs
   and a leaking one aborts at exit. The spelling is the landing's to measure with
   a reader before it lands — see item 9.
4. **A reference is its own mark, on a result or a parameter**, `retains
   <releasers>`: on a live address it adds one and must share a releaser with the
   life it joins, which it keeps; on an address not held it begins a life. **Defect
   079 closes with it.** The reference taken on a freed address stays admitted,
   and that limit is written down with the reason: no record of a borrowed handle
   exists, and one is not priced.
5. **The call-site rule does not land.** Its verdict depends on declarations
   outside the line, which is the llm-ergonomist's veto condition met by
   measurement; the run-time check covers the same programs before C runs. What
   would change this is a local form of it.
6. **Two declarations of one C function must agree**, on every position they
   share at one type, sets as sets (`contract_differs`). The milestone's second
   item closes with it. The note for two sets says to write their union in both.
7. **`sqlite3_bind_text` gets no new form.** The three modes stay reachable as
   measured; a header shim is the spelling for a true `lent` under TRANSIENT; and
   the wrong answer of a `lent` declaration called with `SQLITE_STATIC` is recorded
   as the limit it is — a mark on one parameter cannot depend on another argument's
   value. The milestone's third item closes with that record.
8. **Defect 078 closes with `ffi_owned_const_cell`**, exit 1.
9. **The spellings of items 2, 3 and 4 are put to a reader before they land.**
   `transfers <releaser>`, the success clause and `retains` were not in the
   ergonomist's variants in these forms; panel 177, convened for defects 077 and
   088, carries the reader test with the same seat, and the spec text is priced on
   the real instrument there, all of it paid for on the author's word and by the
   predictions below.

**What a veto compels.** None was cast. What the ergonomist's condition compels
is item 5.

**What conservative would have been** (CL-040): land V1 unchecked with the
call-site rule, as prototyped, because it passes every suite. **Refused**: the
critic measured it silencing defect 075 on three platforms through the relabel its
own note suggests, and refusing a correct module checked alone.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | `heroes check` over the 510 files at `a747e5a2` changes verdict on exactly one, `abort-handle-borrows-that-gives-away` — **void as stated**, because it rests on the call-site rule, which does not land; `contract_differs` fires on none | the landing of this resolution |
| ffi-pragmatist | a result-only R1 leaves `x509_upref.hero` at 134 on the three POSIX legs; the parameter form takes it to 0; `ssl_same_bio.hero` stays 134 whatever consuming word lands | the landing |
| spec-warden | the three `xfer_*` written `consumes` are refused at check naming the transfer word — **void as stated**, it rests on the call-site rule; the landed text reads within ±8 of its own real price | the landing |
| llm-ergonomist | task 1 first-try correct ≤60% under X and ≥90% under V1 | panel 177's reader test, then M-thesis-harness |
| historian | the first transfer a program in `examples/` or a core package marks transfers only on success | the first milestone whose `examples/` gains one |
| panel 175's three spec predictions | scored by this sitting's ergonomist, task 6: (a) `tag void` for a `void *` to give back — **HELD**; (b) two `tag void` records for two allocator families — **HELD**; (c) no `consumes` on a `ptr` — **HELD**, it wrote a `tag void` handle | scored here |

## Author's verdict

**RATIFIED 2026-09-28**, in one act with every sitting
`docs/work/DECIDE.md` held, panels 175, 176, 177, 179, 180, 181 and 182,
on the author's instruction of that evening, meant as: *ratify every
decision on the list*. **Recorded as a reading of this file**, CLAUDE.md
§ 4's default, which the author asked on 2026-09-21 to be taken for
granted; not `by delegation`. The recommendations the list carried are
taken with it. Each sentence below was verified against the tree at
`a6eab736` before it was written.

**What the yes settles**, all of it landed: the set (item 1, defect 075), the
transfer that names where the life goes and is checked against the set (item
2), the success clause that makes a transfer count only on success (item 3,
spelled `when` on the result by panel 177's item 6), and `retains` (item 4, defect
079), each a sentence of § 13 and a production of its `Member` and `CParam`.
Item 5's call-site rule did not land. Item 6 is `contract_differs` and item 8
is `ffi_owned_const_cell`, with 5 and 2 golden files. Item 7 is the record
`docs/records/done/2026-09-25-1151-one-declaration-cannot-reach-all-three-of-sqlite3-bind-text-modes.md`.
The conservative route, V1 unchecked with the call-site rule, stays refused on
the critic's measurement.

**What it does not settle**: the limit item 4 writes down, a reference taken
on a freed address admitted because no record of a borrowed handle exists, and
the unpriced route that would record one.

**What this section said while the sitting was open**, kept because a
record is not rewritten:

*Pending: `docs/work/DECIDE.md` carries this sitting as `panel 176`. Work
proceeds on the provisional resolution: panel 177 on defects 077 and 088 and on
the reader test of item 9, then the landing.*
