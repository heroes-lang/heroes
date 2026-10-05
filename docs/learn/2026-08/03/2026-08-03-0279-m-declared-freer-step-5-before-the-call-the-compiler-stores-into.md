- [ ] **M-declared-freer step 5** | Before the call, the compiler stores `nullptr` into the hidden cell. Delete that store in your head and answer three things: what the C local holds instead; what `free` is then handed; and why running the program — at `-O0`, at `-O2` and under `-fsanitize=address,undefined` — did not notice, on the day it was measured

    **Where to look:** selfhost/ir/inout.hero § argument · tests/golden/ir/owned-cell.hero · docs/panel/116
    **Why it matters:** a guard no run can falsify is the kind that gets deleted by somebody reading a green test
