# Panel 148 — completeness critic

Not a verdict. What is MISSING, what was asserted without being run, and where
two seats cannot both be right.

Everything below was run at commit `94b94bcd`, read-only against the working
tree, with a seed built in a scratch copy (`clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`, **2.91 s user**). The four probes are 12 to 14
lines each and are quoted where they carry a claim.

---

## 1. The three "third options" are two axes, and the group-wide cost is real for two of them and dissolves on a key nobody tried

### 1.1 They are not three options

- **(a)** llm-ergonomist: C's inference becomes a **diagnostic** — a group
  declaring `consumes` for `T` with no `acquires` producing `T` is a compile
  error naming both lines.
- **(b)** ffi-pragmatist's **D**: `acquires` plus a mandatory `borrows` on any
  hand-back of a consumed type; unmarked is an error naming the call.
- **(c)** historian: `acquires <releaser>` — the mark NAMES the consuming
  function.

**(a) and (b) are the same axis and (b) strictly dominates.** Both ask "is this
binding's marking complete?"; they differ only in granularity. (a) fires once
per type and is satisfied by a single marked producer; (b) fires per call site.
The case that separates them is not hypothetical:

```
/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/curl/easy.h:41  CURL *curl_easy_init(void);
…/curl/easy.h:74  CURL *curl_easy_duphandle(CURL *curl);
…/curl/easy.h:44  void curl_easy_cleanup(CURL *curl);
```

Two producers of `CURL`, one releaser. Under **(a)** a group that marks
`curl_easy_init acquires` is complete, and an unmarked `curl_easy_duphandle`
leaks in silence — exactly the failure (a) exists to close. Under **(b)** the
`duphandle` line must say `acquires` or `borrows`. All three are in scope of the
shipped group `examples/curl/main.hero:33`, `extern "curl/curl.h"`, because
`curl.h:3213` is `#include "easy.h"`.

**(c) is the other axis and is orthogonal to both.** It does not close A's
silent failure (a forgotten `acquires` is still forgotten); it enriches the
mark's payload and buys GCC's `-Wanalyzer-mismatching-deallocation` class. It
composes with (b) without interference: `acquires sqlite3_finalize` /
`borrows` / nothing.

**So the maximal resolution is (b) + (c), and nobody proposed it.** CLAUDE.md
§ Precedence's standing instruction of 2026-09-12 — *the most robust and
production-ready resolution, never the compromise* — points there, and the
sitting has no reading of it.

### 1.2 The prices, all re-measured by me with `./heroes measure`

| draft | file | cl100k | delta | merge? | both grammar positions? |
|---|---|---|---|---|---|
| live spec | `spec/heroes-spec.md` | 5863 | — | — | — |
| C | `…/panel-148/spec-c.md` | 5919 | +56 | n/a | n/a |
| A as briefed | `…/panel-148/spec-a.md` | 5927 | +64 | no | **no** |
| A repaired (warden) | `…/warden/spec-a-repaired.md` | 5933 | +70 | no | yes |
| A merged + repaired (warden) | `…/warden/spec-a-merged.md` | 5921 | +58 | yes | yes |
| **(c) `acquires <releaser>`** | `…/panel-148/spec-d.md` | **5929** | **+66** | **yes** | **yes** |
| **(b) `borrows`** | `…/scratchpad/spec/spec-d.md` | **5967** | **+104** | **no** | **no** |

Two things fall out that no report contains.

**(c) is cheaper than the A the warden says must land.** The historian could not
measure it and estimated *"more than A's +64"*. Measured: **+66** — two tokens
over the briefed A, and **four tokens UNDER the repaired A** (+70), because the
coordinator's (c) draft already carries the warden's merge and amends **both**
grammar positions. `diff` of that draft against the live spec is four hunks and
I ran it. The draft is timestamped 13:52, after four of the five reports; no
seat ever saw the number that makes the historian's option the second-cheapest
correct text on the table.

