# Measurement 009 — self-hosting readiness: what the lexer port finds

**Status: closed with M-selfhost-probe, 2026-08-15.** The acceptance holds:
the ported lexer passes the translated Rust lexer suite (55 test blocks green
under `heroes test selfhost/lexer.hero`; 33 of the 34 Rust tests translated,
the untranslatable one named in gap 10 with its cause), the gap list below
stands under the milestone's rule, and the fifteenth closure-list row is
priced (§ hero_spawn below). Speed, measured not asserted: the ported lexer
lexes `examples/calculator/whole.hero` — 2,571 tokens — in **0.04 s** user
time as a compiled binary. This document accumulates
the gap list as the port advances, one entry per finding, under the milestone's
own rule (docs/ROADMAP.md § M-selfhost-probe):

> Every form the probe proposes arrives with three things — the Heroes code
> that does the same job *without* it, that code's line count, and the form's
> own spec cost quoted from `heroes measure` — and a form whose workaround
> compiles is a Part 7 deferral by default, overturned only by §1.0
> compiler-need (nothing in Heroes expresses the case at all) or a measured
> Part 11 effect.

The ROADMAP names this file `004-selfhost-readiness.md`; 004 was taken by
`error-codes` before this milestone opened, so the number is 009 and the
ROADMAP's pointer is corrected with this milestone's close.

