# Panel 227, historian (advisory)

I read every page below on **2026-10-21**, either by fetching it or through web search. Each entry
gives the URL, the day it was read and a status. `verified` means the page was read and states what
the entry quotes. `unverified` means the page could not be read, or the claim is an inference from
what was read; each such entry says which. The quotes reached me through a fetch tool, and where it
paraphrased a page I left the paraphrase out. I started no paid run and made no API call.

**One write outside the repository's root, not mine but caused by my fetches.** When the fetch tool
fetched the two PDFs below (Wirth's Oberon-07 report and Stroustrup's P1428R0), it saved each one,
on its own, into the session's tool-results folder:
`/Users/joseph/.claude/projects/-Users-joseph-Temp-heroes-heroes-lang/7e9f826e-648d-462d-93c0-58de3463ac70/tool-results/webfetch-1792579240003-ywyuux.pdf`
and `.../webfetch-1792579799835-p1ya3r.pdf`. I have no tool that can delete them, so I am
reporting them here for the coordinator. The only file I wrote myself is this one.

## verdict

**approve (advisory)**: route A's spelling and the `slice` message. The precedent also favours
**delivering that message through route E's general path** rather than as a message written only
for `slice` (see § argument).

## precedents

### Q1. How language references write their built-ins

**Lists of names, call shapes with the receiver, full signatures, or a mix:**

- **Go, the specification**: https://go.dev/ref/spec, read 2026-10-21, `verified`. It uses both
  shapes. The § Predeclared identifiers section is a bare list of names: *"Functions: append cap
  clear close complex copy delete imag len make max min new panic print println real recover"*. Each
  built-in then gets a call shape in its own section: *"append(s S, x ...E) S  // E is the element
  type of S"*, and a `Call / Argument type / Result` table whose rows include *"len(s)  string type
  string length in bytes"*. § Built-in functions opens: *"Built-in functions are predeclared. They
  are called like any other function but some of them accept a type instead of an expression as
  the first argument."*
- **Go, the `builtin` package**: https://pkg.go.dev/builtin, read 2026-10-21, `verified`. Full
  signatures, written on a package that does not exist: *"Package builtin provides documentation
  for Go's predeclared identifiers. The items documented here are not actually in package
  builtin"*. Examples are *"func append(slice []Type, elems ...Type) []Type"* and *"func len(v
  Type) int"*.
- **Python**: https://docs.python.org/3/library/stdtypes.html, read 2026-10-21, `verified`. The
  receiver is written as its type: `str.split(sep=None, maxsplit=-1)`, `str.join(iterable, /)`,
  `str.replace(old, new, /, count=-1)`.
- **Lua 5.4**: https://www.lua.org/manual/5.4/manual.html § 6.4, read 2026-10-21, `verified`. This is
  the closest match to Heroes § 9's `x.f(y)` sugar. The heading writes the receiver as the first
  parameter, *"string.rep (s, n [, sep])"* and *"string.sub (s, i [, j])"*, and the section says in
  prose: *"The string library provides all its functions inside the table `string`. It also sets a
  metatable for strings where the `__index` field points to the `string` table. Therefore, you can
  use the string functions in object-oriented style. For instance, `string.byte(s,i)` can be
  written as `s:byte(i)`."*
- **Nim**: the manual, https://nim-lang.org/docs/manual.html § Method call syntax, read 2026-10-21,
  `verified`: *"For object-oriented programming, the syntax obj.methodName(args) can be used instead
  of methodName(obj, args)."* The library, https://nim-lang.org/docs/strutils.html, read the same
  day, `verified`, writes the full signature with the first parameter, *"func repeat(s: string; n:
  Natural): string"*, and its example uses the method form: *`doAssert "+ foo +".repeat(3) == ...`*.
- **D**: https://dlang.org/spec/function.html § UFCS, read 2026-10-21, `verified`: *"A free function
  can be called like a member function when both:"* followed by two conditions.
  https://dlang.org/phobos/std_array.html, read the same day, `verified`, writes the full signature
  with the first parameter: `replicate(S)(S s, size_t n)`.
