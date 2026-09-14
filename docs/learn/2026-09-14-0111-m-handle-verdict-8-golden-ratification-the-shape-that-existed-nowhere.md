- [ ] **M-handle-verdict 8** | Read `tests/golden/run/fixedbugs-a-handle-in-a-record-field.hero` and its `.h` beside it, then build it with a compiler from before the fix: `git show HEAD~1:seed/heroes.c > /tmp/old.c && clang -I runtime /tmp/old.c runtime/runtime.c -o /tmp/old && /tmp/old build tests/golden/run/fixedbugs-a-handle-in-a-record-field.hero -o /tmp/x`. Read the warning it prints. Then build it with `./heroes` and read the silence. Before looking at the fix, say **which of the record's three fields the compiler was getting wrong and why the other two were right.**

    **Where to look:** the comment block above the change in
    `selfhost/emit/extern_record.hero`; the test `a handle field is a scalar in
    the probe, and the record beside it is not`, in the same file;
    `docs/panel/146-the-name-survives-and-the-sitting-convened-over-it-found-a-cast.md` § 1d.

    **Why it matters:** the fault is one character — `{0}` where `0` belonged —
    and the interesting part is **why nobody met it for four days**. The handle
    form landed at step 4. This case was found at step 8, by a panel seat that
    was testing something else entirely. The reason is measurable and was
    measured: **no handle was a field of a record declared INSIDE an `extern`
    group**, which is the only position the completeness probe reaches. Three
    handles ARE fields of plain Heroes records — two in `examples/ledger/`, one
    in `tests/golden/check/` — and those get no probe at all. (The first draft of
    this sentence said *not one anywhere in the tree*, and an adversarial review
    falsified it by grep within the hour. Ask yourself which of the two
    sentences you could have checked, and with what command.) The form shipped, was exercised by three real bindings and a mutation
    operator, and never once met the one position that breaks it.

    **The lesson is CL-061's, and this is the cleanest instance of it in the
    record.** A repair is attacked at the shapes NEXT to the one that provoked
    it — one field, none, padded, nested, tagged. A *form* is owed the same walk,
    and this one got its walk four days late because the walk was done over the
    positions somebody thought of rather than over the positions that exist. Ask
    yourself, when you read the case: what is the next position the handle form
    has still never met?
