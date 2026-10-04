- [ ] **285 — a `pkg-config` that answers in CRLF lines has each line's carriage return shown as `<U+000D>` in the package's notes** | a stand-in `pkg-config` answering *Package zz9 not found* in CRLF lines: the package's message carries `note: Package zz9 not found<U+000D>`, the answer trimmed once before its lines are split (lane b9-harness, measured on this Mac, 2026-10-04, `<scratchpad>/batch9/harness/`) | `selfhost/cli/libraries.hero:318` (`strings.trimmed` of the whole answer, then `shell_split.shown` by line) · **class: adjacent**

    **Origin:** lane b9-harness, 2026-10-04 (its reply's *found beside*, measured), in the notext lane's file, so reported rather than repaired; read at `703af779` by the coordinator.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be, a line end shown as a code.

    Repaired at `ba8e6f67`, 2026-10-04 (lane b10-cli), gated by its cases and the compiler's own tests; the net is owed at the batch's close.
