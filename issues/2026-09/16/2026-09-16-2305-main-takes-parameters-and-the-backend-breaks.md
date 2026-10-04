- [x] **053 — `main` may declare parameters, and the backend breaks in clang's voice** | `function main(n: i64)` passed `check` at exit 0 and `run` exited 2 with *too few arguments to function call* | closed 2026-09-16, M-check-completeness, the same day it was opened

    **Origin:** 2026-09-16, **panel 159's completeness critic, and no seat**. It
    was found while auditing whether the sitting's R3 sentence about `main`'s
    signature was one the compiler actually checks — so the defect was produced
    by asking what a proposed sentence would claim, which is a route no brief
    had asked anybody to walk.

    **The reproducer.**

        function main(n: i64)
            print(f"{n}")

    `check` **0**, run **2**:

        internal error: compiling the generated C failed:
        margs.c:43:22: error: too few arguments to function call,
        single argument 'h0_n' was not specified

    **The cause.** `selfhost/check/decls.hero` guarded `main`'s RESULT — panel
    035's guard, bought when `main() -> i64?` returning a failure printed nothing
    and exited 0, *the only failure in this language with no instrument at all* —
    and nothing read its PARAMETERS. The emitted `int main` calls
    `h_<module>_main()` with no arguments (`selfhost/emit/decls.hero`), so a
    declared parameter has nothing that could ever pass it a value.

    **The only rule that caught any of these said nothing about `main`.** An
    unread parameter is `error[unused_binding]`, which fires on any function.
    Read the parameter — as the reproducer does — and the program reached the
    backend. So the shape that compiled was the one a reader would actually
    write.

    **What it cost the sitting it was found in.** R3 asked whether a sentence
    about `main`'s signature is worth its tokens, given that `error[main_returns]`
    teaches it on first contact. The measured answer is that the diagnostic
    taught **one third** of the rule: it catches `-> ()?` and `-> i64`, it lets
    `-> ()` through by construction because the guard asks `result != unit`, and
    it did not look at parameters at all. A reader told *"`function main()`, that
    signature exactly"* — the wording two of five seats independently proposed —
    would have been told something the compiler does not check and, at
    `main(n: i64)`, something it contradicts at exit 2.

    **The repair.** The parameter half of the same guard, beside the result half,
    with `value_errors.main_parameters` carrying the count and a note naming
    `args()` as where the command line actually reaches a program. Both sentences
    about `main`'s signature are now enforced in one place.

    **Verified**: `function main(n: i64)` is exit 1 with `main_parameters`; a
    plain `main`, `main() -> ()` and any other function's parameters are
    untouched. Golden at
    `tests/golden/check/fixedbugs-main-takes-no-parameters.hero`.

    **And `main() -> ()` still compiles**, deliberately. It is legal, the guard
    asks `result != unit`, and the spec sentence that landed says what is true —
    `main` *takes nothing and produces nothing* — rather than the false *that
    signature exactly*.
