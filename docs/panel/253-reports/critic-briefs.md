# Panel 253, the completeness critic's first reading: the briefs against the world

Read 2026-11-19 from 14:55 CET (`date`), load 10.79 (`uptime`), before any seat started. Every command
runs in the critic's own copy, `.claude/worktrees/rebel-scratch/253-critic/tree/` (a `git archive` of
`181555e9a32d0b18878f9f10e48d2eadd939c285`, 20,037 files by `find -type f | wc -l` before its `.git`
file was written), or against the git objects of `.claude/worktrees/rebel-round`, never a working tree.
No paid run, no `claude -p` with a key, no API call that bills, no credential printed, no GitHub write,
no sub-agent, no commit. Temporary files under `.claude/worktrees/rebel-scratch/253-critic/`.

Kinds: **wrong** (a fact a brief hands a seat that a command contradicts), **unrun** (a claim with no
command behind it, or a negative or an *only* not searched), **route** (an option nobody listed),
**rule** (a rule of `SKILL.md` § 2, 3, 3b, 3c or CLAUDE.md a brief does not hand its seat).

Written as it goes; findings are numbered in the order found.

## Findings

### What held, so no seat re-pays for it

Each by its command, in my copy or against the git objects of `rebel-round`: the frozen commit's line,
`origin/main` `0becec5b6`, the merge base `f6c49d84c`, 3 commits `181555e9a..origin/main`; `git
ls-tree` names no `docs/panel/253*`; `3a5ee2691..181555e9a` is the three commits the brief names and
moves nothing under `selfhost runtime seed spec docs/design.md tests` (`git diff --stat`, empty);
`181555e9a..f4e3594d5` moves the two evidence files alone; 20,037 files; the spec 492 lines, 29,407 bytes,
`1d532d51...`, last moved 2026-11-15 (`46a42deb4`). Seed `sources 5aae00294f7cd6fa` (`real 9.64`);
**stage 2 `sources 23c7d2a626b29fd5`**, `real 129.99`, `user 95.97`, `sys 24.38` at load 12 to 14
(`work/stage2.log`). design.md §1.11 at 482, its box 490-492, 544, 547-560, the ruler at 578-579, the
convenience bullet at 668, the ten `standard library` lines; the spec's silence on a standard library
(`grep`, exit 1) and § 13's first sentence at 377; CLAUDE.md:278-279; `ffi-pragmatist.md:3`; the Zen
at `doctor.hero:254`, `:287`, `seed/heroes.c:365` and both editions' `project.html:95`; the site lines
cited in `why.html`, `why/faq.html`, `why/not.html`, `examples/packages.html`; the nine sittings
`038 041 080 097 100 127 162 234 241`; panel 234's admission test at 275-281; design.md:567's R15; the
milestone page's lines 15, 79, 308; spec 52-53; design.md 2601-2604, 3597; `cli-surface.md` 18-25 and
31-43; 16 `Command(` rows at 187-202; `doc` the seventeenth; the site's four *sixteen verbs* lines and
`claims.ts:264`; every `wc -l` of the compiler-engineer's Q2 list (310, 376, 155, 1,848, 169, 487, 40,
452, 115, 212); Rebel's § A7 at 308-397. Issue states by `gh issue view` (read-only): 808 open
(M-doc-generator), 819 and 853 closed, 1067, 2082, 2313 open, 2404 closed, 2551 open, 2587 open
`blocking`, 2589 open `blocking`, 2582 open `adjacent`, 2069, 2530, 2559 closed.

### 1. Task t6's input is the umb-r arm's own example: the `suffices` cell measures copying (wrong design; changes what the blind seat measures)