- **Kotlin**: https://kotlinlang.org/api/core/kotlin-stdlib/kotlin.text/repeat.html, read
  2026-10-21, `verified`. The receiver's type comes before the dot: *"expect fun
  CharSequence.repeat(n: Int): String"*.
- **Zig**: https://ziglang.org/documentation/master/ § Builtin Functions, read 2026-10-21,
  `verified`. Full signatures: *"@intCast(int: anytype) anytype"*, *"@memcpy(noalias dest, noalias
  source) void"*.
- **Oberon-07 (Wirth), *The Programming Language Oberon*, "Revision 1.10.2013 / 3.5.2016"**:
  https://people.inf.ethz.ch/wirth/Oberon/Oberon07.Report.pdf § 10.2, pages 1 and 13 read
  2026-10-21, `verified`. The section is a table of call shapes with argument types, under columns
  *Name / Argument type / Result type / Function*. It opens: *"The following table lists the
  predefined procedures. Some are generic procedures, i.e. they apply to several types of operands.
  v stands for a variable, x and n for expressions, and T for a type."* Rows include *"ABS(x)  x:
  numeric type  type of x  absolute value"*, *"LEN(v)  v: array  INTEGER  the length of v"* and
  *"INC(v, n)  INTEGER  v := v + n"*. In other words, Wirth's own compact report pays for argument
  types on its built-ins, which bears on panel 120's refusal (*the spec buys prose and not
  signatures*). The report takes one table per kind of built-in, not one sentence.

**Published evidence on which shape gets copied correctly:**

- **Chen, Chen, Cao, Shen, Cheung, *When LLMs Meet API Documentation* (arXiv 2503.15231, 19 March
  2025)**: https://arxiv.org/abs/2503.15231 and https://arxiv.org/html/2503.15231, read 2026-10-21,
  `verified`. The abstract says *"Example code contributes the most to advance LLMs, instead of the
  descriptive texts and parameter lists"*. The body says *"the pass rates of LLMs decrease most
  after we remove example codes"* and *"Interestingly, removing the parameter list (DelParam) even
  slightly improves generation"*. The setting was four less common Python libraries (Polars, Ibis,
  GeoPandas, Ivy; 1,017 APIs), with Qwen2.5-Coder 32B and 7B, GPT-4o-mini and
  DeepSeek-Coder-V2-Lite. These are dynamically typed libraries, so the result does not settle
  whether a *type* in a parameter list helps.
- **Wijaya et al., *ReadMe.LLM* (arXiv 2504.09798, 14 April 2025)**:
  https://arxiv.org/html/2504.09798v2, read 2026-10-21, `verified`. *"Clear function signatures are
  paired with illustrative examples"*, which *"was the structure we found worked best based on the
  libraries we experimented with"*. It covered 2 libraries.
- **Patel, Reddy, Bahdanau, Dasigi, *Evaluating In-Context Learning of Libraries for Code
  Generation* (arXiv 2311.09635)**: https://arxiv.org/abs/2311.09635, read 2026-10-21, `verified`.
  It reports *"a surprisingly high proficiency in learning novel library modules even when provided
  with just natural language descriptions or raw code implementations of the functions"*.
