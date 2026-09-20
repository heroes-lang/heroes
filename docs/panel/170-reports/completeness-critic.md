# Panel 170 — completeness critic

**No verdict. This seat names what is missing.** Run BEFORE the synthesis, which
is what `/panel` § 3b asks and what panel 169 did not do.

**Every number below was produced by a command run in this session, 2026-09-20,
Darwin 25.6.0 arm64**, in a copy of the tree taken with `git archive` at
`65e10d28` (step 16 — verified with `git rev-parse` in the copy before anything
was built), compiler `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`,
**3.75 s** real. Three further compilers were built the same way from
`7267db07` (step 10), `79688a9c` (step 11) and `ecd948c0` (step 12); why, is § 4.
Nothing was rebuilt from `selfhost/`. The main tree carries only this file.

---

## 0. The shortest statement of what is missing

**Three of the four candidates on the table close zero of the four open
defects, measured, and the fourth closes two of four at a cost nobody has
priced. Not one candidate touches defect 070 as it is filed, and not one
touches 072.** The two seats who say 072 is landable describe two different
mechanisms and one of them does not survive `build`. And the sitting is one
defect short: a fifth, reachable from the shipped grammar, that ends in
`internal error`, reproduced below.

---

## 1. Is there a resolution here at all? What each candidate actually closes

### 1.1 The four defects, re-reproduced from scratch in this session

Not taken from any report. My own files, my own runs.

| defect | shape I built | check | build | run | stderr | `--sanitize` |
|---|---|---|---|---|---|---|
| **066** | group `record Sl2 tag Sl2` with `name: u8[8]`, `keep(p: s.name.ptr(), n: 8)` in a function that returns, `later()` read from `main` | **0** | **0** | **0**, prints `112` from the dead frame | **0 B** | run 0, **0 ASan lines** |
| **070 as filed** | `eat(s: cstr)` over `static inline void eat(const char*){free(…);}`, argument a `.lease()`, `end_lease` after | **0** | **0** | ten runs: `133 133 133 134 133 133 134 133 133 133` | **0 B, all ten** | — |
| **070, lend variant** | the same `eat`, argument `x.cstr()` | **0** | **0** | ten runs: **134 ×10** | **0 B** | exit 134, **4 ASan lines** |
| **072** | one `record Block tag void` over `sqlite3_malloc`/`malloc`, each freed by the other | **0** | **0** | **0**, prints `survived` | 0 B | run 0, **0 ASan lines** |

**The sanitizer is live**: the same `--sanitize` flag that reports nothing on 066
and on 072 reports **four AddressSanitizer lines and exit 134** on the lend
variant of 070. So the two zeroes are the language's silence, not a broken flag.

**And one line of the record no longer reproduces.** `docs/work/DEFECTS.md`'s 066
entry ends *"`--sanitize`: AddressSanitizer: stack-use-after-scope"*. On my
reproducer `--sanitize` gives **0 ASan lines and exit 0**, and the
compiler-engineer measured the same zero independently on its own shape. I do
not claim the original never tripped it — I did not have the original file, which
is in no tree I could find (`grep -rln "Sl2\|lend_and_return"` over the
repository returns three prose documents and no `.hero`). **What is missing is
the reproducer itself**: the entry cites a sanitizer result nobody can re-run,
on a class that is about to be legislated for.

### 1.2 The candidates, judged against those runs

| candidate | 066 | 068 | 070 as filed (lease) | 070 lend variant | 072 |
|---|---|---|---|---|---|
| **(a)** ffi's weak mark — one word, refuses a lend, names no ending call | only if the binding author writes it | same | **no — it ADMITS a lease** | only if written | no |
| **(b)** historian's default flip — unmarked foreign pointer parameter is assumed to retain | **yes, no author action** | **yes** | **no — a lease is not a lend** | **yes** | no |
| **(c)** ergonomist's W0 — eight tokens of prose, no grammar | no | no | no | no | no |
| **(d)** nothing | no | no | no | no | no |

Three things follow, and each is a run rather than an argument.

