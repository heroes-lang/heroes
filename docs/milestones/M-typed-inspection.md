# M-typed-inspection — a stopped program shows Heroes values


**Scheduled by author instruction 2026-09-06** — § Who scheduled what carries the
instruction, the three choices the author took and the measurements that placed the
row. **No warrant**, deliberately: the §1.1 argument is available and unclaimed,
because the only instrument that could measure it is Part 11's metric 4, which has
never run, and a place in the table is not a warrant.

**The id is the sentence it retires.** `design.md:594` says *"No typed variable
inspection in v1"* and this file's own M-vscode-extension bullet says *"Typed
inspection is not in v1"*. Both clauses are dated to a v1 that was reached at
M-selfhost-fixpoint on 2026-08-18.

**Half of it already works, and a later reader should not re-derive that.** Measured
on this Mac 2026-09-06, with lldb in batch mode over a hand-written program: a
breakpoint on a `.hero` line resolves and is hit, the source line is printed with a
caret, `bt` names Heroes frames at `.hero:line` (`h_dbg_total(...) at dbg.hero:19`),
a local carries the author's own spelling behind an index (`h3_base = 7`), a `str`
shows its text, and a record shows its fields (`h0_p = (f_x = 3, f_y = 4)`). `-g` is
on every build and the object survives beside the `.c` so Darwin's DWARF resolves
(`selfhost/cli/flags.hero`, `selfhost/cli/toolchain.hero`) — both repaired at
M-selfhost-port, which is why the line half exists at all.

**What it delivers is the other half, and it is four named failures.** `p p` is
`error: use of undeclared identifier 'p'` — the C name is `h0_p` and lldb's
expression parser is C++, so the author must know the mangling to ask a question. A
`[T]` and a `{K: V}` are an opaque `HeroArrayHeader *` and nothing of the contents. A
`T?` prints **both** arms, including a garbage `err` half, under a hashed type name
(`h_0opt_e201354`). And the frame is flooded, because CLAUDE.md §7 hoists every local
to the prologue and `frame variable` has no name filter: **139** in one blessed
emission, 111 named and 28 temporaries, against the **247** in
`syn/expr.hero::compared` that M-qbe-backend's item already carries.

**The route was run rather than argued, and that is what makes this row cheap.**
Thirty lines of lldb Python read `len` out of the array header, resolved the `elem`
descriptor pointer to the symbol `hero_desc_str`, found the C type `HeroStr` and
printed `"ada"` and `"grace"` — no compiler change, no runtime change, memory reads
only. `nm` shows a user type links as `_h_desc_Room_desc`, so **the descriptor's own
symbol name is the type name the runtime does not carry**, which is the same
pointer-identity trick `runtime/parts/sort.c` already uses. A `name` field on
`HeroDesc` is therefore refused before anybody proposes it: it would cost
`HERO_RUNTIME_ABI` a bump to buy a string the linker is already holding.

**The first step is the instrument, because the claim has none.** `design.md:616` and
this file at `:452` both assert that a golden runs lldb in batch mode and asserts a
breakpoint on a `.hero` line is hit. That golden is
`archive/bootstrap-rs/heroes-cli/tests/golden.rs`, nothing has built that tree since
M-bootstrap-archive on 2026-08-19, `tests/harness/` has no lldb suite, and
`.github/workflows/ci.yml` installs lldb on the Linux leg for a test it never runs.
The suite comes back wider than it went away: the three guards the archived test
bought with failures (lldb wrote nothing on either stream · the breakpoint is pending
with no locations · the file, the line and `stop reason`), plus the **stepping** half
that never had a test at all. Its own falsifier is run once by hand and quoted —
`-g` deleted, the suite must go red — because a debugger suite that passes without
DWARF is a decoration. It also settles `docs/panel/085`'s B2 condition, which said in
its own words that the lldb class *"was not tried"*.

