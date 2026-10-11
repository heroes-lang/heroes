---
kind: defect
area: process
milestone: none
filed: 2026-10-11
commit: none
github: none
---

- [ ] **672 — a panel seat printed the author's GitHub token in a tool output, and nothing enforces a brief's *print no credential* in a subagent** | the ffi-pragmatist of panel 212 ran `gh auth token` without redirecting its output and the OAuth token (`gho_`, scopes `repo read:org user gist admin:public_key`) stood in the seat's transcript; the seat's prompt said *do not print any token* and the seat said so itself in the first lines of its report; `guard_bash.py` refuses two secrets in the assistant's own commands and does not see a subagent's; a search of the repository, every lane and every scratch folder for the shape of a token finds 0 files, so it is in session logs outside the repository and was sent to the API as conversation | `.claude/hooks/guard_bash.py` · `.claude/skills/panel/SKILL.md` · `docs/panel/212-reports/ffi-pragmatist.md` · **class: blocking**

    **Origin:** panel 212, 2026-10-11, the ffi-pragmatist's own disclosure; the completeness critic's second pass noted that no other report mentions it. The author is the only one who can rotate it: revoke at github.com/settings/applications, then `gh auth login`. `gh auth refresh` does not rotate a token.

    **Class: blocking**, 2026-10-11: a leaked credential is a robustness failure whatever the work (`.claude/rules/verification.md` § Bounded discovery), here one with write access to every repository of the author's.

    **What a repair owes**: a refusal in `guard_bash.py` of `gh auth token` in any shell the assistant types, and the rule in `/panel` that a seat which must authenticate runs `gh auth status`, which masks the token, and never `gh auth token`; the second is the author's to give, since the skills are amended by author instruction (CLAUDE.md § 4). A seat's subagent shell is not covered by the hook, which is the part of the defect no line of this repository closes: a brief that hands a seat a credential-bearing command at all is the thing to avoid.

    **2026-10-11, lane panel-212**: the hook half is repaired at `ca3d3532` (`gh auth token` is refused in any shell the assistant types, nine cases with defect 671's). **The item stays open**: the token has not been revoked, which is the author's act, and the rule in `/panel` that a seat which must authenticate runs `gh auth status` is the author's to give; neither is a line of this repository.
