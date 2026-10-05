- [ ] **M-declared-extents golden ratification** | `tests/golden/run/ffi-a-lent-field-reads-through-const.hero` prints five numbers. Before running it: a `=` binding `t` is lent twice to a C function that only reads, with a `@` cell `u` filled by C in between. **Which of the five lines could change if the lend from `t` were still `void *`, and which instrument would notice?** | `tests/golden/run/ffi-a-lent-field-reads-through-const.hero`, `tests/golden/fixedbugs/ffi-a-lent-field-c-would-write.hero`

    **Where to look after answering:** none of them — the program is legal
    either way, and that is the point. This golden guards the half the route's
    first landing BROKE, the read from a `=` binding, which must keep
    compiling. The half the defect was about is guarded by the `fixedbugs/`
    case beside it, which must be REFUSED, and by its `surface` row asserting
    the sentence the author sees.

    **The question to carry away.** A repair owes a witness for what it
    forbids and a witness for what it must leave alone, and here they are two
    files in two directories. Ask which of the two the first landing would
    have passed.
