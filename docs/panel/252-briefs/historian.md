# Panel 252, historian

Read `00-shared.md` first, whole: the frozen commit (`181555e9a32d0b18878f9f10e48d2eadd939c285`), the five
questions, their robust and conservative resolutions, and Rebel's witnesses. You judge precedent,
advisory: who decided each question before, what it cost them, and what changed their mind. **Your
report** is `.claude/worktrees/rebel-round/docs/panel/252-reports/historian.md`, **written by you, as you
go** (`.claude/skills/panel/SKILL.md:278-283`), in `.claude/agents/historian.md:40-49`'s structure
(`verdict`, `precedents` each marked `verified` or `unverified`, `argument` of at most 120 words,
`condition`, `search`), one block per question. You build nothing, start no paid run and print no
credential.

**Every precedent is a page you read today**, its URL and the day beside it, and says only what the page
states: a comparison is not an adoption, a mechanism the page does not describe is not cited, a page you
cannot read is not cited (`.claude/rules/records.md` § And the record says whose idea it was: of 90
proposed prior-art lines, 33 said more than their page did). End each block with candidate prior-art lines
in the two forms that section allows, `- **Taken from <language or project>**: [<reference>](<url>): <what
the page states>` or `- **Reached first by <language or project>**: ...`. Your `search` field names the
queries and the sources swept, so a sentence about something not found says what was looked for and
where. **Where a fetched page lands**: if your tool saves a file outside
`/Users/joseph/Temp/heroes/heroes-lang`, say where in your report (panel 251's historian's PDF landed
outside the root and the coordinator removed it,
`.claude/worktrees/rebel-scratch/coordinator-todo.md:1234`, carried).

**Read before you search**, so you do not read again what a sitting already read: panel 031's R6 on
module cycles, which cites *"Go's own design paper"* (`docs/panel/031-modules-the-namespace.md:193-198`,
and its line 105 on Go's imports); panel 121's precedents on interpolation (Python's PEP 498, Rust, C#,
Swift, `docs/panel/121-the-brace-was-already-taken.md:47`, `:147-153`, `:261-262`); panel 203's historian
on inference across a call's arguments (Pierce and Turner 1998, Odersky, Zenger and Zenger 2001, JEP 101
and 215, Rust RFC 212, Swift, C# 12, TypeScript, `docs/panel/203-reports/historian.md`); design.md §4.9 on
float rendering (gnulib's `ftoastr`, Java before Schubfach, PEP 331, `docs/design.md:1752-1772`); design.md
§4.12's historical note on Go's decade without user generics (`docs/design.md:2022-2023`); **and, named by
the critic's first reading**: panel 031's R4, *"There is no re-export"*
(`docs/panel/031-modules-the-namespace.md:178-182`); **panel 135**, which deferred the `alias` form and
whose historian already read Go's alias history, *"Go's `=>` was merged and reverted before 1.8, and 1.9's
warrant was refactoring, not brevity"* (`docs/panel/135-the-form-was-cheap-and-the-reasons-under-it-were-borrowed.md:135-136`,
its seat row at `:90`); **panel 029**, whose R4 refuses polymorphic recursion with MLton as precedent
and Henglein 1993 and Kfoury, Tiuryn and Urzyczyn 1993 as its sources
(`docs/panel/029-generics-by-monomorphisation.md:110-118`); **panel 061**, which measured *"`b: u8 @ 'a'`
prints 97"* (`docs/panel/061-the-fields-nobody-named.md:279-284`). Re-read any of them you lean on; a line
copied from them is carried.

**Every item below was written by the brief-writer from memory, unverified**: each is a question, and a
language that does not do what its line says is a finding.

## Question 1: a module that cannot use the module that uses it

- **Go**: is an import cycle refused by the language specification or by the toolchain, and in which
  words? Can `a/internal/b` import `a` where `a` imports it (the same ring as Heroes' face and private
  module)? What do Go packages do instead: a third package for the shared types, or a **type alias**
  (`type T = internal.T`, Go 1.9)? Panel 135 already read 1.9's warrant as *"refactoring, not brevity"*
  (above): read beyond it only for what this sitting's **Q1-A** asks, **a face naming a private package's
  type as its own**, and whether Go's standard library does it (an `internal` package's type re-exported
  by alias).
- **A re-export in a module system** (Q1-A): Rust's `pub use`, OCaml's module inclusion, Python's
  `__init__` imports, JavaScript's `export ... from`: each a question, and which of them keeps one name
  for one type.
