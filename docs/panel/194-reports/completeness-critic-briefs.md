# Panel 194, completeness critic, first pass (the briefs against the world)

Started 2026-10-06 12:19 (`date`). No verdict. Copy:
`<scratchpad>/194-critic/`, made 12:19:53 by
`git -C /Users/joseph/Temp/heroes/heroes-lang archive 44f3b802 | tar -x`, with
`<scratchpad>/round-b12/run-4254/seed-new.c` (sha256 `8d9cc9f4b1375ee1...`,
equal to facts.md's) copied over `seed/heroes.c`.

## The frozen tree

- `git log -1 --format='%H %P' 44f3b802`: parents `b3738dc9` (lane-round-b12)
  and `e14215ad` (lane-panel-194). So 44f3b802 is NOT only "bef739dd with
  panel 178's two commits merged in": it also carries `f78c4f69` (defect 387
  filed) and `b3738dc9` (the author's goal), and `e14215ad` (178's verdict
  superseded). `git diff --stat bef739dd 44f3b802 -- selfhost runtime seed
  spec tests`: empty, so the code is bef739dd's; the shared brief's sentence is
  imprecise about records only.
- `git worktree list`: lane-b12-ffi13 sits at `44f3b802` at 12:19, so the
  branch the compiler-engineer and ffi-pragmatist are told to read has no
  commit of its own yet; `<scratchpad>/batch12/ffi13/` holds only
  `base/`, `heroes-base`, `runtime-base` (11:58), no evidence.

(continued below as each command runs)

## Verified (between 12:19:53 and 12:24:03 by `date`, each by the command beside it, in `194-critic/`)

- Compiler: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, exit 0, 8,293,016 bytes (equal in size to `run-4254/heroes-round`), `heroes 0.2.0`.
- `git merge-base main lane-panel-178` = `517b8e25` (2026-09-25 10:49); `git rev-list --count 517b8e25..bef739dd` = **1020**.
- Per-file commits `517b8e25..bef739dd`: spec 11, walk 19, fmt 9, ast 2, container 2, ctype 1: all six equal the brief.
- `heroes measure spec/heroes-spec.md`: real **9518** (claude-opus-5, 2026-10-06), vendored maximum 7212, headroom 722, FFI floor 60 (so 9578 judged): equal.
- § 13: the record paragraph is lines 365-370 (364 is blank), the `cstr` abort sentence 386-387; `grep -n rest` empty; `` grep -n '`i8`' `` only line 63 (the type table): equal.
- Layout (`code_lines` replicated in awk on the rule of `suite_layout.hero:817-832`; DECIDED rows at `suite_layout.hero:448,449,453`): walk 1858/1870, fmt 1096/1175, ast 550/550: equal. `heroes run tests/harness/main.hero -- ./heroes layout`: 5 passed, 0 failed (12:22:21 to 12:22:46).
- ROADMAP row 63 `M-buildable-structs` `scheduled` (line 157); `grep -l 'milestone: M-buildable-structs' issues/*/*/*.md`: 6 files; defect files 091-094 under `issues/2026-09/24/`.

## FALSE or imprecise in the shared brief

1. **"`emit/container.hero` 2 (one of them batch 12's `f7576a01` ... not yet on this tree)" is false.** `git log --oneline 517b8e25..bef739dd -- selfhost/emit/container.hero` names `81532acc` (M-agreed-retention step 29, panel 182) and `d64da8ff` (step 15, defect 091's repair). `git merge-base --is-ancestor f7576a01 44f3b802`: not an ancestor; `f7576a01` is on `lane-b12-ir12` only, so it would be a THIRD commit, and it is the one that changes how an array literal is emitted (`selfhost/emit/container.hero` +20/-14, `construct.hero` +2/-1; `git diff --stat $(git merge-base lane-b12-ir12 44f3b802) lane-b12-ir12`). Any emitted-C number a seat takes for a construction holding a fixed array (178's *5555 lines of C* for one `utsname`) moves when ir12 lands.
2. **The frozen tree is not "bef739dd with 178's two commits merged in" only**: `git log -1 --format=%P 44f3b802` = `b3738dc9 e14215ad`; it also carries `f78c4f69` (defect 387 filed), `b3738dc9` (the author's goal) and `e14215ad` (178's verdict superseded). `git diff --stat bef739dd 44f3b802 -- selfhost runtime seed spec tests`: empty, so the code facts hold.
3. **The sitting is not on "the trunk as it is after batch 12"**, which is what the task issue says (*It is sat again on the trunk as it is after batch 12* ... *Taken after batch 12 closes*). At 12:24 `lane-round-b12` is at `1e424ec3`, 6 commits past `44f3b802` (fit12 merged); `lane-b12-ffi13` is at `44f3b802` with 0 commits of its own; `lane-b12-ir12` carries `f7576a01`. The brief should say the tree is batch 12's round mid-batch, and which lanes still land under it (ir12: `emit/container.hero`, `emit/construct.hero`; ffi13: `selfhost/emit/`, `cli/`), so the landing re-measures; or the author is told it was sat before the batch closed.

