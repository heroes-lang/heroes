- [ ] **M-core-packages** | a C symbol spelled with a Heroes keyword cannot be bound at all | `selfhost/keywords.hero` · `docs/panel/094` R2, R3 · `design.md` §4.19

    **Origin:** panel 013's ffi-pragmatist, pre-existing and option-independent;
    carried on the panel watch list from 2026-08-03 until it was retired
    2026-09-04. Its home since 2026-09-07, when M-declared-freer closed without
    reaching it — and the reason is worth keeping, because it is the second time
    this item has outlived its address. Panel 109 opened §4.19's reserved
    annotation vocabulary and `owned` is the first word in it, which is why the
    item was homed there; what the sitting did NOT do is rule on a second
    contextual word, and Principle 0 gave it no reason to. **M-core-packages is
    where a reason appears**: a package that binds a real library is the first
    thing in this repository that can meet a C symbol spelled with a Heroes
    keyword, and that milestone's opening sitting already has §1.11 and §4.19 on
    its agenda. Its previous home read: the panel watch list deferred it to
    §4.19's deferred annotation vocabulary at the milestone now named
    M-ffi-ladder, an address that expired when that milestone closed 2026-08-12,
    and panel 109 is what opened that vocabulary — `design.md:2120-2125`
    reserved it, *"Reserve a keyword"*, and `owned <C function>` is the first
    word in it.

    **And §4.19 has no way to say another name for it.** Measured at panel 013
    over the macOS SDK and homebrew headers, in declarator or field position:
    `function` occurs **571** times, `func` 29, `assert` 11, `test` 5, `match`
    3 — and `selfhost/keywords.hero` reserves every one of them, so `extern
    function function(…)` has no spelling. **Re-measure the five counts at the
    opening rather than trusting these**: the headers on this machine have moved
    twice since.

    **What is NOT the answer, decided and on the record**: panel 094 refused a
    rename clause (ratified 2026-08-26) — but on a premise this case never
    reached, that *"one C symbol carries one arity"*, and its file contains
    **zero** occurrences of `reserved`, `collide` or `registry`, so the
    collision was never priced. R3 of that sitting binds the spelling if it ever
    lands: **`tag`, not `= "cname"`**, which is already the word an `extern`
    record uses for the C tag (`spec:231`) and is contextual, so it costs no
    keyword. The cheap question for the sitting is whether `owned`'s arrival
    makes a second contextual word in the same position free, or whether
    Principle 0 still holds this one out — nothing on the closure list binds a
    colliding symbol, and a binding nobody can write never appears in a corpus,
    so no trigger can ever fire for it.

    **And the same sitting names the shape rule the family already obeys**
    (author question 2026-09-06, M-core-packages' question (vii) in
    `docs/ROADMAP.md`, split from M-reflection-verdict's (iii) because the
    second-contextual-word question is here): six contextual words today —
    `owned`, `tag`, `partial`, `link`, `package`, `as` — each after the thing it
    modifies, each in one position, each carrying a check, none in
    `selfhost/keywords.hero`'s table of 21 (measured 2026-09-07 from
    `selfhost/parse/`); the rule stated by panels 094 R3, 109 and 114 R7
    separately and written as a rule nowhere; whether design.md §4.19 names it,
    so that a `tag` for a colliding symbol, the buffer case M-declared-freer
    queued and panel 003's discardable mark follow it without a fourth
    re-derivation.

    **Where to look also:** `docs/panel/013-function-type-marker.md:179-184` ·
    `docs/panel/114-the-question-was-not-which-platform.md:244-252` ·
    `design.md` §4.19, `:2120-2125` (`:2139-2144` today) ·
    `selfhost/parse/members.hero:81-94` · `spec:231`.
    **Why it matters:** §1.11 says everything comes from C, and this is the one
    class of C name the language cannot reach — the hole is invisible because
    the program that would find it cannot be written.

    **Re-verified 2026-09-10: STILL OPEN, and both of its measured negatives
    hold exactly.** `selfhost/keywords.hero:35-57` is the closed table at **21**
    words and none of the six contextual words is in it; over `docs/panel/094`,
    `reserved`, `collide` and `registry` are still **0**, **0** and **0**. Two
    pointers moved: design.md's *"Reserve a keyword"* is `:2178`, and the `tag`
    sentence is `spec/heroes-spec.md:254-255` where the item says `spec:231`, which is
    now a fence. **The five SDK counts were deliberately not re-run**: they need a
    declarator scan of the macOS SDK plus the Homebrew headers, which the item itself
    asks for at the opening.
