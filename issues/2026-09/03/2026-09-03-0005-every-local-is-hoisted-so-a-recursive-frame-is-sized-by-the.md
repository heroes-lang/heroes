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
