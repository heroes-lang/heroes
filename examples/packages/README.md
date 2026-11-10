# examples/packages/

**The core packages, and the conventions every one of them follows.** A core
package is a module a program fetches into its own folder and carries there, as
it carries its own modules: Go's names and Go's tree, published together as one
package of the catalogue, `core`, written in Heroes alone where Heroes can say
it and over a C library where it cannot. The language ships its runtime and
nothing else (`docs/design.md` §1.11), so these are not a standard library. They
are source, verified the way any program is: by the checker, by their own
`test` blocks and, where they bind C, by clang against the header they name.

How a program fetches a package and names it, and the reasons behind each of
§ 1's rules, are the guide's chapter on packages, in English and in Italian:
[heroes-lang.org/docs/packages](https://heroes-lang.org/docs/packages/).

**Their source is one tree**, the newest version of `core` in the local static
index at the repository's root, `packages/core/<version>/files/` (panel 235's
R15, ratified 2026-11-07; panel 238's R10 moved the index there from inside
`examples/`). A program that uses a package, `main.hero` here among
them, names it `use <core>/strings` and holds one entry, `core`, in its
catalogue `heroes.toml`, which `heroes add core` writes; the verb places the
version's files under the program's own `heroes.packages/`, the index's files
byte for byte. `main.hero` is the packages' own program, the one whose `heroes
test` runs every package's tests.

Each rule below can be checked by reading a package, and each says what holds
it: the compiler, the net's `packages` suite, or a reader's review.

## 1. Where a package lives

1. **`use <core>/<path>` binds the path's last part**: `use <core>/strings`
   binds `strings`, `use <core>/net/http` binds `http`, `use
   <core>/encoding/json` binds `json`. A call into a package is always written qualified,
   `strings.has_prefix(s: line, prefix: "#")`, since a call never reaches
   across a module through its first argument (`needs_qualifying`).
2. **A package is one module at its Go path** under `core`'s root,
   `strings.hero`, `net/http.hero`, and every name a program writes is
   declared in it: a name is reached through the module that declares it, and nothing
   re-exports. That module is the package's **face**. It declares only what a
   program calls, so the package's card, written from it, lists the face alone.
3. **A package's private modules stand under `<package>/internal/`**, Go's
   word for them, and no module outside the package `use`s one: the compiler
   refuses it, `error[private_module]` at the `use` line (panel 235's R16).
   Inside `core` a `use` starts at its own root, so `strings.hero` writes `use
   strings/internal/scan`.
4. **A program fetches the packages it uses**: `heroes add core` in its folder
   writes the entry `core` into its catalogue, with the version and the sum
   the index publishes (`index = "file://../../packages"` for a program here), and
   places the version's files under `heroes.packages/`, which the program
   commits, so `build`, `check` and `run` never fetch (panel 235's R8). The
   compiler holds the placed tree to the entry's `sum` at every build, and the
   `packages` suite holds the sum to the index's. A published version is never
   edited: a change to a package is a new version, so it never breaks a
   program's build, and a program moves to it by `heroes add core` again.
5. **No two module names differ only in case, and a `use` spells its file
   exactly.** On a case-insensitive volume a `use` in another case compiles one
   file twice under two spellings, and Linux refuses the same program (the
   `packages` suite; the compiler's own refusal is owed by
   [defect 2290](https://github.com/heroes-lang/heroes/issues/2290)).
6. **A package is tested through a program that reaches it**, the way a
   program fetches it. `main.hero` here reaches every package that no other
   package uses, and names something from each, since a `use` nothing names is
   `unused_binding`; `heroes test` on it runs the `test` blocks of every module
   it reaches, the placed package's among them (the `packages` suite).

## 2. Names

1. **A package keeps Go's name, in lower case**: `strings`, `strconv`, `path`,
   `json`, `http`, `sql`, `time`, `os`.
2. **A function keeps Go's name, in snake case, where its contract is Go's,
   and takes another where the contract differs.** Go's `(T, error)` and a
   Heroes `T?` are one contract, a failure handed back as a value with its
   cause, so Go's `sql.Open` is `sql.open` here. A sentinel (`-1`, or an empty
   value meaning absent) or a `found` or `ok` flag beside the value is another
   contract, so Go's `Index`, `LastIndex` and `Cut` take other names here; so
   is an answer holding other content, as Go's `Lines` yields each line with
   its end. A function that cannot fail keeps Go's name though Go's answers an
   `error`, as `close` does. A Go name over another contract would be the worst
   of both: a reader who knows Go would trust it.
3. **No function and no `use` binding takes a built-in's name.** `find`,
   `join`, `repeat`, `sort`, `map`, `filter`, `keys` and `len` are refused as
   function names everywhere (`builtin_name_taken`), and no package is named
   like one either: the sort by a caller's order lives in `<core>/slices`, and
   there is no `<core>/sort` (the compiler's refusal of such a binding is owed by
   [feature 2299](https://github.com/heroes-lang/heroes/issues/2299)).
4. **A parameter is named for the call that reads it.** A call names the
   argument of every parameter that shares its type with another
   (`needs_label`), so a parameter's name is part of the package's surface: Go's names where the
   contract is Go's (`s`, `prefix`, `sep`), the subject first. A parameter may
   not shadow a name of its own module (`shadowed_binding`) nor take a
   built-in's, so where Go's would, it gives way: `sql` where Go writes `query`
   beside a function `query`, `arguments` where Go writes `args`.
5. **A record is named for what it holds**, in PascalCase, and read qualified:
   `json.Value`, `http.Request`, `sql.Db`. A constant is in SCREAMING_CASE.

## 3. Failure

1. **A function that can fail on its input answers `T?`, and nothing else says
   so**: no `-1`, no empty value meaning absent, no `bool` beside a value.
2. **A code means one thing in every package.** The built-ins' own codes keep
   their meanings, `not_found`, `does_not_fit`, `not_text`, `file_not_found`,
   `read_failed`, `write_failed`, `missing_key` and `null_cstr`, and a package
   adds a code only for a failure none of them names.
3. **A code a package adds is a constant of the package**, `sql.UNIQUE`, and
   its card lists it: a misspelt constant is a compile error, where a misspelt
   string compares false and the program runs on.
4. **A message says what was asked and what was found**, in words a reader can
   act on: `no "x" in "abc"`, never `failed`.
5. **A package aborts only where its caller broke a precondition the caller
   could have checked**, as the language does on an index past the end. Input
   from outside the program never aborts.

## 4. A package over C

1. **Two layers: a thin floor and a thick face.** The floor is one module,
   named for the header its group names (`database/sql/sqlite3.hero`),
   thin by rule: one Heroes function per C function, in C's order, answering
   C's integer status, and published with a card of its own. The face is the
   package itself: every failure is a `T?`, and no status reaches a program
   unless it asks the floor. Measured on SQLite, with twenty-two mistakes
   planted and run on two machines: over the thin layer five ran to a wrong
   answer in silence, over the thick one one did, because a dropped `T?` is a
   compile error and a dropped `i64` is not.
2. **A handle C hands out is a `record ... tag` in the floor**, carrying the
   marks of spec § 13 (`acquires`, `consumes`), so a handle nobody ends aborts
   when `main` returns, saying how many. A C call that may refuse to end its
   handle, as `sqlite3_close` may, is bound `when 0`. A descriptor C hands out
   as a number, a POSIX `int` or a Windows `SOCKET`, is wrapped in C as a struct
   behind a pointer, so it carries the same marks.
3. **Bytes cross by spec § 13's fill form**, `@buf: [u8] counted_by n lent`,
   and become text by `validated_bytes()`. A write that sends only part of a
   buffer copies the part it did not send, and nothing else is copied.
4. **A C name Heroes reserves takes the route the compiler prescribes**: a
   function through a `static inline` rename in a header of the package's own,
   a field of a group's record through `#define kw_<word> <word>` in such a
   header.