**The second step measures four premises before anything is designed on them**, and
one of them decides the shape of the last: of those 139 and 247 locals, how many are
`t<N>` temporaries, how many are `$`-synthetic slots the lowering invented, and how
many are bindings the author wrote. The IR already knows — `SlotKind` carries
`param_slot`/`local_slot`/`synthetic_slot` and `--dump-ir` prints the `$` — and the
distinction dies in the mangler, which drops the leading `$`. If the census says the
named locals are mostly synthetic, the frame filter is free; if it says they are real
bindings spread over a long function, a slot table beside the binary is what buys
scope, and that is a panel question rather than a decision taken here.

**Not in this milestone**: making `print(p)` work. A `to_str` derived over a record's
fields is M-reflection-verdict's own question in its own words, and taking it here
would be a decision made at a sitting convened about something else. What this
milestone does is make that sitting's exhibit: after it, the author can **see** a
`[Token]` in a stopped frame and still cannot `print` one. Also not here: calling a
generated renderer inside the stopped process. It is the obvious optimisation and it
is unrun and unsafe on its face — the moment a debugger earns its keep is the moment
the program has crashed, and allocating on that process's heap, on whatever thread
lldb picks, after M-isolated-threads gave every thread its own, is a debugger that
mutates what it came to look at.

**What it honestly does not deliver is Windows.** `lldb.exe` exits `0xC0000135`
before running a command, which `.github/workflows/ci.yml` records and CI reports
rather than gates. The `#line` mapping itself is covered on all three legs by the
suite that reads emitted C and needs no debugger. What Windows gets here is a
`heroes doctor` row that says the tool is missing, because a named absence beats a
silence, and the way back in is named as a question rather than a plan:
`llvm-pdbutil` reads the CodeView in the 7.7 MB `.pdb` that `-g` already writes and,
unlike `lldb.exe`, has no reason to link Python.

**Why here.** After M-panic-location because they are two halves of one sentence and
this is the expensive half: a program that stops says *where*, then says *what it was
holding*, and most stops never reach a debugger once the panic names its function.
Before M-generated-programs for that row's own reason one order up — its programs are
the only ones in this project that nobody wrote, so reading the source, the ordinary
triage instrument, helps least exactly there. Before M-lsp-server and
M-vscode-extension so that the extension's variables pane is right on the day it
ships. And independent of M-qbe-backend: the flood is filtered debugger-side over a
convention the mangler already enforces, so this row does not wait on one that sits
behind the books.

*******************************************************************************
**OPEN: 3**