- **Endrikat, Hanenberg, Robbes, Stefik, *How Do API Documentation and Static Typing Affect API
  Usability?* (ICSE 2014)**: https://2014.icse-conferences.org/node/168/index.html, read 2026-10-21,
  `verified` (the abstract only; the PDF at the authors' site returned 404). This is a human study.
  The abstract says the results *"show that the benefits of static typing are strengthened with
  explicit documentation"*, *"but that this was not as strongly felt with dynamically typed
  languages."*
- **Robillard and DeLine, *A field study of API learning obstacles* (EMSE 2011)**:
  https://neverworkintheory.org/2011/11/08/a-field-study-of-api-learning-obstacles.html, read
  2026-10-21, `verified`. Over 440 Microsoft developers; five documentation factors, *"documentation
  of intent; code examples; matching APIs with scenarios; penetrability of the API; and format and
  presentation."*

### Q2. How compilers tell a call missing its first, unlabelled argument

- **Rust, `E0061`**: https://raw.githubusercontent.com/rust-lang/rust/master/tests/ui/error-codes/E0061.stderr
  (and `.rs`), read 2026-10-21, `verified`. For `fn f(a: u16, b: &str)` called as `f(0)`, it prints
  *"error[E0061]: this function takes 2 arguments but 1 argument was supplied"*, *"note: function
  defined here"* and *"help: provide the argument"*, with the whole call suggested: *"f(0, /* &str
  */);"*.
- **Rust, a library item told as a user's**:
  https://raw.githubusercontent.com/rust-lang/rust/master/tests/ui/span/missing-unit-argument.stderr
  (and `.rs`), read 2026-10-21, `verified`. `Ok()` from `core` draws the same code, *"error[E0061]:
  this enum variant takes 1 argument but 0 arguments were supplied"*, labelled *"argument #1 of
  type `()` is missing"*, with *"note: tuple variant defined here --> $SRC_DIR/core/src/result.rs"*
  and *"help: provide the argument"*. So Rust tells a standard-library item the way it tells the
  programmer's own function: one code, and a note pointing at the library's own declaration. That
  is route E's shape.
- **Rust, how it got there**: https://github.com/rust-lang/rust/pull/71827, read 2026-10-21,
  `verified`: *"Better method call error messages"*, Quantumplation, opened 3 May 2020, **closed**.
  https://github.com/rust-lang/rust/pull/92364, read 2026-10-21, `verified`: the same title,
  jackh726, merged 16 April 2022. Its commit message says the algorithm detects *"A function input is
  missing"*, *"An extra argument was provided"*, *"The type of an argument is straight up invalid"*,
  *"Two arguments have been swapped"* and *"A subset of the arguments have been shuffled"*. **What
  happened**: one general matcher replaced a count-only message, on the second attempt, two years
  after the first was opened.
- **Swift**: https://raw.githubusercontent.com/swiftlang/swift/main/include/swift/AST/DiagnosticsSema.def,
  read 2026-10-21, `verified`. The messages are `missing_argument_positional`, *"missing argument for
  parameter #%0 in call"*; `missing_argument_named`, *"missing argument for parameter %0 in call"*;
  and `wrong_argument_labels`, *"incorrect argument label%select{|s}0 in call (have '%1', expected
  '%2')"*.
- **Swift, the exact shape of 2175**:
  https://raw.githubusercontent.com/swiftlang/swift/main/test/Constraints/argument_matching.swift,
  read 2026-10-21, `verified`:
  - `func variadics5(_ x: Int, y: Int, _ z: Int...)` called as `variadics5(y: 1, 2, 3, 4, 5, 6,
    7)` is pinned to *"missing argument for parameter #1 in call"*, and no label error is pinned on
    that line.
  - `method3()` is pinned to *"missing arguments for parameters #1, 'b' in call"*, with the fix-it
    `{{13-13=<#Int#>, b: <#Int#>}}`: a placeholder for the unlabelled first argument and the
    labelled ones.
  - The file runs under `// RUN: %target-typecheck-verify-swift`. Swift's verifier
    (https://raw.githubusercontent.com/swiftlang/swift/main/lib/Frontend/DiagnosticVerifier.cpp,
    read 2026-10-21, `verified`) adds *"unexpected " ... " produced: "* for any diagnostic no
    directive expects. *That the `variadics5` line therefore draws one error and no label error* is
    my reading of the verifier's rule, so `unverified` as a run.
