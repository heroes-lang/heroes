# Panel 175: spec-warden report

Written out verbatim by the coordinator on 2026-09-23 from the seat's final
message; the seat has no file-write tool.

## The instrument, read today

- **The ceiling.** design.md §1.6, reached by grep (`design.md:255-256`): **10240**, counted by `claude-opus-5` through `POST /v1/messages/count_tokens`.
- **The baseline**, taken in my copy at `64c92654`:
  - `./heroes measure spec/heroes-spec.md`: 6088 legacy, **6212** cl100k. That is a lower bound, not the reader's tokeniser.
  - `./heroes measure spec/heroes-spec.md --refresh`: **8270** on `claude-opus-5`, digest `5b96ebe84dac6784`, taken 2026-09-23.
  - Headroom is 1970, of which the FFI floor mortgages 60, leaving **1910** to spend.
- **How each draft was priced.** Every draft was applied alone to my copy's `spec/heroes-spec.md`, with each anchor asserted to match exactly once. It was priced on both instruments and then reverted. After the last revert, `shasum` matched the pristine file and `--refresh` read 8270 / `5b96ebe84dac6784` again.
- **An interruption.** Partway through the sitting, a worktree-isolation hook refused every shell call for a stretch. I did not work around it. Nothing was ever run in the trunk.

## Prices, all measured with `--refresh`

| draft | text | vendored | real | Δ real |
|---|---|---|---|---|
| **candidate 1** | *…owes it, and ending it with another aborts.* | 6220 | 8280 | **+10** |
| **candidate 2** | nothing | 6212 | 8270 | **0** (the digest does not move) |
| **candidate 3** | *A pointer C hands out … nothing catches.* | 6242 | 8310 | **+40** |
| **candidate 4** | nothing | 6212 | 8270 | **0** |
| M1 (merge) | replaces *The live handles are a set, so giving one back twice aborts on its own.* with *Giving one back twice or to another aborts, unless C has since reused its address.* | 6211 | 8272 | **+2** |
| M1b | *giving one back twice, or to another, aborts on its own.* | 6217 | 8275 | +5 |
| R1b | *giving one back twice aborts, unless C has since reused its address.* | 6217 | 8277 | +7 |
| **R1c** | M1 without *or to another* | 6208 | 8269 | **−1** |
| **T1** | *two records may not name one tag but `void`.* | 6215 | 8274 | **+4** |
| Q2M2 | *…aborts on its own, and no set holds a `ptr`.* | 6220 | 8282 | +12 |
| M1+T1 | the two together | 6214 | 8276 | +6 |
| R1c+T1 | the two together | 6211 | 8273 | +3 |

- **The judges on M1+T1.** `./heroes run tests/harness/main.hero -- ./heroes spec|special|grammar` read `special` 10/10 and `grammar` 7/7. `spec` read 16/20: the four failures are `budget`, `spendable`, `real` and `ledger`, which are the pinned record disagreeing with a moved document. `named`, `rejected`, `inventory`, `shape`, `anchors` and `offered` all pass, so `void` in a code span is legal.
- **My budget veto does not fire.** The worst draft, candidate 3, gives 8310 + 60 = 8370 against 10240. Every draft is under `DELTA_GATE`'s 50 vendored.

## Four measured findings the candidates stand on

Probes are in `&lt;scratchpad&gt;/175-spec-warden/probe/`. Each was run with `heroes check`, `heroes build -O0` and five runs, on Darwin arm64.

**F1. An existing § 13 sentence is false once the address is reused.**
- The sentence is *The live handles are a set, so giving one back twice aborts on its own.*
- `u1_handle_reuse.hero` does this: `b = box_open()`, `box_close(b)`, `c = box_open()` (the same address, and it prints `true`), then `box_close(b)` again. It reads `check` 0 and `run` 0 with zero bytes, 5 of 5 at `-O0` and 5 of 5 at `-O2`.
- Adding `box_close(c)` at the end (`u1b_then_close_c.hero`) aborts **at the program's one correct call** (134, 396 bytes, 3 of 3), after the wrong call went through silently.
- The same shape over a `ptr` (`u2_ptr_reuse.hero`) is also run 0 with zero bytes, 5 of 5. No C library can see it, because the second free is a valid free of `c`'s block. So route E cannot reach this shape either.
- The cause, read rather than inferred: `runtime/parts/alloc.c:436` compares the address and nothing else, and `:411-414` says so.
- The records say nothing about it. I searched `docs`, `selfhost` and `runtime` for *reused*, *address out again* and *handed the address*, and the only hits are about `==` on records.

