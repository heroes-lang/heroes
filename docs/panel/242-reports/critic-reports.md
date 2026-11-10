# Panel 242, the completeness critic's second pass, over the briefs and the four reports

Written 2026-11-10, from about 13:00 to 13:50, as the checks ran.

**What was read**: `242-briefs/00-shared.md` and the four seat briefs as rewritten on the first
pass (the first versions are kept as `*.before-critic.md`), and the four reports,
`242-reports/compiler-engineer.md` (619 lines), `ffi-pragmatist.md` (504), `spec-warden.md` (344)
and `historian.md` (844).

**How**: read-only against the trunk at `1edabb957`. Every command that writes ran in
`.claude/worktrees/p242-scratch/critic/` (`$C` below); the seats' scratch folders were read and
never written. The compiler for my one Heroes build was `.claude/worktrees/probe-2419/heroes-probe`
(`sources 45824e7205ca021e`, the tree's key), run from `$C`.

No verdict. Items run most important first, each with its command and what it printed.

## A. What is missing, or false, and changes what the sitting can adopt

### 1. A wrong value at exit 0, from the spelling the design document prescribes, is filed nowhere

The ffi-pragmatist's §1.4 measured, in C, that a variadic reading `va_arg(ap, long)` after an `int`
reads `int -1` as `4294967295` (`c2/slot.c`, this Mac and Linux arm64). It also measured that the
compiler accepts `value: i32` past `...`. Its own Heroes run held, 3 of 3, *"the slot's high half
happened to be zero"*. The finding is not in that report's §7 list of what to file, and the
coordinator filed only 2420.

The Heroes reproducer, run now (`$C/vararg/`):

```
$ cat slot.h      # static inline long slot_read(int count, ...) { ... va_arg(ap, long) ... }
$ cat main.hero
extern "slot.h"
    function slot_read(count: i32, value: i32) -> i64

function main()
    print(slot_read(count: 1, value: -1).to_str())
$ heroes-probe build main.hero -o slot        -> build exit 0, "wrote slot"
$ ./slot; ./slot; ./slot                       -> 4294967295 (three times)
```

That `i32` is the language's own spelling for a C `long` parameter:

- design.md:3890-3894, panel 234's R10, ratified: *"a parameter has a spelling every leg shares, a
  narrower width C converts exactly: `i32` for a C `long` on all four CI legs"*;
- spec § 13, lines 381-385: *"one that disagrees is refused, except a parameter C converts
  exactly"*.

Past `...`, C converts nothing beyond the default promotions. So a program written as the document
says builds at exit 0 and reads a wrong value. That is `blocking` by verification.md § Bounded
discovery's list (*"a wrong value"*) and rank 3 by CLAUDE.md § Precedence.

The near precedents, searched (`gh issue list --state all --search` with `variadic width`, `past the
header`, `va_arg`, `"..." i32 long`): 2106 and 2107 (closed) refuse an **`@`** parameter past `...`;
nothing covers a by-value integer narrower than its `va_arg` read. The Windows reverse (`i64`
passed where `long` is 32 bits) is unrun. **For the coordinator to file** (I write nothing to
GitHub). It also bears on question 2: route (c)'s `long` options are exactly this shape, and the
ffi-pragmatist's veto on (c) without a header rests on it.

### 2. Route (e)'s one stated premise is false on POSIX, measured, and four seats' approvals rest on it

The ffi-pragmatist's §3.4: *"`execvp` and `CreateProcessA` find the program itself through the
parent's `PATH`, not the child's."* Its own arm sets the child's environment first:

```
$ grep -n 'environ = hero_run_child_envp' -A1 p242-scratch/ffi-pragmatist/route-e.diff
144:+    if (hero_run_child_envp != NULL) environ = hero_run_child_envp;
145:     execvp(program, hero_run_words);
```

A probe of exactly those two lines (`$C/pathprobe/probe.c`): fork; in the child `environ =
{"PATH=/nonexistent-p242-critic", NULL}`; then `execvp("true", ...)`, the parent's `PATH=/bin:/usr/bin`:

```
this Mac:                       child exit 40: ENOENT: searched the CHILD's (staged) PATH
heroes-linux-arm64 (Debian 13): child exit 40: ENOENT: searched the CHILD's (staged) PATH
```

So under route (e) as built, a staged `PATH` **does** find the program on POSIX. The seat's premise
(unrun by me, since the box would need a file outside the repository) says Windows' `CreateProcessA`
searches the parent's. The door would mean two things on two systems, and it is a door of the header
decision 2228 promises from `1.0.0` (design.md:3639-3645).

No caller in the tree is hit today: no caller stages `PATH` and then runs a bare program name.
`case_setting.hero` starts by path or `/usr/bin/env`, as the seat read. But the compiler-engineer,
the ffi-pragmatist, the historian and the spec-warden all approve or rank (e) first on the seat's
§3.4 as written. The sitting owes a choice, written into the door's promise:

- the parent's `PATH` on every system (resolve the program before replacing `environ`, then
  `execv`);
