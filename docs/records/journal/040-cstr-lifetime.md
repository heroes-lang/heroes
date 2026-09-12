# 040 — M-cstr-lifetime

Closed 2026-09-09, **no tag**. Five steps, one sitting, two defects repaired and
a third one filed that the milestone was opened to close.

## Goal

Refuse the program defect 022 named: `return ("heap-" + n.to_str()).cstr()`
builds at exit 0 with zero diagnostics under all fourteen flags and hands C a
pointer into freed memory, while the same line with a literal receiver is sound
because `HERO_STR_STATIC` gives it static storage and an immortal refcount. The
two spellings are one brace apart and what separates them is invisible at the
call site.

The milestone was **placed first and alone** by author decision, out of three
the coordinator recommended, on `CLAUDE.md` § Precedence: robustness is rank 3,
above elegance, token cost, ergonomics and compiler size, and a milestone is
tagged only over a clean list.

## What surprised

**The defect was two defects, and the rule that landed closes one of them.**
Both compiling seats at panel 122 built, from opposite ends, a program whose
every lend is an argument of an extern call — the position every candidate rule
blesses — and whose C side keeps the pointer. Run again at this close: `heroes
check` accepts it at exit 0 with zero diagnostics, it prints `0` where 13 is the
answer, and `--sanitize` reports `heap-use-after-free`, READ of size 14, inside
`stash_len` called from `main.hero:14`. The discriminator is the C function's own
contract — SQLITE_STATIC against SQLITE_TRANSIENT, an argument of the call — and
both are `const char *` in the header, so **no rule about where a Heroes
expression stands can see it**. That half is defect 024, against the keyword
§4.19 has reserved since it was written: *"a borrowed pointer you must not
touch"*, case 2 of three, of which `owned` is case 1 and landed at panel 109.

**The project already knew, and had written the rule down for a human.**
`examples/ledger/db/sqlite.hero:305-309` ships that exact shape with this above
it: *"a caller must step before it drops the string, and every caller here does,
in the next line. Written down because it is the one place a correct-looking
rearrangement would be a use-after-free."* A rule performed by nobody is the
shape CLAUDE.md § 3 tells this story about, told here about memory.

**A rule of position and a rule of type are not the same rule, and two seats
vetoed the wrong one from opposite directions.** Written as a rule about the
TYPE `cstr`, it refuses `c = getenv(name: name.cstr())` — the spec-warden
compiled it and it runs — and `owned`, and a `cstr` in an `@` cell, and
`-> cstr owned free`, all of which ship. Written on the POSITION of the
`.cstr()` expression, none of them is visible to it. That reading came from the
compiler seat and it is what dissolved both vetoes at once.

**What actually keeps a lend alive is not what either seat first said.** One
synthetic owner slot **per expression site**, released at function exit **or when
its site re-executes, whichever comes first**: in a loop the emitted C carries
`t17 = h4_own4; h4_own4 = t10; hero_str_decref(t17);` inside the block, so
iteration *n+1* frees iteration *n*'s bytes. Function exit was an upper bound and
not the guarantee, which killed the escape-only candidate's whole premise.

**The strictest rule had no precedent and the cheapest one had a hole.** The
historian could source **no language that uses which KIND of function is being
called as a lifetime criterion**, which is what the extern-only rule did; and
Rust, the closest precedent, planned `temporary_cstring_as_ptr` deny-by-default
and shipped it **warn**, on the recorded ground that the argument position is *"in
typical situations a false positive"*. Read carefully that agrees with the
adopted rule about the position and disagrees about the discriminator.

**And nothing guarantees a tokeniser, which is a different milestone's finding
that landed inside this one.** The ceiling this milestone's spec clause is priced
against moved from 4096 to 6144 mid-milestone, because `heroes measure` was
answering from OpenAI's `cl100k_base` and the spec was 998 tokens over a ceiling
it was told it was 40 under.

## What broke and why

**The coordinator was wrong four times about this defect and a seat caught every
one.** The corpus count went out as *67 of 67 `.cstr()` sites are arguments of an
extern call*; it is 66, and two of the four re-reads were not externs at all —
one is the **negative** fixture `tests/golden/surface-fixtures/externroute/wrong.hero`
and one is a Heroes wrapper. The blast radius was right about files and
undercounted **sites threefold**. The recommended rule had the two-hop hole,
`return pass(c: s.cstr())`, which puts the lend in an argument and returns a call
result. And the §4.19 conflict was located at the spec when it was at one
fixture's parameter type — the compatible idiom was already shipping in a
sibling fixture, and once the extern discriminator was dropped the fixture needed
**no repair at all**.

Each of the four came from reading a grep line where a file had to be opened.

