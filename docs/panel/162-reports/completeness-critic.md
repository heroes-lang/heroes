# Panel 162 — completeness critic

**Not a sixth verdict.** This seat names what is missing from the five reports
and six briefs, and it changed the resolution in every sitting it ran through
panel 161.

Every number and every verbatim line below was produced by a command run on
2026-09-18 in a scratchpad COPY of the tree
(`cp -r`, `rm -rf build`, `clang -I runtime seed/heroes.c runtime/runtime.c -o
heroes`), programs under `.../scratchpad/w/`. **Nothing is carried from a brief
or from another seat's report**; where I confirm another seat I re-ran it, and
where I say I could not check something I say it in those words.
`archive/bootstrap-rs/` was never read. The repository working tree was not
modified outside this file.

---

## The convergence this seat was asked to attack

Four seats point at: `field.validated()` on a `char[N]` extern field yielding
`str?`, reading **to its first zero or whole**, failing `not_text`; plus
widening `repeat(x, n)` to build a fixed array; merged into § 13 at +45 and +18
real against +99 for the § 11 form, with a −6 named removal.

**It does not carry the sitting, and the reason is countable.** The seat holding
the ABI veto listed **six** conditions for moving from `object` to `approve`.
The convergent resolution meets **three**. Its own prediction 3 then says, in
its own words, *"my verdict stands unchanged."* A synthesis that adopts this
resolution and reports convergence is reporting a majority over an unmoved
objector, and the objector is the seat whose brief was *write the C, bind it,
run it*.

---

## Contradictions

### C1. The ffi-pragmatist's six conditions: three met, three not, and nobody counted

`ffi-pragmatist.md` § 9, enumerated from that file rather than summarised:

| # | condition | met by the convergent resolution? |
|---|---|---|
| 1 | field stays at the header's width so the `_Static_assert` still fires | **met** |
| 2 | **lets the caller say where the bytes stop and does not require a terminator** | **NOT met** — *to its first zero or whole* is a terminator rule with a fallback |
| 3 | **accepts `u8[N]` as well as `i8[N]`** | **NOT stated** — the adopted wording says *a fixed byte field* and names no element type |
| 4 | **makes the field passable to a `cstr`/`ptr` parameter**, so a binding can still call `strlen`, `inet_ntop`, `memcmp` | **NOT met** — nothing in the resolution touches argument passing |
| 5 | answers `str?` on invalid UTF-8, consistent with `read_file` | **met** |
| 6 | ships some build route | **met** (`repeat`) |

Condition 2 is the one with the population behind it: that seat measured **13 of
50** `char[N]` fields across 16 headers as reliably NUL-terminated, and 2
(`d_namlen`/`d_name`, `sun_len`/`sun_path`) carrying their length in a sibling
field. *To its first zero or whole* cannot take `d_namlen`.

**And I correct the ffi-pragmatist against the adopted wording, because it
priced a different rule.** Its prediction 1 says a terminator-only route
over-reads `utmpx.ut_id[4]` into `ut_line`. Against *to its first zero **or
whole*** it does not: the fallback stops at the type's own length, which is in
the type, which is the header's. **The *or whole* clause is a real guard** and
the seat's sharpest prediction is falsified by a wording written after it. That
makes condition 2 **weaker, not satisfied**: the resolution is safe on
`ut_id[4]` and still cannot express the length the header already wrote down
for `d_name`, and a program that wants `d_namlen` bytes gets `d_namlen` bytes
plus whatever follows to the first zero.

The other half of that trade nobody has looked at: *or whole* means reading `N`
bytes where `N` is `sizeof` the field, and the same seat's § 5 names the shape
where C allocates **less** than that — `dirent.d_name` last, bounded by
`d_reclen`. For a well-formed record the zero arrives first and the fallback
never fires. **Whether the fallback is reachable on a short record is a §1.12
question and it is unasked.** I could not construct it (see § What I could not
check): `readdir` returns `struct dirent *`, which Heroes binds as a handle,
and a handle has no fields to read.

