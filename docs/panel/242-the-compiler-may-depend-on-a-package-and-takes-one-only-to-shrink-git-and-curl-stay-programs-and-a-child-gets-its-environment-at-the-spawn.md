# 242: the compiler may depend on a package and takes one only to shrink, `git` and `curl` stay programs, and a child gets its environment at the spawn

**Status** a full sitting, convened without asking again (the author's panel gate for
M-core-packages was asked at 07:35 on 2026-11-10), its seats chosen by the author's answer the
same day at about 11:55, *four seats, no blind seat*. The resolution is `provisional — author
ratification pending`.
**Seats** compiler-engineer, ffi-pragmatist, spec-warden, historian; the completeness critic on
the briefs before any seat (18 items, the briefs rewritten on them, the first versions kept as
`242-briefs/*.before-critic.md`) and on the reports after (16 items). No llm-ergonomist: no route
listed moves a token of the specification (the spec-warden: 8314 vendored and 10,898 real before
and after, digest `18bc6ed030668b91`), so the blind seat's input would not differ. No paid run.
**Base** `1edabb9573a00db79fadda2bff695e98371a3290` (`m-package-manager`).
**Briefs** `docs/panel/242-briefs/`, **reports** `docs/panel/242-reports/`, **evidence**
`docs/panel/242-reports/evidence.tar.xz` (its note beside it).

## The proposal

