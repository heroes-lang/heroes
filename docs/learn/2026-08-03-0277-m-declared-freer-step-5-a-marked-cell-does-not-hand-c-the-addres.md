- [ ] **M-declared-freer step 5** | A marked `@` cell does NOT hand C the address of the binding you wrote. Open `ir/inout.hero`'s `argument` and read the six lines under the mark test, then say which of these is what C is given: (a) `&problem`, the author's own `str?`; (b) `&$cell0`, a hidden `cstr` the compiler made; (c) a copy of the string. Then say what would go wrong with the one you did not pick

    **Where to look:** selfhost/ir/inout.hero § argument · tests/golden/ir/owned-cell.expected
    **Why it matters:** the whole design is that two different cells exist, and every other question about the feature follows from seeing that