**F2. *Two records may not name one tag* has been false at `tag void` since panel 170 R2.**
- `t1_two_void.hero` reads `check` 0. The same thing with a struct tag, `t2_two_box.hero`, reads 1 with `duplicate_tag`.
- Panel 171's warden kept the clause as *a live rule* (`171-reports/spec-warden.md:192-195`), without its exception.

**F3. A handle cannot be written for every pointer C hands out.**
- `void *` works: `s1_void.hero` catches the double release, 134.
- `int *` does not: `tag int` is refused with `reserved_word` and `expected_declaration`, and `tag void` over an `int *` result is refused by clang with `ffi_return_type`.
- A `const char **` out-cell does not either: `ffi_parameter_type`.

**F4 (not asked, one line).** `owned free_out` over a `const char **` out-parameter (`s4_cstr_owned.hero`) exits **2** with `internal error: compiling the generated C failed` instead of an `ffi_*` diagnostic.

**Also a claimant on the headroom.** Panel 170 was ratified *in full* on 2026-09-20, and its item 8 is *spec § 13 owes the handle-only rule, +26 real*. There is no ledger row for it: the ledger's tail runs 164, 166, 169, 171, 173. A search for *handle-only rule*, *owes the handle-only* and *item 8* in `docs/records` and `docs/work` finds nothing.

## Verdicts

### Candidate 1: the appended clause
- `verdict`: **object**
- `section`: design.md §1.6 (the payment rule), §1.2; CLAUDE.md §12
- `spec_token_delta`: **8270 → 8280, +10 real** (+8 vendored)
- `removal`: nothing is offered as briefed, and that is a problem. The same clause merged as **M1** costs +2, because it removes *The live handles are a set, so … on its own*.
- `needed_for_self_hosting`: **no**. `selfhost/` has three files with an `extern` group and **0** handle-marked functions; I counted with `awk`, single-line signatures only.
- `argument`: The ratified sentence already binds the compiler. Panel 148 R2 chose the named mark because the bare word *leaves the mismatched-deallocator class open*, so under CLAUDE.md §12 defect 075 is the compiler's bug at zero spec cost, which is how panel 149's warden read *owes* (`149-reports/spec-warden.md:116`). Under §1.2 the clause changes nothing a reader writes: they already call the named releaser, and under route A the abort's own text carries the fix (§4.17). The clause also depends on the route: under B or C the crossing is refused at `check`, and *aborts* is the wrong verb. And it adds a second promise to a set whose first promise is measured false (F1). Nothing pays for it.
- `prediction`: if it lands as M1, the landing commit reads **8272 ±2** real.
- `condition`: I move to approve if the llm-ergonomist measures that readers of the current sentence reach for the other releaser at a rate the clause lowers, and it lands as M1 rather than appended.

### Candidate 2: nothing for Q1
- `verdict`: **approve**, if the route is A or A with B
- `section`: CLAUDE.md §12; `.claude/rules/spec-shape.md` (no section lists every abort, panel 087)
- `spec_token_delta`: **0**, exact (digest `5b96ebe84dac6784` unchanged)
- `removal`: none owed
- `needed_for_self_hosting`: no
- `argument`: Zero tokens, exactly: the document does not move and the digest stays `5b96ebe84dac6784`. The sentence states what the program owes, and the repair makes the compiler collect it, which is what panel 148 ratified the named form for. `.claude/rules/spec-shape.md` says no section lists every abort, and the runtime already has abort sites the document does not name, so an unnamed abort is the document's normal state rather than a gap. It holds only where the route meets the sentence on every path: A, or A with B. B alone leaves the crossing silent across functions. C changes *two records may not name one tag*, so under C the answer is not *nothing*.
- `prediction`: under route A the four defect-075 reproducers go from run 0 to 134, and the spec digest does not move in the repair commit.
- `condition`: I move to object if the route is B alone or C alone.