## The blind seat's input (`<scratchpad>/194-llm-ergonomist/t1..t3`), checked from 12:25 at the coordinator's request

**What holds.**

- `diff 194-critic/spec/heroes-spec.md t1/spec.md`: one line differs, 370, where `[RULE 1] ` is inserted after *elements as the type says.*; `cmp` says t1, t2, t3's `spec.md` are equal, and so are their `headers.txt`.
- `o1-check.txt` is what today's compiler prints: `task1.hero` copied to `194-critic/w/blind1/`, `heroes check task1.hero` plus `exit $?`, `diff` against `t1/o1-check.txt`: **equal** (five `fixed_array_length`, exit 1). `heroes build` stops on the same five, exit 1.
- t1 (uname) has a route under L today: `i8[256]` fields with 256 zeros each, `check` 0, `run` 0, prints `0` and `Darwin` (`w/blind1/fix_i8.hero`).
- t3 (mutex) has a route under L today: `record Mutex tag _opaque_pthread_mutex_t` with `__sig: i64`, `__opaque: i8[56]` (56 zeros), `record NoAttr tag void`, `pthread_mutex_init(@m, attr: nullptr)`: `check` 0, `run` 0, every call 0, counter 3 (`w/blind3/t3a.hero`).
- No file says which variant is proposed or current (`grep -rn -i 'panel\|178\|194\|propos\|adopt'` over the three folders outside `spec.md`: nothing); `o1-check.txt` is today's compiler, so L's, and says so nowhere, which is neutral as written.
- `~/.claude/CLAUDE.md` does not exist; no `CLAUDE.md`, `CLAUDE.local.md` or `.claude/` in any parent of `t1/` (walked to `/`).

**Found, each with its command.**

1. **t2 has no route that builds and is right, under any variant, from the spec and `headers.txt` alone.** Binding `connect(fd: i32, @addr: SockaddrUn, len: u32)` against the real header is refused at `build`: `ffi_parameter_type`, *`addr` of `connect` is declared a different kind of thing from the header's `const struct sockaddr *`*, exit 1 (`w/blind2/t2a.hero`); 178's `sun_bind_direct.hero` is refused the same way today. Panel 178's readers never met this: their task 2 bound a shim, `app_connect(struct sockaddr_un *)` in `w/readers/app.h` (`178-reports/completeness-critic-work/w/readers/I-01/t2.hero`), and 178's own sockaddr programs used `un_shim.h`. The one binding that builds is a `record Sockaddr tag sockaddr` (16 bytes, `sa_data: i8[14]`) lent with `len: 106`: `check` 0, `run` 0, prints `-1`, `--sanitize` 0 (`w/blind2/t2b.hero`); the path does not fit and C is told 106 bytes of a 16-byte record. So t2 measures the `sockaddr` wall and 092's shape, not K, L or M; give the readers the shim as 178 did, or say in the sitting that t2 is the wall's measurement.
2. **The method does not measure what 178's reader test measured.** 178's critic (`178-reports/completeness-critic.md` § 6): 50 fresh readers, **10 per variant, each shown ONE variant inserted in the spec text**, one attempt, **every program compiled and run on Darwin** with a compiler for that variant ("ok" = exit 0 and the right output; `u8[` normalised). Here: 3 sessions, **each shown all three variants side by side in `brief.md`** and writing under each, the spec holding only a marker, the readers judging their own programs "in your head". Three consequences:
   - **K's own effect cannot be measured**: the sentence *C's `char` is `i8`* is in every brief, read before writing under L and M, so the `u8` rate it is meant to move (178: 1 of 10 under I, 6 of 10 under each R1 variant, 22 of 50 overall) is contaminated in every variant;
   - N is 1 per task, so no rate in a `prediction` can be scored against these sessions;
   - nothing says who compiles the programs: under K and M they need `rest: zero`, which only the compiler-engineer's R1 build will have; the author's note for the step says *Judge readers by compiling their programs, not by stated beliefs*.
