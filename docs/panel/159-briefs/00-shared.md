# Panel 159 — shared brief

**The question.** Three rules a reader of the specification had to guess at. All
three have an answer in the compiler, measured. **None of the three is written
in the document.** What does the document owe, and what does each sentence cost?

Convened 2026-09-16 at M-check-completeness, on the item panel 126's
llm-ergonomist filed out of three tasks written twice each. Full panel: every
answer here is a spec sentence.

## The three, with the compiler's answer measured 2026-09-16

| the question | the compiler | the document |
|---|---|---|
| is `sort` **ascending**? | **yes** — `1,2,3`; `apple,fig,pear`; `false,true`, run on all three orderable kinds | says only *"walks them in order"* |
| is `xs[i] @ v` accepted? | **yes**, exit 0, prints `99` | gives `m[k] @ v` and nothing for an array |
| may `main` be `-> ()?` | **no** — `error[main_returns]`: *"a program reports failure by what it prints, not by what it returns"* | says a file holds `function main()` and no more |

**So none of the three is a compiler defect.** All three are silences, and that
narrows this sitting from *what should the language do* to *what does the
document owe, and at what price*.

## Why the seat that found them could not answer them

Its only input is `spec/heroes-spec.md`, which is the point of that seat and the
reason these stayed open for five days. It wrote six programs and reported:

- **`sort`'s direction is the one that matters.** Neither version of the
  specification says ascending; both say only *walks them in order*. A tie-break
  written on the wrong assumption **compiles and prints a silently different
  answer**. That was the single silent-error risk the seat found in six
  programs, and it costs one word.
- **`xs[i] @ v` is given nowhere** while `m[k] @ v` is given, so **every sort the
  seat wrote carried a map of taken keys instead of swapping two elements** — a
  reader building elaborate machinery around a form the language already has.
- **Whether `main` may be `-> ()?`** decides whether `?` is usable in the one
  function every program has.

## What the sitting is asked to decide

- **R1.** Does the document state `sort`'s direction, and in what words? It is
  § 11's, where `sort` lives, and `spec-shape.md` says a rule's home is the
  section of the operation it governs.
- **R2.** Does it state `xs[i] @ v`? § 10 gives `m[k] @ v` beside it, and § 5's
  `Place` production already derives `ident { "." ident | "[" Expression "]" }`
  — so the GRAMMAR may already permit what the prose does not teach. Check that
  before pricing a sentence.
- **R3.** Does it state that `main` takes no result? `error[main_returns]`
  already explains it well; a diagnostic is not the document, but §1.6 asks
  whether a sentence buys anything a reader does not already get on first
  contact with the compiler.
- **R4.** Three sentences or one? `spec-shape.md` says **merging beats
  appending** (panel 122, three drafts measured, the merged one cheapest), and
  each of these has a home sentence it could join.

## The budget, measured 2026-09-16

```
claude-legacy  5863 · cl100k_base 5989 · real 7974 (claude-opus-5)
ceiling 10240 · headroom 2266, 2206 net of the FFI floor
```

design.md §1.6's payment rule is unconditional: an addition owes a named removal
or a registered falsifiable prediction naming an instrument that exists.

## What this sitting must not do

**`spec/heroes-spec.md` is the one document a reader is told is the whole
language**, and `.claude/rules/spec-shape.md` binds every change to it: one rule,
one home; a number, once assigned, never changes; the `Built-ins:` sentence is
one sentence ending at `range`; the characters above ASCII are a closed set; and
an example in a fence is compiled before it is written.

**A change here publishes the site**, because `.github/workflows/deploy-site.yml`
fires on any push touching `spec/`. The author has authorised that in advance for
this work.

## Process rules binding every seat

- **Write your report file FIRST**, then improve it. No seat has been lost to the
  watchdog in three sittings under this rule.
- **No command over ~60 seconds.** Never run the full net.
- **Every number, path and count you report is produced by a command you run**
  (CL-077). Seven claims in this coordinator's briefs have been corrected by
  seats across four sittings; assume this one carries an eighth and look for it.
- Anything you cannot run is written as **UNRUN**, naming the command that would
  settle it.
- **Build in a copy**; never read `archive/bootstrap-rs/`; capture exit codes
  directly, never through a pipe.
