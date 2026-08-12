# Design Brief: **Heroes** — A Small Compiled Language Built for LLM Code Generation

**Read this whole document before writing any code.** It is not a feature list — it is a record of
*why* each decision was made. Many choices look arbitrary in isolation and are not. When you
disagree with something, check the rationale section for that item first; if the rationale doesn't
cover your objection, raise it explicitly rather than silently doing something else.

## The name

The language is called **Heroes**. Compiler binary: `heroes`. File extension: `.hero`.

It is an homage to David Bowie's *"Heroes"* (1977) — note that Bowie titled the album with
quotation marks around the word, to undercut it with irony. The author is writing a book about
programming languages with Bowie as a travelling companion, and this language is the book's running
example.

Two practical notes for the implementation:

- **Write it `Heroes` in prose, `heroes` for the binary and any identifier.** Don't try to carry the
  quotation marks into filenames, package names or the CLI — they will break shells and tooling. The
  irony lives in the prose, not in the paths.
- **Bowie references belong in prose, never in the language surface.** Section headings in example
  files, chapter epigraphs, the README, the book — all fair game. But keyword names, error message
  text, and library function names stay plain and literal, because they are read by a machine that
  has to parse them and by a model that has to learn them from a 4096-token spec. A cute error
  message costs spec tokens and comprehension; a cute README costs nothing. This is the same
  discipline as Part 1.10 (ASCII-only syntax): personality in the packaging, precision in the
  substrate.

One thing worth checking before committing publicly: whether "Heroes" collides with an existing
language, package registry entry, or trademark in this space. The name is unusual for a programming
language, which is a point in its favour, but a five-minute search is cheap insurance.

---

## Part 0 — Who this is for and what success looks like

### The author

One person, learning. Not a compiler expert. On macOS (Apple Silicon). The implementation will be
written largely with your help, which means the author's learning comes from *retrieval, testing,
and debugging* rather than from typing — and it runs **on the author's clock, never as a gate on
the work** (protocol inverted 2026-08-03, author instruction; the executable form lives in the
`/step` and `/debrief` skills). Three consequences that should shape how you work:

1. **Implement first; queue the understanding.** The assistant never stops mid-step to ask.
   Everything worth understanding — a new concept, a surprising output, a design default — becomes
   an entry in `docs/debrief/QUEUE.md`, processed in `/debrief` sessions when the author chooses.
   Inside a debrief, retrieval still comes first: the question before the explanation, because the
   gap between the author's guess and reality is the lesson.
2. **The assistant writes the tests; the author ratifies them.** Golden tests (a directory of
   `.hero` files each paired with expected output) remain the single most important artifact of
   this project. Each milestone's five adversarial cases stay marked
   `# UNVERIFIED — pending debrief` until the author has said what each one guards against.
3. **When something breaks, fix it and keep the symptom.** The raw symptom and the fixing commit
   are queued so the author can hypothesise before reading the fix — diagnosis is still where the
   density of learning is highest; it just no longer blocks the pipeline.

Also maintain a `DESIGN-LOG.md`: one line per decision plus the reason. In three months this is the
only thing that distinguishes reasoning from improvisation.

A declared end-goal of the journey itself: a **mini-book about how this language came to be** — the
adventure, the decisions, the prompts, the wrong turns. The journal, the DESIGN-LOG, the panel
records and the story beats in `docs/book/` are its raw material, collected as we go; see
`docs/book/README.md`.

### Two definitions of success

**Near-term (the project is real):** a program that lexes, parses, type-checks, and emits C that
compiles and runs, for a subset of the language, on macOS ARM64.

**Long-term (the language is real):** the language can express its own compiler. A compiler needs a
tree with heterogeneous nodes, symbol tables, lists, recursive functions, error handling, and
multiple files. If the language can write that, it is not a toy. Historical precedent:
**Pascal-P4**, Wirth's self-hosting Pascal compiler, is roughly 4,000 lines of Pascal — published as
a book because it could be read end to end. That is the target scale.

The acceptance test for self-hosting is the classic bootstrap **fixpoint**: the Rust bootstrap
compiler builds compiler A from the Heroes source; A compiles the same source to `B.c`; B compiles
it again to `C.c`; `B.c` and `C.c` must be byte-identical. (Generated C, not Mach-O binaries —
binary identity additionally depends on clang/ld noise such as `LC_UUID` and DWARF paths.) At the
fixpoint the bootstrap compiler is archived and never maintained again: the final picture is
Heroes → C → native binary, with no third language anywhere.

### The measurable thesis

This language has one novel claim, and it is empirical, not aesthetic:

> A language designed so that *every plausible LLM mistake is a compile error* will produce working
> programs in fewer total tokens than a conventional language, even if its per-program token count
> is higher.

The number that tests this: **give the spec to an LLM, ask for ten programs, count how many pass the
type checker on the first try.** Then change one thing and re-measure. Nothing in this document is
proven. It is all argument. Build the measurement harness early.

