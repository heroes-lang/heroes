- [x] **066 — a `ptr` lend has no lifetime rule, so C may keep the address past the frame** | **closed 2026-09-21**, M-declared-extents step 24 | **The default flipped: a C parameter is assumed to KEEP what it is handed unless declared `lent`, and a lend reaches only one so declared** — the author's ruling of 2026-09-20, priced by panel 171, landed in two commits | `selfhost/check/lend_landing.hero` · `selfhost/lend_errors.hero` · `tests/golden/check/fixedbugs-a-lend-c-keeps-past-the-frame.hero` | 171

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

    ## The repair

    **The direction is the author's**, given 2026-09-20 in plain words with one
    condition, the cost, which `docs/measurements/038` met before the ruling:
    a copy at every unmarked call is 33 ns at worst and the mark costs nothing.
    Heroes is young, nobody depends on its examples yet, and robustness is a
    principle of the language, so the assumption goes the robust way.

    **The word is `lent`** (panel 171), in its own `CParam` slot beside
    `counted_by`, read by `check/lend_extent.hero` where it is declared and by
    `check/lend_landing.hero` where a lend lands. Three questions, in the
    order a reader meets them, one message per lend: the header
    (`lend_needs_a_header`, `field_lend_needs_a_header`), the extent for a
    field (`field_lend_uncounted`), the word (`lend_kept`). No `certain` fix:
    adding the word is a claim about C the compiler cannot check, and a lease
    is wrong where C frees, so both notes are a `guess` and both say so.

    **Two commits, because the seed compiler that builds the next rung has to
    parse the marks that rung writes**: `83a8c92c` the word alone, this one
    the rule, the 27 marks on 19 functions across `selfhost/cli`,
    `selfhost/emit`, `selfhost/library_source.hero` and
    `tests/harness/shell.hero`, the spec sentence, and the goldens. The
    one-commit landing was run and breaks at `selfhost/cli/process.hero:42:40`
    with no binary.

    ## The measurements

    | | before | after |
    |---|---|---|
    | the reproducer, `keep(p: s.name.ptr(), n: 8)` into an unmarked parameter | `check` **0**, C reads a dead frame at exit 0 | **`check` 1, `error[lend_kept]`** at the lend, the golden above |
    | the `cstr` half, `keep_label(s: label.cstr())` | `check` **0**, `heap-use-after-free` under the sanitizer | **`check` 1, `error[lend_kept]`** |
    | `later()`, the pointer C hands back | `check` 0 | **`check` 0, unchanged**: not a lend, written down as what the flip leaves standing |
    | the spec, `heroes measure --refresh` | 8201 real, 6159 vendored | **8216 real, 6172 vendored**, +15 and +13, under `DELTA_GATE` |

    ## What it did not close

    **Defect 070** stays exactly where it was: a lease into a freeing callee is
    `check` 0 and dies with an empty stderr, and its fourth shape, a plain lend
    into a freeing callee, is now `lend_kept` rather than silence. **The
    pointer C hands back** — `strchr(s: text.cstr()) -> cstr` into a keeping
    parameter — is admitted, measured older than the flip, and the corpus
    already knows it as defect 024's caller lease.

    ## The gates

    The whole golden tree, because a change to what the checker refuses is
    judged by every one of them (`.claude/rules/verification.md`): the counts
    are in the commit body. The seed was regenerated in the same commit and
    the fixpoint verified byte-identical.
