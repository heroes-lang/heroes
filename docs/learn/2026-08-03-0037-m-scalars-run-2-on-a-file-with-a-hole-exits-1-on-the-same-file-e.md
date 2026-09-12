- [ ] **M-scalars-run.2** | `heroes build` on a file with a hole exits 1; `heroes check` on the same file exits 0. Both are right. Say what question each command is answering

    **Where to look:** archive/bootstrap-rs/heroes-cli/src/commands/compile.rs · §4.16
    **Why it matters:** the ergonomist measured exit 0 producing a `build && ./artifact` loop that runs yesterday's binary