Condition 4 is the one nobody argued at all. That seat's § 1 row 4 is the
sharpest measurement in the sitting — `strlen(u.sysname)` is
`error[type_mismatch]: expected cstr, found i8[256]`, so the field is not merely
unreadable by Heroes, **it is unpassable to C** — and not one of the other four
reports mentions it. The convergent resolution leaves it exactly where it is.

**What the synthesis owes**: either a second form that takes an extent, or the
sentence *this resolution does not discharge conditions 2, 3 and 4 of the
ffi-pragmatist, whose verdict therefore remains `object`*, in those words.

### C2. Rung 3 of § 4.19's ladder: I ran it, and it is already expressible — which removes it as evidence for the resolution, not for it

The ffi-pragmatist predicts rung 3 (`sqlite3_column_text` + `sqlite3_column_bytes`)
is *"the same shape as `d_namlen`"* and that a route which cannot express the
pair *"has failed the ladder it was written for."* **Measured, it is not the
same shape.** `sqlite3_column_text` returns `const unsigned char *` — a pointer,
not a `char[N]`:

```
extern "sqlite3.h"
    record Stmt tag sqlite3_stmt
    function sqlite3_column_text(statement: Stmt, column: i32) -> cstr
    function sqlite3_column_bytes(statement: Stmt, column: i32) -> i32
```

`./heroes build p9_sq.hero -o ./p9bin` → **`wrote ./p9bin`**. clang accepted
`cstr` against `const unsigned char *`. So rung 3's text column is readable
**today**, with `c.validated()` and no C shim and nothing this sitting adds.

Two consequences, and they cut opposite ways:

- **The sitting cannot claim rung 3 as a win.** It was never blocked by the
  `char[N]` wall.
- **The ffi-pragmatist's prediction 2 is unscoreable as written** and should be
  restated: what rung 3 actually needs from an extent-taking form is a TEXT
  column containing an embedded NUL, where `column_bytes` is the only correct
  length. I did not run a SQLite query with an embedded NUL in a text column —
  see § What I could not check.
