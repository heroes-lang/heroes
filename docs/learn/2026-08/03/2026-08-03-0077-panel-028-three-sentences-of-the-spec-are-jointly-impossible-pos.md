- [ ] **panel 028** | **Three sentences of the spec are jointly impossible.** `range(a, b)` positional · two same-typed parameters mean mandatory labels · `range` is written in Heroes. Task: say which of the three you would have dropped, then read why each of the other two exits was refused — and say what made this invisible for eleven weeks

    **Where to look:** spec lines 81, 113, 152 · docs/panel/028 § The finding
    **Why it matters:** nothing in the project checks the spec against itself, and the compiler could not, because `range` had no implementation to disagree with