- **Swift, the shapes beside it** (same file, `verified`):
  - **A label on an unlabelled parameter is refused.** `func somekeywords1(_ x: Int, y: Int, z:
    Int)` called as `somekeywords1(x: 1, y: 2, z: 3)` gives *"extraneous argument label 'x:' in
    call"*, with a fix-it removing the label. This is the precedent for the critic's G6 (Heroes
    accepts `len(foo: line)`).
  - **Missing labels are named in one message.** `somekeywords1(1, 2, 3)` gives *"missing argument
    labels 'y:z:' in call"*.
  - **A whole label shape is told at once.** `f(3, y: 5)` gives *"incorrect argument labels in call
    (have '_:y:', expected 'x:_:')"*.
  - *Whether Swift tells a call into its standard library this way*: `unverified`. I found no
    standard-library example (searched: "missing argument for parameter 'count' in call" with
    `String(repeating:)`).
- **Go, a built-in told by a message of its own**:
  https://raw.githubusercontent.com/golang/go/master/src/go/types/builtins.go, read 2026-10-21,
  `verified`. The built-in arity check sets `"not enough"` / `"too many"` and reports
  `check.errorf(argErrPos(call), WrongArgCount, invalidOp+"%s arguments for %v (expected %d, found
  %d)", ...)`, which gives counts only.
  https://raw.githubusercontent.com/golang/go/master/src/go/types/call.go, read the same day,
  `verified`, tells an ordinary function under the **same code** `WrongArgCount` but with more:
  `"%s arguments in call to %s"`, `"have %s"`, `"want %s"`.
  https://go.googlesource.com/go/+/ec5b6406b76462a1a97ce19f86c5c23ac1d2c830, read 2026-10-21,
  `verified`: Matthew Dempsky, 28 October 2016, *"Use "have" and "want" and multiple lines like
  other similar error messages"*, fixing #17650. *Why built-ins get counts only* (many of them are
  generic, as `len` is): `unverified`, an inference.
- **Go, a missing receiver once told as that**:
  https://go.googlesource.com/go/+/32f2f72c4716debf658eaec1c400a35b2de7d51d, read 2026-10-21,
  `verified`. Michael Matloob, authored 29 December 2014, committed 18 June 2015, *"provide the more
  helpful error message "not enough arguments in call to method expression Type.Method""*.
  https://raw.githubusercontent.com/golang/go/master/test/fixedbugs/issue8385.go, read the same day,
  `verified`: the test now accepts *"not enough arguments in call to method expression
  Fooer.Foo|incompatible type|not enough arguments"*. The alternation was added by commit `5a25a3f`,
  *"test: recognize gofrontend error messages"*, 15 December 2020
  (https://github.com/golang/go/commits/master/test/fixedbugs/issue8385.go, `verified`). Today's
  `types2/call.go` and `go/types/call.go` (read 2026-10-21, `verified`) hold no error text naming a
  missing receiver. **What happened**: the dedicated wording is absent from Go's present type
  checker. *When and why it left*: `unverified`.
- **CPython, built-ins told by messages of their own**:
  https://raw.githubusercontent.com/python/cpython/main/Python/getargs.c, read 2026-10-21,
  `verified`. The built-ins say *"%.200s expected %s%zd argument%s, got %zd"* (counts) or
  *"%.200s%s missing required argument '%U' (pos %d)"*.
  https://raw.githubusercontent.com/python/cpython/main/Python/ceval.c, read the same day,
  `verified`. A Python function says *"%U() missing %zd required %s argument%s: %U"*, naming the
  parameters.
- **Elm**: https://raw.githubusercontent.com/elm/compiler/master/compiler/src/Reporting/Error/Type.hs,
  read 2026-10-21, `verified`. Too many arguments: *`thisFunction <> " expects " <> D.args n <> ",
  but it got " <> show numGivenArgs <> " instead."`*. Too few, as a hint: *"It looks like it takes
  too few arguments. I was expecting " ++ show (y - x) ++ " more."*
- **TypeScript**: https://raw.githubusercontent.com/microsoft/typescript-go/main/internal/diagnostics/diagnostics_generated.go,
  read 2026-10-21, `verified` for the strings: 2554 *"Expected {0} arguments, but got {1}."* and
  6210 *"An argument for '{0}' was not provided."* The pairing of the two on one call is shown on
  a secondary page, https://bobbyhadz.com/blog/typescript-expected-0-arguments-but-got-1 (read
  2026-10-21). *That a `lib.d.ts` function is told the same way*: `unverified`, no example found.
