- [ ] **M-cleanup-verdict 1** | Open `tests/golden/run/fixedbugs-a-handle-in-a-record-field.hero`, landed 2026-09-13, and find the line `print(s.handle == nullptr)` with its expected `true`. That case ran green for a whole day while `==` on a handle was a constant `true`. **Before reading any further: say why a green run of that case told nobody anything, and write down the smallest change to it that would have gone red.**

    **Where to look:**
    `tests/golden/run/fixedbugs-a-handle-in-a-record-field.hero` and its
    `.expected`; then
    `tests/golden/run/fixedbugs-a-handle-compares-as-an-address.hero`, which is
    the case that separates the two answers;
    `docs/records/done/2026-09-14-0859-defect-032-closed-two-handles-compared-equal-whatever-their-addresses.md`.

    **Why it matters:** the header that case ships sets `s.handle = 0`, so the
    handle it compares against `nullptr` **is** null. The right answer is `true`
    and the wrong answer is also `true`. Of every question that could have been
    asked about handle equality, the one golden in the tree had picked the
    single one where a constant and an address comparison are indistinguishable
    — so the test was not weak, it was **blind in exactly one direction**, and
    a green run carried no information at all.

    **The general shape is worth more than this instance.** A test asserts a
    value; what you want to know is whether the code computes it. Those two come
    apart whenever the expected value coincides with what a broken
    implementation returns by default — `true` for a fold over no fields, `0`
    for an uninitialised counter, the empty list for a walk that never ran. Ask,
    of a test you are about to trust: **what would this still print if the thing
    under it did nothing at all?**
