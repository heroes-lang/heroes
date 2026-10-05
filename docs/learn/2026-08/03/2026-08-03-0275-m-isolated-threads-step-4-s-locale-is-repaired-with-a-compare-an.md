- [ ] **M-isolated-threads step 4** | `f64.c`'s locale is repaired with a compare-and-exchange and NOT by making it `_Thread_local`, which would have been one word. Two threads, both printing a number. Walk what each does under each of the two repairs, and say how much memory a program with a thousand short-lived threads loses under each

    **Where to look:** runtime/parts/f64.c § the one the sweep found
    **Why it matters:** the shorter edit is the wrong one here, and the reason is a shape that recurs: per-race and per-thread are different denominators
