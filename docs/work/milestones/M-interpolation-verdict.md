# M-interpolation-verdict — the ruling on string interpolation *(closed 2026-09-09)*


**Scheduled by author instruction 2026-09-02**, and what it delivers is a
**decision**, not a feature: design.md Part 7 item 7 — *"String interpolation —
deferred; `print` takes multiple arguments"* — evaluated for the first time, against
the shape every other language spells `f"{name}"`, `$"{name}"`, `\(name)` or
`${name}`.

**Why it is admissible now, and was not before.** Part 7's preamble is categorical:
*"Nothing on this list is considered until the Principle 0 closure list (§1.0)
compiles itself"*, and CLAUDE.md §13 repeats it as a place not to go. That condition
has been met since M-selfhost-fixpoint, 2026-08-18. So the bar this item has never
been held to is Principle 0's own, and **it is not on the closure list** — the
compiler self-hosts today with none of it — which leaves the whole warrant to a
**measured Part 11 effect**. A sitting that cannot produce one closes with a
refusal, and that is a legitimate close for this milestone rather than a failure of
it.

**A refusal costs the same as a feature** (CLAUDE.md §12, author decision
2026-08-12): if the answer is no, what lands is a design.md Part 6 row naming the
program or the compiler fact that would make it wrong — not a second deferral. Part
7 item 7 has been one line with no argument since the day it was written, and this
milestone exists to end that either way.

**What the sitting must not re-derive: the trade was already made, in the other
direction.** `design.md:1403` — `print`'s variadic-looking form is *"compiler-known,
not a function value"* and *"was bought by trading away string interpolation (Part 7
item 7)"*, panel 006 (`docs/panel/006-map-order-print.md`), with Pascal's `WriteLn`
as the fifty-year precedent. The question is therefore not *may we add sugar*, it is
**may we buy back something already sold**, and whoever proposes it owes the other
side of that trade in spec tokens and says what `print` becomes afterwards.

**The whole repository says two things about interpolation and both are in
design.md** — measured 2026-09-02 with `grep -rn -i interpolat` over `docs/panel/`,
`docs/records/journal/` and `docs/measurements/`: **zero hits**. There is no ruling to read
forward from (CLAUDE.md §1), which is why this is a milestone and not a footnote.

**Corrected 2026-09-08 by step 1's own count,
`docs/measurements/021-what-would-stand-inside-a-hole.md`.** Every number in the
list below was re-measured and three of the four moved, which is expected on a
tree that grew by 3,626 lines. **What did not survive is the sentence that reads
them**: *"this compiler's own diagnostics being assembled by hand, and it is the
largest single body of evidence in the repository"* is false. The nine modules
that carry diagnostics hold **286** of the **2536** holes, 11.3%, while
`selfhost/emit`, `selfhost/print` and `selfhost/ir` hold **1241**, 48.9%. The
largest body of hand assembly is the code generator and the two program
printers, and the densest is `tests/harness/`, which these counts never read.
The sitting inherits the corrected reading: the thesis is its only available
warrant, and the corpus that warrant would have to be argued over is the
compiler's own emitter.

**What its absence costs today, every number measured 2026-09-02** (and
re-measured 2026-09-08 in 021, which is the current answer).

- **945** lines of `selfhost/` hold the sequence `" + ` — a string literal
  concatenated to something — and **146** hold a `to_str()` call. That is this
  compiler's own diagnostics being assembled by hand, and it is the largest single
  body of evidence in the repository.
- **118** `print(` calls in `examples/` carry a comma: the multi-argument form
  panel 006 bought instead.
- `examples/template/main.hero` (**235** lines) **already interpolates at run
  time**, and it is the witness the sitting must hold rather than imagine. It picks
  `{key}` with `{{` as the escape for a literal brace, refuses an unknown key
  instead of leaving the hole, and its module doc defends both choices in writing.
  A corpus program found a defect in exactly that escape rule
  ([029](journal/029-corpus-coverage.md): `{{name}}` was refused because the rule
  protected `{{` and not `}}`), which is the cheapest available demonstration that
  the rule is not free.
- The spec stands at **3685** tokens of a hard 4096, headroom **411**, spread 79
  (`heroes measure`, this session). Headroom exists; §1.2 still prices the addition
  against a named removal or a pre-registered prediction.

**The three questions, in the order they bite — the spelling is the last of them.**

1. **What may stand inside a hole**: a name, an expression, a call? A bare name is
   the cheapest rule to write and to lex, and it is the one that reads worst on the
   day somebody wants `{count + 1}`. **Answered by a count on 2026-09-08** (021):
   over 1461 chains, a bare-name rule admits **56.9%** of the 2536 holes and lets
   **48.7%** of the chains be rewritten whole; a rule admitting any postfix run, a
   call or an index, reaches **99.7%** and **99.5%**. The whole distance between
   the two is **seven holes**, six of them arithmetic and the seventh `???`, which
   the spec calls valid *anywhere*.
2. **How a value renders.** This half is already normative and costs nothing: every
   type has a canonical `to_str`, and design.md fixes `f64`'s as round-trip-exact
   rather than shortest. An interpolation that rendered differently would introduce
   a **second** rendering rule, which is the expensive answer.
3. **The spelling, and the escape it forces.** `spec:74` fixes the escape set, and
   panel 008 (`docs/panel/008-escape-sequences.md`, ratified 2026-08-04) reserved
   the backslash — *"any other character after `\` is a compile error"* — with R3
   stating that a new escape **reconvenes that panel**. So Swift's `\(name)` is not
   a free spelling: it reopens 008. And `{` already means a map in this language
   (`{K: V}`), so the brace spelling owes the doubling rule
   `examples/template/main.hero` already implements, or an argument against it.

**Full five seats, not the soundness lane** (`/panel`): the form has surface, a spec
token cost and at least one diagnostic class. The llm-ergonomist's seat is the one
that decides it, because the thesis is the only warrant available — and it receives
`spec/heroes-spec.md` and sample programs only, never this section.

**The cost of ordering it here is declared rather than discovered — and it moved.**
It stood at row 41, after the tools and before the two books, because a book is the
expensive consumer: a surface form that lands after M-guide-book rewrites chapters
in two languages, while one that lands after M-vscode-extension adds a rule to a
grammar file. On 2026-09-03 it moved to row 36, before M-core-packages, because the
packages and the framework are a larger consumer still — 51,788 lines of Heroes
in 178 modules is the size of the one body of that kind today, measured
2026-09-03, and a form that lands after them is a form they
were written without (§ Who scheduled what; `DESIGN-LOG.md:539`). The book's
argument holds and is now the second reason. CLAUDE.md §9
is the bill a new form arrives with — the formatter, every `--dump-<stage>` printer,
`heroes mutate`, the diagnostics that quote a program back, and `heroes measure`
where the form has spec text — and it was written the day `as` reached six consumers
one at a time, with the formatter silently deleting it from a working program.

*******************************************************************************
**OPEN: 0**

*******************************************************************************
