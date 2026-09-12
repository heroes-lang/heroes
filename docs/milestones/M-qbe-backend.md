# M-qbe-backend — the proof that the IR is not C in disguise


**Scheduled, no warrant**, and its warrant is stated here more honestly than
"a second backend" ever did. Two things were bought when panel 001 replaced QBE
with C emission, and one of them was never paid for: **as long as exactly one
backend exists, "the IR is target-agnostic" is an assertion no artifact tests**,
and the IR could be a C pre-processor wearing an abstraction's name without
anything in this repository noticing. QBE from the same IR (~500 lines) is what
turns that sentence into a measurement — and it restores the register-allocation
and instruction-selection lesson, which is the half of a compiler this project
deliberately handed to clang (DESIGN-LOG 2026-08-03, panel 001).

*******************************************************************************
**OPEN: 2**

- [ ] **M-qbe-backend** | every local is hoisted, so a recursive frame is sized by the whole body | `CLAUDE.md` §7 · `examples/interpreter/syn/expr.hero` · `selfhost/emit/`

    **Origin:** measured 2026-09-03 at M-corpus-depth step 5, while measuring
    defect 007 in `docs/work/DONE.md`. Its home since 2026-09-04: it had named
    no milestone, only *the next panel that touches the emitter or the IR*. That
    milestone's own warrant is the one this question needs — a second backend is
    what turns *the IR is target-agnostic* from an assertion into a measurement,
    and a slot's lifetime is precisely where the C emitter's convention stops
    being the only possible answer. Architecture, so it still rides a sitting
    rather than convening one (CLAUDE.md §4), and an earlier sitting that
    touches emission may take it.

    **That is what sets the recursion ceiling of every program in this
    language.** Measured with `heroes build --emit-c` over
    `examples/interpreter/`: **247 hoisted locals in the prologue of
    `syn/expr.hero`'s `compared`**, 151 in `primary`, several of them whole
    `Token` and `Expr` variants sized by their largest case, plus two `@`
    parameters copied in by value at every level (§4.8). Under `--sanitize` that
    chain measured **11,728 bytes between `bp` and `sp` for one frame** (ASan
    pads stack variables, so read it as an upper bound). The consequence is a
    ceiling a reader would not expect: this program's nesting limit is **120
    under `--sanitize`, 190 at `-O0`, 340 at `-O2`**, and the compiler's own
    parser gives up at **440**.

    CLAUDE.md §7 states the hoist as a rule with a reason (a `void t0;` is a
    hard error, one `goto`+label per block, all locals in the prologue) and the
    question this item asks is narrower than repealing it: **a temporary live
    inside one basic block does not need a slot for the whole function.** What a
    sitting has to price: whether scoping temporaries per block breaks the
    `#line` discipline or the double-emit determinism test, what it does to the
    seed's line count, and whether clang's own optimiser already does it at
    `-O2` — which the 190-against-340 pair suggests and does not prove. **Not
    urgent and not a defect**: the programs in this repository work. It is filed
    because the number is measured and the next sitting that touches emission
    should argue from it rather than discover it. **And half of the measurement
    that sitting needs is scheduled before it** (added 2026-09-07):
    M-typed-inspection's second step, scheduled 2026-09-06, is a census of
    exactly these hoisted locals — how many are `t<N>` temporaries, how many
    `$`-synthetic slots the lowering invented, how many bindings the author
    wrote — so the sitting reads that census before pricing per-block scoping
    rather than counting again.

    **Where to look also:** defect 007 in `docs/work/DONE.md` ·
    `docs/ROADMAP.md` § M-typed-inspection.
    **Why it matters:** a recursion ceiling nobody chose is a limit set by an
    implementation detail.

    **Re-verified 2026-09-10: STILL OPEN in substance, STALE PREMISE on its
    headline number, and that is the finding.** The **247 and 151** hoisted locals do
    not reproduce. Read off the cached emitted C for that program
    (`build/336f076635d558f1/main.c`, stamped `heroes 0.2.0`),
    `h_synexpr_compared`'s prologue is **119** declarations and
    `h_synexpr_primary`'s **122**; two older cached artifacts agree at about 122. So
    neither the number nor the 247/151 **asymmetry** survives. **UNSETTLED which of
    three things happened** — the source changed, the emitter improved, or the
    original count used a different rule — and settling it needs
    `heroes build --emit-c`, which was deliberately not run here (CL-025, and it
    writes into `build/`). The sitting re-measures it as its first act rather than
    inheriting either number. One pointer moved: the hoist rule left CLAUDE.md §7,
    which is a one-line pointer now, for `.claude/rules/generated-c.md:33-35`.

