# Panel 187, the historian's brief

Repaired 2026-10-02 after the completeness critic's first pass
(`docs/panel/187-reports/completeness-critic-briefs.md` § 5); the text it
read is `historian-before-the-critic.md`. Every precedent below is a CLAIM
TO VERIFY, not a fact handed to you: the coordinator wrote these names from
memory, with no version and no date.

Read `00-shared.md` first. Your evidence is the web; every claim of
precedent carries its source (URL, and a version or a date). Unsourced
precedent is inadmissible, and a characterisation you cannot source is
reported as unsourced, not repeated.

## Your task
How did production compilers bound their error recovery, and how did they
define it done? For each, find what the project's own documentation or
source says, with the version it is true of:

1. **rustc**: whether its parser suppresses follow-on errors after a failed
   construct, by what mechanism and since when; what `-Z treat-err-as-bug`
   is documented to be for (it may be a debugging flag rather than a bound
   on recovery); whether any error budget or limit exists.
2. **Swift and TypeScript**: what each parser documents about recovering
   after an error and about suppressing errors that follow from one; the
   versions.
3. **Go**: whether the `gc` compiler stops after a fixed number of errors,
   the number, the flag that lifts it (`-e` is the coordinator's memory),
   and since which release. `go vet` analyses code that compiles; say
   whether it bears on parse recovery at all.
4. **GCC and Clang**: `-fmax-errors` and `-ferror-limit`, their defaults
   and versions. The "in file included from" chains are include context;
   say whether they bear on recovery.
5. **Elm**: whether its compiler reports one error at a time, for which
   phase (parse, types), in which version, and whether that is stated as a
   design choice.
6. **Measured recovery quality**: work that measured a parser's recovery
   over a corpus of broken programs (names to verify, from the critic's
   memory, unverified: Ripley and Druseikis 1978 on Pascal syntax errors;
   Diekmann and Tratt 2020, *Don't Panic!*, counting cascading errors on a
   Java corpus), and any project that measured it with a mutation
   instrument the way this one does (`00-shared.md` § The recovery
   instrument).
7. **Pinning every message a broken program gets**: rustc's UI tests and
   their `.stderr` files, clang's `-verify` with `expected-error` comments
   (names to verify), the precedent for route (1f).

Then: which of Q1's routes has a precedent that ran for years, which has
none, and what users of each did when recovery fell short.

Write `docs/panel/187-reports/historian.md` in the trunk as you go: a verdict
per route (approve, object; no veto), the precedents with sources, a
falsifiable prediction and the condition that would change your verdict.
English, no em dashes. No paid run: the web search and fetch tools only.
