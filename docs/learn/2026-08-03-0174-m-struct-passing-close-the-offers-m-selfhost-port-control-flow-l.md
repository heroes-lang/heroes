- [ ] **M-struct-passing close — the offers** | M-selfhost-port, control flow lowered (ir_lower.hero) | **A for loop is four blocks and a while three — and the fourth is the interesting one.** Why does `continue` inside a for land on the STEP block instead of the test? What would `for i in xs { if skip(i) { continue } ... }` do if it landed on the test?

    **Where to look:** ir_lower.hero::for_loop, the continue test
    **Why it matters:** an infinite loop from one wrong jump target — the step block IS the difference between the two loop kinds