A second, more diagnostic metric: take ten correct programs, inject one plausible error into each
(swapped same-typed arguments, missing variant case, forgotten `@`, mutation of an undeclared name,
typo'd identifier), and count how many produce a **compile error** rather than a silently different
program. Every design decision below was chosen to raise this percentage.

---

## Part 1 — Design philosophy

These are the principles. They are load-bearing. Every concrete rule in Part 4 is derived from them,
and if you need to make a decision this document doesn't cover, derive it from these rather than from
what other languages do.

### 1.0 Principle zero: the smallest self-consistent surface

> **Every syntactic form is a place where a bug can hide. The language is finished for v1 when it
> can compile itself.**

Self-hosting is **necessary, not sufficient**, as a criterion for a form to enter v1 — the stricter
version ("reject anything the compiler doesn't need") would delete the thesis features themselves
(`???`, the same-typed-argument rule, rich errors), which no compiler needs and this language exists
for. The burden of proof for any proposed form:

- it is on the **closure list** below (the compiler needs it), **or**
- it **provably serves the thesis** — a measured effect on the Part 11 metrics, or an argument
  derived from Part 1 that the panel accepts.

Neither → it waits, regardless of elegance. This gives the design a **stopping rule**, which
language designs normally lack, and it makes Part 7's ordering non-negotiable until the closure
list compiles itself. Ratified by panel 005 (2026-08-03) — with the historian's honesty clause on
record: no verified language ever used self-hosting as a feature freeze; this is a deliberate
departure. The closure-list audit runs at the M-generics-library checkpoint, under three riders: it is mechanical
(grep of the appendix plus a representative compiler pass, not opinion), it audits **spec
coverage** and arrives with named cuts (modules, file I/O, `args()`, `exit` are on this list but
not yet in the spec — mortgaged budget), and it assigns `file I/O`/`args()`/`exit(code)` a
runtime tier (§4.20 Tier 1 vs plain externs).

**The closure list** (what writing this compiler in Heroes requires): `record` · `variant` +
exhaustive `match` · `[T]` · `{K: V}` (order unspecified, panel 026) · `str` · `i64` ·
`bool` · `()` · `T?` with `?`/`.must()`/`.default()` · `=`/`@` bindings · `@` parameters ·
`if`/`else if`/`else` · `while cond` / `for x in xs` (including over maps) / `break`/`continue` ·
`return` · UFCS · function values · generics on functions · `test`+`assert` · file I/O · `args()` ·
`exit(code)` · modules · **spawning a process** (author instruction 2026-08-12: *the self-hosted
compiler must be complete*). The last row is the one the audit could not find: measurement 003 is
mechanical, so it audits what is *on* this list and structurally cannot see what is absent from it.
After the archive `heroes build` and `heroes run` are Heroes programs and they invoke clang, and
`int64_t system(const char *)` is `conflicting types for 'system'` — panel 030 R3's wall on a row
nobody had counted (panel 036). M-selfhost-probe prices it; the shape is a `hero_spawn` in
`hero_os.h`, beside the file and argument rows it already carries.

**A second absent row, found the same way and closed on the spot** (2026-08-12,
panel 039's verification; author decision in `/decide`): **the mangler's typehash**.
`h_library_map_<typehash>` reaches the generated C of any program that uses a
generic, and the fixpoint compares that C byte for byte, so the port must compute
the identical number — which `emit/mangle.rs` said in a comment while being FNV-1a,
whose every ingredient is inexpressible in Heroes: an unsigned offset basis
(`int_out_of_range`, because `i64` is signed), a wrapping multiply (`panic: integer
overflow`, because §4.3 makes overflow an abort), and `^` (`reserved_operator`).
The hash is now a polynomial modulo 2^31 − 1 whose intermediates fit an `i64` with
three orders of magnitude to spare, and
`tests/golden/run/premise-mangler-hash-in-heroes.hero` computes it in Heroes so the
premise has the test CLAUDE.md §11 asks for. **The lesson is the audit's, not the
hash's**: measurement 003 is mechanical over this list, so a requirement absent
from the list is invisible to it — twice now, and both times the missing row was
something the *compiler itself does* rather than something a Heroes program
contains. Library closure: `print`, `len`, `push`, `slice`, `chars`, `keys`, `sort`,
`join`/`Builder`, `to_i64`/`to_f64`/`to_str`, `panic`, plus `map`/`filter`/`fold`/`find`/`any`/
`all`/`range` written in Heroes.

One pattern deserves stating now because the whole self-hosted compiler will be written in it:
**error accumulation**. `T?` carries one error and `?` aborts the current computation, but a
resolver and a type checker must *collect* diagnostics. With no globals and no closures, the
compiler threads a mutable list through its passes: `@diags: [Diagnostic]`. This is ordinary
Heroes — but the spec shows it as an idiom, and the golden tests pin it.

### 1.1 The triangle, and its hierarchy

Three forces, and they are **not equal**:

- **Implementation simplicity is a constraint, not a goal.** It sets the ceiling. If we exceed it the
  compiler never gets finished and there is no project. It is non-negotiable but it is not the thing
  being maximised.
- **LLM comprehension is the objective.** Operationally this means one thing: the percentage of
  programs that compile on the first attempt.
- **Token economy is an indicator, and frequently a misleading one.**

Operational hierarchy: simplicity sets the ceiling, comprehension decides, tokens break ties. When
tokens and comprehension conflict, **tokens win only when comprehension is indifferent**.

### 1.2 The cost formula

```
real cost = program tokens × (1 + rewrite rate)
```

The two factors differ by orders of magnitude. A clever compact construct might save 5% of a file's
tokens. A single correction round-trip — model receives error, re-reads a file, rewrites a function,
recompiles — costs 500–2000 tokens. On a 3000-token program, **one correction round wipes out ten
syntactic optimisations**.

Therefore: any construct that saves tokens but raises error probability is almost always a net loss,
even when the saving looks large and the risk looks small. This inverts the natural instinct.
The instinct says "shorten everything and be careful". The formula says **"eliminate errors and only
shorten when it's free"**.

### 1.3 Locality is the currency

An LLM reads *locally*. It has no project in its head, cannot jump to a definition, does not reliably
remember a file it read twenty messages ago. So:

> **A construct is good for an LLM if the meaning of a line can be determined from that line plus the
> signature of the enclosing function.**

Constructs that fail this test — and note that they are almost all *cheap in tokens* and *beloved by
humans*: inheritance, operator overloading, macros, global type inference, dynamic dispatch, implicit
conversions, ambient context (`with`, `using`, globals). The category to reject is not "verbose
constructs", it is **"constructs whose meaning lives elsewhere"**.

Fortunate side effect: non-local constructs are also the most expensive to implement. Maximising
locality satisfies comprehension *and* simplicity, and is roughly neutral on tokens. The triangle
mostly collapses to a point when viewed from here.

### 1.4 Redundancy is not waste

Natural language is redundant, which is why a typo leaves a sentence comprehensible. Verbose
languages act as a safety net: when an LLM makes a mistake in Java, it typically produces a
*compile error* rather than a silently different program.

In an extremely compact language every token is load-bearing, so one wrong token produces a program
that is **different but valid** — a silent bug. Real-world evidence: **K** and **APL** are genuinely
ultra-compact and in production use, and LLMs work poorly with them. Not for lack of training data
alone: each glyph carries too much, and getting one wrong yields the wrong answer rather than an
error.

So redundancy is spent deliberately, where errors actually occur, and nowhere else.

### 1.5 The declaration/use asymmetry

A construct appears in two kinds of place:

- **Declaration sites**: one per entity.
- **Use sites**: many — ten, a hundred.

Tokens are paid mostly at use sites. Comprehension is gained mostly at declaration sites, because
that's where the contract lives. **The two needs are in different places, not in conflict.** Rule:

> **Verbose where you declare. Terse where you use.**

Full signatures, explicit types, documentation at the declaration — paid once. No redundant
annotations at use sites; anything derivable is derived. Note this is the *opposite* of many
"expressive" languages, where declarations are short and use sites carry annotations.

### 1.6 The spec budget

**The entire language specification — syntax, semantics, built-in library — must fit in 4096
tokens, measured.** About five pages. That document is not documentation, it is *the prompt*.

This is the forcing function that makes the whole project coherent, because it merges two goals into
one: every feature has to pay rent in spec tokens, and a language whose spec fits in a few pages is
necessarily a language one person can implement. Want generics? That's ~200 tokens of spec — justify
it. Want three ways to write a loop? Triple cost, zero gain.

**MEASURED, 2026-08-04 (panel 011): spec v0 is 1989 tokens (Anthropic legacy) / 2048 (cl100k) /
2050 (o200k).** The word heuristic every earlier verdict relied on said 1496 — low by a third. So
the 1500 budget was breached on the day it was written and nobody could see it, and the 2000
ceiling that replaced it was already breached on the pessimistic bound the hour it was set.

**Raised to 3000 by author decision (2026-08-04; panel 012, retro-record) — the first budget number
in this project's history set against a measurement rather than a guess.** It left ~950 tokens of
headroom on the binding instrument, which was taken to be roughly what §1.0's mortgaged closure items
(modules, file I/O, `args()`, `exit`) plus the deferred sentences would cost. **Raised again to 4096
by author decision (2026-08-10; panel 024, retro-record.)** The budget is a **hard measured ceiling
of 4096**, taken as the maximum over the vendored instruments — which reconciles the number with the
panel's veto threshold. The reader's real tokeniser is unpublished, so the ~2% spread applies to the
ceiling too: the effective bound is 4010–4096.

**The soft line stays at 2000, and that is the load-bearing half of the raise.** It is not a fraction
of the ceiling. It is the point where growth stops being free — where an addition owes a named
removal or a pre-registered falsifiable prediction (panel 012) — so rescaling it in proportion
(2:3 would give ~2730) would put the spec, at 2231, *under* it and grant a blanket exemption to the
next ~500 tokens. Panel 024's warden put the reason in one line: **a soft line that moves whenever
the document approaches it is a thermometer, not a thermostat.**

**What the raise is not backed by, recorded rather than argued away** (panel 024). The mortgage above
justifies about 3181 (2231 + 950), which clears 3000 by 181 and is a precise reason to raise. 4096
leaves **915 tokens with no named claimant — 41% of the current spec** — and the mortgage figure was
itself derived *from* the old headroom (3000 − 2048 = 952) rather than costed per item. Measured over
the document's first week: seven amendments, **+183 tokens total (+8.9%)**, **zero removals**, while
the ceiling moved +173%. The warden's summary is the fairest reading available: the number tracks
neither the document nor a costed need, and 4096 is the shape of a context window rather than of a
language.

**So the veto moves and something has to replace it.** At the observed mean of +30 tokens per
amendment, 4096 binds after ~61 more amendments where 3000 would have bound after ~25 — a threshold
that fires after five dozen panels is not an instrument. Two checks are owed, both enforceable at
2231 rather than at the ceiling, and both are queued rather than adopted here because they are
architecture (CLAUDE.md §4): a **delta gate**, failing when a commit's spec delta exceeds ~+50 tokens
without a named removal or a registered prediction — level-independent, so it survives the next
raise — and a **mortgage ledger** quoting each closure item separately, so that 2231 + Σquotes
clearing the ceiling is arithmetic somebody can read.

**Why 1500 fell.** (Raised to 2000 on 2026-08-04, panel 009; superseded the same day by the measurement above.) Two reasons
are on the record. First, 1500 was a forcing function, never a measurement — and it was being
defended with a 1.33 tokens/word prose heuristic applied to a document that is ~14% punctuation by
character, so the figures every budget verdict rested on (~1496) may always have been ~350 low
(panel 008). Second, this document has instructed from the start that the count be taken *with a
real tokeniser, not by estimation* (Part 10); until `heroes measure` does so, the spec's true size
is unknown and no headroom claim means anything.

Note what this leaves unaudited: §1.2's cost formula has two factors, and only the first is now
measured. The rewrite rate needs a model in the loop, so until metric 2 runs the formula remains
the design rule it always was and stops being an audited one (panel 011).

**The discipline the number was proxying for survives independently of headroom: every addition
still carries §1.0's burden of proof — compiler-need or a measured thesis effect — and a bigger
budget buys no exemption from it.** A spec that grows to fill the budget because it can has failed
§1.2 just as surely as one that breaches it. Honest counter-evidence on the mechanism, from the same
measurement: the "named removal" clause has produced **zero removals in seven amendments**. It damps
growth; it does not subtract.

Historical calibration: Wirth's **Oberon report** is sixteen pages, and an entire operating system
was written in Oberon. That is the right order of magnitude for one person.

**The spec has a budget; the compiler does not, and that is now a decision rather than a gap**
(author 2026-08-12, taken in `/decide`; recorded by panel 020's compiler-engineer as an open
question). Measured the same day: `crates/heroes/src` is **25,482 non-test lines**, 27,399 across
both crates — against Pascal-P4's ~4,000 — and 21 files are over CLAUDE.md §11's ~300. A global
ceiling was considered and refused, for a reason worth stating so nobody re-derives it as an
oversight: the bootstrap compiler is **scaffolding with an expiry date** (CLAUDE.md §10's declared
exception, M-selfhost-fixpoint), and a number that constrained it would constrain the artifact the
project is about to archive. What "does it fit one person" measures is the **Heroes** compiler, and
that number does not exist yet — M-selfhost-probe is what produces its first estimate. Until then
the only live ceiling is the per-file one, which is enforced per file, is read every time somebody
opens a file, and has caught two real breaches this way.

### 1.7 Core plus elaboration

A well-designed language is not a set of features. It is a **tiny core** plus a set of
transformations that reduce everything else to that core. The theory calls this a *core calculus*
plus *elaboration*; it is how ML, Haskell and Scheme are formally described.

This is not academic elegance — **it determines the size of your compiler.** Anything in the core
must be implemented in the type checker *and* the lowering *and* the backend. Anything that is sugar
is erased by one function **on the way into the IR** and never touched again (panel 019: the erasure
happens *inside* lowering, not in a pass before it — there is no desugared tree, and `--dump-ir` is
the evidence that the table below was honoured).

**Therefore the working criterion for "is this a simplification?": does it move something from the
core to the sugar, or remove a special case from the compiler?** Not: does it shorten the syntax.

The core is **seven constructs** (Part 5). Everything else — `if`, `for x in xs`, `?`, `T?`, UFCS,
`test`, `assert` — is sugar.

### 1.8 Compare against the real alternative

The most common design error is comparing a feature against *nothing*. Always compare against what
you'd actually write without it. Worked example — generics look expensive at use sites (`<>`, type
arguments), but the alternatives are:

- **Duplicate the code** (`StackInt`, `StackString`) — hundreds of tokens per copy, far worse.
- **Use a dynamic `any`** — cheap in tokens, but converts compile errors into runtime errors, which
  the cost formula forbids.

So generics *pay for themselves* on tokens. The naive intuition was wrong because it compared
against the void.

### 1.9 Semantics familiar, surface original

An earlier version of this design used "familiarity is free tokens" as a tie-breaker on every
surface choice. Applied thirty times in a row, that criterion inevitably produces a clone of an
existing language — and taken to its conclusion it says "just use Rust and write nothing".

The distinction that was missing:

> **An LLM struggles with unfamiliar *semantics*, not unfamiliar *spelling*.**

Tell it that `@` declares a mutable binding and it learns that in one line of spec and never gets it
wrong. Give it an ownership system with lifetimes and it gets it wrong forever. The tokeniser, for
its part, has no preference between `fn` and `::`.

So: **familiar semantics, original surface.** The transfer that matters is conceptual — functions,
records, `match`, errors-as-values — and that is retained regardless of spelling.

### 1.10 Syntax is ASCII-only

Language symbols are ASCII. Strings and comments are full UTF-8. Rationale: non-ASCII glyphs
tokenise badly (2–3 tokens instead of 1), can't be typed on some layouts, and break under
encoding-mangling tools. This is Go's, Rust's and Zig's choice. Related: this is why `§` was
considered and rejected as a symbol.

---

### 1.11 The founding constraint: there is no standard library

**This is where the project started, chronologically and logically. It is a hard requirement, not a
convenience.** The original brief was: *a simple compiled language with modern syntax that can use
other languages' libraries, so I don't have to write a standard library.*

Every architectural decision downstream serves this. Restating it as a rule:

> **Heroes ships a minimal runtime and nothing else. Anything a real program needs — maths beyond
> arithmetic, files, sockets, regex, JSON, crypto, compression, databases, HTTP — comes from C
> libraries through the FFI. Writing a broad standard library is out of scope, permanently.**

This is not laziness, it is the leverage that makes a one-person language viable. A modern standard
library is more work than a compiler.

**Three levels of interoperability exist, and Heroes deliberately targets only the first:**

1. **The C ABI.** C, C++ with `extern "C"`, exported Rust/Zig/Go, Swift `@_cdecl`. **One FFI
   mechanism opens all of it.** This is the broadest and cheapest level, and it is the one we take.
2. **Managed runtimes (JVM, .NET).** Here an FFI is not enough — you have to *run on* that runtime,
   or you pay marshalling on every call. Enormous library access (NuGet, Maven), but you inherit
   that runtime's object model and your language's semantics bend to it. Rejected: it would mean
   emitting CIL or JVM bytecode and inheriting a GC, which contradicts both the memory model and the
   native-binary goal.
3. **Interpreters (Python, Node).** Requires embedding the interpreter. This is Mojo's route to
   numpy/torch. Rejected: the boxing it implies would make every C call require unbox-in/box-out,
   which is precisely the glue we refused to write.

**The accepted cost of choosing level 1:** no access to C++ templates or the STL, and no access to
anything that only exists inside a managed ecosystem. Modern C++ libraries are reachable only through
a C shim you write yourself. This is a real limitation and it is accepted.

**What "minimal runtime" means, concretely.** Two tiers:

*Tier 1 — the C runtime* (must be C; the language cannot express it): allocator, `incref`/`decref`,
the `str` representation with concatenation and slicing, the array with `push` and bounds checking,
the hash map, copy-on-write checks, `panic`, `print`, numeric conversions, `join`. A few hundred
lines. See 4.20.

*Tier 2 — written in Heroes itself*: `map`, `filter`, `fold`, `find`, `any`, `all`, `range`, the
`T?` helpers, and — since panel 036 — **the program's three edges**: `read_file`, `write_file` and
`args`, written over `extern`s against a `hero_os.h` this project ships. This is the payoff from
generics (4.12) — these are library code with `test` blocks, not compiler magic.

**Why the edges are Tier 2 and not built-ins**, which is the decision panel 036 spent its session
on. §1.7's test is mechanical, and `read_file -> str?` fails it: a `T?` is a per-translation-unit
generated struct named by index, so the C runtime has no name for one and cannot return it — the
built-in route needs a second composition arm in the emitter and an ABI bump, and the Tier-2 route
needs zero backend lines. The precedent runs the same way and is unusually clean: **Pascal
predeclared file I/O, command-line access and termination; Kernighan's 1981 paper itemises all
three as defects; Oberon predefines none of them.** Nim, whose surface this project copies, reaches
the same ergonomics from an auto-imported module rather than from the compiler, and Odin could
redesign `core:os` in 2025 *because it was a package*. The one edge that cannot be Tier 2 is
`exit(code)`, which needs `_Noreturn` to survive C's flow analysis — C itself took 22 years and
WG14 N1453 to admit that. **The spec names all four and their signatures, and says nothing about
which tier they live in**, because that is not something a reader of a program can observe.

**That is the whole standard library.** Everything below is explicitly *not* provided, and is
expected to arrive via FFI:

| Need | Where it comes from |
|---|---|
| `sqrt`, `sin`, `pow`, floating-point maths | `libm` |
| File and directory operations beyond `read_file`/`write_file` | `libc` |
| Time, dates, clocks | `libc` |
| Random numbers | `libc` |
| Sockets, DNS, TLS | `libc`, OpenSSL |
| HTTP | libcurl |
| Regular expressions | PCRE2 |
| JSON | write it in Heroes (a JSON value is literally a `variant` with payload — a good fit) or bind cJSON |
| Hashing, crypto | OpenSSL, libsodium |
| Compression | zlib |
| SQL | SQLite |
| Terminal UI, graphics, audio | ncurses, raylib, SDL |

**The consequence for design priorities: FFI ergonomics rank alongside comprehension, not below it.**
When a choice makes bindings harder, that is a serious cost, not a footnote. Several decisions
already recorded in this document exist for this reason and should not be revisited:

- **Indices from 0** — otherwise every boundary becomes a silent off-by-one (4.9).
- **Strings always NUL-terminated** (`len+1` allocated) so `.cstr()` is free with zero copies (4.20).
- **No exceptions** — unwinding across the C boundary is undefined behaviour (4.6).
- **Errors as values** — maps onto C return codes without machinery (4.6).
- **Refcounting rather than a tracing GC** — a tracing GC would need stack maps and safepoints, and
  would force pinning and finalisers around every pointer handed to C (see the memory model).
- **Native compilation rather than wasm** — wasm cannot call native C libraries at all (3.2).

**The tension this section used to carry is resolved.** An earlier revision chose QBE for its
learning value while admitting that "QBE makes the FFI harder than emitting C would" — no headers,
every signature transcribed by hand, macros and `inline` functions unreachable, nothing verifying
declarations against reality. Since FFI ease is a founding constraint and not a preference, the
backend is now **C emission** (§3.1; decided in panel 001), and the three points that managed the
tension become:

1. **The internal IR stays clean and backend-agnostic** — a claim that is only redeemable by
   actually building a second backend, which is why the QBE backend is *scheduled* post-fixpoint
   (Part 7 item 14) rather than left as "maybe".
2. **A thin C shim remains standard practice** for C++ libraries and awkward struct-passing: a `.c`
   file exposing plain functions, compiled by `heroes cc`, linked in. Macros, `inline` functions and
   `#define` constants are now reachable directly, so shims are for the hard cases only.
3. **FFI signatures are verified by clang against the real header** (`importc`-style declarations,
   §4.19) — a wrong type in an `extern` is a *compile* error, which is this project's thesis applied
   to the boundary.

## Part 2 — Non-goals

State these plainly so nobody drifts:

- **Performance is not a goal.** Not a secondary goal, not a tiebreaker. Where a choice trades speed
  for simplicity, take simplicity every time. (This is what licenses value semantics with
  copy-on-write, aborting on overflow, boxing in an array for recursive types, etc.)
- **No standard library beyond a minimal runtime.** See 1.11 — this is the founding constraint, not
  a consequence. Do not accumulate convenience functions; every one you add is a permanent
  maintenance obligation and a competitor to the FFI.
- **Not a systems language.** This is a high-level language with an excellent C FFI. Low-level work
  is delegated to C. The runtime itself will be written in C.
- **Not for the web, for a long time.** Frontend needs a wasm backend (a separate project, and wasm
  can't call native C libraries, which breaks the founding premise). Backend needs concurrency,
  which doesn't exist yet. See Part 9.
- **No concurrency in v1.** This is the largest gap and it is deliberate.
- **No typed variable inspection in v1.** Line-level debugging works: generated C carries `#line`
  directives, so lldb breaks on and steps through `.hero` source lines. But `p x` shows a mangled C
  temporary, not a Heroes value — printing rich values is still `print`'s job.

---

## Part 3 — Toolchain

### 3.1 Backend: C emission (C11)

The compiler lowers to a three-address IR with explicit basic blocks (Part 10 step 6 — that stage
is the heart and it stays), and **emits C11 from the IR**, compiled by clang. Decided in panel 001;
the grounds are §1.11 and §4.19 — the FFI is the founding constraint — never optimisation, which
Part 2 rules out as a justification.

**What clang does for you:**

- **The full C ABI**, per target, maintained by people paid to get it right. Struct passing,
  varargs (including Apple ARM64's on-stack variadics), alignment — never your problem.
- **Header access.** `#include <sqlite3.h>` reaches macros, `inline` functions and `#define`
  constants, and — the property that matters most — **clang verifies every `extern` signature
  against the real header**. A wrong FFI type is a compile error, not a runtime disaster.
- **Debug info.** `#line` directives map generated C back to `.hero` lines; lldb breaks on and
  steps through the author's source.
- The entire LLVM optimisation pipeline at `-O2` — a welcome side effect, not a reason.

**What the emitter must do (the obligations are ours, not clang's):**

- **Declaration ordering.** Heroes' top level is order-free with free mutual recursion (§4.2); C is
  not. Emit prototypes for all functions and `typedef struct`s topologically sorted by by-value
  containment, in deterministic order. **An edge is a field C lays out by value** — the property,
  never a list of permitted indirections (panel 023, measured). The earlier parenthetical here read
  "cycles are legal only through `[T]`" and was wrong in both directions: `record Scope { vars:
  {str: Scope} }` compiles at 8 bytes, and `record R { next: R? }` is `field has incomplete type`
  because a `T?` is by value and therefore **transparent** — it propagates the edge into `T` rather
  than breaking it. The property gets every row right without naming one, and it cannot drift when
  M-generics-library adds function values. A cycle over these edges is `no_size` (§4.17's class, panel 023); the
  same walk yields the order, so the checker computes it and the emitter only filters it.
- **Name mangling.** A Heroes identifier can collide with a C keyword (`default`, `register`) or a
  libc symbol (`index`, `y1`). Every user name becomes `h_<module>_<name>[_<typehash>]`; fields,
  variant cases and labels are mangled too; runtime names are `hero_*`; `extern` FFI names pass
  through unmangled by design. The rule lives in one place (the mangler).
- **Control flow as blocks.** One `goto`+label per basic block, an explicit entry `goto bb0` (the
  entry label is otherwise an unused-label warning), all locals hoisted to the function prologue
  (`goto` may not jump over declarations). A block is emitted **only if some edge targets it**:
  the IR's own `preds` decide, which keeps `-Wunused-label` alive as a check that the emitter's
  `goto` edges agree with the IR (panel 020 — a predecessor-less join block warns on a *correct*
  program, and `-Wno-unused-label` would hide the emitter bug along with it).
- **`#line` when the instruction's line differs from the current effective line** — not per
  statement, and not per span change: `#line N` anchors the *next* line and C then
  auto-increments, so a Heroes line lowering to K C lines drifts by K−1. Restored to the generated
  file around synthetic prologue/cleanup code, so lldb never blames user lines for housekeeping —
  which means the restore directive carries the printer's **own output line count**, making a
  newline-counting writer structural rather than incidental (panel 020, measured against
  `tools/spike/01-first.c`, which advertised `:2` and delivered `:6`). Debugging the emitter
  itself is the *author's* activity, not the tool's capability: it is three lines in the emitter's
  test helper, and `--no-line` is refused by CLAUDE.md §10's stopping rule (panel 016's watch
  list, settled in panel 020).
- **The `@` parameter is a pointer parameter.** `Op::CopyOut` writes the *caller's* place, which
  the callee cannot reach in C, so §4.8's copy-in/copy-out becomes: a `T *p_l` parameter, a
  prologue `T l = *p_l;`, and `*p_l = l;` at every `CopyOut` — in parameter order. `f(@x, @x)`
  cannot arise, because panel 010 made two `@` arguments sharing a root a compile error; that rule
  is what makes this convention total (panel 020).
- **UB is not a diagnostic.** Arithmetic aborts via `__builtin_*_overflow` (§4.14) — whose
  overflow verdict is against the *destination* type, not the operands, which is why Part 7's
  `c_int` will move the threshold from 2⁶³ to 2³¹ with no visible change at the call site — every
  type-system-proven-unreachable point gets `hero_unreachable()`, and generated C compiles with
  `-Wall -Werror=return-type -Werror=uninitialized -Werror=format -Wconditional-uninitialized
  -fno-strict-aliasing`. `%` is guarded exactly like `/`: `INT64_MIN % -1` is UB too, and on arm64
  it does not trap — it returns a wrong answer at exit 0. A missing return after an
  exhaustive `match` must not become whatever `-O2` feels like, and a missing return anywhere else
  is a Heroes diagnostic (`missing_return`) rather than a clang error naming a mangled symbol.
- **Determinism.** Same input → byte-identical `--emit-c` output, enforced by a double-emit diff in
  CI from the first emitting milestone. The bootstrap fixpoint (Part 0) depends on it; no
  timestamps, no absolute paths, no hash-seeded orderings anywhere in the pipeline — and the
  emitted C **never mentions the output path**, so it is the same bytes on stdout and in `-o a.c`.
- **`f64` literals emitted round-trip-exact** (`%a`).
- **Where the runtime comes from**, since an installed compiler has no repository above it:
  `$HEROES_RUNTIME`, then `runtime/` under the working directory, then `runtime/` under an ancestor
  of the executable. The header defines `HERO_RUNTIME_ABI` and every generated translation unit
  `_Static_assert`s it, because a directory named `runtime/` is the commonest thing in the world:
  measured, a decoy replaced the contract with **zero warnings under `-Weverything`** and printed
  `0x1e` where `30` was expected (panel 020). The cached `runtime.o` is keyed on the contents of
  `runtime.c` and the header, plus the flags — a stale relink is otherwise silent — and one `.o` is
  correct across `-O0`, `-O2`, `-flto` and the sanitisers.

Pipeline:

```
heroes build prog.hero        # emit build/<hash>/prog.c, clang, link runtime.o
heroes run prog.hero          # same at -O2, then execute (the dev loop)
heroes build --emit-c ...     # stop at the C and read it (stdout, or -o)
```

Requires Xcode Command Line Tools for clang and the linker. macOS provides no static libc, so
`-static` will not work; binaries always link dynamically to `libSystem`.

**macOS note that survives from the QBE era:** Apple ARM64 passes variadic arguments on the stack —
under C emission this is entirely clang's problem, which is part of the point.

### 3.2 Alternatives that were considered and rejected

Document these so they don't get relitigated:

- **QBE** (`https://c9x.me/compile/`) — the primary backend for most of the design process, and the
  scheduled second backend now (Part 7 item 14). "LLVM but simple": ~12k lines of C, textual SSA IL
  readable in an evening, a fuzz-tested C ABI, register allocation, and non-SSA temporaries (you
  never construct SSA or emit `phi`). Its `arm64_apple` target handles Mach-O symbols and Apple's
  on-stack variadics. Rejected as primary for the reason §1.11 records: **no headers** — every FFI
  signature transcribed by hand with nothing verifying it, macros and `inline` functions
  unreachable, and no DWARF. It was originally chosen *for the learning value* (contact with basic
  blocks and registers); that lesson now lives in the scheduled second backend, `clang -S`, and the
  hand-written spikes. Reference implementations if that day comes: `cproc` by Michael Forney, and
  chapter 63 of DoctorWkt's *A Compiler Writing Journey*.
- **LLVM (C++ API)** — rejected. It does not solve the FFI, it makes it worse: LLVM IR sits *below*
  the ABI, so you decide struct passing, varargs and alignment yourself. Plus a multi-GB dependency
  with an API that churns every release, mandatory SSA construction, and manual DWARF.
- **LLVM IR as text (`.ll` → `clang file.ll`)** — viable, and interesting for learning SSA, since
  you can inspect passes with `opt`. Still leaves ABI work. A possible future third backend.
- **MIR** (~20k lines, C API, ~70% of `gcc -O2`), **libgccjit** (excellent ABI handling, but GPL and
  requires `--enable-host-shared`), **Cranelift** (good, but really wants a Rust frontend; note
  that its reputation for "memory optimisation" is a misreading — it's optimised for *compile speed*
  and verifiability, and its memory work is about wasm linear-memory bounds checks, not about your
  language's memory model).
- **wasm** — interesting as a future backend (sandboxing, runs in browser, and structured
  control flow makes for a genuinely instructive relooper problem). Rejected as primary because
  **wasm cannot call native C libraries**, which destroys the "don't write a stdlib" premise.

**Why C11 emission won** (formerly "Emitting C99 — the fallback"): zero ABI work,
`#include <sqlite3.h>` gives the FFI for free *with verification*, `#line` gives debugging, and the
lineage is the strongest in the business — the first C++ was `cfront`, a C emitter, as were Nim,
Vala, Chicken Scheme, Cython and early Haskell. The compile toolchain is clang only (`tcc` is not
usable on Apple Silicon; `zig cc` remains interesting for future cross-compilation).

### 3.3 The compiler must be a library

Conventional compilers are one-shot programs: run, read files, emit binary, exit. But several
features here require *interrogating* the compiler and receiving answers: expanding a typed hole,
generating a module outline, canonical formatting, and eventually a language server.

**Write the compiler as a library with a thin CLI on top.** The library exposes operations: give me
the AST of this file, give me the outline, pretty-print this function, type-check and report. The
CLI is one client among several. This is not extra work now — it is just not writing a `main()` that
does everything — but retrofitting it later is painful.

### 3.4 Implementation language: Rust, in the Cyclone subset

**Decided: Rust** (the author's preference, and second in the original comfort list). Enums with
payloads plus exhaustive `match` fit an AST exactly, and ownership *enforces* the no-aliasing
discipline Heroes has by construction. The bootstrap compiler is scaffolding with an expiry date:
at the fixpoint (Part 0) it is archived and never maintained again.

It is written in the **Heroes subset of Rust — the Cyclone rule** (named for the precedent in the
appendix: references only as function parameters, never in structs or return types, which
eliminates lifetime annotations entirely and maps 1:1 onto Heroes' `@` parameters):

- **Forbidden**, enforced by `clippy.toml` and `#![forbid(unsafe_code)]`: `Box`, `Rc`, `Arc`,
  `RefCell`, `Cell`, `HashMap`/`HashSet` (nondeterministic iteration breaks the fixpoint — `BTreeMap`
  only), stored closures, `dyn`, trait bounds on generics, references in data structures.
- **Allowed**: owned `struct`/`enum` + exhaustive `match`, `Vec<T>` → `[T]`, `BTreeMap<K,V>` →
  `{K:V}`, `String` → `str`, `Option`/`Result` → `T?`, `?` → `?`, `&mut` **only as a parameter** →
  `@`. Iterator/`Option` closures are fine (expressions, not stored state). Links are indices, never
  references.
- Every necessary violation carries `// PORT-DEBT: <reason>`. The count is the measured distance
  from self-hosting, it must not ratchet upward, and each entry is a design finding about Heroes —
  the same way Part 8's warts were found by writing real programs.

### 3.5 The tool is one command

One executable, `heroes`, with subcommands — the `zig`/`cargo` model, and the direct corollary of
§3.3: one library, one thin CLI, many subcommands. **A second binary never exists**; neither does a
Makefile, a script, or a separate formatter/test-runner/LSP/package tool. Inspection is flags
(`--dump-tokens`, `--dump-ast`, `--dump-ir`, `--emit-c`, `--emit-asm`, `--pipeline`), not
subcommands. `heroes run` is the dev loop: emit C, clang, execute, hash-keyed cache in `build/` —
the runtime compiles once and never at the user's initiative.

Dependencies, honestly: with no standard library (§1.11), a v1 "dependency" is a link flag declared
in the source next to the `extern` that needs it. No package manager exists before modules do; when
it arrives it will be `heroes add`/`heroes fetch` — inside the same binary.

One declared exception with an expiry date: `cargo build`/`cargo test` build *the compiler* until
the fixpoint (as Zig's contributors use Zig's build system while users type only `zig`); after M-selfhost-fixpoint,
`heroes` is the only command for compiler development too.

---

## Part 4 — The language specification

### 4.1 File structure

One file is one **module**, and the file you compile holds `main`. `use geom`
binds `geom` to the declarations in `geom.hero` beside it, written qualified
(`geom.dist2(a: p, b: q)`, `p: geom.Point`); every module you name needs its own
`use`, there are no aliases and no wildcard, and modules may not form a cycle.
The whole program is still emitted as **one `.c`** — one `.c` per module with
prototypes across them and a per-module cache is a build architecture, deferred
to Part 10 step 18 (panel 030 R1, panel 031). This paragraph read "one file, one
program, **no modules in v1**" until panel 031.

```
## Section title
#
# Prose in **markdown**. See `dist2`.

# Doc comment for the entity directly below.
name = entity: type
    body
```

- `#` starts a comment to end of line. There are no block comments.
- **Comment content is markdown.** Not enforced, but the compiler knows and reuses it. Rationale:
  markdown is the format LLMs are trained on more heavily than any programming language. This is not
  aesthetics, it is choosing the format the primary reader reads best.
- **A comment adjacent to a declaration *is* its documentation.** No separate `///` form. A blank
  line between comment and declaration makes it an ordinary comment. This is Go's rule, proven at
  ecosystem scale, and it saves a form from the budget.
- `##` is a section heading — reads as both "comment plus markdown h2" and, visually, as a heading.
- The compiler reuses doc comments in `???` output, in error messages, and in `outline`.
- Entry point: `function main()`.

### 4.2 The four entities

Everything at top level starts with its **kind**. One form (panel 018):

```
kind NAME …
    body
```

`constant` declares a value, so its name takes `: type`; a `function`'s parameter list attaches
directly to its name, as at the call site (`function dist2(a: Point, b: Point) -> i64`).
`record` and `variant` *are* type definitions rather than values, so nothing follows the name.

```
constant MAX_DEPTH: i64
    64

function dist2(a: Point, b: Point) -> i64
    dx = a.x - b.x
    dy = a.y - b.y
    return dx*dx + dy*dy

record Point
    x: i64
    y: i64

variant Token
    num
        v: i64
    name
        s: str
    plus
    times
    lparen
    rparen
```

**Rationale for the uniform entity form.** The previous iteration used `= st` / `= en` / `= fn`,
which was uniform in *prefix* but not in what followed: a value on the same line for constants, an
inline list with separators for enums, an indented block for structs, an inline signature plus block
for functions. Four shapes. The entity form makes it genuinely one shape, and buys three things:

1. **Variants with payload become natural.** A variant case *is* a small record, so it is written as
   one — no `|` separator syntax, no special case. Products and sums are finally symmetric in the
   surface syntax, not just in the theory.
2. **`=` and `@` separate cleanly**, which *deletes* a rule rather than adding one. See 4.4.
3. One shape means less spec and less hesitation.

**Rationale for the keyword-first shape** (panel 018 — the `name = entity` iteration between the
two). The `=` iteration had litigated only what *follows* the keyword; which side of `=` the keyword
sits on was a first visit, and the evidence all pointed one way. `name = entity` made `=` carry two
productions, left `test "…"` and `extern` outside the shape, and kept two headers ending in
non-enders (the panel-007 terminator patch existed only for them). Keyword-first with the parameter
list attached to the name (`function dist2(a: Point, b: Point) -> i64`) deletes three compiler
special cases, keeps `=` mono-semantic ("binds once, forever" is now globally true), unifies all six
top-level heads, measures −8 spec tokens, won the blind experiment (pre-registered 55% vs 45%
whole-file first-try), and is the Pascal→Go→Carbon mainline — while `keyword = name` is attested in
zero of seventeen shipped languages checked. `=` at top level is gone entirely: a declaration is not
a binding, and the surface stops claiming it is (this also retires §4.13's old complaint that
`dist2 = function: …` said "dist2 *is* a value you can pass"). The Python suite colon stays out
(E-vs-F, pre-registered both ways, first metric-2 run is the tiebreaker) but is caught with a
`Certain` fix, since it is the top predicted slip of the colon-less shape.

**Rationale for the vocabulary.** `record` and `variant` are a matched pair from the same tradition —
product and sum, the two ways types compose — from Pascal's *variant record*, carried into ML, and
today into Java and C# `record`. The rejected alternatives:

- `structure`/`enumeration` (from C) were a mismatched pair that hid the symmetry. Worse,
  "enumeration" means *a list of names for integer constants*; ours carries payloads, so it is a
  sum type, and `enumeration` described only the degenerate case.
- `union` (the machine-level word) misreads badly: C's `union` is **untagged**, hence unsafe, while
  ours is tagged with exhaustive `match`. A reader seeing `union` expects the dangerous thing.
- `subroutine` was considered because at the machine level it is accurate — the backend emits the
  same `call` either way, return value or not, and the mechanism is Wheeler's 1951 subroutine. Rejected because in FORTRAN, BASIC and Pascal tradition `subroutine`/`procedure`
  specifically means *returns nothing*, and unlike `procedure` the word was never rehabilitated as
  an umbrella term. Also it would put three theory-level words next to one machine-level word.
- `function` is kept as the modern de-facto umbrella (C, JS, Python, Rust), and it is a single
  token. `procedure` (validated as an umbrella by Odin and Nim, which reserve `func` for
  *guaranteed-pure* functions) and `routine` (Eiffel's explicit umbrella) are acceptable
  alternatives if the purity connotation of "function" grates. Note: `structure` and `enumeration`
  are probably 2–3 tokens each, while `function` and `constant` are single tokens — so if the words
  are ever shortened, shorten those two and leave the others.

**There is no `variable` entity.** Mutable globals are forbidden (locality). Mutable bindings exist
only as locals.

**Declaration order does not matter.** All top-level names are visible throughout the file. No
forward declarations, mutual recursion is free, and the model can emit functions in any order.

### 4.3 Types

| Type | Meaning |
|---|---|
| `i64` | 64-bit signed integer |
| `f64` | 64-bit float |
| `bool` | `true` / `false` |
| `str` | immutable UTF-8 string |
| `[T]` | dynamic array |
| `{K: V}` | map |
| `T?` | fallible: a `T`, or an error |
| `()` | no value — the return type of functions that return nothing |

**Eight integer types** (author decision 2026-08-12, `docs/panel/042`; landed at
`M-sized-integers`): `i8 i16 i32 i64` signed, `u8 u16 u32 u64` unsigned. `int` is **not** a word in
this language — it carries forty years of conflicting widths, so a reader must know the platform to
know what it means, where `i64` is ambiguous to nobody. A literal takes the width its context asks
for and is an `i64` where nothing asks; overflow aborts at every width; `s[i]` is a `u8`;
`fit_<width>` converts, returning `T?` where the value may not fit and `T` where it cannot.

**This paragraph said the opposite until 2026-08-12 and the correction is the record, not a
rewrite** (panel 043). It read *"One integer type only. No `i8`, `u32`, `usize`… Keep them out of v1
without debate"*, and the fear behind it was real but misdirected: what makes widths expensive is
C's **promotion lattice**, the silent conversion that decides what `a + b` means before either name
is read. §4.3 forbids implicit conversions outright and always did, so there was no lattice to
specify — the whole addition came to **+188 spec tokens**, against the "most expensive spec item
that exists" this paragraph warned of. The half that was true is preserved above: conversion rules
are where the errors live, which is why every one of them is written and none is inferred.

**No implicit conversions.** `1 + 2.0` is a compile error. Convert explicitly: `to_f64(x)`,
`to_i64(x)`.

**Character literals are `i64`.** `'+'`, `'0'`, `' '`. No new type, no conversion, ~70 lines in the
lexer (the original text said "one line"; the measured cost is on record in panel 008).
**Rationale:** the first lexer written in this language contained `c == 43`, `c >= 48 && c <= 57`,
`c == 40` — seven magic numbers in twenty lines, and writing `43` for `+` is an error no compiler
can catch. This was judged the best benefit-to-cost modification in the entire review.

**Escape sequences: five, split by context** (panel 008). In a string: `\n` `\t` `\\` `\"`. In a
character literal: `\n` `\t` `\\` `\'`. A `'` needs no escape inside a string and a `"` needs none
inside a character literal, so **each character has exactly one spelling** — §4.15's canonical-form
rule survives into the literals. A character literal therefore holds exactly one *character*: one
ASCII character or one escape, so `'\n'` is four source bytes and one character.

**The backslash is reserved**: any other character after it is a compile error whose diagnostic
names the legal escapes and carries a `certain` fix. This half is the point. Without it,
`print("a\nb")` compiles and prints four characters — a plausible model mistake that is *not* a
compile error, which is the thesis inverted. Go, Rust and Zig all error on unknown escapes from day
one; C's permissiveness is what Python has been unwinding since 2016. **The set is frozen**: `\0`,
`\xNN`, `\u{...}` and octal escapes need a panel, because they can produce an interior NUL, which
silently truncates every C call and voids §4.20's guarantee that `.cstr()` is free.

**Known residual trap** (found in implementation, panel 008): `"C:\temp"` cannot be made loud —
`\t` is legal, so the path silently becomes `C:<TAB>emp`. Every language with C-style escapes
carries this; the remedy is raw string literals, which are not v1 material. Recorded in Part 8.

**Strings** are immutable UTF-8, **indexed in bytes**. `s[i]` yields a `u8`. Iterate
characters with `s.chars()`, which yields single-character `str`. Slicing that lands mid-sequence is
an error. Do **not** introduce an indexable `char` type — that is the trap Python 3 fell into. (The
`byte` type was removed: it existed only to index strings, and `i64` does the job. One fewer base
type; the memory waste is irrelevant here.)

**Structural equality on everything.** `==` works on ints, strings, records, variants, arrays, maps,
recursively. **Rationale:** this was an *omission* discovered in review, and defining it removes
code — all the hand-written predicate functions the earlier code was forced to invent
(`is_plus(t)`, `is_times(t)`) collapse into `ts[p] == .plus`. A rule that deletes code.

### 4.4 Bindings: `=` and `@`

Two symbols, and **no assignment operator exists**.

```
x = 5                 # immutable binding, type inferred
v: i64 @ 0            # mutable declaration — type REQUIRED
v @ v + 1             # mutation — never has a type
vv @ v + 1            # ERROR: `vv` was never declared
```

`=` binds once, forever. `@` declares a cell and re-binds it. One rule — `name SYMBOL value` — where
`=` binds permanently and `@` binds repeatedly.

**Why `@`.** The journey: `::`/`:=` (rejected — two nearly identical symbols, visually confusable at
a dense screen), then `=`+`var` (rejected — `=` doing double duty reopens the typo hole), then `~`
(rejected — reads as "approximately", i.e. *imprecise value*, which is a misread of exactly the kind
that disqualified `^` reading as "raised to"), then `^` (rejected — reads as exponentiation, and
LLMs have seen a great deal of mathematics), then `|` (rejected — already means "or" in variants and
patterns, and would close off bitwise or and pipelines), then `§` (rejected — non-ASCII, see 1.10).

`@` reads as "at", and "at" in computing means *position, address, slot* — which is precisely what a
mutable variable is: a cell that right now holds a value and later holds another. `=` asserts an
identity; `@` indicates what is in a place.

**The visual weight of `@` is a feature, not a cost.** Mutations are the minority of lines and the
hard-to-follow part — they are the only lines that make behaviour order-dependent. A glyph that
jumps out is a marker, not noise. Same logic as `?` marking error propagation: make the non-obvious
flow visible.

**Why the type is mandatory on mutable declaration.** Without it, `x @ 5` doesn't tell you whether
you are *declaring* or *mutating* — you'd have to scan the enclosing block, violating locality. And
this hole lets typos through silently:

```
total: i64 @ 0
for x in xs
    totl @ total + x      # would declare a new variable
```

`totl` is read on that very line, so an "unused variable" rule wouldn't catch it. With the type
mandatory on declaration, `totl @ ...` is an error because `totl` was never declared. The two line
shapes become visually distinct, and **you pay tokens only at the declaration site**, which is
exactly where 1.5 says to pay them.

**All bindings must be initialised.** This is not a style rule — it **deletes an entire analysis
from the compiler.** In languages where `var x: i64` can be declared without a value, the compiler
must chase every control-flow path to verify `x` is assigned before use: that is dataflow analysis
(the same family as move checking). With mandatory initialisation the problem does not exist. One of
the cheapest decisions in the language.

**Unused variable is a compile error.** Ten lines, catches typos, and useful in its own right.
(Go's rule.)

**Shadowing is forbidden.** Redeclaring a name already in scope is an error. Costs nothing, kills a
class of bug where the model believes it is referring to one variable and hits another.

**`@` also marks mutable parameters** — see 4.8. Same symbol, one meaning, three positions.

### 4.5 Type inference

**Signatures are always explicit. Inference applies only to locals.** This is *bidirectional type
checking*, which is the modern consensus over global Hindley–Milner inference, and the reason is
exactly ours: **errors stay local.** With global inference, a wrong type here surfaces as an error
thirty lines away. This was chosen for the right reason before the name was known.

Because there is one integer type, no implicit conversions, and mandatory initialisation, the type
is almost always derivable. **Exactly two cases are not**, and both are "empty container":

```
xs: [i64] = []
m: {str: i64} @ {}
```

Handle this in character with the rest of the language — the annotation is not a feature to learn,
it is something **the compiler asks for by name**:

```
error: cannot infer the type of `result` at line 3
  the map literal is empty, so there is nothing to infer from
  write:  result: {str: i64} @ {}
```

~15 tokens of spec, and the model doesn't have to *decide* anything: either it works, or the
compiler dictates the fix.

### 4.6 Fallibility: `T?`

**There is no `null`.** An `i64` is always a valid integer. Absence is a different type.

An earlier draft had *two* fallible types: `T?` for "normal absence" and `T!` for "something went
wrong", with separate constructors (`empty`, `fail`) and separate match arms. They were collapsed
because they are **the same thing** — a value that might not be there — and the only distinction was
*tone*, which is documentary rather than safety-relevant, since the compiler forces identical
handling either way. This removed ~90 tokens of spec and, more importantly, one *decision* from the
writer: you no longer judge whether your failure is "absence" or "error". Bonus: it freed `!` for
negation.

A `T?` is a built-in variant with two cases, `ok` and `err`:

```
function find(xs: [i64], target: i64) -> i64?
    for i in range(0, xs.len())
        if xs[i] == target
            return i
    return fail("not_found", "no such element")
```

Five operations, each with a distinct job:

| | Meaning |
|---|---|
| `match` | destructure — the only general way |
| `expr?` | propagate to caller (caller's return type must be fallible) |
| `.must()` | extract or abort — for tests and impossible cases |
| `.default(v)` | extract or fall back |
| `.is_err()` | boolean predicate — weakest member, first to cut |

**`.default(v)`, not `.or(v)`.** `|` means "or" everywhere in this language; using "or" for
"fallback value" would give one word two jobs — exactly the incoherence removed by dropping `or` as
an operator. There is a defence (both are "take the first if usable, else the second"), but a pun
costs the reader, and in a two-thousand-token language the reader learns *everything*, so coherence
outweighs resemblance to Python.

**Not an operator (`??`).** In JavaScript `a ?? 0 + 1` parses as `a ?? (0 + 1)` because `??` binds
loosely — easy to get wrong, and the result is a valid, different program. `.default(0)` binds
tightest, so `m[k].default(0) + 1` cannot be misread. Here method syntax beats an operator on
correctness, not taste.

**Errors carry a stable code plus a human message:** `fail(code, msg)`, and `e.code` / `e.msg`.
**Rationale:** tests were asserting on prose (`assert e.msg == "negative stock for A1"`), which
breaks when you reword a message and passes when you break the logic. The code is snake_case and
stable; assert on `e.code`.

**What that costs is measured, not confessed** (panel 034). `heroes mutate`'s `typo-code` operator
slips one character in a code — in the `fail` that builds it or the `e.code ==` that reads it back —
and scores **25 mutants, 0 caught, in both arms**: the only zero in an eleven-row table where
everything else catches 73–100% (`docs/measurements/004-error-codes.md`). The published magnitude is
the same shape: Eghbali & Pradel, *No Strings Attached*, ASE '20 — 42% of 204 real string bugs are
incorrect string literals, a widely used analyser finds 1 of 204, and 11% ever surface as an error
message. **The remedy is `constant`, not a language feature**: write the codes as constants and
compare `e.code == store.ERR_UNKNOWN_ITEM`, and a typo is `error[unknown_name]` today, locally, with
the owning module named — zero spec tokens, zero compiler lines. Typed errors would be better still
and their price is Part 8 wart 5's, not this section's.

**`?` applied to a non-fallible value is a compile error.**

**`ok(x)` is `fail`'s symmetric twin (panel 002, decided 2026-08-03).** Both are checked against
the *expected* type — the bidirectional ⇐ mode that §4.5 already commits the checker to for empty
literals, leading-dot variants, `return` and `???` — so `return ok(v)` where `T?` is expected, and
`fail(code, msg)` likewise. No implicit `T` → `T?` promotion exists, ever (Part 6): besides the
locality argument, it creates the `T??` level ambiguity under monomorphisation with `A := Expr?`,
the exact trap Swift had to patch with SE-0230 five versions in. In synthesis position (`x =
ok(3)` with no expected type) the diagnostic says: annotate. The appendix acceptance program is
written accordingly (eleven sites).

**No exceptions, ever.** This is the most instructive rejected feature, because exceptions are
**unbeatable on tokens**: zero cost at the call site (`save(x)` versus `save(x)?` plus declaring the
error in the signature). But the control flow is *invisible* — looking at `save(x)` you cannot tell
whether it can jump away, and neither can you tell from the signature. Zero locality, and it
produces precisely the class of bug that does *not* raise a compile error: an unhandled error path.
Tokens lose, and must lose. Also, unwinding across the C boundary is a source of undefined behaviour
and would require exception tables.

### 4.7 Control flow

**`match` is the only destructuring construct and the only conditional in the core.**

```
value = match e
    .num n      => n.v
    .variable x => env.lookup(x.name)
    .sum s      => sum_of(s.children, env)
    .product q  => product_of(q.children, env)
```

- **Exhaustive or compile error.** This is the point: a model that forgets a case hits a wall.
- **`_` as a catch-all arm is forbidden on variants.** Rationale: exhaustiveness exists so that
  *adding a variant breaks compilation*. With `_` allowed, the new case falls into the catch-all and
  nothing breaks — exhaustiveness becomes theatre. `_` remains allowed where exhaustiveness is
  impossible (matching on `i64`, on `str`).
- **`_` as a payload *name* is allowed.** `.num _ => []` is fine; `_ => []` is not. This distinction
  is not obvious and must be in the spec.
- **`|` in patterns is allowed:** `.plus | .times | .rparen => fail(...)`. This was deferred, but
  the `_` ban made it necessary — without it, one function had three arms saying the same thing.
  The two rules support each other; neither works alone. Note `|` still means "or", consistently.
- **An arm's body is one statement, inline, or an indented block** (panel 014,
  2026-08-04). `.num n => n.v` and `.ok _ => assert false` are the same shape:
  a single statement, and an expression statement's value is the arm's value.
  Blocks were already expressions ("a block's value is its last expression"),
  so this removes a distinction rather than adding a form — and with it the
  compiler special case that made `heroes fmt` delete an arm's `if` branches.
- **A jump is admissible as an arm body.** `break`, `continue` and `return`
  produce no value; the `match`'s type comes from the arms that can. They must
  **not** be typed `()` — RFC 1216 records that exact wrong turn ("some code in
  the compiler assigns type `()` to diverging expressions because it doesn't
  have a sensible type to assign to them"), and under `()` an `i64`-valued
  `match` with a `return` arm would become a type error. Rust types them `!`,
  Kotlin `Nothing`.
- **A `match` used as a value requires every arm to produce a value *or jump*.**
  As first written (panel 014) this bullet said "every arm", full stop, and it
  contradicted the one above it: `tag = match t` with a `.eof => break` arm is
  exactly the program the bullet above declares legal. Panel 017 resolved it for
  the bullet above — the compiler-engineer built all three typings and measured
  that typing a jump at all rejects every function whose body ends in `return` —
  and the value rule's payoff moves to where the silent program actually was: an
  arm whose body has type `()`, as in `_ => print(0)`, inside a `match` used as
  a value. That case needs no arm-specific rule at all; it is the ordinary
  ⇐-check against the expected type, which is why this rule now costs zero
  attributable lines. A `match` **all** of whose arms jump produces no value and
  is legal only in statement position. Declarations stay out of inline arm
  bodies — a name bound in an arm is a name nothing can read — and that rejection
  lives in the parser, so it is one diagnostic instead of the unused-binding
  message plus a second one.

**`if` takes only a `bool`** — it is sugar for `match` on a two-case variant, which is what `bool`
is. One line of spec covers all its behaviour. `else if` and `else` exist.

```
state = if t.done
    "[x]"
else
    "[ ]"
```

**Two loops, two words** (panel 018 — one keyword with two grammars was the same sin the entity
form fixed, and the old `for cond` sat on no prior while `while` sits on all of them; the `while`
trap left the registry and became a parser-position `Certain` fix on `for` without `in`):

```
while cond
    ...

for x in xs
    ...
```

Ranges use `range(a, b)`, a library function returning `[i64]` — not `a..b` syntax. One fewer
syntactic form. `break` and `continue` exist. No loop labels.

**Blocks are expressions**; a block's value is its last expression. This is one rule seen in two
places (`if` as expression, long `match` arms), not a second form.

**Trailing `if` was removed.** It was liked (`return i if xs[i] == t`) and it genuinely shortened
code, but the author asked for one defined way to do things, and it was a second form of `if`.
Accepted cost: a few functions grow by one line.

**`unless` rejected** — `unless ... else` is notoriously confusing even for humans, and it is a
second negation form when `if !x` exists.

### 4.8 Mutable parameters: `@`

```
function advance(@l: Lex)
    l.pos @ l.pos + 1

function read_number(@l: Lex) -> i64
    v: i64 @ 0
    while !l.at_end() && l.here().is_digit()
        v @ v * 10 + fit_i64(l.here() - '0')
        advance(@l)
    return v
```

Semantics: **copy in, copy out.** The caller copies the value in, the callee mutates its own copy,
and on return the value is copied back. **Aliasing is not reintroduced** — nobody ever sees anyone
else's memory — and the `@` at the call site preserves locality: you can see on the line that this
argument will change. This is Swift's `inout`, which exists for exactly this reason.

**Why this was added.** Value semantics (4.10) forced the parser to invent a `Step` record just to
thread a position through recursive descent — seven lines of struct plus `step.pos` scattered
everywhere, and forgetting to propagate `pos` was a *silent* bug. With `@` parameters, the four
parser functions return `Expr?` directly. This was the biggest cleanup in the review.

**Why `@` and not the keyword `mut`.** The design had two markers for one notion: `@` for local
mutation and `mut` for parameters. Unifying them gives one symbol with **one meaning in three
positions** — in the signature it declares an in-out parameter, at the call site it warns the
argument will change, as an operator it binds a cell. ~30 tokens of spec saved, one concept instead
of two.

**UFCS does not apply when the first parameter is `@`.** `l.advance()` would hide the mutation,
which is the only thing `@` exists to make visible. Always `advance(@l)`, prefix.

**Copy-out happens always**, including on early `return` and on `?` propagation. This was
undefined, and undefined means the model will invent something.

**Two `@` arguments of one call may not share a root binding** (panel 010). `shift(a @ n, b @ n)`,
`shift(a @ p.x, b @ p.x)` and `shift(a @ xs[0], b @ xs[0])` are compile errors, and the diagnostic
ships the `certain` repair: pass a copy for the second argument. The rule is not a new restriction —
it is the precondition that makes §4.10's "no aliasing exists anywhere" *true*, because two
copy-outs landing on one place is aliasing of the destination. Left legal, the call's meaning would
depend on copy-out order, which nothing specifies: the same program would print 16 under a reference
reading, 15 under left-to-right copy-out and 6 under reverse. Precedent is one-sided — Ada defined
copy-back with arbitrary order for thirty-three years and then made the overlapping call illegal in
Ada 2012 ("known to denote the same object"), because order-dependence "is usually a bug, and in any
case, is not portable"; Swift (SE-0176) and Hylo forbid it on the same copy-in/copy-out model, and no
sourced language specifies the order. Since Heroes has no references, every place has exactly one
root, so comparing roots is a *complete* alias test — no dataflow, no borrow checker. It
deliberately over-rejects `f(a @ xs[i], b @ xs[j])` with distinct indices; that form is not on the
closure list and the rule can be relaxed later without invalidating any program. The invariant it
buys the backend is worth stating: **`@` parameters never alias**, so a direct-pointer lowering and
`restrict` stay legal.

**Rejected: sigils on names** (`@total` everywhere, Ruby-style). Every language that uses name
sigils encodes something *permanent* — Ruby's `@x` is scope, Perl's `$x`/`@x`/`%x` is type.
Mutability is precisely the property that *changes* during development, so a name sigil turns a
one-line edit into a twelve-line edit, which is bad for patch-based editing. Also `@total` is
probably 2 tokens where `total` is 1, on every *read*. The deeper point: **a sigil marks the
variable; an operator marks the write** — and it is the write that matters. Reads of a mutable are
harmless; writes are what make behaviour order-dependent. `$` for constants was also rejected:
marking the *default* is anti-economy, and `$` means "variable" in shell/PHP/Perl (misread).
Constants use `SCREAMING_CASE` by convention — free, universal, zero spec.

### 4.9 Records, arrays, maps, calls

```
p = Point(x: 3, y: 4)
print(p.x)
```

**Field names are always mandatory at construction.** `Point(3, 4)` does not exist. Rationale: this
eliminates argument inversion at the point where it is most likely. Note this also means record
construction *is* a call with named arguments — no separate literal syntax, one fewer rule. (This is
why the `Point{...}` brace form was dropped: braces disappeared from the language entirely as
delimiters.)

No methods (UFCS covers it), no inheritance, no private fields, no default values.

```
xs = [3, 1, 4, 1, 5]
m = { "mario": 30, "anna": 25 }
```

- **Indices from 0.** Do not do 1-based indexing: with a C FFI, every boundary becomes a silent
  off-by-one. This is the one Lua inheritance that would genuinely cost.
- **Out-of-bounds index aborts** with a message; it does not read arbitrary memory.
- **Map access returns `V?`**, always. A missing key cannot pass unnoticed.
- `has(m, k)` is **struck** (panel 026, −17 measured): map access returns `V?` always, so
  `has(m, k)` and `!m[k].is_err()` are two spellings of one predicate. The parenthetical that
  used to justify it — "its absence forced a sentinel-value hack" — was retired by the bullet
  above it, which is the shape of a reason outliving the thing it argued against.
- **The order of `keys(m)` is unspecified; a program that needs an order sorts** (panel 026,
  replacing panel 006's insertion-order guarantee of 2026-08-03). The guarantee is not
  weakened, it is **withdrawn as false**: `HeroMapHeader` has no order field, so arrival is
  **not stored** and no iteration strategy can recover it — and `cap` derives from a literal's
  duplicate-inclusive pair count, so two `==`-equal maps written differently iterate
  differently (measured: `{"a":1,"b":2,"c":3}` iterates `b a c` at cap 8, and the same map with
  a duplicated first key iterates `b c a` at cap 16). "Iteration order is a function of the
  contents" was false as implemented from the day the map shipped.
  Panel 006's argument for guaranteeing it was sound and is answered rather than overruled: it
  feared *unspecified* order because a model's silent default is insertion order (the
  Python-3.7/ES2015 prior) and Go's randomization is the recorded disaster. That hazard needs a
  reader who iterates a map **directly**. There is no such form: iteration is `keys(m) -> [K]`
  composed with the `for` and `sort` that already exist (CLAUDE.md §10 — nothing, when two
  invocations compose to it), so the only order a program can rely on is one it sorted. The
  fixed seed stays, because determinism *across runs* is still a fixpoint requirement.
  Port note, and it now cuts the other way: the Rust bootstrap iterates a map in exactly one
  place (`types/holes.rs`) and its own comment says it needs **sorted** order — which insertion
  order would not have given it. Every ordering-sensitive walk is still marked `// ORDER:` at
  the write site, and there are **zero** such marks in the tree today, so nothing exercised the
  guarantee that has just been withdrawn. It becomes an explicit `sort` in the Heroes port, or
  the M-selfhost-fixpoint diff breaks.
- Multi-line literals separate by **newline**, not comma. Single-line literals use commas. The
  canonical formatter picks based on length, so the model never chooses.

**Named arguments become mandatory when two parameters in a signature share a type:**

```
function copy(from: str, to: str) -> bool
function save(id: i64, name: str) -> bool

copy(from: "/tmp/x", to: "/tmp/y")    # mandatory
save(42, "mario")                      # free: i64 and str can't be confused
```

**Rationale, and this is the most original construct in the language.** One of the most classic LLM
errors is swapping two same-typed arguments — an error that produces *no* compile error because the
types match, and a silent disaster at runtime. This rule **spends tokens exactly and only where the
errors occur.** It is not general verbosity, it is targeted verbosity, with zero cost where the type
system already protects. ~50 lines in the type checker.

The criterion is "two parameters with the same type *in the signature*", not "adjacent" — swapping
the first and third is just as plausible. And it applies to *declared* types, not to types after
monomorphisation, otherwise a generic function's call convention would change depending on whether
its type parameters collapse.

No default parameter values, no overloading, no user variadics. The one variadic-looking form is
`print`, and it is **compiler-known, not a function value** (panel 006, decided 2026-08-03): a
comma-separated list of `str`/`i64`/`f64`/`bool` values, each rendered by its canonical `to_str`,
no separator between values, exactly one trailing newline. This was bought by trading away string
interpolation (Part 7 item 7); Pascal's `WriteLn` is the fifty-year precedent.

**Canonical `f64` rendering** (fixed at M-strings-ownership, panel 021). Deterministic, locale-independent, and
**round-trip-exact — never "shortest"**: the algorithm is `snprintf("%.*g")` at increasing
precision until `strtod` reads back the same bits, starting at 1 for a subnormal and at `DBL_DIG`
otherwise, which is what gnulib's `ftoastr` ships. It is not the shortest decimal, and the
distinction is not pedantic: `5e-324` renders `4.94065645841247e-324`, which round-trips, and Java
shipped `1.9999999999999998E23` for eighteen years before Schubfach. A value with no fractional
part still prints a point (`1.0`, not `1`) — under `1`, `print(price * to_f64(count))` and
`print(5 * 2)` emit identical bytes, so an accidental `i64`→`f64` drift would stay green in every
golden, which is exactly what §4.3's no-implicit-conversions rule exists to prevent. Lua made the
same choice when it gained a second numeric type; Rust prints `1` from `Display` and needed `Debug`
to recover the distinction.

**Locale is removed structurally, not promised away.** The render runs inside
`uselocale()` with a lazily-created `newlocale(…, "C", …)`, which covers `snprintf` **and**
`strtod` in one window. "The runtime never calls `setlocale`" was measured worthless: under
`de_DE.UTF-8` the ladder emits the malformed `0,1.0` — the decimal-point rider finds no `.` and
appends one — and the round-trip check cannot see it, because `strtod` reads the same locale and is
wrong consistently. PEP 331 is the shape of the hazard: CPython never called `setlocale`, GTK+ did,
CPython broke anyway. Rust, Go, Python-since-2.4 and C++17's `<charconv>` all removed the
dependency rather than promising.

### 4.10 Value semantics and copy-on-write

**Every value behaves as a copy.** Records, arrays, maps, everything. `b = a` means `b` is
independent of `a`.

The previous design had records copying and arrays/maps sharing — two rules, and the second opened
the door to *aliasing*, where two names refer to one datum and mutating one changes the other at a
distance. For an LLM reading locally that is invisible: `f(xs)` might modify `xs` and you cannot tell
from the line.

Implementation: **copy only on mutation, and only if someone else is looking.** Before mutating an
array, if the refcount is greater than one, copy; otherwise mutate in place. This is copy-on-write,
Swift's choice, and it is a condition to check in the mutation primitives — not a new mechanism,
since refcounting already exists.

**Three gifts follow:**

1. **Aliasing does not exist.** No function can modify the caller's data. Every function is, from
   outside, a box that takes values and returns values. Locality becomes total.
2. **Reference cycles become impossible, so refcounting no longer leaks.** The argument, in its
   published form (Racordon, Shabalin, Zheng, Abrahams, Saeta, *Native Implementation of Mutable
   Value Semantics*, ICOOOLPS '21): **references cannot be stored in variables or object fields,
   hence all values form disjoint topological trees**, whose roots are the program's bindings. Note
   that this is a statement about what a field may *hold* — not, as panel 022's proposal first put
   it, about a copy being finite, which is an argument about copying and does not constrain the heap
   graph. The earlier accepted compromise ("refcounting leaks cycles") **disappears**: memory
   management becomes complete and correct with no cycle collector and no GC. Swift has shipped
   twelve years this way (reference counting applies to *classes*; `weak`/`unowned` exist for them
   alone), and Nim's ORC is gated on `canFormAcycle` with closures as its documented motivating
   case. Two exceptions, named rather than discovered later: §4.19's `ptr`/`cstr` sits outside the
   guarantee, and a *stored* closure would end it silently — which is one more reason CLAUDE.md §5
   forbids one. One departure recorded: the paper forbids recursive type definitions outright, where
   Heroes carries finiteness through `[T]`.
3. **A borrow checker becomes unnecessary.** Not "deferred", not "too expensive": with no aliasing
   there is nothing to check. This is the thesis of the **Hylo/Val** language — memory safety without
   Rust's complexity. Related historical note: this is also the restriction **Cyclone** used
   (references only as function parameters, never in structs or return types) to eliminate lifetime
   annotations.

Price, declared: mutating a shared array copies it, O(n). Performance is not a goal, and in exchange
there is one rule instead of two.

**Why this is not a naive choice.** Value semantics reads as amateurish to anyone whose model of a
"real" language comes from C++, Java or Rust: no references, no graphs, copies everywhere. The
opposite is the case, and it is recorded here rather than left implicit because an implementer who
believes this is a beginner's mistake will keep trying to fix it. The lineage is in the appendix —
APL and its descendants, R and MATLAB, Erlang, Swift, Hylo — and the load-bearing inference from it
is one sentence: **sixty years of production array languages establish that the value model is not
what makes a language slow**, and the one language that industrialised it is slowed instead by the
half of itself that Heroes declines.

**The alternatives, and what each would have cost.** This table exists so that §4.10 is not
relitigated every milestone:

| Alternative | Why not |
|---|---|
| Reference counting with shared references | cycles become possible, so it needs a cycle collector or `weak` — *plus* the aliasing this design exists to remove. The trap Swift fell into via `class` |
| ARC / ORC (Swift, Nim) | as above, plus a compiler pass for ownership optimisation; ORC additionally needs a cycle collector for its `ref` types |
| Tracing garbage collection | a runtime we cannot write, cannot spell out in the spec, and cannot cheaply hand across an FFI boundary. Contradicts §1.11 directly: a GC'd heap is hostile to C interop |
| Borrow checker | already in Part 6 — its whole job is making aliasing safe, and there is no aliasing. The cost avoided is the largest single one in this document: lifetimes in the surface syntax, in the spec, and in every error message |
| Manual allocators (Zig) | correct and honest, but it puts an allocation decision in every signature — spec tokens, and a whole class of plausible LLM error |
| C++ RAII with references | manual lifetime reasoning, and the aliasing question returns to every signature |

**What it buys, plainly.** No garbage collector to write. No cycle collector. No `weak`/`unowned`. No
borrow checker, no lifetimes, no ownership annotations. No aliasing, so no data race becomes
*expressible* once concurrency arrives (Part 7.13). And a counter whose only job is deciding when to
copy, which can stay **non-atomic** for as long as no counted value crosses a thread — a constraint
Part 7.13 records, not a property this section already holds. (The absence of `null` is *not* on this
list: it comes from §4.6 making absence a variant, and crediting it here would inflate the claim.)

**What it costs, equally plainly.** O(n) mutation of genuinely shared data. No linked structures and
no general graphs *in the language* — they go through arena-plus-indices, which is also the
discipline the bootstrap compiler itself is written in (§3.4). No shared-mutable-state patterns:
observers, caches, pools, global registries. No high-performance data structure written in Heroes
itself. The honest summary: **Heroes is strong at transforming data and awkward at interlinking
data.** Performance is an explicit non-goal (Part 2) and the target programs are compilers, parsers
and data transformation, so this is the right side of the trade — but it *is* a trade, and an
implementer should not be surprised by it.

**Consequence for recursive types.** A tree is expressed as:

```
variant Expr
    num
        v: i64
    sum
        children: [Expr]
```

This works because an array is a pointer, so `Expr` has finite size. **No `Box` type is needed, no
pointers in the language: the array is the only indirection**, and it suffices for trees and ASTs.
One case remains awkward — a node with exactly one child needs a one-element array. This is the last
rough edge of the data model and it is left visible rather than patched.

**Known performance consequence to plan for:** `s + t` copies, so building compiler output by
successive concatenation is O(n²). Not a concern in principle, but on a hundred thousand lines of
generated C it becomes real waiting. Provide `join([str]) -> str` or a small `Builder`.

**And the remedy is incomplete as it stands — corrected here rather than left to be discovered
(panel 037).** `join`'s argument is a `[str]`, and `[str]` is built with `push`, which allocates a
fresh array and copies every element: **the array is O(n²) too, so `join` is reached through exactly
the cost it was funded to remove.** Measured 2026-08-12: 50 000 pushes 2.0 s, 100 000 pushes 7.8 s,
1 000 000 pushes 806 s. Two judges found this independently and from opposite directions, and it had
been written down wrong here and in Part 8 wart 8 since the sentence was first drafted.

The answer that works today, measured, needs no language change: **accumulate in chunks.** Keep a
short `[str]` per declaration, push the joined chunk into an outer `[[str]]`, and join once at the
end — 527 000 output lines cost 0.5 s that way against 403 s in one flat array. What does *not* work
is making `push` append in place when the refcount is 1: panel 037 implemented it and the gate never
fires (the ownership pass's own slot makes the count 2 at every accumulator push), and where it does
fire it grows an array nested inside another one behind its holder's back, with every instrument in
this project reporting success. The sound form is a **place store** — `p @ push(p, v)` recognised at
lowering, uniqueness taken from the place rather than guessed from a count — and it waits for
M-selfhost-probe to measure whether anything needs it.

This is the one place where copy-on-write elegance presents a bill.

### 4.11 UFCS

`x.f(y)` is sugar for `f(x, y)`. There are no methods.

```
xs.filter(is_open).map(price_of).fold(0, plus)
```

The resolver, on seeing a dot, first looks for a field; failing that, looks for a free function of
that name and passes the receiver as the first argument. ~30 lines.

**This was cut and then restored**, which is worth recording. It was cut as "the language's only
deliberate duplication" — `f(x)` and `x.f()` do the same thing. It was restored because the
resulting code contained:

```
fold(map(filter(as, is_open), price_of), 0, plus)
```

which reads inside-out, and because constructing correct three-level nesting is *more*
error-prone for a model than chaining. A misplaced `)` is a compile error, but it still costs a
round-trip. The deciding argument: **UFCS has no semantic content.** It is a mechanical rewrite —
one line of spec, thirty of code, pure sugar the core never sees.

**Rejected: pipeline `|>`.** `|>` is a rare sequence so it probably tokenises as 2–3 tokens (the
saving eats itself), and it appears only in F#, Elixir and Elm so the transfer is weak. Dot-chaining
achieves the same effect, tokenises better (`.` is everywhere), and is familiar to everyone.

**Rejected: space application (`f x y`).** This would save 2–3 tokens per call, and calls are the
most frequent construct in a program — the largest remaining saving available. Rejected on locality:
in `f x y` you cannot tell from the line whether `f` is a function of two arguments, or three
variables in sequence, or `f` applied to one composite argument. The meaning depends on `f`'s arity,
which lives **elsewhere**. It would be the only place in the language reintroducing non-locality.
Nesting requires parens anyway (`f (g x) y`), so the saving evaporates in non-trivial cases, and it
breaks chaining. Call parens are the perfect case of punctuation that **earns its cost**: they make
arity locally visible.

**Rejected: omitting `()` on zero-argument calls.** `xs.len` would work fine in v1, since there are
no first-class function values yet, so there is no ambiguity. Rejected anyway because closures are
*deferred, not rejected* — and once functions are values, `xs.len` becomes ambiguous (the call, or
the reference?), leaving three bad options: break all existing code, add a second notation, or give
up closures forever. This is a "decide now" case: **loosening a constraint later is compatible,
tightening is not.** Eight tokens out of five hundred doesn't buy the right to block a future
feature.

### 4.12 Generics

**Functions only, no constraints, always inferred, implemented by monomorphisation.**

```
function map<A, B>(xs: [A], f: (function(A) -> B)) -> [B]
    out: [B] @ []
    for x in xs
        out @ out.push(f(x))
    return out
```

**Rationale, and this is the most counter-intuitive finding of the review: generics make the
compiler *smaller*.** The usual argument against them is implementation cost. Here the opposite
holds, because `[T]`, `{K: V}` and `T?` are *already generic* — the type checker already handles type
constructors and substitution, so the machinery exists. And without user generics, every useful
container function must be **compiler magic**: `map`, `filter`, `fold`, `find`, `sort`, `sum`, `any`
— seven special cases in the type checker, each with hand-written rules. With generics they become
**library code written in the language itself**, testable with `test` blocks like everything else.

Rules that keep the cost low:

- **No constraints.** No `where`, no bounds. If an operation on `T` is needed, pass it as a
  parameter. This is the line that separates cheap generics from expensive ones: simple generics
  (`Stack<T>`, `[T]`, `Option<T>`) are perfectly comprehensible to an LLM; advanced ones (multiple
  constraints, variance, associated types, higher-kinded) are a leading error source *and* 90% of the
  implementation cost. Cut exactly on that line.
- **Always inferred at the use site.** Write `map(nums, plus)`, never `map<i64,i64>(nums, plus)`.
  The token cost of generics is at use sites, so pay nothing there. And because there are no
  constraints, inference is trivial — look at argument types, deduce `T`. **Rust's turbofish
  (`collect::<Vec<i32>>()`) cannot exist in this language**, because there is no syntax to specify
  type arguments manually. That is deliberate: it tokenises terribly, being a very rare sequence.
- **Monomorphisation.** On seeing `map(nums, plus)` with `nums: [i64]`, generate a copy of the
  function with `A := i64`. **This is substitution on the IR, after type checking** — an IR→IR pass
  that runs before the ownership pass and before the emitter, both of which assert that no
  `Ty::Generic` survives. No theory, a few hundred lines.

  *Amended by panel 019 (2026-08-04).* This paragraph used to say "textual substitution on the AST
  before type checking", and M-typed-frontend had already falsified it: the bidirectional checker checks a generic
  body **once, polymorphically**, with `Ty::Generic(i)` in the type table. Substituting first would
  mean checking every instantiation and reporting the same mistake once per instantiation — and it
  has no success story: rustc creates no monomorphised MIR at all (monomorphisation is "the first
  step in the backend"), Swift states there is "no such thing as an instantiation-time error", and
  C++, the language usually described this way, is not textual substitution either — two-phase name
  lookup took MSVC until Visual Studio 2017 15.3 to implement. The order also has a practical edge
  in this compiler: the ownership pass cannot decide whether a `Generic(0)` slot needs a `decref`,
  which is why monomorphisation must precede it and why the invariant is written down rather than
  assumed.
- **Generics on functions only, not on types.** `T?`, `[T]` and `{K:V}` stay built-in.

Historical note: Go shipped without user generics for a decade precisely because the three built-in
containers covered most needs. That covers ~90% of real generic use here too.

### 4.13 Functions as values

There *was* a lie in the pre-018 syntax: `dist2 = function: ...` said "dist2 *is* a function" —
surface says value — while nothing function-shaped could be passed. Panel 018's keyword-first form
retired the lie (`function dist2(…)` claims nothing about value-ness), and the semantic question
underneath it remains this section's, unchanged:

**Functions that capture nothing are values.** A top-level function passed as an argument is just a
pointer — no environment to allocate, no refcounting interaction.

```
function plus(a: i64, b: i64) -> i64
    return a + b

total = values.fold(0, plus)
```

Function type syntax: `(function(A, B) -> C)`, **parens mandatory**, and the marker is the same word
that declares a function — one word, one meaning, everywhere (panel 013, 2026-08-04: `fn` is a
reserved-word error under §4.17, and spelling it inside a type made that error's `certain` fix wrong
in the one position where the word was required). The parameter list mirrors a signature's: two
parameters are `(function(i64, i64) -> bool)`, none is `(function() -> C)`. Honest note on why the
parens: having removed parens
from declarations, `f: fn A -> B -> [B]` would be ambiguous. This is the second price of dropping
parens (the first being the four-line inline `if`), and it is left visible rather than papered over.

**Closures are deferred to v1.5**, and here is a finding worth acting on: **with value semantics,
closures are much easier than usual.** Capture is by copy, so a closure is just a record holding the
captured values plus a function pointer — no shared cells, no lifetime problems, no refcount
interaction. The cost we feared was the cost of closures *with* aliasing. Still ~150 lines and ~60
spec tokens, and the code works without them, so: **first thing after the first running program.**

Note the price being paid meanwhile: every example program needs a handful of named one-line
functions (`double`, `plus`, `is_even`, `price_of`) purely to pass them around. Those same functions
are the ones where the entity form is most disproportionate (see Part 7), which makes this the
strongest single argument for pulling closures forward.

### 4.14 Operators

```
arithmetic     + - * / %
comparison     == != < <= > >=
boolean        && || !
fallible       ?  .must()  .default(v)  .is_err()
```

Precedence, strongest first: call and `.` → unary `-` `!` → `* / %` → `+ -` → comparisons → `&&` →
`||`.

**`&&` and `||`, not `&` and `|` — this is a correction, and it is the most important finding of the
final review.** An earlier draft gave `&`/`|` to booleans on the grounds that "there are no bitwise
operators". But bitwise operators are **exactly** what is needed both for low-level work and for the
web: binary format parsing, network protocols, masks, flags, UTF-8 encoding, hashing. If `&` and `|`
are occupied by booleans that territory is closed forever, or you pay an awkward notation
(`x.bit_and(0xFF)`) precisely in the code that uses it most.

So: **`&&` `||` `!` for booleans; `&` `|` `^` `<<` `>>` `~` for bits.** This is the
C/Go/Rust/Java/JS convention — "the usual characters", done precisely. Costs one character per
operator and closes nothing.

**Reserved-but-unimplemented until 2026-08-12, when they became real** (author decision in
`/decide`; the record is `docs/panel/040`, a retro-record sitting). The deferral was not wrong when
it was made and it stopped being right the day the FFI arrived: with no `|`, a flag union had to be
written `FLAG_A + FLAG_B`, which is correct only while the bits are disjoint and **silently wrong**
otherwise — measured, `O_RDWR|O_ACCMODE` is 3 where `+` gives 5, and `S_IRWXU|S_IRUSR` is 448 where
`+` gives 704. Panel 039 found it from two directions in one sitting: the llm-ergonomist wrote the
`+` form from the spec alone and flagged its own premise, and the ffi-pragmatist compiled the
overlapping case. Four decisions came with the implementation, each of which is a rule rather than a
detail:

- **`i64` only.** A float's bits are not its value, and `bool` is excluded for this section's own
  reason — one of `&&`/`&` short-circuits and the other does not, so admitting both on `bool` would
  invite the reader to believe they are one operator.
- **`&` binds tighter than `==`**, which is where C's table is on record as wrong (`x & 1 == 0`
  means `x & (1 == 0)` in C). Each of the six gets its own precedence level, so
  `a | b ^ c & d` groups as it does in every language that copied C without copying that.
- **`1 << 63` is `INT64_MIN`, not an abort.** The shift is done on the unsigned bit pattern and cast
  back — defined for every input, where C leaves a signed left shift into the sign bit undefined.
  Without the sign bit reachable, a mask set cannot name its own top flag, which is the whole use.
- **A shift count outside `0..63` aborts**, because C6.5.7p3 makes it undefined and CLAUDE.md §7
  admits no C UB. `>>` is arithmetic, written explicitly rather than relying on C's
  implementation-defined choice.

`|` now carries two meanings — the match-pattern join (§4.7) and bitwise or — disambiguated by
position, exactly as `-` is unary minus and subtraction. That dual life cost one silent defect on
the day it arrived: a literal pattern was parsed with the full expression parser, so
`1 | 2 => "small"` became the expression `3` and matched nothing. The premise that made the old code
safe — *no binary operator can follow a literal in pattern position* — had never been written down,
which is §4.15's rule about premises paying for itself again.

`&&` and `||` short-circuit and accept only `bool`. There is **no truthiness**, so `&&` on a
non-bool, or `!` on a non-bool, is a compile error. No operator overloading. No ternary operator
(`if` is an expression).

**A non-`()` expression in statement position is a compile error (panel 003, decided
2026-08-03).** `xs.push(4)` as a statement compiles and silently does nothing useful — `push` is
*pure* under value semantics (§4.10), so the result vanishes: a plausible silent error in exactly
the class this language exists to kill, and the highest-frequency LLM habit error under value
semantics. The diagnostic dictates the fix (`_ = expr` to discard intentionally, or use the
value) and the fix is `certain`, i.e. machine-applicable (§4.17). Zero keywords; subsumes Nim's
`discard`; `_ = e` lowers to C's free `(void)e;`. The check is a type judgment — statement
position expects `()` in ⇐ mode, sharing panel 002's machinery. `extern` calls get **no
relaxation**: an ignored C return code is C's own classic silent bug (§1.11); if real bindings
ever push the `_ =` rate past ~25% of call statements, the valve is a per-`extern` annotation in
§4.19's vocabulary, never a blanket exemption.

**Arithmetic edge cases, all of which must be specified because unspecified means the model
invents:**

- **Integer overflow aborts.** It does not wrap. Loud failure.
- **Division by zero aborts.** Integer division truncates.
- **Out-of-bounds index aborts.**

### 4.15 Layout, formatting, lexing

- **Significant indentation, no braces.** The author's aesthetic preference, and it is defensible:
  the tokeniser compresses leading runs of spaces, so the net difference versus braces is about one
  token per block — roughly 3% of a file.
- **Rigid indentation, which is the mitigation that makes this safe:**
  - **Spaces only. A tab is a compile error**, not an interpretation.
  - **Exactly 4 spaces per level.** Five spaces is not "slightly crooked", it is an error.
  - A dedent must correspond exactly to one open level. No creative alignment.
  This turns the whole family of "nearly right" indentation errors into compilation failures.
- **The residual risk, stated honestly:** this case is unrecoverable —
  ```
  if x > 0
      a()
  b()          # meant to be inside the if
  ```
  Valid, different, no error. With braces this cannot happen, because indentation is then merely
  *decoration* and the canonical formatter can repair it, whereas here the structural information is
  gone. This is the one accepted silent-error case in the language. It is bounded, and it is
  mitigated by the fact that LLMs are extremely well trained on Python's indentation, and that for a
  new language the model will rewrite whole functions rather than patching three lines.
- **No semicolons.** The lexer inserts a terminator when a line ends with an identifier, a literal
  (including `true`/`false`, and string/char literals when they land), `return`, `break`,
  `continue`, `???`, postfix `?`, `)`, `]`, or `}` — Go's rule, faithfully this time: Go's own list
  carries `break`/`continue` and the postfix operators, and the original transcription dropped
  them. `fallthrough` is omitted deliberately (no such keyword). One departure from Go on record:
  its `++`/`--` are statements while Heroes' `?` is an expression — safe only because §4.14's
  statement-position rule makes a stranded continuation a loud error. ~20 lines. (Panel 007.)
- **Continuation lines: inside brackets only.** Within `(` `[` `{`, leading whitespace is not
  structural — no indent/dedent tokens are emitted, so multi-line calls, signatures and literals
  indent freely (Python's discipline; §4.9's newline-separated literals rely on terminators, which
  are inserted unchanged everywhere, brackets included). At bracket depth zero every line's
  indentation is structural: a long expression is broken inside parentheses or not at all. An
  unclosed opener is a compile error reported at end of file, citing the opener — without that
  diagnostic one missing `)` would silently swallow the rest of the file's layout.
  Trailing-operator continuation at depth zero (Nim's rule) was considered and deferred: it enters
  only if the measurement baseline shows models actually produce that break shape. (Panel 007,
  predictions on record.)
- **No parens around conditions.** `if x > 3`, not `if (x > 3)`. Zero information, two tokens saved,
  and Go/Rust/Swift already do it, so no familiarity cost.
- **Canonical formatter, mandatory, `gofmt`-style: there is exactly one correct way to write any
  program.** Two purposes: it removes all style decisions from the model (pure spend, no meaning),
  and it makes any textual difference between two versions *semantic*. Implementation: this is the
  pretty printer, which is needed anyway for error messages and `???` output. Write it once, use it
  for both.

### 4.16 Typed holes: `???`

`???` is a valid expression anywhere. It is **not an error**: the compiler tells you what belongs
there.

```
function centroid(ps: [Point]) -> Point
    sx: i64 @ 0
    for p in ps
        ???
    return Point(x: sx / ps.len(), y: ???)
```

```
hole at line 4
  position: statement
  in scope:
    ps: [Point]
    p:  Point
    sx: i64 (mutable)
  fields of Point: x: i64, y: i64
  nearby functions:
    dist2(a: Point, b: Point) -> i64
      "Squared distance. Avoids sqrt to preserve precision."

hole at line 6
  expected type: i64
  in scope: ps: [Point], sx: i64 (mutable)
```

A program containing holes does not produce a binary, but **it type-checks everything else.**

**Why this matters:** it lets a model work skeleton-first. Structure, then pieces, with the compiler
stating at each step exactly what is needed and what is available. Instead of writing 40 lines and
hoping, it writes 10 and receives guidance. The error rate drops because the model doesn't have to
*guess* types — you hand them over.

**Why it is cheap:** `???` is one extra AST node in the parser (ten lines), and when the type checker
reaches it, instead of computing a type it **prints what it already knows** — the expected type from
context and the symbol table, both of which the type checker holds anyway for its ordinary work.
There is no new mechanism; you are *exposing* information the compiler possesses and normally throws
away.

Two rules discovered by adversarial review (panel 000), both now normative:

- **A file containing `???` suppresses unused-binding errors.** Without this, §4.16's own example
  fails to compile: `p` is bound by `for p in ps` and used only *inside* the hole, which §4.4 would
  reject. The suppression is file-wide and lifts the moment the last hole is filled.
- **Hole suggestions are ranked by type-relevance, capped at 5, in deterministic order — and the
  cap is stated in the spec.** Unbounded "nearby functions" would flood a model's context on a
  5k-line compiler and vary with file layout, making identical holes give different guidance.

Prior art: proof assistants (Idris, Agda) have holes, but for mathematicians. Nobody has them for
LLM code generation.

### 4.17 Errors written to be read by a model

A normal compile error is written for a human with the project open:

```
error: type mismatch at line 12
```

An LLM does not have the project open. It receives that line and must **re-read files** to
understand. Three conversational turns for a comma.

**Every error carries all the context needed to fix it:**

```
error: incompatible types
  at line 12:  save(user.id, user.name)

  save expects:  (id: i64, name: str)
  you passed:    (id: str, name: str)
                  ^^^^^^^^

  user.id has type str
  defined at line 4:
      record User
          id: str
          name: str

  possible fixes:
    - convert:  to_i64(user.id)
    - change save's signature to accept a str id
```

The signature, the record definition, where it lives, and two concrete routes. The model fixes it
**in one turn**, without opening anything.

**Fixes are tagged `certain | guess`, and only `certain` fixes are machine-applicable.** The
example above shows why: "convert: `to_i64(user.id)`" may be right, but "change save's signature to
accept a str id" is usually the *wrong* repair — and a model will apply whatever the compiler
blesses. A `certain` fix (insert the missing type annotation the checker just computed, rename to
the one in-scope candidate) may be applied mechanically; a `guess` is prose for the reader. The
golden convention enforces honesty: `x.hero` + `x.expected` (+ `x.fixed` where a certain fix
exists, and CI asserts that applying it makes the program compile).

**Implementation cost: zero theory.** The type checker already knows all of this at the moment it
detects the error — which function, which signature, which record, which line. Today it throws that
away and prints one line. Instead, print it. An afternoon of formatting work.

This is also the most measurable thing in the project: count the number of exchanges needed to make
a broken program compile, before and after. Rust and Elm proved good errors transform the *human*
experience; nobody has designed them for the consumer that today reads more compiler errors than
anyone.

**Specific errors to include:**

- Reserved-word errors that dictate the fix. The likeliest mistake from a model writing on autopilot
  is `struct`/`enum`/`fn`/`let`/`var`/`function` used wrongly:
  ```
  error: `struct` is not a word in this language
    use `record`:  record Point
  ```
  Loud failure with the solution pre-written. Five lines of code each.
- `@name` in prefix position is a **syntax error, always** — so a model carrying a Python/Ruby prior
  fails loudly instead of producing something valid.
- Type-inference failures on empty containers dictate the annotation (4.5).
- Non-exhaustive `match` lists the missing cases by name.
- Same-typed argument rule violations show the required labels.

### 4.18 Tests in the source

`test` is a keyword. `heroes test file.hero` compiles and runs all `test` blocks; ordinary compilation
ignores them entirely.

```
function dist2(a: Point, b: Point) -> i64
    dx = a.x - b.x
    dy = a.y - b.y
    return dx*dx + dy*dy

test "3-4-5 triangle"
    assert dist2(Point(x: 0, y: 0), Point(x: 3, y: 4)) == 25
```

**Why this is strong for the thesis:** there is currently no way to know whether an LLM *understood*
or merely produced something that compiles. With tests in the source you ask for function plus test
together and get an executable check in one shot. The cycle becomes: model writes, compiler checks
types, tests check meaning. And the tests sit *adjacent* to the function, so the model always sees
them together — maximum locality.

Cheap to implement: collect `test` blocks during parsing, generate a `main` that calls them in
sequence, print which pass. No new language concept. (Zig's design.)

**`assert` shows the source expression**, because the compiler has the source text:

```
assertion failed at line 12
  expression:  dist2(a, b) == 25
  left:        24
  right:       25
```

One extra string on the AST node, and it gives error messages that look like a mature language.

`assert` at the top of a function also reads as a precondition — leave that door open for showing
contracts in `???` output later, but don't implement it now.

**Doctests (runnable examples in markdown fences inside doc comments) are deferred to v2.** They are
a good feature — documentation that cannot lie, because it stops compiling — with precedent in Rust,
Python and Elixir. Deferred because they would be a *second way to write a test*, which the form
budget forbids until the first mechanism is proven.

### 4.19 FFI

**Read 1.11 first — this is the section that carries the project's founding constraint, so treat its
ergonomics as a priority rather than an afterthought.**

The backend emits C (§3.1), so an `extern` declaration is Nim's `importc` design — the signature
plus the header it comes from, no external tool, no libclang, no generated binding files:

```
extern "sqlite3.h" link "sqlite3"
    function sqlite3_open(path: cstr, out: ptr) -> i64
    function sqlite3_close(db: ptr) -> i64
```

**The header and the link flag attach to a group** (panel 036): a head line, then indented
signatures — the shape `record` and `variant` already have, and the shape Rust's `#[link]` on an
`extern` block, cgo's preamble, Odin's `foreign` block and D's `pragma(lib)` all converge on. The
parser **flattens** it: one declaration per member, two spans on each, and no
later pass ever learns the word "group", because a declaration's index is function identity across
five modules. `ptr` is an opaque pointer whose only literal is `nullptr` — `null` belongs to the
foreign-word registry and stays there — and a C out-parameter is an `@` parameter, which §4.8
already compiles to a pointer. Macros and `inline` functions are reachable
because the C compiler sees the real header.

**A `constant` is the group's second kind of member, and its value is the header's**
(panel 038). It carries no body — inside a group a declaration is a *signature, not a
definition*, so `function` gives up its code and `constant` gives up its value:

```
extern "curl/curl.h" link "curl"
    constant CURLOPT_URL: i64
    function curl_easy_setopt(handle: ptr, option: i64, value: cstr) -> i64
```

Until M-header-constants the sentence above said `#define` constants were *reachable*, which
was true of the C compiler and false of the author: nothing in the surface could name
one, so `examples/curl/main.hero` wrote `10002` with the `CURLOPTTYPE_STRINGPOINT + 2`
derivation in a comment and nothing checking either. `heroes mutate`'s twelfth operator
scored that at **five sites in `examples/`, none caught by anything** — the same shape as
panel 036 rider 3, one milestone later, in this section.

The mechanism is the assertion §4.19 already emits, asked of a token instead of a call,
plus one more that only a value needs:

- `_Static_assert(HERO_RET_INT(CURLOPT_URL), …)` — the **same** `_Generic` macro set,
  unchanged; 17 of 18 real header constants pass it as written. Declaring `M_PI` as `i64`
  would otherwise convert `double` to `int64_t` on the way out of the accessor and make
  the constant **3**, silently, with every flag in CLAUDE.md §7 satisfied.
- `_Static_assert(__builtin_constant_p(CURLOPT_URL), …)` — the header must hold a
  **value**, not an object. `stdout` is `#define stdout __stdoutp` over an `extern FILE *`
  and `errno` is `(*__error())`; a zero-argument accessor over either returns whatever it
  holds *at the time of the call*, which is the mutable global §4.2 forbids arriving
  through the back door. Nim, Swift and Go all accept naming a C object; Zig refuses it by
  accident, as a comptime failure its own tracker calls a bug. Refusing it as a *rule*
  appears to be new (panel 038's historian, sourced). The builtin is itself a constant
  expression even when its argument is not, which is what keeps the failure inside an
  assertion carrying *our* message.
- **The accessor is mangled**, and this is the one exception to CLAUDE.md §7's rule that
  `extern` names pass through unmangled. Unmangled, `int64_t SQLITE_OK(void) { return
  SQLITE_OK; }` has the macro eat its own definition. A linker name must survive; a
  preprocessor name must not appear.

Two of the seven boundary types are refused in the checker, and both reasons are facts
about Heroes rather than about any header: a `str` carries the runtime's own magic word
(declare it `cstr` and convert with `to_str`, §4.20), and `()` is a type rather than a
value. `bool` is **not** refused there — under `-std=c11` `stdbool.h` spells `true` as
`#define true 1`, so its type is `i64`, but that is a premise about the world and the
per-constant assertion asks the token instead (CLAUDE.md §11).

What this does not buy: a wrong *number* is now impossible rather than unnoticed, and a
wrong **name** is still the dominant remaining FFI mistake once a binding names a hundred
constants — which is why a name no header declares became its own diagnostic
(`ffi_unknown_name`) in the same milestone, carrying clang's own typo correction as a
`guess`.

**The emitter emits `<header.h>` and never `"header.h"`, and passes `-I<directory of the .hero
source>` after the runtime's.** Measured, and it is not a preference: with a decoy `sqlite3.h`
beside the generated unit in `build/<hash>/`, the quoted form read the decoy **with zero
diagnostics under `-Weverything`** — panel 020's decoy-`runtime/` finding relocated to a header,
where the `_Static_assert(HERO_RUNTIME_ABI)` that rescued it there cannot be written. The quoted
form also fails the case it was chosen for, an author's own `mylib.h`, because the translation unit
lives in `build/` and `""` searches the *including file's* directory.

**Every `extern` carries a `_Static_assert` over a `_Generic` on its return type**, and without it
§4.19's central promise is false. The emitter does **not** re-declare the signature — re-declaring
gives `conflicting types` five times out of five on SQLite, since Heroes' `i64` is `int64_t` and
every entry point returns C `i64` — so it emits the `#include` and the calls, which is what Nim's
`importc` does. In that mode clang checks the *call* and nothing else, and four wrong bindings out
of six compile clean: `extern function sqrt(x: f64) -> i64` exits 0 and prints `1`. The assertion's
controlling expression is a call that C11 6.5.1.1p3 does **not evaluate** but does type-check, so it
costs one line per `extern` in the generated C and nothing at runtime; eleven of eleven correct
ladder bindings pass, and `size_t` — which has no signed C spelling — turns from silent into a
compile error. **Clang verifies the declared signature against the real header** is therefore a
true sentence, and this is the mechanism that makes it one.

**Half of that paragraph was booking a hole as an achievement, and panel 041
measured the other half.** A `size_t` return is a compile error and there is **no
declaration that is not one**: `extern function strlen(s: cstr) -> i64` raises
`ffi_return_type` whose note says *"correct the result type"*, and no result type
exists to correct it to — so the most basic function in C cannot be bound in this
language by any spelling. What makes the paragraph above true is that the wrong
binding is caught; what it does not say is that the right one is unwritable. The
gap is **type-checking only** and not the ABI: `int64_t`↔`size_t` round-trips
losslessly at all four edges on arm64 and x86-64, compiled both directions
(panel 041, `e4-abi.c`). The four-rung ladder never met this because it binds only
`i64`, `cstr` and `ptr` returns — it passed by choosing the functions whose
signatures have no unsigned in them, which is a fact about the corpus and not
about the mechanism.

**The remaining accepted loss:** no automatic binding *generation* — declarations are still written
by hand, they are merely verified. C++ libraries are still reachable only through a shim.

**For C++ and awkward struct-passing, write a thin C shim** (how it is compiled is **undecided** —
panel 036 deferred both candidates, `heroes cc` and a `compile "shim.c"` clause, on the grounds that
the acceptance test needs no shim and neither candidate has a test until one does):
a `.c` file that exposes plain functions taking scalars and opaque pointers.

```c
/* sqlite_shim.c */
#include <sqlite3.h>
void *hero_sqlite_open(const char *path) {
    sqlite3 *db = NULL;
    if (sqlite3_open(path, &db) != SQLITE_OK) return NULL;
    return db;
}
i64 hero_sqlite_exec(void *db, const char *sql) {
    return sqlite3_exec(db, sql, NULL, NULL, NULL);
}
```

This solves struct-passing-by-value, out-parameters, and callbacks in one place — and it is the
only realistic route to C++ libraries, since `extern "C"` wrappers are the same pattern. Budget for
shims as normal work, not as a workaround — but note they are now for the hard cases only, not for
every macro.

**A binding annotation vocabulary will eventually be needed**, because ownership has to cross the
boundary somehow. Three cases to cover: a pointer you *must* free (and with which function), a
borrowed pointer you must not touch, and a buffer that C takes ownership of. Do **not** use `@` for
these annotations (an earlier draft suggested `@owned`, before `@` was assigned to mutability).
Reserve a keyword. Until this exists, treat every `ptr` as opaque and free it explicitly through a
shim function.

**FFI callbacks are `ptr` until a C-width type vocabulary exists** (panel 013, verified by
compiling against the real headers). §4.13's function type is a Heroes-internal feature and must
never be presented as the FFI callback spelling: Heroes' `i64` is `int64_t` while every real
callback takes C `i64` and `const` pointers, so a *typed* callback is a hard clang error —
`qsort`, `sqlite3_exec`, `sqlite3_busy_handler` and all six raylib callback typedefs fail, 9 out of
9. `cb: ptr` compiles, links and runs (SQLite needs no shim). What the boundary lacks is `c_int`
and `const`, not a marker — see Part 7 item 10. Two things stay forbidden meanwhile: the emitter
must never insert a cast from `ptr` to a header's function-pointer type (it compiles clean and
silently calls the wrong callback — the exact silent-wrong-answer class this language exists to
kill), and `-pedantic-errors` waits until callbacks have a typed route.

**Provenance of the mechanism:** Nim's `importc` pragma (`{.importc, header: "sqlite3.h".}`) — the
cheapest FFI mechanism that exists, and it works *only* because the backend is C. An earlier
revision of this document called it "the design to steal from if the backend ever changes to C
emission" and "the single strongest argument for the C backend"; the backend changed (panel 001),
and it was stolen.

**Acceptance tests for the FFI, in order of increasing ambition.** Each one is a real milestone, and
the first is the one that proves the project's premise:

1. **`printf` and `puts`** — proves the ABI works at all, including variadics on Apple ARM64.
2. **`libm`** — `sqrt`, `sin`, `pow`. Proves `f64` passing in floating-point registers.
3. **SQLite** — open a database, run a query, read a result, close. Pure C, clean header, immediate
   value. **If this works without you having written a standard library, the architecture holds.**
4. **libcurl** — fetch a URL (author instruction, 2026-08-12). It is the rung that tests what the
   others do not: `curl_easy_setopt` is **variadic**, which is where Apple ARM64 passes arguments on
   the stack rather than in registers, and `CURLOPT_WRITEFUNCTION` is a callback, which §4.19 fixes
   at `ptr` until a C-width type vocabulary exists.
5. **raylib** — opens a window and draws. Proves the toolchain handles a library with real struct
   passing and framework linking on macOS, and it is disproportionately motivating to see. Measured
   before it was needed: the homebrew **dylib** links with `-I`/`-L` and **no framework at all**;
   the static archive needs five. So what the group head lacks is **search paths**, not a framework
   keyword — and both are paths, validated not to begin with `-`, so the form stays closed rather
   than becoming an arbitrary-flag hole.


### 4.20 The runtime, in C

A few hundred lines, written once. It ships a header, **`heroes_runtime.h`, which generated C
includes — so clang type-checks every runtime call.** Contents:

- the allocator (a wrapper over `malloc`) — and **a single point**, which is `runtime/parts/alloc.c`
  and has been since M-ffi-ladder. It was not before: the sentence below claimed one `malloc` and one
  `free` inside the `str` primitives while there were **eleven calls across five files**, and the
  claim went on reading as true because it was the design rather than the code. Two pairs, not one,
  and the split is the leak gate's: `hero_alloc_block`/`hero_release_block` count what the language
  owns, `hero_alloc`/`hero_release` do not, because a scratch buffer freed inside one runtime call is
  not a leak and counting it invites a report of one that is not. The old wording follows, since the
  reason it gave is still the reason: which today is literally one
  `malloc` and one `free`, both inside the `str` primitives. Not tidiness: Part 7.13's per-thread
  heaps need one place to change, and a second allocation site discovered later is a redesign.
- `incref` / `decref` — **behind a narrow, never-inlined boundary**, i.e. calls into this separately
  compiled unit, for the same forward reason: making the counter thread-local or atomic must be one
  edit, and inlined refcount arithmetic is not one edit.
- the `str` **value**, `{ptr, len}`, passed **by value** (16 bytes, two registers), with the
  refcount and an 8-byte magic word in a heap header immediately before the bytes — the earlier
  wording "(`ptr`, `len`, `refcount`)" cannot be read literally, because a by-value copy with an
  inline refcount diverges. **Always NUL-terminated** — allocate `len+1` so `.cstr()` is free with
  zero copies. (This is Zig's `[:0]u8` trick and it is the single highest-return decision in the
  string design.) By value **because of the FFI** (panel 021, measured): written as a pointer, the
  wrong `str`→`cstr` conversion compiles clean *with an explicit cast* and passes a refcount word
  to `sqlite3_open`; written by value it is `error: operand of type 'HeroStr' where arithmetic or
  pointer type is required` — inexpressible, which is what §4.19 promises. The magic word catches a
  `HeroStr` fabricated from a foreign pointer (`{sqlite3_column_text(…), n}`), which otherwise
  compiles with **zero warnings under `-Weverything`** and corrupts the bound library's heap with
  ASan silent.
- `hero_str_from_bytes` / `hero_str_from_cstr`, which make an owning copy of a borrowed C
  pointer. Without them **no `extern function` may return `str`** and §4.19's ladder is unwritable
  at step 3 — "read a result" — because every C library returns strings as borrowed pointers.
- `hero_runtime_live` / `hero_runtime_check_leaks`: a live-block counter asserted at exit. It is
  the leak gate because AddressSanitizer is **not** one here — `detect_leaks is not supported on
  this platform` on Darwin arm64, measured, with a 999-block leak exiting 0 in silence. ASan and
  UBSan stay in the harness for use-after-free and double-free, which they do catch.
- the array: a heap header (`refcount`, `len`, `cap`, element descriptor) with elements in-line,
  `push`, and an aborting bounds check — representation fixed by the hand-written, ASan-verified
  spike `tools/spike/04-variant.c` before any compiler code existed. **A record is a C struct by
  value and a variant a tagged union by value** (panel 022, confirming that spike rather than
  overturning it: its `h_Expr` was already a tagged union, and the only refcounted header in it was
  the array's). So `str`, `[T]` and `{K: V}` are the only reference-counted things in the language,
  and a recursive type is finite because its recursion passes through `[T]` — measured at 16 bytes
  and two heap blocks for the appendix's five-node tree, against 8+24-per-node and seven blocks
  boxed. `HeroDesc.copy` is **shallow plus incref**: COW is what makes a deep copy unnecessary, and
  the spike's own expected output changed with this decision.
- **A `T?` is a by-value struct, 40 bytes while `T` fits in 32**, because a `Failure` is two `str`s
  and the union takes the larger side — so `m[k]` returns through memory rather than in registers
  (measured on AAPCS64). Past 32 bytes `T` wins: `sizeof(Big?)` is 48 for a 40-byte record, which
  panel 023's ffi-pragmatist measured while compiling something else, correcting "40 bytes for every
  `T`" — the shape of claim that is easy to write and false at one boundary. Being by value also
  makes a `T?` **transparent** to §3.1's containment edge: `record R { next: R? }` has no size.
  Stated here because it was inherited rather than decided for two milestones. Rust polices the same
  largest-case tax with two lints, `result_large_err`'s default threshold being 128 bytes.
- the map (hash table; **fixed seed** — iteration determinism is a fixpoint requirement, panel 006)
- copy-on-write checks in the mutation primitives
- `panic` with a message; `hero_unreachable` for type-system-proven-unreachable points
- `join` / `Builder` for string building
- `print`'s monomorphic entry points (`hero_print_int`, …) — the surface contract is panel 006

**Per-type functions are generated by the compiler, not written in the runtime.** Four rules,
each of which panel 022 found by compiling rather than by reading: `eq` and `hash` walk **fields**,
never bytes (`record Flag { n: i64, on: bool }` carries 7 padding bytes, so two `==`-equal records
hash differently under `memcmp` with no warning and no sanitiser report); `hash` is generated for
**every** type and is never null (a call through a null one is `SEGV on unknown address 0x0, pc 0x0`
— no type name, no source line); a **payload-free variant case is omitted from the union**, because
C11 has no empty struct; and the set of types to generate for is a **reachability worklist**, not a
walk over the interned arena, because an unused descriptor is `-Wunused-const-variable` under §3.1's
own flag set. C has no copy
constructors, destructors, or generic comparison, but §4.3 demands structural `==` recursively and
§4.10 demands value-semantics copies and drops. So the **type-descriptor pass** (Part 5) generates
`h_T_copy` / `h_T_drop` / `h_T_eq` / `h_T_hash` for every reachable type, as plain
C the runtime calls through per-type descriptors. The alternative — a type-erased `void*` runtime —
would void the "clang type-checks every call" property, and is rejected.

`hash` is generated for **every** type and is never null, including types no map is keyed by:
Go's cheaper rule (emit it only for map-key types) reintroduces a null function pointer, and a call
through one is `SEGV on unknown address 0x0, pc 0x0` with no type name and no source line (panel 022).

**The descriptor has five members and gains no sixth** (panel 027). The proposal that would have —
a `cmp`, so `sort` could order every type — was vetoed on evidence rather than taste: C11 6.7.9p21
zero-fills a short initialiser list, so every existing descriptor would silently carry `cmp = NULL`
under this project's own flag set, and the call is the same `SEGV` the paragraph above exists to
prevent. `-Wextra` sees it and `FLAGS` does not; `-Werror=missing-field-initializers` is blind to the
designated form; and `_Static_assert(HERO_RUNTIME_ABI == N)` does **not** catch it, because a stale
`.o` against a new header links at exit 0. Adding a *function* to this ABI is self-guarding — an
undefined symbol at link — while adding a *struct field* is not, and that asymmetry is the rule for
every future addition here. An operation on `T` that the descriptor does not carry is passed as a
parameter (§4.12), which is also what the bootstrap's own four sorts need.

Compile to a `.o` once, cache it, and always link it.

The consolidated built-in inventory (Tier 1 in C, Tier 2 in Heroes) is Principle 0's library
closure list (§1.0): `print`, `len`, `push`, `slice`, `chars`, `keys`, `sort`, `join`/`Builder`,
`to_i64`/`to_f64`/`to_str`, `panic`, plus `map`/`filter`/`fold`/`find`/`any`/`all`/`range` in
Heroes.

---

## Part 5 — Core versus sugar

**This distinction determines the size of your compiler. Respect it.**

### The core: seven constructs

1. Bind a name (immutable, or a mutable cell)
2. Define a function; call a function
3. Construct a product (record); read a field
4. Construct a sum (variant); destructure with `match`
5. Loop; `break`; `continue`
6. `return`
7. Primitive operations

Only these need implementing in the type checker *and* the lowering *and* the backend.

Two passes sit between type checking and emission, and they are **core obligations, not details**
(panel 000, compiler-engineering review — "where this project would actually stall"):

- **The type-descriptor pass** generates `copy`/`drop`/`eq`/`hash` per reachable type (§4.20) —
  C has none of them and §4.3/§4.10 require all of them.
- **The ownership pass** is the first **IR→IR** pass (amended at M-strings-ownership, panel 021: "during lowering"
  was the original wording, and a separate pass is what makes `--dump-ir` show the result and keeps
  the emitter a printer). It inserts `incref`/`decref` as *real instructions* — Swift's SIL is the
  precedent, and LLVM D92808 records what happens when refcount pairing lives only in the backend:
  passes separate the calls from their markers. `cow_check` joins them at **M-value-aggregates**, where `push`
  gives it a call site; at M-strings-ownership it would have none, and an instruction nothing emits is an arm that
  rots. **Five rules, each with a compiled counterexample** (panel 021 R2): a plain parameter is
  *borrowed* and excluded from the sweep, which covers local and synthetic slots only; an `@`
  parameter is *moved in and moved out*, its copy-out replacing the decref; a store increfs the new
  value before decrefing the old; a returned value is increfed before the sweep; and an owning
  temporary — one whose defining op allocates — is decrefed at the end of its defining block, made
  safe by a checked invariant rather than by a liveness pass. Refcounted slots are
  **zero-initialised** in the prologue so cleanup is unconditional (clang's ARC specification
  licenses exactly this for `__strong` locals; rustc's `ElaborateDrops` does the alternative and
  then optimises it into the same thing), with `ptr == NULL` as the one non-value that every
  runtime entry point rejects. The pass builds a cleanup-label chain per function so every exit
  edge (`return`, `?`,
  `break`, `continue`, `panic`, match fallthrough) releases live locals and performs `@` copy-out
  (§4.8: "copy-out happens always"). The runtime cannot know where a scope ends; only lowering can.

### The sugar: erased on the way into the IR

| Sugar | Reduces to |
|---|---|
| `if` / `else if` / `else` | a two-way branch, and a join slot when it has a value |
| `for x in xs` | `while cond` with an index |
| `?` | a branch plus an early `return` |
| `.must()`, `.default()`, `.is_err()` | a branch on the `ok`/`err` tag |
| `T?` | a built-in variant with `ok` / `err` |
| `.variant` (leading dot) | fully-qualified variant, type from context |
| UFCS `x.f(y)` | `f(x, y)` |
| `test` | a zero-argument function, entered only by `heroes test` |
| `assert` | a branch plus `panic`, carrying the source text and both sides |
| named arguments | positional, after checking labels |
| record construction | one construction instruction, fields in declared order |
| generics | monomorphisation on the IR, after type checking (§4.12) |
| `range(a, b)` | library function |

Everything in this table is gone **in the IR**, and the erasure is inspectable with
`heroes build --dump-ir`, one golden per row. **All of it but one row is erased by lowering, on the
way in** — the exception is generics, which the row above says so itself: monomorphisation is a
*pass*, after type checking, so `Ty::Generic` is present in the IR until it runs (panel 029). The
sentence used to claim lowering erased everything, which its own table contradicted two lines
earlier; `ir/verify.rs` had documented the exception informally, which is how a false sentence
survives — the truth was written somewhere nobody compares it against. Three rows used to say `match` where
they meant *a branch*: `match` is the surface's only destructuring construct, and the IR has no
`match` either — it has `switch` on a variant tag and `branch` on a `bool`.

*Amended by panel 019 (2026-08-04).* This section used to end "everything in this table should be
gone before the lowering stage sees the tree", which asks for a desugared tree the compiler does not
build. It would have cost ~450 lines (a second tree type, its printer, its tests) and invalidated two
dense side tables the frontend already produces — `Checked::expr_types` and `Resolved::uses`, both
indexed by `Ast::exprs`. GHC is the usual precedent cited for the extra tree, and it is precedent for
something else: what Core bought GHC was **Core Lint**, "an 100% independent check on the type
inference engine". Heroes buys the check without the tree — `ir::verify` runs on every lowered
function in tests.

---

## Part 6 — Rejected permanently

Do not add these. **Most violate locality, and most are also expensive to implement** — which is
why maximising locality satisfies two vertices of the triangle at once. *Most*, not each: the claim
used to read "each" and was false of four of its own rows. `Subtyping`, `Higher-kinded types,
dependent types`, `Coroutines` and `Metatables / dynamic dispatch` are refused on **cost alone** and
say so in their own reasons, which is a legitimate refusal — §1.1 makes simplicity the ceiling — and
was being contradicted by the sentence above them (corrected 2026-08-12, panel 039).

**A row here must be falsifiable by the compiler** (author decision 2026-08-12, from panel 039's
spec-warden). Every row must name the program or the compiler fact that would make it wrong, so a
reader can check it rather than trust it. The rule exists because of an asymmetry nothing else in
this project covers: Principle 0 (§1.0) is a burden on what **enters** the language, so a *permanent
rejection* was the one design act here subject to no burden of proof at all — the cheapest possible
way to make the most expensive kind of commitment. Panel 039 is what proved the rule has teeth
before it was written: a proposed row rejecting compile-time evaluation was refuted by the emitted C
of *every* Heroes program, each of which carries two `_Static_assert(__builtin_constant_p(…))`, and
the preamble's own "each" died the same afternoon. Where a row's falsifier is a program, it belongs
in `tests/golden/`; where it is a measurement, it belongs in `docs/measurements/`.

| Feature | Why not |
|---|---|
| `null` | the billion-dollar mistake; retrofitting absence is what Kotlin and C# spent years on |
| Exceptions | invisible control flow; produces unhandled-path bugs that raise no compile error; unwinding across the C boundary is UB |
| Inheritance | understanding `this.save()` requires walking a class chain you cannot see |
| Operator overloading | `a + b` can do anything, defined elsewhere |
| Function overloading | which one runs depends on resolution you cannot see on the line |
| Macros | user code that expands into other code, or extends the syntax: the code you read is not the code that runs |
| Implicit conversions | `f(x)` works for an invisible reason |
| Global mutable state | meaning depends on ambient context |
| Shadowing | the model believes it refers to one variable and hits another |
| Global type inference (Hindley–Milner) | moves errors far from their cause |
| Subtyping | variance is the most expensive item in modern type systems |
| Higher-kinded types, dependent types | out of scale |
| Borrow checker | **unnecessary** — value semantics removes aliasing, so there is nothing to check |
| Ruby-style metaprogramming (`method_missing`, `instance_eval`) | you cannot tell from the source what is callable; this is why LLMs err more on Rails/RSpec than on plain Ruby; and it requires runtime dispatch, i.e. an interpreter |
| Coroutines | require stack switching (`ucontext` or stack copying) — a step change in complexity |
| Metatables / dynamic dispatch | boxing every value, which makes every C call require unbox-in/box-out — precisely the glue we refused to write |
| Space application (`f x y`) | arity lives elsewhere |
| 1-based indexing | every FFI boundary becomes a silent off-by-one |
| Literate source (code inside a markdown document) | breaks editors, explodes tokens, noisy diffs. `heroes doc` generates the document instead — one direction only |
| Non-ASCII syntax | tokenises badly, untypeable on some layouts, encoding-fragile |
| Style-insensitive identifiers (Nim's `fooBar` ≡ `foo_bar`) | two spellings for one thing, against §4.15's "exactly one correct way"; for a model it is pure confusion |
| Private record fields | it is an *opaque type* wearing a field annotation's name: §4.9 makes field-named construction mandatory, so a hidden field makes the type unconstructible from outside, and §4.3's structural `==` compares a field the caller cannot name. The precedented form (Haskell 2010 §5.2, Ada 83) hides the **type**, never the field — and §4.20 makes a record a C struct by value, so a shim reads the "private" field at its offset anyway (panel 033) |
| Private variant cases | it forces `_` to be legal on a variant whose hidden case the reader may not see, so a line's legality depends on a declaration nobody may read — and §4.7 bans `_` on variants precisely to keep exhaustiveness meaning something. Rust shipped this (`priv` variants), deleted it in 2014 as rarely used, re-added it per-**type** as `#[non_exhaustive]` in 1.40, and documents the price as the loss of exhaustiveness checking (panel 033) |

**Compile-time evaluation (`comptime`, `constexpr`, CTFE) is examined and
deliberately *unplaced* — it is neither on this list nor on Part 7's** (panel 039,
2026-08-12; the long form is `docs/reasoning/003-comptime-and-macros.md`). This
paragraph exists so the word is greppable: an answer reachable only from a
reasoning note is uncitable under CLAUDE.md §1, and that is exactly how the
question came to be asked twice. The row above does **not** cover it — three
judges found that reason false of comptime, and its author is the source: Zig has
comptime *so that* macros are unnecessary, which runs the implication the other
way. Nothing on the closure list (§1.0) needs it; the strongest case in a language
with no standard library is the header constant, and §4.19's `extern constant`
answers that by **computing nothing** — the preprocessor evaluates, `_Generic`
type-checks, an accessor returns. Neither list took it: Part 6 was refused because
a one-line rejection here is falsified by this compiler's own output (every
emitted unit asserts `__builtin_constant_p` twice, with no `extern` in the
program), and Part 7 was vetoed because its preamble claims its items lose *only*
on simplicity, which is false of a pass that would duplicate 1,754 lines of
runtime semantics and contradict `ir/mono.rs`'s structural refusal of a depth
limit. **Three conditions return it to the table, jointly**: a measured Part 11
effect that `extern constant` does not already close · a *structural* termination
argument of `mono.rs:24-33`'s standard, never a quota (Rust shipped one and
removed it in 1.72; Zig's is bypassed by generic types into a compiler segfault) ·
and a named deletion, because §1.7's test is subtraction and generics passed it
where this does not.

---

## Part 7 — Deferred, in order

These lose only on the *simplicity* vertex, which means they are postponed rather than refused. The
distinction matters and depends on which vertex said no.

**Nothing on this list is considered until the Principle 0 closure list (§1.0) compiles itself.**
The ordering below is not negotiable before the fixpoint; items 2–4 are the exception because they
are *on* the closure list.

1. **Closures** — v1.5, immediately after the first running program. Much cheaper than usual thanks
   to value semantics (capture by copy = a record plus a function pointer). Will delete the handful
   of named one-line helper functions that currently exist only to be passed around.
2. **File I/O** — `read_file(path) -> str?`, `write_file(path, s) -> ()?`. Two functions via FFI to
   `fopen`/`fread`. **Required for self-hosting.**
3. **Command-line arguments** — `args() -> [str]`. One function. **Required for self-hosting.**
4. **Modules** — ~~`use "list"`~~ **landed at panel 031 as `use list`**, a bare identifier rather
   than a quoted path: one file per module, always-qualified references (`list.map`), no
   `import *`, no aliases, no package hierarchy. Qualification costs tokens but **buys locality**:
   seeing `list.map` tells you where it came from without searching. **Required for self-hosting** —
   a compiler is 5–8k lines and one file is masochism. Cut from v1 because self-hosting is not the
   v1 goal; this returned ~60 spec tokens that paid for other additions, and the return cost **+71
   measured** to discharge — 11 over, on the legal example and the transitivity rule that two judges'
   evidence bought. The spelling changed because Go's quoted path is the one form with a documented,
   frozen defect (a file's imports cannot be resolved syntactically) while Nim's identifier rule
   leaves adding strings additive.
5. **`alias`** — `Env = alias` / `{str: i64}`. Deferred on the strongest possible ground:
   **reversibility.** It is the only purely additive thing in the language — adding it in v2
   invalidates no existing code, changes no rule, touches no core. Precedent: Go shipped in 2012 and
   added transparent type aliases in 1.9, in 2017. Five years, nobody died. When added, make it
   **transparent** (not a new type, fully interchangeable) and make **errors expand the alias**
   (`expects: Env = {str: i64}`), which restores the locality the indirection costs. Call it `alias`,
   not `type`: `type` suggests you are creating a type (misread, same family as
   `enumeration`/`variant`), and `type` is far too common an identifier to burn. Keeping `type` free
   also leaves the door open for v2 **distinct types** (`UserId` and `PostId` both ints but not
   interchangeable), which catch a real class of error but cost conversion syntax.
6. **Doctests** — v2, once the `test` mechanism is proven.
7. **String interpolation** — deferred; `print` takes multiple arguments.
8. **Traits / interfaces** — genuinely useful, but instance resolution is expensive. Their absence
   means there is no user-extensible iteration protocol: `for x in ...` stays a compiler special case
   for arrays, maps and ranges.
9. **Variant constructors as values** — the parser's `term` and `expression` functions are identical
   except for two names, and would unify into
   `sequence(@p, is_times, .product)` if a variant constructor could be passed as a value. This is
   the first place the language is measurably poorer than needed. Note it, don't fix it yet.
10. **Sized integers** (`i8 u8 i32 u64`) — the highest-risk addition in the language, because
    conversion rules are the most expensive spec item that exists and the number-one error source
    in C. Design with care, never in v1. **The reason was mis-filed and is corrected here**
    (panel 041, 2026-08-12): this row read *"needed for any binary format"*, which is a v2 case,
    when the live pressure is the **FFI** — §1.11's founding constraint, inside v1. Same shape as
    the `Macros` row panel 039 repaired: a correct decision under a reason that does not describe
    the real case. **The program that would make this row wrong is `strlen`**, not `SIZE_MAX`:
    `extern function strlen(s: cstr) -> i64` is `ffi_return_type` and no result type exists to
    correct it to, whereas `SIZE_MAX` is unreadable under *every* option on panel 041's menu — a
    `u64` that can only be converted aborts, so naming it as the falsifier would name a case no
    addition closes. What §4.19 actually asked for is a **C-width** vocabulary (`c_int`, `const`),
    which is this row; a Heroes-width `u64` wearing a C-width name is not.
11. **A `raw` module for low-level access** — see Part 9.
12. **Inline blocks (Kotlin-style)** — `repeat 3` / `with file("x")` where the last parameter is a
    block expanded at the call site rather than becoming a closure. This is the acceptable substitute
    for Ruby DSLs, because the function name is *written on the line* so you know where to look, the
    receiver is not implicit, and no dynamic dispatch is needed since the body is copied at compile
    time. Requires an inlining pass in the lowering; likely lands together with closures.
13. **Concurrency** — the largest gap. But value semantics puts you in the best possible position:
    **no aliasing means no data race is expressible**, since there is no shared state to protect.
    This is Erlang's 1986 insight (immutability plus message passing for concurrent systems) and the
    reason actors are natural there. The road is open and wide; it is simply far away. The shape is
    written down now — **isolated per-thread heaps, copying at the boundaries** — so the eventual
    implementation is a mechanical exercise rather than a redesign, and because two of its
    consequences constrain code being written today.

    - **A heap**, here, is the region where runtime-sized data lives: `str`, `[T]`, `{K: V}` and the
      records reached through them — not scalars in registers or on the stack. *Separate* heaps means
      each thread allocates from and frees into a region no other thread touches, so there is no
      shared arena and therefore no lock on allocation.
    - **A message is any Heroes value.** No message type, no envelope, no serialisation format. This
      falls straight out of §4.10: a value holds no pointer to data it does not own and no shared
      reference, so it is already a self-contained thing. An `i64`, a `str`, a `[f64]`, a record, a
      map — all equally valid, all handled by one rule.
    - **The transport is a mailbox** — one queue per thread; the sender copies into the receiver's
      heap and appends to the tail, the receiver pops from the head when it is ready. Deliberately
      boring, and it *can* be boring precisely because what goes into it is isolated by construction:
      the question that dominates shared-memory designs — what if the sender mutates it in flight —
      does not exist here.
    - **Two copy regimes, and they are not a contradiction.** Within a thread, §4.10's rule: share
      physically, copy only on mutation of shared data. At a thread boundary, the copy is
      unconditional and real. The first exists to avoid waste, the second to guarantee isolation.
      Expect two distinct code paths and do not try to unify them.
    - **The cost rule**, so the model gets used where it works: a boundary copy costs the size of the
      value. It pays off when **the message is small relative to the work it triggers** — send a
      slice, get back a number — and it is a bad fit for shuttling large values back and forth to do
      little with them. Which is why the first and probably only rung Heroes needs is plain data
      parallelism.
    - **Threads, not green threads, not coroutines.** OS threads — few, real, OS-scheduled — give
      *parallelism*, splitting work across cores, and are the only kind Heroes will use. Green
      threads (language-scheduled workers multiplexed onto OS threads) and coroutines (the suspend/
      resume mechanism they are usually built from) give *concurrency*, thousands of interleaved
      logical tasks. Heroes wants the first and not the second. Coroutines are already refused
      permanently in Part 6; green threads stay *here* rather than joining them because the vertex
      that says no is **simplicity** — a scheduler to write and maintain — and by this Part's own
      preamble that makes a deferral, not a rejection. The operative rule is blunt either way: **do
      not build a scheduler.**
    - **The one industrial precedent does not do what this describes, and the exception is the
      instructive part.** Erlang copies message data between processes *except* refc binaries (over
      64 bytes) and literals, which sit in a shared area with a reference count and travel as a
      pointer. So the boundary copy is not an absolute even in the system that made isolation famous,
      and its escape hatch for large immutable payloads is precisely a **cross-heap refcount** —
      which is an atomic counter, i.e. the one thing this design gets to avoid. That is a trade to
      make consciously if the copies ever hurt, not to rediscover.

    **Net effect on v1: two rules, both already true, both now invariants.** Reach the allocator
    through a **single point** — today that is literally one `malloc` and one `free`, both inside the
    `str` primitives in `runtime/runtime.c` — and keep refcount operations behind a **narrow,
    never-inlined boundary**: `hero_str_incref`/`hero_str_decref` are calls into a separately
    compiled translation unit, so no inlining can smear refcount arithmetic across code that a
    thread-local counter would later have to change. M-value-aggregates's array and map allocation is the first test
    of the first rule. Everything else in this item costs v1 nothing.
14. **Declaration visibility** — `private` on a top-level declaration, hiding it from every other
    module. Costed and deferred at panel 033, where five judges produced no vote to adopt. It is
    cheap — ~80 non-test lines, **zero** in the IR, the ownership pass, the emitter and the mangler,
    because one whole-program translation unit gives visibility no linkage consequence — and the
    landing form is fixed at **+18 measured** (the bare licence alone; the "not written on fields or
    variant cases" clause is a prohibition, and Part 6 now carries both for free). It waits because
    Principle 0's burden is unmet: not on §1.0's closure list, no Part 11 effect, and its offered
    §1-argument — that `private` lets §4.4's unused rule reach the top level — is refuted by the
    compiler, which already compiles one whole program and already proves that shape in
    `unused_use` at zero tokens. **M-selfhost-probe is the instrument, not an exception to this Part's
    preamble**: the probe reports blockages, and a blockage would put visibility on the closure
    list, which is what excepts items 2–4. Absent that it waits like everything else here.
    **Direction unresolved and left to a count**: default-private has the whole ancestry (Modula-2 →
    Oberon, Nim, Go, Zig, Erlang; Rust RFC 0001 and Swift SE-0117 both reversed *towards* it, none
    away), and three judges measured against it — 256 `private` markers against 355 `export` over
    this compiler's own tree, and a binding module is public by construction, so `export` is 20/20
    lines on libm and 287/287 on SQLite for zero bytes of object code.
15. **The QBE backend — scheduled, post-fixpoint.** Not "if and when": one backend proves nothing
    about the IR's claimed agnosticism, and QBE carries the register-allocation and instruction-
    selection lesson this project originally wanted (~500 lines from the same IR; see §3.2 for what
    QBE is). Later still: LLVM textual `.ll`, or wasm (which brings sandboxing and the browser, and
    the genuinely instructive relooper problem). Emitting C, QBE and `.ll` in parallel is also the
    best way to understand what they have in common.
16. **Symmetric variant syntax — v2** (panel 035's historian; author decision 2026-08-12, taken in
    `/decide` over the alternative). Today the language is **asymmetric**: `.case(field: value)`
    builds and `.case name` matches. ML, Haskell, OCaml and Rust all use one syntax for both, which
    is why none of them *can* leave construction unstated — the rule that describes matching
    describes building. Heroes can, and did: variant construction is one of the silences panel 035
    counted, and it is the only one whose cause is structural rather than an omission somebody could
    have noticed. The two ways to remove the *possibility* rather than this instance were an **EBNF
    appendix in Wirth's manner** — every form stated once, mechanically, so a silence is a missing
    production rather than a missing paragraph — and **restoring the symmetry in v2**. The second is
    chosen: an appendix is spec tokens spent on a grammar a reader would then have to reconcile with
    the prose, and this project's own precedent is that the prose plus a compiler diagnostic beats a
    second description (panel 035's mechanism). **What that costs until v2 is stated rather than
    hidden**: construction stays unspecified, so a program written from the spec alone cannot build a
    variant, and Part 11's first-try measurement should expect exactly that failure and attribute it
    here rather than to the writer.

---

## Part 8 — Known warts

Do not hide these. Each was noticed by writing real programs in the language, and each was left
visible rather than patched with a second form.

1. **The header is still heavier than a one-line body deserves.** Panel 018 shrank it —
   `function is_digit(c: u8) -> bool` lost `= …:` and reads like the call site — but a realistic
   file still has nine one-line predicates whose header outweighs the body. On a real function
   like `tokenize` the proportion is right; on a one-line predicate it is not. This remains the
   strongest argument for pulling closures forward — those helpers exist mostly to be passed
   around.
2. **`constant` costs three lines for one number.** For a map of weights the proportion is fine; for
   an integer it isn't. Accepted, because a second form for scalar constants would cost more than it
   saves.
3. **Inline `if` as an expression takes four lines.** There is no `then` keyword and no brace form,
   so a ternary is:
   ```
   state = if t.done
       "[x]"
   else
       "[ ]"
   ```
   This is the first price of dropping braces. `then` was proposed and rejected: a second form of
   `if` to save three lines per file.
4. **Function type parens.** `(function(A) -> B)` needs its parens because declarations don't have any.
   Second price of the same decision.
5. **Errors are codes plus strings, not types.** Weak, and now measured: `typo-code` catches
   **0 of 25** (§4.6). Mitigated by asserting on `e.code`, and better mitigated by writing the codes
   as `constant`s, which makes a typo an `unknown_name` error for nothing.
   **This stays a wart and is deliberately not a Part 7 item** (panel 034): Part 7 admits what loses
   *only* on simplicity, and typed errors lose on three vertices — §4.12's positive rule (generics on
   functions only, **not on types**), Part 7 item 8's traits for any conversion, and the budget.
   Filing them as deferred would turn a wart into a promise. The price, measured against real
   `sqlite3.h` rather than argued: an error carrying what a C library actually reports
   (`sqlite3_errmsg`, copied through `hero_str_from_bytes`) is **32 bytes and refcounted**, and
   `ptr?SqlErr` is **40** — identical to today; the 16-byte win needs a payload-free error, which
   cannot carry a message. `?` across two bindings stops being a struct copy and needs a
   hand-written converter per *(from, to)* pair; `m[k]`'s runtime-built failure has no declared type
   to be; and `record R { next: R? }` is unchanged either way. Swift shipped typed throws in 6.0,
   a decade in and with full generics, and SE-0413 itself says *"Resist the temptation to use typed
   throws"*.
6. **The unrecoverable indentation case** (4.15). One accepted silent-error class.
7. **A single-child node needs a one-element array**, because arrays are the only indirection.
8. **String concatenation is O(n²)** under value semantics — **and so is the `[str]` you build to
   hand `join`** (panel 037, measured: 1 000 000 pushes 806 s). The remedy that works today is
   chunked accumulation, not a language change; §4.10 carries the numbers and the refused
   alternative. Needs `join`/`Builder` for compiler-scale output.
9. **UFCS is the one deliberate duplication**: `f(x)` and `x.f()` both work. Kept for chains,
   acknowledged as the exception to "one way only".
10. **`?` combined with `@` copy-out** needed an explicit rule (copy-out always happens) because it
    was undefined. Watch for other interactions of this kind.
11. **No user-extensible iteration**, since there are no traits.
12. **Every name passes through a mangler** (§3.1): a valid Heroes identifier can collide with a C
    keyword or a libc symbol, and without the mangler a correct program fails to compile for
    reasons invisible to its author — the exact error class this project exists to eliminate. The
    mangled names are what `lldb` shows for variables (see Part 2's reworded debugger non-goal).
13. **An emitter bug surfaces as a clang error**, not a Heroes error. Mitigated — `#line` points the
    message at `.hero` source and the `-Werror` set catches the UB-shaped cases — but not erased:
    when the emitter is wrong, the author reads C.
14. **Error accumulation is manual**: `@diags: [Diagnostic]` threaded through every pass of the
    self-hosted compiler (§1.0). Legal, uniform, and slightly noisy; the price of having neither
    globals nor closures in v1.
15. **`"C:\temp"` is silently a tab** (panel 008). Reserving the backslash makes `"\d+"` a loud
    error, but it cannot catch a path whose next letter happens to name a legal escape. Inherited by
    every language with C-style escapes; the remedy is raw string literals, which v1 does not have.
16. **`print` took only half of `WriteLn`** (panel 008, historian). design.md cites Pascal's
    `WriteLn` as the fifty-year precedent for `print`'s forced trailing newline — but Pascal pairs it
    with `write` (no newline) and out-of-string character codes (`#10`), and Oberon-07 uses `0AX`.
    Heroes has neither, so before escapes landed it was strictly weaker than its own cited
    precedent. Escapes close the gap for literals; there is still no way to print without a
    trailing newline.

---

## Part 9 — Low-level access, when the time comes

The historical answer to this exists and almost nobody knows it. **Oberon** is a small, safe language
with no raw pointers — and Wirth wrote an entire operating system in it. How? By putting *all* unsafe
operations in a pseudo-module called `SYSTEM`. Any code touching the low level had to import it,
visibly, at the top of the file. **Modula-3** did the same with `UNSAFE` modules, Rust with `unsafe`,
Zig with `@ptrCast`.

The principle: **low-level access is a marked, bounded region, not a scattering of features.**

```
use raw

function read_u32(b: [i64], off: i64) -> i64
    return raw.load_u32(b, off)
```

Everything that breaks the guarantees goes in `raw`: pointers, load/store at offsets, casts. Outside
it, the language is unchanged. And because the import is **visible at the top of the file**, locality
is preserved — you know immediately whether a file is dangerous.

Note this requires modules (deferred item 4) and, for anything serious, sized integers (deferred item
10).

**On the web, be honest with the author:** it is far away. Frontend requires a wasm backend, which
breaks the native-FFI premise. Backend requires concurrency, plus sockets, TLS, HTTP and an
ecosystem. One bright spot: **JSON is an excellent fit** — a JSON value is literally a `variant` with
payload, and parsers come out well in this language. But it is a library you would write yourself,
like everything else. The web will arrive through the FFI or not at all, and forcing the language
toward it now would break the simplicity that is its only competitive advantage.

---

## Part 10 — Build order

Core first, sugar second — and Part 5 tells you which is which. **After each step there should be
something runnable or inspectable.** A compiler that compiles `1+2` and executes it teaches more
than six months of design on paper.

1. **Lexer** with rigid indentation (indent/dedent tokens, tab rejection, semicolon insertion),
   character literals, `#` comments retained for the formatter.
2. **Parser** to AST: the four entities, `=` and `@`, expressions with correct precedence.
   `???` as an AST node from the start.
3. **Pretty printer.** Early, because error messages need it and the canonical formatter is it.
4. **Resolver**: name binding, scoping, no shadowing, unused-variable errors, order-independent
   top-level.
5. **Bidirectional type checker**: `record`, `variant` with payload, `match` with exhaustiveness.
   **Rich error messages from day one** — not retrofitted.
6. **Lowering** to a three-address internal IR with explicit basic blocks. This is the heart. Flatten
   every expression into temporaries; reduce control flow to labels and conditional jumps. Pass
   order matters: named-argument checking (§4.9) runs **before** monomorphisation, or generic
   collapse (`A := B := i64`) spuriously triggers the same-typed-argument rule.
7. **C emission from the IR** — `goto`+labels, prototypes and topologically-sorted typedefs,
   the mangler, `#line` from day one, the `-Werror` set, `hero_unreachable`, and the double-emit
   determinism diff (green forever after). **First running program.** Celebrate here.
8. **Sugar layer**: `if`, `for x in xs`, UFCS, `T?` with `?` / `.must()` / `.default()`.
9. **Value semantics with copy-on-write**: the **ownership pass** (refcount insertion in lowering,
   visible in `--dump-ir`, cleanup chains on every exit edge), the **type-descriptor pass**
   (per-type `copy`/`drop`/`eq`/`hash`), and the C runtime — goldens run under
   `-fsanitize=address,undefined` from here on.
10. **Functions as values.**
11. **Generics by monomorphisation.**
12. **`???` output**, `test` runner, `assert` with source text, `outline`.
13. **The library written in the language**: `map`, `filter`, `fold`, `range`, `join`.
14. **Modules — the namespace, and only the namespace.** Qualified cross-module names, one module
    string per declaration through the mangler, the library as an ordinary module — emitted as
    **one whole-program `.c`**, exactly as a single file is. The closure list says *modules*; it
    never says translation units, and one `.c` per module with prototypes across them and a
    per-module cache is a **build architecture**, priced at 2–3× the namespace and deferred to
    step 18 (panel 030 R1). Ordering note: this step precedes the FFI, which reverses revision 2's
    order and is the author's call, taken with the panel's dissent on the record.
15. **FFI**, and one real C binding. **SQLite is the right acceptance test**: pure C, clean header,
    immediate value — and now cheap, since clang verifies the declarations (§4.19). If
    `sqlite3_open`, a query and a close work without you having written a stdlib, the architecture
    holds. This step also **decides the route** for the closure list's last three rows — file I/O,
    `args()`, `exit(code)` — because they do not bind as plain `extern`s: `void exit(int64_t)`
    against `void exit(int)` is `conflicting types`, and the boundary lacks `c_int`, `size_t` and
    `const` (Part 7 item 10, panel 030 R3). Whichever route wins pays its measured spec cost.
16. **A probe before the port.** One representative file — the lexer — ported to Heroes for real,
    to measure what self-hosting still lacks *before* committing to the whole port. It reports
    blockages, not wishes: a form whose workaround compiles is deferred by default, because the
    workaround compiling is the proof that the compiler did not need the form.
17. Then the port to Heroes, finished by the **fixpoint**: A builds `B.c`, B builds `C.c`, `diff
    B.c C.c` empty, clang version pinned and recorded. The bootstrap compiler is archived here.
    (Closures, being off the closure list, wait for the fixpoint.)
18. **Separate compilation** — one `.c` per module, prototypes across translation units, the
    per-module cache. After the fixpoint, and never before it: the cache is where §4.19's
    guarantee dies quietly, since a caller in another translation unit sees a generated prototype
    rather than the header, and a cached object stops re-running the check the guarantee *is*.

**Golden tests from step 1.** A directory of `.hero` files each with expected output, plus a script
that compiles and diffs. This is the only thing that makes it possible to evolve the language without
silently breaking it.

**Write the spec in condensed English early — around step 7, not at the end.** It is the control
instrument: if it doesn't fit in 4096 tokens, too much has been added, and you find out in an hour
instead of three months. Count it with a real tokeniser, not by estimation — the BPE vocabulary
contains arbitrary choices nobody predicts.

---

## Part 11 — Measurement harness

Build this. It is what separates this project from an opinion. Methodology hardened by panel 000
(the LLM-ergonomics review: "revision 1's numbers would have been meaningless").

**The control arm.** Every trial runs against `heroes check` *and* `heroes check --permissive` —
the same compiler with the thesis-bearing checks disabled (`_` on variants allowed, positional
same-typed arguments, no mandatory type on `@` declarations, non-exhaustive `match`, unused
variables tolerated). Same model, same spec size, same unfamiliarity, opposite design choices: the
only comparison that isolates *design* from training-data familiarity. Without it there is no
falsifiable claim.

1. **Token counting.** With **two vendored BPE tokenisers** (Anthropic's legacy `claude.json` and
   `cl100k_base`), pinned by sha256 and read offline — no API key, no network. The **maximum over
   instruments binds** and their spread is published with every run: one instrument cannot detect
   its own drift, and two disagreeing is the honest error bar. Measured on frozen spec v0 the
   spread is 59 tokens (3%), not the order of magnitude this document once feared: an earlier
   revision claimed `tiktoken` "undercounts Claude tokens 15–20%", which is unsourced and was
   contradicted by measurement — `cl100k` counts 3% *above* Anthropic's own legacy tokeniser on
   this document (panel 011). **Count declarations separately from uses** (verifies §1.5's
   asymmetry) and **punctuation separately from the rest** (settles whether braces/parens are 3%
   or 15%). The §1.6 budget is checked against the spec.
2. **First-try rate.** Spec → model → programs → count. Two gradings, both reported: *compile* rate
   (from the type checker's arrival) and *tests-pass* rate (from `test`'s arrival — §4.18:
   compiling is not working). 20 frozen tasks × 5 samples, Wilson intervals; frozen, hashed prompt
   templates; API-only, single-turn, **spec-only context** — the measured model co-designed this
   language, so it never sees this document or the repo. One non-Anthropic model as robustness.
3. **Silent-error rate.** Mutation operators as *data* (swap same-typed args, drop a variant case,
   forget `@`, mutate an undeclared name, typo an identifier), applied mechanically to the golden
   corpus; per-operator kill rate. Deterministic, free, no API needed.
4. **Turns-to-green.** Given a broken program, how many exchanges to make it compile — capped at 5,
   non-convergence counted separately. Measures the rich errors, otherwise unfalsifiable.

Every measurement records provenance: spec sha, compiler sha, model id, prompt sha, suite sha.
Never diff runs with different compiler shas unless explicitly flagged — a stricter checker lowers
first-try rate with zero language change. Re-run after every significant change; change one thing
at a time. The pre-amendment v0 baseline is taken before any panel amendment lands.

---

## Appendix — Historical grounding

The design is not invented from nothing. Where each piece comes from, so that departures are
deliberate:

- **Wheeler, 1951 (EDSAC)** — the subroutine. Why `subroutine` means "returns nothing" in the
  FORTRAN/Pascal tradition, and why we don't use the word.
- **Church, 1930s** — `->` for function types, via ML, Haskell, Rust, Swift, TypeScript, Python type
  hints. The single most familiar piece of syntax in existence; kept unchanged. Also why `->` and
  `=>` are *not* unified: one lives in types, one in terms, and each has exactly one meaning.
- **Pascal / ML** — `record` and `variant` as the product/sum pair; *variant record* is the direct
  ancestor of tagged unions.
- **Wirth** — the Oberon report at sixteen pages as the calibration for spec size; Pascal-P4 at ~4000
  lines as the self-hosting target; the `SYSTEM` module as the model for bounded unsafety.
- **Modula-3 `UNSAFE`, Rust `unsafe`, Zig `@ptrCast`** — the same bounded-unsafety pattern.
- **Cyclone** — references restricted to function parameters only, eliminating lifetime annotations.
  Promoted from a note to a load-bearing precedent: it names the discipline (**the Cyclone rule**)
  the Rust bootstrap compiler is written in (§3.4), which is also why the port to Heroes is
  mechanical.
- **Hylo / Val** — mutable value semantics: memory safety without a borrow checker. The direct
  ancestor of Part 4.10, and the current research frontier for exactly this thesis. The specific
  insight Heroes depends on comes from here: **if values are never aliased, reference cycles cannot
  be constructed, so reference counting is complete without a cycle collector.** That is not a
  compromise — it is the reason the whole design is small.
- **Swift** — `inout` (our `@` parameters), copy-on-write, leading-dot variant syntax. The most
  direct precedent and the most reassuring one, because Swift is not a research language: `struct`,
  `Array`, `Dictionary` and `String` are value types with copy-on-write over reference counting,
  mechanically what §4.10 describes. The difference is that Swift *also* has `class`, with shared
  reference semantics, and therefore inherits everything Heroes escapes — cycles, `weak`/`unowned`,
  and an atomic counter to make sharing thread-safe. **Heroes takes the half that works and declines
  the half that generates the complexity.**
- **Go** — `gofmt` and canonical formatting; documentation by adjacency; unused variables as errors;
  semicolon insertion; shipping without user generics for a decade; adding type aliases five years
  after 1.0.
- **Zig** — tests in the source file; `[:0]u8` NUL-terminated slices (our string design).
- **Erlang, 1986** — immutability plus message passing chosen specifically for concurrent systems,
  enforced absolutely, in systems with famously long uptimes. The reason value semantics leaves the
  concurrency road open. The relationship to Heroes is close but **the motive is inverted**, and the
  inversion is the instructive part: Erlang removes sharing *for concurrency* — isolated processes,
  a copy per message — while Heroes removes it *for local readability* — copy-on-write, sharing
  freely at the physical level as long as nobody writes. Same principle, opposite engines. Erlang
  pays copies always and buys isolation; Heroes pays copies only on mutation and buys the absence of
  aliasing bugs. One correction to the folklore, load-bearing for Part 7.13: Erlang does **not** copy
  every message. Refc binaries (over 64 bytes) and literals live in a shared area with a reference
  count and travel as a pointer, so the boundary copy is not absolute even in the system that made
  isolation famous —
  [erlang.org](https://www.erlang.org/doc/system/eff_guide_processes.html),
  [binary handling](https://www.erlang.org/doc/system/binaryhandling.html).
- **Nim** — promoted from a note to a primary source now that the backend is C: the `importc`
  pragma is the adopted FFI mechanism (§4.19), the per-module compilation cache is the model for
  `build/`, and `nim r` is the model for `heroes run`. Also `proc` vs `func` as the resolution of
  the purity-connotation problem; and as a cautionary tale, a compiler that is simple to *use* and
  enormous to *implement* — the standing rule is **copy Nim's surface, never its implementation**.
- **Odin / Jai** — uniform declaration syntax; `proc` as umbrella.
- **Lua / Nelua / Pallene** — the "Lua but compiled" precedent, and the lesson that the single
  heterogeneous table must be sacrificed because boxed values make every C call require marshalling.
- **Ruby** — the DSL *reading rhythm* (named blocks, named arguments, dot chains) is worth taking;
  the *mechanism* (`method_missing`, `instance_eval`) is the worst possible case for locality.
- **APL / J / K / BQN** — cited twice, in opposite directions, and the split is deliberate. Against,
  on *notation*: the empirical evidence that ultra-compact notation fails for LLMs, and not only for
  lack of training data. In favour, on the *value model*: sixty years of value semantics over whole
  arrays, with no aliasing and no references in the surface language — and K's descendant kdb+ holds
  10 of the 17 STAC-M3 Antuco benchmarks and 9 of 10 Kanaga, the financial industry's own
  time-series suite, which is the evidence that §4.10's model is not what makes a language slow.
  What they establish for Heroes is that *transforming whole values* is a complete programming
  style, not a restricted one.
- **R / MATLAB** — copy-on-write value semantics for the core data structures, and jointly the
  dominant languages of statistical and numerical computing for decades: millions of programmers who
  never needed a mutable reference to do real work. They also document the failure mode honestly,
  and it is *exactly* Heroes': both are slow when someone writes element-at-a-time loops over shared
  structures, which is the same O(n)-per-mutation bill §4.10 declares. Precedent and warning in one
  entry.
- **Idris / Agda** — typed holes, here repurposed from proof assistants to code generation.
- **Rust / Elm** — the demonstration that error message quality transforms the experience; here
  aimed at a different reader.
- **Unison** — code stored as an AST with names as labels; the origin of the two-representation idea
  that was explored and ultimately dropped in favour of a single readable syntax.
- **cfront, Nim, Vala, Chicken Scheme, Cython, early Haskell** — the C-emission lineage, now the
  *chosen* backend (§3.1, panel 001): historically the dominant technique for languages built by few
  people.
- **Bidirectional type checking** — the modern consensus over global Hindley–Milner, chosen here for
  error locality.

---

## Appendix — A complete example program

This is the acceptance target: a lexer, parser and evaluator for arithmetic expressions with
variables. If the language can express this, it can express its own compiler. Note this is written in
the *current* syntax and has not been validated by any implementation — treat discrepancies as bugs
in this document, and flag them.

**Discrepancies resolved (panels 002/006, decided 2026-08-03; a twelfth found by the checker
itself at M-checker-core, 2026-08-04 — `tokenize`'s own `return out`):** the `T`-where-`T?` sites are now
written `ok(...)` — twelve in all: the six originally flagged (`factor`'s `.num`/`.variable` arms,
`group`'s `return inner`, `term`'s and `expression`'s `return first`, `lookup`'s tail) plus five
more found when applying the rule exhaustively (`term`/`expression`'s variant-construction
returns, `sum_of`/`product_of`'s `return tot`, `evaluate`'s `.num` arm). `main`'s
`print(c, " = ", v)` is legal under panel 006's contract (compiler form, `str`/`i64`/`f64`/`bool`
arguments, no separator, one trailing newline). Still open here: `lookup` keeps the
test-then-`.must()` sentinel §4.9 wants gone — `has(m, k)` was struck at panel 026 and the shape
survived it as `!env[name].is_err()` followed by `env[name].must()`, which is the same double
lookup under a new spelling — and the `err` payload (`e.code`, `e.msg`) is a
built-in shape (§4.6), not a user-declared record — the spec states its two fields and nothing
more.

```
## Calculator
#
# **Lexer**, **parser** and **evaluator** for arithmetic expressions
# with variables. A compiler in miniature.
#
# Grammar, increasing precedence:
#
#     expression := term ('+' term)*
#     term       := factor ('*' factor)*
#     factor     := number | name | '(' expression ')'

## Generic library

# Applies `f` to every element, in order. Named `apply` rather than `map`
# because `map` is a built-in name and no file may redeclare one (panel 015);
# the point here is generics, and any name demonstrates them.
function apply<A, B>(xs: [A], f: (function(A) -> B)) -> [B]
    out: [B] @ []
    for x in xs
        out @ out.push(f(x))
    return out

# Reduces the sequence to one value, from the left. `fold` is a built-in name
# too, so this one is `reduce`.
function reduce<A, B>(xs: [A], initial: B, f: (function(B, A) -> B)) -> B
    acc: B @ initial
    for x in xs
        acc @ f(acc, x)
    return acc

function plus(a: i64, b: i64) -> i64
    return a + b

test "generics work across types"
    assert [1, 2, 3].apply(double) == [2, 4, 6]
    assert [1, 2, 3, 4].reduce(0, plus) == 10

function double(n: i64) -> i64
    return n * 2

## Tokens

variant Token
    num
        v: i64
    name
        s: str
    plus
    times
    lparen
    rparen

# Text and position. Travels through the lexer as `@`.
record Lex
    text: str
    pos: i64

function at_end(l: Lex) -> bool
    return l.pos >= l.text.len()

function here(l: Lex) -> u8
    return l.text[l.pos]

function advance(@l: Lex)
    l.pos @ l.pos + 1

function is_digit(c: u8) -> bool
    return c >= '0' && c <= '9'

function is_letter(c: u8) -> bool
    return c >= 'a' && c <= 'z'

# Reads an integer, consuming it.
function read_number(@l: Lex) -> i64
    v: i64 @ 0
    while !l.at_end() && l.here().is_digit()
        v @ v * 10 + fit_i64(l.here() - '0')
        advance(@l)
    return v

# Reads an identifier, consuming it.
function read_name(@l: Lex) -> str
    start = l.pos
    while !l.at_end() && l.here().is_letter()
        advance(@l)
    return l.text.slice(from: start, to: l.pos)

# Splits the text into tokens. Fails on the first unknown character.
function tokenize(text: str) -> [Token]?
    l: Lex @ Lex(text: text, pos: 0)
    out: [Token] @ []

    while !l.at_end()
        c = l.here()

        if c == ' '
            advance(@l)
        else if c.is_digit()
            out @ out.push(.num(v: read_number(@l)))
        else if c.is_letter()
            out @ out.push(.name(s: read_name(@l)))
        else if c == '+'
            out @ out.push(.plus)
            advance(@l)
        else if c == '*'
            out @ out.push(.times)
            advance(@l)
        else if c == '('
            out @ out.push(.lparen)
            advance(@l)
        else if c == ')'
            out @ out.push(.rparen)
            advance(@l)
        else
            return fail("unknown_char", "at position " + l.pos.to_str())

    return ok(out)

test "tokenizes numbers, names and symbols"
    ts = tokenize("12 + xy * (3)").must()
    assert ts.len() == 7
    assert ts[0] == .num(v: 12)
    assert ts[1] == .plus
    assert ts[2] == .name(s: "xy")

test "tokenize rejects the unknown"
    match tokenize("2 $ 3")
        .ok _  => assert false
        .err e => assert e.code == "unknown_char"

## Syntax tree

# A tree node. Children live in an array, which is the language's
# only indirection: that is what makes the recursive type finite.
variant Expr
    num
        v: i64
    variable
        name: str
    sum
        children: [Expr]
    product
        children: [Expr]

# Tokens and position. Travels through the parser as `@`.
record Parse
    ts: [Token]
    pos: i64

function parse_at_end(p: Parse) -> bool
    return p.pos >= p.ts.len()

function parse_here(p: Parse) -> Token
    return p.ts[p.pos]

function parse_advance(@p: Parse)
    p.pos @ p.pos + 1

## Parser
#
# Recursive descent. The four functions call each other; declaration
# order does not matter, so mutual recursion needs no forward
# declarations.

# A number, a name, or a parenthesised expression.
function factor(@p: Parse) -> Expr?
    if p.parse_at_end()
        return fail("unexpected_end", "the expression ends too soon")

    t = p.parse_here()
    parse_advance(@p)

    return match t
        .num n                   => ok(.num(v: n.v))
        .name x                  => ok(.variable(name: x.s))
        .lparen                  => group(@p)
        .plus | .times | .rparen => fail("expected_factor", "found an operator")

# An expression followed by its closing paren.
function group(@p: Parse) -> Expr?
    inner = expression(@p)?
    if p.parse_at_end()
        return fail("unclosed_paren", "missing closing paren")
    if p.parse_here() != .rparen
        return fail("unclosed_paren", "missing closing paren")
    parse_advance(@p)
    return ok(inner)

# A product of one or more factors.
function term(@p: Parse) -> Expr?
    first = factor(@p)?
    children: [Expr] @ [first]

    while !p.parse_at_end() && p.parse_here() == .times
        parse_advance(@p)
        children @ children.push(factor(@p)?)

    if children.len() == 1
        return ok(first)
    return ok(.product(children: children))

# A sum of one or more terms.
function expression(@p: Parse) -> Expr?
    first = term(@p)?
    children: [Expr] @ [first]

    while !p.parse_at_end() && p.parse_here() == .plus
        parse_advance(@p)
        children @ children.push(term(@p)?)

    if children.len() == 1
        return ok(first)
    return ok(.sum(children: children))

## Evaluator

# The value bound to a name, with a decent message if absent.
function lookup(env: {str: i64}, name: str) -> i64?
    if env[name].is_err()
        return fail("unknown_name", "undefined variable: " + name)
    return ok(env[name].must())

function sum_of(children: [Expr], env: {str: i64}) -> i64?
    tot: i64 @ 0
    for c in children
        tot @ tot + evaluate(c, env)?
    return ok(tot)

function product_of(children: [Expr], env: {str: i64}) -> i64?
    tot: i64 @ 1
    for c in children
        tot @ tot * evaluate(c, env)?
    return ok(tot)

# The value of the tree. Fails on the first unknown name.
function evaluate(e: Expr, env: {str: i64}) -> i64?
    return match e
        .num n      => ok(n.v)
        .variable x => env.lookup(x.name)
        .sum s      => s.children.sum_of(env)
        .product q  => q.children.product_of(env)

# Every name appearing in the tree, with repetitions.
function names_used(e: Expr) -> [str]
    return match e
        .num _      => []
        .variable x => [x.name]
        .sum s      => names_of(s.children)
        .product q  => names_of(q.children)

function names_of(children: [Expr]) -> [str]
    out: [str] @ []
    for c in children
        for n in c.names_used()
            out @ out.push(n)
    return out

## Interface

# From text to result, in one call.
function calculate(text: str, env: {str: i64}) -> i64?
    ts = tokenize(text)?
    p: Parse @ Parse(ts: ts, pos: 0)
    tree = expression(@p)?
    if !p.parse_at_end()
        return fail("trailing_tokens", "leftover input at the end")
    return evaluate(tree, env)

test "precedence and parens"
    empty: {str: i64} = {}
    assert calculate("2 + 3 * 4", empty).must() == 14
    assert calculate("(2 + 3) * 4", empty).must() == 20
    assert calculate("2 * 3 * 4", empty).must() == 24

test "variables from the environment"
    env = {"x": 10, "y": 4}
    assert calculate("x * y + 2", env).must() == 42

test "errors reach the top"
    empty: {str: i64} = {}
    match calculate("2 +", empty)
        .ok _  => assert false
        .err e => assert e.code == "unexpected_end"
    match calculate("z + 1", empty)
        .ok _  => assert false
        .err e => assert e.code == "unknown_name"
    match calculate("(2 + 3", empty)
        .ok _  => assert false
        .err e => assert e.code == "unclosed_paren"

test "values are always copies"
    a = [1, 2, 3]
    b: [i64] @ a
    b @ b.push(4)
    assert a.len() == 3
    assert b.len() == 4

## To do

# Simplify the tree: `x * 1` becomes `x`, `x + 0` becomes `x`.
function simplify(e: Expr) -> Expr
    ???

## Program

function main()
    env = {"x": 10, "y": 4}

    cases = [
        "2 + 3 * 4"
        "(2 + 3) * 4"
        "x * y + 2"
        "z + 1"
        "2 +"
    ]

    for c in cases
        match calculate(c, env)
            .ok v  => print(c, " = ", v)
            .err e => print(c, " -> ", e.code, ": ", e.msg)
```

---

## How to start

Suggested first move: **Part 10, step 1 and 2 only** — a lexer with rigid indentation and a parser
for a minimal subset (integers, `=`/`@` locals, `if`, `while cond`, `function` entities, arithmetic),
emitting nothing yet, with a `--dump-ast` flag and a handful of golden tests. Then show the author
the AST for a small program and let them predict the generated C before you emit it.

Do not build the whole language before running anything.

*Status note: this project is underway. The scaffolding, spikes, spec v0 and measurement baseline
exist (M-day-zero); this document is kept current with every decided amendment — history is in git, open
questions are marked inline as `OPEN QUESTION (panel NNN)`, and the session records live in
`docs/panel/`.*