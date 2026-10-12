---
kind: decision
area: none
milestone: none
filed: 2026-10-11
commit: self
github: https://github.com/heroes-lang/heroes/issues/2006
---

# Code contributions open to authorized contributors, through an issue first

2026-10-11, two instructions of the author, meant as: *let us reason about
opening outside contributions a little, but bind them to pull requests: an
issue first, then the pull request that answers it, because the language is
still in development and follows a roadmap*; and, minutes later: *only
authorized contributors may do this; whoever wants to must ask me to be
authorized*. Then *prepare it*, and *put it in 0.3.0*. Process and
outward-facing text, so the author's instruction and no sitting (CLAUDE.md
§ 4).

**The question.** Whether the repository takes code from outside, from whom,
and on what terms, while the version is below `1.0.0`.

**The default until today.** *Issues and questions are welcome. Code is not,
yet*, declared on 2026-09-08 at M-open-repository step 4 in `CONTRIBUTING.md`,
`README.md` § Contributing and the pull-request template, with three
conditions named for the door to open: the language stops moving under a
patch, the process is reachable from outside, the rules a patch must satisfy
are checkable before it is sent.

**Measured 2026-10-11, before the change.** No rule of GitHub forced a pull
request: the one ruleset on `main` refuses deletion and non-fast-forward
pushes, no classic protection exists, forking is allowed, and nobody outside
has write access, so an outside change is a pull request from a fork by
construction. GitHub has no switch that refuses a pull request, so *authorized
only* is a policy the maintainer applies on reading, and the robust form of
the authorization is a fact GitHub shows rather than a list in a file: a
collaborator of the repository, whose pull requests carry that badge. The
Triage role gives no right to push and needs none. Interaction limits
(Settings, Moderation) can restrict pull requests to collaborators for up to
six months at a time, a lever for an emergency and not a policy, so none is
set. No page of the site states the policy (`grep` over `site/src`), so the
change publishes nothing. The three conditions of 2026-09-08, read again: the
first is met by the issue, which ties a change to a row of the chain and lets
the maintainer say when; the second stays inside, a sitting being the
maintainer's to convene, and the issue is the door to ask for one; the third
is the list `CONTRIBUTING.md` § What a pull request carries now gives.

**Resolution.** Code is welcome from authorized contributors, through an issue
first. Authorization is asked once, with the form *An offer to contribute*
(`.github/ISSUE_TEMPLATE/contribution.yml`), and given by the maintainer
adding the person as a collaborator with the Triage role, an outward act and
the author's each time. Then, for every change: (1) an issue, a defect, a
question or an offer; (2) the maintainer's answer in it, *accepted* with its
place on the chain, *later* with its condition, or *no* with its reason, a
change to the language going through a sitting first; (3) the pull request
from a fork naming the issue, carrying its cases annotated, formatted at the
moment of writing, the three suites green, the seed untouched, English, no
second binary. A pull request from somebody not authorized, or with no
accepted issue, is closed with a link to `CONTRIBUTING.md` and its content
reopened as an issue. The maintainer merges with a merge commit and writes
the issue's card in `issues/`; Apache-2.0 § 5 covers the licence of what is
sent, so there is no agreement to sign. No ruleset requiring pull requests is
added: it would bind the maintainer's own batches, which land on `main`
directly, and adds nothing for people who cannot push.

**What moved.** `CONTRIBUTING.md` rewritten, its sections on the licence, the
book, what is useful, the conduct and the security kept; `README.md`
§ Contributing's paragraph; `.github/PULL_REQUEST_TEMPLATE.md` turned into the
checklist of what a pull request carries; the new form, and the comment of
`.github/ISSUE_TEMPLATE/config.yml`. The records that quote the old policy,
journal 037, the pages of M-open-repository and M-publication-gate and the
decision of 2026-09-08, stand at their dates.

## Corrected 2026-10-12

*No page of the site states the policy*, above, was wrong. Six passages in two editions did, read again on 2026-10-12 when the author asked for the site's text to follow the latest updates: the home's book box (*"Code is not accepted here yet, so there is no patch to send"*) and its footer (*"code contributions are not open yet"*), and the about page's book box (*"takes no code contributions yet"*), each in English and Italian; and a seventh in `site/src/llms.txt` (*"code contributions are not open yet, and CONTRIBUTING.md names the conditions"*), with an eighth outside the site, `README.md`'s *"no patch is taken"*. So the live site went on stating the old policy after this decision, until the correction is deployed. All eight are corrected in the commit that adds this section, and `site/src/lib/claims.ts` refuses the removed phrases from then on, `llms.txt` included, the way it already refused the sentences of a private repository.