**The subject moved under the probe before it started.** The ROADMAP's premise
(983 non-test lines, `as_bytes()` at seven sites, "the closure list has no form
that replaces byte access") describes the lexer of 2026-08-12. Today the lexer
is **1,643 non-test lines** across 9 files (M-literal-bases and M-sized-integers
grew it), and the byte wall the premise feared **no longer exists**: `s[i]`
yields a `u8` (spec § Strings), a character literal is an integer that takes
its width from context, and `examples/calculator/lex.hero` already scans bytes
this way. The probe's findings so far are accordingly smaller than the wall
that was predicted — which is itself the measurement.

## Ported so far, tests green under `heroes test selfhost/<file>.hero`

| selfhost file | ports | Rust lines | Heroes lines | tests |
|---|---|---|---|---|
| `token.hero` | `lexer/token.rs` | 194 | 193 | 3 |
| `keywords.hero` | `lexer/keywords.rs` | 87 | 116 | 3 |
| `escape.hero` | `lexer/escape.rs` (the table + decoder) | 100 of 168 | 148 | 4 |
| `digits.hero` | `lexer/digits.rs` (minus the two diagnostic builders) | 155 of 207 | 197 | 4 |
| `diag.hero` | the Diagnostic/Fix slice of `diagnostics.rs` | — | 50 | 1 |
| `state.hero` | LexState half of `lexer/mod.rs` | ~90 | 119 | 3 |
| `layout.hero` | `lexer/layout.rs` | 150 | 168 | 5 |
| `literals.hero` | `lexer/literals.rs` + the escape validator | 130 + 68 | 211 | 5 |
| `bytes.hero` | std's ASCII classes | — | 31 | 1 |
| `number.hero` | `lexer/number.rs` | 296 | 273 | 6 |
| `scan.hero` | `lexer/scan.rs` | 199 | 262 | 6 |
| `lexer.hero` | `lex_one` of `lexer/mod.rs` | ~60 | 106 | 7 |

**The lexer is ported end-to-end: 59 tests green** (48 at the milestone's close,
+2 from the render-in-base port and +9 from panel 066's landing), **and the
cross-check holds** —
`heroes lex --dump-tokens` (the Rust lexer) and the port's `kind_line` produce
the same stream for the same program, terminators, indent and dedent included.
Not yet done: the faithful translation of `lexer/tests/` (843 lines — many
behaviors already covered above, but the acceptance names that suite), and the
multi-file `lex` wrapper, which needs the Source record and is measured when a
second file needs it.

## Gap list — forms the port wanted

Every entry so far is **workaround-compiles**: nothing yet qualifies for the
closure list under §1.0. No proposal leaves this file before the milestone
closes.

1. **No `i128` — and no need for one.** `digits.rs::decode_wide` decodes into
   i128 because no 64-bit type spans u64's top and i8's bottom. The port
   decodes into `u64`, which holds every literal a program can write (literals
   are unsigned by construction; `-1` is unary minus applied to `1`), and the
   signed lower bound moves to the checker port, where negation lives.
   Workaround: 0 extra lines — a different type choice, not a detour.
   **Deferral by default.**

2. **Overflow aborts; a decoder wants it to fail politely.** Rust's
   `from_str_radix` returns `Err` on overflow; Heroes aborts on the wrapped
   multiply. Workaround: guard each accumulation step
   (`value > (U64_MAX - d) / radix`), +2 lines. The alternative form would be
   checked arithmetic (`mul_checked -> u64?`); nothing on the closure list
   needs it and the guard is idiomatic. **Deferral by default.**

3. **No `Option<T>`.** Every `Option` in the Rust lexer ports as `T?` with a
   named miss code (`not_a_keyword`, `not_foreign`, `no_base_marker`), and
   both callers match on the miss anyway. Workaround: 1 constant per code.
   The distinction Rust draws between `Option` and `Result` collapses into
   `T?` without loss *in this codebase*, because a miss always has one cause.
   **No form wanted at all.**

4. **No tuples.** `split_base` returns `(Base, &str)` in Rust; the port
   returns a two-field record (`Split { base, digits }`), +3 lines, self-
   documenting. **Deferral by default** (Part 6 already refuses tuples).

5. **No byte-to-str conversion.** `escape.rs` maps an escape to its byte
   (`Option<u8>`); the port's one table returns the decoded one-character
   *string* instead, because building a `str` from a computed `u8` has no
   spelling. Workaround: return `"\n"` instead of `10` — 0 extra lines here.
   The place this could bite for real is `scan.rs`'s unexpected-character
   path (slicing the offending char out of the source text serves there) and
   any future `to_str`-of-byte want. **Watch, not want, so far.**

6. **No ASCII case fold.** `canonical_int` is `to_ascii_lowercase` in Rust;
   the port maps the six hex digits by hand through a 7-arm `match` in a
   fold, +9 lines. A general `lower()` built-in is exactly what §1.11 says
   comes from C when a program needs it; the lexer needs six letters.
   **Deferral by default.**

7. **`\r` has no spelling** — **CLOSED 2026-08-16, panel 066**: it has one.
   The workaround (`constant CR: u8 = 13`) was right about the *scanner* and
   the constant survives, now spelled `'\r'`; what it could not do was let a
   program **write** the byte, which is where the sitting found the real cost.
   The frozen set met its own compiler and **did not** hold — but it broke on
   the program side, not the compiler side, and `\0`/`\xNN`/`\u{…}`/octal
   stay frozen on the interior-NUL ground, which byte 13 cannot reach.

8. **Arrays have no `pop`.** The saturating bracket-pop becomes a `slice` to
   len-1 behind a length test, +1 line, the emptiness check now visible at
   the site. **Deferral by default.**

9. **`push` cannot be redeclared** (built-in names are reserved), so Rust's
   `LexState::push` is `emit` in the port — arguably the better name.
   **No form wanted.**

10. **`\r`'s missing spelling is a TESTABILITY wall** — **CLOSED 2026-08-16,
    panel 066, and this entry carried a false premise that must not be
    repeated.** What it said: *"the single Rust lexer test the port cannot
    translate today… the Rust side writes `\r` freely."* **No such Rust unit
    test exists, or ever did** — the spec-warden verified it by grep and git
    history in the sitting this entry provoked, and the compiler-engineer
    reached the same result independently. `stray_carriage_return`'s one
    firing test is the golden `tests/golden/check/stray-carriage-return.hero`
    (a raw CR **between tokens**), which both compilers share, so unit-level
    parity was already exact and the wall was one file high, not two. The
    premise traveled from here into `selfhost/scan.hero` and from there into
    panel 066's own brief — three files in one day, CLAUDE.md §11's class at
    speed. **What was true**: the port could not write the test, and now
    does (`selfhost/scan.hero`, both directions — the stray one and the
    tolerated `\r\n`, which was untested on *either* side). The entry stays
    rather than being deleted, because a corrected record teaches what a
    deleted one cannot (§14).

11. **No statement no-op.** A `match` arm that should do nothing has no
    spelling: `_ = 0` in arm position is `declaration_in_arm` (correctly — the
    binding would be unreadable). The port's shape is to make the match yield
    a value both arms produce (`d.fixes @ match … .guidance => d.fixes`),
    which reads better than a no-op would have. **No form wanted** — the
    refusal forced the better shape.

12. **Tuple patterns.** Rust's `punct` matches `(b0, Some(b1), Some(b2))` in
    one table; the port is an if/else ladder in the same order, ~40 lines for
    30 operators, with the longest-match rule carried by ordering alone.
    Workaround compiles; the ladder is more verbose and no less clear.
    **Deferral by default** (Part 6 refuses tuples; the case count here does
    not reopen it).

## The second file — maps, recursive variants, generics (2026-08-16)

The ROADMAP owed this before M-selfhost-port opened, and for the reason it
stated: *"the lexer has zero `BTreeMap` and zero closures, so these findings
are a lower bound."* The file is `resolve/resolved.rs` → `selfhost/resolved.hero`
(7 test blocks), chosen because it is map-bearing **and** carries panel 065's
`// ORDER:` marks, so the port's sort obligation is tested rather than assumed.

**It found what the lexer could not, and the finding is a defect rather than a
missing form.**

13. **A record IS a usable map key.** Rust keys `top` and `module_uses` by
    `(String, String)`; Heroes has no tuples, and a two-field record serves —
    `==` is structural on records, so insert, read and miss all behave.
    Measured. **No form wanted.**

14. **`sort` cannot order records, and the spec's own map-walking idiom is
    therefore unavailable for a compound key.** `<` on a record is
    `bad_operand`; `sort` on `[Record]` **type-checks and then dies in the
    emitter** — `unsupported[builtin]`, exit 1, *"no change to this file will
    fix this"*. But `spec:85` prescribes exactly that idiom: *"`keys(m) -> [K]`
    gives the keys, so `for k in sort(keys(m))` walks in order"*, and `sort` is
    listed among the built-ins with no stated element restriction. So a program
    the spec tells you to write type-checks and cannot be built — CLAUDE.md
    §12's class, where the spec and the compiler disagree and one of them has
    the bug. **This is not a form the port wants; it is a disagreement the
    project's own precedence rule says must be settled**, and it is queued for
    a panel rather than decided here (three shapes are on the table: a
    lexicographic order on records, a checker-level type error in place of the
    emitter's `unsupported`, or a `sort_by` taking a comparison).
    **Workaround, and it is a good one**: key by module then name — two levels
    of `str`, both sortable, panel 065's rule satisfiable at both. The port's
    table is `{str: {str: i64}}`, and `names_in` becomes one map lookup plus
    one sort instead of a filter over every key in the program, which is
    arguably the better port. Cost: one level of nesting, ~8 lines across two
    writer functions.

15. **No set type.** `BTreeSet<String>` ports as `[str]` with a linear
    `contains`, +4 lines — and an array **sorts**, which a set of records would
    not, so the workaround for gap 14 arrives here for free. **Deferral by
    default.**

16. **`?` chains inside an expression.** `r.top[module]?[name]` — propagate the
    outer map's miss, then index the inner one — compiles and reads well. The
    port needed it on its first map lookup. **Confirmed, no gap.**

**What the second file did NOT find**: no generics were wanted (the Rust file
has none), and the recursive-variant question stays open — `Ref` and
`LocalKind` are flat variants with payloads, which port unchanged. A file with
a genuinely recursive tree (`syntax/`'s `Expr`) is the next measurement, and it
is the port's own next step rather than an owed detour.

## The third file — the recursive tree, and a compiler defect (2026-08-16)

`syntax/ast/exprs.rs` → `selfhost/ast.hero`, 6 test blocks. It closes the last
measurement question the second file left open, and the answer is a negative:

17. **The recursive tree costs nothing, because it was never recursive.**
    Expressions and statements live in **arenas** and link by index — the Rust
    file's own first sentence, decided for Rust's reasons. That is exactly what
    design.md §4.10 asks of a Heroes program (a value holds its fields by value
    and may contain itself only through `[T]` or `{K: V}`), so an arena
    satisfies the rule by never needing it. **Zero forms wanted**; the port is a
    transcription, and a tree walk over it is an ordinary recursive function
    that copies nothing but an `i64`.

18. **`case` is a foreign word, and the compiler's own AST wanted it.** The
    registry that exists to catch a model reaching for `switch`/`case` refuses
    the name Rust gives the variant-case node. Renamed `variant_case`, +0 lines,
    arguably clearer. **No form wanted** — but worth the record: the thesis
    machinery and the compiler's own vocabulary can collide, and the collision
    is cheap.

19. **`_` forbidden on a variant is a real cost, and it is the cost that buys
    the guarantee.** A depth walk that cares about five shapes must still name
    the other sixteen — four extra lines. The day a twenty-second `ExprKind`
    arrives, that walk stops compiling instead of quietly answering 1, which is
    the same rule that made the Rust lexer's `nullptr` defect inexpressible
    (gap 11's entry). **Deferral is not even the question**: this is the
    language working.

**And the port found a compiler defect that no test in the tree could reach.**
A variant case carrying a `Record?` that the program never **constructs** made
`==` on that variant emit C naming a descriptor nothing defined — exit 2, the
compiler blaming itself for a correct program. Cause: `emit/descriptors.rs`
expanded container payloads in a pass that sees only types the *program*
reaches, and pushed `T?` **field** types after that pass had run; a
never-constructed case is reached by nothing, yet its `eq` arm is generated
anyway. The worklist now expands as it drains
(`tests/golden/run/fixedbugs-option-payload-descriptor`, symptom/cause/fix in
the case itself, §9), and `generated` moved to its own file under §11's
ceiling — the seam `descriptors.rs` had already named.

**This is the port paying for itself before it is finished**: the shape that
provoked it — a declared-but-unbuilt case with an optional payload — is
ordinary in a compiler's own AST and absent from every program written to date.

## The fourth file — the parser's cursor (2026-08-16)

`syntax/cursor.rs` + `syntax/describe.rs` → `selfhost/cursor.hero` +
`selfhost/describe.hero`, 10 test blocks. One finding, and it is about names:

20. **A method name becomes a global name, and shadowing is a compile error.**
    `c.kind()` and `c.span()` cost Rust nothing — a method lives on its type.
    Ported as free functions they claim two of the most natural parameter names
    in a compiler, and the very next function wanting `span: token.Span` does
    not compile (`shadowed_binding`). The port renames them `current_kind` and
    `current_span`, **which is what they always meant**: neither is the
    *cursor's* kind or span, but the **current token's**. Cost: two renames,
    and the names got better. **No form wanted** — but the shape is worth
    knowing before the parser's ten files arrive, because Rust's method-heavy
    style will hit it again: prefer `<what>_of_<whose>` over the bare noun.

`take_docs` needed `line_col`, which the Rust side asks its `Source` for; the
port computes it from the text in 11 lines and stays free of a `Source` the
parser does not have yet. That defers the multi-file wrapper honestly rather
than blocking on it.

## The fifth file — the first real grammar (2026-08-16)

`syntax/types.rs` → `selfhost/parse_type.hero`, 7 test blocks, plus `TypeNode`
added to `selfhost/ast.hero`. Everything before it was a table, a record or a
cursor; **this is the first ported file that consumes tokens and produces tree
nodes**, so it is the first to exercise the cursor, the arena and the
diagnostics together — including recovery (`recover_past_closer` inside a
malformed function type) and a `certain` fix.

21. **`func` is a foreign word too** — the second time the registry has met the
    compiler's own vocabulary (`case` was the first, gap 18). `TypeKind::Func`
    is `function_type` in the port. **No form wanted**, and the pattern is now
    worth stating for the files ahead: Rust's compiler vocabulary overlaps the
    words other languages use as keywords, so expect the registry to fire on
    `case`, `func`, `enum`, `struct`, `class`, `switch`, `let`, `var`, `const`
    wherever the bootstrap uses one as a name. Each costs one rename and reads
    no worse.

Everything else ported unchanged. The three `@`/plain parameters that replace
Rust's `&mut Cursor, &mut Ast, &Source` make each call site say what it
changes — `parse_type(@c, @a, text)` announces the two mutations on the line,
which is §4.8's whole argument arriving in the compiler that implements it.

## The structural finding — the grammar cannot be split the way Rust splits it (2026-08-16)

The port stopped before writing the expression grammar to ask a question the
lexer never raised: `syntax/expr.rs` and `syntax/primary.rs` **call each other**,
and Heroes refuses module cycles.

22. **`module_cycle` fires on `use`, not on calls — measured both ways.** Two
    modules that call each other are refused (`ty_a uses ty_b uses ty_a`), and
    so are two that merely name each other's **types** with no call at all. The
    rule is on the `use` edge, whatever it carries. So the port's module graph
    must be acyclic in a stronger sense than the Rust call graph is.

**The size of what that forces, measured over `crates/heroes/src/syntax/`
(20 files, call edges verified by reading every call site):**

| knot | files | Rust lines | over §11's ~300 |
|---|---|---|---|
| **A — the expression/statement knot** | `expr`, `primary`, `control`, `stmt`, `name_stmt` | **1025** | 3.4× |
| **B — the declaration knot** | `decl`, `data`, `externs`, `extern_members` | **736** | 2.5× |

Cycle A's loop is `expr → primary → control → stmt → name_stmt → expr`, with
shorter back-edges `expr ↔ primary` and `control ↔ stmt`. Cycle B's is
`decl ↔ data`, `decl ↔ extern_members`, and `decl → externs → extern_members →
decl`. Every other file in the directory is cycle-free and ports as its own
module: `cursor` (268), `types` (272), `members` (215), `uses` (113),
`describe` (86), the four `ast/` files, and `mod` itself.

**This is not a defect and nothing is blocked.** Mutual recursion *inside* one
module is free — declaration order carries no meaning (§4.2) — so each knot
ports as one Heroes module and the grammar works. What it costs is file size,
and that is a **CLAUDE.md §11 question rather than a language one**: the
~300-line ceiling and the language's own cycle rule cannot both be satisfied by
a recursive-descent parser.

**Panel 031 R6 chose this deliberately, and its argument still holds.** The
refusal was adopted because it is *"the reversible direction: relaxing later
breaks nothing, while tightening at M-separate-compilation — where the
per-module cache needs a topological order — breaks the port."* The port has now
arrived and supplies what that decision was waiting for: the cost is two long
files, not a wall. Relaxing the rule would buy a file layout and spend the
topological order M-separate-compilation needs, which is the trade 031 already
priced.

**The resolution the port proposes** (queued for the author in `DECIDE.md`,
because §11 is amended by author instruction and not by panel): the language
rule stands, and **§11 yields for a grammar knot with the seam named** — exactly
as it yielded for `toolchain.rs` at 400 lines on 2026-08-14, where *"the cut
that would take it under runs through the cache key … and a file split against
its own seam is harder to read than a long one."* Here the seam is not merely
awkward: **the language forbids it**, which is a stronger reason than the one
already accepted. What the port owes in exchange is the thing §11 actually
protects — a reader must be able to open the file and not drown — so each knot
gets a module doc that names its cycle, lists its entry points, and says which
function calls which.

## The first knot, written (2026-08-16)

`selfhost/grammar_expr.hero` — the expression half of knot A, as **one module**
under §11's amended rule, carrying the map the rule asks for: which Rust files
it replaces, the call ring, the three entry points, and what is not there yet.
10 test blocks, and they read real token streams and render the tree back
fully parenthesised, because **a precedence table is only testable through what
it builds**: `1 + 2 * 3` → `(1 + (2 * 3))`, `1 - 2 - 3` → `((1 - 2) - 3)`,
`a < b && c` → `((a < b) && c)`, and `x & 1 == 0` → `((x & 1) == 0)` — which is
the one place Heroes deliberately does **not** inherit C's order.

23. **`args` is a built-in, and it is the natural name for a call's argument
    list.** `args() -> [str]` is the process's arguments (spec § Built-ins), and
    built-in names are taken everywhere, so a local called `args` is
    `builtin_name_taken`. Renamed `arguments`. **Third instance of one pattern**
    (`case`, gap 18; `func`, gap 21) and the first from the *built-ins* rather
    than the foreign-word registry — so the pattern's real statement is: **the
    compiler's own vocabulary overlaps both of the language's reserved sets**,
    and a port hits it a few times per file. Each costs one rename.

Nothing else was wanted. `unary` recursing into itself, `binary` climbing by
power, the suffix chain, the two bracketed literals with newline-as-separator,
and both `certain`-fix repairs (`trailing_comma`, `misplaced_mutable_marker`)
ported unchanged.

## Language features the port exercised against their own compiler

- `TokenKind?` **as a record field** holds Rust's `Option<TokenKind>`
  (`last_significant`), with `.is_err()` where Rust asks `.is_some()` —
  the fallible type serves as the missing Option even in stored position.
- `_` **forbidden on variant matches made the bootstrap's own defect
  inexpressible**: Rust forgot `nullptr` in `is_line_ender`'s allow-list when
  the keyword arrived (layout.rs's own comment records it — silent for a
  line, loud on the next). The port's `is_line_ender` must name all 61 kinds,
  so a 62nd kind is a compile error on that function until somebody decides
  its enderness. The thesis, executed on the compiler itself.
- `needs_label` fired on the port's own `new_diagnostic(code:, message:)` —
  two same-typed parameters, named arguments forced, on the first call the
  port wrote.
- Multi-line record construction still separates **arguments by comma** —
  newline separation belongs to container literals only; the parser said so
  at the first attempt, with `expected_args_close`.

## Idioms that carried over without friction (the positive result)

- A 61-case variant and a 61-arm exhaustive `match` as a return expression.
- Contextual case construction (`.kw_if`, `ok(.binary)`) through `ok()` and
  `push()`.
- `match` on `str` with `|` joins and `_`.
- Structural `==` on records and variants (the `ForeignWord` triple).
- Nested-record mutation through `@` (`u.span.start @ 0`) with value
  semantics keeping the original intact.
- `u8` byte scanning: `s[i]`, char literals as `u8` in context, `b - '0'`
  arithmetic, `to_u64(...)/to_i64(...)` with `.must()` at provably-safe sites.

## hero_spawn — the fifteenth closure-list row, priced

After the archive, `heroes build` is a Heroes program and it invokes clang;
`int64_t system(const char *)` is `conflicting types for 'system'` (panel 030
R3's wall), so the row cannot be an ordinary `extern` and must be a runtime
entry point beside `hero_file_read` and `hero_args_at` in `hero_os.h`.

- **Language shape**: `spawn(cmd: str, args: [str]) -> i64?` — runs a
  program, waits, gives its exit code; fails only if it cannot start.
- **Spec cost, measured**: **+39 tokens** (3374 → 3413, headroom 722 → 683),
  by inserting the sentence into `spec/heroes-spec.md` § Files and the
  process and running `heroes measure`; the spec was restored untouched.
- **Runtime shape**: `int64_t hero_spawn(HeroStr cmd, HeroArray args,
  int64_t *status)` over `posix_spawn` + `waitpid`, estimated 40–60 lines in
  `runtime.c` — an estimate, marked as one; the measured number arrives when
  the row lands.
- **This is a price, not a landing.** The row enters design.md Part 5 and the
  spec through a panel when M-selfhost-port needs it (CLAUDE.md §4).

## Two questions this milestone was told to answer

- **`private` (Part 7 item 14)**: the ROADMAP's rule was *"a blockage there
  puts it on the closure list, a wish does not"*. The probe hit **no
  blockage** shaped like visibility — eleven modules, every cross-module read
  intended — so `private` stays Part 7 at its pre-fixed +18.
- **The second file**: owed, as the ROADMAP already said. The byte wall the
  probe was aimed at no longer exists, and every gap found is small — but the
  lexer has zero `BTreeMap` and zero closures, so this list is a **lower
  bound**. A file that exercises maps, recursive variants and generics
  (`types/` or `resolve/`-shaped) opens M-selfhost-port, and panel 065's
  `// ORDER:` marks say exactly where its sorts will be owed.

## What carries to M-selfhost-port

Everything this milestone was told to deliver is delivered; four things carry
forward with their reasons named:

- **The second file** (above) — it opens the port.
- **The multi-file `lex` wrapper**: needs the Source record (`source.rs`,
  not lexer code); measured when a second file needs it.
- **The two diagnostic builders of `digits.rs`** — **half closed 2026-08-15,
  post-ratification loop**: `render_in_base` is written and measured — **11
  lines, no new form**: the sixteen digits already exist as a string and
  `slice` picks one, so the "first real string-building want" needed no
  byte-to-str conversion after all (gap 5 downgraded from *watch* to
  *answered*). `int_out_of_range` (the no-annotation case, whose bounds are
  constants) ports with it, its note text byte-identical to Rust's and
  asserted down to the string. The width-aware twin still ports with the
  checker: it names an IntKind, which is checker vocabulary.
- ~~**The `stray_carriage_return` test**~~ — **closed 2026-08-16** by panel
  066 rather than carried: `\r` got its spelling, the test is written in
  `selfhost/scan.hero` (both directions), and the entry's false premise is
  corrected in gap 10 above.