`253-llm-ergonomist/tools/task-t6.md` asks for a program over Rebel's `csv` and `json` at `0.1.0`, then
adds `use <rebel>/flags`. The arm the umb-r readers hold, `bw/arms/spec-umb-r.diff`, says *"`heroes add
rebel/csv` adds one, and a `use <rebel>/flags` it does not name is refused"* and adds to § 1's example
`[dependencies.rebel]`, `version = "0.1.0"`, **`packages = ["csv", "json"]`** (`cat` of the diff). So
every graded answer of the umb-r cell (the commands `heroes add rebel/csv` and `heroes add rebel/json`,
the entry with `packages` holding exactly `csv` and `json`, question 4 refused with `heroes add
rebel/flags`, `llm-ergonomist.md` § The grading) is printed in the reader's specification with the same
three names and the same version. `suffices` (at least 4 of 5 right) then measures whether a reader copies
an example, not whether the sentence lets a reader place packages it was not shown; it is the mirror of
panel 251's task 2 (a ceiling there, a lookup here). **Repair before the draw is spent**: either the
task names packages the arm does not (any two faces and a third, at another version, say `yaml`,
`template` and `time`), or the arm's example and sentence use names no task uses; the arms are hashed in
`tools/arms.sha256` and the folders in `key.txt`, so the repair re-runs `make_sessions.py` (which refuses
an existing `s/`) and re-registers both, and `spec-umb-r.md` re-measured if its words move.

### 2. Task t4 forbids the hazard its `now` row is registered to count (wrong design; the hazard reading has a ceiling)

`task-t4.md` item 1: the test *"where `test_server()` names none, **never reads as a test that
passed**"*. The registered hazard rule (`llm-ergonomist.md` § The decision rules, Q4) counts `now`
answers whose test reports `ok`, *"2 or more of 5 ... a reader of today's spec writes the silent pass"*.
The task tells every reader not to, so the count measures obedience to the task's sentence, and the
hazard the outcome exists for, Rebel's 106 tests that print a skip line and read `ok` (`00-shared.md`
§ Question 4, re-counted below), is the case no reader is put in. **Route**: a neutral wording (*"and
which, where `test_server()` names none, cannot check the connection"*), with the rows registered before
the run: under `now` the hazard is the count of `ok`; under `skip-r` right is the `skip` call; both arms
still told apart. If the explicit wording is kept, the brief should register that the hazard row
measures obedience and cannot read *"writes the silent pass"*.

### 3. Task t2's `needed` rule is decided by the task's words before any session runs (unrun as a measurement; spends five sessions)

`task-t2.md` asks for the example *"in the form the specification gives for an example"*; the frozen
spec gives none (`grep -n -i example spec/heroes-spec.md` in my copy prints one line, 47, a URL), and the
brief writes *"Expected right under `now`: none"*. So *"`needed` when `doc-r`'s right answers exceed
`now`'s by at least 3"* reduces to `suffices` with 3 in place of 4; only `doc-r`'s count and `now`'s
recorded defaults inform. Not a repair a seat waits for; the coordinator may cut `now`'s t2 replicates
or re-word the task (*"document `count_words` so its reader sees each parameter and an example of its
use"*) so `now` measures the default a reader writes, which is what decides whether the shape must be
in the spec.

### 4. `tools/scan.py` reads a session's own answer as a leak: 2 of panel 251's 51 transcripts, and the registered rule voids on it (wrong instrument; changes which sessions count)

Run over panel 251's 51 transcripts (symlinked from `251-llm-ergonomist/transcripts/` into
`253-critic/work/scan251/home/.claude/projects/`, read-only): **51 `prompt-alone=NO`, 51
`git-status=LEAK`** (a working positive control), and **2 `claude-md-or-memory=YES`** (`t1-x-4`,
`t3-c-2`). The match in both is an `assistant` entry, the session's own `context` answer: *"No other
rule, contract, memory or document (such as a CLAUDE.md or other files) reached my context"* (a
100-character window printed by my script). `scan.py:28` tests `'CLAUDE.md' in s` over every entry,
the assistant's included, and `llm-ergonomist.md` voids a session *"where `tools/scan.py` reads a
leak"*. **Repair**: test only entries that are not the assistant's (`type` not `assistant`: the
attachments, the user-role reminders, the tool results), and re-run this control. On the coordinator's
probe (`coord/blindtest/`) the scan prints `prompt-alone=NO`, because that probe's prompt was `Say
hi.` (read: the first user entry is a 7-character string); so **the scan has never been run on a
transcript holding the real prompt and no leak**: its negative control is unrun until the first
session, which the coordinator can read before the second.

### 5. The sessions write outside the repository's root: panel 251's runner left a file in `/private/tmp/claude-501/`, and 253's runner sets no temporary directory (rule; CLAUDE.md § Hard stops)

`/private/tmp/claude-501/cache-break-state-5ddf4b53-493b-415f-bb48-c0d524113ea3.json`, modified
10:17 on 2026-11-19 (`ls -la -t`), carries the `session_id` of panel 251's blind session `t4-f-2` (`grep
-l` over the 51 `run.json`, one match), which ran from 10:08 under `env -i` with no `TMPDIR`. 253's
`run.sh:82-83` passes `PATH HOME LANG GIT_CEILING_DIRECTORIES ANTHROPIC_API_KEY` and no temporary
directory either, and `check_area.py` asks nothing of it. The CLI 2.1.311 names `CLAUDE_CODE_TMPDIR`
(its error text: *"set CLAUDE_CODE_TMPDIR to a directory on another one"*, `strings` of
`/opt/homebrew/Caskroom/claude-code/2.1.311/claude`). **Repair**: `TMPDIR="$HERE/tmp"
CLAUDE_CODE_TMPDIR="$HERE/tmp"` on the `env -i` line (`mkdir -p tmp`), and after the run a `find
/private/tmp/claude-501 -newer run.lock` that prints nothing; **unrun**: whether those two variables move
that file (no session can be run here to say).

### 6. The blind runner logs a hash of a file that keeps growing, and its gate reads a heading, not a prediction (rule; the registration cannot be re-derived later)

`run.sh:43` refuses unless the spec-warden's report holds the heading line, and `:47` writes that file's
sha256 and modification time into `run.log`. The spec-warden writes *"as you go"* (`spec-warden.md:12`),
so the file the runner hashed no longer exists by the synthesis, and *"only those under its heading as the
runner hashed it"* (`llm-ergonomist.md` § The decision rules) cannot be checked against anything; and a
report opened with its section headings first (the shape *"written as you go"* invites) passes the gate
with nothing under the heading. Panel 251's failure was a timing nobody could prove either way until its
critic read a transcript. **Repair**: the runner copies the report as it stands into the area
(`warden-at-start.md`, beside `run.log`) and hashes the copy, and refuses unless a non-empty line stands
between the heading and the next `## ` line.