- **Kotlin**: https://github.com/Kotlin/kotlinx.coroutines/issues/2625, read 2026-10-21, `verified`
  as the issue's title: *"Kotlin: No value passed for parameter 'onCancellation'"*, on a call to a
  library overload of `resume`. Opened 5 April 2021; the maintainer called it expected behaviour.
  The compiler's message source: `unverified` (the path I tried returned 404).

### Q3. A count typed unsigned by the standard library, told with the conversion

- **Rust, the count**: https://raw.githubusercontent.com/rust-lang/rust/master/library/alloc/src/str.rs,
  read 2026-10-21, `verified`. *"pub fn repeat(&self, n: usize) -> String"*, `#[stable(feature =
  "repeat_str", since = "1.16.0")]`, *"This function will panic if the capacity would overflow."*
- **Rust, the message**: https://raw.githubusercontent.com/rust-lang/rust/master/tests/ui/numeric/numeric-cast.stderr,
  read 2026-10-21, `verified`. `foo::<usize>(x_i64)` gives *"error[E0308]: mismatched types"* and
  *"expected `usize`, found `i64`"*, then *"help: you can convert an `i64` to a `usize` and panic if
  the converted value doesn't fit"*, suggesting *"foo::<usize>(x_i64.try_into().unwrap());"*.
  **It is told with the conversion.** The source,
  https://raw.githubusercontent.com/rust-lang/rust/master/compiler/rustc_hir_typeck/src/fn_ctxt/suggestions.rs
  (read 2026-10-21, `verified`), passes that suggestion as `Applicability::MachineApplicable`,
  although it can panic.
- **Rust, how it got there**:
  - https://github.com/rust-lang/rust/pull/47247, `verified`: *"Suggest casting on numeric type
    error"*, estebank, merged 22 January 2018. At first it suggested `.into()` only, since
    *"`TryFrom` is confined to nightly"*.
  - https://github.com/rust-lang/rust/pull/71051, `verified`: *"Suggest .into() over try_into()
    when it would work"*, merged 13 April 2020, fixing #70851, *"`.try_into().unwrap()` is suggested
    even when `.into()` would work"*; its reason, `.into()` *"cannot fail"*.
  - https://github.com/rust-lang/rust/pull/73195, `verified`: *"Provide suggestion to convert
    numeric op LHS rather than unwrapping RHS"*, merged 12 June 2020, *"since the latter may
    panic"*.

  **What happened**: twice, the help was steered toward a conversion that cannot fail wherever one
  exists. Where none exists, as from a signed to an unsigned count, the panicking one stands.
- **Zig, told with the reason but not the conversion**:
  https://raw.githubusercontent.com/ziglang/zig/master/test/cases/compile_errors/integer_cast_truncates_bits.zig,
  read 2026-10-21, `verified`. *"error: expected type 'u64', found 'i8'"*, with *"note: unsigned
  64-bit int cannot represent all possible signed 8-bit values"*. The note names no `@intCast`.
- **D, unsigned count with an implicit conversion**: `replicate(S)(S s, size_t n)` (above,
  `verified`). https://lists.puremagic.com/pipermail/dip.ideas/2024-May/000334.html, read
  2026-10-21, `verified`: Paul Backus, 12 May 2024, *"Deprecate implicit conversion between signed
  and unsigned integers"*, *"D inherited these implicit conversions from C and C++, where they are
  widely regarded as a source of bugs."* D Bugzilla 12919: `unverified` (the archive returned 522).
