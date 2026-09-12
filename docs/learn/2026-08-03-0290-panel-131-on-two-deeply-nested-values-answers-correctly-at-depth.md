- [ ] **panel 131** | `assert a == b` on two deeply nested values answers correctly at depth 100,000 today. A prototype that says WHERE they differ died at depth 3,700. Both walk the same data. What is the difference?

    **Where to look:** `HeroEqWork` at `runtime/parts/array.c:185-190`, and how
    the outermost call drains it.

    **Why it matters:** panel 117 adopted a repair on the sentence *"the walk is
    `eq`'s walk"* and panel 131 found it false. `eq`'s worklist carries
    `{a, b, elem, len}` and nothing else: no path, no parent, no index. An
    explicit worklist has no stack depth to run out of; a walk that must report
    *where* needs a C frame per level unless the path is carried in the list
    too — and that is the difference between the two numbers.
