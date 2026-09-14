# Panel 148 — llm-ergonomist

**Inputs used, and there were no others:** my brief, and `variant-1.md`,
`variant-2.md`, `variant-3.md` in the panel scratchpad. I did not open the
shared brief, the repository, the design documents or the compiler.

**Disclosure, because it affects how much this verdict is worth.** The git
status in my process environment carried a commit subject reading *"a releaser
keyed on the type goes to Part 6, and the form that enters is marked on the
call"*. I did not go looking for it and I could not refuse it; it tells me which
variant is current and which was refused. My reasoning below stands on the
variant text alone and the veto follows from the locality rule directly, but the
panel should discount the *convergence* accordingly rather than treat it as
independent confirmation.

**What differs.** Read side by side by eye — I have no diff tool in this seat,
so this is a careful reading and not a machine diff. Lines 1 to 374 are
identical in all three. The whole difference is one paragraph near the end of
§13 and one production:

- **Variant 3** has no handle-ownership rule at all. `consumes` ends a value's
  life and nobody owes anything.
- **Variant 1** adds: *"A group that declares a `consumes` function for a handle
  type puts every call that hands one back under the same rule: the program owes
  one such call per handle, and a handle nobody consumes aborts when `main`
  returns, saying how many."* Grammar unchanged.
- **Variant 2** adds: *"`acquires` after a handle result or `@` out-parameter
  says the call BEGINS that handle's life: the program owes one `consumes` call
  for it…"*, and `CParam` gains `[ "consumes" | "acquires" ]`.

A reader would notice 3 against 1-or-2 immediately (a paragraph is missing). A
reader would notice 1 against 2 only by reading §13's last third closely: both
paragraphs sit in the same place, are the same length, and end with the same
clause about aborting at `main`. The difference is *where the switch is*, and
that is invisible until you try to write a binding with a borrowing accessor in
it.

---

## verdict

- **variant 1 — VETO.** Non-local construct, on exactly the ground this seat
  holds a veto for: the meaning of a `function` line in an `extern` group is
  determined by a word on a *different* line, possibly in a different module.
- **variant 2 — approve, with two conditions** (the grammar does not admit the
  result form its own prose promises; the null-handle case is unstated).
- **variant 3 — object, not veto.** It is sound and local; it simply does not
  do the job. Every missed release is silent.

**Which I would hand a writer: variant 2.** It is the only one where a line
saying "this call gives you something you must give back" says so on the line.

**The sentence in each that carried most weight**

- V1: *"puts every call that hands one back under the same rule"* — "every call"
  is the whole problem, and "hands one back" does not say whether an
  out-parameter counts.
- V2: *"`acquires` after a handle result or `@` out-parameter says the call
  BEGINS that handle's life"* — the obligation is created by a mark on the call.
- V3: *"`consumes` after a parameter says the call ends that value's life"* —
  true, complete, and nobody is ever told they owe one.

---

## experiment

### Task 1 — the database group, written under each variant

Under **variant 3** (no obligation exists):

```
extern "sqlite3.h" link "sqlite3"
    constant SQLITE_OK: i64
    constant SQLITE_ROW: i64
    record Db tag sqlite3
    record Stmt tag sqlite3_stmt
    function sqlite3_open(filename: cstr, @ppDb: Db) -> i64
    function sqlite3_prepare_v2(db: Db, zSql: cstr, nByte: i32, @ppStmt: Stmt, @pzTail: cstr) -> i64
    function sqlite3_step(pStmt: Stmt) -> i64
    function sqlite3_column_int(pStmt: Stmt, iCol: i32) -> i64
    function sqlite3_finalize(@pStmt: Stmt consumes) -> i64
    function sqlite3_close(@pDb: Db consumes) -> i64
```

What I had to know: parameters at the header's own width (`nByte: i32`), a
result may be wider (`int` to `i64`), an out-parameter is `@`, a tagged empty
record is `T *`, and `consumes` plus `@` is how a release is spelled.

