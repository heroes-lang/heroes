- [ ] **M-deferral-ledger 8** | Bind `fopen` and `perror`, open the path `"C:\temp\report.txt"`, and look at the bytes the program writes with `od -c`. Count how many escapes were eaten. Then say what happens when those bytes reach a terminal instead of `od`, and why that makes this wart different in kind from a string that is merely wrong.

    **Where to look:** the reproduction in
    `docs/panel/143-one-wart-is-an-inconvenience-and-the-other-erases-the-evidence.md`
    § What was measured; `spec/heroes-spec.md` § 2's six escapes; and the emitted
    C through `--emit-c`, which is where the second surprise is.

    **Why it matters:** the language injected a control byte into a string the
    author transcribed, and that byte is the one that moves a terminal's cursor. So
    the diagnostic C hands back — the only line that names where the failure is —
    overwrites itself on screen. A wart that is invisible in the source, invisible
    in the emitted C, and that erases its own error message is not the same animal
    as one you can see and shrug at, and Part 8 is a list of costs somebody
    consciously accepted.
