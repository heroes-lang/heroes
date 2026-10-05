- [ ] **M-struct-passing close — the offers** | M-harness-port | The harness runs the compiler through `system()` and reads two files back. Open `tests/harness/shell.hero` and answer: why is the command wrapped in `( … )` before the redirection? What does `printf a; printf b > out` put in `out` without it?

    **Where to look:** tests/harness/shell.hero, its `run` function (2026-08-26)
    **Why it matters:** a shell redirection binds to one command, not to a list — and the harness lost half its output to this before its own tests caught it (the harness as it stood 2026-08-26)