Under **variant 1** the group is **byte-identical to the above**. That is the
finding. Nothing in it says which calls acquire. The two `consumes` lines at the
bottom silently re-interpret the four lines above them.

Under **variant 2**, two words move onto the acquiring lines:

```
    function sqlite3_open(filename: cstr, @ppDb: Db acquires) -> i64
    function sqlite3_prepare_v2(db: Db, zSql: cstr, nByte: i32, @ppStmt: Stmt acquires, @pzTail: cstr) -> i64
```

The program, identical in shape under all three (only its *checking* differs):

```
function count_rows(db: Db, sql: str) -> i64?
    stmt: Stmt @ nullptr
    tail: cstr @ nullptr
    rc = sqlite3_prepare_v2(db: db, zSql: sql.cstr(), nByte: -1, ppStmt: @stmt, pzTail: @tail)
    if rc != SQLITE_OK
        _ = sqlite3_finalize(pStmt: @stmt)          # see hesitation 1 — stmt is NULL here
        return fail(code: "prepare", msg: f"sqlite3_prepare_v2 returned {rc}")
    rows: i64 @ 0
    while sqlite3_step(pStmt: stmt) == SQLITE_ROW
        rows @ rows + sqlite3_column_int(pStmt: stmt, iCol: 0)
    _ = sqlite3_finalize(pStmt: @stmt)
    return ok(rows)

function main()
    db: Db @ nullptr
    path = "app.db"
    rc = sqlite3_open(filename: path.cstr(), ppDb: @db)
    if rc != SQLITE_OK
        _ = sqlite3_close(pDb: @db)
        print("cannot open")
        return
    counted = count_rows(db: db, sql: "select n from t")
    _ = sqlite3_close(pDb: @db)                     # bind, release, THEN return
    match counted
        .ok n  => print(n)
        .err e => print(e.code)
```

Note the shape forced on me by variants 1 and 2 and *not* documented by either:
**`?` is unusable after an acquisition.** `expr?` leaves the block at once, so
it can never run the release. Every propagation has to become
bind-release-return, as `counted` is above. That is a real and teachable
pattern, and neither variant teaches it. Under variant 3 the same program is
what a careful writer produces and a hurried one does not, with no signal
either way.

### Task 2 — the function that hands back a statement the library still owns

Real shape, same header:
`sqlite3_stmt *sqlite3_next_stmt(sqlite3 *pDb, sqlite3_stmt *pStmt)` returns a
statement the *connection* owns. `sqlite3 *sqlite3_db_handle(sqlite3_stmt *)` is
the same shape one level up. Both must never be released by the caller.

**Variant 2 — yes, and it is a non-event.** Write the line; write no mark:

```
    function sqlite3_next_stmt(pDb: Db, pStmt: Stmt) -> Stmt
```

Absence of `acquires` is absence of obligation. The safe thing is the default
and the dangerous thing costs a word. A writer who wrongly *adds* `acquires`
here will go on to finalise a borrowed statement — a double free — but they had
to write a word whose definition ("BEGINS that handle's life") is plainly false
of this function, on the line that is wrong.

**Variant 1 — no. The variant does not let you, and the text does not settle
what to do instead.** The same line is what you must write, and because
`sqlite3_finalize(@pStmt: Stmt consumes)` exists in the group, *"every call that
hands one back"* puts this result under the rule. There is no opt-out anywhere
in the document. I looked for four escapes and each one fails from the text
alone:

1. **Satisfy the obligation.** Call `sqlite3_finalize` on the borrowed
   statement. The program now compiles, runs, and double-frees. **Silent memory
   corruption, arrived at by obeying the language.**
2. **Don't satisfy it.** A *correct* program aborts at `main` — "1 handle not
   consumed" — with no way to say the count is wrong. The language punishes the
   right program and the only cure it offers is (1).
