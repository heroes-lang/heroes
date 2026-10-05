- [ ] **M-ir-lowering.1** | `function step(@r: Reader, ...) -> int?` with a `?`, an early `return` and a tail: how many `copyout` instructions does the dump show, and in which blocks?

    **Where to look:** tests/golden/ir/adversarial-try-copies-out.expected · ir/verify.rs (check_copy_out)
    **Why it matters:** §4.8's "copy-out happens always" is the sentence panel 000 called the place this project would stall
