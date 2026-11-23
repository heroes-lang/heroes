# Panel 257, the blind reading: the coordinator's registration. REGISTERED, NOT RUN.

Written 2026-11-23 by the sitting's coordinator at the synthesis (`date` read 16:43 on this Mac), **before any session is
drawn and before any session runs; none is drawn and none runs**: the author's order for the sitting is that no paid
run is authorized. A later coordinator who starts it, on the author's word, runs **this** file's inputs, by hash.
The rows, the decision rules and what the run does to the resolution are `docs/panel/257-briefs/llm-ergonomist.md`
as it stands at the commit that carries this file. The spec-warden's predictions are its own
(`docs/panel/257-reports/spec-warden.md`, `## Predictions registered before the blind run`, for its texts f1, f1b, f2,
f3 and w26), copied and hashed by the runner as it starts; **none of them is for the `adopted` text below**, which
the warden did not write (its nearest are `w27`, `spec-w12.txt`, and f3).

## The inputs, by hash (`shasum -a 256`, 2026-11-23)

The area is `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p257-scratch/257-llm-ergonomist/`: the top of an
empty git repository of its own (`git init`, `.gitignore` `*`, local user `reader` <`reader@invalid`>, **no commit**,
read by `git log`: *"your current branch 'main' does not have any commits yet"*), inside the repository's root and
ignored by it, **no `CLAUDE.md` in it** (`find <area> -maxdepth 3 -name CLAUDE.md | wc -l` printed 0; the repository's
own, above it, is kept out of a session by `--bare` and the preflight's scan, `llm-ergonomist.md`). It holds `arms/`,
`tasks/`, `programs/`, `home/`, `tmp/`; **no session folder, no key, no run**.

| file | sha256 |
|---|---|
| `arms/spec-now.md` (the frozen spec; `cmp` equal to `git show 828567a30:spec/heroes-spec.md`) | `15804f3fb52a62c208bc59efce76aa0e55a48435f879471bff968bd4f213c427` |
| `arms/spec-adopted.md` (`now` with `257-reports/spec-sentence.diff`, **the resolution's sentence**) | `387cd9da261948700d1d1793718c6e082da5ee4b35ca625245984c8f3dc27421` |
| `arms/spec-rival.md` (`now` with `257-reports/spec-x1.diff`, **what conservative-with-a-promise would be**: the same refusal side and a positive clause, *"a callback then runs"*) | `e809ed7491dccdaa3117ab04180208beeb4084ed5fec9d488d52363a3082c33b` |
| `tasks/task-t1.md` | `bc579e33a2ae31461a15a5009b03ba431692a22970d2fe72d54876838ef770d1` |
| `tasks/task-t2.md` | `fc28e03a8849bc4bc623a4f683239bc3bfd60baae967884f9fcd8cb9fdb6a265` |
| `programs/prelude.h` | `95d7d7d6618ae288cb943352a9ecc39fa18bb787492569ae8f761f37e8a1f9eb` |
| `programs/p1.h` ... `p7.h` | `9cadabccde1baaad...`, `174ff046ee2fb202...`, `d64041ce5ee8bbe0...`, `cb459e82fb0d4ac2...`, `f1a08b31e9d15ddb...`, `60001822e49272bb...`, `4b72842a94b5a54b...` (the full hashes are `shasum -a 256 programs/*.h` of the area) |

**Panel 256's runner files, as the lane holds them (the bytes the adapted runner is changed FROM)**, `docs/panel/256-briefs/blind/`:
`run.sh` `00ffe7ec55c38474...`, `check_folders.py` `a2512653cd5b4420...`, `check_area.py` `083089c60775136c...`,
`make_sessions.py` `f4ff134e8463701c...`, `preflight.sh` `4c745eed8efef180...`, `scan.py` `41aae8419d682a6d...`,
`read_runs.py` `d6fe89f5c79e6d93...`, `spent.py` `10c2a8f93476d39b...`, `brief-session.md` `724e8e0d656bb7b0...`,
`brief-verdict.md` `52d37bb04ac8ab20...`. **The five that must change** (`run.sh`, `check_folders.py`,
`make_sessions.py`, `preflight.sh`, `brief-session.md`) are changed by whoever starts the run, each with a `diff` against
these bytes written into a `registration-runner.md` beside this file **before** any session; `scan.py`, `check_area.py`,
`read_runs.py`, `spent.py` and `brief-verdict.md` stay these bytes.

## The cap (`llm-ergonomist.md` § The area, the runner, the spend)

0.60 USD a session, 1.00 for the verdict, margin 0.10, ceiling 12.00 USD; **31 sessions** (t1 and t2, three arms, 5
each, and the verdict); the verdict runs only if the 30 spent at most 10.90 USD. Panel 256's measured run is the
price: 10.775607 USD over 46 sessions, the graded 0.171964 to 0.377992 each. **The author's to change; none given.**

## The coordinator's predictions (each a point and a range), made knowing panel 256's scored run only by its report

**t1** (right of 5), arm `now`: p1 4 (3 to 5), p2 4 (3 to 5), p3 3 (1 to 5), **p4 1 (0 to 3)**, p5 5 (4 to 5), p6 5
(4 to 5), **p7 1 (0 to 3)**. Arm `adopted`: p1 4 (3 to 5), p2 4 (3 to 5), p3 4 (2 to 5), **p4 4 (3 to 5)**, p5 5, p6
5, **p7 4 (2 to 5)** (p7 graded per platform; where the landed builds split it is out of D1). Arm `rival`: as
`adopted` and p3 5 (3 to 5). **t2**: question 1 (does the function run) right 4 (3 to 5) in every arm for A and C,
and for B 1 (0 to 3) under `now`, 4 (2 to 5) under `adopted`; question 3 (*neither*) **under `adopted` 2 (1 to 4)**.
**D1 holds** for `adopted`; **D2 holds**; **D3 fails for `adopted` and for `rival` alike** (the sentence says what
may abort and not whose mistake it is; the words in the task are the same in all three arms): **the sitting expects
the blind reading to say the sentence is silent on blame, and records that as the one thing it does not teach**.
**The run**: 31 sessions, 0 voided, **7.0 USD (5.2 to 11.3)**.

**What the run does to the resolution**: as `llm-ergonomist.md` registers it. D2 failing withdraws the sentence; D1
failing reopens it; D3 failing, as predicted, **adds a clause on blame to a later sitting, never to this one**,
because the only true words about blame are the runtime's (they are printed in the task) and the lane has not argued
them true of every refusal (`resume.md`).