- or the child's on every system (Windows searching the staged block).

Neither is built.

### 3. My first pass framed question 1 (ii) wrongly, and the framing reached a seat's verdict

My first pass, item 4, said `crypto/sha256` is *"the first one whose admission into the compiler
would delete code ... Over `core` 0.2.0 the deletion was zero (241's R1)"*. The shared brief carried
it as *"(ii) the one package (A) would delete code for today"*. That was wrong. Panel 241's route 1
was scoped to `fetch/`'s 33 calls; scoped to the whole compiler, (A) over `core` 0.2.0 deletes
code. The compiler-engineer's diff:

```
$ grep -c '^deleted file mode' p242-scratch/compiler-engineer/routeA-final.diff   -> 0
$ awk '/^diff --git/{f=$3} /^-function /{print f}' routeA-final.diff | wc -l        -> 16
```

16 functions in 12 modules, 186 lines with their comments (its report :89-96), no file. The
spec-warden built §3.5 on the brief's framing: *"(A) can therefore replace nothing until
`crypto/sha256`, or `path/filepath` over defect 2406's copies, is written and in the index."* By the
compiler-engineer's measurement, that holds only if "replace" means "delete a module".

The two rulers then split inside the spec-warden's own report:

- **W3's wording** (*"the step that adds the entry deleting the compiler's own copies of that
  package's contract"*) is **met** by (A) over `core` 0.2.0 today;
- **the verdict's condition** (*"object to any step that takes a package and deletes no compiler
  module"*) is **not met**: `cli/strings.hero` shrinks 218 to 126 lines and survives.

The coordinator's summary, *"approves (A) with wording W3 under the condition that it deletes a
compiler module"*, joins the two. **The synthesis must pick one ruler**, copies or a module, and
write W3 to say it. W3 as drafted would admit the route the spec-warden's verdict refuses.

### 4. Two ratified texts disagree on whether Principle 0 reaches the compiler's catalogue, and the seats split along them

```
$ sed -n 272,274p docs/panel/234-*.md    (R1, ratified 2026-11-06)
... **Principle 0 binds no package and no function of one, and binds every form, built-in and
sentence of the specification the milestone would add** ...
$ sed -n 181,182p docs/panel/241-*.md    (R1, ratified 2026-11-10)
... Principle 0 admits a form the compiler needs or one with a measured effect; this one has neither.
```

The seats read them differently:

- **compiler-engineer** (:450, :482-486): *"Precedence's rank 3 does not decide question 1; rank 4,
  Principle 0, does"*, and *"neither (A) nor (C) enters on need"*;
- **spec-warden** (§3.1): *"Principle 0 does not reach (A), by a ratified ruling ... my veto on
  Principle 0 has no form to fall on"*. The burden that binds is design.md §1.7 and 241's R3.

The question is checkable by reading, and it is not a seat's to settle. If 234's R1 governs, the
compiler-engineer's objection to (A) stands on §1.7 and on cost alone. If 241's R1 governs, the
spec-warden's approval of (A) has a burden it said it did not have. The synthesis must say which
text governs, and why.

### 5. "(C)" names four different objects in the four reports

| who | what they judged as (C) | its state |
|---|---|---|
| the brief | a copy as the compiler's own modules, held to the index by a net check | unspecified |
| panel 241 (carried) | the copy under `selfhost/core/`, or a `path` entry over it | refused exit 1; the `path` entry told *unverified* on every check |
| compiler-engineer | the copy at `selfhost/`'s root (`selfhost/strings.hero`, `selfhost/strings/internal/scan.hero`), no catalogue | `check` exit 0, stage 2 built (`sources 6eb7ef290fd22e94`), net 7 green and 2 red (`layout` 5 modules at 301, `records/areas`); **stage 3, the fixpoint and the compiler's own tests unrun** (:302-303); the check holding it to the index **unwritten**, 30 to 40 lines estimated |
| spec-warden | the letter's reading: *"forbidden too, or it ranks the unverified copy above the verified form"* (§3.3) | written before the compiler-engineer's root placement existed (12:26 against 12:57, the files' times) |
| historian | Go's shape: a copy at a recorded version, **a manifest beside it** (*"as Go's `cmd/go.mod` stands beside `cmd/vendor`"*), and a separate check (`moddeps_test.go`) | precedent only |

The compiler-engineer's root placement has no manifest at all. The version lives only in the net
check that is not written. **A route nobody built**: the historian's hybrid, a catalogue entry as the
manifest beside the copy. Panel 241 measured the nearest form, a `path` entry, told *unverified* on
every `heroes check`. Whether that note can stand silent for a copy the net holds is unasked.

And the root placement is not general. A package's root becomes the compiler's root:

```
$ for n in strings strconv encoding path crypto os net io bufio time math sort slices log rand bytes; do
    [ -e selfhost/$n ] || [ -e selfhost/$n.hero ] && echo TAKEN: $n; done
TAKEN: bytes
$ grep -n '^| `bytes`' docs/roadmap/milestones/M-core-packages.md     -> 28: | `bytes` | ...
```

`selfhost/bytes.hero` is the compiler's own module, and `bytes` is a planned package. A copied
package whose root-relative `use` names `bytes` would resolve to the compiler's module, silently.
Today none does: `grep -rn '^use ' packages/core/0.2.0/files` shows every `use` inside its own
package. Under (A) the placed folder is the package's root, so no such collision exists.

### 6. The per-check costs in the compiler-engineer's verdict compare two batches

The verdict (:470-476) sets *"(A) ... every check costs +0.19 %"* beside *"(C) ... costs every check
+0.30 %"*, which reads as (C) dearer. The logs:

```
$ cat p242-scratch/compiler-engineer/instr-check.txt    (batch 1; base median 110,667,202,906; A on A +0.193 %)
$ cat p242-scratch/compiler-engineer/instr-halves.txt   (batch 2; base median 110,520,836,370)
```

- The same base binary on the same tree reads **146 million (0.13 %) apart** between the batches,
  the size of the effects compared.
- Inside batch 2, with one binary: (A)'s tree +366,494,415 (+0.332 %) and (C)'s +330,304,196
  (+0.299 %). (A) was above (C) in **4 of 4 rounds**, by 45.1, 30.7, 14.9 and 45.8 million (my
  arithmetic over the file; every median the report gives reproduces).
- (C)'s own binary on its own formatted tree was never measured; the (C) tree timed still held
  139 blank lines.
- The report's *"replacing the copies buys back about a third of [panel 241's +0.285 %]"* (:207)
  compares a 2026-11-10 afternoon batch with panel 241's, on another tree under another load. It is
  an inference across sittings.

### 7. Question 1 (ii): the seats judged different routes, and the one most of them name was costed by nobody

- **compiler-engineer**: object, costing *"about 430 lines"* deleted (`cli/sha256.hero` 292,
  `parts/sha256.c` 126, 8 binding lines) and *"+0.14 % of a check"*. That is the fingerprint moved
  into Heroes.
- **ffi-pragmatist** (§4, Q1 (ii)): its condition is *"the two C doors and the agreement test
  stay"*. The spec-warden's W2 and W3 say the same, and the historian objects to the verifier
  checking itself.
- **Nobody built or counted** the route those three name: `crypto/sha256` replacing
  `cli/sha256.hero` alone, the doors kept.

And the interface does not fit:

```
$ grep -rhn '^function' p242-scratch/compiler-engineer/ev241/.../shad/1.0.0/files/crypto/sha256.hero
105:function sum256(data: [u8]) -> str
113:function sum_file(path: str) -> str?
$ sed -n 338,344p spec/heroes-spec.md   # the Built-ins sentence: `chars`, no `bytes`
```

The compiler's three live call sites are `sha256.of(text: str)` (`module/placed.hero:112,117`,
`cli/check_json.hero:132`). The spec names no `str` to `[u8]` conversion, so each site would copy
the text by hand, or the package needs a function over `str`. The spec-warden's P5 (*"a net loss of
at least 250 lines"*) is therefore unmeasured. The compiler-engineer marked the interface question
*unread* (:242-243).

