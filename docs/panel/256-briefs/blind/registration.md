# Panel 256, the blind seat: the coordinator's registration

Written 2026-11-22 before 23:00 (`date` read 23:02 after the preflight) by the sitting's coordinator, **before the area's sessions are drawn and before
any session runs**. The runner (`run.sh`) refuses to start unless this file's sha256 is the one `RUN.md`
names, and copies it beside `run.log` as it starts. The rows, the decision rules and what the run does to the
resolution are `docs/panel/256-briefs/llm-ergonomist.md` as it stands at the commit that carries this file;
the spec-warden's predictions are its own, in `docs/panel/256-reports/spec-warden.md` under `## Predictions
registered before the blind run` (registered 22:14, S26 to S28 added 22:23), copied and hashed by the runner.

## The inputs, by hash

| file | sha256 |
|---|---|
| `arms/spec-now.md` (the frozen spec) | `15804f3fb52a62c208bc59efce76aa0e55a48435f879471bff968bd4f213c427` |
| `arms/arm-gc.md` | `99e080b00a996ad97ccdba4b3bef5ab1a3640e92b89700d6fdd1ab5a87dd728d` |
| `arms/arm-gi.md` (the resolution's § 11 sentence) | `2d14c084ed997ad3c11ab0c121891978d2acbe34cc3c428b87a65df04b6ca856` |
| `arms/arm-gr.md` | `32b0504b44f98852db86431e13a87078f0ce43e00b59e3491168120fb91db13f` |
| `tools/brief-session.md` (panel 254's, unchanged) | `724e8e0d656bb7b0daa51c4422337b2b993f44b6d8499d980fa7801a7652690b` |
| `tools/brief-verdict.md` (panel 254's, unchanged) | `52d37bb04ac8ab20ae12458f1f7550a3ffaa818d5e48cec88fe90359f8835a36` |
| `tools/task-t1-w.md` | `51db6a4032e8f5ac0fc4881f0a98e434d42ba061c198d966ba0fab17545bd061` |
| `tools/task-t1-m.md` | `b77ed6918820ed830f5ea9913dc3df3ecf2f01d45ca04d8a1f53d1908a4b2fb7` |
| `tools/task-t2.md` (seven programs) | `53f54c43e6ffb0336f5455e62543d25637c40091bc205b89852013b9c6d1277a` |
| `tools/task-t3.md` | `d5db90c92591c5f63a426494a444985184f992618ccdc6c50248baf7c9599c3b` |
| `tools/make_sessions.py` | `f4ff134e8463701c242f5e703e3ec700e1b5147248d4f82dfdeeed8ba06a14b3` |

## The coordinator's predictions (each a point and a range)

Made after the seats and the critic reported and after panel 254's scored run, which found every warden
count it missed missed low; so these are made knowing both, and before any session of this run.

**t1** (5 per arm): C1 `right(w)` 4 (2 to 5); C2 `right(m)` 5 (4 to 5); C3 `lost(w)` 1 (0 to 2); C4
`lost(m)` 0 (0 to 0); C5 D1's robustness reading holds (`lost(w) >= 1` and `lost(m) = 0`): **yes, weakly**,
the task's own words (*"Rings made before that wait are told once together"*) naming the trap.

**t2, the resolution's arm `gi`** (right of 5, programs 1 to 7): C6 5, 5, 5, 4 (3 to 5), 5, 5, **4 (2 to 5)**;
program 4 for the `@` on an `i64` told at the call, program 7 for a refusal told at `main`'s call through
two generics, the shape panel 155's llm-ergonomist and this sitting's historian object to.
**Program 7 on the other arms**: C7 `now` by the compiler 0 (0 to 1), by the frozen text 4 (3 to 5); C8 `gc`
4 (3 to 5); C9 `gr` 5 (4 to 5).
**D2**: C10 holds for `gi` (at least six of seven programs at 4 or more), for `gc` and for `gr`.

**t3** (5 per arm): C11 `gc` 4 (3 to 5); C12 `gi` 4 (3 to 5); C13 `gr` 5 (4 to 5). **D3**: C14 holds for all
three.

**The run**: C15 46 sessions, 0 voided; C16 the spend 11.5 USD (8.5 to 16.0), under the 19.50 ceiling.

**What the run does to the resolution**, as `llm-ergonomist.md` registers it: C17 no row of the
resolution read wrong by two or more of five `gi` readers; C18 D2 holds for `gi`, so Q2 is not reopened
toward `gr`.
