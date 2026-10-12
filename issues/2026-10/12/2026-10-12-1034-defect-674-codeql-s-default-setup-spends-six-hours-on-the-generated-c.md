---
kind: defect
area: ci
milestone: none
filed: 2026-10-12
commit: none
github: none
---

- [ ] **674 — CodeQL's default setup spends six hours on the generated C and never finishes, so code scanning stopped** | GitHub's code-scanning default setup, switched on 2026-10-11, has no build to follow, so it extracts C and C++ with none: every `.c` and `.h` of the tree, 710 and 604 files at `808bb4238`, about 96 MB of them generated (`seed/heroes.c` 52.0 MB, the 599 files of `tests/emission/` 44.2 MB) against 0.7 MB of hand-written runtime. Run 38131878650's `Analyze (c-cpp)` was cancelled at GitHub's six-hour limit while still extracting, the run ended `cancelled`, its `Adjust Configuration` job was skipped, and no analysis has run since: the eight alerts batch 22 repaired stand open on `808bb4238`, one of them `high`, and the runtime has never been scanned | `https://github.com/heroes-lang/heroes/actions/runs/38131878650` · the repository's settings, Code security, CodeQL's default setup · `.github/workflows/`, which holds no CodeQL workflow · **class: blocking**

    **Origin:** the author, 2026-10-12, sending a screenshot of the cancelled job with *check this problem*; the coordinator read the run, its jobs, the five analyses, the alerts and the setup over the API the same morning (`gh run view 38131878650`, `code-scanning/analyses`, `code-scanning/alerts`, `code-scanning/default-setup`), and counted the C at the analysed commit with `git ls-tree -r -l 808bb4238`.

    **Class: blocking**, 2026-10-12: a red CI, and a false page in public. The analysis of the repository's C failed, and the eight alerts the security page lists as open were repaired at `a1ec2eb9` and stay open because no analysis has run to read the repair.

    What was measured, 2026-10-12. The job ran from 10:25:13Z to 16:25:23Z on 2026-10-11 and its log ends at *Running extraction commands*, then *The operation was canceled*; which file held it the log does not say, so the culprit is a question (unrun). The four other languages finished in one to two minutes each. `code-scanning/default-setup` answers `state: configured` with `languages: []`, `updated_at: null` and `schedule: null`, and the six pushes to `main` that started the `heroes` workflow after the run, the last at 04:55Z on 2026-10-12, started no CodeQL run; that the skipped `Adjust Configuration` job is why is an inference (unrun). No organisation configuration governs it (`code-security-configuration` answers 204), so the switch is the repository's.

    The repair measured on this Mac the same morning with the CodeQL CLI of the runner's own release, 2.27.2, its checksum the published one: built with the compiler from the seed before tracing starts, then tracing one `heroes build` of `examples/gallery/00-first.hero` in a copy of the tree with a cold cache, CodeQL saw the runtime alone, `runtime/runtime.c` with its 25 parts and seven headers, compiled with the compiler's own words (`-std=gnu11`, `-fsigned-char` and `-Werror=macro-redefined` read in its log), in 10.48 s; the `cpp-code-scanning` suite took 6.78 s over 33 files and reported 0 results. That read the `__APPLE__` branches of the runtime; a Linux leg reads the `__linux__` ones, unrun. The switch from the default setup to a workflow is a setting of the repository, an act on GitHub and the author's.
