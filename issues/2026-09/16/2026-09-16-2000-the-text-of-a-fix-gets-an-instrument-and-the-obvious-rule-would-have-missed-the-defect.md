- [x] **M-check-completeness** | nothing watches the TEXT of a `guess` fix, and one shipped a name the language had withdrawn | **CLOSED 2026-09-16** | `tests/harness/fix_names.hero` · `tests/harness/suite_fixes.hero`

    **Origin:** found 2026-09-08 at M-closures-verdict step 3, while writing
    defect 019's golden case — the case would not pin the thing the defect was.

    **The instrument, and it reads the SPECIFICATION and not the compiler.**
    `fix_names.run_default` runs `check --json` over `tests/golden/check/`,
    reads every `"title"` a fix carries, and judges the backticked spans inside
    it against `spec_text.offered_builtins` — § 11's own `Built-ins:` sentence.
    That source is the decision: an instrument that asked
    `selfhost/inventory.hero` would agree with the compiler by construction and
    could never catch the compiler recommending a name it had itself withdrawn.

    **THE OBVIOUS RULE WOULD HAVE MISSED THE DEFECT IT EXISTS FOR, and the
    measurement is what said so.** The corpus was measured before the rule was
    written — 93 titles, 53 distinct — and the backticked spans fall in four
    shapes:

    | shape | measured example | judged |
    |---|---|---|
    | a pure identifier, called | `to_f64(x)` | YES |
    | a family template, called | `to_<width>(x)` | YES, on the stem |
    | a method or an expression | `.must()`, `"n is " + n.to_str()` | no |
    | a type or a sketch | `T?`, `x: <type> = wanted(…)` | no |

    A rule checking only identifiers would skip `fit_<width>` — it carries `<`
    and `>` — which is **exactly the span that shipped**. The family arm is what
    catches it: the language has `to_str`, `to_i8` … `to_u64` and has never had
    a `fit_` anything, so the stem is the thing that was wrong.

    **Verified by making it fire.** `to_f64(x)` was changed back to
    `fit_<width>(x)` in `selfhost/value_errors.hero`, the compiler rebuilt, and
    the suite went **20 passed, 5 failed**, each failure naming the family, the
    missing stem and the whole title. Restored, it reads 25 passed, 0 failed.
    `fixes` moved **10 to 25**.

    **And the case at the top of that list is the finding.**
    `fixedbugs-a-guess-fix-names-a-family-that-exists.hero` is the golden
    written when the defect was repaired on 2026-09-08 — a case carrying the
    defect in its own name, asserting nothing about the fix title, because no
    instrument could read one.

    **A mistake made inside the repair, and kept in the file's comment.** The leg
    was written with no `report.keep_pass`, so it contributed 0 passes and 0
    failures: green, and indistinguishable from a leg that never ran. That is
    the shape `.claude/rules/diagnostics-and-goldens.md`'s new instrument rule
    names, committed the same hour, and it was made in the instrument written to
    cure it.

    **What it deliberately does not do**, so a later session widens it against a
    stated position rather than a silence: it judges only a span that BEGINS
    with its callee. `"n is " + n.to_str()` is prose containing a call, and
    parsing inside it is the crying-wolf half the item warned about. When that
    becomes the shape that ships a wrong name, the arm is added and the file's
    own paragraph is what it is added against.