### 8. Route (D) was measured on route (C)'s tree, and four suites that walk the tree know no placed tree

The compiler-engineer's (D) runs (:578-589) were made with `./heroes-c2` on route (C)'s tree:
`order` 3 and 0, `packages` 13 and 0 (*"green because blind"*), `records` 27 and 1 (the red being
(C)'s). On the base tree, (D) is unrun.

What knows a placed tree:

```
$ grep -ln '^use placed_trees' tests/harness/*.hero
suite_canonical suite_corpus suite_emission suite_packages suite_records suite_unseen
$ for s in order layout spec probe; do grep -c 'placed_trees\|heroes\.packages' tests/harness/suite_$s.hero; done
0 0 0 0
```

- `canonical` and `unseen` already exempt any `heroes.packages` folder by name
  (`suite_canonical.hero:131`, `suite_unseen.hero:273`). So two of panel 241's *"walks to teach"*
  are taught.
- `order`, `layout`, `spec` (its `anchors` walk `tests/`) and `probe` (its `multi` family walks
  `tests/`) are not, and they are unrun under (D).
- The net's own tests are unrun under (D).
- Placed trees are committed (`git ls-files 'examples/*/heroes.packages/*' | wc -l` -> 195), so
  (D)'s and (A)'s are too.

### 9. Route (e)'s compiler half and its Windows limit are unbuilt or unmeasured

- **The compiler half**: the compiler-engineer's *"about 5 lines"* in `cli/process` is an estimate.
  The five callers are not moved, no stage 2 over `rt-e` exists, and `case_setting`'s cases are
  unrun under it. The ffi-pragmatist ran two of the compiler's own tests in `tree-e2/` and
  programs built with `HEROES_RUNTIME=<rt-e>`.
- **The Windows limit**: the arm keeps `CreateProcessA` with an explicit block (`route-e.diff:153-157`,
  `CREATE_UNICODE_ENVIRONMENT`). The historian verified Microsoft's sentence: *"The ANSI version of
  this function, CreateProcessA fails if the total size of the environment block for the process
  exceeds 32,767 characters."* No seat joined the two. The box's environment reads 1,929 bytes
  (`ssh win 'env | wc -c'`, read only), and a CI runner's is unread. Whether `CreateProcessW` should
  carry the block is unasked.

### 10. No route keeps a value the user set, and Go corrected exactly this

`fetch/transport.hero:160-161` writes `GIT_TERMINAL_PROMPT=0` and `GCM_INTERACTIVE=never`
unconditionally (`sed -n 159-161p`). Route (e) as built replaces a namesake (ffi-pragmatist §3.3).
The historian verified Go's `modload/init.go` (*"If user has explicitly set GIT_TERMINAL_PROMPT=1,
keep prompting"*) and golang/go#12706, whose correction this is. The historian also wrote that
*"Whether Heroes' write leaves a value the user set in place ... was not read"*. Read now: it does
not. No seat put the question to the routes.

### 11. One cross-seat question is already answered, and the synthesis should join it, not carry it open

The historian (Q3, :669-677, and its condition) asks whether a child started by `CreateProcessA`
with no block sees a `_wputenv_s` write, *"the box's to run"*. The ffi-pragmatist ran it (§3.2):
over `_wputenv_s`, `put "set"` reads `[set]` in the child. The historian's condition, *"which would
make panel 241's recorded route fail on its own terms"*, is therefore not met.

## B. Costs nobody counted, by the four the brief names

12. Each of the four:

- **The seed**:
  - (A)'s cost is counted (+11,736 bytes, 13 `#line`s naming the placed folder);
  - (C)'s stage 3 and fixpoint are unrun (item 5);
  - (e)'s compiler built over the new runtime is unbuilt (item 9);
  - (c)'s line is counted, and moves with each placed folder's key (ffi §1.8).
- **The latency of `heroes check` on the compiler**: measured for (A) and (C) in instructions (item
  6). Unmeasured for (e) and for any `crypto/sha256` route that keeps the doors (item 7). Each
  (D) net run also builds the harness over a placed tree; that cost is unmeasured.
- **The site**: (c)'s cost was counted on the first pass. Beside the sitting, under every route:
  `site/src/html/why.html:147-149` tells readers *"there is no package manager and no registry to
  pull it from"*, and `site/src/html/it/why.html:152-153` says the same in Italian (*"né un package
  manager né un registry da cui prenderla"*). Both have been false since M-package-manager
  (`heroes add`, the index at `packages/`; tag `m-package-manager` at `1edabb957`). For the
  coordinator to file; a fix publishes the site.
- **Windows**:
  - (A), (C) and (D) ran on no platform but this Mac (compiler-engineer :606);
  - (b), (d), (e) and the empty value ran on the box (ffi-pragmatist);
  - (e)'s 32,767 limit and `PATH` lookup on Windows are unrun (items 2, 9);
  - the `i32` past `...` reverse on Windows (an `i64` for a 32-bit `long`) is unrun (item 1).

## C. Smaller corrections

13. **My first pass's "21 functions"** for `cli/process.hero`'s `hero_os.h` group was wrong: `sed -n
40,64p selfhost/cli/process.hero | grep -c '^    function'` -> 18, constants -> 2. The 21 counted
the whole file, the `stdlib.h` group's three included. The compiler-engineer's correction (:385-386)
stands.

14. **libcurl's rung of §4.19's ladder.** design.md:3079-3082 makes *"fetch a URL"* rung 4 and says
the write callback is *"fixed at `ptr` until a C-width type vocabulary exists"*.
`examples/curl/main.hero:30` says *"No request is made"*. The ffi-pragmatist's prediction 2 (no
callback spelling both builds and reads the bytes, §1.3) is therefore consistent with the tree.

