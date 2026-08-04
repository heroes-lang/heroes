# 003 — M3a: the resolver, or every name and what it means

Milestone M3a · 2026-08-04 · steps 1–2 · panel 015.

## 1. Goal

Names. One pass between the parser and the checker that answers four questions
before a single type exists (design.md Part 10 step 4): what does this name refer
to, is it defined at all, is anything declared twice or never read, and which
written type is which.

Surface: `heroes check <file.hero> [--dump-scopes]` — and the golden harness
moves onto it, which is where the frontend command's name stops moving (M3b–M3d
deepen `check` without renaming it).

Module born: `resolve/` in nine single-concern files — `builtins` (the names no
file declares), `top` (the order-free top level), `scope` (the stack, shadowing,
the read/write counts), `decls`, `stmts`, `exprs`, `types`, `errors`, and the
`mod.rs` that owns the pass's state. `printer/scopes.rs` renders it. 164 crate
tests (was 110), 21 golden `check/` cases (was 12): 5 adversarial, 4 bulk, 12
inherited unchanged.

The measurable outcome: **design.md's 317-line acceptance program resolves with
zero diagnostics** — 33 top-level names, 60 bindings, 293 resolved uses, of which
53 arrive through a dot.

## 2. What surprised — shapes and rules

- **A good diagnostic is the specification of a rule.** Four of panel 015's five
  decisions cost **zero** spec tokens, and the argument is not thrift: a rule
  whose violation ends in a compile error the model reads is *stated by that
  error*, since §4.17 already obliges it to carry what is needed. The one rule
  that had to change was the one whose violation was **silent** — a file's own
  `map` quietly replacing the built-in. Measured, the five rules written out cost
  +83 tokens to say what four messages say for free.

- **Two judges killed the same clause from opposite directions, and neither could
  see the other's evidence.** The judge that reads only the spec vetoed the
  silent override because the meaning of `xs.map(f)` would depend on a distant
  line; the judge that holds the token budget vetoed it because it is an
  *exception* to a rule the spec already states, and exceptions are what cost
  tokens. Differentiated inputs converging is the only kind of agreement that
  carries information.

- **A veto backed by a compiled counterexample beat sound reasoning.** Reading
  the spec alone — no methods, no overloading — one judge concluded that `x.f(y)`
  needs no types at all, and the logic is airtight. The judge holding the *code*
  found design.md:1153: §4.11 looks for a **field** first, and a field can hold a
  function value, so `h.cb(n)` is legal and unanswerable without the receiver's
  type. Its prototype rejected that file twice. The resolution neither proposed:
  report the unknown function only when **no record or variant in the file
  declares a field of that name** — type-free, never wrong about a legal program,
  and it still catches the misspelling that judge #1 actually made while writing
  Heroes from the spec.

- **The reserved list was wrong in the source document's own terms.** §1.11 and
  §4.20 put `range` among the functions *written in Heroes*, not the compiler
  forms; §4.20's inventory also carries `sort` and `.str()`, neither of which
  appears in the spec's built-in line — and the acceptance program **calls
  `.str()`**, because there is no other way to put an integer into a message.
  A list assembled from the spec alone would have been wrong twice, and the
  program that has never been compiled is what proved it.

- **The escape valve is load-bearing, and it already existed.** An unused-
  parameter error with no way to say "not yet" does not remove a silent wrong
  program: it *relocates* it into the signature, because the likeliest repair is
  deleting the parameter — measured by the judge that wrote the skeleton-first
  program the rule is aimed at. `_` never binds, which was already true, so the
  valve cost nothing: a parameter may be `_`, twice in one list if the C
  signature demands it.

