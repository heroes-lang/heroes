- [x] **052 — a write into a `str` element compiles and traps** | `s[0] @ 65` passed `check` at exit 0 and the program aborted 134 saying *this is a compiler bug*, against a document that already calls `str` immutable | closed 2026-09-16, M-check-completeness, the same day it was opened

    **Origin:** 2026-09-16, **panel 159's compiler-engineer**, which found it on
    exactly R2's form while pricing the spec sentence that would teach that
    form, and reproduced it on the SELF-HOSTED compiler and not only the seed.
    Its own words for why it bound the sitting: a reader TAUGHT `xs[i] @ v` by a
    new sentence, who has also been taught that `s[i]` yields a `u8`, goes
    straight to this. **Widened from one shape to three by the completeness
    critic**, and corrected by it on the repair's home.

    **The reproducer.**

        function main()
            s: str @ "abc"
            s[0] @ 65
            print(s)

    `check` **0**, `build` **0**, run **134**: `panic: entered unreachable code
    — this is a compiler bug, please report it`.

    **The document had already ruled, so this was never a design question.**
    `spec § 3` reads `| str | immutable UTF-8 string, indexed and measured in
    bytes |`, and `spec § 10` gives only the read. CLAUDE.md § 12: spec
    beats compiler, so the compiler had the bug and the repair is a REFUSAL at
    check time rather than a lowering. No panel was needed for the direction,
    which is rare enough to record.

    **The class, and its edges, measured.** Three shapes reached it and the
    neighbours are closed:

        s[0] @ 65               local str           check 0, run 134
        xs[0][0] @ 65           str inside [str]    check 0, run 134
        r.s[0] @ 65             str in a record     check 0, run 134
        s[99] @ 65              index out of range  check 0, run 134 — the
                                                    unreachable panic, not the
                                                    bounds panic
        m["a"][0] @ 65          str out of a map    check 1, not_indexable, for
                                                    an unrelated reason
        s[0] @ "z"              wrong element type  check 1, type_mismatch:
                                                    expected `u8`, found `str`
        xs[0] @ 9 on [i64]      the control         check 0, run 0, prints 9

    The `type_mismatch` row is the one that mattered: **the checker fully
    type-checked the assignment against `u8`** and then let it through. This was
    never a gap in what the checker knew.

    **A first probe read `unused_binding` on five of seven shapes and measured
    nothing.** Without a trailing read that rule fires first and masks the
    question. Recorded because the wrong answer looked like a result.

    **The cause, named, and the C comment named it wrongly.** The IR was
    well-formed — `--dump-ir` prints `store s[$t2] <- $t3`.
    `selfhost/emit/container.hero`'s `.index_step` arm matched `.str` into the
    do-nothing group, fell through to `if element < 0` and returned
    `fail(msg: "not a container step")`; `selfhost/emit/inst.hero:130-132`
    renders any of that file's three failures as the one line
    `hero_unreachable(); /* not an element write */`, which is what the emitted
    C carried at line 119, with the index and the value computed and unused. **The
    emitter turned its own *I have no arm for this* into the program's *you have
    found a compiler bug*.**

    **The other two routes to that trap were looked for and are closed**, so the
    class is exactly one element kind wide: a map step followed by a field step
    is refused by the checker at exit 1, and `xs[0].n @ 1`, `xs[0][1] @ 7` and a
    `{str: {str: i64}}` all run at 0.

    **This class was repaired once and its neighbour was left.** The same file's
    comment records `xs[i].f @ v` being fixed on 2026-08-17. The element-kind
    case beside it was not looked at — *a repair is attacked at the shapes next
    to the one that provoked it*, unpaid for thirteen months.

    **The repair.** `access.refuse_str_element` in `selfhost/check/access.hero`,
    called from the `.mutate` arm of `selfhost/check/walk.hero`, with
    `flow_errors.str_element_write` carrying the message and its note. It asks
    THIS place's base type and never what that type reaches, so an element of a
    `[str]` is still a place. **It went into `access.hero` and not `walk.hero`
    because `layout` refused the second**: `walk.hero` is a ratified knot already
    above the threshold at 1870 lines, and the check's rule is that a file
    already over the ceiling may not grow further. It reads better where it
    landed — `access.hero`'s own module doc says *an INDEX is str -> u8, [T] -> T,
    and {K: V} -> V?*, and this is the write side of that same rule.

    **The critic corrected the repair's home before it was written.** Panel
    159's compiler-engineer priced a guard in the EMITTER at under 40 lines. A
    guard there would leave `heroes check` at exit 0, so the `tests/golden/check/`
    case its own prediction registers could not exist. Only the checker branch
    satisfies the prediction, and the seat had hedged with *or its checker
    counterpart*.

    **Verified**: the three shapes refuse, and seven legal neighbours are
    untouched — a `[u8]` element, a whole `str` element of a `[str]`, the read
    `s[i]`, a plain `main`, `main() -> ()`, an ordinary function's parameters,
    and `xs[0] @ 9`. Goldens at
    `tests/golden/check/fixedbugs-a-write-into-a-str-element.hero` with its four
    annotations and its `.expected`. `check` 123 to **125**, `annotations` 160 to
    **162**; `run`, `emission`, `determinism`, `corpus`, `canonical`, `fixes`,
    `layout`, `order` and the compiler's own 654 all green and unmoved.