### 7. Q5's line-table routes are framed as if no build carried line tables; every build does, and Darwin's binary holds none of them (wrong framing; changes what the ffi-pragmatist's Q5.2 measures)

`selfhost/cli/flags.hero:172-175`, `debug_words`: `-O0` takes `-g`, every other level
`-gline-tables-only`, on every compile and link (`level_words`, `:214-229`), read in my copy. So *"the
binary's line tables"* (`00-shared.md` § Question 5, route W) already exist on every build, and the
ffi-pragmatist's *"what each costs in bytes of the binary (`ls -l` before and after)"* (`ffi-pragmatist.md`
Q5.2) has its *before* already paying for them. **But not in the same place on each platform**:
`bw/q5/deep.hero` built by my stage 2 on this Mac has no `__debug` section (`otool -l deep | grep -c
__debug`: 0, `dwarfdump --debug-line` empty) and a debug map of two `N_OSO` entries naming
`work/q5/build/runtime-....o` and `build/tu-.../deep-....o` (`dsymutil -s`), and no `.dSYM` is made (`ls`).
So on Darwin a run-time reader of lines needs the build folder beside the binary or a `dsymutil` step the
link does not take today; on Linux the ELF carries them (by the flags; not run here, no image started);
on Windows the PDB the link writes (`flags.hero:212`, *"clang's driver for Windows writes the `.pdb` only
when the link line carries a `-g` word"*). **Route nobody listed**: the
runtime writes the frames' return addresses alone, allocation-free, and `heroes run` and `heroes test`,
which are the program's parent, resolve them against the binary and its build's line tables; a binary run
alone shows addresses. The compiler-engineer prices it beside W, P and B.

### 8. Darwin arm64's frame records, stated from memory, settled for this Mac's clang; Linux x86-64 drops them (the brief writer's question, settled by command)

`clang -### -target <t> -O2 -c f.c` (Apple clang 21.0.0, `work/fp/`): `arm64-apple-macos`
**`-mframe-pointer=non-leaf`**, `aarch64-linux-gnu` **`non-leaf`**, `x86_64-linux-gnu` **`none`**, and
`flags.hero` passes no `-fno-omit-frame-pointer` (`grep`, no line). So at `-O2` a walk of frame records
holds on both arm64 targets by the driver's default and finds no records on x86-64 Linux, where W needs
the flag (its cost the compiler-engineer's to measure). The images' and the CI's clang are other builds:
their defaults are **unrun** by me.

### 9. Q3 hands the seats no word of the constant-time code Rebel already ships in Heroes, nor of the ruling Rebel wrote against ciphers in Heroes (a route nobody listed; the measurement route H should start from)

`packages/rebel/0.1.0/files/crypto.hero:15-21`: *"There is no encryption here, no cipher and no
signature: a cipher must compute in a time that its key and its text do not decide, and nothing in this
wave proves that of the C the compiler makes. A digest needs no such proof; the comparison
`equals_constant_time` is written with no branch on the bytes, and says how far that goes."*
`crypto/internal/compare.hero:1-6` adds *"What the C compiler makes of it is not proven here"*; the
function is a loop of `difference @ difference | to_i64(a[at]).must() ^ to_i64(b[at]).must()`
(`compare.equals`, `:10-20`, its loop at `:16-17`; `crypto.hero:567-568` returns it), named in 9 files (`grep -rln equals_constant_time`). So the question Q3's route H rests on,
*whether Heroes as emitted can keep a constant-time discipline*, already has a subject in the tree whose
name makes the promise: its emitted C and its `-O2` machine code (a bounds check per index, a `.must()`
per byte) are the first and cheapest measurement, and a secret-dependent branch found there is a defect
of the frozen tree, not of a prototype. Two spec facts the brief does not hand route H: the bitwise
operators are `i64` only and **every overflow aborts at every width**, `>>` copying the sign (spec lines
210 and 220), so a 32-bit add modulo 2^32 or a field limb product is written inside `i64` with masks, and
no wrapping operator exists. Also already in Heroes: `argon2`, `bcrypt`, `scrypt`, `blowfish`,
`sha256`, `sha512`, `keccak`, `blake2`, `blake3` (`ls crypto/internal`), the password hashes among them
being code whose timing an attacker may also read.

### 10. `swiftc` without Xcode: the Command Line Tools here carry it and CryptoKit's module, through `xcrun` or a named SDK (settled for this Mac; one premise the layer must keep)

`/Library/Developer/CommandLineTools/usr/bin/swiftc` exists; with `DEVELOPER_DIR` naming the CLT,
`swiftc --version` prints *"Apple Swift version 6.4.2 (swiftlang-6.4.2.1.9 clang-2100.4.1.1)"* and the
CLT's `MacOSX.sdk` holds `CryptoKit.framework` (`ls`). A copy of the writer's `swp.swift` (sha256
`fef9066740861a2c`, the brief's) built under the CLT alone, `HOME` and `TMPDIR` in my folder: **called
bare, `swiftc` fails** *"unable to load standard library for target 'arm64-apple-macosx27.2.0'"*;
**through `xcrun swiftc -sdk "$(xcrun --show-sdk-path)"`** it writes `libswp.a`, 6,472 bytes, `T
_swp_ed25519_public`, `real 21.61` with a cold module cache (`work/clt/`). So a Mac with the CLT has
`swiftc`; a member's build must reach it through `xcrun` or name the SDK; and this Mac also has Xcode, so a
Mac with the CLT **alone** is an inference from `DEVELOPER_DIR`'s selection, not a second machine.

### 11. Decision 10's register was enumerated by words; the milestone the decision empties holds 19 decisions and 39 open issues (unrun as a list; changes what the spec-warden is asked to call complete)

The brief's list (`00-shared.md:151-175`) came from `grep` for *standard library* and `gh` searches for
*standard library*, *invalidated*, *register of the decisions*, *Zen law 18*, and says so. Decision 8,
*"`core` no longer exists: every trace removed"*, reaches the milestone `M-core-packages`, which `gh api
.../milestones` reads **open, 39 open issues and 192 closed**; `gh issue list --label kind:decision
--milestone M-core-packages --state all` prints **19 decisions**, of which the list names 2404 and, by
their sittings, 2304, 2329, 2415 and 2423 (panels 234, 235, 241, 242); **not named**: 2564 (panel 250, which
Q6 cites but the register list does not), **2528** (*"Until the core packages reach
1.0.0, every program moves to the newest version of core"*), **2560** (the package manager's second
version meeting R2 by placing modules), **2561** (the hooks' lane over core), **2403** (a face's recursive
type walked by loops), 2562. Its open items: 25 defects, 10 features, 4 tasks; of the 14 features and
tasks the list names 1067, 2082, 2313 (and 2315, which `gh` labels `kind:feature` of M-package-index, not a
task). And **17 programs of `examples/`** hold `[dependencies.core] version = "0.6.0"` from `index =
"file://../../packages"` (`grep -l core examples/*/heroes.toml`: 17 of 17 catalogues; `examples/query`
read), so *"every trace removed"* moves 17 example programs, which the `compat` suite reads at each
release tag. **Repair**: the list is enumerated from the milestone (`gh issue list --milestone
M-core-packages`) and from `examples/*/heroes.toml`, and the spec-warden is handed that set, not the
word search.

### 12. Rebel's § A7 claims a net check that no file performs; the 662 holds 20 misreads (wrong premise; a number to correct)

`examples-rebel/README.md:375-378`: *"The check of the net holds every rule above that a machine can: a
comment above every public declaration, field and case; every parameter named by its own name, in order,
once; ... an example for every function."* No file outside `packages/` and `docs/` names
`packages/rebel` but `examples-rebel/heroes.toml` and `examples-rebel/README.md` (`grep -rl
packages/rebel --exclude-dir=packages --exclude-dir=docs`), so no suite reads Rebel's faces and the
conservative's *"judged by a harness check"* (`00-shared.md` § Question 2) is a check to write, as is
the robust verb's refusal. **The census**: the writer's `doc_census.py` counts a `>` as a closing bracket,
so the `->` inside a function-typed parameter shuts the depth early and the parameters after it are lost;
re-run with that one repair (`work/q2/census_fixed.py`, `->` read as two other characters): **642
mismatches, not 662** (4,568 right), of which **620** are functions with a parameter whose comment holds
no parameter line at all (`work/q2/`). The compiler-engineer's Q2.2 asks where the two readers differ:
here is one, measured.

### 13. Every clock the briefs ask for falls while five agents load the Mac (rule; CLAUDE.md § Verification, *while a clock runs the machine stays still*)

Asked: Q2.2 *"time the largest face (`/usr/bin/time -l`, instructions, best of N stated...)"*, Q5.2 (P) *"its
cost ... in instructions ... against a noise floor"*, Q6.3 *"the time of `heroes fetch` cold, best of N
stated"*. Load read 10.79 to 14 through this reading (`uptime`), and the brief itself records 11 to 18
with five agents; a wall clock taken now is discarded by the ratio rule. `/usr/bin/time -l` on this
macOS prints **`instructions retired`** (run on `/usr/bin/true`: 9,503,315), a count load moves far less.
**Repair**: every cost asked in instructions retired and bytes; seconds, where wanted, named **owed** to a
still machine; Q6.3's *"time of `heroes fetch` cold"* becomes its files, bytes and instructions.

### 14. Q6's robust spelling says nothing of a `path` entry, and Rebel's own 161 example programs share one (a question nobody posed; changes what Q6's seats build)

`examples-rebel/heroes.toml` is one `path` entry (`path = "../packages/rebel/0.1.0/files"`) for **161
programs** (`ls examples-rebel/*.hero | wc -l`) whose `use <rebel>/` lines name **163 distinct paths**
(`grep -h -o | sort -u | wc -l`). `spec-umb-r.diff` says an entry *"places only those its `packages`
names"*, and a `path` entry places nothing (spec line 34: *"by a local `path` alone, which nothing
verifies"*). Either `packages` binds a `path` entry too (then this one catalogue lists every face, and
the refusal checks nothing for these programs), or `path` entries are exempt (then the refusal is a
property of a fetched entry alone, and t6's `now` reference, built over a `path` entry, is the only build
the brief ran). **Asked of the compiler-engineer and the spec-warden**: which, in the arm's words; and
the five measured programs (`00-shared.md` § Question 6 table) are invented package sets, where the 17
`examples/` programs of finding 11 and the 161 of `examples-rebel/` are real ones the placement could be
measured on.

### 15. The verdict session is capped where panel 251's cost 94% of the cap (rule; costs nothing to repair)

Panel 251's verdict session cost **0.3771674 USD** under the same `CAP=0.40` (`251-llm-ergonomist/run.sh`,
`verdict/run.json`), 11,624 output tokens over 65,407 bytes of input; 253's verdict reads 65,721 bytes
(`wc -c verdict/*`). The CLI's behaviour when a session passes its cap is unread by me (no session can
be run). The verdict is last and grades nothing, so its loss costs no cell; a cap of 0.60 for it alone
keeps the stop rule's arithmetic (finding 16) and its record.