- **"Read" and "write" are two questions, and the difference is one line of code
  and three language decisions.** Whether the initialiser is a write (no, or the
  rule sees no cell at all), whether a field write is a use (no — §4.10 has no
  aliases, so a write nobody reads back is unobservable, which closes the hole Go
  has left open since golang/go#20802), and whether a write *through an `@`
  parameter* is a use (yes — §4.8's copy-out is the whole observable effect of
  `reset = function: (@counts: {str: int})`).

- **Order-free top level is one pass, and it deletes a feature.** Collect every
  top-level name before walking a single body, and forward declarations are not
  something the language "does not need" — they are something it cannot express a
  use for. The appendix's four mutually recursive parser functions never come up.

- **The cheaper rule was also the more local one.** Java (JLS §6.4) and C#
  (CS0136) both make the enclosing-shadow error order-*insensitive*: a binding
  below can invalidate a binding above. Heroes checks sequentially, which costs no
  extra state and means nothing below the line you are writing can decide whether
  it compiles (§1.3). A departure from two well-sourced precedents, taken
  deliberately and recorded as such.

- **The dense array is a port decision, not a performance one.** `uses` is one
  entry per expression rather than a map keyed by index, because §4.10 makes the
  array Heroes' only indirection and `[Ref]` ports unchanged. The plan had said
  `BTreeMap<ExprId, Ref>`, which does not even compile — `ExprId` derives no
  `Ord` — and the judge holding the tree said so before a line was written.

## 3. What broke and why

- **The acceptance program stopped resolving**, at exactly two lines: it declares
  its own `map` and `fold`, and both names are in §4.20's inventory. Cause: the
  one-tier rule, working. Fix: design.md's appendix renames them `apply` and
  `reduce` (their doc comments already said "applies" and "reduces"), in its own
  commit citing the panel. This is the **third** rule the appendix has caught,
  after `.var` (panel 013) and `=> assert false` (panel 014), and the pattern is
  stable: it is the only test written with no implementation in mind.

- **The compiler's own suggestion produced a second diagnostic.** `total = 1`
  plus `print(totl)` reported the unknown name *and* an unused binding for
  `total` — but applying the offered rename reads `total`, so the second message
  was a consequence of the first. One typo, two errors, which this project treats
  as a defect (the lexer and parser hold the same invariant). Fix: the unused
  sweep stays quiet about any name the compiler has offered as a repair.

- **`point` was told the closest names were `Point`, `join`, `print`.** All three
  are within the allowed edit distance — `print` is *one* substitution from
  `point` — so the single-candidate certain fix degraded into a list of three.
  Fix: a difference in **case only** wins alone. The letters are already right,
  which is what makes it the one repair that can be handed over as certain even
  when three other names are one edit away.

- **`declare` grew to eight parameters** and clippy said so. Grouped into a
  `Binding` value with five named constructors (`param`, `bind`, `cell`,
  `loop_var`, `payload`), which reads better at the four call sites than the
  eight-argument form ever did.

- **A golden case reported two diagnostics for one mistake — and the case was
  wrong, not the compiler.** `not-mutable.hero` wrote `x = 1` / `x @ 2` and never
  read `x`, so it earned both `not_mutable` and `unused_binding`. Every name in
  that file now gets read, so each mistake produces exactly one message. A golden
  that shows a cascade teaches the cascade.

## 4. Left on the record

- **`+` on `str` is unwritable** per the spec's operator table, while §4.20 gives
  the runtime string concatenation and the appendix uses `"…" + name`. Found by
  the judge that had only the spec, which restructured a whole program to avoid
  it. Its own panel, at M3b, where the operator table becomes code.
- **`print`'s reservation has a counter-precedent**: PEP 3105 made `print` a
  function *so that* it could be replaced in one module, for output capture in
  tests. Heroes has `test` blocks and no capture mechanism. Recorded, not
  resolved.
- **`sort` is reserved and unspecified**: in §4.20's inventory, absent from the
  spec.
- **M7 needs an explicit C-name form for `extern`.** `raise` is C89
  `<signal.h>` and is *already* a Heroes error (the foreign-word registry), so
  name-level reservation collides with real C symbols eventually. Nim's `importc`
  has the escape, and §4.19 says Heroes took its surface from there.
- **Panel 010's repeated-`@` rule is now cheap.** The resolver computes the root
  binding of every place, which is that rule's entire input. It stays queued for
  M3c as decided, but the machinery landed here.
- **`Ref::Top(u32)` indexes one `Vec<Decl>`.** At M6 the tier-2 functions become
  real Heroes source in a prelude, and the index will need a tag or a documented
  concatenation.
