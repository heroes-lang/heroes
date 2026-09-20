# DEFECTS — the compiler defects that are still open

Every item is a **measured** failure of the compiler on a program — a crash, a
wrong answer at exit 0, a silence where a message is owed — carrying its
reproducer, its cause where known, and what is owed. **Only open defects live
here**: a repaired one is ticked, gains a *The repair* section with the
measurements that prove it, and moves to `docs/records/done/`. A repair is owed
at the class and not at the witness, with a `tests/golden/fixedbugs/` case per
shape.

**The shape** is `.claude/rules/records.md` § The lists, and § A live list is a
preamble, a count and its items is why this preamble is fifteen lines. **The
next number is READ, never remembered** — `records/numbering` takes one above
the highest issued across this file and `docs/records/done/`. Who issued which
number since 2026-09-08, and why 014 exists twice, is
`docs/records/log/2026-09-16-2200-the-defect-register-leaves-the-list.md`.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 5**

- [ ] **066 — a `ptr` lend has no lifetime rule, so C may keep the address past the frame** | a field's address handed to C outlives the binding it came from, and a later C call reads a dead frame at exit 0 | `selfhost/check/lending.hero`'s `field_lend_escapes`, `spec § 13`'s lease sentences

    **Origin:** panel 166's completeness critic, 2026-09-19. No seat found it and
    no route A–G touches it. Shipped at `ef7b013b` with the lend itself.

    **Reproducer**, nine lines, and C does the keeping:

        function lend_and_return()
            s: Sl2 @ mk()
            keep(p: s.name.ptr())      # C parks the address in a static

        function main()
            lend_and_return()          # the frame dies here
            print(to_str(later()))     # C reads it anyway

        C reads the dead frame: 1      exit 0, no diagnostic
        --sanitize: AddressSanitizer: stack-use-after-scope

    **What the compiler does check, and why it is not enough.** It refuses the
    two escapes **Heroes** can see — `field_lend_escapes` and
    `field_lend_needs_a_place`, both re-run — and nothing looks at the C side.
    `sqlite3_bind_text` with a null destructor is declared in this repository at
    `examples/ledger/db/sqlite.hero:107`, which is the same shape with a real
    library behind it.

    **What is owed, and the language already owns the answer shape.** `spec § 13`:
    *"`x: cstr @ s.lease()` is a COPY of the bytes that C may read for as long as
    the program says, and `end_lease(@x)` frees it"*, and *"a lease nobody ends …
    aborts when `main` returns, saying how many"*. Panel 164 opened the address
    route and did not carry the lifetime rule across with it. Either the lend
    gains one, or the document says the address dies with the frame and the
    program is on its own — and the second is the shape panel 166's seats vetoed
    for the write direction.

    **CORRECTION, 2026-09-20, by panel 167 and its completeness critic.** This is
    a **lend** defect and not a field-lend defect: `s.cstr()` has the identical
    hole, measured — exit 0, no diagnostic, `heap-use-after-free` under the
    sanitizer — and it is the half the corpus uses, **59 occurrences against
    `.ptr()`'s 33, ten of them in four shipped examples against zero**.

    **And the sanitizer sentence above is false for the case that matters.** It
    holds only because the reproducer's retainer is a `static inline` in a
    header. Bound against the real `libsqlite3.dylib`, `--sanitize` prints **zero
    AddressSanitizer lines, exits 0, and prints the CORRECT value** — it masks
    the defect rather than catching it. So there is no instrument at all.

    **Routes H and C of panel 166 narrowed this by zero**, run rather than
    argued. **Panel 167 adopts route A** — a field lease that copies — with two
    type rules widened from `cstr` to `ptr`, without which the lease is not
    sound; this entry closes when that lands.

    **AND ROUTE A HAS AN UNPRICED HALF, measured 2026-09-20 after the sitting.**
    The trailing header the resolution adopts cannot be found by
    `hero_held_release`, which subtracts `sizeof(HeroHeldHeader)` from the
    pointer — and `end_lease(@p)` carries only the cell, with no length and no
    route back to the field. Two candidates are recorded under the sitting: a
    pointer-keyed side table (the handle set's shape, already in the runtime) or
    the emitter passing the length, which is free at run time and needs a fact
    `check/leasing.hero` holds one pass earlier. **Whether it survives to the IR
    is unrun**, and it is what the next step measures first.

    **IT IS RUN, 2026-09-20, AND THE QUESTION WAS THE WRONG ONE.** The length
    does not survive: `call builtin end_lease(@c)` carries the slot alone, and
    with a loop between them the lease is in `bb0` and the release in `bb3`.
    But with the CORRECT length in hand a trailing header still reads freed
    memory, because C has already freed the block. Panel 168 struck the trailing
    header on the engineer's veto: at 64 KiB and 1 MiB the magic word **survives**
    C's free, so the release's check passes and it frees a block it no longer
    owns. The allocation half of panel 167 clause 1 is gone and route A, if
    built, is built on the leading header that ships.

    **AND THE CLASS IS WIDER STILL, for the second time — panel 168's
    completeness critic, 2026-09-20.** This entry was corrected from *field lend*
    to *lend* this morning. It is corrected again to **lend AND LEASE**, measured
    on this repository's own shipped example: `examples/gallery/13-lease.hero`
    with the print moved two lines down, after `end_lease` instead of before it,
    is `check` **exit 0**, `run` **exit 0** five of five, prints **0 where 13 is
    honest**, and `--sanitize` says `heap-use-after-free` at
    `13-lease.h:7 in kept_label_length`. The file is committed as
    `docs/panel/168-briefs/gallery-example-reordered.hero`.

    **So route A closes NOTHING, and the sentence above that says this entry
    closes when it lands is FALSE.** A copy moves the moment the bytes die from
    *the frame returns* to *`end_lease` runs*, and both are Heroes-side events:
    C's retention is unrelated to either, and copying does not relate them. The
    class this entry names is **nothing in the language relates C's retention to
    the moment the bytes die**. Panel 167's own spec-warden predicted it in those
    terms — *"routes A, B and C each close zero of two reproductions when landed
    alone"* — and panel 168 scores that prediction CORRECT rather than waiting for
    the close. What is owed is named in
    `docs/panel/168-the-property-is-not-the-base-and-route-a-closes-zero-of-two.md`
    § The routes nobody
    listed, and the nearest one is that the language already ships a retention
    vocabulary: `borrows` says a call keeps what it is handed, and `acquires`
    carries a pointer-keyed live set that aborts, which is the instrument two
    sittings said did not exist.

    **AND THE EXTENT IS NOW STATED, 2026-09-20, which reclassifies this entry
    and defect 068 both.** `spec § 13` says *A lend lives for its call and no
    longer: C keeping the pointer reads bytes the program may have changed or
    freed since, and nothing checks it.* Under that sentence **defect 068's
    write is legal and C is the one retaining**, so 068 is not a second class
    beside this one: the two are witnesses of one defect, which is that
    **nothing lets a declaration say a parameter retains, and nothing refuses a
    lend that reaches one**. Panel 169 split them on the historian's
    exclusivity precedent, and the split does not survive the extent the same
    sitting adopted; the correction is written under
    `docs/panel/169-two-defects-two-classes-and-the-one-that-was-never-searched-where-it-happens.md`.

    **What is no longer owed**: a route by which a program can hand C bytes it
    will keep. `docs/measurements/037-the-give-away-case-was-writable-the-moment-the-callback-was.md`
    measures that one exists and needs no new form — the author allocates and
    names the disposer, `check` 0, `run` 0, zero AddressSanitizer lines — and
    that it became writable when defect 069 was repaired six hours earlier.

    **What is still owed**, and it is narrower than this entry has said since it
    was filed: a way to DECLARE that a parameter retains, and a refusal of a
    lend that reaches one. That is the half panel 167's survey of ten ecosystems
    found nobody enforces.

- [ ] **068 — a record rewritten under C's held address, which no sanitizer can see** | C holds a field's address, the program writes the record, and C reads bytes the program never meant it to — a wrong answer at exit 0 with zero AddressSanitizer reports | `selfhost/check/lending.hero`, `spec § 13`'s lend sentence

    **Origin:** panel 167's completeness critic, 2026-09-20, separating defect
    066 into the two defects it is.

    **How it differs from 066, and why the difference matters.** 066 is the frame
    DYING under a held address, which a sanitizer can see when the retainer is
    instrumented. Here nothing is freed and no frame dies: the storage is alive
    and its contents change. **No sanitizer can ever see it**, on any platform,
    because no memory rule is broken — only the program's meaning is.

    **What is owed.** Nothing decides this one from a declaration, a header or an
    argument: panel 167 measured that retention is per-call and that a run-time
    `bool` can pick it. It is filed so that the route adopted for 066 is judged
    against it too, and so that a later sitting does not discover it as new.

    **THE LAYER WAS WRONG, and panel 168's completeness critic found it**,
    2026-09-20. This entry files 068 with 066 as *foreign retention*, where panel
    167's historian proved static enforcement exists in none of ten ecosystems.
    That is true of 066 and **false of 068**: in the reproducer the wrong answer
    is produced by a **Heroes assignment**, two lines below the lend, in the same
    function, to the very binding whose field's address was lent. No C code
    participates in the corruption; C only observes it. That is **caller-side**,
    which is exactly where the historian found static enforcement does exist
    everywhere, and `field_lend_escapes` and `field_lend_needs_a_place`
    (`selfhost/check/lending.hero:218-228`) already relate a lend to its root
    binding. **068 has never been searched at the layer where its defect
    happens**, and *"nothing decides this one from a declaration, a header or an
    argument"* above is an argument about the C side of a line the Heroes side
    writes.

- [ ] **070 — a lease handed to a C function that frees it dies with an empty stderr and an unstable exit code** | `check` 0, `build` 0, and the program aborts saying nothing at all, 133 nine times and 134 once in ten runs | `spec § 13`'s lease sentences, design.md §4.17

    **Origin:** panel 168's compiler-engineer, 2026-09-20, reproduced by the
    coordinator before filing.

    **Reproducer**, six lines, and the C side is one:

        extern "giveaway.h"                  # static inline void eat(const char *s)
            function eat(s: cstr)            # { free((void *)(uintptr_t)s); }

        function main()
            x = "payload"
            c: cstr @ x.lease()
            eat(s: c)
            end_lease(@c)

    `check` **exit 0**, `build` **exit 0**, ten runs: **133 133 133 133 133 133
    133 133 133 134**, and **stderr is empty every time**. Under `--sanitize`:
    `bad-free`, named at the program's own C line.

    **What is owed.** design.md §4.17 asks a diagnostic to carry everything needed
    to fix the program; this carries nothing, not even a stable exit code. The
    repair is not a header layout — panel 168 measured that both layouts abort and
    that the trailing one produces a message blaming the compiler. It is §4.19's
    third reserved case, *a buffer that C takes ownership of*, or a rule about the
    call. **Neither exists**, and panel 168's ffi seat measured that the give-away
    needs the library's own **allocator** rather than any pointer the Heroes
    runtime can hand out.

    **A FOURTH SHAPE, panel 170's compiler-engineer, 2026-09-20**: a plain
    `.cstr()` lend into a freeing callee, with no lease anywhere — `check` 0,
    exit **134**, stderr **0 bytes**. It is the same class and it is named here
    rather than given a number, because a repair is owed at the class.

    **AND THE EMPTY STDERR IS THE LANGUAGE'S OWN REPORT BEING PRE-EMPTED**, which
    the same seat measured against a control: the control prints
    `panic: 1 lease(s) never ended`, **111 bytes**, and this prints zero because
    C's `free` aborts before `main` returns. **The run-time instrument works and
    the corruption outruns it.**

    **What panel 170 settled about the repair.** No mark can carry it together
    with retention: **retention must ADMIT a lease and give-away must REFUSE
    one**, which is the compiler-engineer's veto ground. The route named for the
    next sitting is a MEANING for a word that already parses — `borrows` and
    `consumes` both parse on a `cstr` today and are thrown away by
    `check/marks.hero`'s handle-only sweep.

- [ ] **072 — two allocator families collapse onto one handle type, and each frees the other's blocks in silence** | `check` 0, `build` 0, run **0**, the arena destroyed and the heap block leaked, and the live set says nothing because the count balances | `selfhost/check/decls.hero:313`'s `one_tag_one_type`, `spec § 13`'s handle paragraph

    **Origin:** panel 169's ffi-pragmatist, 2026-09-20, while measuring what caps
    the handle route. Re-run by the coordinator before filing.

    **It is a hole opened by a refusal**, which is why it is filed rather than
    folded into defect 029: 029 closed on 2026-09-13 by making a fieldless group
    `record` a handle, with **two records may not share one tag** as one of its
    two refusals. Two C allocator families that both hand back `void *` must
    therefore share one Heroes type, and the type system can no longer tell their
    blocks apart.

    **Reproducer**, one `record Block tag void` over two pairs:

        a = arena_new()        # acquires arena_free
        h = heap_new()         # acquires heap_free
        heap_free(b: a)        # frees the arena
        arena_free(b: h)       # leaks the heap block

    `check` **exit 0**, `build` **exit 0**, run **exit 0**, and the program
    prints its own line as if nothing happened. **The live set is balanced** —
    two acquired, two consumed — so the exit check is silent too.

    **What is owed.** Either `one_tag_one_type` admits a second record over
    `tag void`, or a handle carries which producer made it. Panel 169 recorded
    this as one of three caps on the handle route and did not price the repair.

    **PANEL 170 PRICED IT AND ADOPTED THE FIRST**, 2026-09-20. `one_tag_one_type`
    is narrowed at `tag void`: the rule's own stated reason is written about
    `struct s *`, where clang is a second judge of the spelling, and **at
    `void *` no second judge can exist**, so the rule buys nothing there and
    costs this defect. Its own claim that the mutant survives every instrument is
    **falsified by a run**: with two distinct tags the crossed program is
    `error[type_mismatch]` at **check, exit 1**. The sitting's completeness critic
    settled the matrix that two seats each had half of — two tags are declarable
    today only through a shim header, so the narrowing is a real change and not a
    discovery.

- [ ] **073 — `owned <fn>` on an INPUT parameter is admitted by the grammar and crashes the backend** | `check` 0, `build` **2**, `internal error: compiling the generated C failed`, and the emitted C passes an optional struct where C wants a `const char *` | `spec § 13`'s `CParam` production, `selfhost/emit/ops.hero`

    **Origin:** panel 170's spec-warden, 2026-09-20, reproduced by that sitting's
    completeness critic and again by the coordinator before filing.

    **Reproducer**, five lines:

        extern "stdlib.h"
            function free(p: ptr)
            function atoi(s: cstr owned free) -> i32

        function main()
            print(to_str(atoi(s: ok("12"))))

    `check` **exit 0**, `build` **exit 2**, and clang says
    *"passing 'h_0opt_f87774a' (aka 'struct h_0opt_f87774a') to parameter of
    incompatible type 'const char *'"*.

    **What it actually is.** `owned` is a RESULT mark: *the compiler frees that
    string with that function and hands it over as a `str?`*. Written on an input
    parameter the checker applies the result semantics anyway — the parameter's
    Heroes type becomes `str?`, which is why the argument must be `ok("12")` to
    get past `check` at all — and the emitter then hands C the optional's struct.
    **The grammar admits it, the prose never defines it, and clang is the only
    thing that notices.**

    **It is load-bearing rather than incidental.** `.claude/rules/c-boundary.md`
    lists the five clang failures that are the AUTHOR's fault and exit 1; this is
    exit 2 and says the compiler is wrong, which it is. And **two of panel 170's
    candidate routes wanted to build on that slot.**

*******************************************************************************
