---
kind: task
area: none
milestone: none
filed: 2026-10-11
commit: self
github: https://github.com/heroes-lang/heroes/issues/2005
---

- [x] **the community checklist** | GitHub's Community Standards page at nine items of nine: `CODE_OF_CONDUCT.md` at the root, the Contributor Covenant 3.0 taken verbatim from its upstream source (the `release` branch of `EthicalSource/contributor_covenant`, commit `d379fc94`, 2025-08-04) with its two adopter notes filled in, the means of reporting being GitHub's own channels and no email (the author's choice of 2026-10-11) and the enforcement ladder adopted as written with the one maintainer as its moderators; a `## Conduct` section in `CONTRIBUTING.md`; and the two repository settings the checklist's last two items name, private vulnerability reporting (defect 664, the author's yes) and reported content for all users (the author's click). Nothing under `site/`, `examples/` or `spec/` moves, so the push publishes no page | `CODE_OF_CONDUCT.md` · `CONTRIBUTING.md` § Conduct · `SECURITY.md` § Reporting · the repository's Insights, Community Standards

    **Origin:** the author's request of 2026-10-11, meant as: *GitHub tells me the project lacks a code of conduct*, then *take it to 100%*, and *the principles I draw on are the most democratic, left-leaning and open-source ones*. Measured that morning before anything moved: the community profile API at 75 with `code_of_conduct` null, the public page at seven items added of nine, *Code of conduct* and *Repository admins accept content reports* not; and no file of the record had ever named a code of conduct, so it was never refused, only never raised. The Covenant's 3.0 is the current release (2025-07-28), drafted by consensus and written around repairing harm rather than punishing it, which is what the author's words asked for.

    **Closed 2026-10-11** in the commit that writes this file, the two settings read back first (`{"enabled":true}` for the form; the page's banner *Content reporting enabled for all users on this repository* for the reports). The checks over the tree before the commit: `records` and `unseen`, counts in the commit's body.

    **Corrected 2026-10-11, 13:01:** *for all users* above was true when written and is not the setting that counts. At 13:00 the public page and the API still read the content-reports item as not added, with the setting at *All users*; the author changed it to *Prior contributors and collaborators* and the item turned green at once. Measured at 13:01: the page at 9 items added and 0 not, the API's `content_reports_enabled` as printed in the commit that adds this line. `CODE_OF_CONDUCT.md`'s reporting route says *collaborators and prior contributors* since that commit.

## What was done

GitHub's Community Standards page reached nine items of nine:
`CODE_OF_CONDUCT.md` at the root, the Contributor Covenant 3.0 taken verbatim
from its upstream source with its two adopter notes filled in, a Conduct
section in `CONTRIBUTING.md`, private vulnerability reporting enabled, and
content reports enabled, set to prior contributors and collaborators after
the first setting left the item unticked; both settings were read back before
the commit. Landed in this file's own commit (`self`).