**First, defect 070 as filed is closed by NONE of the four.** Its reproducer
hands a **lease** to a function that frees it, and every candidate on the table
either admits a lease explicitly (a) or only speaks about lends (b, c). I ran
both halves: the leased shape is `133`/`134` with an empty stderr, the lent shape
is `134` with an empty stderr, and the ffi seat's own condition (b) — *it admits
a lease* — is written into candidate (a) in the seat's own words. **A sitting
that adopts (a) or (b) and reports that it has answered the give-away case will
be wrong, and the instrument that says so is `./heroes check` on the six-line
reproducer: exit 0, today and after.**

**Second, (a) closes nothing that is filed.** In all three of 066's reproducers,
and in 070's, and in 068's, the mark is absent — that is panel 169's spec-warden
objection, and this sitting's own spec-warden restated it with a mechanism
(`check/marks.hero:64-84` sweeps the alternation). Candidate (a) is a facility
for a binding author who already knows. It is not nothing — the ffi seat is right
that it is the only one of the marks a binding author cannot even write today —
but **as a closure of the four open defects it measures zero.**

**Third, (b) is the only candidate with a measured closure, and its cost is
measurable and was not measured by anybody.** I counted it. The ruler:
`grep -rnE '\.(cstr|ptr)\(\)' --include='*.hero'`, comment lines dropped.

| tree | lend-call lines | files |
|---|---|---|
| `examples/` | **8** | 4 |
| `tests/golden/` | **62** | 43 |
| `selfhost/` | 53 | 18 |

`selfhost/`'s 53 are diagnostic message text and fixtures — I read eight of them
and every one is inside a `message:` or a note string, which corroborates the
spec-warden's count rather than repeating it. **The eight in `examples/` are the
number that decides (b)**, and I read all eight: `sqlite3_open` ×2,
`sqlite3_exec` ×2, `sqlite3_prepare_v2`, `sqlite3_bind_text` with
`SQLITE_TRANSIENT`, `curl_easy_setopt(CURLOPT_URL, …)`, and one continuation
line. **Every one of the eight is a non-retaining call and every one is correct
today.** So the default flip's measured first effect on this repository is **8
refusals of 8 correct programs**, repaired either by a new negative-polarity word
on each declaration — which the binding author must know to write, which is the
objection (b) was proposed to answer — or by the lease hoists the ffi seat
priced. That number is not in any report, and it is the number the historian's
correction has to survive.

### 1.3 So: does this sitting close with nothing?

**It closes with nothing on the four defects if it picks from the list it has.**
And *nothing* is not a neutral option, which no report says out loud:
`docs/work/DEFECTS.md`'s banner reads **`**OPEN: 4**`** and CLAUDE.md
§ Verification tags a milestone only over zero open defects, the named exception
being a pending `panel NNN` in `docs/work/DECIDE.md` — whose banner reads
**`**OPEN: 3**`**, all three of them ratifications (167, 168, 169). **So (d)
leaves M-declared-extents untaggable, and it is the fourth sitting in a row to
leave it so.**

What is missing from the list is a candidate that closes something **measured, at
`check`, today**. There is one, and it is not a mark: § 5.

---

## 2. Defect 072 — are the engineer's condition and the ffi seat's shim the same thing?

**No. They are two different mechanisms, and the engineer's *claim* is a third
thing that is not either of them.** I ran the whole matrix.

| program | what it is | check | build | run |
|---|---|---|---|---|
| `one.hero` | one `record Block tag void`, both families, crossed frees | **0** | **0** | **0**, prints `survived`, `--sanitize` 0 ASan lines |
| `two.hero` | two records, **both `tag void`**, the honest tags | **1** `error[duplicate_tag]` | — | — |
| `faketag.hero` | two **invented** tags, no shim, frees **matched** | **0** | **1** `error[ffi_unknown_name]: sqlite3.h declares no HeroSqliteBlock` | — |
| `faketag_cross.hero` | two invented tags, no shim, frees **crossed** | **1** `error[type_mismatch]: expected HeapBlock, found SqliteBlock` | 1 | — |
| `shim.hero` | two tags **declared in a 12-line shim header**, crossed | **1** `error[type_mismatch]` | 1 | — |
| `shim_ok.hero` | the same, matched | **0** | **0** | **0**, prints `survived` |

Read the `faketag` pair together and the sitting's disagreement dissolves:

- **two distinct tags DO give `type_mismatch` at `check`** — the engineer's half
  is true, `faketag_cross` is exit 1 with the caret on the crossing call;
- **but the program an author must be able to ship does not BUILD**: `faketag`,
  the same binding with the frees matched, is `check` **0** and `build` **1**,
  `ffi_unknown_name`, because `sqlite3.h` declares no such struct. The engineer's
  table has a row for the crossed program and **no row for the matched one**, and
  the matched one is where the route dies. That is the shared brief's trap 1 —
  *`check` is not `build`* — landing on the one seat that quoted it.

**So "072 is already closed by two tags and no new code" is false as stated.**
Two tags are declarable only where a header declares them, and supplying that
header is exactly the ffi seat's twelve lines. The engineer's *claim* and the ffi
seat's *shim* are one mechanism, and its honest price is one shim header per
allocator family, `check` 0 / `build` 0 / run 0 measured on `shim_ok.hero`.

**The engineer's PATCH is a genuinely different, third mechanism** — a condition
inside `one_tag_one_type` admitting a second record over `tag void`, which needs
no shim. It is described as *"priced, not built"*, and § 5 is what I found when I
went looking for what it would cost.

**And the defect entry's own pointer is wrong.** `docs/work/DEFECTS.md` 072 says
*"`selfhost/handles.hero`'s `one_tag_one_type`"*. `grep -c one_tag_one_type
selfhost/handles.hero` is **0**. The rule is at
`selfhost/check/decls.hero:313`, in a 346-line file; `handles.hero` holds the
`duplicate_tag` diagnostic and the rule's rationale. A session sent to the named
file finds the message and not the rule.

---

## 3. A claim asserted and not measured, settled

**Can an author write two tags for two `void *` families today?**

**Yes at `check`, no at `build`, unless a shim header supplies the struct tags.**
Both seats are half right and neither said which half:

- the ffi seat's *"forced onto one type by `duplicate_tag`"* is true of the
  **honest** binding, `two.hero`, where both tags must be `void` because both C
  functions really take `void *`: `check` **1**;
- the engineer's *"distinct tags give two `type_mismatch`"* is true of the
  **crossed** program only, and its matched sibling is `build` **1**.

**A second claim of the engineer's that I could not confirm and could not
refute.** Its table row reads *"066, `cstr` lend, C parks the address"*, and a
`cstr` lend is `.cstr()`, whose 066 witness in the docket is `.ptr()`. I built
the `.ptr()` shape and got the same 0/0/0 and the same `112`. I am not saying the
row is wrong; I am saying **two different lends are being written down under one
defect number**, which is CL-078's shape — and the engineer's own third finding
(*"a fourth shape nobody filed"*) is the same observation from the other side.

**A third, and it is the ergonomist's hesitation 10, which I could run and it
does not hold — but what replaces it is worse.** The seat wrote that the
canonical *C keeps the pointer* call *"cannot be written"*, because
`SQLITE_STATIC` is a null function pointer and `nullptr` is documented for `ptr`
and `cstr` only. Run:

- `d: (function(ptr) -> ())` + `nullptr` → `check` **1**,
  `type_mismatch: expected (function(ptr) -> ()), found ptr`;
- **`d: ptr` + `nullptr` → `check` 0, `build` 0, run 0.** The escape exists, and
  the shipped corpus uses it: `examples/ledger/db/sqlite.hero:107` declares
  `destructor: ptr` and line 54 imports `constant SQLITE_TRANSIENT: ptr`.

**But the two spellings are mutually exclusive, and nobody has noticed:**

- `d: ptr` + a **function name** → `check` **1**,
  `type_mismatch: expected ptr, found (function(ptr) -> ())`;
- `d: (function(ptr) -> ())` + **`nullptr`** → `check` **1**, as above.

**So one declaration of `sqlite3_bind_text` cannot reach all three of its modes.**
The binding that makes the give-away route work — measurement 037, the one the
shared brief calls *"the give-away route now works with no new form at all"* —
and the binding the ledger actually ships are **two different declarations of the
same C function**, and each forbids the other's arguments. Every seat stood in
one of the two worlds: the ffi seat ran the three modes **in C**, the engineer ran
R5 with the function-typed parameter, the ledger uses the `ptr`-typed one. **A
mark on the third parameter would have to be true in both worlds, and the fifth
argument that decides the fact is not even the same Heroes type in the two.**

