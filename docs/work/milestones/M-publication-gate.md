# M-publication-gate — the last gate


The repository is **private** today and publishing is a hard stop that only the
author lifts (CLAUDE.md §14). This entry is the checklist that has to be true
first, and it exists because most of its items get worse the longer they wait.

**Overtaken in part on 2026-09-08 by `M-open-repository`** (author instruction;
this paragraph is written underneath rather than in place of the sentence above,
which was true when it was written). **The repository is public since
2026-09-08**, verified unauthenticated, and three of the bullets below were
banked by that milestone rather than by this one. **The licence re-check ran** and disagreed with what `NOTICE` said, which
is recorded there and in `vendor/tokenizers/README.md`. **The contribution
policy is in force**, and the finding is that neither half of it is a repository
setting: forking cannot be disabled on a public repository and pull requests
cannot be closed, so the policy is declared in `CONTRIBUTING.md` and a template
and `main` is protected instead. **The `main`-cannot-fail defect is measured**
rather than remembered, and it is in `docs/work/SCHEDULED.md (retired 2026-09-12)` with its three
exit codes.

**What this entry still owns is unchanged**: the thesis measured, the
compatibility paragraph and the `1.0.0` it governs, the trademark question, and
the outward act of the channels. Only `0.x`'s one sentence is published so far.
The gate is now the gate for *a finished thing being announced*, which was
always its subject, and no longer the gate for the source being readable.

**Already done, ahead of the milestone** (2026-08-11, because a repository
accumulates history and history cannot be relicensed retroactively): `LICENSE`
(Apache-2.0), `LICENSE-RUNTIME-EXCEPTION` — so a program compiled with Heroes
owes nothing for the runtime inside it — `NOTICE`, `README.md`, SPDX headers
across `runtime/`, and the attribution of the two vendored BPE tables.

**Still owed here:**

- **The thesis, measured.** Metric 2 has never run; §1.2's formula has two
  factors and only one is audited. The site and both books will state the claim,
  and stating it unmeasured publishes an opinion with a decimal point — the one
  thing §12 forbids, the author included. Not delegable: the held-out tasks must
  be author-written, or they measure the assistant's priors (panel 011).
  **Since 2026-09-15 both books follow the gate**, by author instruction, so at
  the gate it is the site alone that states the claim, and the books state it
  after.
- **A compatibility policy.** What v1 promises to somebody who writes code
  against it, in one honest paragraph. Silence reads as a promise.
- **The licence re-check on vendored material**, against the upstream
  repositories rather than against this project's recollection
  (`vendor/tokenizers/README.md` § Licensing).
- **The trademark question**, in the narrow form that actually applies: the name
  is a common word and does not worry anybody, but `site/`'s Aladdin Sane bolt is
  iconography attached to an actively managed estate. The style guide already
  keeps lyrics out; this is the other half, and it is cheaper to answer before
  publication than after.
- **Contribution policy in force** — the README's current answer ("issues yes,
  pull requests not yet") either stands or is replaced deliberately.
- **One defect that shows up on the second page of any tour**: `main` cannot
  fail, so a program that goes wrong still tells the shell it succeeded (queued
  from panel 030). Whatever M-ffi-ladder decides for `exit(code)`, this must not
  be true on the day the examples go up.
- **The outward act of M-install-channels**: the tap, the manifest, the flake and
  the image go where a stranger can reach them here and not before, and each is
  installed once more from its public address.
- **The version scheme is in force** — a number `heroes --version` prints and a
  formula can pin, with the compatibility paragraph above saying what it
  promises; `heroes 0.0.1` and milestone-named tags are what stood on 2026-09-03.
  **In force since 2026-09-07** (CLAUDE.md §14 § Release tags): `vX.Y.Z` tags,
  `v0.1.0` first, and what 0.x promises is one sentence. What this bullet still
  owes is `1.0.0`, which is the day the compatibility paragraph above is
  published and not before.
---