### 16. Does 10 USD hold every graded cell? Yes on panel 251's maxima, by arithmetic (checked as asked; an inference)

`python3 -I` over panel 251's 50 graded `run.json`: **min 0.1322, max 0.2112, median 0.1619, mean
0.1663, sum 8.313339**; by task, max 0.2003, 0.2112, 0.1820, 0.1508. **30 at the maximum and the verdict:
6.34 + 0.38 = 6.71 USD**; 30 at the median and the verdict **5.23** (the brief's 5.24 reproduced). The stop
rule (`run.sh:75`) starts a session only while spent + 0.50 <= 9.50, so a graded cell is lost only if the
graded sessions average above **0.30 USD**, 1.42 times panel 251's most expensive. 253's sessions read
the same 29-30 KB spec and a task of 779 to 1,160 bytes (`wc -c`), against 251's 666 to 2,012 (its
`t4` and `t3`); t6 asks four answers. **An inference**: nothing here has run a 253 session. One fact beside it: every one of
251's 51 sessions logged `report.md=no` (`run.log`), and `t3-c-2`'s own answer says *"My only tool can
read files, not write them"*; 253's flags are the same, so its grading reads `result.md`, as the brief
already allows.

### 17. Smaller facts and rows a reader cannot apply the same way

- **t6's rows** say *"the commands are `heroes add rebel`"*: whether an added `heroes fetch` (a real verb
  the frozen spec never names: `grep` finds only `heroes add`, line 35) or `heroes new` is right or wrong
  is not registered. **t2's** *"its first line a sentence"* is a judgment; *"a first sentence that ends
  with a period on its first line"* is a rule. **t4's** *"keeps the two given functions unchanged"*:
  byte-equal lines, or the same meaning?
