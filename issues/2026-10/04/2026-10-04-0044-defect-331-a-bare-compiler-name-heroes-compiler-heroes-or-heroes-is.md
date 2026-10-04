- [ ] **331 — a bare compiler name, `HEROES_COMPILER=heroes` or `-- heroes`, is checked in the working directory and started through the PATH, so the net can judge another compiler** | a compiler named without a `/` is checked for a file in the working directory and then started through `execvp`, which searches the PATH; where the PATH holds another `heroes`, the net judges that one; on the lane's machine none was on the PATH, so it failed loudly (unrun in the shape that misleads) | `tests/harness/shell.hero` (`some_compiler` and the start of a named program) · defect 316 · **class: adjacent**

    **Origin:** lane b10-harness, 2026-10-04, measured on its worktree at `bd1f168c` (its final reply's *Found beside*), beside 316.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a question about which program is judged, answered by the machine's PATH where the author named a file; no wrong verdict measured.
