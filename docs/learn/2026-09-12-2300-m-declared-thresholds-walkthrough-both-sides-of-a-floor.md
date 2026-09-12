- [ ] **M-declared-thresholds walkthrough** | Read `tests/harness/floors.hero` and say what the check does that the eleven suites were not doing before, in one sentence — then say why it needed no new list of floors anywhere.

    **Where to look:** `tests/harness/floors.hero`, and any suite it now calls
    from, for instance `tests/harness/suite_layout.hero`'s `run_all` or
    `tests/harness/suite_canonical.hero`'s.

    **Why it matters:** the shape of the repair is the point, not the check. The
    suites already counted the real number — every one of them, on every run —
    and compared it to a floor to catch a walk that found nothing. Nothing was
    missing except the other direction of the same comparison. A version of this
    that kept a register of floors would have been more code, a second thing to
    forget, and a list that goes stale exactly the way the floors did.
