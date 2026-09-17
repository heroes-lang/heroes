- [ ] **M-check-completeness walkthrough** | A value can be fallible twice, and four operations read one level of it. Panel 160 refused exactly ONE of them. **Before reading the resolution: here are the four, with what each hands back — say which one is the problem, and why the other three are not.** | `spec/heroes-spec.md` § 6's table · `docs/panel/160-the-reader-that-forgets-the-level.md`

    On a `m: {str: i64?}`, the expression `m["a"]` is an `i64??` — fallible
    twice, because the map read can fail (no such key) and the stored value can
    fail (the parse that made it). The four readers:

    ```
    m["a"].must()        hands back an i64?
    m["a"]?              hands back an i64?
    m["a"].default(v)    hands back an i64?, and types v against it
    m["a"].is_err()      hands back a bool
    ```

    **Where to look after answering:** the synthesis's § *THE HINGE*, then the
    table of thirteen SQLite probes under it.

    **Why it matters.** Three of the four leave the second level in the TYPE, so
    a program that got the depth wrong fails at the next line and the compiler
    says where. The fourth returns a `bool`: the level is gone, and
    `if m["a"].is_err()` compiles, runs, and answers *was the key there* on a
    line that reads *did the stored value fail*. The asymmetry is exactly one
    reader wide, and the refusal is exactly one reader wide.

    **The question to carry away.** One seat wanted all four refused and had
    built a compiler to prove it. Its own probes, run against the narrower
    option it had NOT built, refuse every program it was worried about — and the
    one mistake the narrow option lets through aborts at 134 anyway. Ask what
    that says about a recommendation made from prose about the alternative, and
    what a sitting would have to do differently to catch it. Then ask why the
    coordinator found it and not the five judges.