- `00-shared.md:497-498`: *"The package grain and the module grain differ by under 1% in every row"* holds in
  lines; in **bytes** the two grains differ by 9.5 to 15.0% in the five rows (`bw/q6/closures.txt`,
  `python3 -I`): the ledgers and the files no module reaches.
- `00-shared.md:162-163`: 2315 is a `kind:feature` (`gh issue view`), not a task.
- `compiler-engineer.md:91` cites `hero_panic_where` at `panic.c:69`: that line is its declaration; its body is in `runtime/parts/place.c` (`panic.c:67-69`, read).
- `00-shared.md:14-19` and three seat briefs (`compiler-engineer.md:17-19`, `ffi-pragmatist.md:15-17`,
  `historian.md:20-22`) still say *"if the coordinator splits"*: the coordinator's decision to keep six
  belongs in `00-shared.md` § Six questions, with the seats' conditional sentences struck, or a seat
  plans its clock against a split that will not happen.
- The count_tokens premise (*"free"*, carried thrice): the `claude-api` reference bundled with Claude Code
  2.1.326 calls a pass on the token-counting endpoint *"free"* (`shared/preserved-thinking-migration.md:157`,
  read today); its `token-counting.md` states no price. Carried, consistent.
- `ffi-pragmatist.md:30-31` asks *"whether the platform layer ... is where every system call Rebel makes
  stands, as `selfhost/cli/layer.hero:1-23` says"*: the file says the capabilities *"whose C differs by
  platform"* are its members and that *"`hero_os.h`'s process, files, directories, spawning and threads,
  stays there"*, in the runtime (`:5-6`, `:21-23`, read); and Rebel's `extern` groups name `sqlite3.h`,
  `math.h`, `string.h` and `stdlib.h` beside the layer's headers (`grep -rh '^extern '`, 23 files, the
  brief's own count). Ask it as a question, not as the file's claim. design.md:499 already names *"Swift
  `@_cdecl`"* among the C ABI's producers Heroes targets, the record route S rests on (the brief cites
  only the milestone page's mention of it).