15. **The libcurl macro premise**: the ffi-pragmatist's §1.7 corrects my first pass (*"libcurl 8.18
makes `curl_easy_setopt` a macro"*). Its measured reading is that the type checker is included under
clang only when libcurl's guard names `__clang__` (8.23.0 here, 8.18 on the CI), and 8.7.1 and
8.14.1 never include it. Its correction stands. Defect 2420 (`gh issue view 2420` -> OPEN,
`blocking`) is the refusal it met first.

16. **The tally the synthesis starts from**, as written in the reports (no verdict of mine):

| question | compiler-engineer | ffi-pragmatist | spec-warden | historian |
|---|---|---|---|---|
| 1 (i), inside B5? | no, measured (`noreach/`) | — | no, read to source; P1 | no, by precedent |
| 1, the form | object (A); (C) at root if any; (D) first | — | approve (A) with W3; object (B), (C) | (C)'s shape, (A)'s catalogue beside it |
| 1 (ii) | object, the fingerprint moved | doors stay | W2/W3: doors stay | object to self-verification |
| 2 | (a); object (b), (c), (d) | (a), (b) and (d) at the boundary; (c) object, veto without a header | (a), (b), (d); object (c) | (a); (c) unsupported |
| 3 | (e), conditional | (e); object 241's route; object keeping it | (e) first, 0 tokens | (e) |
| 4 | (D), then `strings.Split` | none | (D), then `crypto/sha256` | none |

