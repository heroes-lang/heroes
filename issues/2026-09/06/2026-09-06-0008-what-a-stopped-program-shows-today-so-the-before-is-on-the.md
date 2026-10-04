---
kind: task
area: emit
milestone: M-typed-inspection
filed: 2026-09-06
commit: none
github: none
---

- [ ] **M-typed-inspection** | what a stopped program shows today, so the "before" is on the record | `selfhost/emit/mangle.hero` · `selfhost/emit/body.hero` § prologue · `runtime/heroes_runtime.h`

    **Origin:** measured 2026-09-06 on this Mac, with lldb in batch mode over a
    hand-written program carrying one of each shape.

    **Half the promise already works and four things are broken, and nobody had
    run it.** Works: a breakpoint on a `.hero` line resolves and is hit with the
    source line printed, `bt` names Heroes frames at `.hero:line`
    (`h_dbg_total(...) at dbg.hero:19`), a local carries the author's own
    spelling behind an index (`h3_base = 7`), a `str` shows its text, a record
    shows its fields (`h0_p = (f_x = 3, f_y = 4)`).

    Broken: `p p` is `error: use of undeclared identifier 'p'`, so the author
    must know the mangling (`selfhost/emit/mangle.hero` § slot) and lldb's
    expression parser is C++; a `[T]` and a `{K: V}` are an opaque
    `HeroArrayHeader *`; a `T?` prints **both** arms including a garbage `err`
    half, under a hashed type name (`h_0opt_e201354`); and `frame variable`
    dumps **139** locals in one blessed emission
    (`tests/emission/run-adversarial-aggregate-overwrite.c`, 111 named and 28
    temporaries) against **247** in `syn/expr.hero::compared`, because §7 hoists
    every local to the prologue and `frame variable` has no name filter.

    Owed at the milestone: this table re-run as the "after", and the census that
    splits those 139 into temporaries, `$`-synthetic slots and real bindings,
    which is what decides whether the last step needs a slot table at all.

    **Where to look also:** `selfhost/ir/containers.hero` § SlotKind ·
    `runtime/heroes_runtime.h` § HeroArrayHeader, HeroDesc.
    **Why it matters:** the compiler is a 55,050-line Heroes program and the
    person learning from it cannot see a value in it.

    **Re-verified 2026-09-10: STILL OPEN, one number STALE, one UNSETTLED.**
    The compiler is **57,120** lines of Heroes, not 55,050 — which is what
    `docs/ROADMAP.md` says today, so the old figure survives only in
    `docs/records/journal/036-declared-freer.md:156`, where a record keeps what it measured.
    The **139 locals** split into 111 named and 28 temporaries is an lldb
    `frame variable` figure and is **UNSETTLED** without running lldb; declaration-line
    proxies give 103 to 151 depending on the pattern, which is why the census is this
    item's own step. And `syn/expr.hero::compared` is
    `examples/interpreter/syn/expr.hero:64`, not a compiler module — the item says so
    in full further down, and the short form is what misleads.
