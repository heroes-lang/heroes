- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the IR state | **Value id 0 is () in every function, defined by no instruction.** What breaks at the C level if a call to print HAD a destination? (The answer is one clang error, and raylib made it matter.)

    **Where to look:** selfhost/ir/build.hero::begin, ir.hero's call op
    **Why it matters:** a void call with a fake destination is the kind of plausible-looking IR that C refuses outright
