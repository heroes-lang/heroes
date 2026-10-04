- [ ] **M-closures-verdict step 1** | `--dump-ir` on a whole program prints `funcref` once for every place a function is used as a value. Run it on `examples/calculator/main.hero` and count them. Then run it on `selfhost/main.hero` and count them, and explain why the second number is not the compiler's total

    **Where to look:** selfhost/ir/print.hero § the .func_ref arm · selfhost/ir/emissions.hero
    **Why it matters:** the second number is 1 and the compiler has 190 modules — the reason is separate compilation, and mistaking it for a total is how a measurement goes wrong
