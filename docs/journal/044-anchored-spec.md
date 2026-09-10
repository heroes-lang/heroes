# 044 — M-anchored-spec

Closed 2026-09-11, **untagged**: defect 025 was open when the work finished, and
CLAUDE.md § Verification tags a milestone only over a clean list. One sitting,
panel 126, five seats over two evenings, and one check the author asked for that
no panel would have thought to run.

## Goal

The author read the document that is the whole language and said it looked
disorganised: every rule was there, and each stood where it had been dropped.
The measurement behind that impression, taken before a line was written: **74
commits on `spec/heroes-spec.md` and not one heading ever added, removed,
renamed or moved.** The eleven sections were v0's, from 2026-08-03, and every
rule since had landed where it happened to fall. So `range` stood in two
sections and `to_<type>` in two, `print` was described twice, `_` was explained
half in Bindings and half in Control flow, ten of the places a program aborts
were spread over six sections, and *abort* itself was defined inside § Types.

**What was decided, and by whom.** The author allowed up to a tenth more tokens
to buy the form (5916 real as the ceiling of the operation, against a language
ceiling of 6144), asked that the work pay homage to the tradition of the
language Reports, asked that blind readers confirm the new text says the same
thing as the old before it is called the specification, asked that the form
become a written rule with an executor, and asked that the site's specification
page become copyable in one click. All five landed.

## What surprised

**The tidying is lighter than the mess.** The pure move, thirteen numbers and
two new homes, cost **+51** real tokens; the twenty merges that gave each rule
one home gave back **−71**. A document that says a thing once is smaller than
one that says it twice, and this project had measured that before, at panel 035
(−12) and panel 090 (−5), without ever stating it as a rule.

**One line-number citation in seventy-six pointed at its own sentence.** The
compiler seat opened every `spec:NNN` and `spec line N` in `selfhost/`, `tests/`
and `runtime/`, read what it claimed, and found the sentence: `probe.hero:23`
was right and the other seventy-five were off by five to thirty lines. CL-037
had said a line number is the one citation shape no instrument can see; the
number 75 is what that costs. All of them are now `spec § N <title>`, and a
check reads them.

**A check that could not exist for lines exists for sections, and it found real
defects on its first run.** `spec/anchors` walks `selfhost/` and `tests/` and
asks of every `spec § …` whether it names a section that exists. Three modules
cited a `§ Values` the spec has never had, in any version; `cli/io.hero` cited a
`§ Built-ins` that this milestone is what makes true; and one comment in the
harness cited a `§ Rule 3` of its own file. None of the five was a language
defect, and none had ever been visible.

**Every Report puts the vocabulary before the types, and the draft had frozen
the reverse.** The historian read Naur's 1960 Algol Report page by page, Wirth's
Pascal, Modula-2 and Oberon reports, and the Scheme reports R5RS to R7RS, and
found the order uniform: basic symbols, identifiers, numbers and strings come
before types and declarations in every one of them, with R6RS the single
exception that R7RS did not repeat. The draft had Types at 2 and Literals at 3,
under numbers meant never to change. They were swapped the evening before the
numbers set. The same reading found the anchoring precedent: the 1960 Report's
index says *all references are given through section numbers*, and the 1963
Revised Report kept every one of them.

**`heroes test` runs each test in its own process, so a panic does not stop the
run.** The proposal had bought a clause reading *nothing catches one* for six
tokens; the compiler seat wrote a two-test program, saw `panic: array index out
of range` followed by `FAIL "aborts"` and then `ok "passes"`, and pointed out
that the one place a reader watches an abort happen shows it apparently caught.
The clause reads *no `T?` carries one* instead, which is the fact
`docs/measurements/022` wanted stated and the one the checker enforces.

**The compiler enforced a rule the prompt never stated.** The ffi seat wrote a
`getenv` binding from the new § 13 alone, hesitated over a null `cstr`, and
measured the runtime's answer: `fail("null_cstr", "a null cstr holds no text")`.
Neither text said it. A reader who guessed *aborts* writes a needless guard; one
who guesses `.must()` is safe ships a program that dies when the variable is
unset. The clause is in, at four tokens, and `suite_spec.hero`'s own header had
already named this class: a rule the compiler enforces and the spec omits is not
terseness, it is a wrong briefing.

**Six blind readers, and the criterion met from both sides.** Four inventoried
one text each, one line per rule, 1166 rules in all; two read both and listed
what one says and the other does not. Reader D, holding the current text as
`one`, found nothing of it absent from the proposed one. Reader C, holding the
proposed text as `one`, found nothing of the current one absent either. The 73
quotes that did not match verbatim were read by hand and every one is a named
merge or a code line the reader joined. The one difference of meaning the
readers found is in the **current** text: the gloss *(which does not)* after
`args_checked()` sits two clauses away from the *aborts* it negates, and the
re-shaping repaired a sentence nobody had reported. `docs/measurements/028`
carries all of it.

