---
kind: learn
area: process
milestone: M-generics-library
filed: 2026-08-03
commit: none
github: none
---

- [ ] **M-generics-library step 4** | The dumps show the user's program and the emitted C shows the library. Task: say why that asymmetry is right, using CLAUDE.md §10's own sentence about what a `--dump-<stage>` answers — then say what `--emit-c` would look like with seven library functions and no reachability walk

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/builtins.rs (`reachable`) · archive/bootstrap-rs/heroes/src/ir/print.rs
    **Why it matters:** one artifact answers a question about the file, the other has to build a binary
