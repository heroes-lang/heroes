# Panel 149 — report of the spec-warden

Ceiling reached by grep at the start of the sitting, never from the brief:
`docs/design/design.md:255`, **10240 tokens, measured by `claude-opus-5`**. Every
number below is from a `heroes measure` run the seat performed in its own copy.
The frozen tree was never written: `diff -q` against the repository's spec is
clean. **`--refresh` was used thirteen times**, each on a candidate draft placed
at the spec path inside the copy, because `real` is a pinned record printed only
for `budgets.SPEC` (`selfhost/cli/measure.hero:106-116`) — a draft at any other
path gets no `real` row. Nothing here is an estimate and nothing is scaled from
the vendored ratio.

| | verdict |
|---|---|
| **R1 — what the rule SEES** | **adopt, at ZERO required spec tokens.** The document already states the duty. |
| **R2 — what the diagnostic SAYS** | **adopt, at zero spec tokens**, and this is where the sitting should spend. |
| **R3 — the multi-handle record** | **adopt with condition: refuse, +14 real**, and refuse the class measured rather than the class the brief describes. The permission form **waits** under Principle 0. |

## Which reading the text supports: the FIRST. The compiler has the bug

**The obligation is not in the sentence the second reading quotes. It is in the
last clause, and that clause's trigger is an ACT, not a type:**

> `borrows` says the call hands back one it keeps, and **where a group consumes a
> handle type every call handing one back says which it is.**

`pair_make(n: i64) -> Pair` where `Pair` holds `s: Slot` hands a `Slot` back: the
seat ran `slot_close(p.s)` and it compiled, ran, and the runtime counted it. **A
reader who answered *no, it did not hand me a Slot* is contradicted by their own
next line.**

The second reading rests on *"after a handle result or `@` out-parameter"* being
a restriction on which functions may carry the mark. **The document's own
production refutes that, in the same section:**

```
Member = "function" ident "(" [ CParam { "," CParam } ] ")"
           [ "->" Type [ "owned" ident ] [ "acquires" ident | "borrows" ] ] NEWLINE
```

`Type`, not *handle type*. The mark stands after any result the document admits,
and the seat measured that it works: `-> Pair acquires slot_close` checks, builds
and exits **0**; `-> Pair borrows` exits **0**. So the first sentence is
definitional, saying what the mark MEANS where a reader first meets it, and the
third states the duty. **Nothing has to be added for R1. Under CLAUDE.md § 12 the
compiler has the bug and the repair is free.**

## Every candidate, measured. `real` is the binding row

Baseline `spec/heroes-spec.md`: legacy 5862 / cl100k 5988 / max 5988 / **real
7974**. Headroom 2266, of which the FFI floor mortgages 60.

| draft | what it is | vendored max | **real** | Δ real |
|---|---|---|---|---|
| baseline | unchanged | 5988 | **7974** | 0 |
| `dsub` | R1 by SUBSTITUTION: "after a result or `@` out-parameter **reaching** a handle" | 5990 | **7978** | **+4** |
| `d3rm` | R3 refusal, merged, short | 5998 | **7988** | **+14** |
| `d3p` | R3 permission: prose + production `{ "acquires" ident }` | 6001 | **7991** | +17 |
| `dsubr` | `dsub` + short refusal | 5999 | **7991** | **+17** |
| `d1m` | R1 as a new merged clause | 6001 | **7993** | +19 |
| `dsubp` | `dsub` + permission | 6003 | **7995** | +21 |
| `dref_only` | refusal naming both positions | 6006 | **7998** | +24 |
| `dall` | `d1m` + short refusal | 6010 | **8006** | +32 |
| `d1a` | R1 APPENDED as its own sentence | 6013 | **8007** | +33 |
| `dallp` | `d1m` + permission | 6014 | **8010** | +36 |
| `d3ra` | refusal STANDALONE, carrying its reason | 6017 | **8011** | +37 |
| `dfinal` | `dsub` + refusal + an `@` clarifying sentence | 6020 | **8015** | +41 |

**No draft breaches anything**: the most expensive leaves 2225 under 10240. **No
veto on budget.**

**Merging beats appending, replicated a third time** (panel 122): the same R1
rule costs **+4** as a substitution, **+19** as a merged clause, **+33** as its
own sentence. The appended form is **8.25 times** the substitution.

## The price of the refusal, asked for by name

| form | Δ real |
|---|---|
| merged, short | **+14** |
| merged, naming both positions | +24 |
| standalone, carrying its own reason | +37 |

**Take the +14 and put the reason where it is free.** CLAUDE.md § 12 asks a
refusal to name the fact that would make it wrong in a **design.md Part 6 row**,
and design.md §1.6 says the spec has a budget and the compiler does not. Writing
the justification into the spec instead costs a measured **23 real tokens** for
nothing the reader of the prompt needs.