**(b)'s +104 is not comparable to anything.** Its draft leaves `Member`
untouched (`diff` shows one grammar hunk, `CParam` only) and duplicates the
abort clause. Apply the warden's two repairs and it is about +98. Worse: **the
ffi-pragmatist's own decisive prediction is not derivable from its own draft.**
Its prediction writes

```
    function sqlite3_next_stmt(db: CDb, statement: CStmt) -> CStmt
```

and says *"Under D: it compiles with `borrows` on the second line"* — a RESULT
position, which its `CParam`-only grammar does not admit. That is the warden's
Finding 1 reproduced inside the report proposing the alternative, by a seat that
had not read the warden.

### 1.3 The transfer question: verified, then refuted on the key

**Every line the compiler-engineer cited reproduces.** I re-ran its instrument
in Python against `tests/harness/suite_layout.hero:484`'s `code_lines` rule:
`ast.hero` **503**, `print/fmt.hero` **1153**, `check/consuming.hero` **114**,
`check/ffi_sweep.hero` **93**, `ir/owned_release.hero` **277**. `wc -l
runtime/parts/alloc.c` = **321**, `hero_live_held` at `:98`, its pair at `:180`
and `:185`. `consuming_positions` at `check/consuming.hero:45-58` is 13 lines
and reads one declaration. `parse/group.hero:6-9` and `ast.hero:437-439` say
what the seat quotes. All confirmed.

**And I found a pair it did not count.** `extern "stdlib.h"` appears twice in
the compiler's own program: `selfhost/cli/process.hero:49` and
`selfhost/emit/literal.hero:41`. So the split-group shape is **two** headers in
`selfhost/`, not one.

**The scenario the seat described as a risk, I built, and it checks clean.**
Producer in one module's group, `consumes`-marked releaser in another module's
group, one handle type:

```
# lib/a.hero
extern "sqlite3.h" link "sqlite3"
    record CDb tag sqlite3
    function sqlite3_open(filename: cstr, @out: CDb) -> i64
# main.hero
use lib/a
extern "sqlite3.h" link "sqlite3"
    function sqlite3_close(@db: a.CDb consumes) -> i64
```

`./heroes check p3/main.hero` → **exit 0** at the frozen commit. So a
GROUP-keyed rule — C, and **(a)** and **(b)** exactly as their authors wrote
them — does not fire on a program this language accepts today, and does not say
so. **The group-wide cost and the group-wide silent miss do transfer to (a) and
(b) as worded.**

**But it is not the cost the seat priced, because the compiler solved this
problem already and the sitting never looked.**
`selfhost/check/decls.hero:285-302`, `one_tag_one_type`, is **17 lines** and a
`{str: str}` over ALL declarations. Its own comment at `:281-284` is the answer
to the seat's hinge, written before the sitting convened:

> The walk is over the declarations rather than the groups because a group is
> flattened in the parser and no later pass knows the word: two records in two
> groups over one header are the same mistake, and the header is not compared
> for the same reason.

And the invariant it enforces is program-wide. Probe 1, two groups, two record
names, one tag:

```
error[duplicate_tag]: `CDb2` and `CDb` both name the C type `sqlite3` — one tag, one type
```

So **one tag is one handle type across the whole program**, which means a
question keyed on the TYPE needs no group identity, no span reconstruction, no
header-text key, and no new kind of table: it is `one_tag_one_type`'s shape, a
second time. Restate (a) and (b) on the type and the hinge cost is 17 lines of
shipped precedent instead of 90 lines over a key the seat correctly says the
language does not define. Keying a **completeness diagnostic** on the type does
not re-open panel 147: the obligation is still created by the marked call, and
the type only scopes the question "is this binding finished?".

This does not save C. C dies on the ffi seat's 12-of-19 correctness
measurement, which is independent of the key. But the synthesis must not carry
"a new table over a destroyed entity" as C's cost, because the same sentence
would kill (a) and (b), and it is wrong for all three.