3. **The project's name reaches the readers beyond the spec's own title**: `t2/brief.md:22` names the path `/tmp/heroes.sock`; `t1/brief.md` and `o1-check.txt` name `task1.hero` (the extension); and a `claude -p` session is told its working directory, `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/...`. The spec names Heroes at its lines 1, 3 and 312, as in every blind sitting. `o1-check.txt` also prints `(§4.19)`, a section the reader cannot open.
4. **The user settings a `claude -p` loads by default** (`~/.claude/settings.json`, keys read, no secret): `language: italian`, `model: fable`, `effortLevel: xhigh`, and `~/.claude/skills/synced/` (docs, docx, pdf, pptx, xlsx, morning, ...). Unless the command passes `--setting-sources` without `user` (or `--settings`), `--model`, `--tools "Read,Write"` and `--max-budget-usd`, the reader may answer in Italian, on a model nobody named, with tools the brief forbids. **No budget bound is named in any of the three `brief.md` files nor in a seat brief** (`ls p194/briefs/`: no llm-ergonomist file), while the task issue says *its budget bound named in its brief*. `claude --help` (no paid call) lists `--max-budget-usd`, `--tools`, `--setting-sources`, `--model`.
5. **`headers.txt` simplifies one prototype the compiler will not**: it gives `pthread_mutex_init(pthread_mutex_t *m, const void *attr)`; Darwin's header says `const pthread_mutexattr_t * _Nullable`. A reader who binds `attr` as the file says may be refused at `build` where the real header was the judge (unrun; `t3a.hero` used `tag void`, which C converts).
6. **The task1 mistakes and the output**: `task1.hero` holds two, the one-element literals and `u8` for C's `char`; `o1-check.txt` shows only the first. With the lengths fixed and `u8` kept, `check` is 0 and `run` exits 1 with `ffi_field_type` on all five fields (`w/blind1/fix_u8.hero`), and **its note does not name `i8`** (*every field is at the header's own width and sign: correct `sysname`*). Step 3 will therefore measure a repair that is not one turn under L, which is fine if said; and 178's point 5 second half (*the note names `i8` if it does not already*) is unlanded: a route at zero spec tokens nobody listed (below).

## The largest gap: panel 186 moved the construction of a group record, and no brief names it

4. **"`missing_fields`: emitted at `selfhost/check/walk.hero:1986`" is half the world.** `grep -rn missing_fields selfhost`: a second emitter, `selfhost/emit/ffi_built.hero:160`, told from clang's marker `layout_sites.OMITTED`. `walk.hero:1772-1778`: *A group's record is built naming one member of each C union (panel 186 R7, spec § 13): a construction naming some of its fields holds only its labels here, and `build` judges what it leaves out (`check/group_fields.hero`)*: `if !rec.header.is_err() && given.len() != expected.len()` it calls `built_from_some` and returns before `check_named_fields`. So **`walk.hero:1986` serves only Heroes records and variant cases**; for a group record the refusal is `build`'s, from the header's layout. The 18 goldens under `tests/golden/unsupported/` are that `build` refusal (e.g. `ffi-a-construction-of-a-plain-struct-leaves-out-a-field.expected`: *this construction of `PT` leaves out `y`, which shares no byte with the fields it names, so C would build it with `y` zero*), whose own comment says *Until R7 this was `check`'s refusal; it is `build`'s now*.
   - Panel 186 (`docs/panel/186-...md`, **RATIFIED 2026-10-02 at 15:03**, R7 home (a)) is the very rule R1 relaxes, and it postdates 178 by eight days. None of the six briefs names it: `grep -c 186 p194/briefs/*` is 0 in each of the six files (12:30).
   - Consequences a seat would otherwise miss: 178's condition *the construction check leaves `check/walk.hero` for its own module* is already half met (`selfhost/check/group_fields.hero` exists, `selfhost/emit/layout_sites.hero` judges); R1 today is a relaxation of a `build` marker, not a `check` change for group records; and **R1 must say what `rest: zero` does to a union**, since § 13 now says a record is *built naming exactly one* member of each union: may `rest: zero` name none of a union's members, and which member's zero is it then (C11 §6.7.9p10 is the historian's to cite; unrun here)? 178's sentence says nothing of it.
   - 178's condition, in its own words (§ The resolution, 1): *a golden for `missing_fields` on a Heroes record **and on a group record** lands first*. The shared brief shortens it to *a golden for `missing_fields` first*. The group-record half exists (18 goldens, by 186); the Heroes-record half does not (`grep -rl missing_fields tests/golden`: 18 `unsupported`, 1 `run` comment, 0 `check`; `grep -rn missing_fields selfhost`: the only test is `ffi_built.hero:289`, the `build` path).
