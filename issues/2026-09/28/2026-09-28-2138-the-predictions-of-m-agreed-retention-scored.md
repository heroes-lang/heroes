---
kind: task
area: records
milestone: M-agreed-retention
filed: 2026-09-28
commit: c7b03a12c432bdea708de4f2d9526719d6cbb2ce
github: none
---

# The predictions of M-agreed-retention, scored

For the M-agreed-retention close: panels 174 to 177 and the four ledger rows
they spent (6282, 6344, 6643, 6693), scored on 2026-09-26 over the trunk at
`bdf430f1`; panels 179 to 182 and the two ledger rows they spent (6794,
6838), scored on 2026-09-28 over the trunk at `a63cf9ed`. Panel 178 sat on
2026-09-25 on a peer session's branch, `lane-panel-178`, for M-buildable-structs,
and is not on the trunk or this milestone's.

- [x] **M-agreed-retention** | every prediction the milestone's eight sittings and its six ledger rows registered, scored, voided or lapsed with its reason, and the ones whose horizon is a later milestone carried unchanged | **closed 2026-09-28**, at the milestone's close | Panels 174 to 177 and 179 to 182, `docs/measurements/010-spec-budget-ledger.md` rows 6282 to 6838, `/step` § Close: *score every prediction whose milestone this is, and lapse the ones you cannot* | 174-182

    **Origin:** the close checklist, and panel 046 R2's rule: a prediction is
    scored, or it is marked `lapsed` with its reason, and **never renewed with
    a new milestone name**. A prediction whose horizon is a later milestone
    (M-thesis-harness, or *the first milestone whose `examples/` gains one*)
    is listed as carried, with that horizon, and not scored.

    **Where the list came from.** The *Predictions to score* table of each of
    `docs/panel/174-*.md` to `177-*.md`; the `prediction` paragraph of every
    file under `docs/panel/174-reports/` to `177-reports/` (grepped for
    `predict`, `scored at`, `checkable at`, then read); rows 6282, 6344, 6643
    and 6693 of `docs/measurements/010-spec-budget-ledger.md`; the milestone
    file, whose item list is empty (`**OPEN: 0**`) and registers none; and
    `grep -rn "M-agreed-retention\|the landing\|at the landing"` over those
    files. A grep of `docs/records/done/` for entries naming both the
    milestone and a prediction found three (defects 075, 079, 084), and each
    cites a prediction already listed here. A report's prediction the
    synthesis did not copy into its table is listed too, because the report
    is where it was registered.

    **Panel 174 belongs here.** It sat on 2026-09-21 after the
    `m-declared-extents` tag (`316d69c4`), and its landing (`eca8d5d1`,
    `c27deda8`, 2026-09-22) is the first work on the trunk after that tag,
    so *the close of the milestone landing B* is this close.

    **How the numbers were taken.** `$S` is
    `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/e7e3d0df-8db3-44f2-b628-af9bb1a6080e/scratchpad/score-ar`.
    Each compiler was built from its own commit's seed, `clang -O2 -I runtime
    seed/heroes.c runtime/runtime.c`, in a `git archive` copy under
    `$S/t/<sha>` (trunk: `$S/tree`, `$S/heroes`), with `HEROES_RUNTIME`
    pointing at that copy's runtime. Darwin arm64 is this Mac, with Homebrew's
    json-c 0.19, openssl@3 3.6.4 and sqlite 3.53.4. Linux x86-64 and Linux
    arm64 are the `heroes-linux` and `heroes-linux-arm64` images, the
    compiler built from the seed inside the container, OpenSSL and SQLite from
    the image, **json-c 0.18** (Debian's `libjson-c-dev`, installed in a
    throwaway container, not 0.19). Windows x86-64 is the box, reached over
    `ssh win`; a `find` over `C:` found no `sqlite3.h`, no json-c and no
    OpenSSL header there, so every Windows clause over a real library is
    unrun. A seat's program was adapted only to the landed spelling, and the
    adaptation is named in its row. "×5" is five runs of one binary, each
    `exit/stderr bytes`. The runs and their logs are under `$S/w*`,
    `$S/ci`, `$S/p174` and `$S/*.txt`.

    **Already scored by the coordinator on 2026-09-26, taken as given and
    not re-run**: panel 175's three spec predictions (scored at panel 176),
    panel 177's spec-warden P2 and P3, and ledger row 6643's own prediction.
    Their evidence is cited in their rows.

    ## Panel 174: the two files the whole net shares

    | seat | prediction | verdict | how it was scored |
    |---|---|---|---|
    | compiler-engineer | on the close's pre-push Windows CI leg and the two after it, 0 lines carrying `the operating system's own reason is 32`, and `harness:` reads `0 failed` | **NOT YET SCORABLE: the legs it names do not exist**, because nothing has been pushed since `d02b8bf4` (2026-09-22). The close's push writes this verdict. What exists: the reason-32 half has held on every Windows leg since B landed, 3 of 3 | `gh run view <run> --log --job <job> \| grep -ac "operating system's own reason is 32"`. Control, run 35651518558 (`497a048f`) job 106504808513: **120**, harness 1788 passed, 130 failed. After B: 35672606232 (`eca8d5d1`) job 106572200896: **0**, harness 1902 / **1 failed**, and that failure is `layout` (`selfhost/cli/toolchain.hero` at 317 lines), not a spawn. After C: 35693306661 (`c27deda8`) job 106634683153: **0**, 1903 / 0; 35699444097 (`d02b8bf4`) job 106653710998: **0**, 1903 / 0. Logs in `$S/ci/` |
    | compiler-engineer | the orphan probe prints `gone` after route C on Darwin and Linux, where it prints `STILL ALIVE` at `497a048f` | **HELD** on Darwin arm64, Linux x86-64 and Linux arm64, in both shapes | The report's own probe is gone: its scratchpad's `probe/` holds only `build/` and `grandchild.pid`. Scored with the coordinator's `orphan.c` (session `e64edfa2`'s scratchpad, the probe `c27deda8`'s body quotes: the child exits normally, through the real `hero_run_go`), and with `$S/p174/orphan-wd.c`, the same probe with a 2 s limit and a child that hangs, which is the report's description of its own probe. Each built against `497a048f`'s runtime and the trunk's. Heartbeat before → after one second, `497a048f` vs trunk. Darwin: −1→15 STILL ALIVE vs 1→1 gone; watchdog 36→51 STILL ALIVE vs 35→35 gone. Linux x86-64: −1→14 vs −1→−1; watchdog 30→44 vs 30→30. Linux arm64: −1→19 vs 1→1; watchdog 40→58 vs 39→39 |
    | compiler-engineer | `wc -l runtime/parts/run.c` ≥ 640 | **HELD** | `git show <sha>:runtime/parts/run.c \| wc -l`: 572 at `497a048f`, 601 at `eca8d5d1` (B), **817** at `c27deda8` (C), 822 at `bdf430f1` |
    | ffi-pragmatist | with C and the `FILE_SHARE_DELETE` bit alone, **the redirect paths left shared**, a Windows leg reads 0 lines carrying `reason is 32` | **VOID**: it was written against a configuration never built. B (one path per spawn) landed at `eca8d5d1`, 2 h 38 min before C at `c27deda8`, so no leg ever ran C and the DELETE bit over shared paths. The sitting's table dropped that condition. In its wording (the first Windows leg after the landing reads 0) the leg does read 0, but B is in that build, so it cannot say what C and the bit bought | Job 106634683153 above, 0 lines |
    | ffi-pragmatist | route A as the brief states it (all three share bits, no B, no C) lets another case's bytes into a `run/` capture | **VOID**: route A was never built. The sitting vetoed `FILE_SHARE_WRITE` | none possible |
    | ffi-pragmatist | the `waitid(WNOWAIT)` sweep costs nothing measurable: the net's wall time on Darwin moves by less than its own run-to-run spread | **HELD**, on CI's Darwin legs and not on this Mac | The step timings of CI's own job API (`gh api …/actions/runs/<run>/jobs`, step *The net, in Heroes*, `completed_at − started_at`). Darwin arm64 without the sweep: 1090 s (`bec2de33`), 1316 s (`316d69c4`), 1191 s (`497a048f`), 1316 s (`eca8d5d1`), so the spread is 226 s. With it: 1286 s (`c27deda8`), 1277 s (`d02b8bf4`). `eca8d5d1` → `c27deda8` is **−30 s**. Timing the net on this Mac was not run: it needs the machine still for about an hour, and other sessions share it |
    | ffi-pragmatist | `git diff --stat` on the landing names no file under `selfhost/emit/` or `examples/` | **HELD** | `git diff --stat <sha>^ <sha>` for `eca8d5d1` (`runtime/hero_os.h`, `runtime/parts/run.c`, `seed/heroes.c`, `selfhost/cli/process.hero`, `selfhost/cli/toolchain.hero`, two harness files) and `c27deda8` (`runtime/parts/run.c`, the seed, six `selfhost/cli/` files). Neither names `selfhost/emit/` or `examples/` |

    ## Panel 175: a consuming call is three things

    | seat | prediction | verdict | how it was scored |
    |---|---|---|---|
    | compiler-engineer | at A's landing, `xacquires/main.hero` checks 0 and runs exit 0 with 0 B of stderr, five of five, while `popen_darwin`, `oneacq` and `outacq` exit 134 with a line naming both functions | **HELD** at the landing, step 10 (`5ebbe8ba`) | `$S/run5.sh` with step 10's compiler over copies in `$S/w175/`: `xacquires` check 0, **0/0B ×5**. `popen_darwin` 134/231B ×5 (*given back to `fclose` … marked `acquires pclose`*); `oneacq` and `outacq` 134/236B ×5 (*`h_close2` … `acquires h_close`*). On the trunk `xacquires` is `check` 1 `contract_differs`, as the seat's own panel 176 report expected once step 12 landed |
    | compiler-engineer | the landing commit's `git diff --numstat -- selfhost/` shows at most 30 added non-test lines | **FALSIFIED** | `git diff --numstat f7a2161a 5ebbe8ba -- selfhost`: 303 added, 143 deleted. Outside `test` blocks (`$S/codelines.py`, which uses panel 176's compiler-engineer's own test-block rule): **264 added**, 135 deleted, net +129 non-blank; net +77 non-comment. The prediction priced A with one name. The landing carried the set, whose parser is the new 155-line `parse/marks.hero`, and 122 lines left `parse/members.hero` |
    | compiler-engineer | the same commit's `emission` run before blessing reads exactly 227 failed | **FALSIFIED: 240**. What the number stood for held: every blessed emission moved, 237 of 237 | A copy of `5ebbe8ba` with `tests/emission/` restored from `f7a2161a`, step 10's compiler, `heroes run tests/harness/main.hero -- ./heroes emission`: **274 passed, 240 failed**. That is 237 *no longer emits* (the ABI 22→23 assert line) plus 3 *nothing blessed* (the step's three new goldens). There were 227 blessed files at the sitting's `64c92654` and 237 at `f7a2161a`. Log: `$S/s10pre-emission.log` |
    | ffi-pragmatist | with a one-name mark `sqlite_v2.hero` exits 134 on Darwin arm64, Linux arm64 and Linux x86-64; with a mark naming both it exits 0 there, and on Windows | **HELD on the three POSIX legs; the Windows clause LAPSED**: there is no `sqlite3.h` on the box | The seat's `work/sqlite_v2.hero` as filed, and with `acquires sqlite3_close \| sqlite3_close_v2`. Step 10's compiler, Darwin: 134/258B ×5 and 0/0B ×5 (`open 0 close_v2 0`); trunk the same. Linux x86-64 and arm64 at step 10 (`$S/linleg.sh`), plain and `--sanitize`: 134 (269 B, *given back to `sqlite3_close_v2` … `acquires sqlite3_close`*) and 0, 0 B |
    | spec-warden | route A takes `popen_darwin`, `oneacq`, `outacq` and `xacquires` from 0 to 134 | **FALSIFIED**, scored at the sitting by the critic (`xacquires` 0, five of five), and confirmed at the landing | the first row above: `xacquires` 0 at step 10 |
    | spec-warden | … and leaves `u1_handle_reuse.hero` at 0 | **LAPSED**: the sitting ruled it depends on the allocator and not on the route (byte-identical C gives 0 and 134) | none, by the sitting's ruling |
    | spec-warden | candidate 1: if it lands as M1, the landing commit reads 8272 ± 2 real | **VOID**: M1 never landed. The releaser clause landed at step 10, appended to the set sentence, at +82 real (row 6344) | none possible |
    | spec-warden | candidate 2, *nothing for Q1*: the spec digest does not move in the repair commit | **VOID**: it was written against route A with no sentence, which was not taken. The repair commit carried the set sentence, and the digest moved `0159e9b26bd998a2` → `3cb9c9345d5a33fc` | `heroes measure spec/heroes-spec.md --refresh` in the copies of `f7a2161a` and `5ebbe8ba`, 2026-09-26 |
    | spec-warden | candidate 3: a reader following the `ptr` sentence for an `int *` gets `check` 1 | **VOID**: the sentence was vetoed and never landed | none possible |
    | llm-ergonomist | Task 1, ≥20 trials a variant: silent wrong closer ≥25% under X and 0% under Y; first-try correctness moves ≤10 points | **CARRIED to M-thesis-harness**, as the sitting named | not scored now |
    | historian | the first `examples/` program to declare two consuming functions for one handle type declares an interchangeable pair | **CARRIED to the first milestone whose `examples/` gains one**. Its consequence clause (*correct programs will then abort under that version of A*) is void: A landed keyed on a set, not on one name | `git diff m-declared-extents bdf430f1 -- examples \| wc -l` is 0: `examples/` did not change this milestone. The seven files there mentioning `consumes` pair no two consuming functions on one type |

    ## Panel 176: a transfer names where the life goes

    | seat | prediction | verdict | how it was scored |
    |---|---|---|---|
    | compiler-engineer | `heroes check` over the 510 files at `a747e5a2` changes verdict on exactly one, `abort-handle-borrows-that-gives-away` | **VOID**, as the sitting marked it: it rests on the call-site rule, which did not land | none possible |
    | compiler-engineer | `contract_differs` fires on none of those 510 | **HELD** | `$S/checkall.sh` with the step 12 merge's compiler (`3f76a72d`) over the 510 `.hero` files `git archive a747e5a2 tests/golden examples` holds: 351 check 0, 159 check 1, **0** carrying `contract_differs`. Output: `$S/check-a747-by-step12.txt` |
    | compiler-engineer | that landing adds between 250 and 450 non-test lines to `selfhost/` | **FALSIFIED** | The resolution landed in three steps, 10 (`5ebbe8ba`), 11 (`9f813de2`) and 12 (`572367a8`), and nothing else touched `selfhost/` between them (the per-step nets sum to the total). `git diff --shortstat f7a2161a 572367a8 -- selfhost`: +2189 −449. Outside `test` blocks: **+1608** −386 non-blank (net +1220), +1075 −230 non-comment (net +843). By step, net non-comment: +77, +583, +183. The landing carried more than the prototype priced (V1c's check, the success clause, the receiver rule, the review's four clauses) and left out the call-site rule |
    | compiler-engineer | `check/contracts.hero` is the largest new file | **HELD** | `git diff --numstat --diff-filter=A` over the three steps: `check/contracts.hero` 278 lines; next `check/releasers.hero` 268, `parse/marks.hero` 155 |
    | ffi-pragmatist | a result-only R1 leaves `x509_upref.hero` at 134 on the three POSIX legs | **VOID**: R1 landed with its parameter form, and a result-only build never existed | none possible |
    | ffi-pragmatist | the parameter form takes `x509_upref.hero` to 0 on Darwin arm64, Linux arm64 and Linux x86-64 | **HELD** at the landing (step 11) and on the trunk. Windows unrun: no OpenSSL on the box | The seat's `work/routes/x509_upref.hero` with `X509_up_ref(a: X509 retains X509_free) -> i32 when 1` (and without `when 1`), real OpenSSL 3. Step 11, Darwin: 0/0B ×5 both ways; as filed, with no mark, 134/515B ×5. Linux ×2 at step 11, plain and `--sanitize`: 0, `two references, both given back`. Trunk, Darwin: 0/0B ×5; the unmarked file is now `check` 1 `handle_used_after_end`. Ledger row 6643 had scored the golden analogue; this is the seat's own program |
    | ffi-pragmatist | `ssl_same_bio.hero` exits 134 on the three legs whichever consuming word lands | **HELD** | The seat's file as filed (`consumes`) and with `transfers BIO_free` at both positions, real OpenSSL 3. Step 11 and trunk, Darwin: 134/229B ×5 (*`SSL_set_bio` is not one of them*) and 134/371B ×5 (*one call takes the same C handle at two consuming positions*). Linux ×2 at step 11, plain and `--sanitize`: 134 and 134. Defect 084's close record adds the golden form on Windows, at the box's 127 |
    | spec-warden | `xfer_jsonc`, `xfer_cj` and `xfer_ssl` written `consumes` are refused at `check` naming `transfers` | **VOID**, as the sitting marked it: it rests on the call-site rule | none possible |
    | spec-warden | the same three written with `transfers` run at 0 | **HELD** on the three POSIX legs | `xfer_jsonc`: panel 177's P3, given (`xfer_jsonc_t` 0). `xfer_cj` with `item: Json transfers cJSON_Delete` (and with `when 1`), and `xfer_ssl` with `rbio: Bio transfers BIO_free`, from `docs/panel/176-briefs/`. Step 11 and trunk, Darwin, plain and `--sanitize`: 0/0B ×5 each. Linux ×2 on the trunk, plain and `--sanitize`: 0, `transferred and deleted` and `the BIO went to the SSL and was freed with it` (`$S/lin-*-trunk.txt`) |
    | spec-warden | if C1 lands as priced, `measure --refresh` reads 8543 ± 8; appended, ≥ +200 | **VOID**: C1 is not the text that landed. The landing split into step 10's sentence and step 11's three, a different text | none possible |
    | spec-warden (the sitting's wording) | the landed text reads within ±8 of its own real price | **HELD, 0 off** | `heroes measure spec/heroes-spec.md --refresh` in each copy, 2026-09-26, on `claude-opus-5`: `f7a2161a` **8361**, `5ebbe8ba` **8443** (row 6344 says 8443), `9f813de2` **8805** (row 6643 says 8805). The digests match the rows: `3cb9c9345d5a33fc`, `293f81d403914a7d` |
    | spec-warden | `refcount.hero` exits 0 and `getter_wrong_repaired.hero` stays 134 before C, at the landing | **HELD** | `docs/panel/176-briefs/` copies; `refcount` with `obj_ref(o: Obj) -> Obj retains obj_unref`. Step 11, Darwin: `refcount` 0 ×5, with 24 B of stderr that is the header's own `[C] freed at refcount 0`; unmarked as filed, 134/515B. `getter_wrong_repaired` 134/515B ×5, the runtime's stray line and no allocator report. Trunk, Darwin: 0 and 134/288B (the dead set, before `g_close`). Linux ×2 on the trunk, plain and `--sanitize`: 0 and 134 |
    | spec-warden | A2 *at one type*: `xmod-lent` and `xmod-owned` are `check` 1 and `twoarity` stays `check` 0, at the landing | **HELD** | `heroes check` on copies: with step 11's compiler all four are 0 (the rule is not there yet). With the step 12 merge's and the trunk's: `xmod-lent`, `xmod-owned`, `xmod-xacquires` **1**, `error[contract_differs]`; `tests/golden/surface-fixtures/twoarity/main.hero` **0** |
    | historian | under R1, `getter_wrong_repaired` stays caught | **HELD**, the same runs as the spec-warden's row. The comparison clause (*counting every address turns it into a raw double free*) is about panel 175's `runtimeA2`, which did not land | as above |
    | historian | V2: incomplete sets, so a correct transfer aborts | **VOID**: V2 was refused | none possible |
    | historian | the first transfer a program in `examples/` or a core package marks transfers only on success | **CARRIED to the first milestone whose `examples/` or packages gain one** | `examples/` did not change this milestone (0 diff lines), and no file there says `transfers` |
    | llm-ergonomist | task 1 first-try correct ≤60% under X and ≥90% under V1, at ≥20 attempts | **CARRIED to M-thesis-harness**, as panel 177 named: one reader cannot give a rate | not scored now |
    | panel 175's three spec predictions | see *The ledger's own rows* below | scored at this sitting | — |

    ## Panel 177: a dead handle is poisoned where it lay

    | seat | prediction | verdict | how it was scored |
    |---|---|---|---|
    | compiler-engineer | P + M-must + β: `selfhost/` grows +500 to +700 non-blank, non-comment lines outside `test` blocks | **FALSIFIED**, on the landing as the sitting kept it. The landing was not the shape priced: β had landed at step 11, and T landed beside P, so no component-exact score exists | `$S/codelines.py 884d7135 0e7a61be selfhost` (the landing's merge against its first parent, the lane having merged the trunk at `884d7135`): **+988 −83, net +905**. Without `emit/callback_thunk.hero`, which is T's alone: **+774**. The checker's half alone (`c4c29026`, M-must): +502, against the +329 it was priced at. β is in neither figure |
    | compiler-engineer | `runtime/` +20 to +40 | **VOID**, as the sitting marked it: T was adopted beside P | For the record: `git diff --shortstat 884d7135 0e7a61be -- runtime` is 3 files, +365 −7 |
    | compiler-engineer | `heroes check` over the 510 files changes verdict on exactly two, `run/abort-handle-given-back-twice.hero` and `run/fixedbugs-a-real-deallocator-given-the-same-handle-twice.hero`, both 0 → 1 | **HELD** | `$S/checkall.sh` over the 510 `.hero` files of `tests/golden` and `examples` at the sitting's HEAD `521c5e02`, with the compilers of `884d7135` and `0e7a61be`: exactly those two move, 0 → 1, `error[handle_used_after_end]`. Over the 567 files at `884d7135`, a third also moves, `run/fixedbugs-a-double-release-before-any-acquisition-is-stopped-before-c.hero`, which step 5 added after the sitting |
    | compiler-engineer | no program in `examples/` changes its `run` verdict: `corpus` stays 55 / 0 | **HELD** | `heroes run tests/harness/main.hero -- ./heroes corpus` in the copy of `0e7a61be` with its compiler: **55 passed, 0 failed** (`$S/corpus-0e7a61be.log`) |
    | compiler-engineer | if R lands, `ffi-borrows-owes-nothing` goes red | **VOID**: R was refused | none possible |
    | ffi-pragmatist | if R lands, `jsonc_read` aborts; if S lands, `ossl_verify_cb` fails to build | **VOID**: R and S were refused, on this seat's vetoes | none possible |
    | ffi-pragmatist | P checked only at handle arguments leaves `pair_escape` at 0 | **FALSIFIED in the sitting**: the compiler-engineer's P aborts it, 134 on three legs (critic) | the sitting's |
    | ffi-pragmatist | the success clause: `jsonc_success` exits 0 at n=1 and 134 at n=2; with the value written wrong, 134 at n=0, *never given back* | **HELD on the landed build**, Darwin arm64, Linux x86-64 and Linux arm64. Windows unrun: no json-c on the box | The seat's `work/x/jsonc_success.hero` and `jsonc_success_wrong.hero`, the mark written as landed: `val: Json transfers json_object_put) -> i32 when 0`, and `when 1` for the wrong one. Step 11 and trunk, Darwin json-c 0.19, plain and `--sanitize`. Correct value: n=0 **0**, n=1 **0**, n=2 **134** *1 C handle(s) never given back*. Wrong value: n=0 **134** *never given back*, n=1 0, n=2 134. Linux ×2 on the trunk, json-c 0.18: the same nine exits in each mode |
    | spec-warden | P1: 8675 ± 8 real, or +25 above if the grammar is inline | **VOID**, as the sitting marked it: the landed text adds items 1-3, 6, 7 and 9 | none possible |
    | spec-warden | P2: the reader binds `json_object_object_add` with `when 0` and a failure path releasing `val`, and `X509_up_ref` with `retains X509_free` and `when 1`, first try | **HELD**, scored by the coordinator 2026-09-26, taken as given | A fresh llm-ergonomist seat given only the landed spec (sha `4123f19de2237bef`); both bindings right on the first attempt, `check` 0, built and run against json-c 0.19 and OpenSSL 3 on Darwin arm64, 0 plain and under `--sanitize` (`$S/../reader-landing/t1.hero` to `t4.hero`, `answers.md`) |
    | spec-warden | P3: panel 176's critic's four programs run 0 / 134 / 0 / 0 | **HELD**, scored by the coordinator 2026-09-26, taken as given. Windows unrun (no json-c on the box) | `$S/../p3/*.hero`, adapted only to the landed spelling: `jsonc_failed_add_t` 0, `jsonc_failed_leak_t` 134 *1 C handle(s) never given back*, `jsonc_ok_add_t` 0, `xfer_jsonc_t` 0, plain and `--sanitize`, on Darwin arm64, Linux x86-64 and Linux arm64 |
    | spec-warden | P4: under sentence I the reader expects 088's program stopped | **FALSIFIED in the sitting**: the reader expected it to reach C, so D+'s +20 is unpaid (critic) | the sitting's |
    | llm-ergonomist | P2: III misses 088 on at least one leg | **HELD in the sitting**, on all three POSIX legs | the sitting's |
    | llm-ergonomist | P1 (β no worse than α on polarity, 20 fresh readers), P3 (≥15 of 20 free one name twice under II), P4 (≥1 of 20 edits the binding in an A4 task, 0 of 20 with a receiver) | **CARRIED to M-thesis-harness**, all three | not scored now |
    | historian | P aborts all five of the brief's reproducers before C on every leg | **HELD on the three POSIX legs**, in the sitting. **Its Windows half cannot be run in its own terms any more**: it is a claim about P alone, and no build of P alone exists outside the sitting's prototypes | Measured instead on the landed build, 2026-09-26, the brief's five programs on the Windows box (the trunk's seed built there, plain and `--sanitize`): `read_after_consume`, `use_after_consume`, `u1_static` and `reuse_malloc` are `check` 1 `handle_used_after_end`; `helper_consume_then_use` builds and dies at the box's **127** before `cJSON_AddItemToObject`, *a dead C handle reached the argument `item`*. Darwin on the trunk gives the same, with 134/383B ×5 for the helper |
    | historian | one line of copy returns each row to today's number under P | **HELD for P alone**, in the sitting (the compiler-engineer's E13) | the sitting's |

    ## Panels 179 to 182, scored 2026-09-28

    **Where the list came from.** The *Predictions to score* section of each of
    `docs/panel/179-*.md` to `182-*.md`, read with the whole sitting, its
    resolution, its landing section and any later section beneath it. Panel 181
    registered a second table beneath its synthesis (the second blind reading),
    and it is scored here. For panel 180, two predictions stand in the verdict
    table and were never copied into the section (the llm-ergonomist's first
    reading); they are listed at the end of that section and counted in the
    tally separately. A prediction with several independent clauses is split
    into one row per clause, as panel 179's own landing table did.

    **How the numbers were taken.** `$T` is
    `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/e7e3d0df-8db3-44f2-b628-af9bb1a6080e/scratchpad`.
    Each compiler was built from its own commit's seed, `clang -O2 -I runtime
    seed/heroes.c runtime/runtime.c -o heroes` (`2b1a1f24` without `-O2`), in a
    detached worktree under `.claude/worktrees/`: `score-179-182` (the trunk,
    `a63cf9ed`), `score-0fc98107`, `score-e8ed8732`, `score-08581ace` and
    `score-2b1a1f24`. Nothing in the repository was edited. **Nothing was timed**:
    another lane was gating on this machine, so every prediction about seconds is
    scored from the committed record that measured it, cited with its sentence.

    **How lines were counted.** By `$T/codelines.py`, a replica of
    `tests/harness/suite_layout.hero`'s `code_lines` (lines 669-687 at
    `a63cf9ed`, with `strings.lines` and `strings.trimmed` from
    `tests/harness/strings.hero`): a `test "` line at column 0 opens a block, a
    non-empty line at column 0 that is not a `#` comment closes it, and every
    non-blank line outside a block counts. It carries the suite's own three unit
    cases as asserts, and it reproduces the figures the landings recorded:
    `probe/reader.hero` 296, the probe's 1,324 and 936, `print/guard.hero` 168,
    `cli/syntax_cmds.hero` 181 to 69, and lane 181's per-module rows exactly.
    `suite_layout` itself was also run at the trunk:
    `heroes run tests/harness/main.hero -- ./heroes layout`, *4 passed, 0
    failed*. Never `wc -l`, except where a row says so.

    ### Panel 179: the formatter's probe is a verb, and its oracle is ruled before it lands

    | seat | prediction | verdict |
    |---|---|---|
    | compiler-engineer | the probe's modules each at or under 300 counted lines | **HELD**, scored at the landing and re-counted at `e8ed8732` and the trunk: the largest is `probe/reader.hero` at 296; `cli/probe.hero` 292 (293 at the trunk, +1 from lane 181), `probe/lines` 229, `print/guard` 168, `probe/judge` 166, `probe/seed` 146, `probe/tokens` 103, `probe/reduce` 92 |
    | compiler-engineer | the three modules the seat named together at or under 420 (the bound the sitting kept on those three) | **FALSIFIED**: predicted ≤ 420, measured **936** at `e8ed8732` (deform = `probe/seed` 146 + `probe/lines` 229 + `probe/tokens` 103 = 478, judge 166, command 292), 937 at the trunk; with the reader and the reduction the probe is 1,324 |
    | compiler-engineer | `heroes probe selfhost/check/walk.hero` reports 39,731 variants for comment insertion and bracket break | **HELD** on the text the seat measured. `walk.hero` as at `83ac68c1` (`git show`), `heroes probe <it> --family single` and `--family bracket` with `--stride 9999999` (every variant counted, one judged), on `e8ed8732`'s compiler and on the trunk's: **10,364 + 29,367 = 39,731**, predicted 39,731. The same command on the landing's own `walk.hero` reads 10,367 + 29,373 = 39,740, as the landing recorded; on the trunk's, 10,367 + 29,391 = 39,758, lane 181 (`5772c830`) having touched it since |
    | compiler-engineer | the fully judged run over `walk.hero` against `83ac68c1` refuses at least 1% of the parsing bracket breaks (5 of 225 in the sitting's 1-in-100 sample) | **VOID**. The sitting registered it as *"a number to take at the landing, not a prediction"*, and the configuration it names, the probe judging `83ac68c1`'s formatter, was never built: item 8 put lane g's repairs before the probe landed. The number taken at the landing on the landed printer: 0 of 22,860 (`e8ed8732`'s body). Re-sampled on the trunk at the sitting's stride, `heroes probe selfhost/check/walk.hero --family bracket --stride 100`: 294 judged, 239 parsing, **0 refused** by either judge, 55 discarded as `line_end_before_continuation` |
    | spec-warden | the spec reads 8861 real and 6693 vendored after the landing | **FALSIFIED** as numbers; the zero delta it stood for held. `heroes measure spec/heroes-spec.md` at `e8ed8732`: real **8999** (the pin, *claude-opus-5, 2026-09-27*), vendored maximum **6794**, identical at its first parent `0fc98107`. `git diff 0fc98107 e8ed8732 -- spec/` and `git diff d3fad66c 49e50f34 -- spec/` (the merge against its first parent) are both empty. The spec moved at panel 180's landing, `74203a64`: vendored 6693 to 6794 (`measure` on both texts, run here), real 8861 to 8999 (the pins in `selfhost/measure/pinned.hero` at `2b1a1f24` and `74203a64`; `--refresh` not re-run) |
    | spec-warden | `heroes --help` grows to 74 lines and 948 to 950 vendored tokens (the verb form) | **FALSIFIED**: predicted 74 lines and 948 to 950, measured **75 lines and 1001** at `e8ed8732` and unchanged at the trunk (`--help` captured, `wc -l`, then `heroes measure` on the capture: claude-legacy 844 to 940, cl100k_base 901 to 1001). At the lane's base `0fc98107`: 71 lines and 901. The seat's own base was 72 and 925, at `fa813325` |
    | spec-warden | the fixtures give 7,982 generated and 6,350 parsing (four families, 30 files) | **FALSIFIED on the probe that landed, on both counts.** `heroes probe <the 30 comments101 files as at 83ac68c1> --family <f>`, unstrided, on `e8ed8732`'s compiler and on the trunk's, identical: single 3,077 generated / 3,077 parsing, multi 270 / 270, bracket 4,077 / 3,012, paren 981 / 618; **8,405 generated, 6,977 parsing** against 7,982 and 6,350. The three families the port shares with the seats' generators give exactly 7,424, the seats' 3,347 comment and 4,077 bracket; the whole generated difference is paren, 981 against 558, the correction the landing itself records (the seats' `parengen.py` never wrapped a literal). **The landing's "generated HELD" rests on the recovered Python, not on the probe** (`e8ed8732`'s body: *the recovered Python reads 3,077 + 270 + 4,077 + 558*; not re-run here, since the generators import an `rd` module whose only recovered copy is `laneg/results/rd-final.py`), **and its 6,560 is a sum across the two instruments**, the probe's 3,347 + 3,012 and the seats' paren 201, which no single run prints. Bracket parsing 3,012 against the seats' 2,802: that panel 180's R1 is the 210 stays an inference, as the landing said. Item 7's own fixture set is not these 30 files (41 at the sitting; 63 pinned at the landing, *26,813 generated, 23,669 parsing*), so the number cannot hold on it either |
    | historian | none of the ten formatter CLI references it cites lists a generator of deformed inputs within twelve months; Black's `--safe` stays default-on | **NOT YET CHECKABLE**, until 2027-09-27, the horizon the seat registered |
    | critic | `parens.hero` gives 576 variants, 456 parsing, 6 refused at `83ac68c1` | **HELD, scored in the sitting** (`docs/panel/179-reports/completeness-critic.md` § 0: 213 + 9 + 291 + 63 = 576, 456 parsing, 6 refused by the guard, cross-checked against the compiler-engineer's `c101.out`). Re-run on the landed probe over `parens.hero` as at `83ac68c1`, on `e8ed8732`'s compiler and the trunk's: 213 + 9 + 291 + **84** = 597 generated (paren 84, the port's wider set), 486 parsing, **0 refused**, where `83ac68c1`'s formatter refused 6 |

    ### Panel 180: a line inside brackets breaks by how it ends, and a list refuses a subtraction it would split

    The compiler-engineer's prediction names *M-agreed-retention's close*; it is
    measured at `a63cf9ed`, and the coordinator re-measures it after the next lane
    merges.

    | seat | prediction | verdict |
    |---|---|---|
    | compiler-engineer | at the close, `layout` 3/0 | **HELD** on what it stood for, 0 failed; read literally, the pass count is 4, not 3. `heroes run tests/harness/main.hero -- ./heroes layout` at `a63cf9ed`: *layout: 4 passed, 0 failed*. The fourth check, `layout/concat`, entered at `95dc08fe` (step 20, defect 105, 2026-09-27 14:40), after the sitting's frozen trunk `29ed5601` (11:52), so the seat's count could not include it (`git log -S'layout/concat' -- tests/harness/suite_layout.hero`) |
    | compiler-engineer | at the close, `grammar_expr.hero` at most 1080 code lines, 1056 if the refusal leaves the knot, within 2 | **FALSIFIED** on the branch that applies. R2's refusal left the knot, into the new `parse/list_line.hero` (147), so the figure is 1056 ± 2. Measured: 1051 at `2b1a1f24` (the lane's base, and the sitting's `29ed5601`), **1061** at the lane's `74203a64`, 1062 at the merge `49197fc9` and at `0fc98107`, **1072** at `a63cf9ed` (lane 181's +10, comment lines by that lane's body). The outer bound of 1080 held at every commit, and so did the 1085 ceiling |
    | compiler-engineer | at the close, `parse/type.hero` at 285, within 2 | **FALSIFIED**: predicted 285 ± 2, measured **295** at `74203a64` and unchanged to `a63cf9ed` (280 at `2b1a1f24`). The +15 is every type closer moved to `line_end.expect_closer(... holds: .one_type ...)` and `line_end.expect_after`, one call re-wrapped over eight lines: R3's route, which the resolution calls *unprototyped*, so the growth is the resolution's reach beyond the seat's (b)+(ii) |
    | compiler-engineer | no existing program edited to keep compiling | **HELD**. `git diff --stat 2b1a1f24 74203a64` modifies eight existing `.hero` files: `grammar_expr`, `parse`, `parse/members` and `parse/type`, which carry the repair; `print/breaks.hero` and `comments101/indexparens.hero`, whose edits are comment-only (the two R6 ordered; `git diff` read); and two harness files, `suite_spec.hero` (the spec pin) and `suite_surface.hero` (new rows, and the row quoting the corrected header). No golden, example or harness program was edited to keep compiling |
    | spec-warden | the `real` delta between 1.2 and 1.7 times the vendored delta | **HELD, scored in the sitting** (+138 against +101, 1.37). Re-checked on the landed text: vendored 6693 at `2b1a1f24` to 6794 at `74203a64` (`heroes measure` on both, run here); real 8861 to 8999 by the pins at those commits and the ledger row `docs/measurements/010-spec-budget-ledger.md` *6794*; 138 / 101 = **1.37** |
    | llm-ergonomist, second reading | P1: its twelve answers match the (b)+(ii) prototype, 12 of 12 | **HELD, scored in the sitting** (`docs/panel/180-reports/llm-ergonomist-second-reading.md:138-141`). Re-run on the landed compiler, the brief's twelve fragments in `$T/p180/frag/`, `check` then `run` (11 by `parse`), trunk: 1 accepted; 2 accepted, 2 elements; 3 accepted; 4 refused, `spaced_minus_element`; 5 accepted, 2; 6 accepted, 1; 7 refused, now by R3's `line_end_before_continuation` where the prototype said `expected_group_close`; 8 accepted, 2; 9 accepted, 2; 10 refused, `spaced_minus_element`; 11 refused, `expected_extent`; 12 accepted: **12 of 12**. The compiler before the landing (`2b1a1f24`) answers 2, 3, 4 and 10 otherwise |
    | llm-ergonomist, second reading | P2: zero silent misreadings at breaks and the first-try rate moving at most 5 points, at the next harness run that generates such programs | **LAPSED**: the instrument has no arm. `harness/tasks/README.md:13` reads *Status: 0 tasks*, the directory last touched at `8715133c` (2026-09-12); panel 046 R1, as amended 2026-08-14, makes such a registration an observation that pays nothing, and R2 forbids renewing it under a new horizon. The seat named an event, not a milestone, so nothing is carried. Panel 181's twin was ruled the same way in its own table |
    | llm-ergonomist | prediction 3: zero silent misparses at breaks inside brackets, most loud refusals from a leading operator or a trailing comma, at the next harness run that generates such programs | **LAPSED**, the same instrument and reason; the sitting itself wrote *unscorable until then* |
    | historian | P1: the silent `[a` / `- b]` reads as two elements | **HELD, scored in the sitting** (prints 2). Re-run, the seat's program (`xs = [a` over `- b]`, `print(xs.len())`): `2b1a1f24`'s compiler, check 0, prints **2**; the trunk's, check 1, `spaced_minus_element`, R2 as intended |
    | historian | P2: Go refuses `x := (1` / `+ 2)`; Odin accepts it and refuses `f(1` / `+ 2)` | **HELD, run by the critic in the sitting**, and re-run on the critic's files from `panel-180/critic/precedent/`: `odin check o1` exit 0, `odin check o2` exit 1, *Expected a comma, got a newline* (Odin dev-2026-09); `go build g1_group_before_op.go` exit 1, *unexpected newline, expected )* (go1.27.1) |
    | llm-ergonomist (verdict table only) | the reader's Q column matches today's compiler, 10 of 10 | **HELD, scored in the sitting**; not re-run here |
    | llm-ergonomist (verdict table only) | `deltas = [` / `1` / `-1` / `]` prints 2 | **HELD, scored in the sitting**, and re-run: prints **2** on `2b1a1f24`'s compiler and on the trunk's (`$T/p180/e1_deltas.hero`) |

    ### Panel 181: outside brackets a line ends its statement, in both directions

    The sitting's rule for the compiler-engineer's row: *the resolution reaches
    further than the seat's route (items 1 (ii) and 5), so a growth there is
    scored as the resolution's and said so.* The counts below are lane 181's
    merge `08581ace` against its first parent `e90b682e`; for every file below
    they equal the counts against `0fc98107`, and none of these files moved again
    before `a63cf9ed`. The lane's commit body (`5772c830`) scores the same figures.

    | seat | prediction | verdict |
    |---|---|---|
    | compiler-engineer | (1) no module under `parse/`, `check/`, `ir/`, `emit/`, nor `cursor.hero`, `grammar_expr.hero`, `parse.hero`, grows in code lines for the repair | **FALSIFIED as registered; every added line is the resolution's reach, by the sitting's own rule.** `grammar_expr.hero` 1062 to 1072 (+10, item 1 (ii)'s suffix stop, defect 119); `parse.hero` 117 to 118 (+1) and the new `parse/wrap_break.hero`, 98 (item 3's parenthesised `guess`); `parse/line_end.hero` 247 to 254 (+7: `use open_line` and the comment above a premise test tying the two token lists); `parse/use_line.hero` 304 to 306 (+2, the `use` path read whole across the refused join, item 2's one diagnostic per break on the `use sub/` over `m2` shape the seat itself measured at three). `check/walk.hero` edited, +0; nothing under `ir/`, `emit/`, nor `cursor.hero`. Whether the seat's route alone would have grown them is unrun: no build of it exists at the landing |
    | compiler-engineer | (2) the lexer modules grow at most 180 code lines together over `0fc98107` | **FALSIFIED**: predicted ≤ 180 (the prototype 162), measured **+484**, 604 to 1,088, at the landing and at the trunk: `layout` 117 to 166 (+49), `state` 101 to 118 (+17), `lexer` 140 to 142 (+2), `scan` 246 to 163 (-83) with the new `punctuation` 110 (the table moved out so `open_line` can read the next line's first token), the new `open_line` 247 and `next_line` 142. No reading rescues it: `layout`, `state`, `lexer` and `open_line`, the modules the seat's own route needed, are +315 alone |
    | compiler-engineer | (3) none of the ten `.expected` files of the 16 error-token and 4 `use` hits changes | **HELD**. The brief's enumeration (`docs/panel/181-briefs/00-shared.md:123-145`) names nine files; seven carry an `.expected` (`check/literal-bases`, `leading-zero`, `base-prefix-fix` with its `.fixed`, `unterminated`, `unterminated-hole`, `use-has-a-path`, `fixedbugs-use-refusal-eats-the-next-line`), and the two `json102` surface fixtures carry none; the seat's *ten* could not be reconstructed. All eight snapshot files: 0 diff lines `0fc98107` to `08581ace` and to `a63cf9ed`. The one existing `.expected` the landing modified is `check/depth-zero-continuation.expected`, which the seat's condition exempts |
    | spec-warden | over the 1164 `.hero` files of `0fc98107` as they stand there, exactly one `check` exit changes, `comments107/margin.hero` 0 to 1 | **HELD**, scored at the landing (the four defect records, *634 and 530 after*) and re-run here: every path of the seat's baseline `panel-181/spec-warden/price/exits.tsv` (1,164 rows, sha256 prefix `edcb78bf10c1fd0d`, matched), `heroes check` from the root of the `0fc98107` worktree (`$T/p181/exits.sh`). `0fc98107`'s compiler reproduces the baseline row for row (635 zero, 529 one, `diff` empty); `08581ace`'s gives 634 and 530, the one differing row `./tests/golden/surface-fixtures/comments107/margin.hero`, **0 to 1** |
    | llm-ergonomist | the compiler refuses 1, 2, 4, 5, 6, 7, 8, 10 | **FALSIFIED, scored in the sitting** (it accepts 1, 4, 5 and 10). Re-run on the coordinator's key `k01` to `k10`: `0fc98107`'s compiler accepts 1, 3, 4, 5, 9, 10; the trunk's accepts 3 and 9 alone, which is the seat's column |
    | llm-ergonomist | at the next harness run, Y at most halves X's first-try refusals, silent errors 0 | **LAPSED**, as the sitting ruled (*unscoreable as registered ... not renewed (panel 046 R1)*): `harness/tasks/README.md:13`, *Status: 0 tasks* |
    | historian | P2: Guido van Rossum's typo in Heroes exits 1 with a type diagnostic | **HELD, scored in the sitting**, and re-run on the key's `p2_guido.hero`: `0fc98107`, exit 1, `bad_operand`. Since the landing (`08581ace`, the trunk) it exits 1 with `continuation_outside_brackets`, a layout diagnostic: the resolution working, since the prediction was about the compiler the sitting sat on |
    | historian | P1: Nim 2.2 admits `let y = a +` / `1` at the same column and refuses `let n = xs.` / `len` there | **HELD, run by the critic in the sitting**, and re-run on the critic's `n1_plus_same.nim` and `n2_dot_same.nim` with Nim 2.2.12: `nim check` exit 0, and exit 1, *invalid indentation* at 4:3 |
    | llm-ergonomist, second reading | the landed compiler answers the fifteen fragments as the seat did, 15 of 15, or 14 of 15 with fragment 9 the one; no refused fragment accepted with another meaning; both written programs compile | **HELD, 15 of 15**, scored at the landing (`5772c830`'s body) and re-run here on `08581ace` and the trunk: the brief's fifteen, verbatim at their columns, inside `function main()` with its names and every bound name read afterwards (`$T/p181/frag/gen.py`). Accepted 3 (`y` 6), 9 (`ys.len()` 3, `ys[2]` 3) and 12 (the `if` body does not run); the other twelve refused with one diagnostic each, ten `continuation_outside_brackets`, 10 and 11 `discarded_value`; no silent divergence. The seat's two Task 2 programs with one-parameter stubs: check 0, print 14 |

    **Also run beside the rows, on item 3** (*a `certain` fix that fails to
    compile or prints other than the joined control is a defect of the landing*):
    `check --apply` on the twelve refused fragments with the trunk's compiler.
    Eight carry a `certain` fix; each applied program checks 0 and prints the
    joined reading (6, 6, 5, 6, 13, 5, 6, 1). The other four offer only
    `guess`es and `--apply` leaves them unchanged. No new fault found. Defect 129
    (open, carried by the author's instruction of 2026-09-28 16:40) is a known
    case of exactly this rule failing: a line holding `-` alone draws two
    `certain` joins that together write a program `check` refuses. Not
    re-reproduced here.

    ### Panel 182: a value is never zeroed, a slot is, and every definition is written whole

    | seat | prediction | verdict |
    |---|---|---|
    | compiler-engineer | with (a-min), (d) or (f) landed, `grep -c '= {0};' seed/heroes.c` at most 22,000 | **HELD**: predicted ≤ 22,000, measured **21,566** at the lane's `81532acc` and **21,652** at the merge `c8ed80bc` and at the trunk (102,990 at `0fc98107`, 107,282 at the merge's first parent `b244372e`); `grep -c` and `grep -o \| wc -l` agree, as the record says (`docs/records/done/2026-09-28-1731-defect-114-closed-...md`, *21,566 ... 21,652 after the merge*) |
    | compiler-engineer | clang's `-Wuninitialized -Wsometimes-uninitialized -Wconditional-uninitialized` on the seed 0 | **HELD**: `clang -fsyntax-only` with the three flags on the trunk's seed (Apple clang 21.0.0): **0** warnings. Control, the same seed with every ` = {0};` stripped: 21,903 warnings (17,229 `-Wuninitialized`, 4,366 conditional, 308 sometimes), so the instrument fires where it should |
    | compiler-engineer | `fmt` on sixteen copies of `walk.hero` at most 0.85 of the `0fc98107` seed's user time, interleaved | **HELD**, from the record, not re-timed: `81532acc`'s body, *fmt -O0 base 3.06 3.05 3.04 ... lane 2.39 2.39 2.39 (0.78)*, with *base = 0fc98107's seed* and *real within 0.14 s of user plus sys on every run*; the same figures in the defect 114 record, § The time. 36,688 lines is 16 × 2,293, `walk.hero` at `0fc98107` by `wc -l` (checked). At `-O2` 0.93 |
    | ffi-pragmatist | with (a), (b) or (d) landed, no declaration of `heroes_runtime.h` changes and the ABI reads 26 | **HELD**: `git diff b244372e c8ed80bc -- runtime/heroes_runtime.h` and `git diff 0fc98107 a63cf9ed -- runtime/heroes_runtime.h` are both empty, nothing else under `runtime/` changed in the lane, and line 37 reads `#define HERO_RUNTIME_ABI 26` on both sides |
    | ffi-pragmatist | no `.expected` of the 119 FFI `run` goldens and nothing under `examples/sqlite/` changes | **HELD**: no file under `tests/golden/run/` changed in the lane (`git diff --stat b244372e c8ed80bc -- tests/golden/run/` empty), no `run` `.expected` changed from `0fc98107` to the trunk, and `examples/sqlite/` has 0 diff lines both ways. The seat's 119 is not enumerated in its report; this tree has 117 `run` goldens with a top-level `extern` and 120 that mention `extern` or `link ` anywhere, and since nothing in the directory moved, the clause holds for every such set |
    | ffi-pragmatist | `run` green on all 119 | **HELD**, scored at the landing (`81532acc`'s body, *210/0 over the 210 run goldens*; Linux arm64 206 and the Windows box 204, each 0 failed, in the defect 114 record) and re-run here at the trunk with its own compiler: `heroes run tests/harness/main.hero -- ./heroes run`, *run: 210 passed, 0 failed*, exit 0, the 117 goldens with a top-level `extern` among them |

    ### Tally of panels 179 to 182

    | sitting | HELD | FALSIFIED | LAPSED | VOID | NOT YET CHECKABLE | rows |
    |---|---:|---:|---:|---:|---:|---:|
    | 179 | 3 | 4 | 0 | 1 | 1 | 9 |
    | 180, its section | 6 | 2 | 2 | 0 | 0 | 10 |
    | 180, verdict table only | 2 | 0 | 0 | 0 | 0 | 2 |
    | 181 | 5 | 3 | 1 | 0 | 0 | 9 |
    | 182 | 6 | 0 | 0 | 0 | 0 | 6 |
    | **total** | **22** | **9** | **3** | **1** | **1** | **36** |

    Of the 22 HELD, 2 are scored here for the first time (panel 180's `layout`
    and *no existing program edited*). The other 20 were scored in their sitting
    or at their landing; 18 of those are re-confirmed here by a command, panel
    180's Q column is taken as the sitting scored it, and panel 182's timing
    clause is taken from the lane's record, neither re-run.

    ## The ledger's own rows

    | row | prediction | verdict | how it was scored |
    |---|---|---|---|
    | 6282 (panel 175's text) | (1) asked to bind `void *make(void)` given back to `release`, the reader declares a `tag void` record and not a `ptr` | **HELD**, scored at panel 176 by that sitting's ergonomist, task 6 | `docs/panel/176-*.md`, *Predictions to score*, last row |
    | 6282 | (2) asked for `malloc`/`free` beside `sqlite3_malloc`/`sqlite3_free`, it gives the two families two `tag void` records | **HELD**, the same sitting | the same |
    | 6282 | (3) asked to mark a C function that frees a `ptr`, it does not write `consumes` on the `ptr` | **HELD**, the same sitting: it wrote a `tag void` handle | the same |
    | 6343 (step 10) and 6693 | pays with panel 176's ffi seat's, historian's, panel 177's spec-warden's P2 and P3, the llm-ergonomist's P1, P3 and P4, and the historian's | scored in the sitting tables above | — |
    | 6643 (step 11) | asked to bind `fclose`, the reader writes `consumes` and not `transfers`; asked to bind `BIO_new_fp(stream, BIO_CLOSE)`, it writes `transfers fclose` on the stream | **HELD**, scored by the coordinator 2026-09-26, taken as given | the same seat and sitting as panel 177's P2: both `check` 0 and run 0 against the real libraries |
    | 6794 (panel 180) | pays with the sitting's spec-warden's ratio and the llm-ergonomist's second reading | scored in panel 180's table above | — |
    | 6838 (panel 181) | pays with the sitting's spec-warden's census and the second blind reading's fifteen fragments | scored in panel 181's table above | — |

    The reader's three guesses at that sitting (a tag spelled with a typedef
    name, a null `acquires` result beginning no life, a second release
    through one name after `retains`) were all three right by the compiler.
    They are clarity questions and not predictions, so they are not scored.

    ## What this leaves

    **One verdict is the close push's to write**: panel 174's
    compiler-engineer named the close's pre-push Windows leg and the two after
    it. Nothing has been pushed since `d02b8bf4`, so those legs do not exist.
    The instrument is one `grep -ac` per job log and the `harness:` line.

    **Eight predictions are carried, each with the horizon its seat named**:
    to M-thesis-harness, panel 175's llm-ergonomist, panel 176's
    llm-ergonomist, and panel 177's llm-ergonomist's P1, P3 and P4. To the
    first milestone whose `examples/` gains one: panel 175's historian (a
    second consuming function for one handle type) and panel 176's historian
    (a transfer marked in `examples/` or a package). Neither horizon was
    reached here, since `examples/` did not change this milestone.

    **Unrun and said so**: every Windows clause over json-c, OpenSSL or
    SQLite (the box has none of the three); and panel 174's timing clause on
    this Mac, scored instead on CI's own step timings.

    **And of panels 179 to 182**:
    **One prediction is carried, with the horizon its seat registered**: panel
    179's historian, to 2027-09-27.

    **Three lapse** on one instrument with no arm, the metric 2 task suite
    (`harness/tasks/README.md`, *0 tasks*): panel 180's llm-ergonomist P3, its
    second reading's P2, and panel 181's llm-ergonomist on Y against X. None is
    renewed.

    **Three readings the coordinator may want to overrule**, each written in its
    row: panel 180's `layout 3/0` scored HELD on 0 failed although it reads 4/0
    (a check added after the sitting); panel 179's spec-warden fixture count
    scored FALSIFIED on the probe where the landing scored generated HELD on the
    seats' Python; and panel 179's compiler-engineer's *number to take* scored
    VOID where the landing wrote *taken*.

    **Found beside the rows, not defects of the compiler**:
    - the landing's 6,560 parsing figure for panel 179's fixtures
      (`e8ed8732`'s body and the sitting's landing table) is a sum of the probe's
      single, multi and bracket rows with the seats' paren row; the landed probe
      prints 6,977;
    - panel 181's compiler-engineer counted *ten* `.expected` files for the 20
      hits; the brief's nine files hold seven `.expected` and one `.fixed`;
    - panel 182's ffi-pragmatist's *119 FFI goldens* is not reproducible from its
      report; the tree gives 117 or 120 by two plain readings.

    **Nothing broken was found in the compiler** by the runs above: no crash, no
    wrong answer, and every `certain` fix applied compiled. Defect 129, open and
    carried, is the one known `certain`-fix fault in this area.

    **Scored beneath, 2026-09-29, from the close's push** (`d02b8bf4..f0b84729`
    and `f0b84729..79aeeffa`, 2026-09-28): panel 174's compiler-engineer, *on the
    close's pre-push Windows CI leg and the two after it, 0 lines carrying `the
    operating system's own reason is 32`, and `harness:` reads `0 failed`*, is
    **HELD on the first leg**, with two owed. The first push's run
    (36480104335) was cancelled by the workflow's `concurrency` group when the
    second push landed, every leg of it included, so the first Windows leg to
    finish is run 36482022429, job 109130284853, on `79aeeffa`:
    `gh run view 36482022429 --log --job 109130284853 | grep -ac "operating
    system's own reason is 32"` reads **0**, and its net reads *harness: 3067
    passed, 0 failed*, with the compiler's 828 tests and the net's own 179, in
    68 minutes. The two legs after it are the next two pushes' Windows legs,
    scored when they run, and the prediction is not renewed.
