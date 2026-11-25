# Panel 260, the blind reading: the registration. REGISTERED, NOT RUN.

Written 2026-11-25 by the sitting's clerk, **before any session is drawn and before any session runs; none is drawn and
none runs**; repaired the same day after the critic's first reading (`docs/panel/260-reports/critic-briefs.md`, its
item 3), still before any session. **Authorised by the author** (the answer *ok a* to the coordinator's question of
about 10:31, relayed at 10:37, meant as: *run it, with a ceiling of 20 USD*): **the coordinator starts it after the
synthesis**, on that word, with this file's inputs, by hash, and what the synthesis adds below. The tasks, the grades,
the key, the decision rules (D1, D1b, D1c, D2, D3, D4), the size and the caps are
`docs/panel/260-briefs/llm-ergonomist.md` as it stands at the commit that carries this file. The spec-warden's
predictions are its own (`docs/panel/260-reports/spec-warden.md`, `## Predictions registered before the blind run`),
copied and hashed by the runner as it starts, and read by the rule of § How a prediction is read, below.

## What the repair changed, and why (the critic's item 3)

- **W1 needs no state any more.** As first written it asked for *"the nanoseconds since the first call to it in this
  process (0 at that first call)"*, a value that outlives a call, and spec line 154 says *"There are no mutable
  globals"* (`sed -n 154p spec/heroes-spec.md` at the base): a reader could only write a C header of their own holding
  a `static`, which is the own-header shape of `00-shared.md` § 6.2, or answer something else. W1 now asks for
  `mark()` and `elapsed_ns(since:)`, both plain calls over `hero_time_monotonic_ns`. **Run**: a reference of all four
  jobs as a reader under `now` would write them (no declaration exists there), kept in `blind/reference-t1/`, builds
  at exit 0 under the base's compiler on this Mac and prints what each job asks (§ The reference, below). Whether a
  reader could still choose state for W1 (a header of their own) is theirs; the task no longer asks for it.