5. **The emission is already a compound literal.** `heroes build uname_darwin.hero --emit-c -o u.c` (178's `uname_darwin.hero`, one `i8[256]` field, `partial`): line 894 is `t258 = (struct utsname){.sysname = {t1, ..., t256}};`, 1,160 lines of C and 256 temporaries; `run` 0, prints `Darwin`. So 178's ffi-pragmatist veto 2 (*never member stores into a bare cell*) constrains nothing new today, and C already zeroes every field the designated list leaves out: what R1 removes is the refusal and the literal, not an emission. The **5555 lines** in 178 (`ffi-pragmatist.md:103`, its `uname_S_darwin`, five fields) are CARRIED, and lane ir12's `f7576a01` (array literal in one block) changes that number when it lands.

## More framing facts, run from 12:24 to 12:34 (`date` before and after each)

6. **"Linux x86-64: unrun today: no amd64 image here" is false.** `docker image inspect heroes-linux --format '{{.Architecture}}'` = `amd64` (created 2026-09-09). The coordinator's command, `docker run --platform linux/amd64 heroes-linux`, fails with *pull access denied* (a lookup quirk, reproduced 12:31:01); **without `--platform`**, `docker run --rm heroes-linux uname -m` prints `x86_64` (emulated), with Debian clang 22.1.8, `/usr/include/openssl/sha.h` present, 8 CPUs. So the x86-64 census (CARRIED 23), 178's x86-64-only padding finding and its x86-64 zero-validity rows can all be re-run today.
7. **MemorySanitizer builds and fires on both Linux images**: a positive control (`w/msan/ctl.c`, a read of `malloc`ed memory) gives *MemorySanitizer: use-of-uninitialized-value* and exit 1 in `heroes-linux` (x86_64) and `heroes-linux-arm64` (12:31:19 to 12:31:21).
8. **The zeroed mutex, CARRIED in the shared brief, holds today** (`w/zm/zm.c`, `memset` 0 then lock and unlock): Darwin `sizeof 64 lock 22 unlock 22`; aarch64 `sizeof 48 lock 0 unlock 0`; x86_64 `sizeof 40 lock 0 unlock 0` (12:32:37 to 12:32:39).
9. **Defects 091 to 094 re-run on this copy** (12:24:31 to 12:24:36), the reproducers from the issue files' own paths: 091 `check` 0, `run` 0, prints `72`; 092 `check` 0, `run` 138 after `4096` and `1094795585`; 093 `check` 0, `run` 134, the `.cstr()` NUL panic; 094 `check` 0, `run` 2, *internal error: compiling the generated C failed*. All equal to the brief. The historical seeds (`9e17d471`, `d64da8ff`, `ca5fa51e`, `6b33db23`) I did not rebuild; those halves rest on the issue files.
10. **The census's Darwin row mixes two counting rules.** Today's `census-darwin-i.tsv` holds the same 108 fields as 178's `census-darwin-arm64.tsv` (`diff`: only five OpenSSL lines, typedef spellings `SHA_LONG[16]` for `unsigned int[16]`, OpenSSL 4.0.3); the record sets are identical. Counting with awk (`$1>8`, records by name): 62 records including one anonymous (`__mbstate8 char[128]`, empty name), 43 public with it, 42 without. The brief writes **62** (with) beside **42** (without); 178 wrote 61 and 42 (without, its § 1 says so). So *62* is not a change in the world. The no-flag figure **39** counts the anonymous record (38 named), so 42 minus the four absent OpenSSL records is 38, not 39. `echo | clang -x c -E -v -` confirms `/opt/homebrew/include` is not searched.
11. **`/usr/bin/time -l` reports `instructions retired` on this Mac** (`/usr/bin/time -l /usr/bin/true`: 10,074,757). It counts the children too, and `heroes build` writes a cache under `build/` (it appeared in `w/uts/build/` on the first `--emit-c`), so a before/after count with the cache in different states measures the cache.
12. **178's R1 prototype does not apply to today's tree**: `patch -p8 --dry-run --batch < docs/panel/178-reports/compiler-engineer-work/r1-and-091.diff` in this copy: **14 of 36 hunks fail** (`check/walk.hero` 1 of 9, `data_errors.hero` 1 of 1, `emit/container.hero` 2 of 3, `grammar_expr.hero` 2 of 6, `ir/flatten.hero` 2 of 5, `print/fmt.hero` 6 of 6). 178's *+435/−27* and its three DECIDED rows are CARRIED, not a cost of today's tree.
13. **The Principle 0 count, run once as a check of the instruction**: `find . -name '*.hero'` outside `tests/`, `docs/`, `archive/` with a fixed-array field longer than 8 (`grep -E '^\s+[a-z_0-9]+: [a-zA-Z0-9]+\[(9|[1-9][0-9]+)\]'`): **0 files**; under `tests/golden`: 3. 178's lapse condition (*fewer than 3 bindings construct such a struct*) reads 0 today; its payment is registered at M-core-packages' close, and row 64 `M-core-packages` is `scheduled` (`docs/ROADMAP.md:158`). The spec-warden must say which reading it judges.
14. **OpenSSL 4.0.3's `SHA256_Init` is `OSSL_DEPRECATEDIN_3_0`** (`/opt/homebrew/include/openssl/sha.h:74`): a C probe compiles with *'SHA256_Init' is deprecated*, links against `-lcrypto`, and `SHA256_Init` returns 1 (the probe exits 0, 12:32:28). The ffi-pragmatist's `SHA256_CTX` binding is of a deprecated API, and three of Darwin's 42 public census records (`SHA256state_st`, `SHA512state_st`, `SHAstate_st`) are its contexts.

## Negative sentences written as facts

- *"no amd64 image here"* (shared brief, census table; facts.md 5): false, item 6.
- *"`missing_fields`: emitted at `selfhost/check/walk.hero:1986`"* stated as the one site: a second site, `emit/ffi_built.hero:160`, item 4.
- *"Today it says nothing, `check` is 0 and C writes past the record"* (092): run, true (item 9). But the brief frames the shape as `void *` only; see the routes below.
- *"no sentence says C's `char` is `i8`"*, *"No `rest` anywhere"*, *"none under `tests/golden/check/`"*: run, true.
- ffi13's lane brief, *"nobody else touches `selfhost/emit/` or `selfhost/cli/` now"*: false at 12:24, lane ir12's `f7576a01` edits `selfhost/emit/container.hero` and `emit/construct.hero` (lane brief, not the panel's; it matters because R1's emission lives in the file ir12 rewrote).

## CARRIED numbers a seat would lean on

| number | where | state today |
|---|---|---|
| 23 public structs, x86-64 | shared brief, ROADMAP row 63 | re-runnable today (item 6) |
| a zeroed mutex locks on Linux, EINVAL on Darwin | shared brief | **re-run, holds** (item 8) |
| spinlock LOCKED at zero on x86-64, barrier SIGFPE | 178 § What the sitting measured | x86-64 re-runnable |
| padding lost by value on x86-64 under MSan | 178 point 9 | x86-64 and MSan both available (items 6, 7) |
| `+32` real for R1's sentence (8361 base, 2026-09-24) and *+32 ± 2 above the new base*, unrun | 178 § What the sitting measured | the spec-warden re-prices; the base is now 9518 |
| 1280 literal zeros for `utsname` | ROADMAP row 63, 178 | structural: five `char` fields of `_SYS_NAMELEN`, which `sys/utsname.h:72` in the SDK defines as 256 (`grep -n _SYS_NAMELEN`) |
| 905 / 4016 real tokens per `utsname` construction | 178 § The question | a replica of the real instrument, not `--refresh` |
| 5555 lines of C for one `utsname` (route S) | 178 ffi-pragmatist | today's one-field `partial` emits 1,160 (item 5); ir12 changes it |
| 22 of 50 readers wrote `u8` | 178 critic § 6, the author's notes | not measurable by the blind design as written (blind item 2) |
| +435/−27, three DECIDED rows | 178 compiler-engineer | the diff no longer applies (item 12) |

## Routes nobody listed

1. **The `ffi_field_type` note names `i8`, or carries a `certain` Fix `u8[N]` to `i8[N]`, when clang says the field is `char`**: zero spec tokens, aimed at the largest measured first-try failure. Today's note does not name `i8` (blind item 6). It is 178's point 5's second half, unlanded; a Fix's tag is panel 193's ground. It may make the § 13 sentence unnecessary, or not: the blind design cannot tell (blind item 2).
2. **R1 as a relaxation of panel 186's `build` marker** (`layout_sites.OMITTED` / `ffi_built.hero:160` not raised when `rest: zero` ends the construction), the emission already being a designated-initialiser compound literal that C zero-fills (item 5), against 178's `check`-side prototype with `ir/zeros.hero`. Its cost and its union rule (item 4) are the compiler-engineer's to build.
3. **A diagnostic that offers `rest: zero`**: `fixed_array_length` on a one-element literal for a group record's field (exactly `o1-check.txt`'s five errors) offering the form, so the repair turn is served even if the first turn is not. A diagnostic text change, not a class; the ergonomist's step 3 is where it would show.
4. **092 is wider than `void *`**: `connect(fd, @addr: Sockaddr, len: 106)` over a 16-byte `record Sockaddr tag sockaddr` is `check` 0, `run` 0, `--sanitize` 0 (`w/blind2/t2b.hero`). ASan does not see the kernel's read, so the over-read is an inference from `connect`'s contract, unproven here. And the correct idiom, a `sockaddr_un` lent to `const struct sockaddr *`, is refused at `build` (`ffi_parameter_type`), which 178 bypassed with `un_shim.h`. The ffi-pragmatist's task 1 (*`sockaddr_un` (`connect` to a path)*) will hit it: whether a shim is admissible should be said, and *how a binding lends one struct to a parameter of its family's base type* is a question beside 092's.
5. **Wait for batch 12 to close** (the task issue's own words): the routes that ir12 (`f7576a01`) and ffi13 (094) change are then measured once, on the tree that lands.

## Instructions that would make a seat's work unsound or unsafe

- **spec-warden, Principle 0 count**: `git ls-files '*.hero'` in a `git archive` copy fails, *fatal: not a git repository* (run in `194-critic/`). Say: `git -C /Users/joseph/Temp/heroes/heroes-lang ls-tree -r --name-only 44f3b802` (read-only) or `find` in the copy; the brief's *never the repository* otherwise forbids the first.
- **compiler-engineer and ffi-pragmatist, *read its branch (`git log lane-b12-ffi13`)***: needs `git -C <repo>` read-only, same conflict. At 12:19 the branch is at `44f3b802` with no commit of its own and `<scratchpad>/batch12/ffi13/` holds only `base/`, `heroes-base`, `runtime-base`: a seat that writes before the lane reports judges 092's routes and Z3 on nothing. Name the sync point, or let the synthesis rule on 092 alone and say so.
- **spec-warden, eight `--refresh`**: safe as written (`refresh.hero:43` writes only under `build/measure-refresh` of the working directory, prints the record and rewrites no pin; three requests per run, `refresh.hero:166-170` and `:148`); run it from the copy, never the repository. Spend the first on the **unchanged base** as the instrument's control (178's critic re-measured a seat's draft, *padding too*, at 8397 for the same purpose; the pin 9518 is from today, so a base refresh that disagrees with it says the instrument moved), so seven remain for drafts. The R1 draft should be priced **with its union clause** (item 4), which 178's sentence lacks.
- **ffi-pragmatist, the Windows box**: `00-lane-rules.md:108-111` says the box is shared by lanes str192 and cli12, and ffi13's brief adds itself: three lanes on 2 cores. Task 6 needs only `clang -fsyntax-only` per census header over `ssh win`, no tree copied and no compiler built; the brief's *10 GB free before you copy* invites a copy. Say: no copy, no build, one header loop.
- **ffi-pragmatist, x86-64**: tell it the image is `heroes-linux` and to run it **without** `--platform` (item 6), or it reproduces the coordinator's refusal and reports x86-64 unrun.
- **compiler-engineer, instructions retired before and after R1**: fix the cache state (item 11): both runs on a fresh `build/` or both warm, and say which.
- **compiler-engineer, R1 on today's code**: 178's diff fails 14 of 36 hunks (item 12) and R1's group-record path now runs through `check/group_fields.hero` and `emit/layout_sites.hero` (item 4): a rebuild, and its layout rows are not 178's.
- **The blind seat**: items 1 to 6 of its section above, in particular the budget bound, the user settings, and that t2 cannot discriminate K, L and M.

Written 12:34 (`date`). Second pass after the seats.
