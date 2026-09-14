- [ ] **M-handle-verdict 8** | Four builds, and you have to do them in order. Write a two-module program in a scratch directory whose `extern` names a header you also write, and put a plain `static` function in that header that nothing calls — clang will say `unused function`. Build it, and see the warning. Now change that one word to `static inline` and build again: silent. Build a **third** time. Then a **fourth**. Before you run the third, predict what it prints. Then do the whole thing again with a compiler from before this milestone's step 8.

    **Where to look:** the comment above `kept_text` in `selfhost/cli/units.hero`;
    the sixth entry in `tests/harness/suite_cache.hero`'s numbered list, and the
    check `a warm cache never replays a warning that was repaired`;
    `selfhost/cli/deps.hero`'s module comment, which is about the *other*
    direction.

    **Why it matters:** a warm object replays the warnings its compile produced.
    That replay was added on purpose and for a good reason — without it a cache
    swallows every warning after the first hit, and a warning nobody sees cannot
    falsify anything. The saved file was written **only when clang said
    something**. So a clean recompile left its predecessor's text sitting there,
    and every later build replayed a compile that no longer existed. **A repaired
    program went on reporting its old warnings forever**, until somebody deleted
    the cache directory by hand.

    **Three things worth sitting with.** First, the defect lives only in the
    builds **after** the rebuild, so a three-build test would have passed — which
    is why the check does four. Second, it is the exact mirror of the case that
    was already there: that one asks whether a warm cache can turn a *failure*
    into a *pass*, this one whether it can turn a *pass* into a *failure*, and
    the same instrument catches both. Third, and this is the real lesson: the
    part that was **correct** looked guilty. `deps.fresh` was measured and it
    does its job — edit the header and the answer changes. The fault was in the
    one output the object file does not carry, which nothing was watching because
    nothing thought of it as an output at all.