- **Rust**: modules inside one crate referring to each other freely, and crates refusing cycles: is that
  so, and what does the reference say?
- **Python** (circular imports and the partially initialised module), **OCaml** (no cycle between
  compilation units), **Haskell** (`hs-boot` files for mutually recursive modules), **Java** (packages
  that cycle), **Swift** (modules that cannot cycle, files within one that see each other), **Ada**
  (`limited with`), **Nim** (cyclic imports): what each allows, and what it cost them.
- **Has a language allowed cycles and then refused them, or refused and then allowed?** Panel 031 called
  the refusal *"the reversible direction"*; a language that relaxed it later is the precedent for Q1-R, one
  that tightened is the counter-precedent.

## Question 2: a type parameter from a map literal's key

- In Rust (`HashMap::from([("a", vec![])])`), Swift (`["a": []]` passed to a generic parameter),
  Kotlin (`mapOf("a" to emptyList())`), TypeScript and Java (`Map.of("a", List.of())`): is the key's type
  inferred while the value is an empty collection, and is the empty collection's element type then taken
  from the same letter? What does each compiler say where it cannot?

## Question 3: generics on functions and not on types

- Which languages shipped **generic functions without generic types**, or the other order, and what
  happened? Go shipped neither for a decade and both together in 1.18 (design.md §4.12's historical note):
  did the type-parameters design discuss functions only, and why was it refused? Zig's `comptime` types,
  Odin's parametric structs, C's absence of both, early Java's 1.5: each a question.
- **Polymorphic recursion through a type** (Q3-R's shape beside, the critic's finding 8): a record holding
  itself at another instance, `Nest<T>` with `inner: [Nest<[T]>]`, Haskell's *nested datatypes*: what does
  a monomorphising compiler (MLton, Rust, C++ templates) do with one, refuse it or loop, and in which
  words? Panel 029's R4 read MLton for functions only.
- **The shape Rebel wrote** (a record of state beside the program's own list, lent at each call): is
  there a precedent of a standard library written that way for want of generic types, and what did its
  users get wrong?

## Question 4: a byte written as its number

- **Python 3**: `b"use"[0]` is `117`, and `f"{b"use"[0]}"` writes `117`, where Python 2's `"use"[0]` was
  `"u"`: did the 3.0 change record the trap, and does a linter warn on it? **Go**: `s[0]` is a `byte`,
  `fmt.Println(s[0])` prints `117`; does `go vet` warn on `%s` or `%v` of a byte? **Rust**:
  `s.as_bytes()[0]` displays as `117`; does Clippy have a lint? **Swift** (`UInt8` against `Character`),
  **Java** (`char` printing as a character, `byte` as a number), **C#**: which give a byte and a character
  two types so the trap cannot be written, and which refuse a byte in interpolation? **And the narrower
  refusal, Q4-R2**: does any compiler or linter refuse, or warn on, an index into a string written where a
  value becomes text (`f"{s[0]}"`, `fmt.Println(s[0])`), rather than every byte?

## Question 5: an exponent the language prints and refuses to read

- **Does any language refuse exponent literals while printing floats with an exponent?** (Panel 035's R6
  refused them for Heroes on Principle 0, `docs/panel/035-the-specs-accumulated-silences.md:175-179`.)
- **The promise that output reads back as a literal**: Python's `repr` and `eval` round trip, Go's
  `strconv.FormatFloat` and Go's literal syntax, JavaScript's `Number.prototype.toString`, JSON's number
  grammar (RFC 8259): does each promise that what it prints is a literal of the language, and in which
  document?
- **A literal written out in full**: is there a record of a dropped or added zero in a long decimal literal
  causing a defect (the hazard Rebel's 81 literals over 300 characters carry, `00-shared.md` § Question 5)?
- **Positional printing** (Q5-P): which languages print `1e-06` positionally by default, and what do they
  print for the largest `f64`?
- **Digit separators in a float literal** (Q5-U): Java 7, Python 3.6 (PEP 515), Rust, C++14's `'`, Go 1.13:
  which allow `_` in a float, where in it, and in its exponent?

## What you are asked to say

Per question: who reached it first, what it cost them, what changed their mind; which of Q1-R, Q1-R2,
Q1-A, Q1-C, Q2-R, Q2-C, Q3-R, Q3-C, Q4-R, Q4-R2, Q4-C, Q5-R, Q5-P, Q5-U and Q5-C each precedent supports or
counts against;
and the candidate prior-art lines, each from a page you read today.
