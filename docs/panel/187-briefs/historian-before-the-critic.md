# Panel 187, the historian's brief

Read `00-shared.md` first. Your evidence is the web; every claim of
precedent carries its source (URL, version or date). Unsourced precedent is
inadmissible.

## Your task
How did production compilers bound their error recovery, and how did they
define it done? At least, each sourced: rustc (its parser's recovery and
the "cascading error" suppression, `-Z treat-err-as-bug`, the error budget
if any), Swift's and TypeScript's parsers (best-effort recovery, the
suppression of follow-on errors), Go (`go vet`/the compiler's 10-error
limit and `-e`), GCC and Clang (`-fmax-errors`, `-ferror-limit`, the
"in file included from" chains), Elm (one error at a time, by design), and
any project that measured recovery quality with a mutation instrument the
way this one does. Then: which of Q1's routes has a precedent that ran for
years, which has none, and what users of each did when recovery fell short.

Write `docs/panel/187-reports/historian.md` in the trunk as you go: a verdict
per route (approve, object; no veto), the precedents with sources, a
falsifiable prediction and the condition that would change your verdict.
English, no em dashes.
