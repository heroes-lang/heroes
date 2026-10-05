- [ ] **M-separate-compilation step 7 (what a compiler asks the compiler)** | The repair does not add the header to the key. It asks clang, on every compile, "which files did you open?" (`-MD`), records each one's content digest beside the object, and re-checks all of them before reusing it. Three choices were available: (a) hash the header names the `extern` groups write down, (b) run the preprocessor first so the key can contain the answer, (c) what was done. Say what (a) misses, and what (b) costs — the numbers are in the module header

    **Where to look:** selfhost/cli/deps.hero (WHY CLANG'S OWN LISTING, WHY BESIDE THE OBJECT) · docs/panel/093 R4
    **Why it matters:** the sitting named the goal and not the mechanism, and the mechanism was where the whole cost was