### Candidate 3: the `ptr` sentence
- `verdict`: **veto of this wording**, not of the subject
- `section`: design.md §1.0 (Principle 0), §1.12 (*does not suspend Principle 0*), §1.2
- `spec_token_delta`: **8270 → 8310, +40 real** (+30 vendored)
- `removal`: nothing, and that is a problem
- `needed_for_self_hosting`: no
- `argument`: Principle 0 is unmet. The compiler binds no handle-marked `extern`, no measured Part 11 effect exists, and §1.12 chooses between admitted shapes, not sentences. The wording is also false where it was tested. *Is a handle* cannot be written for `int *` (`tag int` is refused, and clang refuses `tag void`) or for a `char **` out-cell (`ffi_parameter_type`). *A `ptr` given back twice* leaves out the brief's own `b_out`, which is a `cstr`. *Nothing catches* is false on both Linux legs, where glibc prints the double free (the brief's table). And the promise it leans on, that a handle catches the double release, fails once the address is reused (F1). A false sentence in the prompt adds rewrites.
- `prediction`: a reader following it for an `int *` gets a `check` exit 1; this was measured in the adjacent probes.
- `condition`: I lift the veto for a sentence that is true across F1 to F3 and on Linux, priced with `--refresh`, and carried by a measured reader effect.

### Candidate 4: nothing for Q2
- `verdict`: **approve**, with T1 and R1c owed regardless
- `section`: design.md §1.6; CLAUDE.md §12
- `spec_token_delta`: **0**. The owed corrections R1c+T1 measure **+3 real**.
- `removal`: R1c's removal of *The live handles are a set, so … on its own* is inside that +3. The rest is paid by the prediction below.
- `needed_for_self_hosting`: no
- `argument`: No sentence about `ptr` is owed. From *the live handles are a set*, a reader already learns that handles are what is counted, and the `unread_mark` note already teaches `tag void` at the moment a reader marks a `ptr` (measured, `s5_ptr_consumes.hero`). What § 13 owes near this is two corrections, and neither is Q2's addition. *Two records may not name one tag* is false at `tag void` (F2), and it is the sentence that tells a reader a second `void *` family can be its own handle: T1, +4. *Giving one back twice aborts on its own* is false after reuse (F1): R1c, −1. Q2M2 (+12) has no measured effect behind it, so I do not recommend it.
- `prediction`: see the registered prediction below.
- `condition`: I move to object if T1 and R1c are not scheduled, because then *nothing* leaves two false sentences standing in the paragraph.

## The registered prediction

Route A as briefed keeps the releaser beside the address. I predict it takes `popen_darwin`, `oneacq`, `outacq` and `xacquires` from run 0 to 134 on Darwin arm64, and **leaves `u1_handle_reuse.hero` at `check` 0, `run` 0, zero bytes, 5 of 5**.

- **Instrument:** `heroes check`, `heroes build -O0` and five runs.
- **Scored at:** the M-agreed-retention step that lands 075's repair.
- **Falsified if** `u1` aborts, which would mean the repair changed the key and F1 closed with it, or if any of the four still exits 0.

## A route nobody listed

The brief enumerates three places a releaser's identity could live: the checker, the runtime set, and the type system. All three identify a handle by name or by address. A fourth place is the **Heroes value itself**: each acquisition gets a serial that the value carries beside C's pointer, and the set maps serial to releaser. That would close 075 and F1 together, because a reused address gets a fresh serial.

For this to work, a handle must reach C only as a call argument, unwrapped at the call. That is false for a handle stored as a field of a group `record`, which is C's struct and would lose the serial. This is **unmeasured and unpriced**, and a question for the compiler-engineer rather than a premise.

## Unrun

- F1 on Linux and Windows.
- What reading `c` does after its block is freed in silence.
- `--sanitize` on `u1` and `u2`.
- The `spec` suite on R1c alone. Its text is M1 minus three words, so that it passes is an inference.
- Any reader-side effect of any sentence.
- `b_out` and the glibc line, which are the brief's measurements and were not re-run here.

## Files

- Probes: `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/e7e3d0df-8db3-44f2-b628-af9bb1a6080e/scratchpad/175-spec-warden/probe/` (`u1_handle_reuse.hero`, `u1b_then_close_c.hero`, `u2_ptr_reuse.hero`, `t1_two_void.hero`, `t2_two_box.hero`, `s1_void.hero`, `s2_int.hero`, `s2b_int_as_void.hero`, `s3_cstr_consumes.hero`, `s4_cstr_owned.hero`, `s5_ptr_consumes.hero`, `s6_charp_as_void.hero`, `r2.h`)
- Pricing scripts and suite logs: `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/e7e3d0df-8db3-44f2-b628-af9bb1a6080e/scratchpad/175-spec-warden/price/`
- By a slip, a pristine copy of the spec sits outside my directory at `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/e7e3d0df-8db3-44f2-b628-af9bb1a6080e/scratchpad/175-spec-warden-orig-spec.md`. Nothing reads it.