**(c) needs no group question at all, and the historian is right about that.**
`acquires <releaser>` is the `owned ident` shape, which already parses in BOTH
positions: `parse_members.owned_marker` at `selfhost/parse/members.hero:100`,
called from `selfhost/parse/tails.hero:143` for the result and the parameter,
stored as `ast.hero:448 owned_result`, resolved and refused-when-called at
`selfhost/check/freer.hero:221`. Verified by reading, not inferred.

---

## 2. Claims asserted, not run, and the command that settles each

**compiler-engineer**

1. *"Group identity has to be reconstructed from a span offset"* and the whole
   four-part hinge. Reasoned, never keyed on the type. Settled by
   `sed -n '281,302p' selfhost/check/decls.hero` plus probe 1. **This is the
   sitting's largest unrun claim and it is the sitting's stated hinge.**
2. *"a binding split across two modules — which §4.19 forces"*. True, but the
   citation `resolve/names.hero:48-56` is the LIBRARY arm; the general arm is
   `selfhost/resolve/qualified.hero:66` (`is_library: false`). Settled by
   `grep -rn extern_across_modules selfhost/` and by probe 2 (exit 0).
3. *"Under C a correct program aborts at exit saying N-1 leaked"* for
   `curl_slist_append`. Nothing implements C; this is a consequence, not a run.
   The ffi seat ran the C equivalent, so the claim survives, from the other
   report.
4. *"It is a BALANCE, not a matching … the same hole `consumes` already
   documents at `check/consuming.hero:22-31`."* The balance claim is sound; the
   citation does not say it. Those lines document the COPY hole (a copy made
   before the consume still holds the freed address), not a counting hole.
   Settled by `sed -n '20,33p' selfhost/check/consuming.hero`.
5. **The timing, `+0.17 s, +1.2%`, declared "machine still".** The five reports
   are timestamped 13:44, 13:46, 13:51, 13:52, 13:54; the ffi seat was
   compiling AddressSanitizer probes and linking `libsqlite3`, and the warden
   was running `heroes measure`, inside that window.
   `.claude/rules/verification.md` is explicit: *parallel work is free on
   correctness and forbidden on duration*, and CL-025 says the machine stays
   still. **A panel that runs its seats concurrently cannot produce a valid
   timing from any seat.** Either the number is discarded or the procedure
   serialises the seat that times. Settled by `ls -la docs/panel/148-reports/`.

**ffi-pragmatist**

6. *"Headroom on the binding `real` row is 386, so D fits."* That carries a
   VENDORED delta across to the REAL budget, which is the conversion
   `docs/measurements/010:172` forbids in terms (*"No row is convertible"*) and
   which its own sitting's warden brief warned about by name. Settled by
   `grep -n "No row is convertible" docs/measurements/010*.md`.
7. *"The mark has zero C footprint … neither A nor C can move a handle."* Run,
   and true **of the surface**. It is not true of the instrument: the counter
   emits an increment at the acquiring call and a decrement at the release
   (`runtime/parts/alloc.c:180,185`), and `docs/measurements/032:115-122` says
   that half is unbuilt. The ABI clearance should be scoped to the mark.
8. *"12 of 19"*, measured "within ONE header file". The unit is not the
   language's unit: a Heroes group names one header and clang resolves its
   includes, so `extern "curl/curl.h"` reaches `easy.h`. The seat listed
   `curl/easy.h` among the three that are *"each alone safe"* while it holds
   `curl_easy_init` and `curl_easy_duphandle` (:41, :74). **12 of 19 is a lower
   bound.** Settled by `grep -n 'include.*easy.h' curl.h` → `3213`.
9. *"`examples/curl` emits C and fails only at the link step"*. Contradicted:
   `./heroes build examples/curl/main.hero -o /tmp/curlout` → **exit 0**,
   `wrote /tmp/curlout`, in my scratch copy on this Mac.

**spec-warden** — I found nothing asserted that it did not measure or mark
*unrun*. Its three vendored counts, the 126-127 spread, the +6 grammar repair,
the merged +58, the `DELTA_GATE` = 50 at `tests/harness/suite_spec.hero:169`,
the guard `if path == budgets.CONTRACT` at `selfhost/cli/measure.hero:136`, and
the absence of contextual marks from `spec/reserved-words.md` all reproduce
exactly. **Finding 6 is sharper than the warden put it**: the constant's own
comment at `suite_spec.hero:164` says *"on both judged documents"*, and the only
enforcer covers one. That is a false sentence in the tree, not just a gap.