Panel 241 (ratified 2026-11-10, R8's order reversed) sent this sitting four questions
(`242-briefs/00-shared.md` § The four questions):

1. (i) whether the compiler depending on a package, as any program does, falls inside panel 234's
   R1 veto of route B5, the source of design.md:570-572 (*"No package is compiled into the compiler
   or the runtime"*); the form, if any: (A) a catalogue entry, (B) none, (C) a copy held by a check
   of the net, (D) programs outside the compiler first; (ii) whether the compiler's fingerprint may
   become a package's code;
2. what the second version's transport becomes: (a) `cli/process` as today, (b) an `os` package,
   (c) a `net/http` package over libcurl, (d) only the layer that runs `git` and `curl`;
3. where the environment's write lives: `hero_env_put` into `hero_os.h` over `_wputenv_s`
   (panel 241's recorded route), (e) the child's environment given at the spawn, or as today;
4. the order of what follows.

## The verdicts

| question | compiler-engineer | ffi-pragmatist | spec-warden | historian (advisory) |
|---|---|---|---|---|
| 1 (i), inside B5? | **no**, measured on the built compiler | — | **no**, read to its source | **no**, by precedent |
| 1, the form | **object** to (A) today; (C) at `selfhost/`'s root if the compiler must carry one; **approve (D)** first | — | **approve (A)** with W3; object (B), (C) | **(C)'s shape**, a catalogue beside the copy |
| 1 (ii) | **object**: the fingerprint moved into Heroes | the C doors and the agreement test stay | W2, W3: the doors stay | **object** to self-verification |
| 2 | **approve (a)**; object (b), (c), (d) as the compiler's | **approve (a)**, (b) and (d) at the boundary; **object (c)**, **veto (c) without a header of its own** | approve (a), (b), (d); object (c) | approve (a); (c) unsupported |
| 3 | **approve (e)**; object to the move into `hero_os.h` | **approve (e)**; object to 241's route and to today's | (e) first, 0 tokens | **approve (e)** |
| 4 | (D), then `strings.Split`-shaped helpers | none at the boundary | (D), then `crypto/sha256` | no verdict |

No veto falls on a route this resolution adopts.

## What each seat measured

**The compiler-engineer** (`242-reports/compiler-engineer.md`), in its own worktree, its compiler
built from the seed:

- **B5's scope, built**: route (A)'s compiler holds the prelude's text once and `core`'s source
  zero times, and three programs with no catalogue are refused byte-identically by it and by the
  base (`noreach/`). So a dependency of the compiler is not what B5 vetoed.
- **(A) scoped to the whole compiler** over `core` 0.2.0: two stages and their fixpoint (`cmp`
  silent), the compiler's own tests 2,006 passed; it deletes 16 functions, 186 lines, in 12 modules,
  no whole file, moves 654 calls, and adds 2,334 placed lines: the compiler's tree grows by 2,325
  lines net, every `core` version a seed refresh (+11,736 bytes, 13 `#line`s naming the placed
  folder). The 13 live copies it would replace are correct: 0 `.slice(` calls among them, so (A)
  closes no crash and no wrong answer. The trunk's guard refuses every net suite on it.
- **(C) at `selfhost/`'s root**, a placement nobody had listed: `check` exit 0, stage 2 built, the
  hook mirror keys it as the compiler does; the net 7 green and 2 red (`layout`, five modules at 301
  lines; `records/areas`); stage 3, its fixpoint, its own tests and the check holding it to the index
  unrun or unwritten (30 to 40 lines estimated).
- **Instructions retired per `heroes check selfhost/main.hero`**, one batch, one binary: (A)'s tree
  +366,494,415 (+0.332 %), (C)'s +330,304,196 (+0.299 %), (A) above (C) in 4 of 4 rounds (the
  critic's reading of `instr-halves.txt`); the fingerprint is about 36 million of it.
- **Question 1 (ii)**: Heroes' SHA-256 at `-O0` costs 1,933 instructions a byte against 316 through
  the C door; moving the fingerprint into Heroes would cost about +0.14 % of a check.
- **(D)**: with `core` in the net's own catalogue the hook mirror's key did not move and the guard
  admitted `order`, green, on (C)'s tree; the net's own `strings` module holds three copies called
  1,459 times.
- **Question 4**: for the compiler, `path/filepath` has 0 copies whose answer matches Go's on every
  input; `path.Split` has 4 and `strings.Split` 4.

**The ffi-pragmatist** (`242-reports/ffi-pragmatist.md`), on this Mac, the Linux arm64 container
and the Windows box, its compiler built in two stages on each:

- **(a)**: the `curl` program is on macOS (`/usr/bin/curl` 8.7.1) and Windows (`System32\curl.exe`
  8.16.0, Git's 8.21.0); libcurl is on neither machine's link line by default, and the container
  has libcurl and no `curl` program.
- **(b) and (d)**: an `os` package over `hero_os.h`, bound from a package, built and ran on all
  three (read `Darwin`, `Linux`, `MINGW64_NT-10.0-26100`; exit 3 told as 3; a missing program by
  its system number), `--sanitize` clean here; task 2263 owes one ownership sentence for each of the
  six `HeroStr` results it binds.
- **(c)**: without a header of the package's own, unsound by measurement: a `cstr` write callback
  read 2 of 7 bytes holding a zero; a function type cannot spell `counted_by`; past
  `curl_easy_setopt`'s `...` no width is checked, an `int -1` read as `long` came back 4294967295;
  `curl_easy_getinfo` is refused. With a 43-line header it is sound against libcurl 8.7.1, 8.14.1
  and 8.23.0, and its cost is the seed's line (`-I <the placed folder>` and `-lcurl`, both needed),
  the box (no `curl/curl.h`, no `curl.lib`), and a `heroes` that exits 127 for `--version` in a
  Linux without libcurl.
- **Question 3, on the box**: `_wputenv_s(name, L"")` **removes** the variable where `setenv(name,
  "", 1)` keeps it empty, so panel 241's route changes what `""` means on Windows; today's
  `SetEnvironmentVariableW` keeps the POSIX meaning for the child and misses the CRT's copy.
- **Route (e), built** in a scratch runtime, 128 lines (`p242-scratch/ffi-pragmatist/route-e.diff`):
  on all three machines the child read `café ∂ 日本 😀` whole and the empty value as empty, an
  override replaced the parent's variable by each system's own rule, the parent's environment was
  never written; sanitizers clean here and on Linux.
- **Beside the sitting**: `shadowed_header` refuses Homebrew's keg-only libcurl 8.23.0 (defect 2420,
  filed and reproduced by the coordinator).

**The spec-warden** (`242-reports/spec-warden.md`): every route moves the specification by 0
(8314 vendored); *"and the environment"* in spec § 13 would cost +3 vendored, real unrun;
CLAUDE.md 0 under every route but (c) (+3). design.md:570-572 widened when it was copied: panel
234's R1 said *shipped*, and commit `79a42b509` wrote *package* for *core package* and *compiled
into* for *shipped with*. Three wordings priced (W1 +99, W2 +136, W3 +171 vendored on design.md,
which no ceiling judges); five predictions.

**The historian** (`242-reports/historian.md`): Go, rustc, Cargo, Nim and pip build their
toolchains from their own ecosystem's packages and no page read bans it; Go carries a copy with a
manifest beside it (`src/cmd/vendor`, `src/cmd/go.mod`) and a test holding them; pip vendors
because *"you can't use pip to fix pip"*; Rust moved `compiler-builtins` off crates.io over a
bootstrap cycle; Go, uv, pip and npm run `git` as a program; Rust 2024 made `set_var` unsafe and
points at `Command::env`; Go's `modload` writes `GIT_TERMINAL_PROMPT` only where the user has not
(issue 12706), and Go's open issue 63567 records `os.Setenv` crashing beside glibc's `getenv`.

**The completeness critic** (`242-reports/critic-briefs.md`, `critic-reports.md`), whose second
pass this file rests on where it differs from a seat: a wrong value at exit 0 nobody had filed, an
`i32` for a variadic `long` (defect 2421, filed and reproduced by the coordinator); route (e)'s one
premise false on POSIX (a staged `PATH` is the one `execvp` searches, measured here and on Linux);
its own first pass's framing of question 1 (ii) wrong (route (A) deletes 16 functions, not zero);
two ratified texts read apart on Principle 0; *(C)* naming four objects; a cost comparison across
two batches; the site's why page saying there is no package manager (defect 2422, filed).

## The resolution adopted, provisionally

`provisional — author ratification pending`. The most robust and complete resolution (CLAUDE.md
§ 4).

- **R1, a dependency of the compiler is not B5.** Panel 234's R1 vetoed source shipped inside the
  compiler for programs to reach, the prelude's case (panel 097's condition 5); the compiler
  depending on a package for its own code, as any program does, is a different act, measured (the
  compiler-engineer: `core`'s text zero times in the binary, programs without a catalogue refused
  byte-identically), read back to the source (the spec-warden) and reached first by every toolchain
  the historian read. **design.md:570-572 is amended to its source**, in its own commit, in W3's
  shape with R2's ruler (below): no package is shipped with the compiler or the runtime for
  programs, nothing a program reaches without an entry of its own catalogue is a package's code, no
  package reaches `selfhost/library_source.hero`; the compiler may carry a package through its own
  catalogue, which no program it compiles reaches; it checks every placed tree with the runtime's
  SHA-256 door and never with a package's code; and it takes one only by R2's ruler.
- **R2, the ruler: the compiler takes a package only where its own tree shrinks or a measured
  defect closes.** A step that adds an entry to `selfhost/heroes.toml` deletes every live copy of
  that package's contract in the compiler, **and** either the compiler's tree, placed files counted,
  is smaller after it than before, or a copy it deletes is measured crashing or answering wrong on
  an input a caller reaches. This settles the critic's item 3: W3's *"deleting the compiler's own
  copies"* alone would admit (A) over `core` 0.2.0 today (16 functions deleted, +2,325 lines net, no
  defect closed), which three seats refuse on cost; the module-deletion condition alone would refuse
  a package that closes a crash. **Which ratified text governs** (the critic's item 4): panel 234's
  R1, *"Principle 0 binds no package"*, stands, since a package is code a program carries and no form
  of the language; what binds the compiler taking one is design.md §1.7 (*does it remove a special
  case from the compiler?*) and §1.1's ceiling, which panel 241's R1 applied under Principle 0's
  name. Panel 241's R1 stands on those grounds, and so does its conclusion: **the compiler takes no
  package of `core` 0.2.0**, since by the ruler it neither shrinks nor closes a defect.
- **R3, the form, when the ruler is met: (A), the catalogue entry.** A placed tree is a committed
  copy, the catalogue is its manifest and the fingerprint its check, at every build: Go's
  `cmd/vendor` beside `cmd/go.mod` and its test, the historian's hybrid, with the check at every
  build rather than on a builder with the network, and the one mechanism every program already uses,
  so no special case (§1.7). **(C) at `selfhost/`'s root is not adopted**: it has no manifest, its
  fixpoint and its index check are unrun or unwritten, and a package's root becoming the compiler's
  collides with the compiler's own `selfhost/bytes.hero` for the planned `bytes` (the critic, item
  5). Panel 241's R3 conditions stand for (A): the trunk's provenance mirror follows `use <name>/`,
  `order`, `layout`, `spec` and `probe` learn a placed tree, and each `core` version the compiler
  takes is a seed refresh.
- **R4, the verifier stays out of the verified.** The compiler's fingerprint of every placed tree
  stays the runtime's C doors (`hero_file_sha256`, `hero_text_sha256`), held to the Heroes copy by
  the agreement test (`module/fingerprint.hero:76`); no package's code computes it. A `crypto/sha256`
  package may replace `selfhost/cli/sha256.hero` only (placed-folder names, `check --json`), and only
  with a function over `str` serving its three call sites, since the spec names no `str` to `[u8]`
  conversion; that route is costed by nobody (the critic, item 7) and is built before it is taken.
- **R5, the transport: `git` and `curl` stay programs, (a).** The second version replaces no
  transport. **(c) is refused for the compiler**: without a header of its own, by the
  ffi-pragmatist's veto (a wrong value past `...`, defect 2421; 2 bytes of 7 through a `cstr`); with
  one, by its cost (the seed's line, a library every verb loads, the Windows leg, row 83's *nothing
  else installed*). **(b), an `os` package over `hero_os.h`, is approved for programs**, built on
  three platforms, with task 2263's ownership sentences; the compiler's `cli/process` does not move
  onto it (96 modules and 764 calls behind a placed tree, the compiler-engineer).
- **R6, the environment: (e), the child's variables given at the spawn.** A `hero_run_env(name,
  value)` staged as `hero_run_arg` is, read by the spawn on every system, so no process-wide write
  exists; `hero_env_put` leaves `hero_compiler.h` once its five callers move; it joins the
  `hero_run_*` doors of `hero_os.h` that decision 2228 promises, and spec § 13's *"processes ...
  from the language's `hero_os.h`"* already covers it (0 tokens). **Three choices the door's promise
  writes down**: (1) the program is found through the **parent's** `PATH` on every system, resolved
  before the child's environment replaces the parent's, so a staged `PATH` means one thing
  everywhere (the critic, item 2: today's route-(e) arm searches the child's on POSIX and, by the
  seat's reading, the parent's on Windows); (2) the Windows block is wide, `CreateProcessW` with
  `CREATE_UNICODE_ENVIRONMENT`, below no 32,767-character limit of the ANSI call; (3) a value the
  user set is kept: `GIT_TERMINAL_PROMPT` and `GCM_INTERACTIVE` are staged only where the user's
  environment does not hold them (Go's correction in issue 12706). Panel 241's recorded route, into
  `hero_os.h` over `_wputenv_s`, is refused: it changes what `""` means on Windows, measured.
- **R7, the order.** (D) first: the net's own catalogue takes `core` 0.2.0, as a program that is
  not the compiler and whose three copies are called 1,459 times, once `order`, `layout`, `spec`,
  `probe` and the net's own tests have run on it from the base tree (unrun under (D) there, the
  critic's item 8). Then the compiler's first package by R2's ruler, measured when one exists;
  `crypto/sha256` under R4 and the `path.Split`-shaped helpers are the candidates read. Feature 2315
  waits for that package; M-core-packages' acceptance condition gains R2's sentence.
- **R8, what was found beside it, filed**: defects 2420 (`shadowed_header` and Homebrew's
  libcurl), 2421 (an `i32` past `...` read as a `long`, a wrong value at exit 0) and 2422 (the
  site's why page, in both editions), each `blocking`.

**What the cheaper reading would have been**, recorded so the author can choose it: W1's wording,
the sentence read back to its source and nothing else (+99 vendored on design.md, against W3's +171
with R2's ruler); (C) at the root as the compiler's form, cheaper per check in one batch and with no
hook change; and closing feature 2315 now, since no route replaces anything over `core` 0.2.0.

## Disagreements, not smoothed

- **The form.** The spec-warden approves (A); the compiler-engineer objects to (A) today and
  prefers (C) at the root if the compiler must carry one; the historian prefers a copy with a
  manifest beside it. R3 takes (A), whose placed tree with its catalogue is that copy and that
  manifest, and records (C) at the root as the conservative choice.
- **The ruler.** The spec-warden's wording and its condition read apart (the critic, item 3); R2
  writes one ruler that both halves of the disagreement can check.
- **Principle 0.** The compiler-engineer's objection rests on panel 241's R1, the spec-warden's
  approval on panel 234's; R2 says which governs and on what ground.
- **Route (e)'s `PATH`.** The ffi-pragmatist's premise that the program is found through the
  parent's `PATH` is true of the Windows arm, by its reading, and false of the POSIX arm as built
  (the critic, measured); R6 chooses the parent's on every system, which the arm owes before it
  lands.

## Prior art, per resolution

R1 rests on Go's and Rust's toolchains, reached first; R3 on Go's `cmd/vendor` beside `cmd/go.mod`
and its test, and on pip's vendoring rule; R4 on pip's and Rust's reasons; R6 on Rust's
`Command::env` and Go's issues 12706 and 63567. Every page was re-read on 2026-11-10 by the
coordinator's fetch, each sentence found verbatim. The lines are the decision issue's `## Prior art`.

