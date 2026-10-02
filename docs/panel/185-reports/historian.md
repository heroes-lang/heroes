# Panel 185, historian's report

Seat: historian (advisory, no veto). Written as it went, 2026-10-02, from the
shared brief `docs/panel/185-briefs/00-shared.md` and the seat's brief
`docs/panel/185-briefs/historian.md`. In the repository this seat read those two
files and panel 184's historian report
(`docs/panel/184-reports/historian.md`), as a map of what was searched before;
**no row below is carried from it unrefetched**.

**Method.** Every precedent below was fetched or searched in this session, and
each carries the address it came from, its date or version, and the words it
says, quoted. A search that found nothing is reported as the words searched. A
claim this seat could not confirm at a source says `unverified` beside it. The
project author's book was not used as a source for any row.

**The caveat on the quotes.** Pages were read through the session's fetch tool,
which returns a page as seen by a small model asked to quote verbatim. A quote
from a raw source file or from GitHub's API is the document's text; a quote from
a rendered page is that page as the tool returned it; the Oberon-07 report was
read from the PDF's own pages. Where the tool added reasoning of its own (it did
on Rust's `while true` and on a Scala test file), that reasoning is not quoted.
A reader who needs a quote to the letter re-fetches the address.

**Interruptions and corrections, recorded.** The session stopped once at the
account's session limit, before Q1 was written; the coordinator's resume
message of 01:44 says it reset at 01:40. Every row below was fetched in this
session, before or after the stop. The file-editing tool is not available here,
so each section was added by rewriting the file whole. Corrections made that
way: P1.8's Swift quote first read *"that more complex"*, and the page says
*"that are more complex"*; P1.11 first said GHC 7.6.1's year was unverified,
and it is now sourced; Q3's table first said Swift's `exit` returns `Never`,
which this seat had not fetched, and it now says `fatalError` (sourced) and
`exit` unverified. A last read of the whole report, before closing, corrected
five more: Q4's prediction first cited a count the shared brief does not hold
(this seat has no shell, so it is now stated as unrun); Q5's table first gave
ESLint's year from panel 184's row; Q5 first said panel 184's census matched
its historian's condition, which this seat cannot know; Swift's and Rust's
handling of a spaced `-` on a string or in a pattern are now marked as
readings; and RuboCop's severity, not read, is no longer stated.

**Nothing of Heroes was run by this seat**, which has no shell in this session.
Where a sentence says what a precedent predicts for Heroes, it is a prediction;
where it reads a Heroes fact it quotes the shared brief's measurement.

**The headings.** The shared brief asks each report for `verdict`, `section`,
`cost`, `prediction` and `condition` per question; this seat's brief asks for
`verdict`, `precedents`, `argument` and `condition`. Each question below
carries the verdict per route, the precedents, the argument, a prediction and a
condition. **`section` and `cost` are not this seat's to give**: it cites no
design.md or spec section as a premise beyond what the shared brief quotes, and
it measured no line of compiler, no spec token and no program (no shell, no
build). Where a precedent records a cost of its own (a false-positive count, an
issue open for years, a usability cliff), that cost is in its row.

**Verdict (advisory), at a glance**, the reasons per question below:

| question | approve | approve as conservative | object |
|---|---|---|---|
| Q1 macro-only C name | (1a), (1a') read as the more robust; (1d) as fallback; (1c) optional | | (1b) |
| Q2 one-line arm | (2a) | (2c) | (2b) |
| Q3 value block that leaves | (3a) with R4's predicate; `assert false` only if never switched off | | (3b) |
| Q4 the certain `-` | (4a) | (4b) | (4c) |
| Q5 the forgotten `f` | (5c) | | (5a), (5b); (5d) unless measured clean |

Status: all five questions written; the summary for the synthesis is at the end.

## Q1. A macro-only C name in an `extern` group

### Precedents

**P1.1 Zig, `translate-c` / `@cImport`: a function-like macro becomes a
generic inline function, and one it cannot translate becomes a compile error
at the point of use. Verified.** The language reference's text, as corrected
in the commit *"Fix minor langref typos"* (Zhora Trush, 2022-09-24,
<https://github.com/ziglang/zig/commit/f7f15e99c418ab0efac268e9087ce45d55744edc.patch>):
*"C Translation makes a best-effort attempt to translate function-like macros
into equivalent Zig functions. Since C macros operate at the level of lexical
tokens, not all C macros can be translated to Zig."*, and the ones that cannot
are *"demoted to @compileError"*. What the translation looks like, from Zig's
own test file at tag 0.14.0,
<https://raw.githubusercontent.com/ziglang/zig/0.14.0/test/translate_c.zig>:
`#define MIN(a, b) ((b) < (a) ? (b) : (a))` becomes `pub inline fn MIN(a:
anytype, b: anytype) @TypeOf(if (b < a) b else a)`, and `#define noreturn
_Noreturn` becomes `pub const @"noreturn" = @compileError("unable to translate
C expr: unexpected token '_Noreturn'");`. **The parameters are not typed**:
`anytype` takes whatever the caller passes, and the translated body is
type-checked by Zig at each use. The limit is real at the user's end: a
libssh2 user hit *"unable to translate C expr: unexpected token 'a string
literal'"* on a function-like macro (<https://ziggit.dev/t/libssh2-issue/7020>,
date not read), and the advice given in that ecosystem is a C file of one's own
that wraps the macro (the same search's summary; the post's words unverified).

**P1.2 D, ImportC: the same shape as Zig. Verified.**
<https://dlang.org/spec/importc.html>, fetched 2026-10-02: *"Many macros look
like functions, and can be treated as template functions: `#define DEF(a) (a +
x)` ... `auto DEF(T)(T a) { return a + x; }`"*, with the caveat that *"Some
macro formulations, however, will not produce the same result"*. The parameter
is a template type `T`: untyped, as in Zig.

**P1.3 Rust, `bindgen`: a function-like macro produces nothing, for nine years.
Verified.** Issue <https://github.com/rust-lang/rust-bindgen/issues/753>,
*"Functional C macros are not expanded."*, opened 2017-06-16, **open** (GitHub's
API, fetched 2026-10-02). Pull request
<https://github.com/rust-lang/rust-bindgen/pull/3350>, *"Add
`--translate-function-macros` to translate C function-like macros to Rust
`const fn`"*, opened 2026-03-14, **open, not merged**: *"Function-like C macros
produce no output from bindgen, which means any constant defined through them
silently disappears."*; it would infer parameter types from operator context
and *"Macros that can't be safely translated are skipped; a warning is
emitted"* (the API's body as the tool returned it). What bindgen did ship is a
**second clang round for object-like macros only**: `--clang-macro-fallback`,
bindgen 0.70.0 (2024-08-16, per its CHANGELOG,
<https://raw.githubusercontent.com/rust-lang/rust-bindgen/main/CHANGELOG.md>),
from PR <https://github.com/rust-lang/rust-bindgen/pull/2779> (opened
2024-03-11, merged 2024-04-01); its doc comment
(<https://docs.rs/bindgen/latest/bindgen/struct.Builder.html>, bindgen 0.73.2):
*"Use Clang as a fallback for macros that fail to parse using `CExpr`. This
uses a workaround to evaluate each macro in a temporary file. Because this
results in slower compilation, this option is opt-in."* LWN, *"Improving
bindgen for the kernel"*, Daroc Alden, 2024-10-09
(<https://lwn.net/Articles/992693/>): *"bindgen can now capture the name of the
macro, create a temporary C file with a main function that returns the value
of the macro, and then use Clang to compile it"*, opt-in *"due to performance
considerations"*.

**P1.4 bindgen `--wrap-static-fns`: the compiler writes the C wrapper from the
declaration, for `static inline` functions, and not for macros. Verified.**
CHANGELOG, bindgen 0.64.0: *"Added a new set of flags ... `--wrap-static-fns`
... to generate C function wrappers for `static` or `static inline`
functions."*; 0.64.0 published 2023-02-07 (crates.io API,
<https://crates.io/api/v1/crates/bindgen/0.64.0>). The doc comment: *"generate a
C source file with non-`static` functions that call the `static` functions
found in the input headers"*. Its use in Linux, Alistair Francis, *"[PATCH v5
00/11] rust: bindings: Auto-generate inline static functions"*, 2025-01-06
(<https://lkml.iu.edu/hypermail/linux/kernel/2501.0/04593.html>): the generated
file holds e.g. `crypto_shash_descsize__extern(...) { return
crypto_shash_descsize(tfm); }`; *"The Rust code currently uses rust_helper_*
functions ... But this is a hassle as someone needs to write a C helper
function."*; and the approach **does not work for C macros** (the cover
letter as the tool summarised it; its exact words unverified), with inline-asm
and bit-mask functions failing to compile, so *"functions must be manually
enabled"*. **This is route (1a')'s mechanism, shipped, for the case where a
prototype exists to copy.**

**P1.5 The Linux kernel: route (1d), by hand, at scale. Verified.**
<https://raw.githubusercontent.com/torvalds/linux/master/rust/helpers/helpers.c>,
fetched 2026-10-02: *"Non-trivial C macros cannot be used in Rust. Similarly,
inlined C functions cannot be called either. This file explicitly creates
functions ("helpers") that wrap those so that they can be called from Rust."*
The file includes one `.c` per subsystem (the tool counted 56 `#include`
lines; that count is the tool's, unverified).

**P1.6 Rust's `libc` crate: the macro re-written by hand, typed. Verified.**
<https://docs.rs/libc/latest/libc/fn.WEXITSTATUS.html> (libc 0.2.189): `pub
const extern "C" fn WEXITSTATUS(status: c_int) -> c_int`, its source in
`src/unix/linux_like/mod.rs` inside a `safe_f!` block: `pub safe fn
WEXITSTATUS(status: c_int) -> c_int { (status >> 8) & 0xff }`. **The binding
does not reach the header's macro at all**; it restates its body per platform.

**P1.7 Nim, `importc` + `header`: the macro reached by name, its parameters
typed by the declaration and checked by nobody but the C compiler. Verified.**
<https://raw.githubusercontent.com/nim-lang/Nim/devel/lib/posix/posix_other.nim>,
fetched 2026-10-02: `proc WEXITSTATUS*(s: cint): cint {.importc, header:
"<sys/wait.h>".}` with the comment `## Exit code, if WIFEXITED(s)`. On Linux
amd64 the same standard library re-writes it instead,
<https://raw.githubusercontent.com/nim-lang/Nim/devel/lib/posix/posix_linux_amd64.nim>:
`proc WEXITSTATUS*(s: cint): cint = (s and 0xff00) shr 8`. Why the first form
works: the header pragma *"specifies that it should not be declared and
instead, the generated code should contain an `#include`"*, and the manual's
own `nodecl` example imports a macro, `EACCES {.importc, nodecl.}: cint #
pretend EACCES was a variable, as Nim does not know its value` (Nim manual
text, read from the third-party mirror
<https://nim-docs.readthedocs.io/en/latest/manual/pragmas/specific_pragmas/>;
the official page, <https://nim-lang.org/docs/manual.html>, 2.2.12, reached the
tool truncated). **Nim emits the call and includes the header; nothing probes
the declaration**, so a wrong Nim signature against a real function is caught
only as far as C's implicit conversions allow. That is the gap panel 092's probe
closed for Heroes, and the reason it costs the macro case.

**P1.8 Swift's Clang importer: function-like macros are not imported.
Verified.** <https://developer.apple.com/documentation/swift/using-imported-c-macros-in-swift.md>,
fetched 2026-10-02 (copyright 2026): *"Swift automatically imports simple,
constant-like macros ... C macros that are more complex than simple constant
definitions have no counterpart in Swift. ... In Swift, you can use functions
and generics to achieve the same results without any compromises."* No
diagnostic text for the refusal was found; whether the importer names the
macro when a program calls it is unverified.

**P1.9 Go, cgo: niladic function-like macros since Go 1.10, through a
generated wrapper; a macro with arguments, not. Verified, the last clause a
reading.** Go 1.10's notes (<https://go.dev/doc/go1.10>; released 2018-02-16,
<https://go.dev/doc/devel/release>): cgo *"now supports the use of niladic
function-like macros."* The commit, Hiroshi Ioka, 2017-05-23,
<https://github.com/golang/go/commit/03876af91c50c6e0227218a856f037dd20a45729.patch>,
*"cmd/cgo: support niladic function-like macros"*: *"macros which can be
represented as niladic functions"*, by *"a thin wrapper function per macro"*;
it fixes <https://github.com/golang/go/issues/10715>, *"cmd/cgo: can't access
C.Stdout if stdout is a macro"* (2015-05-06 to 2017-08-30, milestone Go1.10).
How cgo learns what a name is, `guessKinds` in
<https://raw.githubusercontent.com/golang/go/master/src/cmd/cgo/gcc.go>:
*"guessKinds tricks gcc into revealing the kind of each name xxx for the
references C.xxx in the Go input"*, by compiling one snippet per question,
`__typeof__(name) *x` (*"not-declared"*), `name *x` (*"not-type"*), an `enum`
constant, a `double` constant and a `char[]` literal, and reading which fail;
the failure is *"could not determine what C.%s refers to"*. **That `C.WEXITSTATUS(s)`
is refused is this seat's reading of that code**: `__typeof__(WEXITSTATUS)`
names a function-like macro without its parentheses, which the preprocessor
does not expand, so the name reads as undeclared. Unrun, and no report of the
exact refusal was found (search *"C.WEXITSTATUS" cgo* returned gccgo's
`libgo/go/syscall/wait.c`, below, and no cgo report).

**P1.10 gccgo's runtime: the macro wrapped in a C file of its own. Verified.**
<https://gnu.googlesource.com/gcc/+/trunk/libgo/go/syscall/wait.c>
(*"Copyright 2011 The Go Authors"*): *"We use C code to extract the wait status
so that we can easily be OS-independent."*, `intgo ExitStatus (uint32_t *w) {
if (!WIFEXITED (*w)) return -1; return WEXITSTATUS (*w); }`. Route (1d), inside
a compiler's own runtime.

**P1.11 GHC, the `capi` calling convention: routes (1a') and (1c) together,
since 2012. Verified.** GHC 7.6.1's release notes
(<https://downloads.haskell.org/ghc/7.6.1/docs/html/users_guide/release-7-6-1.html>):
*"There is a new FFI calling convention `capi`, enabled by the `CApiFFI`
extension."*; 7.6.1 was announced on the haskell@ list by 2012-09-06 (a reply
to *"ANNOUNCE: GHC version 7.6.1"* dated that day,
<https://mailman.haskell.org/archives/list/haskell@haskell.org/message/X3KNMNPM6TKJ4BA2AFOHALGLA5D7DBVL/>).
The user's guide today (GHC 9.14.1,
<https://downloads.haskell.org/ghc/latest/docs/users_guide/exts/ffi.html>):
CApiFFI, *"Since: 7.6.1"*; *"Rather than generating code to call `f` according
to the platform's ABI, we instead call `f` using the C API defined in the
header `header.h`. Thus `f` can be called even if it may be defined as a CPP
`#define` rather than a proper function."* What it emits, Ben Gamari, *"Best
practices for foreign imports"*, Well-Typed, 2021-08-02
(<https://well-typed.com/blog/2021/08/capi-usage>): *"GHC will construct a C
source file which `#include`'s `stdio.h`. and defines a stub function which
performs the call"*, e.g. `HsInt32 ghczuwrapperZC0ZCmainZCHelloZCputs(void* a1)
{ return puts(a1); }`, and the advice: *"use `foreign import capi` for imports
of libraries not under their control (e.g. system libraries)"*. **The mark is
on the line** (`capi` instead of `ccall`), and the wrapper's parameter types
come from the declared Haskell types. **Its known cost**, GHC ticket #15531,
*"CApiFFI generates bad prototypes for pointers of `Foreign.C` types"*, hvr,
2018-08-17 (<https://mail.haskell.org/pipermail/ghc-tickets/2018-August/069378.html>):
the wrapper was written `(void* a1, void** a2)` for `int getfilecon(const char
*path, char ** con)`, and *"modern C compilers will refuse to coerce a pointer
`void**` into an argument to a function expecting a `char**`"*. Whether #15531
is fixed today is unverified (the GitLab page refused the tool).

**Searched and not confirmed.** SWIG: the search engine's summary says SWIG's
preprocessor expands macros with arguments but *"they are ignored during the
creation of wrapper code"*; the sentence was not found in the three SWIG
manuals fetched (<https://www.swig.org/Doc2.0/Preprocessor.html>,
<https://swig.org/Doc4.4/Preprocessor.html>, 1.3.19's). Unverified, and no row
rests on it.

### What each checks of a macro's parameters

| system | reaches `WEXITSTATUS(s)` | the parameter's type comes from | who checks it |
|---|---|---|---|
| Zig translate-c | yes, if translatable | nowhere (`anytype`) | Zig, on the translated body, per use |
| D ImportC | yes, if translatable | nowhere (template `T`) | D, per instantiation |
| bindgen | no (#753, open since 2017) | | |
| bindgen `--wrap-static-fns` | not macros; `static inline` only | the C prototype | clang, on the generated `.c` |
| Swift | no | | |
| cgo | only niladic ones, since 1.10 | | |
| Nim `importc`+`header` | yes | the Nim declaration | the C compiler, on the emitted call, with C's conversions |
| GHC `capi` | yes | the Haskell declaration | the C compiler, on the generated stub |
| Linux helpers, gccgo `wait.c`, Heroes (1d) | yes, by a hand-written C function | the helper's author | the C compiler, on the helper |
| Rust `libc`, Nim on Linux amd64 | no: the body re-written | the re-writer | the host language |

### What the precedents say together

1. **No system found types a macro's parameters from the macro.** They take the
   type from nowhere (Zig, D), from the binding's own declaration (Nim, GHC), or
   from a person (the kernel, gccgo, `libc`). A macro has no prototype to read;
   the declaration in the binding is the only typed statement there is.
2. **Where the declaration is the source, the C compiler compiling a call with
   it is the check, and it is the whole check.** GHC's stub and Nim's emitted
   call are the same thing (1a) and (1a') propose; neither GHC nor Nim adds a
   probe on top. Heroes' panel 092 probe is stricter than both for real
   functions, which is why it is the one that costs macros.
3. **(1a') has shipped twice**: GHC's `capi` for macros since 7.6.1 (2012), and
   bindgen's `--wrap-static-fns` for `static inline` since 0.64.0 (2023). Its
   recorded failure is GHC #15531: the stub's C types were derived from a
   binding type that had lost information (a pointee), and a modern C compiler
   refused the coercion. In Heroes the analogue is the C spelling a declared
   type maps to; for `WEXITSTATUS(status: i32)` it is `int32_t`, and whether
   every Heroes FFI type has one spelling that a macro's body accepts is a
   compiler question, not a precedent.
4. **A second compiler round for macros is a known shape, and opt-in where
   found**: bindgen's `--clang-macro-fallback` (object-like only, *"opt-in"* for
   speed), cgo's `guessKinds` (one round of snippets classifying every name,
   always on). Heroes' existing second round for a handle's `struct` tag, paid
   only by a program that needs it (the shared brief, defect 037), is the cgo
   shape.
5. **The refusal route (1b) is where bindgen and Swift stand, and the cost is
   recorded**: bindgen's issue has been open nine years, and the kernel
   carries a directory of hand-written helpers, which is what (1d) asks every
   Heroes program to write. Swift's refusal is documented as a design stance,
   with the advice *"use functions and generics"*, which a C header's user
   cannot take.

**Defect 145, beside it.** cgo's `guessKinds` is the precedent for asking clang
several questions about one name in one round and reading which fail: its
*"not-declared"* against *"not-type"* is the typedef-against-misspelling split
the critic's `sizeof` query makes. Riding (1a)'s round is the cgo design;
whether it is cheap in Heroes is the compiler-engineer's to measure.

### Verdict per route (advisory)

- **(1a) a second round, probed with `(T){0}` arguments: approve.** It is
  Nim's and GHC's check (the C compiler on a call built from the declaration)
  with a classification round in front, which is cgo's. No precedent checks
  more for a macro.
- **(1a') the compiler writes the wrapper: approve, and of the two this seat
  reads it as the more robust**, because it is the one with a shipped record
  for exactly this case (GHC `capi`) and because the wrapper is a real
  function, so the panel 092 probe applies to it unchanged. The record also
  names its failure mode (#15531), which a Heroes census can check before it
  lands.
- **(1b) refused, with a message naming the repair: object.** It is bindgen's
  and Swift's position, and the record of both is the cost carried by users
  (#753 open since 2017; the kernel's helper directory). It is the
  conservative route, and correct as the message for a macro (1a) cannot
  reach, such as one whose argument is a type.
- **(1d) the program's own header: approve as the documented fallback**, never
  as the answer: it is the kernel's, gccgo's and every ecosystem's last resort,
  and design.md §1.11's four sentences promise more.
- **(1c) a mark on the line: approve as an option the record supports, not
  required.** GHC is the precedent: `capi` is a mark, but it marks the calling
  mode rather than the macro, and GHC's own advice is to use it for every
  system import. A Heroes mark that says *macro* would have no precedent found.

**Argument** (120 words or fewer). Nobody types a macro's parameters from the
macro, because a macro has no types; every system that reaches one either
generalises (Zig, D) or trusts the binding's own declaration and lets the C
compiler judge a call built from it (Nim, GHC). GHC's `capi` has done route
(1a') since 2012, with a stub function the C compiler checks, and bindgen ships
the same mechanism for `static inline`. The systems that refuse (bindgen,
Swift) left their users writing helpers by hand, which is (1d) at scale: the
Linux kernel keeps a directory of them. The precedent's warning is narrow and
checkable: a stub's C types derived from a lossy binding type can be refused by
the C compiler (GHC #15531).

**Prediction** (falsifiable). Under (1a) or (1a'), the five refused
`ffi_unknown_name` files of the shared brief stay refused, because none is a
macro and `#ifdef` finds none defined; and among the 206 that do not emit, none
moves to emitting except through a name `#ifdef` finds. Checkable by re-running
`probes/q1/emit-one.sh` over the 412 files on the compiler that lands the
route, at that batch's census.

**Condition.** A system that types a macro's parameters from something other
than the binding's declaration, and checks a call against it, would change the
reading of (1a) as the most a binding can check. A record that GHC's `capi`
was withdrawn or deprecated for macros, or that #15531's class recurs for
integer types, would move (1a') below (1a).

## Q2. A one-line `match` arm

### Precedents

**P2.1 Rust: an arm is any expression, loops and assignments included; a
`let` is not one. Verified.** The Reference,
<https://doc.rust-lang.org/reference/expressions/match-expr.html>, fetched
2026-10-02 (the page shows no version): `MatchArms → ( MatchArm => (
ExpressionWithoutBlock , | ExpressionWithBlock ,? ) )* MatchArm => Expression
,?`. <https://doc.rust-lang.org/reference/expressions.html>:
`ExpressionWithBlockNoAttrs → BlockExpression | ConstBlockExpression |
UnsafeBlockExpression | LoopExpression | IfExpression | MatchExpression`, so a
`while`, a `for`, an `if` or a `match` may stand after `=>` with its own block.
<https://doc.rust-lang.org/reference/expressions/operator-expr.html>:
`AssignmentExpression` and `CompoundAssignmentExpression` (`+=`, `-=` and the
rest) are operator expressions, and *"An assignment expression always produces
the unit value."* <https://doc.rust-lang.org/reference/statements.html>: *"A
`let` statement introduces a new set of variables"*, and a `LetStatement` is a
`Statement`, not an `Expression`, so it is not an arm. **That is route (2a)
word for word**: a statement on the arm's line, a binding excepted.

**P2.2 Rust, and `_ = e`: an assignment, not a binding. Verified.**
<https://doc.rust-lang.org/reference/expressions/underscore-expr.html>:
*"Underscore expressions, denoted with the symbol `_`, are used to signify a
placeholder in a destructuring assignment. They may only appear in the
left-hand side of an assignment. Note that this is distinct from the wildcard
pattern."*, with the example `_ = 2 + 2;` and the comment *"unused result,
assignment to `_` used to declare intent and remove a warning"*, beside `let _
= 2 + 2;` as *"equivalent technique using a wildcard pattern in a
let-binding"*. Destructuring assignment shipped in Rust 1.59.0, 2022-02-24
(<https://blog.rust-lang.org/2022/02/24/Rust-1.59.0/>). So in Rust `0 => _ =
f(),` is an assignment arm.

**P2.3 Go: the blank identifier binds nothing, by the specification. Verified.**
<https://go.dev/ref/spec> (*"go1.27 (May 26, 2026)"* as the tool read it): *"The
blank identifier may be used like any other identifier in a declaration, but it
does not introduce a binding and thus is not declared."*, and *"The blank
identifier provides a way to ignore right-hand side values in an assignment"*.

**P2.4 Python: an indentation language, where a header's own line takes only
simple statements, for a stated reason, since at least 2.7. Verified.**
<https://docs.python.org/3/reference/compound_stmts.html> (3.14.8): `suite:
stmt_list NEWLINE | NEWLINE INDENT statement+ DEDENT`, `stmt_list: simple_stmt
(";" simple_stmt)* [";"]`, and the `match` statement's `case_block: 'case'
patterns [guard] ":" suite`. The rule and its reason: *"A suite can be one or
more semicolon-separated simple statements on the same line as the header,
following the header's colon, or it can be one or more indented statements on
subsequent lines. Only the latter form of a suite can contain nested compound
statements; the following is illegal, mostly because it wouldn't be clear to
which `if` clause a following `else` clause would belong: `if test1: if test2:
print(x)`"*. The same paragraph is in the 2.7.18 reference
(<https://docs.python.org/2.7/reference/compound_stmts.html>). So in Python
`case 1: x += 5` is legal and `case 1: while x < 3:` is not; and an assignment
on a case's line binds: *"Assignment statements are used to (re)bind names to
values"* (<https://docs.python.org/3/reference/simple_stmts.html>).

**P2.5 Nim: the same line as Python, drawn in the grammar, a binding included
in the excluded set. Verified, by the grammar text, unrun.**
<https://raw.githubusercontent.com/nim-lang/Nim/devel/doc/grammar.txt>, fetched
2026-10-02: `ofBranch = 'of' exprList colcom stmt`; `stmt = (IND{>}
complexOrSimpleStmt^+(IND{=} / ';') ';'? DED) / simpleStmt ^+ ';'`;
`simpleStmt = ((returnStmt | raiseStmt | yieldStmt | discardStmt | breakStmt |
continueStmt | pragmaStmt | importStmt | exportStmt | fromStmt | includeStmt |
commentStmt) / exprStmt) COMMENT?`, with `exprStmt = simpleExpr '=' optInd expr
...`; and `complexOrSimpleStmt` holds `ifStmt | whenStmt | whileStmt | caseStmt
| tryStmt | forStmt | ... | ('let' | 'var' | 'using') section(variable)`. So
by the grammar, an `of` branch's own line takes a jump, a `discard`, an
expression or an assignment, and an `if`, `while`, `for`, `case`, `let` or
`var` needs the indented block. Whether Nim's parser accepts more than its
grammar file says is unverified (unrun).

**P2.6 Swift: a case is any statements, declarations and loops included.
Verified.** The book's source,
<https://raw.githubusercontent.com/swiftlang/swift-book/main/TSPL.docc/ReferenceManual/Statements.md>,
fetched 2026-10-02: `switch-case → case-label statements`; `statement →
expression ;? | declaration ;? | loop-statement ;? | branch-statement ;?`; and
*"The scope of each case can't be empty. As a result, you must include at least
one statement following the colon (`:`) of each case label."* Swift is not
line-sensitive here: the next `case` label ends the body.

**P2.7 Kotlin: a `when` entry is a block or one statement, loops and
assignments included. Verified by the grammar.**
<https://kotlinlang.org/spec/syntax-and-grammar.html>: `whenEntry: whenCondition
{"," whenCondition} [","] "->" controlStructureBody`; `controlStructureBody:
block | statement`; `statement: {label | annotation} (declaration | assignment
| loopStatement | expression)`. Whether the compiler accepts a declaration as
an entry's one statement is unverified (search *Kotlin "Declarations are not
allowed in this position" when branch val* found only a thread about
declarations BETWEEN entries, <https://discuss.kotlinlang.org/t/declarations-in-when-statement/1462>).

**P2.8 Scala 3: a case is a block, definitions included. Verified by the
grammar.** <https://docs.scala-lang.org/scala3/reference/syntax.html>:
`CaseClause ::= 'case' Pattern [Guard] '=>' Block`, `Block ::= {BlockStat
semi} [BlockResult]`, `BlockStat ::= Import | {Annotation {nl}}
{LocalModifier} Def | Extension | Expr1 | EndMarker`, and `Expr1` holds `'while'
Expr 'do' Expr`, `'for' Enumerators0 ('do' | 'yield') Expr` and the
assignments.

**P2.9 Oberon-07: a case label takes a statement sequence; Oberon has no
declaration among its statements. Verified, read from the PDF.** *"The
Programming Language Oberon"*, Niklaus Wirth, *"Revision 1.10.2013 /
3.5.2016"*, <https://people.inf.ethz.ch/wirth/Oberon/Oberon07.Report.pdf>,
§9.5: `case = [CaseLabelList ":" StatementSequence].`, with the example `CASE k
OF 0: x := x + y | 1: x := x - y | ... END`; §9: `statement = [assignment |
ProcedureCall | IfStatement | CaseStatement | WhileStatement | RepeatStatement
| ForStatement].` Declarations live only in a `DeclarationSequence` (§10), so
*a binding excepted* is structural in Oberon, not a rule.

### The two families

| language | layout | on the arm's own line | a loop there | a binding there |
|---|---|---|---|---|
| Rust | delimited | any expression | yes | no (`let` is a statement) |
| Swift | delimited | any statements | yes | yes, scoped to the case |
| Kotlin | delimited | a block or one statement | yes | by grammar yes; compiler unverified |
| Scala 3 | either | a block | yes | yes |
| Oberon-07 | delimited | a statement sequence | yes | impossible by grammar |
| Python | indentation | simple statements only | **no** | yes (assignment binds) |
| Nim | indentation | simple statements only (grammar) | **no** | **no** (`let`, `var` are compound) |

### What the precedents say together

1. **No language found refuses a mutation on an arm's line.** Rust, Swift,
   Kotlin, Scala, Oberon, Python and Nim all accept an assignment there. Route
   (2b) would make Heroes the only one, at the cost of about 600 lines of its
   own compiler.
2. **Among delimited languages, a loop on the arm's line is ordinary**; Rust's
   grammar admits it as an `ExpressionWithBlock`, and the binding is the one
   thing excepted. That is (2a) exactly.
3. **The two indentation languages both refuse a compound statement on a
   header's own line**, and Python gives the reason: a continuation clause
   (`else`) could belong to two headers. That is (2c)'s shape, **and stricter
   than (2c)**: Python and Nim refuse an `if` or a `match`/`case` head there
   too, where (2c) refuses only `while` and `for`.
4. **`_ = e` is not a binding in either language that has the form**: Rust
   calls it an assignment *"distinct from the wildcard pattern"*, Go says the
   blank identifier *"does not introduce a binding"*. Heroes' own spec § 5 says
   the same, by the shared brief. So *a binding excepted* does not, on
   precedent, take `_ = e`, and a message saying `_` *"would be bound"*
   describes something neither precedent nor the spec calls binding.

### Verdict per route (advisory)

- **(2a) the production says what the parser does, a binding excepted:
  approve.** It is Rust's arm, and it writes down a shape 704 tracked lines
  already use. It departs from Python and Nim, which are Heroes' layout
  family, on the compound heads; that departure is deliberate only if Python's
  stated reason does not hold in Heroes (see the condition).
- **(2c) mutation in, `while` and `for` out: approve as the conservative
  route.** It is the Python and Nim line, drawn halfway: by their precedent an
  `if` or `match` head after `=>` would go too (11 tracked lines by the shared
  brief's grep). Heroes' `if` and `match` are value forms where loops are not,
  which is a principled place to stop that neither Python nor Nim offers; a
  departure, then, and a reasoned one.
- **(2b) refuse what the production does not list: object.** No precedent
  found refuses an assignment on an arm's line.
- **The side question**: `_ = e` should not fall under *a binding excepted*,
  and the message for `.blue => _ = 0` should not say *bound*; Rust and Go
  both read `_ = e` as discarding a value.

**Argument** (120 words or fewer). Rust has (2a) already: an arm is any
expression, a `while` or an assignment included, and a `let` is a statement, so
it is excepted. Swift, Kotlin, Scala and Oberon go further and take any
statement. No language found refuses a mutation on the arm's line, so (2b) is
unprecedented and costly. The precedent that cuts the other way is the layout
family Heroes belongs to: Python and Nim keep a header's own line for simple
statements, Python because an `else` after a nested head has two readings.
(2c) is that rule drawn halfway, and drawn at loops, which are Heroes'
non-value forms. On `_ = e`, Rust and Go agree it discards and binds nothing.

**Prediction** (falsifiable). If (2a) lands, the three `=> if` lines of the
shared brief's grep are the only place a reader can meet Python's ambiguity in
the tracked tree, and none of them has an `else` whose owner differs between
the indentation reading and the nearest-head reading. Checkable by opening
those three lines (`grep -n '=> if ' $(git ls-files '*.hero')`) before the
synthesis; this seat has no shell and has not opened them.

**Condition.** A Heroes program in which an `else` or a further arm after a
compound head on an arm's line is read by the parser as belonging to a
different head than its indentation says would bring Python's reason into
Heroes, and move this seat from (2a) to (2c) extended to `if` and `match`
heads. A delimited or indentation language that refuses an assignment on its
arm's line would weaken the objection to (2b).

## Q3. A value block whose last statement leaves on every path

### Precedents

**P3.1 Rust: the rule is (3a)'s, in so many words, and one predicate serves
the value and the unreachable-code warning. Verified.** The Reference,
<https://doc.rust-lang.org/reference/expressions/block-expr.html>, fetched
2026-10-02: *"A block is considered to be diverging if all reachable control
flow paths contain a diverging expression, unless that expression is a place
expression that is not read from."* and *"When a block does not contain a final
operand and the block diverges, the block has the never type and has no final
value"*. <https://doc.rust-lang.org/reference/expressions/if-expr.html>: *"An
`if` expression diverges if either the condition expression diverges or if all
arms diverge."* <https://doc.rust-lang.org/reference/expressions/match-expr.html>:
*"If either the scrutinee expression or all of the match arms diverge, then the
entire `match` expression also diverges."*
<https://doc.rust-lang.org/reference/divergence.html>: *"A diverging expression
is an expression that never completes normal execution."*, listing `return`,
`break`, `continue`, an infinite `loop`, calls to functions returning `!`, and
`panic!`. The chapter is new as text: rust-lang/reference PR #2067, *"Add a
chapter on divergence"*, opened 2025-10-28, merged 2026-02-01 (GitHub's API);
since which release the behaviour itself holds is unverified here. **A third
rule reads the same predicate**: `let`-`else`, stable in Rust 1.65.0
(*"Announcing Rust 1.65.0"*, 2022-11-03,
<https://blog.rust-lang.org/2022/11/03/Rust-1.65.0/>), where *"a refutable
pattern can match and bind variables in the surrounding scope like a normal
let, or else diverge"*. **The unreachable-code lint reads it too**:
<https://doc.rust-lang.org/rustc/lints/listing/warn-by-default.html>,
`unreachable_code`, example `panic!("we will never get past here!"); let x =
5;`, *"any code following this expression is unreachable"*. **Rust's three
path-enders against R4's**: `pub fn exit(code: i32) -> !`, since 1.0.0
(<https://doc.rust-lang.org/std/process/fn.exit.html>), so it counts; *"A
`loop` expression without an associated `break` expression is diverging and
has type `!`"* but *"A `while` expression evaluates to `()`"*
(<https://doc.rust-lang.org/reference/expressions/loop-expr.html>), so `while
true` does not count, and the warn-by-default `while_true` lint says *"denote
infinite loops with `loop { }`"*, *"help: use `loop`"* (the lint listing
above); and `assert!(false)` does not count: Clippy's `assertions_on_constants`,
version 1.34.0, category `style`
(<https://raw.githubusercontent.com/rust-lang/rust-clippy/master/clippy_lints/src/assertions_on_constants.rs>):
*"Will be optimized out by the compiler or should probably be replaced by a
`panic!()` or `unreachable!()`"*.

**P3.2 Kotlin: `Nothing` makes a jump an expression of any type. Verified.**
<https://kotlinlang.org/docs/returns.html>: of `return`, `break` and
`continue`, *"All of these expressions can be used as part of larger
expressions: `val s = person.name ?: return` ... The type of these expressions
is the Nothing type."* <https://kotlinlang.org/docs/exceptions.html>:
*"`Nothing` ... a built-in type that is a subtype of all other types"*, used for
expressions that *"never complete successfully, either because they always
throw an exception or enter an endless execution path like an infinite
loop"*. `inline fun exitProcess(status: Int): Nothing`, *"Since Kotlin 1.0"*
(<https://kotlinlang.org/api/core/kotlin-stdlib/kotlin.system/exit-process.html>).
That a `when` branch block ending in an `if` whose branches both `return` is
accepted as a value follows from these rules; it is this seat's reading, unrun.

**P3.3 Zig: `noreturn`, with `while (true) {}` in it, and a block that returns
is `noreturn` for its scope. Verified.** The language reference's line, as
edited by Andrew Kelley, 2018-01-23
(<https://github.com/ziglang/zig/commit/b71a56c9df5e3b20d06cac24062456821b69dbba.patch>):
`noreturn` is *"the type of `break`, `continue`, `return`, `unreachable`, and
`while (true) {}`"* (whether today's reference keeps the sentence is unverified;
the 0.15.2 page reached the tool truncated). And Gregory Anders, 2022-06-06,
*"Treat blocks with "return" as "noreturn""*
(<https://github.com/ziglang/zig/commit/135b91aecd9be1f6f5806b667e07e383dd481198.patch>):
*"Block statements that end with "break" should not be considered "noreturn"
for the enclosing scope, but other "noreturn" instructions (return, panic,
compile error, etc.) should be."*, with the test
`code_after_return_in_block_is_unreachable.zig`: `{ return; } return;` gives
*"unreachable code"*, *"note: control flow is diverted here"*. **That test is
`c5`'s shape**: a statement after a block that left on every path, refused.

**P3.4 Java's switch expression: a block rule must not complete normally, so a
`throw` or an infinite loop is a leaving block, and `return` is barred
outright. Verified.** JEP 361, *"Release: 14"*, *"Status: Closed / Delivered"*
(<https://openjdk.org/jeps/361>): *"A switch expression must either complete
normally with a value, or complete abruptly by throwing an exception."* and
*"The control statements, `break`, `yield`, `return` and `continue`, cannot jump
through a switch expression."* The block rule as drafted for JEP 325, quoted by
Manoj Palat on jdk-dev, 2018-11-28
(<https://mail.openjdk.org/pipermail/jdk-dev/2018-November/002262.html>): *"If
the switch block consists of switch labeled rules, then it is a compile-time
error if any switch labeled block can complete normally."* The predicate is
the JLS's oldest, JLS 1.0 §14.19
(<https://titanium.cs.berkeley.edu/doc/java-langspec-1.0/14.doc.html>): *"A
`while` statement can complete normally iff at least one of the following is
true: The `while` statement is reachable and the condition expression is not a
constant expression with value `true`. There is a reachable `break` statement
that exits the `while` statement."*; *"A `break`, `continue`, `return`, or
`throw` statement cannot complete normally."*; *"An expression statement can
complete normally iff it is reachable."* So in Java `while (true) {}` counts,
`System.exit(0);` does not (an expression statement), and `assert false;` does
not: *"By default, assertions are disabled at runtime."*, and *"If a statement
is unreachable as defined in the Java Language Specification, you will get a
compile time error if you try to assert that it is not reached. Again, an
acceptable alternative is simply to throw an `AssertionError`."*
(<https://docs.oracle.com/javase/8/docs/technotes/guides/language/assert.html>).

**P3.5 Swift's `if` and `switch` expressions: `Never` and `throw` admitted,
`return` admitted in a draft and taken out, and no multi-statement branch at
all. Verified.** SE-0380, *"Status: Implemented (Swift 5.9)"*
(<https://raw.githubusercontent.com/swiftlang/swift-evolution/main/proposals/0380-if-switch-expressions.md>):
*"Each branch of the `if`, or each `case` of the `switch`, must be a single
expression."*; *"An exception to this rule is if a branch either explicitly
throws, or terminates the program (e.g. with `fatalError`), in which case no
value for the overall expression needs to be produced."*; *"The one exception
to this rule is that some branches could produce a `Never` type."* with `let x
= if .random() { 1 } else { fatalError() }`. On `return`: *"An earlier version
of this proposal allowed use of `return` in a branch."*, then *"Allowing new
control flow out of expressions could be unexpected and error-prone."*, *"The
control flow impact of nested return statements would become more difficult to
reason about"* and *"The use-cases for this functionality presented in the
review thread were also fairly niche."* And the cost of the single-expression
rule, in the proposal's own words: *"an unfortunate usability cliff"*. A
follow-on pitch, *"Multi-statement if/switch/do expressions"*
(<https://forums.swift.org/t/pitch-multi-statement-if-switch-do-expressions/68443>),
exists; its status today is unverified. Whether Swift's `exit` is declared
`-> Never` is unverified (searched, not fetched).

**P3.6 OCaml: `exit` and `assert false` are of every type, and `assert false`
is made immune to switching off for that reason. Verified.** The standard
library, OCaml 5.5 (<https://ocaml.org/manual/latest/api/Stdlib.html>): `val
raise : exn -> 'a`, `val failwith : string -> 'a`, `val exit : int -> 'a`. The
manual, <https://ocaml.org/manual/latest/expr.html>: *"As a special case,
assert false is reduced to raise (Assert_failure ...), which gives it a
polymorphic type. This means that it can be used in place of any expression
(for example as a branch of any pattern-matching). It also means that the
assert false "assertions" cannot be turned off by the -noassert option."*

**P3.7 TypeScript 3.7: a call that never returns enters the control-flow
graph. Verified.**
<https://www.typescriptlang.org/docs/handbook/release-notes/typescript-3-7.html>:
*"In order to ensure that a function never potentially returned `undefined` or
effectively returned from all code paths, TypeScript needed some syntactic
signal - either a `return` or `throw` at the end of a function. So users found
themselves `return`-ing their failure functions."* and *"Now when these
`never`-returning functions are called, TypeScript recognizes that they affect
the control flow graph and accounts for them."*, `process.exit(1)` being the
example.

**P3.8 Go: a syntactic predicate that names its built-in and nothing else.
Verified.** <https://raw.githubusercontent.com/golang/go/master/src/go/types/return.go>:
*"calling the predeclared (possibly parenthesized) panic() function is
terminating"*; a `for` is terminating when `s.Cond == nil &&
!hasBreak(s.Body, label, true)`; an `if` when it has an `else` and both
branches are terminating. So `os.Exit` does not count, and `for true {}` does
not either (it has a condition); only `for {}` does. Go has no value `switch`,
so this row bears on the predicate, not on value position.

### How each counts R4's three path-enders

| language | the jump words | the exit call | `assert false` | `while true`, no `break` | a value position whose every path leaves |
|---|---|---|---|---|---|
| Rust | yes | yes (`-> !`) | no (lint says use `panic!`) | no; `loop` yes | **yes**, by the block rule |
| Kotlin | yes (`Nothing`) | yes (`exitProcess`) | unverified | unverified | yes, by type |
| Zig | yes | unverified | unverified | **yes** (langref, 2018) | yes, blocks with `return` (2022) |
| Java switch expr. | `throw` only; `return`, `break`, `continue` barred | no | no (off by default) | **yes** | yes, *cannot complete normally* |
| Swift `if`/`switch` expr. | `throw` only; `return` taken out | `fatalError` yes; `exit` unverified | unverified | unverified | single expression only |
| OCaml | `raise` | yes (`int -> 'a`) | **yes**, and never switched off | unverified | yes, by type |
| TypeScript | yes | yes since 3.7 (`never`) | unverified | unverified | (no value `switch`) |
| Go | yes | no (`panic` only) | (none) | no; `for {}` only | (no value `switch`) |

### What the precedents say together

1. **(3a) is Rust's written rule**: *"all reachable control flow paths contain
   a diverging expression"* makes the block diverge, and Kotlin, Zig, Java's
   switch expressions and OCaml reach the same answer by a bottom type or by
   *cannot complete normally*. No language found that admits a jump as a value
   arm refuses the same jump one statement deeper.
2. **The one refusal found is Swift's, and it is a different refusal.** Swift
   took `return` out of value branches altogether, on the grounds that nested
   returns are *"more difficult to reason about"*, and allows no block at all.
   Heroes has already taken the other side of the first question (panel 017
   R1, jumps admissible as arm bodies), so Swift's reason argues against panel
   017, not against (3a); and Swift's proposal calls its own block refusal *"an
   unfortunate usability cliff"*.
3. **Where both rules exist, one predicate serves both.** Rust's divergence
   decides the block's type, `let`-`else`'s `else`, and the unreachable-code
   lint; Java's *can complete normally* decides the switch-expression block and
   the unreachable-statement error; Zig's 2022 commit makes the leaving block
   `noreturn` for its scope and refuses the statement after it in one change.
   So (3a)'s *leaves on every path* should be R4's predicate, and if it is, R4
   reaches `c5`, as Zig's test reaches its shape.
4. **R4's three path-enders split the record.** The exit call counts in every
   language found that gives it a bottom type (Rust, Kotlin, OCaml, TypeScript;
   Swift's `fatalError`) and in none of those without one (Java, Go). `while
   true` with no `break` counts in Java and Zig and not in Rust or Go, both of
   which count only the conditionless loop. `assert false` counts only in
   OCaml, which made it immune to `-noassert` so that it could; Java and Rust
   leave it out, because an assertion can be switched off (Java) or is not a
   divergence (Rust).

### Verdict per route (advisory)

- **(3a) the block leaves, so its arm is a jumping arm: approve**, with R4's
  predicate as *leaves on every path*, the three path-enders included. It is
  Rust's rule as written and Java's and Zig's in effect, and it keeps one
  predicate for the value rule and the unreachable rule, which is what every
  precedent with both does. That it then reaches `c5` is the precedent's
  prediction (Zig's `code_after_return_in_block_is_unreachable`).
- **R4's `assert false`: approve on one condition the record names**: that a
  Heroes `assert` can never be switched off. OCaml is the only precedent that
  counts it, and it counts it because it guaranteed that.
- **(3b) the refusal stands, its message naming the repair: object.** It is
  Swift's position, which its own proposal calls a usability cliff, and Swift
  holds it while also refusing `return` in value branches, which Heroes does
  not.

**Argument** (120 words or fewer). Rust's Reference states (3a) word for
word: a block diverges when every reachable path contains a diverging
expression. Kotlin and OCaml get the same answer by a bottom type, Java's
switch expressions by *cannot complete normally*, and Zig by making a block that
returns `noreturn` for its scope. Each language that has both rules uses one
predicate for the value and the unreachable statement, so R4's predicate is the
precedented choice, and it reaches `c5`. The record splits on R4's path-enders:
the exit call counts wherever a bottom type exists, `while true` in Java and
Zig only, and `assert false` only in OCaml, which first made it impossible to
switch off. The one refusal, Swift's, rests on refusing `return` in values,
which Heroes already admits.

**Prediction** (falsifiable). Under (3a) with R4's predicate, every probe of
the shared brief's Q3 list (`a55`, `a69`, `a73`, `b1` to `b8`, `c1` to `c3`,
`c6`) checks, and `c5` becomes a refusal at the `print`, with no other program
in the tracked tree newly refused except where a statement follows a
statement `match` or `if` whose every branch leaves. Checkable by the batch's
`check` census, trunk against batch, when the route lands.

**Condition.** A Heroes `assert` that can be switched off at build or run time
takes `assert false` out of the predicate, as Java's and Rust's record says.
A language that admits a jump as a value arm and refuses it one statement
deeper, for a reason other than Swift's, would change the reading of (3a).

## Q4. The certain `-` in a pattern

### Precedents

**P4.1 Swift: today's Heroes rule, shipped. Verified, its reach into a `case`
label and onto a string a reading.** The diagnostic,
<https://raw.githubusercontent.com/swiftlang/swift/main/include/swift/AST/DiagnosticsParse.def>,
fetched 2026-10-02: `ERROR(expected_prefix_operator,none, "unary operator
cannot be separated from its operand", ())`. Where it is raised,
<https://raw.githubusercontent.com/swiftlang/swift/main/lib/Parse/ParseExpr.cpp>:
in a unary position, on a `tok::oper_binary_spaced` token, *"For recovery
purposes, accept an oper_binary here."*, then `diagnose(PreviousLoc,
diag::expected_prefix_operator).fixItRemoveChars(OperEndLoc, Tok.getLoc());`,
the fix-it deleting the space and nothing else. The rule it rests on, the book's
source
(<https://raw.githubusercontent.com/swiftlang/swift-book/main/TSPL.docc/ReferenceManual/LexicalStructure.md>):
*"If an operator has whitespace around both sides or around neither side, it's
treated as an infix operator."* So Swift refuses a spaced `-` before its
operand and offers to close the gap, without looking at what the operand is.
That a Swift `case - 1:` reaches this path (an expression pattern parsed as an
expression), and that on a string the fix-it leads to a second error, are this
seat's readings of the parser, unrun; whether the fix-it is applied without a
person is unverified.

**P4.2 Python: the bulleted reading would run silently. Verified for the
grammar, the consequence a reading.**
<https://docs.python.org/3/reference/compound_stmts.html> (3.14.8):
`literal_pattern: signed_number | signed_number "+" NUMBER | signed_number "-"
NUMBER | strings | "None" | "True" | "False"` and `signed_number: ["-"]
NUMBER`. The sign is a token of its own and only a number takes it, so by the
grammar `case - 1:` is the pattern `-1` and a `-` before a string is not a
pattern at all. Whether CPython's parser accepts the space is unrun.

**P4.3 Rust: the grammar admits a sign before any literal, and a string with a
sign is left to the type check. Verified for the grammar.**
<https://doc.rust-lang.org/reference/patterns.html>: `LiteralPattern → -?
LiteralExpression`, and *"Since negative numbers are not literals, literals in
patterns may be prefixed by an optional minus sign, which acts like the
negation operator."* That is the shape of spec § 8's `[ "-" ] string`
(admitted by the production, refused by the checker); Python's grammar is the
other shape (only a number takes the sign). That rustc refuses `-"a"` in a
pattern, and accepts `- 1` with the space, are readings, unrun.

**P4.4 GHC's LexicalNegation: a language that decided the space after `-` is
not noise. Verified.**
<https://downloads.haskell.org/ghc/latest/docs/users_guide/exts/lexical_negation.html>
(GHC 9.14.1's guide): *"Since: 9.0.1"*; *"This means that `(- x)` is the right
operator section of subtraction, whereas `(-x)` is the negation of `x`."* The
page says nothing of patterns.

**P4.5 What *certain* means where a fix can be applied by a machine.
Verified.** rustc,
<https://raw.githubusercontent.com/rust-lang/rust/master/compiler/rustc_lint_defs/src/lib.rs>,
`Applicability::MachineApplicable`: *"The suggestion is definitely what the user
intended, or maintains the exact meaning of the code. This suggestion should be
automatically applied."*; `MaybeIncorrect`: *"The suggestion may be what the
user intended, but it is uncertain."* Clang,
<https://clang.llvm.org/docs/InternalsManual.html>: fix-it hints *"should only
be used when it's very likely they match the user's intent"*, *"Clang must
recover from errors as if the fix-it had been applied"*, and one that cannot
obey these goes on a note, which is not applied automatically (the tool's
rendering of the last rule). Ruff, <https://docs.astral.sh/ruff/linter/>:
*"The meaning and intent of your code will be retained when applying safe
fixes."*, *"The meaning could change when applying unsafe fixes."*, and *"Ruff
only enables safe fixes by default."* RuboCop's `Lint/InterpolationCheck`
(<https://docs.rubocop.org/rubocop/latest/cops_lint.html>): autocorrection
*"Always (Unsafe)"*, *"because although it always replaces single quotes as if
it were miswritten double quotes, it is not always the case."* **Every
definition found is about the writer's intent, and none about how often the
other intent occurs.**

**P4.6 The bulleted-list habit: searched, not found.** Searches *LLM generated
code contains markdown formatting inside code block bullet list syntax error
study* and *taxonomy of errors in LLM-generated code "markdown" artifacts
syntax errors "code fence" OR "bullet" paper arXiv* found no study or tracker
recording markdown bullets written as code. The nearest measured class:
Kjellberg, Staron and Fotrousi, *"From LLMs to Agents in Programming: The
Impact of Providing an LLM with a Compiler"*, arXiv 2601.12146 (v1 2026-01-17,
v2 2026-01-23, <https://arxiv.org/html/2601.12146v2>), C programs on
RosettaCode from 16 models, whose *"Markdown Error"* is *"sometimes the models
generated programs in C, but the markdown around the code was missing, and
there were textual descriptions (not comments) alongside the code"*, and
*"accounted for less than 10 % of the total number of errors for the baselines
and 16 % for the agents"*. So prose leaking into code is a measured class of
model mistake; bullets as arm markers are not measured anywhere this seat
found. A question, not a finding of absence (CLAUDE.md § RUN IT).

### What the precedents say together

1. **Swift ships exactly the rule defect 123 and panel 180 gave Heroes**: a
   spaced `-` before its operand refused, the fix closing the gap, blind to what
   the operand is. By this seat's reading (unrun), on a string it would do what
   Heroes' `s5c` does: the fix produces a new error.
2. **Python, the indentation language with `match`, would not catch the
   bulleted arms at all**: its grammar reads `- 1` as `-1`. Heroes refusing the
   spaced `-` is already stricter than Python's grammar; the question is only
   what the fix may claim.
3. **The definitions of a machine-applicable fix are about intent** (rustc's
   *"definitely what the user intended"*, clang's *"very likely they match the
   user's intent"*, Ruff's *"meaning and intent ... retained"*). `s5a` is a
   fix applied and a program that builds and prints the wrong answer, which is
   what Ruff calls unsafe and rustc `MaybeIncorrect`, however rare the
   bulleted writer is.
4. **(4a)'s string clause matches rustc's definition**: before a string the
   `-` has one reading (it cannot be a sign), so deleting it is *"definitely
   what the user intended"*. Its character clause is `MaybeIncorrect` by the
   same definition. Its integer clause is clang's *"very likely"* made into a
   test the program's own text can answer.

### Verdict per route (advisory)

- **(4a) lane 135c's three clauses: approve.** It is the precedents'
  definition of *certain* applied to each operand kind, and it leaves Swift's
  shipped refusal in place. On the question whether a spaced `-` after a `|` is
  a neighbour: no precedent found bears on it. This seat's reading, offered as
  a reading: a `-` after `|` cannot be a bullet, so it is evidence that the
  writer spaces signs, which argues for the sign reading of the head rather
  than against it.
- **(4b) both fixes always guesses: approve as the conservative route.** It is
  rustc's `MaybeIncorrect` throughout, and costs the certain fix where only one
  reading exists (the string clause).
- **(4c) the premise stands: object.** The defining texts found do not make
  certainty depend on how often the other reading occurs, and nothing measures
  that frequency either way (P4.6).

**Argument** (120 words or fewer). Swift ships Heroes' current rule: a spaced
`-` before its operand is an error whose fix-it closes the gap. Python would
not even notice the bulleted arms. So refusing is precedented; what is in
question is the word *certain*. rustc, clang and Ruff all define a fix a machine
may apply by the writer's intent: *definitely what the user intended*, *very
likely*, *meaning and intent retained*. `s5a` shows a fix that builds and
changes the answer, which fails all three. (4a) applies their definition per
operand: certain where one reading exists (a string), a guess where two
compile (a character), and certain for an integer only where the program's own
text shows no bullet. No study records bullets in code; none rules them out.

**Prediction** (falsifiable). Under (4a), a tracked program's diagnostics move
only where an arm opens with a spaced `-`, and every such arm today draws the
same refusal it draws under (4a), so no tracked program changes from checking
to refused or back; what changes is the `Fix` tag in the `fixes` form and in
the four `probes/q4/` cases. How many tracked arms open with a spaced `-` is
unrun by this seat (no shell); `grep -nE '^[[:space:]]*- ' $(git ls-files
'*.hero')`, filtered to arm heads, would count them before the route lands.

**Condition.** A study measuring that models do not write bulleted arms in a
`match` would weaken the objection to (4c) without changing what *certain*
means. A shipped fix-applying tool that marks as machine-applicable a fix with
a second plausible reading, and keeps it so after reports, would change the
reading of P4.5.

## Q5. The forgotten `f`

Re-verified today, as the brief asks; panel 184's historian report was used
only to know where to look.

### Precedents

**P5.1 Ruff, RUF027 `missing-f-string-syntax`: still preview, still decided by
scope. Verified 2026-10-02.**
<https://docs.astral.sh/ruff/rules/missing-f-string-syntax/>: *"This rule is
unstable and in preview"*, since v0.2.1; *"Expressions inside curly braces are
only evaluated if the string has an `f` prefix."*; the fix *"will always change
the behavior of the program"* and is unsafe. The source,
<https://raw.githubusercontent.com/astral-sh/ruff/main/crates/ruff_linter/src/rules/ruff/rules/missing_fstring_syntax.rs>:
`#[violation_metadata(preview_since = "v0.2.1", category =
Category::Suspicious)]`, and its seven exclusions, verbatim: *"The string is a
standalone expression. For example, the rule ignores all docstrings."*; *"The
string is part of a function call with argument names that match at least one
variable"*; *"The string (or a parent expression of the string) has a direct
method call on it"*; *"The string has no `{...}` expression sections, or uses
invalid f-string syntax."*; *"The string references variables that are not in
scope, or it doesn't capture variables at all."*; *"Any format specifiers in the
potential f-string are invalid."*; *"The string is part of a function call that
is known to expect a template string"*. The scope test is
`semantic.simulate_runtime_load_at_location_in_scope(...)`, a builtin not
counting. The current Ruff is 0.16.10 (PyPI's JSON, fetched 2026-10-02), so
the rule has stayed in preview from v0.2.1 to 0.16.10. Why, AlexWaygood on
<https://github.com/astral-sh/ruff/pull/15247>, 2025-01-04 (GitHub's API):
*"RUF027 already has too many false positives for us to consider stabilising
it. I definitely don't think we should be making changes that introduce new
false positives here, honestly"*; the PR closed unmerged 2025-04-28.

**P5.2 Clippy, `literal_string_with_formatting_args`: by scope, and in
`nursery`. Verified 2026-10-02.**
<https://raw.githubusercontent.com/rust-lang/rust-clippy/master/clippy_lints/src/literal_string_with_formatting_args.rs>:
*"Checks if string literals have formatting arguments outside of macros using
them (like `format!`)."*, *"It will likely not generate the expected content."*,
`#[clippy::version = "1.85.0"]`, category `nursery`; a hole is kept only if
its name is a local in MIR (`mir.var_debug_info ... local.name.as_str() ==
name`). <https://github.com/rust-lang/rust-clippy/pull/14014>, opened
2025-01-17, merged 2025-01-19 (GitHub's API): the lint produced *"thousands of
false positives"* on GitHub, changelog *"`literal_string_with_formatting_args`
change category to `nursery` from `suspicious`"*.

**P5.3 Pylint: never shipped. Verified 2026-10-02.**
<https://github.com/pylint-dev/pylint/issues/2507>, *"Detect when
f-string-syntax is used in a string, but not marked as an f-string"*, opened
2018-09-21, **open**, labelled *Blocked* and *High priority*. PR
<https://github.com/pylint-dev/pylint/pull/4787>, *"Add
`possible-forgotten-f-prefix` checker"*, DanielNoord, opened 2021-08-02, closed
unmerged 2022-04-01, its author's last word: *"The logic I worked on worked
(sort of) but there were just too many false positives an edge cases that I
would be comfortable merging it."*

**P5.4 Scala, `-Xlint:missing-interpolator`: by scope, lint-only. Verified
2026-10-02.** <https://docs.scala-lang.org/overviews/compiler-options/index.html>:
*"A string literal appears to be missing an interpolator id."* The compiler's
own test,
<https://raw.githubusercontent.com/scala/scala/2.13.x/test/files/neg/forgot-interpolator.scala>,
runs under `-Werror -Xlint:missing-interpolator` and holds `def ok = "Don't warn
on $nosymbol interpolated."`, `def pass = "Don't warn on $pancake package
names."`, `def types = "Or $NonVal type symbols either."`: a name not in scope
as a value draws nothing.

**P5.5 PVS-Studio V3138 (C#): found today, where panel 184's search found no
C# check. Verified.** <https://pvs-studio.com/en/docs/warnings/v3138/>, dated
2019-06-10 on the page as the tool read it: *"V3138. String literal contains
potential interpolated expression."*, *"The analyzer has detected a string that
could contain an interpolated expression, but there is no interpolation
character '$' before the literal."*, with one exception: *"an exception is made
for string literals passed as arguments to methods whose other arguments are
variables contained in that same literal"* (Ruff's second exclusion, in
another ecosystem). Whether it requires the name in braces to be in scope: the
page does not say (asked of the tool directly). A commercial analyzer; whether
the rule is on by default is unverified. The C# reference itself,
<https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/tokens/interpolated>
(updated 2026-09-22): *"Use the `$` character to identify a string literal as
an interpolated string."*, *"To include a brace, "{" or "}", in the text
produced by an interpolated string, use two braces"*, and no check for a
missing `$`.

**P5.6 The shape-only checks key on a sigil. Verified 2026-10-02.** ESLint,
<https://raw.githubusercontent.com/eslint/eslint/main/lib/rules/no-template-curly-in-string.js>:
type `problem`, recommended `false`, *"Disallow template literal placeholder
syntax in regular strings"*, the whole test the regular expression
`/\$\{[^}]+\}/u`. RuboCop's `Lint/InterpolationCheck`
(<https://docs.rubocop.org/rubocop/latest/cops_lint.html>): *"Checks for
interpolation in a single quoted string."*, enabled by default *Yes*, added
0.50, changed 1.40, autocorrection *"Always (Unsafe)"*.

**P5.7 rustc's `unused_variables`: route (5c), shipped four years ago.
Verified 2026-10-02.** PR <https://github.com/rust-lang/rust/pull/100941>,
lyming2007, opened 2022-08-24, merged 2022-08-30, milestone 1.65.0 (released
2022-11-03, <https://blog.rust-lang.org/2022/11/03/Rust-1.65.0/>). The test is
still on master,
<https://raw.githubusercontent.com/rust-lang/rust/master/tests/ui/type/issue-100584.stderr>:
the unused parameter `xyza` beside `"{xyza}"` draws a note that the literal
looks like interpolation and that *"string interpolation only works in
`format!` invocations"* (the tool's rendering of the file).

**P5.8 Rust 2021's `panic!`: a plain position made a format position, with
`{{`, at an edition. Verified 2026-10-02.**
<https://doc.rust-lang.org/edition-guide/rust-2021/panic-macro-consistency.html>:
*"`panic!("{")` is no longer accepted, without escaping the `{` as `{{`."* and
*"The `non_fmt_panics` lint has already been a warning by default on all
editions since the 1.50 release (with several enhancements made in later
releases)."*

**P5.9 The languages with no prefix, and what they pay. Verified.** Kotlin,
<https://kotlinlang.org/docs/strings.html>: *"String templates let you embed
variables and expressions directly inside a `String` literal."*, *"A template
expression starts with `$`."*, a literal `$` written `${'$'}` or by
multi-dollar interpolation, where *"Dollar signs below that number are treated
as literal characters"*; multi-dollar interpolation went Stable in Kotlin
2.2.0 (<https://kotlinlang.org/docs/whatsnew22.html>, released 2025-06-23).
Swift: interpolation `\(...)` is active in every ordinary literal and inert in
a raw one unless written `\#(...)`; SE-0200, *"Implemented (Swift 5.0)"*
(<https://raw.githubusercontent.com/swiftlang/swift-evolution/main/proposals/0200-raw-string-escaping.md>):
*"Strings using custom boundary delimiters mirror their pound sign(s) after the
leading backslash."* (the rest of that row is the tool's rendering of the
proposal).

### Which judge by the literal's own text, and which by scope

| check | judged by | default | where it stands |
|---|---|---|---|
| ESLint `no-template-curly-in-string` | the text, on `${` | not recommended | shipped; its release date not re-fetched here |
| RuboCop `Lint/InterpolationCheck` | the text, on `#{` | **enabled** | shipped; severity not read; autocorrect unsafe |
| PVS-Studio V3138 | the text, one exclusion; scope unverified | unverified | shipped 2019 |
| Ruff RUF027 | text and **scope**, seven exclusions | preview only | preview from v0.2.1 to 0.16.10 |
| Clippy `literal_string_with_formatting_args` | text and **scope** (MIR locals) | off (`nursery`) | moved out of `suspicious` over *"thousands of false positives"* |
| Pylint `possible-forgotten-f-prefix` | text and variables | | never merged |
| Scala `-Xlint:missing-interpolator` | text and **scope** | lint-only | shipped, off unless asked for |
| rustc `unused_variables` note | the unused name, then the literal | on (a warning's note) | shipped 1.65, 2022 |

### What the precedents say together

1. **No check on a bare `{` found reached a default-on error or even a
   default-on warning.** Every bare-brace check added scope to cut false
   alarms, and each is off by default, in preview, or unmerged. The default-on
   check found keys on a sigil (`#{` in RuboCop) that ordinary text rarely
   holds.
2. **Scope did not save them.** Clippy, Ruff and Pylint all decide by scope and
   all retreated over false positives; the false positives they name are
   strings that something else fills later, where the name is in scope by
   construction (Ruff's and PVS-Studio's shared exclusion is that case).
3. **Heroes has no warning level** (design.md `:3545`, by the shared brief), so
   any check here is a refusal. Every precedent above ships, if at all, below
   that line; route (5a) or (5b) would be the first found to ship as an error.
4. **Panel 184's own census for (1a), recorded and not re-run here**, reads
   *34 false alarms and 0 true ones, and 22 files stop checking, the compiler
   among them*: the same ratio the precedents retreated over. Panel 184's
   historian made (1a)'s approval conditional on what kind those alarms are
   (strings filled later, or text showing Heroes source); the shared brief does
   not say, and this seat did not read the census, so whether that condition
   is met is open.
5. **The message route has shipped and stayed**: rustc since 1.65.
6. **The class does not exist where the hole cannot be written by accident**:
   Swift's `\(` and Kotlin's `$` are active in every literal, and Kotlin paid
   for it with multi-dollar interpolation, stable only in 2025. Heroes refused
   an active brace without a prefix (panel 121), which is the Kotlin side of
   that trade; Swift's spelling is a different one.

### Verdict per route (advisory)

- **(5c) R2's note alone: approve, and this seat reads it as the landing the
  record supports.** It is rustc's, shipped and kept since 2022.
- **(5a) panel 184's (1a), refused by shape: object.** No precedent ships a
  bare-brace check as an error, and the recorded census shows the ratio that
  pulled the others back.
- **(5b) panel 184's (1b), amended, refused by scope: object.** Three
  ecosystems tried scope and none kept it on; it also makes legality depend on
  the names around the literal, which the blind reading of panel 184 objected
  to.
- **(5d) judged by the literal's own text alone, a name or dotted names and
  calls: object, unless its measured false alarms are near zero.** The
  precedent for text-only checks is that they work when keyed on a sigil, and
  `{name}` is not one; this route is the shape check with a narrower shape, and
  its false alarms are a census, not a precedent.
- **(5e) a route the seats find**: the one precedent under which the class
  does not exist is a hole spelling no plain literal can hold by accident
  (Swift's `\(`). It is named, not recommended: it meets panel 121's refusal
  only by not being a brace, and it reopens the hole's spelling, panel 008's
  ground.

**Argument** (120 words or fewer). Every check for a forgotten prefix that
reads a bare brace has retreated: Clippy to `nursery` over thousands of false
positives, Ruff's held in preview from v0.2.1 to 0.16.10, Pylint's never
merged, Scala's lint-only. Scope was each one's cure and cured nothing, because
the false alarms are templates whose names are in scope by construction. The
default-on check found keys on a sigil, and none found is an error; Heroes has
no warning level, so (5a) and (5b) would be the first to ship this as an
error, and panel 184's recorded census, 34 false alarms to 0 true, has the
ratio the others retreated over. rustc's note on the unused name, (5c), has
stood since 2022.

**Prediction** (falsifiable). A census of (5d)'s predicate over the tracked
tree finds its false alarms among the 34 panel 184 recorded, chiefly literals
that show Heroes source (the compiler's messages and tests about holes, e.g.
`selfhost/lexer.hero:452`), and no true forgotten `f`. Checkable when a seat or
the coordinator runs that census.

**Condition.** A language or linter shipping a bare-brace check as an error,
or as a default-on warning, for years without retreat would lift the objection
to (5a). A (5d) census with zero false alarms over the tracked tree would move
this seat to approve (5d).

## Summary for the synthesis

- **Q1**: approve (1a) and (1a'), reading (1a') as the more robust: GHC's
  `capi` has written the C stub from the declared signature for macros since
  2012, bindgen does it for `static inline` since 2023, and GHC #15531 names
  its one known failure. Object to (1b): bindgen and Swift refuse, and their
  users write helpers by hand (the Linux kernel's directory, bindgen #753 open
  since 2017). (1d) as the fallback, (1c) optional.
- **Q2**: approve (2a), Rust's arm exactly (any expression, loops and
  assignments, a `let` excepted); approve (2c) as the conservative route, the
  Python and Nim line drawn halfway; object to (2b), unprecedented. `_ = e` is
  not a binding in Rust or Go.
- **Q3**: approve (3a) with R4's predicate, Rust's written rule and Java's,
  Zig's and Kotlin's in effect, one predicate for value and unreachable, which
  reaches `c5`; `assert false` counts only if a Heroes `assert` can never be
  switched off (OCaml). Object to (3b), Swift's usability cliff.
- **Q4**: approve (4a), the precedents' definition of *certain* (rustc, clang,
  Ruff) applied per operand; (4b) as conservative; object to (4c). Swift ships
  today's Heroes rule; no study records bulleted arms.
- **Q5**: approve (5c), rustc since 2022; object to (5a), (5b) and (5d) unless
  measured clean: no bare-brace check found has shipped on by default, and
  Heroes' only level is an error.