5. **No package ships platform C of its own.** What differs by platform, a
   socket or a clock, is C of the language's own beside the runtime, one file
   per capability with its `#ifdef`s beneath, archived so that the linker takes
   only what a program calls; a package reaches it by naming that layer's
   header ([feature 2295](https://github.com/heroes-lang/heroes/issues/2295)).
6. **Owed before the first package over C is tagged**: through a package, a
   handle used after its end is an abort told at the package's line, where in
   one function it is a compile error
   ([feature 2298](https://github.com/heroes-lang/heroes/issues/2298)).

## 5. The shape of a function

1. **A function answers a new value.** `@` stands only where a function works
   on the caller's place by nature, a reader advancing through input or a
   builder, never to save a copy.
2. **No package prints, exits or reads `args()`**, but `log` and `os`, whose
   work it is (the `packages` suite).
3. **Generic where the operation is**: the sort by a caller's order takes the
   order as a function value.

## 6. Documentation and tests

1. **Every function has a doc comment**, the comment directly above it: its
   first sentence says what the function answers, and the codes it fails with
   are named.
2. **Every function has `test` blocks in its own module**, the edges first:
   empty, one element, absent, the largest value (the `packages` suite holds a
   doc comment and a `test` per function).

## 7. What enters a package

1. **A function enters when a program already needs it**: a count shows the
   corpus writing it by hand, the command that counts written beside the
   function, or a named acceptance program calls it; **and** the loop it
   replaces is more than three lines, or a run shows the hand copies subtly
   wrong. Measured before the first package: the nine hand copies of
   `split_lines` in `examples/` disagree on `"a\n"`, and two of nine
   `is_space` count a newline.
2. **A substitution not adopted is written beside the package**, with the loop
   a program writes instead.
3. **A card line exists only for an admitted function.**

## 8. The card

1. **Every package has a card**: its `use` line, each function's signature
   with the call a program writes beside it, labels included where two
   parameters share a type, and each code. Measured: with each call written
   beside its signature, one of the two models tested was right at the first
   try 14 times in 15, against 5 in 15 when the same card listed the
   declarations alone; the other was unchanged.
2. **A card is written from its face module by a program of the net, never by
   hand**, and the net refuses a stored card that differs, so a card is true by
   construction ([task 2294](https://github.com/heroes-lang/heroes/issues/2294)).
3. **A card spends at most 1,024 tokens** as `heroes measure` counts them, and
   all the face cards together no more than the specification itself; a
   floor's card counts apart, read only by a program that reaches the floor.

## 9. What these packages do not promise

The packages stand outside the language's `1.0.0` compatibility promise until a
later milestone rules on them. A program holds the version it fetched, and a
published version is never edited, so a package's change never breaks a
program's build: it reaches a program only when the program fetches the new
version.

## Where these rules come from

Panel 234, the opening sitting of M-core-packages, ratified by the author on
2026-11-06:
`docs/panel/234-the-core-packages-are-code-a-program-carries-in-gos-tree-under-core-a-thick-face-on-a-published-floor-a-card-that-shows-the-call-and-the-platforms-c-an-archive-beside-the-runtime.md`
and decision [2304](https://github.com/heroes-lang/heroes/issues/2304). Where
the packages live and how a program names and reaches them, § 1's rules 1,
3, 4 and 6, are panel 235's (R1, R4, R7, R15 and R16), ratified 2026-11-07 (decision
[2329](https://github.com/heroes-lang/heroes/issues/2329)), which amended panel
234's R2: until then each program carried a copy under its own `core/`, held
byte for byte to a canonical tree here, and named it `use core/strings`. A
package that breaks a rule here has a defect; a rule that turns out wrong is
changed here first, with its date.