- The `const`-refusal precedent the ffi-pragmatist carried from panel 161
  (*"refused at every spelling"*, and its own report says *"Not re-measured
  here"*) is true of a `const char[N]` **field** and **false of a `const
  unsigned char *` return**, measured above. A sitting that carries that
  sentence one type further will refuse a binding that builds.

### C3. The spec-warden's falsification of premise (b) does not survive `build`, and the seat that measured why is in the same sitting

`spec-warden.md` § 2(b) reports *"A `char *` field is ALREADY readable"* from
this program, at **`heroes check`, exit 0**:

```
extern "pwd.h"
    record Passwd tag passwd partial
        pw_name: cstr
    function getpwuid(uid: u32) -> Passwd
```

I ran it past `check`. Verbatim:

```
error[ffi_return_type]: `getpwuid` does not return `Passwd` — that is what
`pwd.h` says, and clang read it
```

`getpwuid` returns `struct passwd *`. **The program the spec-warden rested a
premise-falsification on does not build.** And the seat sitting two chairs away
measured exactly this hazard: `ffi-pragmatist.md` § 2, *"FFI verification happens
at `build`, not at `check`. All four refusals above passed `heroes check`
silently."* One seat published the warning and another published a `check`-only
FFI measurement in the same sitting, and no document connects them. That is
CL-062 — *reading is not measuring; finish the command* — inside a panel whose
shared brief opens by invoking CL-077.

**The claim is nonetheless TRUE and I proved it with a program that builds.**
Own header, group record, `cstr` field, by value, end to end:

```
extern "own.h"
    record Thing tag thing
        name: cstr
        tag4: i8[4]
    function make_thing() -> Thing
...    print(t.name.validated().must())
```
`./heroes run p7_own.hero --include .` → **`hello`**.

So: shared-brief premise (b) is false, the compiler-engineer's independent route
to the same conclusion (`selfhost/check/lending.hero:200`, the in-group test at
`:282`) stands, and `spec/heroes-spec.md:349-350` says it in the spec's own
words — *"A field is a number, `bool`, `ptr`, `cstr`, another record of the
group, or a fixed array of one"*. **The finding survives; the evidence offered
for it does not.** A synthesis that cites the `getpwuid` program is citing
something a reader can refute in one command.

### C4. The `mutation instrument` claim is false, it contradicts design.md's own list, and it is what chose the payment

`spec-warden.md` § 4: *"no first-try-rate instrument and no mutation instrument
exist"*, and on that basis *"Every prediction in the llm-ergonomist's report
names an instrument that does not exist."* That sentence is why its § 5 pays
with a named removal rather than a registered prediction.

**The claim splits, and only half of it is true.**

*The first-try half is true.* Metric 2 is the first-try rate
(`docs/design/design.md:3784`, *"Spec → model → programs → count"*), it has never
run, and `docs/ROADMAP.md:146` still carries row 68 **M-thesis-harness**,
`scheduled`, *"Part 11's metrics 2 and 4 run for the first time"* — note metrics
2 **and 4**, not 3.

*The mutation half is false.* `heroes mutate` is a subcommand of the one binary:

```
$ ./heroes mutate --help
error: `mutate` does not accept `--help` — it accepts --survivors, --operator
```

It has nine lines in `tests/harness/suite_surface.hero` (counted), including
`mutate examples --operator typo-digit` described in that file as *"the gate
every leg runs"*. **And it is the instrument of metric 3, the silent-error
rate**, `design.md:3789-3791`: *"Mutation operators as data … per-operator kill
rate. Deterministic, free, no API needed."* It has run at least four times —
`docs/measurements/001-metric-3.md` and `002-metric-3.md` (2026-08-04),
measurement 007 (2026-08-13) and `014-mutate-over-thirty-five.md` (2026-09-03).

**And design.md names it in the payment rule itself.** `design.md:319-321`,
read this session, on what makes a registered prediction admissible *as payment*:

> …**and that instrument exists on the day of registration**: metric 3,
> `heroes mutate`, `heroes measure`, a line count, a compile, a diagnostic
> transcript.

So `heroes mutate` and metric 3 are **two of the six instruments design.md lists
by name as existing**, and the seat carrying Principle 0's burden of proof ruled
out the registered-prediction payment by asserting one of them absent. **A
payment route was available and was closed on a false premise.** Concretely: a
prediction of the form *after this resolution, `heroes mutate`'s kill rate over
a corpus program that reads a `char[N]` field is N%* names metric 3, is
deterministic, is free, and pays under `design.md:318`.

Two further things the same sentence loses. `design.md:322-323` says *"A
prediction naming an instrument nobody has built is still registered as an
observation; it simply pays nothing"* — so the ergonomist's metric-2 predictions
are **registered, not void**. And the procedure for them already exists and has
a precedent: `docs/records/journal/040-cstr-lifetime.md:120` handles panel 122's
identical case — *"UNSCOREABLE HERE, metric 2 has never run; it lapses to
M-thesis-harness rather than being renewed"*, under `design.md:331`'s *"An
outstanding prediction is re-decided, never renewed."* **The spec-warden neither
registered nor lapsed them.** § Q2 below says which of the seven survive.

### C5. `repeat` is a § 11 name and the resolution writes its new meaning into § 13

`spec/heroes-spec.md:293` carries `repeat(s, n)` inside § 11's `Built-ins:`
sentence, and `selfhost/inventory.hero:74` has it at `tier: .runtime` with
`labelled_of("repeat").is_err()` (`inventory.hero:198`) — positional, two
arguments. The spec-warden's adopted build-half wording puts *"or `repeat(x, n)`
for n of the same"* into **§ 13**.

`.claude/rules/spec-shape.md`'s rule, as the spec-warden itself quotes it, is
that a rule lives in the section of the operation it governs — **one home**.
After this amendment `repeat`'s signature is described in § 11 (`repeat(s, n)`,
a string builder, per § 10's prose at `:283`) and its widened domain in § 13.
Two homes for one built-in. The seat's own § 8 condition says that admitting
`[u8]` moves the home to § 11 and the price to +99; **widening a § 11 built-in
is the same move and was not priced that way.** I did not re-price it — see
§ What I could not check — but the sitting cannot adopt +18 without answering
whether § 11's `repeat(s, n)` spelling must change too.

### C6. The historian's axis split is accepted in the argument and dropped in the wording

The historian's single strongest contribution is that every precedent answers
**two** questions separately — *termination* (where does the run stop) and
*validity* (is it text) — and that three languages answer them differently.
Ada raises `Terminator_Error` on a missing terminator and has no validity
concept; Go and Zig truncate silently and never validate; Rust errors on
validity and makes the caller slice.

The adopted wording — *"to its first zero or whole; … bytes that are not UTF-8
`not_text`"* — collapses both onto one outcome shape and gives **termination no
failure mode at all**. A `sa_data[14]` with no zero and a `sysname[256]` with
one produce the same kind of answer, and the program cannot tell which happened.
Ada's 1995 answer distinguishes them with a `Trim_Nul` flag and an exception;
the resolution distinguishes them with nothing. The historian said this in
§ Misstatements and the four convergent seats did not carry it into the text.

---

## Unmeasured claims

**U1. The convergent resolution's own token price was measured on a wording that
does not exist yet.** The spec-warden priced *"read half merged, tight
wording"* at +45 and the build half at +18, and reports both halves plus the
removal at **8041**. The two halves were priced **separately** and the
combination is reported as +57 with the −6 applied. That arithmetic
(45 + 18 − 6 = 57) holds, and the seat's own table also lists *"both halves,
merged, unpaid | 8047 | +63"* — which is 45 + 18 = 63. Consistent. **What is
unmeasured is the resolution the synthesis will actually write**, which must
also answer the sign (§ C7 below), the `u8[N]` arm, and whatever it does about
conditions 2 and 4. Every one of those is text. The spec-warden's own prediction
1 sets the tripwire at **≤ 8060**, i.e. **19 real tokens** of room for all of
it. I did not price the missing sentences.

**U2. "Zero programs in `examples/` declare a fixed byte array" is used as a
Principle 0 finding and is a statement about today's corpus, not about need.**
The shared brief runs `grep -rln "i8\[\|u8\[" examples --include='*.hero'` and
closes the compiler-need branch on the empty result. I re-ran it and it is
empty. But the milestone that motivates the sitting, M-core-packages, is quoted
in `docs/work/milestones/M-readable-bytes.md` as declaring *"roughly eighteen C
headers"* — so the corpus is empty **because the wall is there**, and citing its
emptiness as evidence that nothing wants the feature is the survivorship
argument. Nobody named that. It does not change Principle 0's arithmetic (the
thesis branch still has to carry it) but it should not be repeated as if the
corpus were a free vote.

**U3. The llm-ergonomist's hesitation #1 was never resolved for it, and it is
the hinge of its own report.** That seat wrote: *"`u.sysname[i]` — may a fixed
array be indexed at all? … This single fact decides how bad route 4 is,"* and
made it an explicit condition: *"If the harness shows A.6 does NOT compile …
route 4 stops being dangerous."* **Measured, A.6 compiles and runs.** My
program, five fields at 256 elements each:

```
len=6 joined=6897114119105110
```

`68 97 114 119 105 110` is `Darwin`. `heroes run`, exit 0, no diagnostic. So the
ergonomist's condition resolves **against** route 4 and its objection stands at
full strength — and the synthesis should record that the condition was
discharged by measurement rather than leaving a live `if` in an adopted report.
This is also the spec-warden's § 2(a) confirmed independently; it is the one
premise in this sitting that two seats asserted and that I could fully reproduce.

**U4. The compiler-engineer's line-count estimate was never bounded on the
build half it prices at zero emitter lines.** `~15-30` checker lines for a zero
default is priced, then the same report's § 8 concludes a zero default is the
right route — while the spec-warden measures that a zero default **contradicts
design.md §4.9's *no default values*** and is therefore *"an amendment to §4.9
and a separate bill."* Two seats, two incompatible build halves, both marked
approve-able, and the convergent resolution silently takes the spec-warden's
(`repeat`). **The compiler-engineer's §8 was not told it lost**, and its
prediction 2 ("under 150 lines across … `selfhost/check/walk.hero`") was written
for the other route. Re-scoring is owed.

**U5. "`validated` is written in Heroes, not a built-in" is cited as making the
change cheap, and the read half needs the opposite.** The spec-warden cites
`selfhost/library_source.hero:192` for this. The compiler-engineer's § 7
measured, on the same tree, that a **library** function cannot reach a fixed
field: `strlen(s: u.sysname)` is `type_mismatch`, `strlen(s: @u.sysname)` is
`not_mutable`, because `@` means *mutable out-parameter*, not *address-of*. So
widening `validated` to a fixed field cannot be done where `validated` currently
lives. **One seat's cheapness argument rests on a file the other seat measured
cannot host the feature**, and the synthesis has both sentences and neither
contradiction.

---

## Routes nobody listed

**R1. Return the EXTENT, not the text.** `u.sysname.len_to_zero() -> i64`, then
hand the pair to a conversion. Nobody priced it, and it is the only route that
splits the historian's two axes: termination becomes a value the program can
inspect, print, compare against `d_namlen`, or ignore; validity stays in the
existing `str?`. It is also the only route under which `d_namlen` and
`ut_id`'s full-four-bytes case are expressible by the *program* rather than by
the *rule*. Not priced here.

**R2. `slice` already exists and was never tried on the field.** I ran it:

```
error[bad_operand]: `slice` takes `str` or `[T]`, found `i8[256]`
```

`slice(from:, to:)` is in § 11's `Built-ins:` sentence (`spec:292`), in the
inventory at `tier: .runtime` (`inventory.hero:75`), and carries labelled
params `from`/`to` sharing `i64` (`inventory.hero:143`). **The route the
ffi-pragmatist's condition 2 asks for is one existing built-in's domain away**,
and the shared brief's route 3 was priced by the compiler-engineer as *"a new
type"* and vetoed on that basis. Widening `slice` adds **no type** — it answers
what `slice` already answers. The compiler-engineer's veto is against a slice
**type**, and its report says so in those words; **no seat priced a slice
OPERATION over a fixed field.** This is the largest unexamined option in the
sitting.

**R3. The pair, which is what four real headers actually hand you.**
`u.sysname.validated()` for the terminated case **and**
`field.validated(to: n)` — or `field.slice(from:, to:).validated()` under R2 —
for the sibling-length case. The ffi-pragmatist's condition 2 and the historian's
Ada precedent (`To_Ada (Item, Target, Count, Trim_Nul)`, caller-supplied extent
*and* an explicit termination flag, standardised 1995) both describe this shape.
The sitting treated *terminator* and *extent* as competing routes; every
precedent the historian fetched ships both.

**R4. Make the field passable, and the read half may not be needed at all.** If
a `char[N]` field decayed to `cstr` as an argument — C's own rule, and the thing
condition 4 asks for — then `strlen`, `inet_ntop`, `memcmp` and
`sqlite3_bind_text` all work, and `validated()` on the result is the door that
already exists. Nobody costed it; the ffi-pragmatist's § 6 veto is against
changing the field's **declaration**, and argument decay changes no declaration
and no `sizeof`. I did not measure whether the emitter can spell `&r.field`
where a `cstr` parameter is expected — see § What I could not check.

**R5. A build half that is neither a default nor `repeat`.** The historian
fetched four languages that let you omit a large array field, and one of them is
Ada's `(others => nul)` — an **explicit** aggregate whose every element is
named-by-rule rather than defaulted. That is not a default value and does not
touch §4.9. No seat considered an `others`-shaped literal; both candidate build
halves were framed as default-vs-repeat.

---

## The question not asked

**Q1. What is the SIGN of an inbound byte, and does `validated()` on an `i8[N]`
field contradict § 10?**

The compiler-engineer names it and calls it *"the highest-value thing I
return"*; **no route in the sitting answers it**, and its own prediction 3
forecasts that the resolution will not state it. Re-measured here, both halves:

- `spec/heroes-spec.md:270` — *"`s[i]` yields a `u8`"*, read this session.
- `u8[256]` against `char sysname[256]`:
  ``error[ffi_field_type]: `Utsname.sysname` is not `u8[256]` in
  `sys/utsname.h` — clang read the header's struct and the field disagrees``

So outbound a byte is `u8` and inbound from a plain `char[N]` it **must** be
`i8[N]`. The adopted wording says *"a fixed byte field"* and the language has
two of them, which the compiler already refuses to confuse (the ffi-pragmatist
measured `u8[128]` against `unsigned char fh_data[128]` building clean).

Does `validated()` on `i8[N]` *contradict* § 10? **No — and that is the answer
the synthesis should write down rather than leave open.** § 10's `u8` is about
indexing a `str`, an outbound operation on a language-owned value; the inbound
field's element type is the header's and § 13 already says so twice (*"declared
at the header's own width and sign"*). The two sentences are about different
values. **But the resolution must state the sign anyway**, for a reason that is
not about contradiction: `validated()` must accept **both** `i8[N]` and
`u8[N]` or half the measured population is out — and `spec/heroes-spec.md:349`
lists a group field's fixed-array form as `i32[4]`, which names neither.
`M-arm-platform` closed hours before this sitting on exactly this question in a
different place (`git log --oneline -3`: *"C's plain `char` is signed on every
leg"*). A sitting that does not say `i8[N]` and `u8[N]` in the same sentence is
re-opening the milestone above it in the chain.

**Q2. Which of the ergonomist's seven predictions are scoreable?** The
spec-warden retired all seven on a false premise (§ C4). Enumerated from the
seven, against the instruments `design.md:319-321` names as existing and that I
measured present (`heroes mutate` / metric 3, `heroes measure`, a line count,
**a compile**, a diagnostic transcript):

| # | the prediction | scoreable today? |
|---|---|---|
| 1 | first-try-correct ≈ 0% under today's spec | **no** — metric 2 |
| 2 | >half of first tries hallucinate a name | **no** — metric 2 |
| 3 | A.6 compiles and is a silent wrong program, 10–30% of attempts | **the compile half: YES — *a compile* is a listed instrument and I ran it** (§ U3). The rate: no |
| 4 | ≥70% correct under `validated()` | **no** — metric 2 |
| 5 | ≥20% write `u.sysname[0].to_str()` and get `"72"` | the rate: no. **The behaviour: YES**, it is `p1_run.hero`'s output |
| 6 | ≥30% put `?` in `main` | **no** — metric 2 |
| 7 | ≥80% fail to construct the `Utsname` | **no** — metric 2 |

So **two of seven have a checkable half today**, and #3's compile half is
already resolved — against route 4. The other five are metric-2 predictions:
by `design.md:322` they are **registered as observations that pay nothing**, and
by the panel-122 precedent at `journal/040-cstr-lifetime.md:120` they **lapse to
M-thesis-harness**, `docs/ROADMAP.md` row 68. That is a disposition, not a
deletion, and the synthesis owes it in writing.

**And the payment the sitting did not look for is a metric-3 one.** The
ergonomist's central claim is a *silent-error* claim, and the silent-error rate
is metric 3, which exists, is free and has run four times (§ C4). CL-057 applies
to the sentence that closed this off: the list of instruments is itself a
measurement, and the seat that made it enumerated neither the tool surface nor
design.md's own list.

**Q3. Nobody asked what `partial` does to construction, and the language has no
written answer.** The llm-ergonomist's Task D.5 stopped there: § 13 requires
`partial` when a group record names some fields, § 9 says construction names all
of them, and *"the document does not say how a `partial` record is
constructed."* Its hesitation #7 grades a wrong guess **silent memory garbage**.
The convergent resolution's build half (`repeat`) does not answer it: it lets
you write 256 zeros in four characters and says nothing about the 251 bytes of a
`partial` record that were never named. **This is inside the milestone's
deliverable** — the brief's own program uses `…` for four of five fields — and
no seat priced it.

**Q4. Nobody asked whether `check` should refuse the FFI shapes that `build`
refuses.** Three separate reports contain a program that passes `check` at exit
0 and dies at `build` (ffi-pragmatist § 2, four of them; spec-warden § 2(b), one,
unnoticed; my § C3). That is a diagnostic-class question and therefore panel
territory by CLAUDE.md § 4. It is out of this sitting's scope and it should be
queued rather than lost.

---

## Admissibility

- **The llm-ergonomist declares contamination** (§ 0): four files arrived
  unbidden, including `.claude/rules/spec-shape.md` with two token counts for
  the document it was judging. It says it used none of it. **Its brief's
  isolation rule was broken by the environment, not by the seat**, and the
  synthesis must record that its report is admissible-with-notice rather than
  silently clean. The specific hazard is that `spec-shape.md` carries the
  section map, and that seat's value is that it navigated § 3, § 10, § 11, § 13
  by reading. I cannot verify non-use; I can only record the declaration, which
  is the correct thing to have done.
- **The historian's report is written by the coordinator** ("this seat has no
  write tool", line 3). Every claim carries a fetched URL and three are marked
  `unverified` in those words, which is the standard its brief sets. Admissible.
  One item to carry: it marks the C11 paragraph number unverified and the rule
  verified via cppreference, correctly.
- **The spec-warden's § 2(b) is inadmissible as measurement** (§ C3) and its
  conclusion is independently sound. **Its § 4 instrument enumeration is
  inadmissible** (§ C4): it is a negative claim about the tool surface, CL-018's
  own shape, and it does not name what was searched for.
- **The ffi-pragmatist's report contains a correction it made against itself**
  (§ 3, the withdrawn corruption claim, re-run and refuted by its own program).
  That is the standard CLAUDE.md § RUN IT asks for and it should be named in the
  synthesis as such, not buried.
- **The compiler-engineer's § 5 `ffi_incomplete_record` finding and § 7
  library-route refutation are the two measurements in this sitting that most
  change what is being decided**, and both are in the report of the seat that
  votes `object`.
- **`docs/work/DEFECTS.md` reads `**OPEN: 0**`, measured.** Both defects found
  beside this question are therefore **unfiled** — see below.

---

## The two defects, and where they go

**Both are out of scope for the resolution and must be filed today**, because
`DEFECTS.md` is at `OPEN: 0` and a milestone tags only over a clean list
(CLAUDE.md § Verification), so an unfiled defect is one that disappears at the
close rather than one that blocks it.

1. **`ffi_incomplete_record` names a field that IS present. Reproduced here,
   independently.** A `Utsname` naming **all five** fields, with `sysname`
   spelled `cstr`:

   ```
   error[ffi_incomplete_record]: `Utsname` does not name `nodename`, and
   `sys/utsname.h` says the struct has it — a group's `record` IS the header's
   struct, so a field left out is a field C fills with zero
     at p11_inc.hero:2:12
   ```

   `nodename: i8[256]` is on the line directly below `sysname`. The note then
   compounds it: *"a `Utsname` built here would carry a zero `nodename` into
   C"*. Mechanism per compiler-engineer § 5,
   `selfhost/emit/ffi_record.hero:38-60`: clang's
   `-Wmissing-field-initializers` wording is forwarded verbatim, and a pointer
   initialiser against an array member makes clang say *missing field*. The
   diagnostic blames the program for an omission it did not make, **in the exact
   place a reader tries route 2 first**, and its `Fix` would be wrong.
   Diagnostics are a deliverable (CLAUDE.md § 8). **File separately.**
2. **An FFI program with a struct out-parameter has no writable initial value.**
   llm-ergonomist Task D, corroborated by the ffi-pragmatist's § 3 table (one
   build route compiles, the fieldless handle, and it is a dead end) and by its
   § 3 finding that `uname(NULL)` **exits 0 reporting a success it did not
   have**. The `repeat` half of the convergent resolution closes the writable
   value; it does **not** close the handle route that compiles and lies.
   **In scope for the build half is `repeat`; the silent-success handle is a
   separate defect.** The ffi-pragmatist says so itself: *"It should still be
   filed."*

A third, from this seat: **the `check`/`build` gap of § Q4** is either a defect
or a panel item, and it is currently neither.

---

## What I could not check

Said in those words, as CLAUDE.md § RUN IT requires.

- **I could not check the price of the resolution the synthesis will actually
  write.** I priced no spec draft. The +45/+18/−6/8041 figures are the
  spec-warden's, taken on the real instrument in its session, and I neither
  reproduced nor re-ran `measure --refresh`. Everything in § U1 about the
  remaining 19 tokens of headroom to that seat's own tripwire is arithmetic over
  its numbers, not a measurement of mine.
- **I could not check whether widening `slice` (R2) or a decay-to-`cstr`
  argument rule (R4) is implementable**, in lines or in emitter spellings. I
  measured only that both are refused today and that neither adds a type. No
  seat priced either and neither did I.
- **I could not check `sqlite3_column_text` on a text column containing an
  embedded NUL**, which is the only case where `column_bytes` is load-bearing.
  I built the binding; I ran no query.
- I **did** reproduce `ffi_incomplete_record` (above), but **I could not check
  its mechanism**: I did not read `selfhost/emit/ffi_record.hero` and I take the
  `-Wmissing-field-initializers` explanation from the compiler-engineer's report.
- **I could not check `d_namlen` end to end.** I built a `dirent` binding
  carrying both `d_namlen: u16` and `d_name: i8[1024]` (`wrote ./p10bin`), which
  establishes that the sibling-length shape is **bindable**; I did not open a
  directory and read a name. **Nor could I construct the short-record case** of
  § C1 — whether *or whole* can read past a C allocation shorter than `sizeof`
  the struct. `readdir` returns `struct dirent *`, a handle, and a handle has no
  readable fields, so I have no program that reaches it. **That is a failure to
  construct, not a finding that it is unreachable**, and it is the shape
  CLAUDE.md § RUN IT calls a question rather than a premise.
- **I could not check whether the llm-ergonomist actually used the contaminating
  files.** Only its declaration is on the record.
- **I could not check the ffi-pragmatist's 50-field, 16-header classification.**
  I re-ran none of the clang AST walk. The counts in § C1 are that seat's,
  attributed, and my argument in § C1 does not depend on the exact split — only
  on the two sibling-length rows, which I read out of the headers directly
  (`sys/dirent.h:106-108`, `sys/un.h:77-79`, quoted in that report).
- **I could not check on Linux or Windows.** Everything above is this Mac,
  Darwin 25.6.0, arm64. `char` signedness is a platform fact
  (`.claude/rules/platforms.md`), and the `i8[N]`-vs-`u8[N]` question of § Q1 is
  precisely a platform question that `M-arm-platform` closed hours ago for a
  different case.

---

## What this seat asks the synthesis to change

1. **Say that the ffi-pragmatist's verdict is unmoved**, and which three of its
   six conditions the resolution does not meet — or meet them.
2. **State the sign**: `i8[N]` **and** `u8[N]`, in the same sentence, and say
   that § 10's `u8` is about a `str` and not contradicted.
3. **Price and decide R2** — widening the existing `slice(from:, to:)` over a
   fixed field — before adopting a terminator rule. It is the cheapest form of
   condition 2, it adds no type, so it is outside the compiler-engineer's veto,
   and no seat looked at it.
4. **Correct the record on the two false claims**: the `getpwuid` program does
   not build, and `heroes mutate` exists — `design.md:319-321` lists it and
   metric 3 among the six instruments a prediction may be paid with.
   **Re-open the payment**: a metric-3 prediction is available, free and
   deterministic, and the −6 removal was chosen because it was believed not to
   be. Dispose of the ergonomist's five metric-2 predictions as
   `design.md:322` and `journal/040-cstr-lifetime.md:120` require — registered
   as observations, lapsing to M-thesis-harness — rather than dropping them.
5. **Record that the llm-ergonomist's own condition was discharged by
   measurement** — A.6 compiles and prints `6897114119105110` — so its
   objection to route 4 stands at full strength and route 4 is closed on
   measurement rather than on preference.
6. **File the two defects and the `check`/`build` gap** before the milestone
   tags over `OPEN: 0`.
