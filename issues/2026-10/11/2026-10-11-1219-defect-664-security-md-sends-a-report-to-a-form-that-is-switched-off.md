---
kind: defect
area: none
milestone: none
filed: 2026-10-11
commit: self
github: none
---

- [x] **664 — SECURITY.md sends a report to a form that is switched off** | `SECURITY.md` § Reporting says *use GitHub's private vulnerability reporting, on this repository's Security tab*, and the issue chooser's contact link *A security report* lands on the policy page; measured on 2026-10-11 with the GitHub API's `private-vulnerability-reporting` endpoint for `heroes-lang/heroes`: `{"enabled":false}`. So a reporter who follows the policy finds no form on that tab, and the fallback the policy names, a public issue with no detail in it, is the only channel that works. The policy has read this way since M-open-repository step 4 (2026-09-08), and nothing in the tree can see a repository setting. The repair is the setting, not the sentence: enabling the form with the same endpoint's `PUT`, an outward act asked of the author, and reading it back | `SECURITY.md` § Reporting · `.github/ISSUE_TEMPLATE/config.yml` (the contact link) · the repository's Settings, Code security and analysis, Private vulnerability reporting · **class: blocking**

    **Origin:** found by the coordinator on 2026-10-11, while writing `CODE_OF_CONDUCT.md`'s reporting route, which names the same form; the author's request that day was GitHub's community checklist at 100%.

    **Class: blocking**, 2026-10-11: a false message, the public policy promising a private channel that does not exist; nothing in the compiler moves.

## The repair

The setting, not the sentence. With the author's yes of 2026-10-11, the
coordinator ran the GitHub API's `PUT` on the repository's
`private-vulnerability-reporting` endpoint and read it back the same minute:
`{"enabled":true}`. `SECURITY.md` is unchanged, because the channel it names is
the right one once it exists; what the tree could not see is now written down
here, and `CODE_OF_CONDUCT.md` names the same form as one of its three routes.

**Closed 2026-10-11** in the commit that ticks this item, which also adds
`CODE_OF_CONDUCT.md`.
