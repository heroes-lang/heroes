---
kind: task
area: emit
milestone: M-core-packages
filed: 2026-09-07
commit: none
github: none
---

- [ ] **M-core-packages** | four repairs to design.md that ride its opening sitting, §4.19's `#include` sentence among them | `design.md` §1.11, §3.5, §4.19, §4.20, Part 7 item 4 · `selfhost/emit/externs.hero:69-70` · `docs/panel/056`, `091`

    **Merged 2026-09-10** from two items, by author instruction: both are repairs
    to design.md, both ride the same opening sitting, and one editing pass closes
    all four. **Both bodies are kept whole below.**

    **Re-verified 2026-09-10: STILL OPEN, all four owed, and every pointer in the
    second body has moved.** `grep -n "declaration order" design.md` is **0**, so
    the `#include` invariant is still written only in code, at
    `selfhost/emit/externs.hero:69-70` (the item said `:72`, which is now the
    `hero_os.h` seed line of that same doc comment). `args_checked` appears **0**
    times in design.md and `validated` only outside §1.11 and §4.20, so both are
    still omitted; they are `spec/heroes-spec.md:204-205`, not `spec:186-190`.
    *"no aliases, no package hierarchy"* still stands at `design.md:2641`, and
    §3.5 still holds no paragraph naming what would return a project file.
    **Three pointers**: §1.11's Tier-2 list is `design.md:493`, §4.20 opens at
    `:2266`, Part 7 item 4 is `:2641`. **The second body's own evidence sentence is
    now false**: *"`grep -n unplaced design.md` hits only `:2407`"* — it hits
    `:2566`, `:2584`, `:2617` and `:2712`, and `:2407` is not among them.
    **And the `#include` sentence has to say more than it did**: the list now
    carries two unconditional seeds, `math.h` and `hero_os.h`
    (`selfhost/emit/externs.hero:76`), so *the order groups are declared in is the
    order of the includes* is true only after those two.

    **A FIFTH repair was found while verifying and it belongs here**, same class as
    the second: `design.md:806` (§4.1) still says *"there are no aliases and no
    wildcard"*, which `spec/heroes-spec.md:11` contradicts — `use syntax/decl as
    sd` has bound an alias since M-package-layout closed on 2026-09-02. The
    sentence to repair is design.md's, since §12 gives the spec precedence.

    **The first item, as it stood.** §4.19 owes one sentence: a group's `#include` order is load-bearing

    **Origin:** panel 091, the ffi-pragmatist's explicit *not covered by
    design.md*. Its home since 2026-09-07, when M-declared-freer closed without
    taking it. It named that milestone from 2026-09-04 on the ground that panel
    109 amends §4.19 and this is a §4.19 sentence — true, and the sitting ruled
    on ownership and never on the `#include` list, so the sentence stayed
    unwritten. It rides M-core-packages' opening sitting instead, which touches
    §1.11, §4.19 and §4.15 by its own six questions, and where a later item
    already parks three other design.md repairs for the same reason. The lesson
    is this file's own: *the next sitting that touches X* is a waiting condition
    and not a home, and naming a milestone did not fix that — what fixes it is
    naming a sitting whose AGENDA contains the question.

    Measured: `<jpeglib.h>` alone is 8 errors (`unknown type name 'size_t'`);
    `<stdio.h>` first, then clean. So the order in which groups are declared
    **is** the order of the `#include` list (`emit_externs.headers`, "in
    declaration order"), and it is the author's only lever on it. design.md
    §4.19 does not say so, which means nothing stops a later pass from
    reordering or thinning that list — and the sitting that would do it would be
    reasoning from a document that never mentioned the constraint. The sentence
    is owed whether or not anything is ever pruned; it is CLAUDE.md §11's
    expiring premise before it expires. Amending design.md Parts 1-11 is a panel
    path (CLAUDE.md §4), so this rides the next sitting that touches emission
    rather than convening one.

    **Where to look also:** `docs/panel/091` § What the ffi-pragmatist compiled.
    **Why it matters:** an invariant nobody wrote down is one somebody will
    optimise away.

    **The second item, as it stood.** three repairs to design.md that ride its opening sitting

    **Origin:** found 2026-09-03 reading forward (CLAUDE.md §1). They ride the
    opening sitting because that sitting touches §1.11 anyway.

    **(a)** §1.11's Tier-2 list (`design.md:474-478`) and §4.20's inventory
    (`:2273-2276`) omit `validated` and `args_checked`, landed 2026-08-24
    (`DESIGN-LOG:408`, `:413`); the spec has them (`spec:186-190`) and
    `suite_spec` polices the spec, not design.md. **(b)** Part 7 item 4
    (`design.md:2447-2449`) still reads *"no aliases, no package hierarchy"*
    after M-package-layout landed `use syntax/decl as sd` on 2026-09-02; `grep
    -n 'panel 099\|panel 100\|panel 101' design.md` is 0. **(c)** Panel 056
    deliverable B — *"a greppable paragraph in design.md §3.5"* naming what
    would return a project file — was ratified 2026-08-15 and never written:
    `grep -n unplaced design.md` hits only `:2407`, the comptime paragraph.

    All three are design.md Parts 1–11, so none is written without a sitting
    (CLAUDE.md §4); deliverable D, the same conditions in the ROADMAP's
    M-package-manager entry, was discharged 2026-09-03.

    **Why it matters:** a document that omits what shipped briefs the next
    sitting wrong.