**Two suites went red for the right reason and the repair was never to loosen
them.** `emission` refuses to let a `tests/golden/fixedbugs/` case fail at the
checker, because that directory's written premise is that its cases are refused
by **clang** at build time — so four new checker-refused cases belonged in
`tests/golden/check/` under a `fixedbugs-` prefix, where the milestone before had
already put four of its own. And `spec` went red the moment the misleading
`the binding number` label was corrected, because `maximum_of` found the figure
by searching for that prose: a check that reads a human-facing label breaks when
the label is corrected, so it keys on the row's **name** now.

**And the file ceiling caught a repair that was one line over.** The first draft
of defect 023's fix put `value_errors.hero` at 301 code lines against §11's 300.
The rule says split along a seam and never at a line count, so the repair was to
write the branch as the `if` **expression** it is — one push, one fix, the choice
expressed as a choice — and the file came back inside the threshold with better
code than it had before.

**The one instrument that could see defect 023 is a test somebody had to think to
write.** `.expected` compares `check --brief`'s stderr, which carries no
`fix (…)` line, and `suite_fixes` tests only `certain` fixes through `.fixed`, so
a `guess` fix's TEXT is unwatched by every golden. That is the gap
[039](039-closures-verdict.md) filed rather than closed, and it is why a fix that
sent a reader in a circle could have stood indefinitely.

## Predictions scored

| prediction | verdict |
|---|---|
| ffi-pragmatist, panel 122: a lend the C side retains is accepted at `check` and reports heap-use-after-free under `--sanitize` | **CONFIRMED**, run at this close: exit 0 with zero diagnostics, prints `0` where 13 is right, `heap-use-after-free` READ of size 14 in `stash_len` from `main.hero:14` |
| compiler-engineer, panel 122: the same shape is still accepted at the close, i.e. the class is not shut | **CONFIRMED**, same run. Its falsifier — that the closing record names the retained-pointer shape and cites a spec sentence putting it on the programmer — is **not** met: the record names it as defect 024 and puts it on the compiler |
| compiler-engineer, panel 122: `selfhost/check/lending.hero` lands at **≤140** lines in `suite_layout.hero`'s unit | **FALSIFIED**: **207**. The counter was replicated and validated against `check/builtins.hero`, which gives 374 and matches its own `DECIDED` entry exactly. The overrun is the module doc, which §11 requires and which carries the four corrections and the three dropped candidates |
| compiler-engineer, panel 122: zero new `DECIDED` rows | **CONFIRMED**: none added, and `layout` is green |
| spec-warden, panel 122: with the merged wording, `measure` reads 4010 vendored and the fixture repair is ≤6 lines at 0 spec tokens | **PARTLY CONFIRMED, and the miss is honest.** The adopted rule is not the one it priced — the extern discriminator was dropped — so the wording differs and reads **4020** vendored, **5128** real. Its fixture claim is better than predicted: **0 lines**, because the dropped discriminator made the fixture legal again |
| llm-ergonomist, panel 122: ≥30% of first-try programs declare a non-extern function whose signature mentions `cstr` | **UNSCOREABLE HERE**, metric 2 has never run; it lapses to M-thesis-harness rather than being renewed |
| historian, panel 122: a *"may not be bound"* clause is the first relaxed within two milestones | **MOOT**: no such clause was adopted. Recorded rather than dropped, because the reason it is moot is the reason it was worth saying |

## What landed, and what carried forward

The lend rule in three clauses, positional and never typed, at **0 files of 638**
changing verdict. Four `tests/golden/check/fixedbugs-a-lend-*` cases whose
annotations equal their diagnostics exactly. Six tests inside the two modules
that hold the rule and the repaired diagnostic, 591 → 597. The spec's own
sentence, merged rather than appended, at **+34** real tokens against 955 free.
And `selfhost/emit/gate.hero`'s note, which had been prescribing the shape one of
the new clauses refuses.

**No tag, and that is the rule working.** `CLAUDE.md` § Verification says a
milestone is tagged only over a clean list, and `docs/work/DEFECTS.md` holds
defect **024** — open by panel 122 R5's own design, because the class the
milestone was opened for turned out to have a half no position rule can reach.
A tag says the tree is clean. It is not. The tag waits on §4.19's reserved borrow
keyword, which is a sitting and a milestone of its own.

**Carried forward, each one filed rather than remembered**: defect 024 with both
seats' reproducers; the ffi seat's request that a `tests/golden/run/` case be
added for the retained-pointer shape, which cannot be added while it is accepted
because it can carry no `#~` annotation; and the observation that
`examples/ledger/db/sqlite.hero` ships that shape today with a comment where a
rule belongs.

The chain's next entry is the instrument the budget is measured with, whose
ceiling landed inside this milestone and whose `--refresh` half has not.
