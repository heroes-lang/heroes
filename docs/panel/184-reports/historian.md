# Panel 184, historian's report

Seat: historian (advisory, no veto). Written as it went, 2026-09-30, from the
shared brief `docs/panel/184-briefs/00-shared.md` and the seat's brief
`docs/panel/184-briefs/historian.md`. In the repository this seat read only
those two files and the two panel files its brief cites by path
(`docs/panel/121-the-brace-was-already-taken.md`,
`docs/panel/107-the-number-cannot-be-uniform-the-abort-can.md`).

**Method.** Every precedent below was fetched or searched in this session, and
each carries the address it came from, its date or version, and the words it
says, quoted. A search that found nothing is reported as the words searched and
where. A claim this seat could not confirm at a source says `unverified` beside
it. The project author's book was not used as a source for any row.

**One caveat on the quotes, stated once.** Pages were read through the session's
fetch tool, which returns a page as seen by a small model asked to quote
verbatim. Where a quote below came from a project's own source file (raw
GitHub), from GitHub's API or from a PDF read directly, it is the document's
text; where it came from a rendered page, it is that page's text as the tool
returned it. A reader who needs a quote to the letter re-fetches the address
given. **The caveat bit once, and the quote was thrown out**: a first fetch of
JLS SE 21 chapter 14 returned a "verbatim" §14.22, and a second fetch of the
same page showed that the text the tool received ended in §14.14.2. So that
§14.22 was the tool's invention. Every JLS sentence in Q2 comes instead from
the JLS 1.0 text, where the tool confirmed the section was in what it read.

**Interruptions, recorded.** The session was stopped twice, neither time by a
person: once by an API session limit (HTTP 429) at about 17:10, while fetching
the Scala, RuboCop and ShellCheck pages (resumed at the author's instruction,
17:22), and once by the stream watchdog at about 18:20, after the Wirth PDFs
had been read and before Q2 was written (resumed at the author's instruction,
19:52). Every row below was fetched in this session, before or after the
stops. The file-editing tool is disabled in this session, so each correction
was made by rewriting the file whole; two sentences of the first Q1 draft were
corrected that way within minutes of being written (the Clippy group, and a
claim that no ecosystem leaves the mistake silent).

**Nothing of Heroes was run by this seat.** Where a sentence below says what a
precedent predicts for Heroes, it is a prediction, and where it reads a Heroes
fact it quotes the shared brief's measurement.

Status: all three questions written, and the closing block is at the end.

## Question 1: a literal whose `f` was forgotten

### Precedents

**P1.1 Ruff, RUF027 `missing-f-string-syntax` (Python). Verified.**
<https://docs.astral.sh/ruff/rules/missing-f-string-syntax/> and the rule's
source,
<https://raw.githubusercontent.com/astral-sh/ruff/main/crates/ruff_linter/src/rules/ruff/rules/missing_fstring_syntax.rs>,
both fetched 2026-09-30. *"Searches for strings that look like they were meant
to be f-strings, but are missing an `f` prefix."* The source's metadata:
`#[violation_metadata(preview_since = "v0.2.1", category =
Category::Suspicious)]`, and the page: *"This rule is unstable and in preview.
The `--preview` flag is required for use."* So **a warning, and off unless
preview is asked for**. **It decides by scope**: the source asks the semantic
model whether each name resolves at the literal
(`semantic.simulate_runtime_load_at_location_in_scope(...)`, a builtin not
counting). And it skips, by its own list: docstrings and any standalone string;
a string in a call *"with argument names that match at least one variable"*; a
string that *"(or a parent expression of the string) has a direct method call
on it"*; one with no hole or with *"invalid f-string syntax"*; one whose names
are *"not in scope"*; invalid format specifiers; and a call *"known to expect a
template string"* (logging, gettext, FastAPI paths). Its fix: *"the fix is
always marked as unsafe"*. **Why it has stayed in preview**, from
<https://github.com/astral-sh/ruff/pull/15247> (a pull request to recognise more
expressions, opened 2025-01-04, closed unmerged), AlexWaygood on 2025-01-04:
*"RUF027 already has too many false positives for us to consider stabilising
it. I definitely don't think we should be making changes that introduce new
false positives here."* And the other side, an open issue's title,
<https://github.com/astral-sh/ruff/issues/15227>: *"[RUF027]
missing-f-string-syntax has false negatives if calling a function inside
brackets or accessing attribute of interpolated variable"*.