- [ ] **M-qbe-backend** | the QBE the sitting will meet is not the one panel 001 rejected | `design.md` §3.2 · `DESIGN-LOG.md:6`, `:8` · `selfhost/emit/extern_probe.hero`

    **Origin:** measured 2026-09-06 from this Mac, after the author asked
    whether the backend should move to QBE at the end of the chain; `qbe` is not
    installed here, so what c9x.me says is READ and not run. Its home is that
    milestone because every fact below is about the tool it builds on, and the
    sitting that opens it is the reader; nothing here convenes anything.

    **Two of design.md §3.2's sentences about it have expired.**
    `https://c9x.me/compile/releases.html`, read 2026-09-06: release **1.3
    (2026-06-01)** says *"Windows ABI support"*, and release **1.2
    (2024-02-16)** added *"new experimental `dbgfile` and `dbgloc`
    directives"*, which is line-level debug information. `design.md:702-704`
    says QBE has *"no DWARF"*; the site's front page still lists amd64 (linux
    and osx), arm64 and riscv64 with no Windows target, so the two pages
    disagree about Windows; and the IL document (`doc/il.html`) does not describe
    the two directives yet. **So the sitting's first step is to install QBE 1.3
    and RUN it** — `arm64_apple` on this Mac, the Windows target on the box
    (`docs/environment/windows/WINDOWS-MACHINE.md`) — before a line of emitter
    is written or a sentence of §3.2 is amended: a platform fact that has not
    been run on that platform is an inference (CLAUDE.md §1).

    **What did not expire is the reason the answer to the author's question is
    *a second backend, never a replacement*.** The IL has no way to include a C
    header or to check a declared signature against one (read the same day), and
    the verification of every `extern` in this compiler is clang reading the
    header. Counted in `selfhost/emit/` on 2026-09-07: **34** `_Static_assert`,
    **35** `_Generic`, **10** `__builtin_classify_type`, **7** `__typeof__`,
    **4** `__builtin_types_compatible_p`, **48** `#line`, **5**
    `__builtin_*_overflow`. **20 of 54** program directories under `examples/`
    (those holding a `main.hero` or `whole.hero`) declare an `extern`, and each
    would lose the header check. The runtime (**751** lines in `runtime.c` and
    the header, **4911** in `parts/`) and the seed (**747,095**) are C and still
    want a C compiler, and QBE itself emits assembly for `cc out.s` — so QBE
    adds a dependency and removes none. The `--sanitize` configuration in
    `tests/harness/suite_corpus.hero::configurations()` is clang's; QBE has no
    sanitizer. **And clang's DWARF has two consumers now, not one**:
    M-typed-inspection, scheduled 2026-09-06, is built on `-g`, on `#line` and
    on lldb reading the C types, so a second backend must say what a stopped
    program shows under it, or say plainly that it shows nothing. **Where the
    milestone sits is unchanged and the author's**: after the tools and before
    the books, for the 2026-09-03 reason that the IR should have stopped moving
    first (`docs/ROADMAP.md` § Who scheduled what).

    **Where to look also:** `design.md` §3.2 (`:698-707`) · `docs/ROADMAP.md`
    § M-qbe-backend, § M-typed-inspection ·
    `tests/harness/suite_corpus.hero::configurations` ·
    `https://c9x.me/compile/releases.html`.
    **Why it matters:** a sitting that argues from a 2026-08-03 description of a
    tool that has shipped two releases since is a sitting convened on a premise
    that expired in silence.

    **Re-verified 2026-09-10: STILL OPEN, six of the seven emitter counts
    exact.** In `selfhost/emit/`: `_Static_assert` **34**, `_Generic` **35**,
    `__builtin_classify_type` **10**, `__typeof__` **7**,
    `__builtin_types_compatible_p` **4**, `#line` **48** — all as written — and
    `__builtin_*_overflow` is **6**, not 5. Both unamended design.md sentences are
    still there (`:723` *"no DWARF"*, `:721` *"no headers"*). **Four numbers moved**:
    §3.2's QBE bullet is `design.md:717-726` not `:698-707`; `runtime.c` plus its
    header is **776** lines not 751; `runtime/parts/*.c` is **5013** not 4911; and
    `seed/heroes.c` is **773,509** not 747,095. Externs are **20 of 55** program
    directories, the numerator unchanged. **And one stale citation found beside it**:
    `design.md:718` calls the QBE backend *Part 7 item 14*, where item 14 is
    declaration visibility and QBE is item **15** (`design.md:2807`) — the same slip
    the ROADMAP's own cells carried until 2026-09-03.

*******************************************************************************
