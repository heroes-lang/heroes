<!-- Written out by the coordinator from the seat's final message, 2026-09-24:
the harness refused the seat's report file. The one mechanical change: the
transport escaped `->`, `<` and `>` as HTML entities, and they are unescaped
here. Nothing else is altered. -->

# Panel 177: completeness critic

I am not a judge and give no verdict. I worked at HEAD `521c5e02`, in my own directory only (`<scratchpad>/177-completeness-critic/`), and did not touch the trunk. I built the compiler from the seed there (3.70 s), plus an `-O2` seed (`heroes-o2`) to rebuild the edited trees (30.4 to 31.8 s each).

I copied the seats' prototypes and programs into `work/ce/` (the compiler-engineer's P, R, M, Mm and B trees and their binaries) and `work/ffi/` (the ffi-pragmatist's programs), and ran them there.

**Legs:**
- **Darwin arm64:** everything.
- **Linux aarch64 and Linux x86_64 (emulated):** the 17 header-model shapes in § 1, at `-O0` and `-O2`, three runs each. The compilers were built on Linux from their emitted C.
- **Real-library programs:** Darwin only (json-c 0.19, SQLite 3.53.4, OpenSSL 3.6.4, all Homebrew).
- **Windows:** unrun.

Every number below comes from a command I ran, unless the sentence says *read* or *carried*. The harness refused a report file, so the running log is `work/DRAFT.md`.

---

## 0. What I built

**`C`, the composite nobody built.** It is the compiler-engineer's `B` (P plus the result-side success clause on `consumes`), plus its `Mm` (M-must), plus **T**, the ffi seat's dead set. T is built into the compiler here, where the ffi seat had it as a patch on emitted C.

**The runtime part** is in `work/C/runtime/parts/alloc.c`:
- A second open-addressed table sits under the live set's own lock.
- `hero_handle_consumed` marks an address dead, and `hero_handle_acquired` clears the mark.
- `hero_handle_alive` is P's check. It is already emitted before every handle a C argument reaches, fields and elements included. It now aborts on P's poison first, then on a dead address, with a `[T]` message.
- Two new runtime calls clear a dead mark:
  - `hero_handle_borrowed`, emitted after every `borrows` result and every `@ … borrows` cell (`emit/handle_traffic.hero`, +6 code lines).
  - `hero_handle_entered`, emitted at the entry of every function C can call back, once per handle parameter (`emit/body.hero`, +14).
- Three environment switches, `HV_NOT`, `HV_NOBORROW` and `HV_NOCB`, turn off T, the borrows clear and the callback clear at run time. One binary can then answer each question.

**The checker part:**
- M-must is hooked in beside B's success rule (+2 lines).
- One added condition: a call carrying `when` is not a definite end (`check/moved.hero:330`).

**Two variants:**
- `CnoM` is `C` without M-must, to see the run-time half alone.
- `Cloc` is `C` with M-must's program-wide `@` summary switched off.

**Size over `B` plus `Mm`:** `selfhost/` +23 code lines; `runtime/` +76 (the switches included) and 2 header lines. `C` checks its own `selfhost/main.hero` clean (exit 0).

## 1. P + T + M-must together (brief item 1): run

| program | base | `CnoM` (P+T) | `C` (P+T+M-must) | legs |
|---|---|---|---|---|
| `read_after_consume` (088) | 0 | 134 before C (P) | **check 1** | 3 |
| `use_after_consume` | 134 stray | 134 (P) | check 1 | 3 |
| `helper_consume_then_use` | 134 stray | 134 (P) | check 1 (through the summary) | 3 |
| `u1_static` (077) | 0, `true` | 134 (P), prints `false` | check 1 | 3 |
| `reuse_malloc` (077) | 0 | 134 (P), `same address: false` | check 1 | 3 |
| `rac_eq` | 0 | 134 (P) | check 1 | 3 |
| ffi's 8 correct real programs (`jsonc_read`, `sqlite_borrowed` ×3, `ossl_pump`, `ossl_cert`, `ossl_store`, `ossl_verify_cb`) | 0 ×8 | 0 ×8 | 0 ×8 | Darwin |
| `cb_reuse` (correct, header model) | 0 | 0 (with the callback clear) | 0 | 3 |
| `sqlite_copies` n=1,2,3 (copies) | 0 | **134 [T]** | 134 [T], check 0 | Darwin |
| `sqlite_copies` n=4 (the binding, via `db.handle` of an `@` cell) | 0 | 134 (P) | 134 (P), check 0 | Darwin |
| `pair_escape` (dead handle in a C struct passed by value) | 0 (garbage on Linux) | **134 (P)** | check 1 | 3 |
| `copy_before` (the compiler-engineer's E13) | 0 | 134 [T] | 134 [T] | 3 |
| `while_twice` | 134 stray | 134 (P) | check 0, then 134 (P) | 3 |

**Over the 510 files:**
- **`check` under `C`:** exactly two verdicts move, from 0 to 1: `run/abort-handle-given-back-twice.hero:29:16` and `run/fixedbugs-a-real-deallocator-given-the-same-handle-twice.hero:52:18`. No other `--brief` line changes. Under `CnoM`, nothing moves.
- **`run` suite** (built with the net's own binary):
  - `C` reads 136 passed, 2 failed: those two now fail to build.
  - `CnoM` also reads 136 and 2: the same two, now with P's message instead of the set's.
  - **T moved no other `run` golden.** `ffi-borrows-owes-nothing` and `abort-handle-borrows-that-gives-away` both pass.
- **`corpus`:** 55 passed, 0 failed under both. I did not re-run base `run`; its 138 and 0 is the compiler-engineer's E9, carried.

**Time per call, run.**
- The program is `loop_100000000` at `-O2`, six interleaved rounds.
- I discarded round 1, where `real` read 0.68, 0.49 and 0.83 against a much lower `user`. In rounds 2 to 6, `real` equals `user` plus `sys`.
- The machine was not idle: VS Code and `mds_stores` held about 110 % on other cores, and none of my processes ran.
- Results: base 0.08 s, P 0.13 s, **P+T 0.47 s**, five of five.
- So T adds **about +3.4 ns per handle argument over P**. That is R's price (the lock and a probe), roughly seven times P's +0.5 ns. The ffi seat counted T's probes but did not time them.

**Memory, run.**
- The program is `many.hero`: one million handles acquired, then released, none reused, measured three times with `/usr/bin/time -l`.
- Peak memory: base 66.8 MB, P 66.8 MB, **P+T 84.6 to 87.8 MB**.
- The dead set never shrinks unless C hands those addresses out again. Nobody priced either cost.

## 2. T's false abort, and the repair (brief item 2): run

**The borrows half of the repair is not new.** It is already in the ffi seat's emulation, `exp/patch.py` (read): `if 'T' in modes and fn in borrowing and res: post.append(... hv_t_alive(res))`, and the same for `@` out cells. The ffi seat's report never says so, which is why the brief lists this repair as unpriced.

**It carries weight on a real library, not only on a header model.** With both clears off (`HV_NOBORROW=1 HV_NOCB=1`), `C` aborts **`ossl_pump` with 134 [T]**. That program is real OpenSSL: `SSL_get_wbio` hands back the BIO that `SSL_set0_wbio` consumed. The other seven real programs still run at 0.

**The json-c shape.**
- The program is `q2t/jsonc_same_addr.hero`, and it is correct. A child is transferred into a parent through `consumes`, then read back, borrowed, through `json_object_object_get`.
- With `HV_NOBORROW=1` it aborts: **134 [T] three of three**. With the clear it runs at 0, three of three.
- **No allocator reuse is needed:** it printed `same address as the freed int: false`. The program's own transferred child comes back at its own dead address.
- The brief's exact shape did **not** reproduce in two attempts (`jsonc_parse_addr`, `jsonc_parse2`). That shape is json-c freeing one object and its tokener then allocating a child at that address. In both attempts the root took the freed address and the child did not. It stays a question, not a result.

**The callback half closes `cb_reuse`.** With `HV_NOCB=1` it aborts 134 [T] (Darwin); with the clear it runs at 0 on all three legs.

**It also opens a hole nobody priced.**
- The clear sits at the function's entry, the same place as the guard in `callback_guard.hero`, because in Heroes a function value is its C address. So the clear also fires when Heroes code calls the function directly.
- The program `cbt/cb_launder.hero` hands a stale copy to `seen`, a function whose address is also given to C. It **runs 0 on all three legs under `C` and reads the freed word**: `2` on Darwin, garbage on Linux.
- Controls, on Darwin:
  - With `HV_NOCB=1`, the same program aborts 134 [T].
  - The same stale copy passed to a function whose address is never taken (`cb_nolaunder`) aborts 134 [T] either way.
  - Base with `--sanitize` reports `heap-use-after-free`.
- So the repair **launders** every stale copy that reaches a callback-capable function through an ordinary call.
- A thunk used only at the address handed to C would narrow the hole to calls through function values (argued, unbuilt).

## 3. A4 and the receiver rule (brief item 3): run on emulation

**A4 is confirmed.** I used panel 176's critic's V1c emulation: its `heroes-176` emission and its runtime with `hero_handle_meet`, with one emitted line edited.
- The relabel `fclose(stream: File transfers fclose)` on a `popen` stream aborts 134 before C, three of three, as panel 176 found.
- **The ergonomist's relabel, `transfers pclose`, runs 0 0 0.** It prints `fclose on a popen stream: 0`, says nothing, and `pclose` is never called.
- Panel 176's critic called this shape *"a deliberate lie"*. The ergonomist wrote it as a colleague's first repair.

**The ergonomist's repair, as worded, breaks the one real transfer that justified V1c.**
- The rule says *"a call with no other handle parameter cannot transfer"*. That refuses `BIO_new_fp(stream: File transfers fclose, close_flag: i32) -> Bio acquires BIO_free`, whose receiver is its RESULT. `BIO_s_file(3ssl)` (read): *"Setting the BIO_CLOSE flag calls fclose() on the stream when the BIO is freed"*.
- I re-ran panel 176's emulations: `e1_fp_popen_V1` (a popen stream into `BIO_new_fp`) aborts **134 before C**, three of three; `e2_fp_fopen_V1` runs 0, three of three.
- The rule forces the binding back to `consumes`. That is `e5_fp_popen_consumes`, which runs **0 on base and 0 under `C`**, with `pclose` never called.
- A rule that keeps both results: **the receiver is another handle parameter or the acquiring result**. Under it, `fclose` (which returns `i32`) still cannot transfer. Argued, unbuilt.

**Census, read.**
- I grepped OpenSSL 3.6.4's headers for `set0|add0|push0|assign` and got 121 declaration lines.
- Of the first 80 printed, every one names a receiver pointer first, with three exceptions:
  - `EVP_PKEY_asn1_add0` and `EVP_PKEY_meth_add0` take one parameter and transfer into a global table; both are deprecated.
  - `OSSL_LIB_CTX_set0_default` is not a transfer.
- All the functions in the ffi seat's Q2 table have a receiver parameter.
- Outside that table, three real transfers have their receiver in the result: `BIO_new_fp`, `curl_slist_append` (its man page, read) and `realloc`.

## 4. The success clause (brief item 4): run where a tree exists

**The compiler-engineer's claim about `consumes` holds on real SQLite 3.53.4.**
- The program is `busy/close_busy.hero`: close with a statement still open (returns `SQLITE_BUSY`, 5), finalize, close again (returns 0).
- Without the clause: base 134 stray, P 134, and `C` refuses it at check.
- With `-> i64 when 0`: **0 three of three** under `CnoM` and `C`, printing `first close: 5 second close: 0`.

**The clause composes with P, T and M-must.** The program is `succ/jsonc_success_B.hero`: real json-c, `put_idx … -> i32 when 0`.
- n=0 (success): 0.
- n=1 (failure, the program frees the child): 0, and check passes. The `when` exemption in M-must is what makes this correct failure path legal.
- n=2 (failure, nobody frees it): 134 at exit.
- With the value written wrong (`when 1`), n=0 aborts 134 with *"never given back"*. That matches the ffi seat's prediction 4.
- The compiler-engineer's six success shapes and `two_transfers` give the same verdicts under `C` as under `B`.

**Is there one spelling that covers all three? No text priced at this sitting does.**
- `B` conditions `consumes` only; its tree has no `transfers` and no `retains`.
- The spec-warden's recommended beta-ext sentence says *"nothing is transferred or retained"*. It never mentions an END, so it does not cover `sqlite3_close`.
- A result-side `when` governing every end, transfer and reference of the call is unwritten and unpriced. It must not govern acquisitions, because `sqlite3_open` hands a handle back even on error.

**Neither placement can write a success that is a non-null result, and one sits in the ffi seat's own census.**
- `OCSP_request_add0_id` returns `OCSP_ONEREQ *` and succeeds when that is non-NULL (ffi table and header, read). `curl_slist_append`, `BIO_new_fp` and `realloc` have the same shape.
- Both priced grammars take only `integer | true | false`. `B` refuses `when` on a handle result with `success_value_type` (run).
- My model, `slist/slist_fail.hero`, has a correct failure path that frees the old list:
  - base: 0 on success, but **134 stray on the correct failure path**, so today's language already cannot bind it;
  - P: 134 on the failure path;
  - `C`: refused at check, success run and failure run alike.
- So the clause as adopted leaves a class of real conditional transfers unbindable.

**The zero-token alternative, run.**
- The program is `shim/jsonc_shim.hero`: a header that frees the child when json-c refuses it, with the binding written `val: Json consumes`.
- It runs 0 on success and on failure, on base and on `C`, and base `--sanitize` reads 0 on both.
- It works for json-c. It cannot let a program keep the child after a failure. SQLite already ships its own unconditional `sqlite3_close_v2`. This is CPython's recorded move, the historian's `PyModule_Add`.

**Unrun:** `retains` has no tree, and no seat has run `X509_up_ref`'s failure path.

## 5. M-must against H14, and §4.4 (brief item 5): run

| program | M (may) | M-must | P at run time |
|---|---|---|---|
| `h14_revive` (a write revives the name) | 0 | 0 | 0 |
| `b2_correct` (the ergonomist's loop) | 0 | 0 | 0 |
| `b2_buggy` | **1** | **0** | 134 (3 legs) |
| `b1_fallthrough` (the ergonomist's forgotten `return`) | **1** | **0** | 134, on the failure path only (3 legs) |

A write does revive the name, and M-must refuses no correct B2 loop. But it **catches neither the buggy loop nor the program the ergonomist called II's strongest case**; M-may catches both at check. That is the price of the compiler-engineer's *"0 false refusals"*, and nobody stated it.

**M-must's verdict is not local.**
- The directories `xmod/one` and `xmod/two` differ only in the BODY of `finish(@j: Json)` in `lib/tidy.hero`. The signature is the same.
- The same line, `main.hero:9`, is **refused at check in one and accepted in the other**.
- The cause is the program-wide `@` summary: a caller's verdict depends on a callee's body in another module. That is the ground on which panel 176 refused the call-site rule (its item 5). The ergonomist was never shown it.

**`Cloc`, the same analysis with the summary off** (intraprocedural, which is Cyclone's scope):
- 4 of the 5 reproducers are refused at check. `helper_consume_then_use` moves to P, at run time.
- Both `xmod` programs pass check.
- The same 2 of the 510 files move.

**What §4.4 requires (read).**
- I grepped design.md for `dataflow`, `flow analysis`, `flow-sensitive`, `move check`, `typestate` and `definite`. The hits are lines 520, 1080-1081 (§4.4) and 1342 (§4.8's alias test).
- No Part 6 row refuses a flow analysis. §4.4 says mandatory initialisation deleted an analysis of *"the same family as move checking"*. That is true of initialisation, and it stays true.
- What M-must falsifies are the checker's own comments (`check/consuming.hero:22`, `check/leasing.hero:29`, `check/acquiring.hero:25`, `check/freer.hero:12,24`) and Part 8 wart 20. A landing corrects them in the same commit.
- The requirement that binds harder is locality, above.

**A defect in the prototype.** M-must's note, as the `run` suite prints it, says *"on at least one path that reaches this line the value's life has ended"*. That is M-may's wording; M-must fires only when the life has ended on every path.

## 6. P's conditions reconciled (brief item 6): run

| condition | whose | needed for soundness, or only better | evidence |
|---|---|---|---|
| poison a PLACE | the coordinator's reading of the ffi seat | needed, and **already met** by the compiler-engineer's P | `sqlite_copies` n=4 (`db.handle` through `closed(@db)`) is caught by P alone |
| check at every READ | same | **not needed as stated**: P already checks every handle a C argument reaches, fields and elements included | `pair_escape` aborts 134 under the compiler-engineer's own `heroes-P` (Darwin) and under `CnoM` (3 legs). The ffi seat's *"P leaves `pair_escape` at 0"* describes its emulation, which checks only direct handle parameters |
| **check at every place a value crosses into C** | nobody | **needed for soundness** | callback RESULTS are unchecked. `cbret/cb_return_poison64.hero` returns P's dead cell to C, and C writes 64 bytes into a 16-byte `max_align_t` runtime static: **exit 0 under `C` on 3 legs, and 0 under `C --sanitize`** (Darwin), where base `--sanitize` reports `heap-use-after-free`. P turns a use-after-free that ASan catches into a write ASan cannot see. `@ … borrows` cells: nobody has run them |
| the dead value on a page of its own | compiler-engineer (3) | needed while any crossing stays unchecked; better as a PROT_NONE page, so a write faults | the run above. MSVC's 0x8123 (the historian, verified-weak) is, by my inference, an unmapped address for this reason |
| a ruling on `==` | compiler-engineer (1), spec-warden | needed for the spec to stay true (§ 5's *binds once*, § 13's *compares the address*), not for memory safety | `eqb/eq_branch.hero`: two `=` bindings freed on one branch compare **true** under `C` and false on base, and check passes. M-must does not make this moot |
| `@` in/out cells | compiler-engineer (2) | needed but narrow: an unmarked `@` handle of a consumed type is already refused (`unmarked_handle_producer`, run), so only `@ … borrows` remains | unrun |
| composing with the success clause | ffi (c) | needed, to avoid refusing correct programs | `close_busy` and `jsonc_success_B`, § 4 |
| the set's end on the IR's success edge | compiler-engineer (4) | engineering, not soundness | `B` runs correctly with the comparison written twice |

## 7. Contradictions between seats, and which side the runs support

1. **P and `pair_escape`.** The ffi seat says P misses it and that no route catches a handle inside a C struct. The compiler-engineer's P walks `reaches.handle_suffixes`. **The runs support the compiler-engineer**, on three legs. What no route catches is C *keeping* a struct that holds a handle across the release, as SDL's bindings do: argued, unrun.
2. **S.** The historian approves, the spec-warden calls it admissible, the ffi seat vetoes and the compiler-engineer objects. **The runs support the ffi seat.** I re-ran its C on Darwin:
   - the naive callback is a clang `incompatible function pointer types` error;
   - the trampoline build exits 0 printing `strict: error 18`, meaning the wrong callback ran;
   - `s_struct` is a clang error.
   
   The historian's own sources say every S precedent either owns its handle table or unwraps at the boundary, in development only.
3. **M's join rule.** The historian approves M with a join rule, citing Cyclone's union (the may rule). The compiler-engineer rejects may and approves must. Both measured halves are true (§ 5): may refuses two correct programs that have a local rewrite, and must lets the ergonomist's two wrong programs through to run time. Nobody cited a precedent for a must join.
4. **α against β.** No run can settle it beyond `B` existing while α is unbuilt; the ergonomist's P1 needs 20 fresh readers (unrun). Both placements miss the non-null success shape (§ 4).
5. **The spec-warden's P4, scored inside the sitting** (read, not run). P4 says: *"if [the reader under I] predicts that the program reaches C, D is not misleading, D+ has bought nothing, and its +20 is unpaid."* The ergonomist's B1 row for I reads: *"No Heroes abort … The program prints, exits 0 … Silent."* By P4's own terms, D+'s +20 is unpaid.
6. **T's *"no false refusal on the 8 real programs"*** is true only with the borrows clear that the ffi seat's emulation carries and its report does not mention. Without that clear, `ossl_pump` aborts (§ 2).

## 8. Routes nobody listed

1. **T under `--sanitize`**, meaning the dead set plus ASan's quarantine (run).
   - The program is `sqr/sqlite_copy_reuse.hero`, real SQLite: a copy of a closed connection is used after a second open has reused its address.
   - Base runs 0 and prints `same address: true`: the stale copy reads the NEW connection.
   - `C` without sanitizing also runs 0.
   - **`C --sanitize` aborts 134 [T] before C.**
   - **Base `--sanitize` runs 0**, because ASan is blind inside the uninstrumented library.
   - `erg/copy_reuse.hero` behaves the same, and on both Linux legs too.
   - This closes the copy class in sanitize builds for any library that uses the system allocator. It would make Part 8 wart 20's *"The remedy today is `--sanitize`"* true again for real libraries.
   - Its limits: static cells (077's `u1_static`), library-private allocator pools, and sanitize builds only.
2. **M-must, intraprocedural** (`Cloc`, run, § 5): local, and Cyclone's scope.
3. **A receiver that is another handle parameter or the acquiring result** (§ 3): argued.
4. **A success value that is a non-null result**, or the spec-warden's question, *"a transfer into a result that comes back null is not made"*: argued. § 4 shows four real functions that need it.
5. **The header shim** (run, § 4): zero spec tokens.
6. **A check on callback results**, one `hero_handle_alive` on the value a callback-capable function returns (argued). And **the callback clear placed in a thunk** at the address handed to C (argued).
7. **M-may with a `certain` Fix** for the correlated shapes, whose rewrites are local (read: `flow/if_correlated.hero`, `match_correlated.hero`). Unpriced. Heroes has no warning class to put a may-moved finding in: grepping `selfhost/diag.hero` for severity or warning finds nothing.

## 9. The questions the sitting should have asked

1. **Where does a handle cross between Heroes and C, in each direction?** P and T are exactly as sound as that list, and nobody enumerated it.
   - Into C: arguments are checked; callback results are not (§ 6); `@ borrows` cells are unrun.
   - Into Heroes: acquisitions, `borrows` hand-backs and callback parameters, which are T's clears. A clear at a Heroes function's entry cannot tell C's call from Heroes' own (§ 2).
2. **Is M-must's verdict local?** No (§ 5).
3. **Does P keep what `--sanitize` catches today?** No, on the callback-result shape (§ 6).
4. **What does "success" mean when the result is a pointer?** (§ 4.)
5. **Does the repair of A4 keep V1c's one real true positive?** Not as worded (§ 3).

## 10. For every recommended route: what it was run against, and what was only argued

| route (who recommends it) | RUN against | only ARGUED against |
|---|---|---|
| **P** (compiler-engineer, ffi, historian) | Compiler-engineer: the 5 reproducers on 3 legs, the 510 checks, `run` and `corpus`, `eq`, 7 flow shapes, `copy_before`, 2 borrows shapes. Me: `pair_escape`, `sqlite_copies`, `eq_branch`, `cb_return` and its two poison variants, `close_busy`, `slist_fail`, `b1_fallthrough`, `b2_*`, and 17 shapes on 3 legs | `@ borrows` cells, the poison page, the IR success edge, threads |
| **M-must** (compiler-engineer) | Compiler-engineer: the 5 reproducers, the 510, 7 flow shapes, check time. Me: with `when` (built), the ergonomist's four programs, `xmod` (not local), `sqlite_copies` (it misses a field ended through an `@` helper), `slist_fail`, `close_busy` | a `match` arm returning early inside a loop; cycles in the cross-module summary |
| **T** (ffi) | ffi seat: as a C-patch emulation over 8 real programs and 4 models. Me: as a compiler, over the 510, `run`, `corpus`, time, memory, `cb_launder`, `jsonc_same_addr`, `copy_reuse`, `sqlite_copy_reuse` and `--sanitize`, with the models on 3 legs | threads; a set that shrinks; composing with `retains` (the ffi seat's `hv_release_t`, emulated only) |
| **success clause β** (compiler-engineer, ffi, spec-warden) | Compiler-engineer: 6 shapes plus `two_transfers` and `void_when`, all on `consumes`. Me: real json-c and real SQLite; `slist`, which has no spelling | `transfers` and `retains` (no tree); `X509_up_ref`'s failure; a success range such as `> 0` |
| **α** (ergonomist) | nothing | everything |
| **`retains`** (all seats) | the ffi seat's emulation only | its failure path; composing with T |
| **receiver rule** (ergonomist) | Me: what it would lose, through the V1c emulation (`e1`, `e5`) | the rule itself |
| **D+** (spec-warden) | tokens only; HEAD's behaviour is the brief's table | nothing further |
| **S** (historian) | the ffi seat's C shapes, re-run by me on Darwin | S as a compiler, by everyone |
| **A** (historian) | nothing: Part 6's falsifier is still unmet | everything |

## Unrun

- Windows, all of it.
- Real-library programs on Linux.
- T and M-must under the compiler's own tests (`heroes test`).
- Any spec text for the routes in § 8.
- The brief's json-c tokener-reuse shape (not reproduced).
- `@ borrows` in/out cells.
- Threads.

Nothing I ran contradicts the coordinator's *known* list. `sqlite_copy_reuse` is one more instance of the ffi seat's finding 2: base `--sanitize` exits 0 on a use-after-free inside libsqlite3.

## Files

Everything is under `<scratchpad>/177-completeness-critic/work/`:
- `DRAFT.md`, the running log.
- The composite trees: `C/`, `CnoM/` and `Cloc/`, with their compilers; `C/` and `CnoM/` also hold their emitted `compiler-*.c`.
- Diffs: `C-over-B.selfhost.diff`, `T.runtime.diff`.
- Scripts: `run1.sh`, `libs.sh`, `sweep-check.sh`, `sweep-run.sh`, `linux/legs.sh`.
- Results: `check-*.txt`, `brief-*.txt`, `suite-*-*.txt`, `linux-arm64.clean.txt`, `linux-x86.clean.txt`, `timing/times.txt`.
- Program directories: `repro/`, `ffi/`, `q2t/`, `cbt/`, `cbret/`, `succ/`, `flow/`, `erg/`, `busy/`, `slist/`, `shim/`, `sqr/`, `xmod/`, `eqb/`, `a4/`.
