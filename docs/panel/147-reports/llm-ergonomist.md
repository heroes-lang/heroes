# Panel 147 — llm-ergonomist

Inputs: the three variants named in my brief, and nothing else. I did not open
the repository, and I do not know which variant is current (my guess, and its
tell, is at the end).

## verdict

| variant | what it adds | verdict |
|---|---|---|
| 1 | `cleanup <call>` statement (§5 grammar, §8 paragraph) | **object** |
| 2 | nothing — no scope-exit construct anywhere | **object** |
| 3 | `released <fn>` on a handle declaration (§13, grammar) | **approve**, conditional |

**I would hand a writer variant 3.** No veto. My reasons and the two sentences
variant 3 still owes are below.

## Where they differ (found, not told)

Variant 2 is the common base. Variant 1 = base + `"cleanup" Expression NEWLINE`
in §5's `Statement` and a four-line paragraph in §8. Variant 3 = base + a
four-line paragraph in §13 and `[ "released" ident ]` in the `Member`
production. Everything else is byte-identical as far as I read.

**Would a reader notice without being told? Only variant 3's, reliably.**
Variant 3 puts its addition in §13, the section you are already reading because
your problem is a C library, and it shows the syntax inline
(`record Stmt tag sqlite3_stmt released sqlite3_finalize`). Variant 1 puts its
addition in §8, *Control flow*, which a writer opening a database has no reason
to re-read, and **`cleanup` appears in no code block anywhere in the document**
— every other construct in the language is demonstrated at least once. A writer
who reads §13 top to bottom to solve Task 1 will finish variant 1 without
learning that `cleanup` exists.

---

## experiment

Shared FFI group, written identically under all three (variant 3 adds
`released`):

```
extern "sqlite3.h" link "sqlite3"
    constant SQLITE_OK: i64
    constant SQLITE_ROW: i64
    record Db tag sqlite3
    record Stmt tag sqlite3_stmt
    function sqlite3_open(path: cstr, @out: Db) -> i64
    function sqlite3_close(db: Db) -> i64
    function sqlite3_prepare_v2(db: Db, sql: cstr, n: i32, @out: Stmt, tail: ptr) -> i64
    function sqlite3_step(s: Stmt) -> i64
    function sqlite3_column_text(s: Stmt, col: i32) -> cstr
    function sqlite3_finalize(s: Stmt) -> i64
```

### Task 1 under variant 2 — six release sites for two resources

```
function names(path: str) -> [str]?
    db: Db @ nullptr
    if sqlite3_open(path.cstr(), @db) != SQLITE_OK
        _ = sqlite3_close(db)                                   # 1
        return fail(code: "open_failed", msg: f"cannot open {path}")

    stmt: Stmt @ nullptr
    if sqlite3_prepare_v2(db, "select name from t".cstr(), -1, @stmt, nullptr) != SQLITE_OK
        _ = sqlite3_close(db)                                   # 2
        return fail(code: "prepare_failed", msg: "cannot prepare")

    out: [str] @ []
    while sqlite3_step(stmt) == SQLITE_ROW
        match sqlite3_column_text(stmt, 0).validated()
            .ok s  => out @ out.push(s)
            .err e =>
                _ = sqlite3_finalize(stmt)                      # 3
                _ = sqlite3_close(db)                           # 4
                return fail(code: e.code, msg: e.msg)

    _ = sqlite3_finalize(stmt)                                  # 5
    _ = sqlite3_close(db)                                       # 6
    return ok(out)
```

**Six release calls for two resources, and the language checks none of them.**
The one-line version of the loop body —

```
        out @ out.push(sqlite3_column_text(stmt, 0).validated()?)
```