**historian**

10. *"Its spec cost is unmeasured … Estimate only: more than A's +64."*
    Measured here: **+66** (see § 1.2). Right in sign; the seat could not know
    it lands under the repaired A.
11. The `sqlite3_errmsg` analogy. Weaker than "analogy": **neither limb of C's
    trigger is present.** `sqlite3_free` is declared without `consumes`
    (`examples/ledger/db/sqlite.hero`, `function sqlite3_free(p: ptr)`), and
    `cstr` is not a handle type. Settled by reading the group.

**llm-ergonomist**

12. *"Lines 1 to 374 are identical in all three"*, by eye, no diff tool.
    **Machine diff: exactly right.** The variants differ only at `374a375-377`
    (variant 2), `374a375-378` (variant 1) and line `386`.
13. *"`?` is unusable after an acquisition"*. Reasoned from the text. It holds —
    `expr?` propagates (spec:152) and the language has no defer — but the spec
    does discuss `?` and early exit at spec:255 for copy-out, so the seat's
    "neither variant teaches it" is about release, not about `?` in general.
14. Its two structural escapes, checked against the compiler it was not allowed
    to open: `duplicate_tag` fires (probe 1), and a handle cannot be passed to a
    `ptr` parameter — probe 4, `error[type_mismatch]: expected `ptr`, found
    `CDb``. **Both blind readings are correct.**

---

## 3. Contradictions between seats, and which side is checkable

**(i) The historian against the ffi-pragmatist and the compiler-engineer on
SQLite. Not a contradiction: two different populations, and the historian's
conclusion should still not be carried.** The historian counted the SHIPPED
binding (`examples/ledger/db/sqlite.hero`): 18 `function` declarations
(`grep -c '^    function'` = **18**, confirmed), zero of which return `CDb` or
`CStmt` as a result — correct, I read all 18. The other two counted the HEADER:
`sqlite3_db_handle` and `sqlite3_next_stmt` are in `sqlite3.h` and bound by
nobody, which `docs/measurements/031:54-60` already records. Both are right.

But *"A and C agree on 18 of 18"* is **vacuous and should be struck from the
synthesis**. The historian itself supplies the reason two lines later:
`sqlite3_close` carries no `consumes`, so C's trigger is absent and C infers
nothing; A's marks are also zero today. They agree at zero. The moment the
milestone does its job and `consumes` lands, they diverge: under A two of 18
gain a word, and under C the answer depends on whether an `@` out-parameter
"hands one back" — which the ergonomist's hesitation 3 shows the text does not
decide, so C infers either 0 or 2 and nothing in the sitting says which.

**(ii) The line numbers for the same two functions do not match, and they are
two SDKs.** The warden cites `sqlite3_db_handle` at
`/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/sqlite3.h:7052`
and `sqlite3_next_stmt` at `:7187`. The engineer and the ffi seat cite `:6861`
and `:6996`. Both are right: this Mac has two SDKs whose `sqlite3.h` differ by
341 lines (12016 vs 11675), and `xcrun --show-sdk-path` returns the Xcode one.
The defect is the ffi seat's elision, `…/MacOSX.sdk/usr/include/sqlite3.h`,
which is ambiguous between them; the curl lines agree only by luck (2798/2808 in
both). A panel record whose citations cannot be resolved to one file has lost
the property it was written for.

**(iii) Three seats carry three different values for the same gap.** The
compiler-engineer argues twice from *"8 vendored spec tokens"* — the brief's
figure. The warden measured in the same sitting that the gap is **14** as
drafted and **2** merged. Reproduced: 5927−5919 = 8, 5933−5919 = 14,
5921−5919 = 2. The seats were concurrent so nobody is at fault; the synthesis
would be, if it repeats the 8.

**(iv) `examples/curl`**: ffi says it fails at link; it builds here, exit 0.