**P1.2 ESLint, `no-template-curly-in-string` (JavaScript). Verified.**
<https://eslint.org/docs/latest/rules/no-template-curly-in-string>, fetched
2026-09-30: *"Disallow template literal placeholder syntax in regular
strings"*; *"It can be easy to use the wrong quotes when wanting to use template
literals, by writing `"${variable}"`, and end up with the literal value
`"${variable}"` instead of a string containing the value of the injected
expressions."* Introduced in ESLint **v3.3.0, released 12 August 2016**
(<https://eslint.org/blog/2016/08/eslint-v3.3.0-released/>: *"New:
`no-template-curly-in-string` rule (fixes #6186) (#6767) (Jeroen Engels)"*).
The rule's source,
<https://raw.githubusercontent.com/eslint/eslint/main/lib/rules/no-template-curly-in-string.js>:
`type: "problem"`, `recommended: false`, message *"Unexpected template string
expression."*, and the whole decision is one regular expression,
`/\$\{[^}]+\}/u`. **Shape alone, no scope, not in the recommended set.** Its
only stated limit: *"This rule should not be used in ES3/5 environments."* No
false alarms and no per-string escape are documented.

**P1.3 Clippy, `literal_string_with_formatting_args` (Rust, the same brace
spelling as Heroes). Verified.** The source,
<https://raw.githubusercontent.com/rust-lang/rust-clippy/master/clippy_lints/src/literal_string_with_formatting_args.rs>:
*"Checks if string literals have formatting arguments outside of macros using
them (like `format!`)."*; *"It will likely not generate the expected
content."*; `#[clippy::version = "1.85.0"]`, category `nursery`. **Shape and
scope**: it parses the literal as a format string, then drops a hole whose name
is not a local of the function, read from MIR (`mir.var_debug_info ...
local.name.as_str() == name`). Its history:

- <https://github.com/rust-lang/rust-clippy/pull/13410>, opened 2024-09-17,
  merged 2024-12-01, changelog line *"Added new
  [`literal_string_with_formatting_args`] `pedantic` lint #13410"*.
- <https://github.com/rust-lang/rust-clippy/pull/14014>, *"change
  `literal_string_with_formatting_args` lint category to nursery"*, opened
  2025-01-17, merged 2025-01-19: *"thousands of false positives on GitHub"*,
  the example being `.replace("{var}", var)`; changelog *"change category to
  `nursery` from `suspicious`"*.
- <https://github.com/rust-lang/rust/pull/136982>, *"[beta] Clippy beta
  backport"*, opened 2025-02-13 against the 1.85 beta, carrying that move
  because the lint had *"too many false positives"* to reach stable. Rust 1.85.0
  shipped 2025-02-20 (<https://blog.rust-lang.org/2025/02/20/Rust-1.85.0/>).
- The groups, from <https://raw.githubusercontent.com/rust-lang/rust-clippy/master/README.md>:
  `suspicious`, *"code that is most likely wrong or useless"*, **warn**;
  `pedantic`, *"lints which are rather strict or have occasional false
  positives"*, allow; `nursery`, *"new lints that are still under
  development"*, allow.

**A correction to the coordinator's resume message**, which says the lint moved
*from `pedantic` to `nursery`*: PR 14014's changelog and PR 136982 both say
**from `suspicious`**, the warn-by-default group; PR 13410's description says
`pedantic`. The sources disagree on the group it merged in; the two later ones
agree it was `suspicious` when it was moved. **A scope-aware check for this
mistake, in the brace spelling Heroes uses, was in the warn-by-default group
when it was moved, seven weeks after it merged, and was pulled to
off-by-default before its first stable release, on thousands of false
alarms.** Whether it sat in `suspicious` from the day it merged is not settled
by these three sources.

**P1.4 Pylint (Python). Verified, and it never shipped.** Issue
<https://github.com/PyCQA/pylint/issues/2507>, *"Detect when f-string-syntax is
used in a string, but not marked as an f-string"*, opened 2018-09-21, still
open, labelled Blocked and High priority. Pull request
<https://github.com/pylint-dev/pylint/pull/4787>, *"Add
`possible-forgotten-f-prefix` checker"*, DanielNoord, opened 2021-08-02, closed
unmerged 2022-04-01; it checked whether the text between braces named
variables, and its author closed it with *"The logic worked (sort of) but there
were just too many false positives and edge cases"*.

**P1.5 Scala, `-Xlint:missing-interpolator` (in the compiler). Verified.**
<https://docs.scala-lang.org/overviews/compiler-options/index.html>: *"A string
literal appears to be missing an interpolator id."* The warning's text, from
<https://github.com/scala/bug/issues/8761> (2014-07-30): *"possible missing
interpolator: detected interpolated identifier `$foo`"*. **It decides by
scope**: the compiler's own test,
<https://raw.githubusercontent.com/scala/scala/2.13.x/test/files/neg/forgot-interpolator.scala>,
runs under `-Werror -Xlint:missing-interpolator` and expects a warning where the
name after `$` is a term in scope, and none for a non-existent symbol, a
package, a type, or a method that needs an argument; its `.check` file lists 23
warnings. Its history: <https://github.com/scala/bug/issues/8525>, *"Put missing
interpolator warning behind a flag"*, opened 2014-04-23: *"Some templating
applications will naturally trigger false positives and working around that is
annoying."* The fix, <https://github.com/scala/scala/pull/3792> (som-snytt,
opened 2014-05-25, milestone 2.11.2): *"add a lint-only warning for the
infamously nagging 'Did you forget the interpolator?'"*. Scala 2.11.2 shipped
2014-07-24 (<https://www.scala-lang.org/news/2.11.2/>). False alarms kept
arriving afterwards: <https://github.com/scala/bug/issues/9127> (2015-01-31,
2.11.5, braces and whitespace) and <https://github.com/scala/bug/issues/10217>
(2017-03-02, an escaped `\\$T`). **Whether it was on by default before 2.11.2
is unverified**: the pull request says it made the warning lint-only, and no
page fetched says what it was before.

**P1.6 RuboCop, `Lint/InterpolationCheck` (Ruby). Verified.**
<https://docs.rubocop.org/rubocop/latest/cops_lint.html>: *"Checks for
interpolation in a single quoted string."*; enabled by default **Yes**;
autocorrection *"Always (Unsafe)"*; added 0.50, changed 1.40. Its safety note:
*"This cop's autocorrection is unsafe because although it always replaces
single quotes as if it were miswritten double quotes, it is not always the
case."* Pull request <https://github.com/rubocop/rubocop/pull/4480>, opened
2017-06-08, merged 2017-08-25; a reviewer in it: *"This cop should probably not
check regular heredocs."* **Shape alone** (`#{`), and **on by default**, the only
default-on check of this family found.

**P1.7 CodeNarc, `GStringExpressionWithinString` (Groovy). Verified.**
<https://codenarc.org/codenarc-rules-groovyism.html>: since CodeNarc 0.19,
*"Check for regular (single quote) strings containing a GString-type expression
(${..})."* Its own examples mark `'abc {123}'` as **ok**: a brace without the
`$` is not flagged. Whether it is on by default: unverified.

**P1.8 ShellCheck, SC2016 (shell). Verified.**
<https://www.shellcheck.net/wiki/SC2016>: *"Expressions don't expand in single
quotes, use double quotes for that."* A script that means the `$` says so with
`# shellcheck disable=SC2016`; the page gives three such cases (a literal
`$PATH` written to a file, `PS4`, `envsubst`). Severity: not stated on the page.

**P1.9 Go, `vet`'s printf check, and `go test` runs it. Verified.**
<https://pkg.go.dev/golang.org/x/tools/go/analysis/passes/printf>: *"The checker
also uses a heuristic to report calls to Print-like functions that appear to
have been intended for their Printf-like counterpart: log.Print("%d", 123) //
log.Print call has possible formatting directive %d"*. And
<https://pkg.go.dev/cmd/go>: *"As part of building a test binary, go test runs
go vet ... If go vet finds any problems, go test reports those and does not run
the test binary. Only a high-confidence subset of the default go vet checks are
used. That subset is: atomic, bools, buildtag, directive, errorsas,
ifaceassert, nilfunc, printf, stdversion, stringintconv, and tests."* **The
nearest thing to an error in this family**: a shape check, on `%`, that stops
the test binary by default. `-vet=off` turns it off.

**P1.10 Rust 2021, `panic!`: a plain position made a format position, and the
brace refused there. Verified.** Pull request
<https://github.com/rust-lang/rust/pull/78088>, *"Add lint for panic!("{}")"*,
m-ou-se, opened 2020-10-18, merged 2020-11-20, shipped in Rust 1.50.0
(2021-02-11, <https://blog.rust-lang.org/2021/02/11/Rust-1.50.0/>). The lint's
source,
<https://raw.githubusercontent.com/rust-lang/rust/master/compiler/rustc_lint/src/non_fmt_panic.rs>:
*"In Rust 2018 and earlier, `panic!(x)` directly uses `x` as the message. That
means that `panic!("{}")` panics with the message `"{}"` instead of using it as
a formatting string ... Rust 2021 always interprets the first argument as format
string."* The edition guide,
<https://doc.rust-lang.org/edition-guide/rust-2021/panic-macro-consistency.html>:
*"`panic!("{")` is no longer accepted, without escaping the `{` as `{{`"*, and
*"The `non_fmt_panics` lint has been a warning by default on all editions since
the 1.50 release."* The 2021 edition shipped with Rust 1.56.0 on 2021-10-21
(<https://blog.rust-lang.org/2021/10/21/Rust-1.56.0/>). **The one precedent
where a language took a place a literal was plain text and made an unescaped
brace a compile error there**, with `{{` as the way to mean a brace, and it did
it at an edition boundary, after a warn-by-default lint had run for eight months.

**P1.11 Java: the only outright refusal found, and it refused nothing a
program could have held. Verified.** JLS SE 21 §3.10.7,
<https://docs.oracle.com/javase/specs/jls/se21/html/jls-3.html>: *"It is a
compile-time error if the character following a backslash in an escape sequence
is not a LineTerminator or an ASCII b, s, t, n, f, r, ", ', \, 0, 1, 2, 3, 4,
5, 6, or 7."* So `\{` in any plain literal was already an error. JEP 430,
<https://openjdk.org/jeps/430> (JDK 21, preview), on why the hole is `\{`:
*"we considered using `${...}`, but that would require a tag on string
templates (either a prefix or a delimiter other than `"`) to avoid conflicts
with legacy code."* JEP 459, <https://openjdk.org/jeps/459> (JDK 22, preview):
*"if we forget to use a template processor such as `STR`, `RAW`, or `FMT` then
a compile-time error is reported: `error: processor missing from template
expression`"*. Withdrawn: Gavin Bierman, 2024-04-05,
<https://mail.openjdk.org/pipermail/amber-spec-experts/2024-April/004106.html>:
*"there will be no string template feature, even with --enable-preview, in JDK
23"*; the reason given there is the processors, not the delimiter.

**P1.12 rustc's `unused_variables`: route (1c), shipped. Verified.** Issue
<https://github.com/rust-lang/rust/issues/100584>, *"Detect likely attempts to
use interpolation in string literals"*, estebank, opened 2022-08-15, closed
2023-09-29 (GitHub's timeline API). Pull request
<https://github.com/rust-lang/rust/pull/100941>, *"Point at the string inside
literal and mention if we need string interpolation"*, lyming2007, opened
2022-08-24, merged 2022-08-30, milestone 1.65.0 (released 2022-11-03,
<https://blog.rust-lang.org/2022/11/03/Rust-1.65.0/>). Its diff
(<https://api.github.com/repos/rust-lang/rust/pulls/100941/files>) adds to
`compiler/rustc_passes/src/liveness.rs` the texts *"you might have meant to use
string interpolation in this string literal"* and *"string interpolation only
works in `format!` invocations"*, with the test
`src/test/ui/type/issue-100584.stderr`; that test is still on rustc's master
today as `tests/ui/type/issue-100584.stderr`, fetched 2026-09-30. **The unused
variable's warning points at the literal that names it in braces, which is
route (1c) word for word, and it has stood for four years.**

**Searched and not found.** (a) A C# compiler or analyzer warning for a missing
`$`: web search *C# Roslyn analyzer string literal contains braces "missing $"
interpolated string warning*; the results were Roslyn issues about the opposite
case and about brace escaping, none a check for a missing `$`. (b) A language
that refuses, as a compile error, a plain literal whose hole spelling is
ordinary text: web search *language compile error string literal "looks like"
interpolation without prefix refused error not warning "missing interpolator"
OR "missing prefix"*; nothing. Both are the searcher's vocabulary, not the
world's (CLAUDE.md § RUN IT): a question, not a finding of absence.

### What the precedents say together

1. **Every shape-only check keys on a sigil** (`${`, `#{`, `$`, `%`), never on a
   bare `{`. CodeNarc's own examples mark `'abc {123}'` as fine.
2. **Every check on a bare `{` added scope, and none of the three reached a
   default-on stable release**: Clippy pulled before stable, Ruff in preview
   with *"too many false positives"*, Pylint's closed unmerged. Scala's, on `$`
   with scope, is in the compiler and lint-only, *"infamously nagging"*.
3. **The false alarms they name are strings that something else reads
   later**: `.replace("{var}", var)` (Clippy), `.format()` templates (Pylint),
   logging, gettext, FastAPI paths (Ruff's exclusions), *"templating
   applications"* (Scala), heredocs (RuboCop), docstrings. **Scope does not
   discriminate these**: in `.replace("{var}", var)` the name is in scope by
   construction.
4. **The precautions cost exactly the silent sites the brief shows.** Reading
   Ruff's documented rules against the brief's two quoted sites, not running
   Ruff: `"row-{at}-payload".lease()` has *"a direct method call on it"*, which
   Ruff skips, and `{kept_label_length()}` is a call inside the braces, which
   Ruff's open issue 15227 says it misses. A reading, unrun.
5. **The two compile errors are both by construction.** Java refuses because its
   hole opener was already illegal in every literal, so no program's text could
   be refused. Rust 2021 refuses an unescaped `{` in a place it redefined as a
   format position, at an edition, with `{{`.
6. **The message route has shipped**: rustc since 1.65.

### Verdict per route (advisory)

- **(1a) the shape: approve, as a deliberate departure.** No language found
  refuses a bare-brace plain literal, and every bare-brace lint pulled back.
  But what they pulled back on is documented, and it is strings that another
  mechanism fills later. Heroes has no macros (CLAUDE.md § 6) and no standard
  library (§ 13); whether its programs build templates of their own is a count,
  not a precedent. Rust 2021 is the precedent for the `{{` price being paid.
- **(1b) shape and scope: object.** In three ecosystems scope was the remedy
  for false alarms, and none of them shipped it on by default. Scope cannot see
  a template whose name is in scope by construction, and it makes a literal's
  legality depend on the code around it.
- **(1c) the message: approve**, whatever else lands. rustc has done it since
  2022, and nobody has reverted it. It reaches the brief's 6, not its 17.
- **(1d) today: object.** Not because every ecosystem is loud: by default
  Python (Ruff preview, Pylint nothing), JavaScript (ESLint's rule not
  recommended), Scala (`-Xlint` asked for) and, as far as the search found, C#
  say nothing. But the compiler with Heroes' own brace spelling, rustc, changed
  exactly the message Heroes prints today: its unused-variable warning stopped
  saying only *unused* and started pointing at the literal. Heroes' message
  today is *remove the binding, or read it*, and the brief measured that
  following it produces a program that checks and prints `{count} rows`
  (task 1). A message whose advice completes the mistake is the thing rustc
  repaired.
- **A route nobody listed, which precedent names: (1e) a hole spelling that is
  illegal in a plain literal**, so the gate cannot be forgotten. Java did this
  with `\{`, and Swift has `\(` active in every literal. Panel 121 recorded
  `\(e)` as a live branch (`error[unknown_escape]` today, 0 occurrences,
  `docs/panel/121-the-brace-was-already-taken.md:236-244`). It reopens panel
  121 R3, ratified, and panel 008 R3. It is named here as the one route under
  which the class does not exist, not as this seat's recommendation.

**Argument** (120 words or fewer). The record is uniform that a bare-brace
check produces false alarms at scale. Clippy pulled one before stable over
thousands of them, Ruff has kept one in preview, and Pylint's was never merged.
The record is just as clear about why: the alarms are strings that `.format`,
`.replace`, logging, gettext or a templating engine fills later. Adding scope
did not remove them. So (1b) takes on the precedents' cure without the reason
the cure was needed, and adds non-locality. (1a) departs from precedent, and
that departure is sound only if Heroes lacks the thing that sank the others. A
census can check that, and no precedent can. (1c) has quietly worked in rustc
since 2022.

**Prediction** (falsifiable). Suppose (1a)'s predicate is run over the tracked
tree, a plain literal holding a `{` whose text up to its `}` parses as a hole.
Then the hits that are not forgotten `f`s will mostly be one of two kinds:
literals whose braces something else reads later, or text that shows Heroes
source, such as the compiler's own messages and fixtures about holes. Scattered
false alarms with no common consumer are not predicted. This becomes checkable
when a seat or the coordinator runs that count, before (1a) lands.

**Condition.** Two findings would each change the reading. First, the census
finding many literals of the template kind, whose braces a program fills later.
Then Heroes has the reason the others failed, and (1a) should wait for (1e) or
settle for (1c). Second, a language shipping a default-on bare-brace check
without scope for years with no pull-back; that would lift the objection's
premise.

## Question 2: a statement after a jump

### Precedents

**P2.1 Java: an error since 1996, and one predicate for both rules.
Verified.** The first edition's text, JLS 1.0 §14.19,
<https://titanium.cs.berkeley.edu/doc/java-langspec-1.0/14.doc.html> (the page
says *"HTML generated by Suzette Pelouch on February 24, 1998"*; the book is
the 1996 Addison-Wesley first edition, ISBN 9780201634518, per the listing
<https://www.biblio.com/book/java-language-specification-james-gosling-bill/d/1595603589>):
*"It is a compile-time error if a statement cannot be executed because it is
unreachable. Every Java compiler must carry out the conservative flow analysis
specified here to make sure all statements are reachable."* The rules: *"A
`break`, `continue`, `return`, or `throw` statement cannot complete
normally."*; *"Every other statement S in a nonempty block that is not a switch
block is reachable iff the statement preceding S can complete normally."*;
*"A `while` statement can complete normally iff at least one of the following
is true: The `while` statement is reachable and the condition expression is not
a constant expression with value `true`. There is a reachable `break` statement
that exits the `while` statement."*; *"An if-then statement can complete
normally iff it is reachable."*, and an if-then-else can complete normally iff
the then-statement or the else-statement can (the tool returned that sentence
cut short); *"An expression statement can complete normally iff it is
reachable."* **The other rule reads the same predicate**, JLS 1.0 §8.4.5,
<https://titanium.cs.berkeley.edu/doc/java-langspec-1.0/8.doc.html>: *"A
compile-time error occurs if the body of the method can complete normally. In
other words, a method with a return type must return only by using a return
statement that provides a value return; it is not allowed to "drop off the end
of its body.""*, with the example `int pitch() { throw new
RuntimeException("90 mph?!"); }`. Both rules are still there today, as §14.22
*"Unreachable Statements"* and §8.4.7 *"Method Body"* in the JLS SE 21 table of
contents (<https://docs.oracle.com/javase/specs/jls/se21/html/index.html>).
**What it cost, in three places:**

- **The spec built its own escape.** From the same §14.19: *"the following
  statement results in a compile-time error: while (false) { x=3; } because the
  statement `x=3;` is not reachable; but the superficially similar case: if
  (false) { x=3; } does not result in a compile-time error ... The idea is that
  it should be possible to change the value of `DEBUG` from `false` to `true`
  or from `true` to `false` and then compile the code correctly with no other
  changes to the program text."*
- **A call that ends the program is not in the predicate**: `System.exit(0);`
  is an expression statement, which by the rule quoted *"can complete normally
  iff it is reachable"*, so a method whose last statement is `System.exit(0);`
  must still say `return` or `throw`. That is a reading of the rule, not a
  fetched sentence. It is the same gap as Heroes' `exit(code:)`.
- **`assert false` does not count either, and cannot be written where the rule
  already knows.** Oracle's guide,
  <https://docs.oracle.com/javase/6/docs/technotes/guides/language/assert.html>:
  *"By default, assertions are disabled at runtime."* It recommends `assert
  false;` at *"any location you assume will not be reached"*, and then: *"If a
  statement is unreachable as defined in the Java Language Specification ...,
  you will get a compile time error if you try to assert that it is not
  reached. Again, an acceptable alternative is simply to throw an
  `AssertionError`."*
- And a user's word, in P2.7's thread: Java's error *"has annoyed me so many
  times"* (mikehearn, on the Kotlin forum, 2016).

**P2.2 Zig: an error, reaching calls that never return. Verified.**
<https://github.com/ziglang/zig/issues/11686> (2022-05-20, Zig
0.10.0-dev.2315+dd6ac9a22), a case that escapes the rule, names what the rule
prints: *"error: unreachable code"* with the note *"control flow is diverted
here"*. <https://github.com/ziglang/zig/issues/8631> (N00byEdge, 2021-04-28):
a call to a `noreturn` function followed by `unreachable;` gets *"error:
unreachable code"*, and the workaround users reach for is to wrap the call in
`if(true)`, *"since the compiler then treats the call as conditionally
reachable"* (the tool's rendering of the issue). So Zig's analysis reaches a
`noreturn` call, and its users route around it with a condition the rule does
not evaluate.

**P2.3 TypeScript: an error by default for two years, then relaxed, over the
debugging habit. Verified.** TypeScript 1.8's notes,
<https://www.typescriptlang.org/docs/handbook/release-notes/typescript-1-8.html>:
*"Statements guaranteed to not be executed at run time are now correctly
flagged as unreachable code errors. For instance, statements following
unconditional `return`, `throw`, `break` or `continue` statements are
considered unreachable."*, with an example after an `if`/`else` whose branches
both return, and the catch it was proudest of, a `return` split from its value
by automatic semicolon insertion. The announcement, 2016-02-22,
<https://devblogs.microsoft.com/typescript/announcing-typescript-1-8-2/>: *"If
for whatever reason you are experiencing too many false positives, please let
us know, but you can toggle this feature off with the `--allowUnreachableCode`
flag."* Then <https://github.com/Microsoft/TypeScript/issues/10233>
(2016-08-09): *"I don't want to allow unreachable code in my code, I just don't
want to see errors for unreachable code that is in my node_modules."* Then
<https://github.com/microsoft/TypeScript/issues/24026> (Alex Eagle,
2018-05-10), in full as the tool returned it: *"Unreachable code is exactly
this case. Developers sometimes add a quick `throw` statement just to make a
runtime assertion while debugging. Then they get an error diagnostic which
forces them to comment out remaining lines in this control flow, or add a fake
guard condititonal `if (1==1) throw new Error('')`"* and *"I think unreachable
code is not an invalid TypeScript program and therefore should be treated like
an unused variable."* Marked fixed for 2.9.1; TypeScript 2.9 was announced
2018-05-31 (<https://devblogs.microsoft.com/typescript/announcing-typescript-2-9-2/>),
and <https://github.com/microsoft/TypeScript/issues/25738> (2018-07-17):
*"Apparently, since 2.9.1, unreachable code is allowed by default, you have to
explicitly set `allowUnreachableCode` to `false` in `tsconfig.json` to get the
old behavior."* Today, <https://www.typescriptlang.org/tsconfig/allowUnreachableCode.html>:
`undefined` (default) *"provide suggestions as warnings to editors"*, `true`
*"unreachable code is ignored"*, `false` *"raises compiler errors about
unreachable code"*. **The other half arrived in 3.7** (announced 2019-11-05,
<https://devblogs.microsoft.com/typescript/announcing-typescript-3-7/>), from
<https://www.typescriptlang.org/docs/handbook/release-notes/typescript-3-7.html>:
*"In order to ensure that a function never potentially returned `undefined` or
effectively returned from all code paths, TypeScript needed some syntactic
signal - either a `return` or `throw` at the end of a function. So users found
themselves `return`-ing their failure functions."* and *"Now when these
`never`-returning functions are called, TypeScript recognizes that they affect
the control flow graph and accounts for them."*, `process.exit` being the
example.

**P2.4 C#: a warning, and two predicates for one fact. Verified.** The
specification,
<https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/language-specification/statements>:
*"A warning is reported if a statement other than throw_statement, block, or
empty_statement is unreachable. It is specifically not an error for a statement
to be unreachable."* and *"It is a compile-time error for the end point of the
block of a function member or an anonymous function that computes a value to be
reachable."* Its `switch` note: *"a `while` statement controlled by the Boolean
expression `true` is known to never reach its end point. Likewise, a `throw` or
`return` statement always transfers control elsewhere and never reaches its end
point."* And the split, <https://github.com/dotnet/csharplang/issues/3826>
(2020-08-27): a helper marked `[DoesNotReturn]` still left CS0161, *"not all
code paths ... return a value"*; the replies, the same day, from GitHub's API
(<https://api.github.com/repos/dotnet/csharplang/issues/3826/comments>):
CyrusNajmabadi, *"[DoesNotReturn] is somehow only used in nullability analysis
and not in flow of control analysis? That is correct."*; GrabYourPitchforks
(association MEMBER), *"if that feature existed then OP's `Rethrow` helper
method would have a _never_ return type instead of a _void_ return type"*.
**An attribute that says a call never returns is read by one analysis and not
by the one that demands the `return`**, and the user's remedy was a redundant
`throw`.

**P2.5 Swift: a warning; `Never` closes the other half. Verified.**
<https://raw.githubusercontent.com/swiftlang/swift/main/include/swift/AST/DiagnosticsSIL.def>:
`WARNING(unreachable_code, ..., "will never be executed")`,
`WARNING(unreachable_code_after_stmt, ..., "code after
'%select{return|break|continue|throw}0' will never be executed")`,
`ERROR(missing_return_decl, ..., "missing return in %kindonly1 expected to
return %0")` and `ERROR(missing_never_call_decl, ..., "%kindonly1 with
uninhabited return type %0 is missing call to another never-returning function
on all paths")`. SE-0102,
<https://raw.githubusercontent.com/swiftlang/swift-evolution/main/proposals/0102-noreturn-bottom-type.md>
(Joe Groff, implemented in Swift 3.0): `@noreturn` removed for an empty `Never`
type, whose expressions are *"considered unreachable by control flow
diagnostics"*.

**P2.6 Rust: a warning; `!` closes the other half, and `while true` is not a
leaving loop. Verified.** `UNREACHABLE_CODE`, level `Warn`, in
<https://raw.githubusercontent.com/rust-lang/rust/master/compiler/rustc_lint_defs/src/builtin.rs>:
example `panic!("we never go past here!"); let x = 5;`, explanation *"Unreachable
code may indicate a mistake or incomplete work. If code is no longer needed,
consider removing it."* `pub fn exit(code: i32) -> !`, since 1.0.0
(<https://doc.rust-lang.org/std/process/fn.exit.html>). The Reference,
<https://doc.rust-lang.org/reference/expressions/loop-expr.html>: *"A `loop`
expression without an associated `break` expression is diverging and has type
`!`."* and *"A `while` expression evaluates to `()`."* So Rust demands a
statement after `while true` too, and its answer was a spelling, not a wider
predicate: `WHILE_TRUE`, level `Warn`
(<https://raw.githubusercontent.com/rust-lang/rust/master/compiler/rustc_lint/src/builtin.rs>),
*"`while true` should be replaced with `loop`. A `loop` expression is the
preferred way to write an infinite loop because it more directly expresses the
intent of the loop."* And `assert!(false)` does not diverge either: Clippy's
`assertions_on_constants` (1.34.0, `style`,
<https://raw.githubusercontent.com/rust-lang/rust-clippy/master/clippy_lints/src/assertions_on_constants.rs>),
*"Will be optimized out by the compiler or should probably be replaced by a
`panic!()` or `unreachable!()`"*.

**P2.7 Kotlin: a warning, by stated principle. Verified.**
<https://discuss.kotlinlang.org/t/why-compiler-allow-to-put-unreachable-code-after-return/1436>
(2016-01-28), asked why Kotlin warns where javac refuses; yole, whom the thread
shows as Kotlin team (affiliation as the tool rendered it): *"Kotlin does not
report errors for code that has an unambiguous meaning and can be correctly
executed."* In the same thread, mikehearn on Java's error: *"has annoyed me so
many times"*; Krupal against: *"Unreachable code definitely should not be
allowed"*, a *"very popular bug in other languages"*. `Nothing`,
<https://kotlinlang.org/docs/exceptions.html>: *"`Nothing` is a special type in
Kotlin used to represent functions or expressions that never complete
successfully"*; `inline fun exitProcess(status: Int): Nothing`, since 1.0
(<https://kotlinlang.org/api/core/kotlin-stdlib/kotlin.system/exit-process.html>).

**P2.8 Go: no compiler rule for the statement, a syntactic rule for the
`return`. Verified.** `vet`'s `unreachable`,
<https://pkg.go.dev/golang.org/x/tools/go/analysis/passes/unreachable>:
*"check for unreachable code"*, statements *"preceded by a return statement, a
call to panic, an infinite loop, or similar constructs"*; it is **not** in the
subset `go test` runs (P1.9's list). Go 1.1, released 2013-05-13
(<https://go.dev/doc/devel/release>), <https://go.dev/doc/go1.1>: *"Before Go
1.1, a function that returned a value needed an explicit "return" or call to
`panic` at the end of the function ... In Go 1.1, the rule about final "return"
statements is more permissive. It introduces the concept of a terminating
statement ... Examples include "for" loops with no condition and "if-else"
statements in which each half ends in a "return" ... Note that the rule is
purely syntactic"*. The type checker,
<https://raw.githubusercontent.com/golang/go/master/src/go/types/return.go>:
*"calling the predeclared (possibly parenthesized) panic() function is
terminating"*, a `for` is terminating when `s.Cond == nil &&
!hasBreak(s.Body, label, true)`, an `if` when both branches are. **`os.Exit`
is not**: the predicate names the built-in and nothing else. The Go spec page
itself reached the tool truncated before *"Terminating statements"*, so this
row rests on the release notes and the checker.

**P2.9 C: a specifier, and a warning withdrawn. Verified, one row
secondary.** `_Noreturn`, from cppreference (secondary,
<https://en.cppreference.com/w/c/language/_Noreturn>): C11, deprecated in C23
for `[[noreturn]]`; *"If the function declared `_Noreturn` returns, the
behavior is undefined."* GCC, Ian Lance Taylor on gcc-help, 2011-05-25
(<https://gcc-help.gcc.gnu.narkive.com/kzLJZby3/gcc-wunreachable-code-option>):
*"The -Wunreachable-code has been removed, because it was unstable: it relied
on the optimizer, and so different versions of gcc would warn about different
code."*

**P2.10 Wirth: the jumps deleted, for the reason this sitting is building a
predicate. Verified, read from the PDFs.** Oberon-2 (Mössenböck and Wirth,
October 1993, <https://cseweb.ucsd.edu/~wgg/CSE131B/oberon2.htm>) had them:
*"Statement = [ Assignment | ProcedureCall | IfStatement | CaseStatement |
WhileStatement | RepeatStatement | ForStatement | LoopStatement |
WithStatement | EXIT | RETURN [Expression] ]."* Oberon-07,
<https://people.inf.ethz.ch/wirth/Oberon/Oberon07.Report.pdf> (*"Revision
1.10.2013 / 3.5.2016"*): `statement = [assignment | ProcedureCall |
IfStatement | CaseStatement | WhileStatement | RepeatStatement |
ForStatement]`, `ProcedureBody = DeclarationSequence [BEGIN
StatementSequence] [RETURN expression] END`, and a function's *"body must end
with a RETURN clause which defines the result of the function procedure."*
Wirth's reasons, *"Differences between Revised Oberon and Oberon"*, 22.03.2008 /
15.7.2011, <https://people.inf.ethz.ch/wirth/Oberon/Oberon07.pdf>: *"The loop
statement with its exit statements represents, however, a break with the idea
of a structured language, where properties of a statement can be derived from
those of its components. The loop statement with its syntactically unconnected
exit statements does not allow this. It has therefore been deleted from the
language together with the associated exit statement."* And: *"The result of
a function procedure was specified by a return statement. This form has the
unpleasant property that the return statement is syntactically disconnected
from the function procedure declaration, similar to the exit from the loop
statements. It is therefore difficult to check, whether or not a function
procedure declaration specifies a result, or perhaps even several of them."*
And, in passing: *"HALT is replaced by ASSERT(FALSE)."*

### How far each reaches

| language | a statement after one that always leaves | an `if`/`else` whose branches all leave | a call that never returns | `while true`, no `break` | the missing-`return` rule reads the same answer |
|---|---|---|---|---|---|
| Java, 1996 | **error** | leaves | not in the predicate | leaves | **yes**, *can complete normally* |
| Zig | **error** | unverified | leaves (`noreturn`) | unverified | unverified |
| TypeScript | error by default 1.8 to 2.9.1; now an editor suggestion, an error on request | leaves | leaves since 3.7, if declared `never` | unverified | yes, since 3.7 |
| C# | warning | leaves (by the spec's end-point rules) | not read (`[DoesNotReturn]` is nullability only) | leaves | the end point, not the attribute |
| Swift | warning | unverified | leaves (`Never`) | unverified | yes (its two errors) |
| Rust | warning | unverified | leaves (`!`) | **does not leave**; `loop` does | yes, by type |
| Kotlin | warning | unverified | leaves (`Nothing`) | unverified | unverified |
| Go | `vet` only, not in `go test` | leaves | the built-in `panic` only, not `os.Exit` | `for {}` only | syntactic, *terminating statement* |
| C | no rule; GCC withdrew its warning | | `_Noreturn` | | |
| Oberon-07 | impossible by grammar | | | | impossible by grammar |

### What the precedents say together

1. **One predicate for both rules is Java's design, since 1996**, and it is
   route (2a) exactly: *can complete normally* decides both the §14.19 error
   and the §8.4.5 error. C# shows the other design and what it costs: a fact
   (*this call does not return*) that one analysis reads and the other does
   not, and a user writing a redundant `throw` to satisfy the one that does
   not.
2. **Two languages found refuse the statement today, Java and Zig.** TypeScript
   refused it by default for two years and relaxed on the debugging `throw`;
   Kotlin and C# chose a warning on the stated ground that the code is a valid
   program.
3. **Every refusing language has a way round, and it is always the same one**:
   a condition the rule does not evaluate. Java's spec builds it in (`if
   (DEBUG)`), Zig's users write `if(true)`, and TypeScript's issue quotes `if
   (1==1) throw`.
4. **The call that ends a program goes into the predicate in one of two
   ways**: a bottom type (Rust `!`, Swift `Never`, Kotlin `Nothing`,
   TypeScript `never` since 3.7, Zig `noreturn`), or a built-in named in the
   predicate (Go's `panic`, and not its library `os.Exit`). Java and C# have
   neither and keep demanding the dead statement; TypeScript's notes name that
   cost in its own words and removed it.
5. **`assert false` counts nowhere found.** Java's assertions are off by
   default, so it completes normally; Rust lints `assert!(false)` towards
   `panic!()`; Oberon-07 made `ASSERT(FALSE)` its halt, with no jump to follow.
   What decides it for Heroes is whether a Heroes assertion can be switched
   off, a Heroes fact this seat did not read.
6. **`while true` splits**: Java and C# count a constant `true`; Go counts only
   `for {}`; Rust counts only `loop` and lints `while true` towards it.
7. **A marker of unreachability, written where the rule already knows, is
   itself refused** in both refusing languages: Java's guide and Zig's issue
   8631 are the same shape.
8. **Wirth's answer was to delete the jumps**, and his words are this
   sitting's: a statement's properties should be *"derived from those of its
   components"*, and a disconnected `return` makes it *"difficult to check,
   whether or not a function procedure declaration specifies a result"*.

### Verdict per route (advisory)

- **(2a) one predicate for both rules: approve.** It is Java's design, held for
  thirty years, and each item the brief asks the seats to settle has a
  precedent. Jumps and an `if`/`else` or `match` all of whose branches leave:
  Java, Go, TypeScript. `while true` with no `break`: Java and C#. `exit(code:)`
  is a built-in (`spec/heroes-spec.md:319`, per the shared brief), so Go's way
  is open: name the built-in in the predicate, with no type-system change and
  no `Never`. `assert false` belongs in it only if a Heroes assertion can never
  be switched off, since Java's reason for leaving it out is that it can.
- **(2b) the three jump words only: object.** It keeps the dead statement the
  language demands, which is the cost TypeScript 3.7 removed and Java never
  did. And it leaves two predicates for one fact, which is the C# shape: a
  statement after an `if`/`else` whose branches both return would stay legal,
  while `missing_return` already counts that `if` as leaving (the brief's
  `after-if-else-returns.hero`).
- **(2c) today's rule: object.** Two of the brief's six silent `over-indent`
  mutants are this shape exactly, and the one at `refused_since` turns a
  terminating loop into one that does not end. Java, Zig and TypeScript 1.8
  would refuse both; the rest would warn.
- **A route nobody listed, which Wirth took: (2d) no jump in mid-block.** Named,
  not recommended: the compiler itself returns from inside loops (the brief's
  `selfhost/cursor.hero:237`, `return true` inside a `while`), so under
  Principle 0 it is a rewrite of the compiler, not a rule.

**Argument** (120 words or fewer). Java has lived since 1996 with the shape the
brief calls (2a): one predicate, *can complete normally*, behind both the
unreachable-statement error and the missing-return error. It works. The
precedents also record the two costs Heroes would inherit. First, a program-ending call left out of the predicate
forces a dead `return`; TypeScript removed that cost in 3.7, and Go avoided it
by naming its built-in. Second, the debugging habit: TypeScript relaxed its
default over it, Kotlin refused to make it an error, and Zig and Java users
route around it with an always-true condition. That second cost is a departure
Heroes can take on purpose, since its premise is the opposite of Kotlin's. (2b)
recreates C#'s split.

**Prediction** (falsifiable). Under (2a), the first program in the tracked tree
that needs to keep a statement after one that always leaves will route around
the rule with an `if true` guard around the jump, the way Java's spec, Zig's
users and TypeScript's issue did. Checkable by searching the tracked tree for
an `if true` whose body is a jump, at the close of the milestone that ships
(2a). And if `exit(code:)` is counted, every `return` written after an
`exit(code:)` in the tracked tree becomes a refusal, and the census the batch
gate runs counts them before the rule lands.

**Condition.** Two findings would change the reading. First, a Heroes assertion
that can be switched off at run time, which takes `assert false` out of the
predicate as Java's took it out. Second, a language that refused unreachable
code with one shared predicate and withdrew the refusal for a reason other than
the debugging habit or other people's code. That would be a cost the record
here does not show.

## Question 3: how deep a source may nest

### Precedents

**P3.1 C: numbers in the standard, as floors, with a footnote against them.
Verified.** C89's draft, §2.2.4.1,
<http://port70.net/~nsz/c/c89/c89-draft.html>: *"The implementation shall be
able to translate and execute at least one program that contains at least one
instance of every one of the following limits:"*, among them *"15 nesting levels
of compound statements, iteration control structures, and selection control
structures"*, *"8 nesting levels of conditional inclusion"* and *"32
expressions nested by parentheses within a full expression"*. C11, N1570
§5.2.4.1, <http://port70.net/~nsz/c/c11/n1570.html>, the same sentence, with
*"127 nesting levels of blocks"*, *"63 nesting levels of conditional
inclusion"*, *"12 pointer, array, and function declarators"*, *"63 nesting
levels of parenthesized declarators within a full declarator"*, *"63 nesting
levels of parenthesized expressions within a full expression"*, and footnote
18: *"Implementations should avoid imposing fixed translation limits whenever
possible."* **So C states numbers, but they are what an implementation must
reach on at least one program, not a ceiling a program must respect**, and the
standard's own footnote argues against a fixed ceiling.

**P3.2 C++: Annex B, larger floors. Partly verified.**
<https://eel.is/c++draft/implimits>: *"Nesting levels of compound statements,
iteration, and selection structures [256]"*, *"Nesting levels of conditional
inclusion [256]"*, *"Nesting levels of parenthesized expressions within a
full-expression [256]"*, *"Recursively nested template instantiations
[1024]"*. The annex's opening paragraph, which says what standing the numbers
have, did not reach the tool; that standing is unverified here.

**P3.3 clang: a fixed ceiling on brackets, a fatal error, and what it did to a
language that emits C. Verified.** The diagnostic,
<https://raw.githubusercontent.com/llvm/llvm-project/main/clang/include/clang/Basic/DiagnosticParseKinds.td>:
`def err_bracket_depth_exceeded : Error<"bracket nesting level exceeded maximum
of %0">, DefaultFatal;` and `def note_bracket_depth : Note<"use
-fbracket-depth=N to increase maximum nesting level">;`. The default,
<https://raw.githubusercontent.com/llvm/llvm-project/main/clang/include/clang/Basic/LangOptions.def>:
`LANGOPT(BracketDepth, 32, 256, Benign, "maximum bracket nesting depth")`.
It is a language option, not a measure of the stack, so it is the same number
wherever clang runs (a reading of the definition, not a run on three
platforms). **Why it matters here**:
<https://github.com/aether-lang-dev/aether/issues/2071> (2026-09-18), a
language whose code generator wrapped every binary operation in parentheses:
*"left-associated chains like `x0 + x1 + … + xN` ... generate deeply nested C
code: `((((x0 + x1) + x2) + …)`"*, stopped by *"fatal error: bracket nesting
level exceeded maximum of 256"*, in generated lookup-table checksums and
unrolled polynomials. The repair, PR 2080, left a same-precedence left operand
unbracketed so that nesting *"grow[s] with actual expression depth rather than
expression length"* (the tool's rendering). Heroes emits C for clang; the
shared brief's `--emit-c` of `after-return.hero` shows three-address
temporaries, and every closed shape the brief built at `check`'s largest
accepted depth compiled, so today's emission does not meet this ceiling. An
emitter change that nests C expressions would.

**P3.4 GCC: searched, not found.** Web search *gcc bugzilla "deeply nested"
parentheses OR expressions "Segmentation fault" stack overflow c-parser
recursion internal compiler error* returned no GCC tracker entry. The only
statement found is aether's issue above, a third party: *"GCC has no such
restriction"*. GCC's behaviour on a deep source is **unverified** by this seat.

**P3.5 MSVC: one fixed ceiling, one stack-derived one, both fatal errors.
Verified.** C1061,
<https://learn.microsoft.com/en-us/cpp/error-messages/compiler-errors-1/fatal-error-c1061>:
*"compiler limit : blocks nested too deeply"*; *"Nesting of code blocks exceeds
the limit of 128 nesting levels. This is a hard limit in the compiler for both
C and C++, in both the 32-bit and 64-bit tool set. The count of nesting levels
can be increased by anything that creates a scope or block. For example,
namespaces, using directives, preprocessor expansions, template expansion,
exception handling, loop constructs, and else-if clauses can all increase the
nesting level seen by the compiler."* C1026,
<https://learn.microsoft.com/en-us/cpp/error-messages/compiler-errors-1/fatal-error-c1026>:
*"parser stack overflow, program too complex"*; *"The space required to parse
the program caused a compiler stack overflow."* An `else if` chain counts as
nesting in MSVC; the shared brief measured Heroes' `else if` chain holding at
10,000.

**P3.6 CPython: fixed ceilings in the lexer, a stack-derived one in the
parser, and the stack-derived one moved with the platform. Verified.**
<https://raw.githubusercontent.com/python/cpython/main/Parser/lexer/state.h>:
`#define MAXINDENT 100 /* Max indentation level */` and `#define MAXLEVEL 200
/* Max parentheses level */`; <https://raw.githubusercontent.com/python/cpython/main/Parser/lexer/lexer.c>:
`if (tok->level >= MAXLEVEL) { return
MAKE_TOKEN(_PyTokenizer_syntaxerror(tok, "too many nested parentheses")); }`.
The parser's guard is told as *"MemoryError: Parser stack overflowed - Python
source too complex to parse"*. Its history in one year:
<https://github.com/python/cpython/issues/135028> (2025-06-02): `eval("(" * 200
+ ")" * 200)` gave *"s_push: parser stack overflow"* in 3.8, worked in 3.9 to
3.13, and failed again in 3.14 alpha 6 after a change to the stack-based
recursion checks, repaired by raising the parser's `MAXSTACK` (PRs 135031 and
135059). <https://github.com/python/cpython/issues/131338> (2025-03-16):
*"Stack overflow test errors in Alpine after GH-130398"*, the new stack-size
code not accounting for non-glibc Linux, repaired by disabling it there (PR
134336, backport 137175). <https://github.com/python/cpython/issues/137231>
(2025-07-30), *"[3.14] Parser stack overflow on `musllinux` `x86_64`"*, closed
with *"I can confirm this is fixed in 3.14.0rc2"* (2025-08-16, GitHub's API).
And the documentation of `compile()`, <https://docs.python.org/3/library/functions.html>:
*"Warning: It is possible to crash the Python interpreter with a sufficiently
large/complex string when compiling to an AST object due to stack depth
limitations in Python's AST compiler."*

**P3.7 Go: a ceiling added to the library parser for a CVE, none in the
compiler's, and a runtime that grows the stack to 1 GB. Verified.**
<https://github.com/golang/go/issues/53616> (2022-06-29), *"go/parser: stack
exhaustion in all Parse* functions"*, CVE-2022-1962: *"Calling any of the Parse
functions on Go source code which contains deeply nested types or declarations
can cause a panic due to stack exhaustion."* The CVE record
(<https://app.opencve.io/cve/CVE-2022-1962>): *"before Go 1.17.12 and Go
1.18.4"*. The fix,
<https://raw.githubusercontent.com/golang/go/master/src/go/parser/parser.go>:
`const maxNestLev int = 1e5`, the error *"exceeded max nesting depth"*, with
the comment that the counter is kept *"to prevent stack exhaustion"*. The gc
compiler's own parser,
<https://raw.githubusercontent.com/golang/go/master/src/cmd/compile/internal/syntax/parser.go>:
no depth limit found, the tool reporting the file complete (a reading, not a
run). What it runs against instead, <https://pkg.go.dev/runtime/debug>,
`SetMaxStack`: *"1 GB on 64-bit systems, 250 MB on 32-bit systems"*, and past
it *"the program crashes"*. So Go's compiler takes route (3b) through its
runtime, with a crash at a ceiling 128 times this Mac's 8 MB.

**P3.8 rustc: the stack grown on demand, and now being given up. Verified.**
<https://doc.rust-lang.org/stable/nightly-rustc/src/rustc_data_structures/stack.rs.html>:
`const RED_ZONE: usize = 100 * 1024; // 100k`, `const STACK_PER_RECURSION:
usize = 1024 * 1024; // 1MB` (16 MB on AIX), and *"Grows the stack on demand to
prevent stack overflow. Call this in strategic locations to "break up"
recursive calls. E.g. almost any call to `visit_expr` or equivalent can benefit
from this. Should not be sprinkled around carelessly, as it causes a little bit
of overhead."* Then rust-lang/compiler-team major change 1011,
<https://github.com/rust-lang/compiler-team/issues/1011>, *"Let the OS handle
stack growth"*, ChrisDenton, opened 2026-07-07, closed 2026-07-31 as
`major-change-accepted` (GitHub's API): *"Making effective use of
`ensure_sufficient_stack` has proved to be difficult to do because it requires
constant vigilance so in practice we can still run out of stack space."*;
*"Stacker only supports a limited number of platforms so it's a no-op on many.
Worse, on platforms that are partially supported (such as android), one failure
mode is that the thread only gets 1 MiB of stack even if more stack space is
available."*; *"On Windows it uses fibers so the performance hit is likely
higher."*; *"By increasing the stack limit and disabling manual stack growth I
saw performance improvements of ~1%"*. And the one number Rust does state is
not about source nesting: `recursion_limit`,
<https://doc.rust-lang.org/reference/attributes/limits.html>, *"the maximum
depth for potentially infinitely-recursive compile-time operations like macro
expansion or auto-dereference"*, default 128.

**P3.9 javac: no limit; the overflow caught and given its own exit code.
Verified.**
<https://raw.githubusercontent.com/openjdk/jdk/master/src/jdk.compiler/share/classes/com/sun/tools/javac/main/Main.java>:
`OK(0), // Compilation completed with no errors.`, `ERROR(1), // Completed but
reported errors.`, `CMDERR(2), // Bad command-line arguments`, `SYSERR(3), //
System error or resource exhaustion.`, `ABNORMAL(4); // Compiler terminated
abnormally`, and `catch (OutOfMemoryError | StackOverflowError ex) {
resourceMessage(ex); return Result.SYSERR; }`. What a user sees,
<https://bugs.openjdk.org/browse/JDK-8166672> (2016-09-23): *"The system is out
of resources. Consult the following stack trace for details.
java.lang.StackOverflowError"*. Where the depth goes, a chain a reader sees as
flat: <https://gist.github.com/jprante/3f167c34d81e1ccf3053a339c385057e> (last
active 2017-01-04), more than 700 calls in one `a.add(b).add(c)...` chain
overflowed in `Attr.visitApply` and `Attr.visitSelect` at `-J-Xss1m`, and
`-J-Xss2m` let it compile.

**P3.10 Roslyn: the stack probed and turned into an error, whose threshold
moves. Verified.**
<https://raw.githubusercontent.com/dotnet/roslyn/main/src/Compilers/CSharp/Portable/Errors/ErrorCode.cs>:
`ERR_InsufficientStack = 8078`, told as *"An expression is too long or complex
to compile"*; the mechanism, per the search results, is
`RuntimeHelpers.EnsureSufficientExecutionStack` (the mechanism's page itself was
not fetched: unverified in detail). It moves:
<https://github.com/dotnet/roslyn/issues/29428> (gafter, 2018-08-21), *"Stack
overflow checks not reliable on Ubuntu Linux"*, two tests restricted to
Windows; and <https://github.com/dotnet/roslyn/issues/82024> (2026-01-15),
generated source that compiled with compiler 5.0.0-2.25569.105 and failed with
CS8078 on 5.0.0-2.26054.2.

**P3.11 Lua: every syntactic level counted against one constant, and the flat
chain made a loop. Verified.** <https://www.lua.org/source/5.4/llimits.h.html>:
*"Maximum depth for nested C calls, syntactical nested non-terminals, and other
features implemented through recursion in C. (Value must fit in a 16-bit
unsigned integer. It must also be compatible with the size of the C stack.)"*,
`#define LUAI_MAXCCALLS 200`. <https://www.lua.org/source/5.4/lparser.c.html>:
`#define enterlevel(ls) luaE_incCstack(ls->L)`, called on entry to `subexpr`,
whose binary operators are a loop, `while (op != OPR_NOBINOPR &&
priority[op].left > limit) { ... nextop = subexpr(ls, &v2,
priority[op].right); ... }`, so a run of one precedence returns to the loop
instead of deepening (a reading of the code). <https://www.lua.org/source/5.4/lstate.c.html>:
`if (getCcalls(L) == LUAI_MAXCCALLS) luaG_runerror(L, "C stack overflow");`.
**This is route (3c) as shipped**: nesting and one-operand runs counted, flat
chains built without recursion, one number sized below the C stack.

**P3.12 SQLite: from "no arbitrary limits" to a tested one on the tree's
depth. Verified.** <https://www.sqlite.org/limits.html>: *"SQLite was
originally designed with a policy of avoiding arbitrary limits"*, which
*"created problems because upper bounds were not well defined and not tested.
Since version 3.5.8 (2008-04-16), SQLite has well-defined limits that are
tested as part of the test suite."* Expression trees: *"During code generation,
SQLite walks this tree recursively. The depth of expression trees is therefore
limited in order to avoid using too much stack space"*, `SQLITE_MAX_EXPR_DEPTH`
default 1000 (the page as the tool rendered it). The error text SQLite prints
at the limit was not fetched: unverified.

**P3.13 Pascal-P4 (the Wirth lineage): fixed ceilings, numbered errors, 1976.
Verified in the source.**
<https://raw.githubusercontent.com/kenyapcomau/pascal-p4/main/comp.p>, whose
header reads *"pascal p4 ... authors: urs ammann, kesav nori, christian jacobi
... eidg. technische hochschule ... last changes completed in may 76"*: `const
displimit = 20; maxlevel = 10;`, with `if top < displimit then ... else
error(250)` where a scope opens (records, procedures, `with`) and `if level <
maxlevel then level := level + 1 else error(251)` for nested procedures. The
messages for 250 and 251 are said to be in Jensen and Wirth's second edition,
page 119 (<https://www.standardpascaline.org/p4.html>); their words are
unverified here. No `error(259)` appears in the source, so no limit on
expression depth was found.

**P3.14 A fixed count sized for a larger stack than it ran on. Verified.**
<https://github.com/MDSplus/mdsplus/issues/3085> (2026-09-04): a guard
`TDI_INTRINSIC_REC > 1800`, *"sized for the 8 MB default stack"*, on client
threads given `0x40000` (256 KB); *"59 concatenated `+` operations"* crash the
server with SIGSEGV before the guard can fire. A fixed number is only as good
as the smallest stack it runs on, and the input that found this was a flat
chain.

**P3.15 JSON: a specification that permits a limit and names none. Verified.**
RFC 8259 §9 (December 2017), <https://www.rfc-editor.org/rfc/rfc8259>: *"An
implementation may set limits on the maximum depth of nesting."*

### Panel 107's finding, asked of a source

Panel 107's historian found that *no language specification states a number*
for a program's recursion at run time. **For a source's nesting, C and C++ do
state numbers, so they are the counter-example in the letter and not in
substance**: C89 and C11 name what an implementation must reach on at least
one program, C11's footnote 18 tells implementations to avoid fixed limits, and
JSON permits a limit without a number. No specification found makes a nesting
number binding on programs. **The numbers that are real ceilings belong to
implementations**: clang 256 brackets, MSVC 128 blocks, CPython 200
parentheses and 100 indents, Lua 200 levels, SQLite 1000, go/parser 100,000,
Pascal-P4 20 scopes and 10 procedure levels. Each is a constant, so each is the
same on every platform its implementation runs on. **The limits that are not
constants moved in the record**: Roslyn's on Ubuntu and between two compiler
builds, CPython's between 3.13 and 3.14 and between glibc and musl, and
javac's with `-Xss`.

### Which kinds each limit sees (the brief's three)

| precedent | nesting a reader sees | one-operand runs (`- -`, `!!`) | flat chains (`+`, `&&`, method chain) |
|---|---|---|---|
| clang `-fbracket-depth` | brackets only | not counted | not counted (aether's chain met it only once the generator bracketed it) |
| MSVC C1061 | blocks, `else if` included | not counted | not counted; C1026 catches any exhaustion |
| CPython `MAXLEVEL`, `MAXINDENT` | parentheses, indentation | not counted | the parser's stack guard, platform-dependent |
| Lua `LUAI_MAXCCALLS` | every syntactic level | counted | built in a loop, not recursion |
| SQLite `SQLITE_MAX_EXPR_DEPTH` | the tree's depth | counted, as tree depth | as deep as the tree the parser builds (unverified how it builds a chain) |
| javac, Roslyn, MSVC C1026 | the stack | the stack | the stack: a 700-call chain in javac, generated chains in Roslyn |

### Verdict per route (advisory)

- **(3a) a fixed limit on openers: approve only as part of (3c).** It has the
  longest record (Pascal-P4 in 1976, clang, MSVC, CPython), each a constant,
  the same on every platform, told as a diagnostic. But every opener counter
  found leaves the chains to the stack: javac's 700-call chain, MDSplus's 59
  `+`, aether's generated sums. In the brief's table that is the method chain
  aborting at 200 with one bracket open, and the `- -` and `+` rows.
- **(3b) no limit: object, as the whole answer.** Its best-known example is
  leaving it: rustc accepted on 2026-07-31 that grow-on-demand *"requires
  constant vigilance so in practice we can still run out of stack space"*.
  Go's runtime grows to 1 GB and then *"the program crashes"*. The sub-route
  rustc is moving to, a large stack the compiler chooses, keeps the abort at
  a larger number. Panel 107 refused `main` on a created thread for Heroes
  programs because of SDL. Whether that reason reaches the compiler, which
  links no SDL, is a question for the seats that compile, not a finding here.
- **(3c) a limit on the tree's depth, every kind counted, chains built without
  recursion: approve.** Lua and SQLite are this route. Lua counts every
  syntactic level against a number *"compatible with the size of the C
  stack"* and makes the flat chain a loop, and SQLite limits the tree its
  recursive walk visits. MDSplus is the warning attached: size the number for
  the smallest stack every pass runs on, `build`'s included. The brief measured
  `build` aborting before `check` does (`g(...)` at 100).
- **(3d) today's abort: object.** Exit 134 is outside the command's own
  contract, per the brief. Every tool found that meets its stack names the
  failure and has its exit code for it: javac's `SYSERR(3)`, Roslyn's CS8078,
  MSVC's C1026.
- **A route nobody listed: (3e) the abort made the contract's exit 2, with the
  place named**, as the backstop under (3c) for whatever the count does not
  see. javac returns *"System error or resource exhaustion"* as its own code,
  and Roslyn and MSVC turn the exhausted stack into a diagnostic. The brief
  says the guard-page handler already exists (`runtime/parts/stack.c:21-25`),
  so what changes is what it prints and which code it returns. Alone it is
  (3d) made honest, and its threshold still moves with the platform, as
  Roslyn's and CPython's did.

**Argument** (120 words or fewer). Across fifty years the ceilings that held
are implementation constants, counted where the parser recurses and told as
diagnostics: Pascal-P4, clang, MSVC, CPython, Lua, SQLite. They are the same
number on every platform because they are constants. The limits taken from the
stack moved, across platforms and across versions. Growing the stack is being
given up by its best-known user. Counting openers leaves the flat chain to the
stack, and that is where javac, MDSplus and aether's generated code failed.
Lua's shape answers all three kinds: every level counted, the chain made a
loop. And every tool that still meets its stack at least names the failure and
gives it its own exit code.

**Prediction** (falsifiable). If (3a) lands without the chains, the next
stack abort `check` prints will come from a chain, not from a nesting: a `+`,
`&&` or method chain, or a `- -` run. It will most likely be in generated or
model-written code, the shape of aether's checksums and MDSplus's 59 `+`.
Checkable by re-running the brief's `depth.py` rows that (3a) does not count,
after it lands. And if (3c) lands with one constant, `check` accepts and
refuses the same programs on this Mac, Linux x86-64 and the Windows box.
Checkable by running the brief's programs at the constant and one above it on
all three.

**Condition.** Three findings would change the reading. First, a language
specification that makes a nesting number binding on programs (none found).
Second, rustc reverting major change 1011, or a record of grow-on-demand
holding on every platform a compiler ships to, which would reopen (3b). Third,
a measurement that every walk in Heroes' compiler can be made iterative at a
cost the panel accepts, which would make (3b) hold with no number at all.

## Closing block

**verdict** (advisory; this seat has no veto):

- Question 1: **approve (1a)** as a deliberate departure, on the census's
  answer; **object (1b)**; **approve (1c)** whatever else lands; **object
  (1d)**; route (1e) named, not recommended.
- Question 2: **approve (2a)**, with `exit(code:)` in the predicate as Go puts
  its built-in there, and `assert false` only if a Heroes assertion cannot be
  switched off; **object (2b)**; **object (2c)**; route (2d) named, not
  recommended.
- Question 3: **approve (3c)**, with (3e) as its backstop; **approve (3a)**
  only as part of (3c); **object (3b)** as the whole answer; **object (3d)**.

**section**: design.md §4.17, the sitting's home per the shared brief (this
seat did not read design.md); for Q3 also §1.12, robustness, as CLAUDE.md §
Precedence names it.

**cost** (measured): nothing of Heroes was measured by this seat. Its own
cost was web searches and fetches in this session, not tallied, and no paid
run.

**prediction**: the three above, one per question, each with the moment it
becomes checkable.

**condition**: the three above, one per question.