- [ ] **M-typed-inspection** | the mechanism is the opening sitting's first question | `CLAUDE.md` §10 · `selfhost/cli/table.hero` § run_flags · `docs/ROADMAP.md` § M-vscode-extension

    **Origin:** measured 2026-09-06, the cheap route RUN rather than argued.
    §10's stopping rule is asked before it.

    **Thirty lines of lldb Python turned an opaque array into its elements with
    no compiler change and no runtime change.** It read `len` out of the header,
    resolved the `elem` descriptor pointer to the symbol `hero_desc_str`, found
    the C type `HeroStr` and printed `"ada"` and `"grace"`; `nm` shows a user
    type links as `_h_desc_Room_desc`, so **the descriptor's own symbol name is
    the type name `HeroDesc` does not carry**, which is `runtime/parts/sort.c`'s
    pointer-identity trick one level up. So a `name` field on `HeroDesc` is
    refused before it is proposed: `HERO_RUNTIME_ABI` +1 to buy a string the
    linker already holds.

    What the sitting must rule: whether CLAUDE.md §10's *"never a script"*
    forbids a formatter the BUILD emits and nobody types (Rust ships
    `rust-lldb`, a wrapper script, which is exactly what §10 refuses); whether
    the surface is `heroes run --debug` (a flag: same input, same question,
    different how, and at `-O0` because `run` defaults to `-O2` and the locals
    are gone there), a new verb (which §10 admits only with a proven overload),
    or **nothing** (the conservative reading, refused on §12: two invocations
    cannot guarantee the formatter and the binary came from one build, and a
    stale formatter shows the wrong variable's name at exit 0).

    Not this milestone's: calling a generated `to_str` inside the stopped
    process, which allocates on that process's heap on whatever thread lldb
    picks, after M-isolated-threads gave every thread its own.

    **Where to look also:** `design.md:594`.
    **Why it matters:** the mechanism decides whether the compiler changes at
    all, and three of the four routes leave it untouched.

    **Re-verified 2026-09-10: STILL OPEN, no sitting held, no surface landed.**
    `grep -rn '"--debug"' selfhost/` is empty and `selfhost/cli/table.hero:90-97`'s
    `run_flags` are unchanged. One pointer moved: *"No typed variable inspection in
    v1"* is `design.md:613`, not `:594`.

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
    `docs/journal/036-declared-freer.md:156`, where a record keeps what it measured.
    The **139 locals** split into 111 named and 28 temporaries is an lldb
    `frame variable` figure and is **UNSETTLED** without running lldb; declaration-line
    proxies give 103 to 151 depending on the pattern, which is why the census is this
    item's own step. And `syn/expr.hero::compared` is
    `examples/interpreter/syn/expr.hero:64`, not a compiler module — the item says so
    in full further down, and the short form is what misleads.

- [ ] **M-typed-inspection** | the claim with no live test, and the harness has no lldb row | `.github/workflows/ci.yml` · `tests/harness/main.hero` · `docs/panel/085`

    **Origin:** measured 2026-09-06.

    **`design.md:616` and `docs/ROADMAP.md:452` both assert that a golden runs
    lldb in batch mode and asserts a breakpoint on a `.hero` line is hit, and
    nothing has run it since 2026-08-19.** The golden is
    `archive/bootstrap-rs/heroes-cli/tests/golden.rs`, which M-bootstrap-archive
    left where nothing builds it; `.github/workflows/ci.yml` installs lldb on
    the Linux leg for it, describes it as executing in four comments, and runs
    no cargo at all; `tests/harness/` has no lldb suite.

    Owed: one harness suite driving lldb through `shell.run`'s argv list and
    watchdog, carrying the three guards the archived test bought with failures
    (lldb wrote nothing on either stream · the breakpoint is pending with no
    locations · the file, the line and `stop reason`) plus the **stepping** half
    that never had a test, and **its own falsifier run once by hand and quoted
    in the commit body** — `-g` deleted from `selfhost/cli/flags.hero`, the
    suite must go red. It also settles `docs/panel/085` B2's own condition,
    which said the lldb class *"was not tried"*. Windows is stated rather than
    silent: `lldb.exe` exits `0xC0000135` before running a command, so what that
    leg gets is a `heroes doctor` row naming the absence.

    **Where to look also:** `CLAUDE.md` §9.
    **Why it matters:** a debugger suite that passes without DWARF is a
    decoration, and a promise with no instrument is how this one went eighteen
    days unnoticed.

    **Re-verified 2026-09-10: STILL OPEN, and half of its cited claim never
    existed.** Still open: `tests/harness/` holds 20 suites and no lldb suite, and CI
    still installs lldb on the Linux leg (`.github/workflows/ci.yml:271`) for a test
    it never runs. **The false half**: *"`design.md:616` and `docs/ROADMAP.md:452`
    both assert that a golden runs lldb in batch mode."* `grep -n "batch mode"
    design.md` is **empty** — design.md never claimed it, its lldb sentences being
    `:614`, `:635-636`, `:664` and `:2886` — and the ROADMAP's claim is at **`:500`**.
    The same stale pair stood in `docs/ROADMAP.md:1737` and in
    `tests/harness/suite_records.hero`, and **the second was repaired on 2026-09-10**
    in the commit that carries this verification.

*******************************************************************************