- `historian.md:16-20`: Deno stands in 12 historian reports (153, 158, 179, 185, 194, 209, 217, 230, 234,
  235, 238, 250), where the brief names four; `go doc` in 161, 167, 214, 215, 228, 244, 251 and 234 by `godoc` or `pkg.go.dev`, 11 lines, so the brief holds there; Phobos at 212 and
  227. The negatives hold: `dudect`, `constant-time`, `CryptoKit`, `Boost`, `t.Skip`, `PEP 594`,
  `pclntab`, `exit 77` match no historian report (`grep -l -i -F`), and no sitting file names
  `constant-time`, `CryptoKit` or `@_cdecl` (`grep -l`, 0).

## The blind seat, item by item (what the coordinator's prompt asks)

| check | result | command |
|---|---|---|
| the three repairs: `< /dev/null` | on the `claude` line, `run.sh:88`; `check_area.py:35` reads it | read |
| the empty repository, `.gitignore` `*`, local `user.name` | the area's `.git`, no commit, no ref, `reader <reader@invalid>`, status clean | `check_area.py` exit 0 on a copy (`work/area/`), `git log`, `git config --local --list` |
| `HOME` inside the area | `HOME="$HERE/home"`, `run.sh:82`; `home/` empty | read, `ls -la` |
| a fourth way out, unrepaired | the CLI's temporary directory, `/private/tmp/claude-501/` | finding 5 |
| no spend before the predictions | the heading check precedes every spend (`:43` before `:52-60` and the loop) | read; finding 6 for what it does not check |
| the free preflight is free | `check_folders.py`, `check_area.py`, `spent.py` exit 0 on a copy, `0.000000`; the copy's `.git` unchanged (`stat` of every file before and after, `diff` empty) | `work/area/` |
| each task's input tells its arms apart | t2, t4, t6 each do; t6's umb-r cell is its arm's example (finding 1); t4's hazard row is forbidden by the task (finding 2); t2's `needed` is decided by the task (finding 3) | read |
| rows and rules applicable the same way | three rows need words (finding 17); `scan.py`'s voiding misfires 2 of 51 (finding 4) | run |
| 10 USD holds every graded cell | yes on 251's maxima, 6.71 USD; the verdict's cap is tight (findings 15, 16) | `python3 -I` |
| folders and arms as registered | `check_folders.py` exit 0; every arm `cmp`-equal to `bw/arms/`; `spec-A.md` `cmp`-equal to the frozen spec, `spec-B.md` to `spec-all-r.md`; `brief-session.md` and `brief-verdict.md` `cmp`-equal to 251's; `key.txt`, `order.txt`, `run.sh` at the hashes the brief gives; the project-name `grep` over the briefs and tasks exit 1 | `cmp`, `shasum` |
| the order | five rounds, odd forward, even reversed, the verdict last (`order.txt`, 31 lines, 31 distinct) | read |

## The split, and what six questions in one sitting owe the briefs

**The compiler-engineer's load, counted from its brief**: four questions that each change `selfhost/`
(Q2 a verb, its refusals and a golden form; Q4 a built-in, the runner's channel and a forge test; Q5 a
walk in the runtime and possibly the emitter, on this Mac and in Linux arm64, at two levels; Q6 a
catalogue field, `add`, `fetch`, Rebel published into its copy's index and a refusal with its fix),
twenty numbered asks and Q1's two; each change a stage 2 of 130 to 240 s here (mine 129.99, panel 251's
critic 237.88) before its cases run. Panel 251's seat took about three hours on three questions
(carried, `00-shared.md:537-539`). **What the briefs must change for six in one sitting**:

