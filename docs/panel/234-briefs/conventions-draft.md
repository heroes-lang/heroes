# The core packages' conventions: the coordinator's draft for panel 234

A draft for the seats to judge, item C of the sitting. Every rule here is a
proposal; a fact the compiler already enforces is marked with the probe that
shows it (`probe/README.md`, run 2026-11-05 on the tree's compiler). The
author asked for this page before the first package (instruction 3 of
2026-11-05: *coherent, clean, idiomatic, organised extremely well; the code
people will read to learn the language*), so it is written as the page a
stranger would open, and its rules are the ones a reviewer can check by
reading a package.

## 1. The tree

1. **Go's tree, under one directory `core/`**, reached by `use core/<path>`,
   which binds the path's last part: `use core/strings` binds `strings`,
   `use core/net/http` binds `http`, `use core/encoding/json` binds `json`
   (probe 2). The alternative the sitting weighs is the same tree with no
   `core/`, `use strings` (probe 1); item B rules between them.
2. **One package is one module, one file, at its Go path**: `core/strings.hero`,
   `core/net/http.hero`. Every name a program writes, a function, a record, a
   variant, a constant, is declared in that file, because a name is reached
   through the module that declares it and nothing re-exports.
3. **A package past one module's size** (`.claude/rules/module-shape.md`)
   keeps every name a program writes in its file and moves its private work
   into modules under its own directory, `core/encoding/json/parse.hero`.
   Those are modules no program `use`s, by convention, since the language has
   no visibility.
4. **A package over C keeps its C in one module of its own**, named for the
   header it binds, under the package's directory:
   `core/database/sql/sqlite3.hero` holds the `extern` group and, beside each
   member, the one Heroes function the package calls, since an `extern` is
   never callable from another module (probe 3; panel 033 R5). That module is
   the package's floor and never its face.
5. **No two module names differ only in case**: this Mac's file system does
   not tell `call_index.hero` from `call_Index.hero` (probe 4's own history).
6. **A root driver runs every package's tests**: one file at the root that
   holds `core/`, so `heroes test` on it runs every `test` block of every
   module it reaches, **transitively** (the critic's run, F31: a driver
   reaching `core/net/http` alone ran `http`'s test and `strings`'s); it
   reaches only the packages no other package uses, and it must **name**
   something from each package it `use`s, since a `use` nothing names is
   `unused_binding` (F31). A package that `use`s another cannot be tested
   alone (probe 1).

## 2. Names

1. **Modules are Go's names, lower case**: `strings`, `strconv`, `path`, `json`,
   `http`, `sql`, `time`, `os`.
2. **A function keeps Go's name, in snake case, where its contract is Go's**,
   `HasPrefix` becoming `has_prefix`; **where the contract differs, the name
   differs too** (the author's rule, 2026-11-05). Go's `strings.Index` answers
   `-1` when nothing is found; a Heroes function answering `i64?` that fails
   `not_found` is not called `index`. **The boundary, proposed by the
   coordinator for the seats to judge** (the critic's F23: the rule states
   none): Go's `(T, error)` and a Heroes `T?` are **one** contract, a failure
   handed back as a value with its cause, so `os.ReadFile` or `sql.Open` keeps
   its name; a **sentinel** (`-1`, an empty value meaning absent) or a
   **`found` or `ok` flag** beside a value is **another** contract, since a
   reader must know the sentinel or the flag, so `Index`, `LastIndex` and
   `Cut` change their names; and a function whose answer holds other content
   (Go 1.24's `Lines` yields each line with its end) is another contract. Measured: a program writing Go's
   contract on a `T?` is refused `bad_operand`, and the fix offered is
   `.must()`, an abort (probe 4); a program writing Go's name where the
   package has another gets no suggestion, `index` against `index_of` (probe 4).
3. **A function never takes a built-in's name**: `find`, `join`, `repeat`,
   `sort`, `map`, `filter`, `keys`, `len` are refused everywhere,
   `builtin_name_taken` (probe 3 for `find`; the critic ran all eight, F6). A module may take one, `use sort` beside the
   built-in `sort` compiles and runs (probe 3), and item M rules whether one
   should.
4. **Parameters are named for the call that reads them**, because a call into a
   package is always written qualified (UFCS does not reach across a module,
   `needs_qualifying`, probe 5) and a call with two parameters of one type
   names every argument (`needs_label`, probe 5): a parameter's name is part of
   the package's surface. Go's parameter names where the contract is Go's
   (`s`, `prefix`, `sep`), the subject first.
5. **Records are named for what they hold**, in PascalCase, read qualified:
   `json.Value`, `http.Request`, `sql.Db`. Constants in SCREAMING_CASE.

## 3. Failure

1. **A function that can fail on its input answers `T?`** and nothing else: no
   `-1`, no empty value meaning absent, no `bool` beside a value, no sentinel.
2. **A code is snake_case, stable, listed on the package's card, and means one
   thing in every package.** The built-ins' own codes keep their meanings:
   `not_found`, `does_not_fit`, `not_text`, `file_not_found`, `read_failed`,
   `write_failed`, `missing_key` and `null_cstr` (spec § 10, § 11, § 13; the
   eighth found by the critic, F35). A package adds a code
   only for a failure none of those names.
3. **A message says what was asked and what was found**, in words a reader can
   act on: `no "x" in "abc"`, never `failed`.
4. **A package aborts only where its caller broke a precondition it could have
   checked** (an index past the end, as the language does); input from outside
   the program never aborts.
5. **A C status never reaches the caller as a number**: the package turns it
   into a `T?` with a code, keeping C's own text in the message where C gives one.

## 4. The shape of a function

1. **A function answers a new value**; `@` only where the function works on the
   caller's place by nature (a reader advancing through input, a builder), never
   to save a copy.
2. **No package prints, exits, or reads `args()`**, except the packages whose
   job it is (`log`, `os`).
3. **A handle C hands out is a `record ... tag` in the C module, carrying the
   marks spec § 13 gives it** (`acquires`, `consumes`), so a handle nobody ends
   aborts at exit, saying how many; the package's face ends it in one function
   whose parameter is `@`.
4. **Generic where the operation is**, never `any`: `sort.by<A>(xs: [A], lt:
   ...)`.

## 5. Documentation and tests

1. **Every function has a doc comment**: its first sentence says what it
   answers; the codes it can fail with are named.
2. **Every function has `test` blocks in its own module**, the edge cases
   first: empty, one element, absent, the largest value.

## 6. Admission

1. **A function enters a package when a program already needs it**: the corpus
   wrote it by hand (a measured copy) or an acceptance program calls it, **and**
   the hand-written loop it replaces is more than three lines or easy to get
   subtly wrong (Goose's test, `docs/roadmap/milestones/M-core-packages.md`,
   2026-10-25). Each substitution is measured against the loop it replaces, and
   one not adopted is written down beside the package with the reason.

## 7. The card

1. **Every package has an interface card**: its `use` line, each function's
   signature and one line, each code; under a budget per package measured with
   `heroes measure` (item E rules how it is made and held true).