The splits that need a ruling, not an average:

- question 1's form, (A) with W3, (C) at the root, or Go's hybrid (items 3, 4, 5);
- question 4's first compiler package, `strings.Split` against `crypto/sha256` (items 3, 7).

## What the synthesis must not claim

- **That (A) over `core` 0.2.0 "replaces nothing".** Scoped compiler-wide, it deletes 16 functions,
  186 lines, in 12 modules, and no file (item 3). Nor that it closes a crash or a wrong answer: the
  compiler-engineer found none among the 13 live copies (:440-449).
- **That W3 refuses a step that deletes no module.** As drafted, it refuses only a step that
  deletes no copy (item 3).
- **That Principle 0 decides question 1**, without saying which of two ratified texts governs
  (item 4).
- **That "(C)" was measured** without naming which (C). The root placement's stage 3, fixpoint, own
  tests and index check are unrun or unwritten, and it collides with `selfhost/bytes.hero` for a
  planned package (item 5).
- **That (C) costs more per check than (A)**, or that replacing the copies "buys back a third" of
  panel 241's cost. The first compares two batches 0.13 % apart; the second compares two sittings.
  In one batch, (A)'s tree cost more than (C)'s in 4 of 4 rounds (item 6).
- **That route (e) gives a staged variable one meaning on every system.** On POSIX, as built, a
  staged `PATH` finds the program (measured on this Mac and Linux); on Windows, by the seat's
  premise, it does not (item 2). Nor that (e) is built: its compiler half is not, and its Windows
  block's size limit is unmeasured (item 9).
