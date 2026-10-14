# Panel 215, the completeness critic, pass 1: the briefs

Written 2026-10-14 from 08:18, before any seat ran, in the critic's copy
`.claude/worktrees/scratch-panel-215/critic` at `7e5c02048`, its compiler built from the seed
(`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, real 8.15 s, `heroes 0.4.0`). Every
finding names its command and what it printed. The copy is removed at the end; what it built is
kept in `docs/panel/215-reports/critic-work/` (`taskA/`, `taskB/`, `pad/`), every file there
rebuilt from that folder before the copy went (`a.hero` printed `16`, `b.hero` and `copy.hero`
`3`).

## In short, most consequential first

1. **Q1's route B, measured, does not close issue 1334's case** (§ 4): the padding probe with every
   construction ending `rest: zero` reads the same 3 MemorySanitizer reports at the same line on
   Linux x86-64 at `-O0` as the probe without it; the bytes are lost in the by-value call, which no
   construction-side zero reaches.
2. **Q3 is framed on panel 178, which panel 194 superseded** (§ 1); 194's warden held the
   plan count to be the wrong instrument and registered R1's payment on a reader test that is owed
   and unscored since 2026-10-06; measurement 044 says it does not gate the landing; and 178's grep
   payment reads 19 today, every hit the compiler's own source.
3. **Task A has one binding, `partial`**, so it scores the discovery of `partial` and not R1, K or
   the path loop; it is not panel 194's three-arm test; **Task B's "correct" cannot see a copy**: a
   program that copies the mutex three times prints `3`, exit 0 (§ 3).
4. **The readers' isolation rests on an unmeasured route**: panel 209 measured `--restricted
   --safe-mode` letting the project's git status in from a folder under `.claude/worktrees/`, its
   remedy was `git init` per folder, and `GIT_CEILING_DIRECTORIES` set to the reader's folder itself
   still finds the repository (§ 2).
5. Panel 178's probes do not build today without edits (`@=`, and `copies.hero` a `counted_by`
   rewrite), the brief's examples command prints 0 under zsh (21 quoted), the census reads 905 not
   878, and `a975e3d6a` is K0, not R1 (§ 5, § 6). Routes missing from each question and the
   premises that steer each seat are §§ 1, 4 and 7.

## 1. Q3 is framed on a superseded sitting, and the sitting that ratified R1 is not named

The shared brief's Q3 reads panel 178's spec-warden condition (*R1 turns to object if every
long-array construction stands in two bindings or fewer*) as the live question, and offers routes
A (stands), B (withdrawn), C (shortened). What the records say, each read in the copy:

- **Panel 178 is SUPERSEDED.** `sed -n 255,286p docs/panel/178-the-rest-is-zero-*.md` prints
  *"SUPERSEDED 2026-10-06 ... So it is sat again on the trunk of that day as panel 194, and its
  `panel 178` item is closed as superseded"*. R1 was ratified by **panel 194**, not 178:
  `git show 54c5f6187` opens *"Panel 194 R2, ratified by delegation 2026-10-06: the spec-warden's
  drafts, applied verbatim to § 13"*. `00-shared.md` never names panel 194 (`grep -c 194
  00-shared.md` reads 0 in the brief's own text, beside the census line that cites panel 214).
- **Panel 194's spec-warden held the plan count to be the wrong instrument.**
  `docs/panel/194-reports/spec-warden.md:219-223`: *"178's lapse condition (fewer than 3 bindings)
  reads 0, but 178's own warden withdrew the corpus count as the wrong instrument ... and I hold
  that reading: when one construction is 905 to 4016 tokens, absence does not measure need."* It
  re-registered R1's payment on two other instruments (`:268-277`): **the reader test at
  M-buildable-structs' landing** and 178's grep at M-core-packages' close.
- **Panel 194's warden wrote R1's live turn-to-object condition, and it is not the plan count.**
  `docs/panel/194-reports/spec-warden.md:240`: *"to object if that test reads `u8` >= 3 of 10 under
  R1+K, or if any leg shows a union none named, or a field sharing no byte with a named one,
  nonzero after `rest: zero` (MSan on the Linux legs)"*, and `:260`: *nothing* *"becomes my verdict
  if the landing's reader test fails R1+K's prediction"*.
- **Measurement 044 says the same of itself.** Its § opening: *"The sentence landed on 2026-10-06
  (`54c5f618`), paid instead by panel 194's registered predictions, so this answers a question the
  payment still rests on, not one that gates the landing."* The brief quotes 044's count and drops
  this sentence.

So the brief hands every seat a premise (178's condition is the live trigger) that the ratifying
sitting and the measurement itself both disown. **The honest Q3 is: does R1 meet panel 194's
conditions?**, and its instruments are the experiment's Task A (if it is designed to score 194's
test, § 3) and MSan on the union shapes (the milestone page's reading of 2026-10-14, arm64 only;
the Windows box unrun). **Routes missing from Q3**: **D** R1 stands and 178's condition is
recorded void as a superseded sitting's; **E** R1 is judged on 194's registered conditions (the
reader test, the MSan legs), which this sitting can score; **F** R1 stands and its payment is
re-registered on a working instrument (below). And route B, *withdrawn with its sentence*, is
underspecified: R1's text stands at **two** lines, not one (`grep -n "rest: zero"
spec/heroes-spec.md` prints 383 and **389**, the union clause *"or none when it ends with `rest:
zero`"*), and K (`i8` where C says char, line 357) landed in the same commit because 194's warden
approved R1 *only composed with K* (`:240`); a withdrawal must say what happens to line 389 and
to K.

**And 178's registered payment, as written, is already met vacuously.** Its instrument is
*`rest: zero` in 3 or more `.hero` files outside `tests/` and `docs/`*
(`docs/panel/178-reports/spec-warden.md:35`). `git grep -l 'rest: zero' 7e5c02048 -- '*.hero'
':!tests' ':!docs' | wc -l` reads **19**, every one under `selfhost/` (the compiler's own comments
and messages: `ast.hero`, `check/rest_zero.hero`, `emit/zeroed.hero`, ...); with `':!selfhost'
':!archive'` added it reads **0**. A seat that runs the grep as registered will read the payment
met; the programs that construct with it number zero (`examples/` 0 by the same grep).

## 2. The experiment's isolation: `GIT_CEILING_DIRECTORIES` is unmeasured, and its value decides it

The ergonomist's brief puts the readers' folders under `.claude/worktrees/blind-215/`, inside the
repository, and relies on `--safe-mode`, `--restricted` and `GIT_CEILING_DIRECTORIES`. What each
flag's own words say (`claude --help`, Claude Code 2.1.287, read; no session started):

- `--safe-mode`: *"Start with all customizations (CLAUDE.md, skills, installed plugins, hooks, MCP
  servers, custom commands and agents, output styles, workflows, custom themes, keybindings, and
  more) disabled"*. It names CLAUDE.md. **It does not name auto-memory or git status.**
- `--restricted`: removes code-running tools and WebFetch, *"ignores user, project and local
  settings files"*, *"confines the file tools to the working directories"*. **It names neither
  CLAUDE.md, memory nor git status.**
- `--bare` is the one flag whose words name *"auto-memory"* and *"CLAUDE.md auto-discovery"*; the
  brief does not use it (it also changes authentication to `ANTHROPIC_API_KEY` only, which the
  coordinator would have to weigh).

**Measured precedent: those two flags do NOT keep git status out of an in-tree folder.** Panel
209 ran readers in `.claude/worktrees/scratch-b15/209-blind/` with `--restricted --safe-mode
--strict-mcp-config`; `docs/panel/209-reports/llm-ergonomist-t1-A.md:74`, the reader's own
`context`: *"The harness also placed into my context, unrequested, a git status snapshot (branch
`main`, recent commit titles mentioning panels and defects, untracked `docs/panel/209-briefs/` and
`docs/panel/209-reports/`)"*. The coordinator then made each folder **its own git repository**
(`git init`, one commit of the inputs; `llm-ergonomist-t3-A-c1.md:3`), and the re-run's `context`
lists only the folder's own `run.err` and `run.json` (`llm-ergonomist-t1-A-c1.md:68`). That is the
measured route; `GIT_CEILING_DIRECTORIES` is a new one, unmeasured with `claude`.

**Its value decides whether it works at all**, measured with `git` alone on a folder of the same
shape (`.claude/worktrees/scratch-panel-215/blindprobe/r01`, ignored by `.gitignore:35`):

| `GIT_CEILING_DIRECTORIES` | `git -C <folder> rev-parse --show-toplevel` |
|---|---|
| unset | `/Users/joseph/Temp/heroes/heroes-lang` |
| **the reader's folder itself** | **`/Users/joseph/Temp/heroes/heroes-lang`**: the repository is found |
| the folder's parent | `fatal: not a git repository` |
| the repository's root | `fatal: not a git repository` |

Git's rule is that the ceiling is a directory it will not chdir up INTO, and the current directory
is never excluded. The brief names no value. And whether Claude Code computes its git status by
running `git` with the inherited environment, rather than by its own walk for `.git`, is
**unrun** (it would take a session). **The route missing**: 209's `git init` of each folder, which
was measured; or both, and a pre-flight `git -C <folder> rev-parse --show-toplevel` that must
print `not a git repository` (or the folder itself, under `git init`) before any paid launch.

**Two further channels the brief does not close**, both from 209's `context` answers: the
environment section carries **the working directory's path**, which here reads
`.../heroes-lang/.claude/worktrees/blind-215/...` (the project's name and the sitting's number,
which the brief says `brief.md` must not name), and the account's email. **Auto-memory**: earlier
blind sessions were keyed by their own folder (`ls ~/.claude/projects | grep blind` lists
`-Users-joseph-Temp-heroes-heroes-lang--claude-worktrees-scratch-b15-209-blind-t1-A` and 23 more,
none with a `memory/` folder), so the project's `MEMORY.md` was not loaded there; whether that
holds when the git root is found is unrun. The panel skill still says the folder lives *"under the
scratchpad, outside the repository and outside any git tree, with no `CLAUDE.md` in or above it"*
(`.claude/skills/panel/SKILL.md:153-155`), which CLAUDE.md § Hard stops (2026-10-08) now forbids;
the repository's `CLAUDE.md` IS above `.claude/worktrees/blind-215/`, and only `--safe-mode` keeps
it out. The brief should keep the skill's requirement that each reader answer `context`, and void
any reading that names a project rule, as 209 did.

## 3. Task A is solvable, but only through `partial`, so it does not score what it is registered for

**Solvable: yes.** `critic-work/taskA/a.hero` (§ 9) builds and prints `16`, exit 0, on this Mac, against
the critic's own stand-in `sockpath.h` (the experiment's is not written yet: `ls
.claude/worktrees/blind-215` reads *No such file or directory*). The expected `16` is right:
`/tmp/heroes.sock` is 16 bytes.

**But the struct admits exactly one binding, `partial`**, because `via` is a NAMED member of an
UNTAGGED union type, which no Heroes field type can name. Measured, each with this copy's compiler:

- `fd: i32` and `handle: ptr` declared on the record (a reader of § 13's *"A field in a C union,
  named or anonymous ... a record names one or more of each union"*): `error[ffi_unknown_field]:
  sockpath.h declares no fd in struct sock_path`, exit 1 (`v1.hero`);
- the union left out without `partial`: `error[ffi_incomplete_record]: SockPath does not name via`,
  exit 1, and its note does not name `partial` (`v2.hero`);
- the union bound as its own record `record Via` and `via: Via`: `error[ffi_unknown_name]:
  sockpath.h declares no Via`, exit 1 (`v4.hero`);
- `record SockPath tag sock_path partial` with `family: u16` and `path: i8[108]`: builds (`a.hero`
  with `rest: zero` and an element loop; `v3.hero` with a 108-element literal and no `rest: zero`,
  also `16`, exit 0).

So Task A's first-try rate is dominated by whether a reader finds `partial` for a union shape the
spec's union sentence seems to cover. **Panel 194's registered test** (`docs/panel/194-the-rest-*.md:179`:
*"at least 10 fresh readers per arm: L, R1 honest, R1 honest with K; one task holding a union;
every program compiled ... at least 8 of 10 correct first try and at most 2 of 10 writing `u8`
under R1 with K"*) used an anonymous union (`SA`) and a typedef'd union type (`UD`) in its own
cases (`tests/golden/run/ffi-rest-zero.h`); 194's warden named *"178's Darwin `utsname` task"* as
the task (`spec-warden.md:240`). Task A is neither: a failure there cannot be attributed to R1, to
K, to the path loop (issue 1333's T condition) or to `partial`. **Scored by one arm of today's
spec it is not 194's test**: it has no L arm and no *R1 honest* arm, so the comparison 194
registered is unscorable; only the two absolute thresholds of the R1-with-K arm are, and today's
spec is not *"R1 honest with K"* alone (panels 195 to 214 landed text since). Say so in the
report, or add the arms. **That test is owed and unscored since the landing of 2026-10-06**:
step 6's body (`git show ba46e79f5`) reads *"1333 to 1335 waiting on a paid reader test or a
sitting"*, and the ledger's row 7467 (`docs/measurements/010-spec-budget-ledger.md:215`) counts
194's token prediction as scored and names no reader-test score for it. This sitting is its first
chance, so a design that cannot score it should say so rather than be read as scoring it. **And issue 1333's T condition** (*"writing a path fails first try at
least half the time"*) is confounded the same way: a reader who never reaches the path because of
`partial` fails *first try* for another reason. Score *path written correctly* only among programs
that bound the struct.

**The scoring rules are not well defined in four places:**

1. *"whether the union was handled"*: under today's spec the union cannot be declared at all, so
   "handled" can only mean `partial` and `via` left zero; and `via` is not zero by C's promise
   unless the construction ends with `rest: zero` (`v3.hero`'s designated initialiser zeroes C's
   first member `fd` and leaves `handle`'s other four bytes C-unspecified, `zeroed.hero:6-12`'s own
   reading of C11 §6.7.9p10). The run cannot tell: `sp_send` returns the path's length, which no
   value of `via` changes. If *leave via zero* is to be scored, make `sp_send` read `via` (return
   -1 where `via.handle != NULL`), so the expected line tests it.
2. *"the path C reads"*: `strlen` or `strnlen(p->path, 108)` differ on an unterminated field
   (the T condition's whole question). The header's body must be fixed and shown.
3. **The helpers must be defined, not declared.** Panel 194's blind seat failed for this:
   *"task 2's shim declared `connect_un` without defining it, so every task-2 program stopped at
   `ffi_missing_link`"* (`docs/panel/194-the-rest-*.md:166-168`). The brief does not say
   `sp_send` and the four `counter_` functions are `static inline` (the critic's stand-ins are).
4. **Task B's "first try correct" cannot see the copy it is about**: `critic-work/taskB/copy.hero`, which
   passes the counter by value and returns it three times (the mutex copied on every call), builds
   and prints `3`, exit 0, exactly as the correct `b.hero` does. Correct for B must be *prints 3 AND
   no copy of the record after `counter_init`*, with "copy" listed (a by-value parameter, a return,
   `=`/`@=`/`@` from an existing value, a field of a Heroes record or an element of an array that
   grows). A Heroes `@` parameter is NOT a copy: the emitted C passes the address
   (`b.c:171: void h_b_bump(struct counter *ph0_c)`, `b.c:212: h_b_bump(&h0_c)`), so the scorer
   must not count it as one.

**And `long n` is not "the same on every platform"**: on the Windows box (LLP64) C's `long` is 32
bits and `n: i64` is refused there, and `pthread.h` is not a Windows header; if the programs are
built only on this Mac, say so in the report.

## 4. Q1's route B rests on a premise the probe's own reading contradicts

`00-shared.md` gives route B as *"every construction of a group record zeroes the whole object
first, as `rest: zero` already does (`selfhost/emit/zeroed.hero`)"*. **True of the construction**:
`zeroed.hero:20-24` emits `t3 = (SA){0}; __builtin_memset(&t3, 0, sizeof t3);` then the stores,
and the critic's rest-zero variant of the probe emits 3 `memset`s where the original emits 0
(`grep -c memset small.c small_rz.c`: 0 and 3). **But the same file says the zero does not
survive**: `zeroed.hero:14-16`, *"§6.2.6.1p6 and p7 leave padding and a union's other members
unspecified, bytes § 13 does not promise"*, i.e. a store into a member may rewrite padding. And
the milestone page's re-run of 2026-10-14 puts all 3 reports at **the lend after `cd_through`**,
none at the construction, so the construction already reads clean and the bytes are lost in the
by-value call or return. Whether B closes the measured case is therefore a **question, not a
premise**, and the critic ran it.

**Measured: B's mechanism does not close the measured case.** The probe `small.hero` (edited to
`@=`, § 5) and a copy whose three constructions end `, rest: zero` (`small_rz.hero`, 3 `memset`s in
its C), each built in the container by a compiler made there from this tree's seed (`clang -O1`),
then `clang -std=gnu11 -g -fno-strict-aliasing -fno-delete-null-pointer-checks -fsigned-char
-D_GNU_SOURCE -O<n> -fsanitize=memory -fsanitize-recover=memory`, run with
`MSAN_OPTIONS=halt_on_error=0` (script `critic-work/pad/msan215.sh`, panel 178's `msan.sh` with the
variant added, run as `docker run --rm -v "$PWD":/src:ro <image> bash /src/build/pad/msan215.sh`
from the copy; its two outputs are `critic-work/pad/msan-x86.txt` and `msan-arm64.txt`):

| leg | program | -O0 | -O1 | -O2 | -O3 |
|---|---|---|---|---|---|
| `heroes-linux` (x86_64, emulated), Debian clang 22.1.8 | `small.hero` | **3**, at `small.hero:29` | 0 | 0 | 0 |
| the same | `small_rz.hero` (`rest: zero`) | **3**, at `small_rz.hero:29` | 0 | 0 | 0 |
| `heroes-linux-arm64`, Debian clang 22.1.8 | `small.hero` | 0 | 0 | 0 | 0 |
| the same | `small_rz.hero` | 0 | 0 | 0 | 0 |

Line 29 is `n2 = w_cd(@b)`, the lend of `b: Cd @= cd_through(x: a)`. So the milestone page's
reading of 2026-10-14 reproduces (3 at `-O0`, none above, x86-64 only), and **zeroing every byte at
construction leaves the same 3 reports at the same line**: the bytes are lost in the by-value
call or return, which B does not touch. A seat that adopts B as closing issue 1334 is refuted by
this table; B can only be adopted together with a route that changes how a record crosses a call.
No positive control was run beside it (178's `ctl.h` was not), so the `-O1` to `-O3` zeros are a
reading of this build, not a proof that nothing is lost there.

**Routes missing from Q1**: **F** the copy, not the construction: every by-value parameter and
return of a group record is copied by `memcpy` from a zeroed callee-side slot (the remedy issue
1334's own body names, *"by address or by `memcpy`"*, and which C and D only half cover); **G**
zero at the boundary only: a group record lent to C (`@`, `.ptr()`) is re-zeroed in its padding
before the call, which costs nothing on records that never reach C; **H** a toolchain answer, not
a language rule: the padding question owned by `.claude/rules/generated-c.md` (where *"eq or hash
walks fields, never bytes"* already lives, line 56) with no spec sentence, which the spec-warden's
brief asks about but no route offers. And route C (*"crosses a call by address"*) needs the shape
next to it: a Heroes function with an `@` parameter already crosses by address (`b.c:171`), so C is
"every by-value record parameter lowered as `@` with copy-on-write", and its interaction with
recursion and with a record held in a `[T]` is unpriced.

## 5. Panel 178's probe does not build with this compiler as written

`./heroes build small.hero` on `docs/panel/178-reports/completeness-critic-work/w/pad/small.hero`
prints **6 errors**, each `fix (certain): write @=` (panel 209's declaration form, landed since
2026-09-25). The compiler-engineer's brief says *"run panel 178's critic's padding probe"*; it
must be edited first (`a: Cd @ ...` to `a: Cd @= ...`). Beside it, `./heroes check` prints 2
`old_cell_symbol` errors for `union_arm.hero` and 7 for `copies.hero`; and `copies.hero`, once
edited to `@=`, meets two more refusals that landed after panel 178: `never_rebound` on `x` and
`list` (2), and then **`ffi_parameter_type`**: *"`buf` of `write` lends a whole record to C, and
the header takes it as `const void *` ... how far it reaches is another argument this declaration
ties to nothing"*, panel 194's route C for defect 092. So the copies probe needs its `write`
binding rewritten with `counted_by`, which changes what C is handed. The milestone page's *"with
the critic's own probe and script"* must therefore have run an edited copy, which it does not say.

## 6. Framing facts verified, and the ones that are not

- **R1's commits**: `git merge-base --is-ancestor <c> main`, `origin/main` and `7e5c02048` succeed
  for `838d27198`, `a975e3d6a` and `54c5f6187`, all dated 2026-10-06. **But `a975e3d6a` is not
  R1**: its subject is *"M-buildable-structs step 2: a u8 field clang refuses is told C's char is
  i8"*, K0 (the note), 4 files under `selfhost/emit/ffi_field.hero` and a golden. R1 is `838d27198`
  (the form) and `54c5f6187` (the sentences, with K).
- **Line 383**: `grep -n "rest: zero" spec/heroes-spec.md` prints 383 and 389; the section above
  383 is `## 13. FFI` (line 348). Verified, with the second site above.
- **The specification promises nothing of padding**: `grep -n -i padding spec/heroes-spec.md`
  prints nothing. Verified as a grep, which is the vocabulary of one word.
- **`==` and `hash` walk fields**: `.claude/rules/generated-c.md:56`, *"A generated `eq` or `hash`
  walks fields, never bytes"*. Verified.
- **The spec's numbers**: `./heroes measure spec/heroes-spec.md` prints legacy 7519, cl100k 7655,
  real 10048 (claude-opus-5, 2026-10-13), headroom 192, the FFI floor 60. Verified; 132 is
  192 - 60.
- **The census of programs with an `extern`**: the brief's command
  `git grep -l -E '^[[:space:]]*extern ' 7e5c02048 -- '*.hero' ':!docs' | wc -l` reads **905** on
  this tree, not 878 (866 of them under `tests/golden`, 7 under `examples/`... by `awk -F/` on the
  paths). The brief asks the seats to re-run it; its own figure is panel 214's tree's.
- **The examples' population: the brief's command prints 0 in this shell.** `grep -rl "^extern"
  examples --include=*.hero | wc -l` under zsh (this Mac's shell) prints *"no matches found:
  --include=*.hero"* and **0**, because zsh expands the unquoted glob first. Quoted,
  `--include='*.hero'`, it reads **21** files, holding 10 `record` lines inside an `extern` group
  (1 of them `partial`; handles not separated). A seat that pastes the brief's command reports an
  empty population.
- **The milestone page's quotes**: the brief's Q1 re-run (3 reports at `-O0`, all after
  `cd_through`, none at `-O1` to `-O3`) and Q2's mutex measurement (*trylock* 16, two locks, exit
  0) match `docs/roadmap/milestones/M-buildable-structs.md:83-96` word for word; the page says
  *"on both legs"* where the brief says *"on this Mac and on glibc"*, which agree. Issue 1334's
  body (read, `gh issue view 1334`) says reports *"at -O0 to -O3"* on 2026-09-25 and *"the same
  with today's `partial` construction"*; the drop from four levels to one on 2026-10-14 is
  unexplained in both (a clang version, the emitted C, or the edited probe: a question).
- **Measurement 044's quotes**: `sockaddr` (`net`), `SHA256_CTX` (low-level route), `fd_set`
  (only through `select`), *"one binding, or two"*: match 044 § 3. Not re-run (its instrument
  needs the Homebrew OpenSSL and sqlite headers; unrun by the critic).
- **Issue 1335's body**: *"The emitted C passes the cell's address (`pthread_mutex_lock(&h0_m)`)
  ... but a value copy of a locked mutex is **unrun**"*. The brief's quote is right, and the
  page's 2026-10-14 run is what made it run.

## 7. Premises that steer a seat

- **The spec-warden** is told to price Q3 from *"panel 178's own condition, read in its file:
  what turn to object commits the sitting to, now that measurement 044 holds it"*. The verb
  *commits* presumes the condition binds; § 1 says the ratifying sitting replaced it. The warden
  should be pointed at `docs/panel/194-reports/spec-warden.md:240` and `:260` as well.
- **The compiler-engineer** is told to *"build the route you adopt far enough that the probe reads
  clean"*: a probe that reads clean at `-O0` only after the route may have read clean for another
  reason (the drop from four levels to one, § 6); ask for the positive control panel 178's critic
  used (`ctl.h`, `ctl_write`, in the same folder) on every level, or a clean reading proves nothing.
- **The ffi-pragmatist** is asked *"Can route C decide from the header alone, and how often would
  it be wrong?"* for Q2's header-decided mark, and given a list of C types; the missing shape is a
  struct that HOLDS such a type by value (Task B's `struct counter`), which is the case a program
  meets, and a `partial` record that leaves the mutex field undeclared (the critic's `b.hero`), for
  which a mark decided from the Heroes record's own fields sees nothing to refuse.
- **Q2's routes omit the shape the experiment's readers write**: the copy happens through a
  `partial` record whose mutex field the program never declares. Route B (a mark the binding
  writes) catches it only if the binding author writes the mark; route C (from the header) must
  read the C struct, not the Heroes record. Neither route says which, and the 878/905 census cannot
  answer it, since it counts files with an `extern`, not records holding a non-copyable type.
- **The historian** is asked about Rust's `!Copy` and Swift's SE-0390 for Q2; the brief does not
  ask about the precedent closest to route C, C11 `_Atomic` and the `<stdatomic.h>` rule that an
  atomic object must not be copied by `memcpy`, nor about clang's own `__attribute__((...))` set
  (none marks a type non-copyable in C, which would decide route C's input), so the route can be
  favoured or refused on evidence the seat was not asked for.

## 8. Not run by the critic

Measurement 044's instrument (headers), issue 1335's mutex copy on Linux (the page's run stands
unrechecked), the Windows box, any `claude -p` session (forbidden), whether Claude Code honours
`GIT_CEILING_DIRECTORIES` (it would take a session), and a positive control beside the MSan run
(§ 4). The emitted C cited as `b.c` and `small.c` was in the copy and went with it; `heroes build
<file> --emit-c -o <file>.c` on the kept programs remakes it. One slip of the critic's own: a
canonical check wrote one temporary file under `/tmp` (the output of `heroes fmt`), removed by the
same command; nothing else was written outside the repository's root.

## 9. The critic's two programs

`docs/panel/215-reports/critic-work/taskA/a.hero`, against the critic's stand-in `sockpath.h` beside it
(`struct sock_path` as the brief writes it; `static inline int sp_send(const struct sock_path *p)
{ return (int)strnlen(p->path, sizeof p->path); }`), builds and prints `16`, exit 0:

```
extern "sockpath.h"
    record SockPath tag sock_path partial
        family: u16
        path: i8[108]
    function sp_send(@p: SockPath) -> i32

function make(path: str) -> SockPath?
    p: SockPath @= SockPath(family: 1, rest: zero)

    for i in range(from: 0, to: path.len())
        p.path[i] @ path[i].to_i8()?

    return ok(p)

function main()
    p: SockPath @= make(path: "/tmp/heroes.sock").must()
    print(sp_send(@p))
```

`docs/panel/215-reports/critic-work/taskB/b.hero`, against the critic's stand-in `counter.h` beside it
(`struct counter { pthread_mutex_t lock; long n; }`, the four functions `static inline`, each
taking `struct counter *` and `counter_init` setting `n` to 0 and calling `pthread_mutex_init`),
builds and prints `3`, exit 0, with no copy of the record after `counter_init`:

```
extern "counter.h"
    record Counter tag counter partial
        n: i64
    function counter_init(@c: Counter) -> i32
    function counter_lock(@c: Counter) -> i32
    function counter_unlock(@c: Counter) -> i32
    function counter_value(@c: Counter) -> i64

function bump(@c: Counter)
    _ = counter_lock(@c)
    c.n @ c.n + 1
    _ = counter_unlock(@c)

function main()
    c: Counter @= Counter(rest: zero)
    _ = counter_init(@c)
    bump(@c)
    bump(@c)
    bump(@c)
    print(counter_value(@c))
```