**The falsifier that row owes**, searched for rather than asserted absent: *name
a header whose function returns a by-value struct holding two owning handles of
different types.* The seat looked at sqlite3, libcurl, SDL and POSIX
(`getaddrinfo`, `passwd`, `stat`) and found none; C hands two resources back
through two out-parameters, which the language already writes. That is a
question, not a proof: the seat's vocabulary, not the world.

## The thing the brief did not know, and it moves R3

R3 is framed as *the record the surface cannot express*. **The class is wider,
and part of it is expressible today, spec-legal today, and silently wrong
today.**

Two `@` out-parameters each marked `acquires` — the shape the `CParam` production
admits and `examples/sqlite/main.hero` already uses one at a time:

| program | exit |
|---|---|
| two marks, both handles closed — **the CORRECT program** | **abort 134**, *"1 more handle(s) given back than were taken"* |
| two marks, one handle closed — **the LEAKING program** | **exit 0** |

**Cause**, read at `selfhost/emit/ops.hero:162`: the emitter asks
`handles.hands_a_handle_over(decls[e.decl])`, a per-DECLARATION question, and
writes **one** `hero_handle_acquired();` per call however many handles the call
hands over. **The counter counts CALLS, not HANDLES.**

The spec already says otherwise — *"which the program owes it. The owing is
counted"*, one owing per handle and not per call. So this too is a compiler bug
at zero spec cost, and **it must be settled BEFORE the refusal is worded**:
refusing *a result reaching two handle types* leaves the two-`@` case standing,
where the instrument rewards the leak.

*(Coordinator's note, 2026-09-14: this was re-run independently in the sitting's
own scratchpad with two distinct handle types, `Slot` and `Conn`, and reproduced
exactly — the correct program aborts 134 and the leaking one exits 0. Filed as
defect 034.)*

## Principle 0

**`needed_for_self_hosting`: no.** Measured: `selfhost/` declares four `extern`
groups (`cli/process.hero:37,49`, `cli/io.hero:34`, `emit/literal.hero:41`) and
**not one handle, not one `acquires`, `borrows` or `consumes`.** The closure list
does not want any of this. So each part must serve the thesis on its own measured
argument:

- **R1, R2 and the per-call counting defect add no form at all.** They are the
  document's existing rules enforced. Principle 0 does not gate them; CLAUDE.md
  § Precedence rank 3 carries them, since today a program that correctly closes
  its handles aborts and the one that leaks exits 0.
- **The R3 refusal adds no form either.** It subtracts one, and it buys a compile
  error in place of an abort nobody can read.
- **The R3 PERMISSION (+17 to +21 real) adds a form, the compiler does not need
  it, and no measured Part 11 effect has been offered. It waits.** Not a close
  call: it also needs the per-handle counting repair underneath it, so it buys a
  second mechanism on top of one the sitting has not yet fixed.

## The structured verdict

- **spec_token_delta, MEASURED.** 7974 → 7974 required (R1, R2, per-handle
  counting: zero). 7974 → **7988** if the R3 refusal lands merged. 7974 →
  **7991** if the panel also buys the +4 substitution that kills the second
  reading at its root. Ceiling 10240; worst measured draft 8015.
- **removal**: nothing comes out of the counted document. The one thing that does
  come out is the runtime panic's third-cause clause, 60-odd words at
  `runtime/parts/alloc.c`, naming a defect R1 abolishes — not spec text, so it
  pays nothing. Payment is therefore a registered prediction in panel 046's
  shape.
- **prediction (the payment).** At the close of the milestone that settles defect
  033, `heroes measure spec/heroes-spec.md --refresh` reads **at most 7991
  real**, and `check`, `run`, `emission`, `determinism` and `corpus` are green
  with **zero** existing programs newly refused —
  `tests/golden/run/fixedbugs-a-handle-in-a-record-field.hero` included, whose
  group consumes no handle type and therefore owes no mark. If the spec reads
  above 8015 the sitting bought something nobody here priced.
- **condition.** The seat withdraws its adoption of R3's refusal if the panel
  words it about records alone, because two marked `@` out-parameters miscount
  today and a refusal about results only leaves the language where the correct
  program aborts and the leaking one exits 0. Settle the per-handle counting
  first; then the refusal is honest at +14. And **any draft above +41 real is
  refused without a named removal from § 13 itself.**

```sh
./heroes measure spec/heroes-spec.md --refresh     # must read <= 7991 real
./heroes run tests/harness/main.hero -- ./heroes   # check run emission determinism corpus
```