— is what §6 and §5 actively teach (*"`expr?` propagate the error to the
caller"*, *"A `_` never drops a `T?` … Answer the error."*). It is five lines
shorter, it is the program a model writes, and it leaks both handles. **Variant
2's own ergonomics manufacture the bug.**

### Task 1 under variant 1 — three release sites, one new trap

```
function names(path: str) -> [str]?
    db: Db @ nullptr
    if sqlite3_open(path.cstr(), @db) != SQLITE_OK
        _ = sqlite3_close(db)                       # unavoidable: cannot register
        return fail(code: "open_failed", msg: f"cannot open {path}")
    cleanup sqlite3_close(db)                       # AFTER the check, not before

    stmt: Stmt @ nullptr
    if sqlite3_prepare_v2(db, "select name from t".cstr(), -1, @stmt, nullptr) != SQLITE_OK
        return fail(code: "prepare_failed", msg: "cannot prepare")
    cleanup sqlite3_finalize(stmt)                  # AFTER the call that fills @stmt

    out: [str] @ []
    while sqlite3_step(stmt) == SQLITE_ROW
        out @ out.push(sqlite3_column_text(stmt, 0).validated()?)
    return ok(out)
```

The natural, Go-shaped version of the same code — **which compiles, and which
releases `NULL` every time**:

```
    db: Db @ nullptr
    cleanup sqlite3_close(db)                       # reads db HERE: nullptr
    if sqlite3_open(path.cstr(), @db) != SQLITE_OK  # fills db AFTERWARDS
        return fail(code: "open_failed", msg: "")
```

§8 warns about this in nine words — *"The call's arguments are read where it is
written, not where it runs"* — and §13's own example teaches the out-parameter
idiom (`function sqlite3_open(path: cstr, @out: Db) -> i64`) that springs it.
**The spec sets the trap in one section and disarms it in another.** Every habit
a writer brings (`defer f.Close()`, `with`, RAII) says *register at the
declaration*; in this language that is the wrong line, and being wrong is silent.

### Task 1 under variant 3 — zero release sites

```
    record Db tag sqlite3 released sqlite3_close
    record Stmt tag sqlite3_stmt released sqlite3_finalize
```

```
function names(path: str) -> [str]?
    db: Db @ nullptr
    if sqlite3_open(path.cstr(), @db) != SQLITE_OK
        return fail(code: "open_failed", msg: f"cannot open {path}")

    stmt: Stmt @ nullptr
    if sqlite3_prepare_v2(db, "select name from t".cstr(), -1, @stmt, nullptr) != SQLITE_OK
        return fail(code: "prepare_failed", msg: "cannot prepare")

    out: [str] @ []
    while sqlite3_step(stmt) == SQLITE_ROW
        out @ out.push(sqlite3_column_text(stmt, 0).validated()?)
    return ok(out)
```

**This is the naive body.** I wrote it without thinking about release once, and
it is correct. No registration to remember, no order to get right, no
"arguments read where written". The two `released` words went into a declaration
I was writing anyway.

**Release-call sites in a correct two-resource, three-failure-path program:
variant 2 = 6, variant 1 = 3, variant 3 = 0.** Anyone can check that number by
writing the program.

### The cost variant 3 charges, and it is the right kind

```
function open_db(path: str) -> Db?          # REFUSED under variant 3
    db: Db @ nullptr
    if sqlite3_open(path.cstr(), @db) != SQLITE_OK
        return fail(code: "open_failed", msg: "")
    return ok(db)                           # "may not be returned or stored"
```

You cannot factor out an opener for a released type. That is a real
expressiveness loss and it is a **compile error at the return, citing a rule
written in the declaration you wrote**. The workaround is available and is the
right one: mark `Stmt` (short-lived, the thing people forget) and leave `Db`
unmarked (long-lived, wants to be returned). `released` is opt-in **per type**,
which is the correct granularity, because lifetime discipline is a property of
the type and not of the call site.

---

## Task 2 — what happens to the statement when the marked line fails

First, in **all three variants** the shape as given may not compile at all:

```
    _ = finalized(@statement)
```

§5: *"A `_` never drops a `T?`: not `_ = e` … Answer the error."* If `finalized`
returns `()?` or `i64?` — which any honest wrapper over `sqlite3_finalize`
does — this line is a **compile error in every variant**. That refusal is
correct and it is the baseline's one existing guard: the language already
refuses to let you ignore a failing close. It does not, in variants 1 and 2,
refuse to let you skip one.

- **Variant 1 — the statement leaks, and the text settles it.** No `cleanup` is
  registered here; §8 lists *"a `?` that propagates"* among the exits a
  registered cleanup covers, so by contraposition an unregistered one covers
  nothing. `?` unwinds past `_ = finalized(@statement)`. Silent leak.
- **Variant 2 — the statement leaks, and the text settles it by exhaustion.**
  The document contains no scope-exit construct of any kind, and it says *"This
  document is the whole language."* Settled, if weakly: you conclude it by
  finding nothing rather than by reading something.
- **Variant 3 — the shape alone does not settle it, but the ambiguity cannot be
  silent.** It depends on one word in `Stmt`'s declaration, which may be in
  another file. Two cases, and they are distinguishable: if `Stmt` is **not**
  `released`, it leaks exactly as in 1 and 2; if it **is**, then the manual
  `finalized(@statement)` wrapper cannot compile (*"refuses your own call of
  it"*), so **this program existing and compiling proves `Stmt` is unreleased**.
  There is no third case where the code compiles and quietly means something
  else. Demerit: the refusal lands inside the wrapper's body, one file from the
  line that provoked it.

---

## Task 3 — most plausible mistake, and what it becomes

**Variant 2.** Most plausible mistake: *writing `?` on any fallible call between
acquire and release.* It is the most idiomatic construct in the language and the
spec pushes you to it twice. → **silent wrong answer** (correct rows, one leaked
statement per call). Runner-up: getting one of six hand-written release sites
wrong on one of four paths — also silent. Double-close on an overlapping path:
also possible, and that one is memory corruption.

**Variant 1.** Two, of different kinds. (a) *Not writing the `cleanup` at all* —
variant 2's mistake, undiminished, because the construct is opt-in per site,
lives in §8, and has no example. → **silent leak.** (b) *Registering before the
`@out` parameter is filled* — → **silent release of `nullptr`**, a leak produced
by the feature that exists to prevent leaks. (c) A `cleanup` alongside a manual
release left over from an earlier edit → **double free, undefined behaviour.**
Variant 1's text says nothing about double release.

**Variant 3.** Most plausible mistake: *returning a released handle from an
opener helper* → **compile error**, loud, local, self-explaining. Second:
*calling `sqlite3_finalize` yourself out of habit* → **compile error**
(*"refuses your own call of it"*), so the habit is caught rather than doubled.
Third: *forgetting the word `released`* → silent leak, identical to variant 2 —
but made **once per type, not once per use site**, so every function written
afterwards inherits the fix.

**Variant 3 is the only one of the three in which a double release is
impossible.** That is a robustness result, not an ergonomics one, and it is the
strongest single thing on this page.

**Ranking by this criterion alone: 3, then 1, then 2** — with the caveat under
`condition` that variant 1's lead over variant 2 is not secure.

---

## hesitation_points

Where I guessed, and what a wrong guess produces.

**Variant 1**

1. *"a `cleanup` may not appear at the top level of a function whose value it
   would follow."* **I cannot settle this sentence.** Reading A: no `cleanup` at
   the top level of any value-returning function — which deletes my whole Task 1
   program, since every function touching C returns `T?`, and §5's grammar has
   **no bare block statement**, so the only nesting escape is writing `if true`.
   Reading B: only a *trailing* `cleanup` is banned, because §8 says a block's
   value is its last expression. Reading C: a cleanup may not run after the
   return value is computed. A, B and C give three different programs. Wrong
   guess → **compile error** (loud, good) but a **stall**, and a stalled writer
   falls back to the hand-written variant-2 program.
2. `cleanup sqlite3_close(db)` discards an `i64`. Does §5's *"A line that
   computes a value must use it"* bite? You cannot write `cleanup _ = f(x)`:
   `_ = …` is a Statement and `cleanup` takes an Expression. If it bites,
   **`cleanup` cannot be used over any value-returning C function** — which is
   `fclose`, `sqlite3_close`, `sqlite3_finalize`, nearly all of them. I guessed
   it does not bite (~70%). Wrong guess → compile error.
3. Does a `cleanup` in the function body run when a `?` inside a nested `while`
   propagates? §8's list mixes two scales (`break`/`continue` leave inner
   blocks; `return`/`?` leave the function). I guessed yes (~85%). Wrong guess →
   **silent leak**.
4. Double release: unspecified. Wrong guess → **memory corruption**.

**Variant 3**

5. Is `released` called on a **null** handle? My Task 1 program leaves `db` as
   `nullptr` on the open-failure path. `sqlite3_finalize(NULL)` is harmless;
   another library's is not. **The text does not say.** Wrong guess → crash.
   This is variant 3's worst gap and one clause closes it.
6. `released` against `consumes`: if a released handle is passed to a `consumes`
   parameter, is it released again at scope exit? **Text does not say.** Wrong
   guess → **double free**. Variant 1 owes the same clause for `cleanup` plus
   `consumes`; variant 3's is more load-bearing because its release is automatic.
7. Must the released function still be declared in the group for `released` to
   name it? I declared it. Wrong guess → compile error.

**All three (shared, so they do not discriminate)**

8. `db: Db @ nullptr` to make a cell for an `@out` handle. §3 says `nullptr` is
   the null of `ptr` and `cstr`; §13 repairs it for handles. Took a re-read.
9. `sqlite3_column_text` returns `const unsigned char *`; I declared `cstr`.
   §13's width rule speaks of numbers, not of this. Wrong guess → compile error
   from clang, loud.
10. The `@tail: ptr` out-parameter: I declared it non-`@` and passed `nullptr`,
    to dodge §5's *"a write is not [a use], except through an `@` parameter"*,
    which I could not parse with confidence.

---

## argument

Variant 2 makes the idiomatic program the leaking program: `?` is what §5 and §6
teach, and every `?` between acquire and release leaks. Variant 1 cuts six
release sites to three and gets LIFO ordering free, but it adds a new silent
failure exactly where §13's own out-parameter example leads — register before
the cell is filled and you free `nullptr`, compiling — and its governing
sentence has three readings, one of which deletes the feature for every fallible
function. Variant 3 cuts release sites to zero, puts the fact on the type where
it is written once, converts the habitual manual call into a compile error, and
is the only variant under which double release cannot be written. Its cost is a
refusal, not a miscompile.

## prediction

Falsifiable when the harness next runs, on an "acquire two C resources, one
fails partway" task:

- **Release-call sites in a correct solution: 6 / 3 / 0** for variants 2 / 1 / 3.
  Checkable by writing the program once, without a model.
- **Variant 2: ≥ 50% of attempts exit some path without releasing**, and ≥ 1 in
  3 uses `?` between acquire and release.
- **Variant 1: ≥ 1 in 5 attempts registers `cleanup` before the `@out`
  parameter is filled** (a silent release of `nullptr`), and ≥ 1 in 5 stalls or
  errors on *"may not appear at the top level of a function whose value it would
  follow"*. **Variant 1's leak rate will not be less than half of variant 2's**,
  because the construct is opt-in, undemonstrated, and filed under *Control
  flow*.
- **Variant 3: leak rate for the marked type near zero**; its dominant failure
  becomes the loud *"a released handle may not be returned or stored"* on an
  opener helper, in ≥ 1 in 4 attempts.
- **Unanswerable questions a writer can list from the text alone: ≥ 3 for
  variant 1 (must-use, top-level restriction, double release), ≤ 2 for variant 3
  (null handle, `consumes`).** Checkable by asking a model to enumerate what it
  could not determine.

## condition

- **Variant 1 → object becomes veto** if *"may not appear at the top level of a
  function whose value it would follow"* means reading A or C. Under those
  readings `cleanup` is unusable in every function that touches C, and a
  construct that cannot be used where it is needed but compiles where it is not
  is worse than its absence. **Variant 1 → approve** only if all four of my
  hesitations get a sentence: the must-use question, the nested-`?` question,
  the top-level restriction restated unambiguously, and an out-parameter
  example showing the registration *after* the filling call.
- **Variant 3 → unconditional approve** on two added clauses: what happens when
  a released handle is null at scope exit, and whether `consumes` suppresses the
  release. Without them I approve with the gaps named.
- **All three equal** would be shown by a measured leak rate within noise across
  variants. I predict it will not be; the 6/3/0 site count is too coarse a
  difference to vanish.

## non-local constructs: no veto, and where I drew the line

I hold a veto on anything whose meaning cannot be determined from the line plus
the enclosing signature. Both additions touch it. Neither crosses it.

**`cleanup` (variant 1) — not vetoed, but it is the more non-local of the two.**
The `cleanup` line itself is local. The line that becomes non-local is the other
one: `return ok(out)` or `…validated()?`, whose meaning (*does this close the
database?*) requires holding a statement written earlier in the block. It is
**non-local per site**: the same `return` closes the database in one function and
leaks it in the function below, and nothing at the `return` distinguishes them.
I object rather than veto because the block is visible by indentation, and
because my sharper complaint is the register-before-fill trap, which is a silent
wrong program rather than a locality failure.

**`released` (variant 3) — not vetoed.** Strictly the line
`statement: Stmt @ prepared(db, "…")?` needs a word from `Stmt`'s declaration,
possibly in another file. But the fact is attached to a **type name written on
the line**, it is **one fact per type**, and it is **uniform at every use** —
the same class of fact as `record Db tag sqlite3` meaning `sqlite3 *`, which the
reader must already hold to write any of this. A qualified name (`db.Stmt`) even
says which file. A per-type fact reachable from a type name on the line is
inside the rule; a per-site fact written elsewhere in the function is the
borderline.

**What I would veto**, and this is the guardrail I want on the record: any
version of `released` that could be attached anywhere other than the type's own
declaration — at a use site, per function, per binding, or overridable by an
annotation. Then the same type would behave differently in different files and
the fact would stop being reachable from the type name. As written, variant 3
does not do this, and it must not be relaxed later to do it.

## Did a variant make a task harder?

Yes, and it is the finding a token count would miss. **Variant 1 made Task 1
harder than variant 2 did.** Under variant 2 I wrote a long, dull program with
no unanswered questions about the language — tedious, but every line was
certain. Under variant 1 I wrote a shorter program and accumulated four
questions I could not settle from the text. *Shorter to write, harder to be sure
of.* Variant 3 was the only one where the program got shorter **and** my
uncertainty went down: I wrote the naive body and my remaining two questions
(null handle, `consumes`) are about edges I had to go looking for, not about the
line I just wrote.

## My guess at which is current, and the tell

**Variant 2 is the current language; 1 and 3 are both proposals.** The tell is
not the content, it is the demonstration. Every construct in the base document
is shown in use at least once — `match`, `for`, `use`, `test`, `???`, the extern
group, even `Point::x`. `cleanup` appears in variant 1 exactly twice, in §5's
grammar and in one §8 paragraph, and **in zero code blocks**. A construct a
language grew up with gets demonstrated; a construct inserted into a spec gets
described. Variant 3 is also an insertion by the same test, but it shows its
syntax inline, and it lands in the section a reader with this problem is already
in.

That asymmetry is itself an ergonomics finding, independent of the semantics: on
the current text, a writer solving Task 1 under variant 1 would very likely never
find `cleanup` at all, and would write the variant-2 program.
