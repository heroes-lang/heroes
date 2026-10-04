- [ ] **326 — an empty complaint from `pkg-config` is told as the note *pkg-config said:* with nothing after it** | a `pkg-config` that fails and writes nothing on its error stream: the package's `ffi_package` carries the note *pkg-config said:* and no words, a note that promises an answer and holds none | `selfhost/cli/package_answer.hero` (the notes written from pkg-config's complaint) · defect 285's reading of the answer's lines · **class: adjacent**

    **Origin:** lane b10-cli, 2026-10-04, reproduced on its compiler at `86b29733` (its final reply's *Found beside*).

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be: the note should say pkg-config gave no reason.
