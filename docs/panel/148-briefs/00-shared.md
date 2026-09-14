# Panel 148 — shared brief

**The sitting**: M-marked-acquisition, 2026-09-14. Convened because the form
about to land has **surface**, and `spec/` is a panel trigger (CLAUDE.md § 4).

**Working tree is FROZEN** from now until the synthesis. Build in a copy:
`cp -r` the tree to your own scratch and `rm -rf target build` after. The seed
builds in ~3.4 s — `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`
— and rebuilding from `selfhost/` takes ~20 minutes and will kill you on the
watchdog.

Repository root: `/Users/joseph/Temp/heroes-lang`, commit `94b94bcd`.

---

## What is ALREADY decided, and is not yours

**Panel 147 ruled and the author ratified it in full on 2026-09-14.** A form
enters; **the obligation is marked where it is CREATED, on the acquiring call,
never on the type**; and **the compiler never picks the release call**. A
releaser keyed on the handle type is refused to design.md Part 6 with a
falsifier. Do not re-open any of that. If you think it is wrong, say so in one
line and answer the question anyway.

## What this sitting decides: the SURFACE, and only that

Two candidates. Their spec cost is measured and it does **not** separate them.

**A — a new contextual word, the mirror of `consumes`.**

```
    function sqlite3_prepare_v2(db: CDb, sql: cstr, length: i32,
                                @statement: CStmt acquires, @tail: cstr) -> i64
    function curl_easy_init() -> Curl acquires
```

Spec text added, after § 13's `consumes` sentence:

> `acquires` after a handle result or `@` out-parameter says the call BEGINS
> that handle's life: the program owes one `consumes` call for it, and a handle
> nobody consumes aborts when `main` returns, saying how many.

plus the production `CParam = [ "@" ] ident ":" Type [ "owned" ident ] [ "consumes" | "acquires" ] .`
**Measured +64 vendored tokens** (5863 → 5927). Draft at
`…/scratchpad/panel-148/spec-a.md`.

**C — no new word at all.** The obligation is read off the group's own
`consumes` function: if a group declares a consuming function for handle type
`T`, then every call in that group handing back a `T` acquires one.

> A group that declares a `consumes` function for a handle type puts every call
> that hands one back under the same rule: the program owes one such call per
> handle, and a handle nobody consumes aborts when `main` returns, saying how
> many.

**Measured +56 vendored tokens** (5863 → 5919). Draft at
`…/scratchpad/panel-148/spec-c.md`.

**A third nobody listed is exactly what this sitting wants to hear about.**

---

## The facts you are handed, all measured this week

- **`consumes` already exists and already carries the release half**:
  `examples/curl/main.hero` reads
  `function curl_easy_cleanup(handle: Curl consumes)`. Panel 145 landed it at
  **114 code lines**, refusing 2 of 17 functions in the shipped SQLite binding.
- **The mark's surface was BUILT and measured**: **+25 code lines across 7
  files**, and the compiler enumerated every missing site itself —
  `error[missing_fields] … acquires_result:` at **6 constructor sites across 5
  files**. `docs/measurements/032`.
- **Two files land over their ceilings**: `ast.hero` 505 → 508,
  `print/fmt.hero` 1156 → 1160.
- **CL-036 was reproduced live during that build**: with the formatter not
  taught, `@out: Db acquires` came back from `heroes fmt` as `@out: Db` — word
  gone, exit 0, no diagnostic.
- **The producers in the shipped tree are three**, and one is already marked:
  `sqlite3_open` (`@out: CDb`), `sqlite3_prepare_v2` (`@statement: CStmt`),
  `curl_easy_init` (`-> Curl`). `docs/measurements/031`.
- **Escape refusal is not a rival instrument**: it would refuse **2 of 2** of
  the reference binding's handle-producing wrappers, `opened` and `prepared`,
  with 7 and 6 call sites. `docs/measurements/032` § 1.
- **The corpus discards the releaser's own return code at 21 of 21 sites**, and
  `sqlite3_close` answers `SQLITE_BUSY` exactly when a statement leaked.
- **The urgency is smaller than it looks**: every failure path in
  `examples/ledger/main.hero` closes the database and calls `exit(1)`, so **no
  shipped program leaks a handle and then goes on running**.

## What your report must contain

A verdict on A and C, the design.md section it rests on, a cost or delta you
**measured** (say the word *estimate* if you could only estimate), a
falsifiable prediction, and any condition. If you could not run something,
write *unrun*. A negative claim names what you searched for.