---

## 4. The non-reproducible emission divergence — reproduced, and it is not the emitter

**Settled. It is a stale compiler binary, not nondeterminism, and the emitter is
byte-identical on both sides of it.**

The ffi seat preserved both files at `scratchpad/r5/t2.c` and `t2b.c`, in the
scratchpad this session shares with it, and they differ on one line: 146.

| compiler built from | line 146 of the emission |
|---|---|
| `7267db07`, **step 10** | `hero_unreachable(); /* the gate refuses this form */` |
| `79688a9c`, **step 11** | `hero_unreachable(); /* the gate refuses this form */` |
| `ecd948c0`, **step 12** | `t6 = free;` |
| `65e10d28`, **HEAD** | `t6 = free;` |

And the identification is exact, not similar:

```
diff <(grep -v '#line\|Generated by heroes' step10_emission.c) \
     <(grep -v '#line\|Generated by heroes' scratchpad/r5/t2.c)   → empty
diff <(grep -v '#line\|Generated by heroes' HEAD_emission.c) \
     <(grep -v '#line\|Generated by heroes' scratchpad/r5/t2b.c)  → empty
```

Both raw diffs are **62 lines and every one of the 62 is a `#line` or the
generated-by header**, absolute path against relative — an artefact of where I
put the file, not of the compiler.

**Determinism holds on both binaries**: 20 emissions each, `rm -rf build` before
every one, `shasum | sort -u` → **1 unique hash** for the step-11 compiler and
**1** for HEAD.

`ecd948c0` is step 12, *"a gate that refuses a form and then emits it is not a
gate"*, and `git log -S "the gate refuses this form"` names it. **The seat
emitted once with a pre-step-12 compiler and once with a post-step-12 one.** Both
binaries still in the shared scratchpad emit `t6 = free;` today, and
`scratchpad/tree/heroes` hashes **identical** to my own HEAD build
(`d0e7a76a3db0…`) — which is the mechanism: `scratchpad/tree/` is the
**compiler-engineer's** copy, the one its own report says was six commits behind
at `7267db07` before it rebuilt. **Two seats shared one scratchpad path and one
rebuilt the other's compiler under it.** The ffi seat's four searches — build
cache, header directory, group order, binary identity — were four right questions
asked of the wrong object: the binary at that path was not the binary at that
path ten minutes earlier.

**What is missing, and it is not a defect in the emitter.** This is
`.claude/rules/verification.md` § *The compiler that judges is a build artifact*
arriving in a panel for the **second time in one sitting** — the engineer caught
it in itself and reported it, the ffi seat did not and filed a question against
the determinism rule. **The missing rule is about the sitting, not the compiler:
seats must not share a scratchpad path, and a seat's compiler is identified by
`shasum`, not by where it sits.** `git ls-files`-based copies make this invisible,
because the copy has no `.git` to ask — `git rev-parse` in `scratchpad/tree/` says
*not a git repository*.

The defect-069 determinism worry raised by the question is **not supported**: I
could not make one binary emit two files, in 40 attempts across two binaries.

---

## 5. The route nobody listed

**What would have to be true for one to exist?** The shared brief asks *"what a
DECLARATION should be able to say"*, so every route outside the declaration was
invisible by construction, to five seats at once. Three exist. The first is
runnable today and closes a defect.

### 5.1 For 072: narrow `one_tag_one_type` at the tag `void`, and the rule's own reason says why that is different

Nobody quoted the rule's stated reason. It is `selfhost/handles.hero:127-130`,
verbatim:

> *"Two handles of one tag are two Heroes types and ONE C type, `struct s *`
> twice, so the checker refuses a swap between them and clang accepts it: the
> mutant survives every instrument the language has, permanently."*

Three things follow that no report contains.

**(i) The reason is written about `struct s *`, and `void` is not that.** For two
real struct tags, refusing the duplicate keeps clang as a second judge of a swap.
For `tag void` there is no second judge available in C **at all** — `void *` is
compatible with every object pointer — so at that one tag the rule buys nothing
it could ever buy, and costs defect 072. A condition that admits a second handle
**only where the tag is `void`** is therefore strictly narrower than the
engineer's proposal and does **not** reverse panel 145's narrowing or the author
decision of 2026-09-13 for any struct tag.