- **W4 is added, over a member's header the spec does not name.** The spec names three of the runtime's and the
  layer's headers, `hero_os.h`, `hero_net.h` and `hero_time.h`, at line 401, and `hero_net.h` and `hero_os.h` again
  at line 428, which names W3's own function, `hero_net_stop_asked() -> i64` (`grep -noE '[a-z_]*hero_[a-z_]*\.h'
  spec/heroes-spec.md` at the base: lines 401 and 428 and no other). So W1 and W3 alone could not tell a text that
  lists the layer's headers from one that gives a reader a rule for the 39 member headers it names nowhere
  (`hero_audio.h`, `hero_crypto.h`, ...; 41 members have a header at `runtime/<member>.h`, `00-shared.md` § 3.1).
  W4 binds `hero_activation_count`, declared at `runtime/hero_activation.h:155` (`grep -n`), a member's header
  (`runtime/platform/hero_activation.c` is a member, `ls runtime/platform/*.c`), described in the task exactly as
  the other three are. It is what measures the spec-warden's question 3 (`spec-warden.md`).
- **The grader's table is split by file** (§ The grader's reading of each arm), so *"in the home the arm names and
  nowhere else"* reads one way for two graders.
- **The session brief's names are registered** (§ Every session's folder): `spec.md`, and where each job's
  `prediction` and `confidence` go.

## The inputs, by hash (`shasum -a 256`, 2026-11-25, at the base `c3885b81b`)

| file | sha256 |
|---|---|
| `arms/spec-now.md`: `spec/heroes-spec.md` at the base (unchanged since `80e2e39ad`: `git diff --stat 80e2e39ad c3885b81b -- spec` prints nothing) | `c915f0f2d47f35c6b3162d198c27bedfbb3fe2b962e05185a878619356372bee` |
| `arms/spec-adopted.md`: `now` with `docs/panel/260-reports/synthesis-adopted.diff` applied by `patch -p1` (the text `synthesis-adopted.txt`, sha256 `142445a4b8b990b37ad9b36304702e8dbd66f40f4ecf49ed6da5fe52a4d21d24`, 749 bytes; the diff `78b15a71fb699447785a982246efcefae5a871b1b37be432527bd2105de4fc17`), written by the synthesis 2026-11-26 | `70b8e497cea7f490e7884d4b7f5d66ba95866d43b0bf16b44e8acbb7d513f1a7` (34,855 bytes) |
| `arms/spec-rival.md`: `now` with `docs/panel/260-reports/synthesis-rival.diff` applied, `cmp`-equal to the spec-warden's `spec-w1-group-mark.diff` (the text `synthesis-rival.txt`, sha256 `f4305a917f0d5443c3b75db8ed020f9bb8379810d2bc8e4c7346876681e744a8`, 635 bytes; the diff `af4718ccae74d423f9a733ecf767bbed21a9c20de237e47cd1bc7844ed6f83c8`, as the warden registered it) | `abe96488a5dcfd8eeb9d6c6bb986c583e03b47b3cf72eaa6c4f62dda323facc4` (34,740 bytes) |
| `tasks/task-t1.md` (`blind/task-t1.md`, as repaired; 2,849 bytes, `wc -c`) | `3d71ca51116a47bee0f63d3304710870ef7c4d5a4935333ed7963f1b247b41a2` |
| `tasks/task-t2.md`: **written by the synthesis's clerk once the spelling is adopted** | *(before any session)* |
| `programs/` of t2, six files or folders: **the same** | *(each, before any session)* |

The first registration named `task-t1.md` at `3076b3d77b3be818cd590cad6c4582658c079af6f81db257f279284f8e7d080c`,
2,401 bytes (three jobs, W1 with state); that file is superseded by the one above and no session ever read it.

## Every session's folder

**`brief.md`, `task.md`, `spec.md`** and, for t2, **`programs/`**, nothing else (panel 257's names): `brief.md` the
runner's `brief-session.md`, `task.md` one of the two tasks, `spec.md` one arm, never named. A t1 session holds no
`programs/`. The key's files, this registration and `blind/reference-t1/` are never in a session's folder.

**The runner's `brief-session.md` is panel 257's, adapted, and its diff is registered before any session.** Panel
256's committed `brief-session.md` (`docs/panel/256-briefs/blind/brief-session.md`, sha256
`724e8e0d656bb7b0daa51c4422337b2b993f44b6d8499d980fa7801a7652690b`, 281 words by `wc -w`) names the spec
`heroes-spec.md` and asks four fields, `answer`, `choice_points`, `prediction` and `context`. Panel 257's adaptation
(its `tools/registration-runner.md` § diff of brief-session.md, read by the repair from
`docs/panel/257-reports/llm-ergonomist-blind-answers.tar.xz` with `tar -xJOf`) names it `spec.md`, adds a line for
`programs/` (*"the C files the task names"*) and drops `prediction` (*"the tasks ask the verdict themselves"*). This
sitting's `brief-session.md` is 257's with three changes, and `registration-runner.md`, written beside this file
before any session, shows each as a `diff -u` against panel 256's committed bytes:

1. the input line `spec.md`: the language's specification, the whole language (257's, kept);
2. the `programs/` line reads *"`programs/`: where the task names a folder of that name, the files it holds; a task
   that names none has none"*, since t2's `programs/` holds Heroes packages and programs, not C files, and t1 has none;
3. the `answer` field reads *"what the task asks, each job or question in its order; every file whole, under its
   path; and each job's `prediction` and `confidence` where the task asks them"*, so t1's per-job `confidence`
   (1 to 5) and `prediction` stand under `answer`, read the same way in every session; `choice_points` and
   `context` are 257's.

A runner whose `brief-session.md` differs from these three changes by any other line refuses to start.

## The grader's reading of each arm, by file, before any session

Written by the synthesis, **before any session**, from each arm's text alone, never from a build: what counts as the
declaration in that arm (its words) and, for each file a job can hold, whether that file owes it. Every path is relative
to the job's folder. A reader is **declared-right** on a job when every file the table names for that job holds the
declaration where its cell says *declares* and holds none where it says *nothing*, **and no other file the reader
wrote for that job holds one**; anything else is declared-wrong. A job whose files are not where the task puts them is
read at the paths the task gives, a file elsewhere counting as *another file*. Under `now` there is no declaration,
every cell is *nothing*, and every W1, W3 and W4 is declared-wrong by construction.

| arm | the declaration's words | job | file | owes |
|---|---|---|---|---|
| `now` | none | W1 | `ticks/clock.hero` (the package's module), `main.hero` (the program), `heroes.toml` (the program's catalogue), `ticks/heroes.toml` (the package's own catalogue) | nothing, each |
| `now` | none | W2 | `ident/me.hero`, `main.hero`, `heroes.toml`, `ident/heroes.toml` | nothing, each |
| `now` | none | W3 | `main.hero`, `heroes.toml` | nothing, each |
| `now` | none | W4 | `main.hero`, `heroes.toml` | nothing, each |
| `adopted` | the word `layer` after a group's header string, before `link` or `pkgconfig` where the group has one: `extern "hero_time.h" layer` | W1 | `ticks/clock.hero` | **declares**: `layer` on every group whose header is `hero_time.h` or includes a file of the layer (the job's group over `hero_time.h`), on no other group |
| `adopted` | | W1 | `main.hero` | **nothing**: the program binds through the package; a group of its own over a layer header, if the reader writes one, declares, and `layer` on any other group is declared-wrong |
| `adopted` | | W1 | `heroes.toml`, `ticks/heroes.toml` | **nothing**, each; the text adds no home |
| `adopted` | | W2 | `ident/me.hero`, `main.hero`, `heroes.toml`, `ident/heroes.toml` | **nothing**, each (`hero_os.h` is the language's; *"no other group may"*) |
| `adopted` | | W3 | `main.hero` | **declares**: `layer` on its group over `hero_net.h` |
| `adopted` | | W3 | `heroes.toml` | **nothing** |
| `adopted` | | W4 | `main.hero` | **declares**: `layer` on its group over `hero_activation.h` (*"every other `hero_` header it provides"*) |
| `adopted` | | W4 | `heroes.toml` | **nothing** |
| `rival` | the same words: the same home and the same production | W1 to W4 | every file as `adopted`'s rows | every cell as `adopted`'s, read from `w1`'s text: it names the same headers the layer's and the same word after the header |

## The key, on both graded platforms

The landed build's run, three runs of three, **on this Mac (Darwin arm64) and in the Linux arm64 image**, of: each
reader's W1 to W4 as written (the **built** grade), and t2's six (the key of D3: *builds*, or *refused* and the file the
message names). A t2 program whose two platforms disagree has no key, is named in the scoring and is out of D3.

## The reference (run by the repair; not an input of any session)

`blind/reference-t1/w1` to `w4`: the four jobs as a reader under `now` would write them, no declaration in any (none
exists there), each program's catalogue a `path` entry for its package. Built with `heroes build main.hero -o bin` in
each job's folder and run, under the base's compiler (`c3885b81b`, built as `00-shared.md` § 10 says, `sources
41f32a8f4b745c0c`) on this Mac, 11:50 (`ground/repair-after.sh`): all four build at exit 0 and print `true`, `true`,
`stop asked: false` and `handed: 0`, each run at exit 0 (`blind/reference-t1/results.txt`); every `.hero` of it is
canonical by `heroes fmt`. It shows only that each job can be written in Heroes alone at the base; it is no key, it
was not run in the Linux image, and it never enters a session's folder.

## How a prediction is read

The spec-warden registers its predictions before any text is adopted, so it can predict only for its own texts
(`docs/panel/260-reports/spec-w*.txt`, each with its diff and its home). At the run: the prediction read for an arm
is the one the spec-warden made for **its own text whose diff is byte for byte the arm's** (`cmp` of the diffs);
where none is, the prediction for its text **of the same home** is read, and the scoring names every line the arm's
diff has and that text's does not; where it wrote no text of that home, that arm is **unpredicted**, counted neither
right nor wrong. A prediction is a number and a range per rule (D1, D1c, D2, D3, D4) and per arm; it is right where
the measured count lies in the range.

## The cap (`llm-ergonomist.md` § The size, the spend and the cap)

**31 sessions**; 0.60 USD a graded session, 1.20 the verdict, margin 0.10, **a ceiling of 20.00 USD**, the author's;
the verdict runs only if the 30 spent at most 18.70 USD. The price it rests on is panel 257's measured run, 31
sessions for 6.297764 USD; this run's estimate, about 6 to 8 USD, is **an inference, unrun** (t1 now asks four small
jobs where the first registration asked three).

## What the synthesis adds before any session

The two arms' texts and hashes in the first table, and which home of `00-shared.md` § 7.1 each is; t2's task and six
programs, label-stripped and hashed; the grader's reading above, every cell; and, where the adopted resolution has
been built by then, its run of t2's six on both platforms as the key. **Where it has not been built, or where a t2
program's two platforms disagree, that program is out of D3, and where the landed build does not exist the run
waits**: a reading graded against a key nobody ran is not a measurement.

## What the synthesis added, 2026-11-26, before any session

**The two arms.** `adopted` is the resolution's R6 sentence
(`docs/panel/260-a-group-that-binds-the-platform-layer-says-layer-a-program-none-of-whose-groups-says-it-links-nothing-of-the-layer-and-there-is-no-fallback.md`):
the spec-warden's `w1` rewritten to say what the resolution refuses, merged into § 13's line 401 in place of *"linked
where a group names one"*, and `w1`'s production at line 508, `Extern = "extern" string [ "layer" ] [ ( "link" |
"pkgconfig" ) string ] NEWLINE`:

> Anything beyond this document comes from packages or C libraries; processes and files from the language's
> `hero_os.h`, and sockets, clocks and the rest from its platform layer: `hero_net.h`, `hero_time.h` and every other
> `hero_` header it provides but the compiler's `hero_compiler.h`. A group whose header is one of these or includes a
> file of the layer at any depth, or whose library calls the layer, must say `layer` after its header, and no other
> group may; a name one of these declares is bound only by a group whose header is or includes it, and a build links
> the layer only where a module it compiles holds a group saying `layer`.

`rival`, the second-ranked text, is **the spec-warden's `w1` byte for byte** (`synthesis-rival.diff` `cmp`-equal to
`spec-w1-group-mark.diff`): the text the warden approved, the same home and the same word, false against the check on
three clauses (the critic's `critic-reports.md` § 5). It is ranked second over another home because only a rival of the
same spelling can be keyed: t2's six programs are written in the adopted spelling and the key is the landed build's,
which reads no other home's word, so a rival in `w2`'s module line (the warden's second home inside the source) could
be graded on D1, D1c and D2 alone, its D1b and D3 unkeyable. That rival is recorded and not run. **Counts**
(`heroes measure` of each applied copy, the base's compiler, `sources 41f32a8f4b745c0c`, from a tree of the base's
root, 2026-11-26 at 00:12, no `--refresh`): `adopted` claude-legacy 9,978, cl100k_base **10,155**; `rival` 9,953,
**10,130**; the base 9,877 and 10,054: **+101 and +76, a lower bound, vendored only**. Both apply to `now` and give the
arms of the first table (`patch -p1`, `cmp`); line 428 is byte-equal in both.

**The grader's reading** is in § The grader's reading of each arm, every cell, from each arm's text alone. The two arms
give one reading, since both name `hero_net.h`, `hero_time.h` and *"every other `hero_` header it provides"* the
layer's and `hero_os.h` the language's, and both put the word after the header.

**How the spec-warden's predictions are read** (§ How a prediction is read): `rival`'s diff is `spec-w1-group-mark.diff`
byte for byte, so that text's predictions are read for it as registered. `adopted` is of the same home and has no text
of the warden's whose diff is its own, so `w1`'s predictions are read for it, the scoring naming the lines its diff
has and `w1`'s does not: in line 401, *"includes a file of the layer at any depth, or whose library calls the layer"*
for *"includes one at any depth, or that binds a name one declares"*, the clause *"a name one of these declares is
bound only by a group whose header is or includes it"*, and *"holds a group saying `layer`"* for *"holds such a
group"*. **D3, said before the run** (the critic's § 5): the warden predicts the number of t2's six on which at least 4
of 5 readers are build-right (`w1`: 5 [4-6]), while D3 is a gate on each of the six, so a reading of 5 is a right
prediction and a failed D3 at once; the two are read apart.

**The ceiling** stands at **20.00 USD for the run**, the author's (*ok a*, relayed at 10:37 on 2026-11-25,
`llm-ergonomist.md`), with § The cap's per-session caps; the spend of about 6 to 8 USD is an inference, unrun.

**What is still owed before any session, and the run waits for it**: t2's task and six programs in the adopted
spelling, label-stripped and hashed into the first table (the clerk's, unwritten); and **the key**, the landed build's
run of t1's jobs and t2's six on this Mac and in the Linux arm64 image. **No landed build exists**: the
compiler-engineer's patch (`programs/compiler-engineer/route.patch`, `sources 2ac05d2a50ea1fb7` as the critic built it)
is not the adopted resolution (its R2 reading of the layer's files, R3 and R4 are unbuilt), so by § What the synthesis
adds before any session, *"where the landed build does not exist the run waits"*.
