- [ ] **295 — a root file that does not exist is told *cannot read*, where no file of that name is what is true** | `check nosuch.hero`: exit 2, *error: cannot read `nosuch.hero`*, `cli/input`'s one message for every failure to read the root, where the file is absent (lane b9-notext's compiler, 2026-10-04) | `selfhost/cli/input.hero` · defect 274, which made `read_file` tell absence from a failure · **class: adjacent**

    **Origin:** lane b9-notext, 2026-10-04, reproduced on its compiler (`<scratchpad>/batch9/notext/report.md`, *Found beside* 7).

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

    Repaired at `9224fae3`, 2026-10-04 (lane b10-cli), gated by its cases and the compiler's own tests; the net is owed at the batch's close.
