- [ ] **M-robustness-guards step 1** | `add(@bs[0], "y")` on a cell now compiles to a READ of the element into a temporary, the call with the temporary's address, and a STORE back through `bs[0]` after the call. Count the instructions `ir/inout.hero` adds to `main` for that one line by reading `--dump-ir`, and say which of the three would be missing if the index expression were lowered twice

    **Where to look:** selfhost/ir/inout.hero (`argument`, `element_value`, `write_back`) · `heroes build tests/golden/run/inout-through-paths-of-a-cell.hero --dump-ir`
    **Why it matters:** the index is evaluated once, and `pick` printing once in that golden is the proof