3. **Declare a second handle type for the same C type.** Closed explicitly:
   *"two records may not name one tag."*
4. **Declare the result as `ptr`.** Then it takes no `Stmt` parameter (*"A
   parameter declared with it takes no other handle"*), so you need a shadow set
   of `ptr`-taking declarations and you have thrown away handle typing for the
   whole library. It compiles. It is silently weaker everywhere.

**Variant 3 — yes, trivially, because nothing is tracked.** Declaring the borrow
is free, and so is forgetting every release in the file.

### Task 3 — the most plausible mistake, and how loud it is

| | most plausible mistake | what happens |
|---|---|---|
| V1 | writing an accurate binding that contains one borrowing accessor | correct program **aborts**; the obvious repair is a **double free** (silent corruption) |
| V1 (second) | omitting `consumes` from the one closer | **the leak check silently turns off for the entire library**, and no acquisition line changes |
| V2 | forgetting `acquires` on one acquiring call | that one call goes unchecked — **silent leak**, localised, visible on the line that lacks the word |
| V2 (the release mistake itself) | forgetting to release on an early-exit path | **loud abort at `main`, saying how many** |
| V3 | forgetting to release on an early-exit path | **silent leak**, always |

**Ranked on Task 3 as worded — loudness of the forgotten release: 2 > 1 > 3.**
Variants 1 and 2 both catch it with the same abort; 2 is ahead because the
binding mistake that disables the check is one word governing one call, where in
1 it is one word governing every handle of that type in the group.

**Ranked overall, worst outcome first: 2 > 3 > 1.** Variant 3's failure is a
leak. Variant 1's failure is a double free reached by following the rules, and a
leak is not worse than corruption.

---

## hesitation_points

1. **A failed acquisition — does a null handle carry the obligation?** (V1 and
   V2, unstated in both.) `sqlite3_prepare_v2` writes NULL and returns an error.
   Is the obligation created anyway? If I guess "yes", I write
   `sqlite3_finalize(pStmt: @stmt)` on a NULL handle — harmless in sqlite,
   undefined behaviour in most libraries. If I guess "no" and the compiler says
   yes, a **correct program aborts at `main`**. Both wrong guesses compile. This
   is the single most costly gap and one sentence closes it.
2. **Does `acquires` fit where its prose says it fits?** (V2.) The prose says
   *"after a handle result or `@` out-parameter"*, but `Member` is
   `[ "->" Type [ "owned" ident ] ]` — `acquires` appears only in `CParam`. So
   `function sqlite3_backup_init(...) -> Backup acquires` is **not derivable
   from the printed grammar**, and the result form is the majority shape in C
   (`fopen`, `curl_easy_init`, `cairo_create`, `sqlite3_backup_init`). Wrong
   guess is a parse error — loud, and needless. Variant 1 needs no grammar
   change here, and this is the one place it is genuinely ahead.
3. **Does an out-parameter "hand one back"?** (V1 only.) *"every call that hands
   one back"* most naturally means *returns*. Under the narrow reading
   `sqlite3_open` and `sqlite3_prepare_v2` create **no** obligation at all and
   the feature does nothing for the commonest acquisition shape in C — silently.
   Under the broad reading it does. Nothing in the text chooses. Variant 2 has
   no such question: the mark is written on the out-parameter.
4. **Is `@` required with `consumes`?** (all three.) *"mark the parameter `@` and
   the value does not survive the call"* reads as consequence, not requirement,
   and the grammar makes `@` optional. If `consumes` without `@` leaves the
   value alive, a use-after-free compiles.
5. **Is use-after-consume an error?** (all three.) *"passing one the function
   borrowed is an error"* is the only sentence nearby and I could not parse it
   with confidence. If a second `sqlite3_close(pDb: @db)` merely compiles, the
   double free is silent.
6. **`.cstr()` on a literal.** *"A lend and a lease name stand only as an
   argument of a call"* — I could not tell whether "name" is load-bearing, so I
   hoisted `path = "app.db"`. Wrong guess is a compile error. Cheap.
7. **Named arguments in extern calls.** Two `cstr` parameters in
   `sqlite3_prepare_v2` make names mandatory if §9 reaches `extern`. I named
   everything. Wrong guess is a compile error. Cheap.

Items 4 to 7 are identical in all three variants and do not discriminate; they
are reported because they are real and 4 and 5 are corruption-shaped.

---

## argument

Variant 1 puts the switch off the line. `function sqlite3_next_stmt(...) -> Stmt`
means "you owe a release" or "you owe nothing" depending on a `consumes` keyword
elsewhere in the group — possibly in another module. That is the locality rule's
exact target, so I veto it. Worse, its default is obligation-on, so an accurate
binding of any real C library aborts a correct program, and the repair the
language steers you to is a double free. Variant 2 writes the mark on the call
it governs, defaults to no obligation, and makes the dangerous thing cost a
word you must consciously type. Variant 3 is safe and inert: every missed
release is silent. Ship 2, with the result-position grammar fixed and the null
handle settled.

*(119 words)*

---

## prediction

Checkable when the harness next runs, on binding tasks for a real C library:

1. **Task 1 first-try compile rate: no separation.** Variants 1, 2 and 3 differ
   by at most a few points on the plain open-prepare-step-finalise-close group,
   because `acquires` costs one word on a line the writer is already writing. If
   variant 2 measures materially *worse* on Task 1, my read is wrong.
2. **Task 2 silent-error rate is where they separate.** Given a library whose
   binding includes one borrowing accessor, variant 1 yields, in at least one
   of three attempts, a program containing a release call on a borrowed handle
   (double free) or a spurious "handle not consumed" abort in an otherwise
   correct program; variant 2 yields neither in three attempts.
3. **Variant 2's grammar gap bites now, not later.** Feed the compiler
   `function sqlite3_backup_init(pDest: Db, zDestName: cstr, pSource: Db, zSourceName: cstr) -> Backup acquires`.
   I predict either a parse error (the printed grammar is the real one, so the
   prose over-promises) or acceptance (the printed grammar is wrong). One of the
   two texts is false today.
4. **The null handle.** A program that lets `sqlite3_prepare_v2` fail and returns
   early without finalising aborts at `main` under variant 2. If it does not, the
   obligation is conditional and the spec does not say so.
5. **Variant 3 leaks on the error path** in a majority of attempts at any task
   whose brief mentions early exit, with zero diagnostics.

---

## condition

- **The veto on variant 1 lifts** if the text gains a way to say "this call
  hands back something you do not own", written **on that call's own line**, and
  a sentence saying whether an out-parameter counts as handing one back. At that
  point variant 1 is variant 2 with the default inverted, and the choice between
  them becomes a measurement: count, over N binding tasks, (a) how often a writer
  omits `acquires` under 2 and leaks, against (b) how often a writer under 1
  releases a borrowed handle or ships a program that aborts while correct. If
  (a) greatly exceeds (b) **and** none of (a) becomes corruption, I would revisit
  the ranking — but not the locality objection, and the cheaper fix for (a) is
  below.
- **My approval of variant 2 is conditional on two sentences.** (i) `Member`
  must admit `[ "->" Type [ "owned" ident ] [ "acquires" ] ]`, or the prose must
  stop promising the result form. (ii) One sentence on the null handle: *"a
  handle that is `nullptr` when the call returns carries no obligation"* — or the
  converse, stated.
- **Recovering variant 1's automation without its non-locality, if the panel
  wants it:** make it a *diagnostic*, never a change of meaning. A group that
  declares a `consumes` parameter of handle type `T` and no call marked
  `acquires` producing `T` is a compile error naming both lines. The unmarked
  line still means exactly what it says; the group is merely refused as
  incomplete. That closes variant 2's one silent failure — the forgotten
  `acquires` — at the cost of no locality at all.