**(ii) The reason's own claim is falsified by a run.** *"The mutant survives
every instrument the language has"* — `shim.hero` is that instrument:
`error[type_mismatch]: expected HeapBlock, found SqliteBlock`, **`check` exit 1**,
with a caret on the crossing call. A diagnostic at `check` is annotatable in the
source and snapshottable in `tests/golden/check/`, which is exactly a mutation
that would not survive. The rule was written before the checker could refuse the
swap **legibly**; it can now, and I ran it.

**(iii) I went looking for what the relaxation would break and found one site the
engineer did not price — and then bounded it.** `emit/ffi_tag.hero:197`'s
`record_by_tag` returns the **first** record carrying a tag, disambiguated only
by KIND, and its own module comment says the lookup *"answered with the first
record on the tag until 2026-09-15, and that was safe only while one tag meant
one record"*; panel 153's completeness critic measured the wrong caret there.
Admitting two **handles** on one tag makes KIND stop discriminating. The bound:
both live callers (`ffi_tag.hero:159` and `:235`) pass `want_handle: false` and
parse clang text naming `struct <tag>`, and a `tag void` handle is emitted as
`void *` with no `struct` — I read it in the emitted C of `one.hero`
(`void * h0_a;`, `_Static_assert(HERO_RET_RECORD(sqlite3_malloc(…), void *)`).
clang cannot say `struct void`, so neither caller can be reached at that tag.
**That last step is a reading of two call sites and not a run, and I mark it as
such** — it is the one thing in this section a session landing the change must
verify with the `emission` and `check` suites rather than take from me.

**And the emission has no collision.** Comparing `one.hero`'s C with
`shim_ok.hero`'s: the C **type** comes from the tag (`void *` /
`struct HeroSqliteBlock *`) but the generated helper names come from the
**record** (`h_one_Block_eq` / `h_shimok_SqliteBlock_eq`, `h_shimok_HeapBlock_eq`).
Two records over `tag void` give two distinct helper names over one C type — no
redefinition.

**So 072 has a route that needs no mark, no spec token, no shim in the author's
program, and one condition in a 17-line function.** It is the only thing in this
sitting that closes an open defect at `check` with the language it already has.

### 5.2 For 070: the slot is already in the grammar, already reaches the backend, and today it is a crash

The historian's condition 5 — *"evidence that Heroes' existing `borrows` already
carries the freeing case"* — could not be run by that seat. I ran it, and the
answer generalises past `borrows`:

- `eat(s: cstr borrows)` → `check` **1**, `error[unread_mark]`;
- `eat(s: cstr consumes)` → `check` **1**, `error[unread_mark]`.

Both words **parse** on the parameter and are then thrown away by the handle-only
sweep. So the shape of 070's answer is not *a word the language lacks*; it is *a
meaning for a word the language already admits*. That is the spec-warden's V0
(+63 real) and VD (+74 real), and what is missing from both is the observation
that makes them cheap: **the grammar already carries `[ "owned" ident ]` on every
parameter, the checker already admits it on an input parameter, and it already
reaches clang.** § 6 is what happens when it does.

### 5.3 For the mark itself: the fact the sitting is trying to declare is chosen per call, and Heroes already passes it per call

The ffi seat measured that argument five of `sqlite3_bind_text` decides
retention, per call. § 3 above measures that the ledger **already passes that
decision as an argument** (`destructor: SQLITE_TRANSIENT`) and that the
declaration cannot even hold both spellings of it. Nobody proposed the
corresponding move: **say it at the call, not at the declaration.** Heroes has
labelled arguments and already carries a call-site marker (`@` for an
out-parameter), so the shape exists. I am not recommending it — it is a grammar
change nobody has priced and the ergonomist has not read a program in it. **I am
saying it was never on the list, and the list is a measurement (CL-057): a
recommendation among (a)–(d) is a claim about the option set, and this option is
outside it.**

---

## 6. The fifth defect: it is not in the list, it is reachable from the shipped grammar, and it is an `internal error`