**(v) Inside the tree, found by the warden and confirmed**: `DELTA_GATE`'s
comment claims both judged documents, its enforcer covers `CLAUDE.md` only.

**(vi) The brief against the tree on two numbers**: see § 4.

---

## 4. Framing facts the seats were handed and did not check

Beyond the two already known (the omitted +6 grammar repair, and a priced A
against an unpriced C), five more.

**F1 — the brief silently settled the milestone's OTHER open question.** Both
variants' spec text ends *"aborts when `main` returns, saying how many"*.
`docs/measurements/032:133-134` says in terms: *"It does not choose between a
compile error and a loud exit — that is the milestone's second item."* By
writing the abort into both drafts, the brief removed that choice from every
seat, including the one that holds the veto on comprehension: the ergonomist's
three variants are C, A and the status quo, so a compile-error instrument was
never on any seat's page. The compiler-engineer noticed the hole from inside its
own condition (*"the sitting should hear the loud-exit-versus-compile-error
question again before the word lands"*) and could not act on it.

**F2 — a third instrument exists in the milestone's own record and was demoted
to a bullet.** `docs/measurements/031:103-118` prices the discarded release
return code at 21 sites (19 `_ =` plus 2 wrapper returns) and says *"It belongs
on the ladder as a third row"*. The brief kept the fact and dropped the ladder,
and asked for a verdict on two.

**F3 — "Two files land over their ceilings: `ast.hero` 505 → 508" reads as a
before-and-after and is not one.** 505 and 1156 are the DECIDED **ceilings**
(`tests/harness/suite_layout.hero:369,373`); today's measures are **503** and
**1153**, which I computed with that file's own `code_lines`. So landing A
means **moving two ceilings in the `DECIDED` table**, which that file's own
comments at `:340-365` treat as an act needing a written reason. The warden
judged the spec budget. **Nobody judged the layout budget, and it is the only
budget A actually breaches.**

**F4 — "refusing 2 of 17 functions in the shipped SQLite binding" is two errors
in one clause.** Panel 145's figure is 2 of 17 **calls**
(`docs/panel/145-…:206-217`), and the binding declares **18** functions. The
17/18 distinction was live in panel 145's own record (`:451`).

**F5 — the brief's grammar is too WIDE as well as too narrow, and only the
narrow half was found.** `00-shared.md:43` writes
`CParam = [ "@" ] ident ":" Type [ "owned" ident ] [ "consumes" | "acquires" ]`.
`@` is optional, so `function f(db: CDb acquires)` on a plain input parameter is
derivable and the prose (*"after a handle result or `@` out-parameter"*) does
not define it. `consumes` carries the same looseness today, which is the
ergonomist's hesitation 4 arrived at blind. The warden's repair closes the
result position and leaves this open.

### The llm-ergonomist's disclosure: the leak is the harness, and it is wider than the seat said

**Confirmed, and not from the seat's word.** The identical block is in THIS
agent's own environment: *"88e59061 M-cleanup-verdict closes: a releaser keyed
on the type goes to Part 6, and the form that enters is marked on the call"*.
The seat did not go looking; the harness injects `gitStatus` into every
subagent.

**And the same injection carries more than the seat disclosed.** It also
carries the full text of `CLAUDE.md`, which I can prove of my own prompt and
which the seat's brief had no way to suppress: the thesis sentence *"every
plausible LLM mistake is a compile error"*, § 4's panel rules, § 13's *"Anything
in design.md Part 6"*. The ergonomist disclosed the commit subject and not this,
so either it did not receive the contract or it did not notice it. Either way
**the blind cannot be asserted, only hoped for**, and the agent definition's
*"MUST receive only `spec/heroes-spec.md` and sample programs"* is not enforced
by anything.

**How far it contaminates: the convergence, not the findings.** The seat's
decisive work is derivable from the variant text and I checked it against the
compiler it never opened — the eye-diff is exact, `duplicate_tag` fires, the
`ptr` escape is refused by `error[type_mismatch]`, the grammar gap is real. So
the veto on variant 1 stands on its own evidence. What does NOT stand is
treating its agreement with the warden on the grammar gap, or with the ffi seat
on the borrowing accessor, as **independent** confirmation. The synthesis should
count this seat once, not three times.

**A second contamination that nobody disclosed, and it is structural.** The
ergonomist's brief hands the seat the exact shape option C fails: *"Task 2.
Under each variant, a library has a function that hands back a statement it
still owns … This is the question that matters most"*. The coordinator chose the
discriminating case. That is defensible experiment design, but it means the
result reads *a writer who is TOLD about the borrowing accessor cannot express
it under C* — not *a writer would discover the problem*. The experiment that
would have said the second thing, a binding task with no such hint, was not run.

**What the procedure should do.** Three things, all cheap. Run this seat with
its cwd in the variants' own scratch directory, which is not a git repository
and has no `CLAUDE.md` above it, so the harness has nothing to describe. Require
the seat to report, as its first line, everything about this project that
reached it other than its named inputs, so the blind is measured instead of
assumed. And keep the discriminating case out of the brief in at least one task,
so the sitting learns whether the writer finds it.

---

## 5. The question the sitting should have asked and did not

**Where may the mark be written — and if the answer is "only inside an `extern`
group", what carries the obligation across the wrapper this corpus is built out
of?**

All four drafts amend `Member` and `CParam` (spec:382-386), which are reachable
only from `Extern` at spec:380. The ordinary function grammar is `Params` /
`Param` at spec:110-111 and admits no mark of any kind: not `owned`, not
`consumes`, not `acquires`. So under every option on the table the mark is
extern-only.

Now put that beside what `docs/measurements/031:74-89` counted. The places this
corpus actually acquires a handle are **Heroes wrappers**: `opened() -> Db?`
with **7** call sites and `prepared(db, sql) -> Statement?` with **6**, because
an `extern` is not callable across a module boundary — which I confirmed twice
(probe 2 and probe 3 compile only because the CALL sits in the declaring
module). The acquiring `extern` call is one module away from every place a
handle is held, and the wrapper that hands the handle to the rest of the program
cannot say so in any of the four texts.

That is survivable if the instrument is the loud exit, because a runtime counter
is placed at the extern call and does not care where the value goes next. It is
fatal if the instrument is a compile error, because the fact has to cross a
signature that has no room for it. And **which instrument this is remains the
milestone's open second item** (`docs/measurements/032:133-134`), settled
silently by the brief (F1). So the sitting priced, argued and approved a surface
without establishing what the surface is for, and the one option that would have
forced the question into the open — a `borrows`/`acquires` pair that has to
appear on ordinary functions too — was proposed by the seat that measured it and
priced against a grammar that cannot hold it.

Two smaller ones, both inside the sitting's own remit and both unasked:

- **What is the scope of any completeness question: the group, the type, or the
  program?** Answered in § 1.3 — the type, because `one_tag_one_type` already
  makes it well defined and the group demonstrably is not. Four seats reasoned
  about a group and none tested it.
- **The counter is a balance, not a matching.** Acquire two, consume one of them
  twice: net zero, exit 0, one leak and one double free. The spec sentence both
  variants carry says *"a handle nobody consumes aborts when `main` returns,
  saying how many"* and is silent on this. The compiler-engineer says it
  *"belongs in the spec sentence now, not in a defect later"*, and no draft on
  the table contains it — including the two written after that sentence was
  filed.

---

*Probes, all at `94b94bcd` in a scratch copy, tree unmodified: p1 two groups one
tag → `error[duplicate_tag]`; p2 a second module's group naming another module's
handle type → exit 0; p3 producer and `consumes`-marked releaser in two modules'
groups → exit 0; p4 handle into a `ptr` parameter → `error[type_mismatch]`.
`heroes measure` run on eleven files; `diff` run on five drafts against the live
spec; `code_lines` reimplemented from `tests/harness/suite_layout.hero:484` and
validated against `docs/measurements/032`'s own table. `build/` and `target/`
removed.*
