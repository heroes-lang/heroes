## What production-ready means, and who owns each part

**The yardstick the chain is judged by, written 2026-09-10 because it did not
exist.** design.md defines v1 as *"the language is finished for v1 when it can
compile itself"* (`:114`, reached 2026-08-18) and defines production readiness
**nowhere**: a sweep of all 3,612 lines found no section on distribution,
versioning, stability or a 1.0 promise, and no occurrence of semver, deprecation,
backward compatibility, release cadence, LTS or an installer. The only `1.0` in
the file is **Go's** (`:3202`). So every row of § The chain had been scheduled on
its own reason and none had ever been measured against a standard.

**The standard is not a mature language**: the question is whether a stranger can
put **production code that is not mission critical** in Heroes, which is a lower
bar than either half of that phrase suggests on its own. **And it is a list, so
it is a measurement** (CLAUDE.md § RUN IT): the
ten rows come from what Go 1, Nim 1.0 and Rust 1.0 each shipped, read against
what this tree already reaches for. **Six of the ten were already owned**, which
is the finding.

| | what a production user needs | who owns it | state on 2026-09-10 |
|---|---|---|---|
| 1 | a **written promise** about what keeps compiling | M-compatibility-promise | the paragraph was owned, **the instrument was not** |
| 2 | **getting the compiler** | M-install-channels | owned |
| 3 | **the machines it runs on** | M-arm-platform | three legs, and **arm64 Linux unowned** |
| 4 | **shipping the binary you built** | M-deployable-binary | **owned by nobody** |
| 5 | **depending on other people's code** | M-core-packages · M-package-manager | owned; pinning is that sitting's |
| 6 | **the capabilities a real program needs** | M-core-packages' package table | owned in the large, silent in four places |
| 7 | **diagnosing a failure in production** | M-panic-location · M-typed-inspection | owned |
| 8 | **not falling over** | M-generated-programs · M-check-completeness | owned |
| 9 | **editing it** | M-lsp-server · M-vscode-extension · M-doc-generator | owned, and **the colouring had drifted** |
| 10 | **knowing the claim is true** | M-thesis-harness · both books | owned |

**Row 6's four silences** are `docs/work/SCHEDULED.md (retired 2026-09-12)` items at M-core-packages —
text to a number, width and precision, a signal handler that cannot record that
it fired — except the fourth, a release bound to a scope, which is a question
about the language and became **M-cleanup-verdict**. **Row 9's drift** was the VS
Code grammar four days behind `f"…"` while the site's highlighter kept up, with
nothing judging either: the repair is an item at M-vscode-extension and the rule
that prevents the next one is `.claude/rules/diagnostics-and-goldens.md`'s walk,
widened the same day.

**The thirteen candidates this reading refused, each with the rule that refused
it, are in `docs/work/DONE.md`** with the five `DESIGN-LOG.md:539` had already
refused on 2026-09-03 — because that entry's own stated reason is *so the
candidate is not proposed again as new*.

---

*******************************************************************************