## Left open, as questions

- Route (e) on Windows with the parent's `PATH` resolution and a wide block: unbuilt; the compiler's
  half (five callers moved, `case_setting`'s cases) unbuilt.
- (D) from the base tree: `order`, `layout`, `spec`, `probe` and the net's own tests unrun.
- A `crypto/sha256` with a function over `str`, and the net lines it deletes from the compiler.
- The Windows reverse of defect 2421: an `i64` where `long` is 32 bits, unrun.
- The real token price of a § 13 word for the environment, unrun (no route adopted needs it).

## Predictions to score

- **compiler-engineer**: if any route moves the compiler onto `core/strings` before M-core-packages
  closes, the commit that lands it either splits or grants an allowance to `cli/package_reads`,
  `cli/toolchain`, `cli/units`, `emit/ffi_field` and `emit/ffi_narrowed`, or `layout` reads red on
  them; and `heroes check selfhost/main.hero` on its tree retires 0.1 % to 0.5 % more instructions
  than its parent's. Checkable at that landing, or M-core-packages' close.
- **ffi-pragmatist**: (1) a `net/http` package binding `curl/curl.h` with no header of its own is
  refused `ffi_macro_name` on the CI's two Ubuntu 26.04 legs and builds on Darwin; (2) no spelling of
  `CURLOPT_WRITEFUNCTION`'s data both builds and reads the bytes, until a function type takes
  `counted_by`; (3) route (e), landed, moves all five `hero_env_put` callers, retires `hero_env_put`
  from `hero_compiler.h`, and turns no case red in `fetch` or `case_setting` on the four CI legs; (4)
  a `heroes` built with `-lcurl` exits 127 for every verb on a Linux without `libcurl.so.4`.
  Checkable at the first CI run of such a package, at route (e)'s landing, and at row 83.
- **spec-warden**: P1, at the step that first writes `selfhost/heroes.toml`, the no-catalogue
  program in `p242-scratch/spec-warden/nocat/` still prints `error[unknown_entry]` at 1:5 and exits
  1; P2, the landings leave the spec at 8314 vendored and 10,898 real; P3, CLAUDE.md moves by 0; P4,
  at M-core-packages' close `module/fingerprint.hero` holds no `use <` line and a byte appended to a
  placed file is refused `package_unread`; P5, the commit adding `crypto/sha256` to the compiler's
  catalogue deletes `selfhost/cli/sha256.hero` for a net loss of at least 250 lines.

## Process notes

- Four seats by the author's answer, which is neither of `/panel`'s two lanes; what it gave up is
  the blind seat's reading of a specification no route moves.
- The critic's first pass changed the briefs in 18 places before any seat started; its second pass
  is where R2's ruler, R3's rejection of (C) at the root and R6's three choices come from.
- Two writes into the trunk's `build/` cache: the critic's first pass ran a compile there at
  12:06-12:07 (688 `build/tu-*` directories), and the coordinator's `issues.hero` run that filed
  defect 2420 at about 12:58; a cache, no tracked file moved, left in place.
- The ffi-pragmatist worked on the box in `C:/w/p242-ffi-pragmatist/` alone; the compiler-engineer's
  worktree holds route (C) uncommitted, route (A) saved in its scratch.

## Author's verdict

Pending. The sitting's decision issue, `Panel 242:`, is filed open. **What a yes settles**: R1 to
R8 as adopted, design.md:570-572 amended in its own commit, the compiler taking no package until
R2's ruler is met and then by (A), the fingerprint the runtime's, `git` and `curl` programs, route
(e) with its three choices as M-core-packages' work, (D) first. **What it does not**: the
predictions above, on their own clocks; the cheaper reading, which stays the author's to choose.

**Ratified, R1 to R8, 2026-11-10**: the author's answer at about 13:55 the same day, put with the
coordinator's recommendation through the question form, *ratify R1 to R8*, recorded as a reading
(`.claude/rules/records.md` § And the record says whose idea it was). With it, by the same form:
feature 2315 stays open and waits for the compiler's first package by R2's ruler; the next batch
takes all fifteen open defects and features 2424 (R6) and 2425 (R7); the site publishes with that
batch's push. What the yes settles: every resolution as adopted, design.md §1.11 as amended at
`498c32967`, now ratified. What it does not: the predictions above, on their own clocks.