## What broke and why

- **Four of the five judges died on a session limit** before writing a line, and
  were reconvened under a new account. The historian, whose tools are Read and
  the web, could not write its own verdict file at all: the coordinator saved it
  verbatim, and said so at the top of the file.
- **The citation script stopped on a line number that had drifted by seven**,
  because the file it named, `suite_records.hero`, had grown under this very
  milestone while the seat was measuring it. The second run looks for the
  pattern within twelve lines of the number given and leaves an already
  converted line alone.
- **`records/table` counts the alias table in § The names, not the chain.** A
  new milestone needs a row in both, and the count in `suite_records.hero` moves
  by hand in the same commit, which is that constant's own rule.
- **`records/citations` reads `spec/shape` as a file under `spec/`.** The rule
  file names the checks without their prefix, and says why in a line of its own.
- **The instrument could not judge its own source.** `spec/anchors` reads
  `suite_spec.hero`'s test cases as citations, since they are citations, written
  to be wrong. It skips that one file, named, and the check that would catch a
  real defect there is the file's own tests.
- **A comment in an intestation moved a golden's emitted C.** Five lines added
  at the top of `tests/golden/run/spec-the-documents-own-example.hero` moved
  every `#line` in its blessed emission trace and turned `emission` red. The
  note is at the end of the file now, and says why it is there.

## What landed, and what carried forward

**The document.** 289 lines, thirteen numbered sections, **5598** real tokens on
`claude-opus-5` through `count_tokens`, digest `319c66850e1586bb`, **4410** on
the vendored ranks; the ceiling is 6144 and **486** are free net of the FFI
floor. The three movements: the move **+50** vendored, the twenty merges
**−59**, the eight additions **+209**. The two additions the sitting's own
experiments produced are the cheapest of the eight: a null `cstr` through
`validated()` at +4, from the ffi seat's `getenv` binding, and a variant's case
at +17, from the ergonomist's two lexers failing at the same line.

**The instruments.** `spec/shape` reads the thirteen headings and refuses a
renumbering; `spec/anchors` asks of every `spec § …` in `selfhost/` and `tests/`
whether it names a section that exists. Both were seen **red** on a tampered
copy before they were believed green, and both are green on the tree: the spec
suite is **13 passed, 0 failed**, where it was seven checks before this
milestone. `.claude/rules/spec-shape.md` is what they enforce, and its pointer
is one row of CLAUDE.md § Where the rest lives, which took the contract from 21
tokens of headroom to 21 after a sentence was shortened to pay for it.

**The citations.** 81 in living code, from panel 126's own tables: 55 `spec:N`,
21 `spec line N` in `selfhost/`, `tests/` and `runtime/`, and 5 by heading name
that the moves made stale. In `tests/golden/`, a record, the old line stays and
a dated note at the end of the file gives the section. The 678 in the
append-only records were not touched and read against the layout at `834d804f`,
which is what CLAUDE.md §14 asks and what CL-037 already knew.

**The site.** The specification page copies the whole text on one click, and one
click selects it whole where scripts do not run. The rule it lands under is new:
JavaScript is never the only way, with `site/src/lib/scripts.ts` reading every
page the build writes to enforce it. The copied bytes were compared with the
file by `cmp`, zero differences, and both editions carry two sentences on the
Report lineage the historian verified.

**Not tagged, and the reason is a measurement.** The ergonomist's first
prediction was falsified in a way that found a real discrepancy: `fail("a", "b")`
and `xs.slice(1, 3)` are accepted, `range(1, 4)` is refused, and `spec § 9`
states the label rule without exception. That is **defect 025**, filed rather
than repaired, because a diagnostic class is a panel path of its own. CLAUDE.md
§ Verification tags a milestone only over a clean list, so this one closes
untagged, on `M-cstr-lifetime`'s precedent, and the tag waits on the defect or
on the author's word.

**What carried forward.** Defect 025. `panel 126` in `docs/work/DECIDE.md`, whose
one question is whether the two worked examples stay. Three holes the ergonomist
found in **both** texts, filed at M-check-completeness: `sort`'s direction, which
is the one place in six programs where a plausible mistake compiles and prints a
different answer, `xs[i] @ v`, and whether `main` may be fallible. And the
journal index, which stops at 035 while 45 journals exist, filed at
M-journey-book.

### The chain entry

| 45 | **M-anchored-spec** | done 2026-09-11 | untagged, defect 025 open | [044](journal/044-anchored-spec.md) | the specification takes the shape of a Report: thirteen numbered sections in reference order, one home per rule, the numbers as the citation anchors, two checks that keep the form, 81 citations converted, and the site's page copying the text in one click · **§1.6** |
