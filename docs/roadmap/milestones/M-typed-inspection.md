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
shows its text, and a record shows its fields (`h0_p = (f_x = 3, f_y = 4)`). `-g` was
on every build and the object survives beside the `.c` so Darwin's DWARF resolves
(`selfhost/cli/flags.hero`, `selfhost/cli/toolchain.hero`) — both repaired at
M-selfhost-port, which is why the line half exists at all. **Since panel 197
(2026-10-07) `-g` is the `-O0` build's**, which `heroes build` and `heroes test` make,
and `-O2`, `heroes run`'s level, carries line tables alone: there lldb still breaks on
and steps through `.hero` lines and `frame variable` says *no variable information is
available*, measured by the sitting. So the half that works is measured at `-O0`, and
`flags.hero`'s test that an unoptimised unit describes its locals is this row's
instrument in its smallest form, pulled forward.

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
that never had a test at all. It builds its programs at `-O0`, the build that carries
the variables since panel 197. Its own falsifier is run once by hand and quoted —
`-O0`'s `-g` deleted (`flags.debug_words`), the suite must go red — because a
debugger suite that passes without DWARF is a decoration. It also settles `docs/panel/085`'s B2 condition, which said in
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