1. `00-shared.md` § Six questions states the coordinator's decision and its reason, and the three seat
   briefs' *"if the coordinator splits"* sentences are struck (finding 17's last item);
2. the compiler-engineer's brief gives an **order and a stop**: the spec-facing halves first, since the
   spec-warden's payment and the blind seat's rows wait on them (Q4's name and channel, Q2's refusals,
   Q6's refusal and the `path` question of finding 14), each question's block written whole before the
   next begins, so a seat stopped at its clock leaves whole blocks; Q5 last, its spec cost +13;
3. every clock becomes instructions and bytes (finding 13).

**What I would scope down first**, in this order: (a) Q5's W built on this Mac alone, its Linux arm64
rows owed to the ffi-pragmatist's Q5.1, which already builds `deep.hero` in both images, and (P) priced
by `--emit-c | wc -c` and one program's instructions, not the compiler's self-build; (b) Q6's index
publication and `fetch` costs after the refusal is built over a `path` entry (M' and E stay priced, as
the brief already says); (c) Q2.5, the `heroes check` widening, counted only if the verb is built. Q4 is
the smallest and stays whole. The ffi-pragmatist's Q3 is the sitting's other long pole (a Swift member
in the layer, a primitive in Heroes, a constant-time tool, route C, route V): its first measurement is
finding 9's existing function, which may decide route H before any primitive is written.

## The brief writer's questions it could not run, settled where a command settles them

The writer's final list of eleven is not on disk (`grep -rl` over `rebel-scratch/coord/` and
`253-briefwriter/` for its words found only this prompt); I took `00-shared.md`'s thirteen bullets
(§ Questions no one has run) and its two in-text *unrun* sentences.