- **Signed counts refused at run time, the shape of route D**:
  - **Go**: https://pkg.go.dev/strings#Repeat, `verified`. *"func Repeat(s string, count int)
    string"*, *"It panics if count is negative or if the result of (len(s) * count) overflows."*
  - **Kotlin**: (above) `CharSequence.repeat(n: Int)`, *"Throws IllegalArgumentException when n <
    0."*, `verified`.
  - **Java**: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/String.html,
    `verified`. *"public String repeat(int count)"*, *"IllegalArgumentException - if the count is
    negative."*, Since 11.
  - **Swift**: https://raw.githubusercontent.com/swiftlang/swift/main/stdlib/public/core/StringRangeReplaceableCollection.swift,
    `verified`, `init(repeating repeatedValue: Character, count: Int)`. Its book,
    https://raw.githubusercontent.com/swiftlang/swift-book/main/TSPL.docc/LanguageGuide/TheBasics.md,
    read 2026-10-21, `verified`, states the reason: *"Use `UInt` only when you specifically need an
    unsigned integer type with the same size as the platform's native word size. If this isn't the
    case, `Int` is preferred, even when the values to be stored are known to be nonnegative. A
    consistent use of `Int` for integer values aids code interoperability, avoids the need to
    convert between different number types, and matches integer type inference"*. That is the cost
    measurement 050 counted at turn 1, named in advance.
  - **C++, its designer**: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1428r0.pdf,
    page 1 read 2026-10-21, `verified`. Bjarne Stroustrup, P1428R0, *"Subscripts and sizes should
    be signed"*; the page prints *"Date: 2018-01-18"*, in the `papers/2019/` folder.
- **A subrange, a third route**: https://nim-lang.org/docs/system.html, read 2026-10-21, `verified`:
  *"Natural = range[0 .. high(int)]"*. The manual's § Subrange types, `verified`: *"Assignments from
  the base type to one of its subrange types (and vice versa) are allowed."* and *"Assigning any
  other value to a variable of type Subrange is a panic (or a static error if it can be determined
  during semantic analysis)."*

### Searched and not found (questions, not premises)

- **No study comparing the three shapes.** Nothing compares a bare name list, a call shape with
  its receiver and a full signature for what a reader or a model copies. I searched "documentation
  format ablation signature vs example", "LLM unseen language reference manual documentation
  format" and "API documentation components parameter list versus code examples". The five studies
  above remove or add documentation components, or survey readers; none varies the receiver.
- **No Rust test of a `std` method called by path without its receiver.** Searched: "argument #1 of
  type" with "this method takes".
- **Not run: what today's `go` prints for `T.M()`.** That it lists the receiver's type under
  `want` is an inference from `call.go`.

## argument

Every reference I read writes a method-like built-in with its receiver or first argument: Python
`str.split(sep=None, maxsplit=-1)`, Kotlin `CharSequence.repeat(n: Int)`, Lua `string.rep (s, n [,
sep])` beside its object-style sentence, Nim `repeat(s: string; n: Natural)`. Today's `slice(from:,
to:)` is the departure from that, and route A returns to it. Swift (`missing argument for parameter
#1 in call`, no label error pinned) and Rust (`argument #1 ... is missing`, the whole call
suggested) both name the missing first argument. Rust's general matcher tells `core`'s `Ok()`
exactly as it tells a user function. Go and CPython special-case their built-ins and say less. So:
A's content, best carried by E's general path. For `repeat`, Rust's `help:` names the conversion.

## condition

Three findings would change this reading:

- **A controlled study, with a model or a person, showing that a receiver-qualified call shape in a
  reference is copied *worse* than a bare one.** I found none.
- **A compiler that moved built-in arity errors onto its general matcher and moved them back
  because the general message misled.** That would weigh against E. Go's dedicated
  missing-receiver wording of 2015 is absent from today's checker, and the direction of that change
  is toward the general message; why it happened is unverified.
- **The compiler-engineer finding that Heroes' built-ins cannot carry one declared shape.** If some
  accept several types, as Go's `len` does, then Go's count-only message for built-ins becomes the
  relevant precedent, and E's note would need one shape per accepted type.

On the shape beside 2175: Swift refuses a label on an unlabelled parameter (*"extraneous argument
label 'x:' in call"*), which bears on the critic's G6.