- **That the verifier-among-the-verified question was run.** The compiler-engineer answered it
  from the compiler's facts, *"not from a run of the attack"* (:254-255). What is agreed is a
  condition, the C doors stay, not a measurement.
- **That `crypto/sha256` in the compiler deletes about 250 or 430 lines.** The 430 includes the
  doors the other seats keep; the 250 has no interface that serves the compiler's `of(text: str)`
  (item 7).
- **That no package manager runs `curl` as a program.** The historian's six are *"what was read"*,
  and for three of them (Go, uv, npm) the HTTP library was not read.
- **That (D) costs nothing.** It was measured on (C)'s tree, with `order`, `packages` and `records`;
  `spec`, `probe`, `layout` and the net's own tests are unrun under it, and `packages` cannot see it
  (item 8).
- **That every finding beside the sitting is filed.** The `i32`-past-`...` wrong value (item 1) and
  the site's two false sentences (item 12) are not; 2420 is.
- **That the spec moves 0 tokens under a § 13 word for the environment.** The +3 is vendored; the
  real price is unrun (spec-warden §2). Under the routes as listed, 0 is the measured vendored
  delta, and the real is unrun by design (no `--refresh`).

## State left

- **The trunk**: nothing written there by this pass. `git -C /Users/joseph/Temp/heroes/heroes-lang
  status --short` prints nothing. My one build ran from `$C/vararg/`, and its `build/` cache is
  there: `find build -newer $C/vararg/slot.h -type f | wc -l` in the trunk printed 0.
- **Someone else's write**: the trunk's `build/` was last written at 12:58:05 (`build/reads`,
  `build/525b56326dbf149e`), after my first-pass slip (12:07:50) and before this pass's first file
  (`$C/pathprobe/probe.c`, 13:02:08). Not mine; recorded so it is not read as mine.
- **Written in `$C`**: `pathprobe/` (`probe.c`, `probe-mac`, `mac.txt`, `linux.txt`), with the
  container mounting `$C/pathprobe` alone, and `vararg/` (`slot.h`, `main.hero`, `slot`,
  `build.log`, its `build/`).
- **The box**: one read, `env | wc -c`, with no file written.
- **Nothing else**: no GitHub write, no paid run, no credential printed.