| question | settled? |
|---|---|
| `swiftc` with the Command Line Tools and no Xcode | **yes on this Mac**, through `xcrun` or `-sdk` (finding 10) |
| Darwin arm64's frame records at `-O2`, *"from memory"* | **yes**: `-mframe-pointer=non-leaf` by Apple clang 21's driver (finding 8) |
| a frame walk at `-O2` on Linux x86-64 | **no records by this clang's default** for `x86_64-linux-gnu` (finding 8); the image's clang and an inlined frame: unrun |
| a `use <rebel>/x` of an unlisted package through a `path` entry today | **accepted**: the writer's own `ref/t6/with-flags-used.txt` (`wrote table2`) and spec lines 33-34; what `packages` means for a `path` entry is finding 14 |
| an exit code no program's exit can forge | **none of 0 to 255**: spec line 362 gives `exit(code:)` every one, an abort is 134; only a channel the runner owns can carry a skip (read; a build owed) |
| the 662 by the compiler's parse | **20 are the reader's** (finding 12); the rest is the compiler-engineer's |
| `heroes doc` twice, two platforms | no verb exists (`heroes-next doc` exits 2, the brief's): owed to the build |
| the Swift member under `--sanitize`, both levels, `macos-26` | unrun: a build in the layer, the ffi-pragmatist's |
| a constant-time checker on the binary | unrun; the subject exists (finding 9) |
| CNG's X25519 and its byte order | owed to the box |
| (P)'s cost in instructions; `fetch` under `packages` | unrun: the seats' builds |
| what Go, Rust, Python, Zig, Swift, Deno, Boost do | the historian's |
| every blind transcript read clean by `scan.py` | after the run; the scan misreads its own sessions' answers (finding 4) |

## What each brief hands its seat of `SKILL.md` § 3 and 3c

| seat | own folder, built from the seed | report as it goes | no credential | no paid run | adopted route built |
|---|---|---|---|---|---|
| compiler-engineer | `:9-10`, `00-shared.md:556-570` | `:11-13` | `00-shared.md:590-592` | `00-shared.md:590` | `:13-16` |
| ffi-pragmatist | `:10-11` | `:12-13` | `00-shared.md:590-592` | `00-shared.md:590` | `00-shared.md:601-602` |
| spec-warden | `:10-11` | `:12-13` | `:15` | `:15`, its refreshes named and counted at 0 | not asked (reads) |
| historian | builds nothing | `:7-8` | no shell | `:9-10` | n/a |
| llm-ergonomist | the coordinator runs it (findings 4-6, 15) | the coordinator copies (`llm-ergonomist.md:203-206`) | the key's length alone (`run.sh:59`) | 31 sessions, 0.40 each, stop at 9.50 | n/a |

`00-shared.md:572-608` carries the common rules (a program that may not end, Docker named and stopped,
`HOME` and `TMPDIR` inside the folder for `swiftc`, `go`, `lldb`, the machine held awake). `pmset -g
assertions` at 15:08 still reads `PreventUserIdleSystemSleep 1`, Amphetamine pid 750 (*"Single-Use -
System"*), as `00-shared.md:606-608` says. Missing: finding 13's clock rule and finding 5's temporary
directory for the blind sessions.

## Not run by me, and why

No blind session and no refresh (paid, or an API call); no Linux image (no finding needed one); the
compiler-engineer's and ffi-pragmatist's builds (theirs); the historian's pages; Windows. Re-run and the
same as the brief: the doc census (149, 7,718, 5,210, 4,548, 4,305, 4,281), the skip census (14,003, 54
in 17 files of 13 packages, the per-package counts, 51 silent), the macOS gap (15 rows, `diff` against
`bw/q3/macgap.txt` empty), Rebel's layout (0 C files, 1,639, 1,495, 144, 16 members, 23 `extern` files,
859,150 lines, 38,309,694 bytes), the two Q5 programs I rebuilt (`deep.hero` exit 134 with
`at deep.hero:3:12, in deep.pick`; Rebel's abort at `<rebel>/strings.hero:921:12`, its caller unnamed).

## Tally

Seventeen numbered findings and a section each on the blind seat, the split and the writer's questions.
**wrong** 6 (1, 2, 4, 7, 12, and the small facts of 17); **unrun** 2 (3, 11); **route** 3 (7's address
route, 9, 14); **rule** 5 (5, 6, 13, 15, and 17's split sentences); **settled by command** 3 (8, 10, 16).

## Summary: what a brief must be repaired for before any seat starts, then the rest

The spec-warden registers predictions in the blind seat's counts and the seats price against the
briefs, so a task, an instrument or a framing the seats lean on is repaired before the first launch:

1. **t6 is the umb-r arm's example** (finding 1): `csv`, `json`, `0.1.0`, `packages = ["csv", "json"]`
   and `use <rebel>/flags` refused all stand in `spec-umb-r.diff`; change the task's packages or the arm's,
   re-draw and re-register.
2. **t4 forbids the hazard its `now` row counts** (finding 2): *"never reads as a test that passed"*;
   word it neutrally or register that the row measures obedience.
3. **`scan.py` voids sessions for their own answers** (finding 4): 2 of panel 251's 51 transcripts flag
   `CLAUDE.md` from an `assistant` entry; scan non-assistant entries only.
4. **The sessions write to `/private/tmp/claude-501/`** (finding 5): 251's `t4-f-2` left
   `cache-break-state-<its id>.json` there; set `TMPDIR` and `CLAUDE_CODE_TMPDIR` inside the area.
5. **The predictions' gate** (finding 6): copy the spec-warden's report at start, hash the copy, refuse an
   empty heading.
6. **The register's list** (finding 11): enumerate from `M-core-packages` (19 decisions, 39 open) and the
   17 `examples/` catalogues naming `core`, not from a word search.
7. **Clocks under five agents** (finding 13): instructions retired and bytes, seconds owed.
8. **`packages` and a `path` entry** (finding 14): 161 `examples-rebel/` programs share one; ask which the
   robust spelling means.
9. **Q3's route H has a subject in the tree** (finding 9): `crypto.equals_constant_time` and
   `crypto.hero:15-21`'s refusal of ciphers in Heroes; the spec's `i64`-only bitwise operators and
   aborting overflow.
10. **Q5's line tables** (finding 7): every build carries them; Darwin's binary holds only a debug map to
    `build/*.o`, no dSYM.
11. **Six questions in one sitting** (§ The split): state the decision, strike the conditional
    sentences, give the compiler-engineer an order and a stop.

**The rest**: finding 3 (t2's `needed` decided by its words), 8 and 10 (settled facts to hand on: frame
records by target; `swiftc` in the Command Line Tools through `xcrun`), 12 (642 not 662; § A7's net check
does not exist), 15 (the verdict's cap), 16 (10 USD holds, 6.71 at 251's maxima), 17 (smaller facts and
three rows to word).

Finished 2026-11-19 at 15:13 CET (`date`), begun at 14:55; no seat had started. Nothing of mine runs
(`pgrep -fl 253-critic` read below); every file I wrote is under
`.claude/worktrees/rebel-scratch/253-critic/` but this report.

**Added 2026-11-21.** The folders under `.claude/worktrees/` this file names were never part of
the repository, git ignoring that directory, and were removed that day by the author's
instruction, meant as: *delete every worktree that is not active; where one is cited, find a
solution*. What a seat or the coordinator kept under `rebel-scratch/` for this sitting is read
from `docs/panel/253-reports/evidence.tar.xz`, its paths relative to `rebel-scratch/`; `evidence.md`
beside it says what was kept and what was left out. `coord/blindtest/`, the coordinator's preflight of a blind session, is in no archive and
is gone.