The spec-warden filed this in its report, in its § 3(c), and said *"it is filed
here because no seat's brief covers it"*. **It is also not in
`docs/work/DEFECTS.md`, whose four open items are 066, 068, 070 and 072.** I
reproduced it, and it took three shapes to find the one that crashes — which is
the § RUN IT rule about attacking the shapes beside the one that provoked it:

| shape | check | build |
|---|---|---|
| `atoi(s: cstr owned free)`, `free` not declared | 1 `error[unknown_freer]` | 1 |
| + `function free(p: cstr)`, argument `.cstr()` | 1 `error[type_mismatch]: expected str?, found cstr` | 1 |
| + `function free(p: cstr)`, argument a `str?` | **0** | 1 `error[ffi_writable_parameter]` |
| + **`function free(p: ptr)`**, argument a `str?` | **0** | **2** |

The last row, in full:

```
internal error: compiling the generated C failed:
own5.hero:7:33: error: passing 'h_0opt_f87774a' (aka 'struct h_0opt_f87774a')
  to parameter of incompatible type 'const char *'
    7 |     t4 = atoi(hero_cstr_nonnull(t3));
```

**`check` exit 0, `build` exit 2, the compiler blames itself, and the caret is on
the caller's line rather than on the declaration that is wrong.** The mangled
name is `h_0opt_f87774a`, the same one the warden reports, so this is that
finding and not a cousin.

Why it belongs in this sitting rather than in a queue: **`owned <fn>` on an input
parameter is the slot two candidate routes want to build on** (the warden's VD,
and any give-away mark). A sitting that adopts a meaning for that slot lands it on
top of a shipped crash, and the crash is in the `build` stage, which is where the
brief's own trap 1 says FFI refusals live.

---

## 7. The shorter list of what else is missing

1. **Nobody scored panel 169's predictions**, and two of them are about this
   sitting's subject. Its completeness critic registered: *a rule stated over the
   AST needs a fourth widening within two milestones*. This sitting's
   compiler-engineer states the rule **over the AST** and argues the IR carries no
   marks — a direct contradiction of panel 169's adopted item 3 (R6), which the
   shared brief lists as *the default in force*. **Two adopted resolutions now
   point opposite ways on the layer question and no report says so.**
2. **The `068`-is-`066` reclassification is asserted in `DEFECTS.md` and in the
   shared brief and is not carried by any 170 report.** Four of the five reports
   treat 068 as a line item they inherit. If the synthesis keeps the merge, **068
   should be ticked and moved**, not left as a fourth open number that every
   future count reads as a separate class; if it does not, the brief is wrong. I
   have no measurement that settles which — it is a ruling, and it is owed.
3. **The `--sanitize` claim in 066's entry does not reproduce** (§ 1.1), and the
   reproducer file it refers to is in no tree.
4. **`DEFECTS.md` 072 points at the wrong file** (§ 2).
5. **No seat ran a suite.** The engineer says so plainly of its patch
   (*"the one thing I could not run"*); the others did not need to. So every
   claim in this sitting about what a change would refuse across the corpus rests
   on greps, mine included. The named instrument for the 072 route in § 5.1 is
   `check`, `emission` and `determinism` per `.claude/rules/verification.md`'s
   new-refusal rule, and **it is unrun by everybody.**
6. **Two seats shared one scratchpad and one overwrote the other's compiler**
   (§ 4). That is a process finding and its home is `/panel`, not the language.

---

## 8. What this seat registers, so it can be scored

No verdict. Two falsifiable registrations, instruments named.

**C1.** If the synthesis adopts any of (a), (b) or (c) and reports that the
give-away case is answered, then at the M-declared-extents close
`./heroes check` on 070's six-line reproducer will exit **0** and the built
program will exit **133 or 134** with **0 bytes** on stderr. Instrument: those two
commands on the file in 070's entry.

**C2.** If the synthesis adopts the `tag void` narrowing of § 5.1, then
`./heroes check` on `two.hero` goes **1 → 0**, `./heroes check` on the crossed
program stays **1** with `error[type_mismatch]`, and `docs/work/DEFECTS.md`'s
banner goes **4 → 3**. If it adopts the shim instead, the banner does not move,
because a shim is what an author writes and not what the compiler refuses.
Instrument: `./heroes check` on the two files, and the banner.
