- [ ] **M-declared-extents mutation drill** | The full net read **1862 passed, 23 failed** on a tree whose own ROADMAP said 1891 and 0, and the tree was right. **Before reading: two of the failures said `STALE: the recorded count is for 6bdb9b497a141864 and this file is 3c065c560426eb07`, and one said `no function named validated_bytes`. What single cause produces both?** | `.claude/rules/verification.md` § The compiler that judges is a build artifact

    **Where to look after answering:** `./heroes` was built at 02:08 and HEAD
    was 18:17. The binary was sixteen hours older than the tree it was
    judging. Rebuilt from the seed in **2.97 seconds**, the same net read
    **1891 passed, 0 failed**.

    **Why neither message pointed at it.** The old binary carried the OLD
    pinned digest inside itself and compared it against the NEW file, so it
    reported — correctly, from where it stood — that the *document* had moved.
    `selfhost/measure/pinned.hero:64` had held the right digest all along. And
    a golden using a built-in that landed after 02:08 fails as *no such
    function*, which reads as a missing built-in rather than as a compiler that
    predates it. **Every one of the 23 lines was a true sentence, and every one
    pointed away from the cause.**

    **Why `git status` could not help.** `heroes` is `.gitignore:15`. It is the
    one input to every gate that a clean tree says nothing about.

    **The question to carry away.** The project's contract says a defect is
    written down as the finder saw it, and that what it actually is becomes
    visible from the shapes beside it (CL-078). Here the finder saw *the spec
    is stale*. Ask which shape beside it was the one that settled it — and what
    you would have had to run to get there on the first try instead of the
    second.
