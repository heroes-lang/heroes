# Panel 176: completeness critic

Not a sixth judge, and no verdict. Everything below was run on 2026-09-23 in
`<scratchpad>/176-completeness-critic/`: a `git archive` of `a747e5a2` (the
trunk's HEAD, `git log -1` read before the copy) plus the untracked
`docs/panel/176-briefs/`. In that directory I built:

- `heroes`, from the seed (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`), 3.45 s real;
- `heroes-176`, the compiler-engineer's `work/prototype-176-final.diff` applied to a
  copy of `selfhost/` and `runtime/` (`P/`, patch applied cleanly, 19 files) and built
  with `./heroes build selfhost/main.hero`, 78.45 s real;
- `heroes-n1`, `heroes-176` plus a one-line narrowing of `contract_differs` (§ 5), 75.34 s;
- `heroes-q4`, the engineer's `work/prototype-176-q4.diff` on a clean `selfhost/`, 76.14 s;
- `P2/runtime`, `heroes-176`'s runtime with one 12-line change to `hero_handle_begun` (§ 4).

"Three runs" means the binary ran three times and every run gave the number
shown. `check`/`run` rows read `exit(stdout bytes/stderr bytes)`. Linux runs used
`heroes-linux-arm64` and `heroes-linux` (x86-64, emulated), with my `w/linux/`
directory mounted read-only. Inside each container a compiler was built from
`heroes-176`'s own `--emit-c` of the prototype's `selfhost/main.hero`: 26,487,900
bytes, the fixpoint size the engineer reports. It builds in 4 s on arm64 and 6 s on
x86-64. **Windows is UNRUN.** Nothing in the trunk was built, run or written except
this file.

## Before and after the interruption

The coordinator reported a network error while my first container run was
starting. That run's output did reach me, but only its last 40 lines, because I
had cut it with `tail -40`. So I reran it after the resume and kept all of it.

- **Before the interruption:**
  - the three builds above (seed, `heroes-176`, `heroes-n1`);
  - § 0's reproduction;
  - on Darwin, every row of § 1, § 2, § 3 and § 6, and the rows of § 4 and § 5
    except those listed below;
  - the 510-file sweep;
  - the C programs `fp_zombie.c` and `sig_same.c`;
  - the `_up_ref` and `examples/` counts;
  - the `--emit-c` of the prototype for the containers (91.07 s).
- **After the resume:**
  - the full Linux arm64 run (`w/linux/arm64.txt`);
  - the Linux x86-64 run (`w/linux/x86.txt`). The first attempt, with `--platform
    linux/amd64`, exited 125 without starting; the rerun without the flag ran;
  - reading the spec-warden's C1 text;
  - § 7's use-after-consume rows;
  - § 4's `retains … | …` set variant and `rc_after_release`;
  - the grep of `heroes_runtime.h` for a borrowed record;
  - `sig_same.hero` in Heroes;
  - the `heroes-q4` build and § 7's typedef rows;
  - § 5's `realpath` static-buffer row.

No measurement taken before was retaken after, apart from the arm64 run: it
matches the 40 lines I saw, row for row.

---

## 0. The prototype I judged is the prototype the engineer judged

- The brief's eight programs, stock `heroes` against `heroes-176`, three runs each,
  reproduce the engineer's § 1 and § 2 tables **exactly, byte counts included**:
  - `popen_fopen` goes from 0 to 134 (223 B, before C);
  - `vk_cross`, `xfer_cj`, `xfer_jsonc` and `xfer_ssl` are `check` 1,
    `unadmitted_release`;
  - `refcount` and `getter_wrong_repaired` stay at 134 (396 B).
- `heroes-176 --emit-c` and the engineer's own `heroes-final` binary (copied, not
  run in their directory) give byte-identical C on three programs I wrote
  (`cmp`: 10,846, 13,132 and 12,190 B).
- **The 510-file sweep** ran `check --brief` over every `.hero` in `tests/golden/`
  and `examples/`, with stock against `heroes-176` (`w/sweep_*.txt`). The two
  differ on **exactly one file**, `tests/golden/run/abort-handle-borrows-that-gives-away.hero`,
  from 0 to `unadmitted_release`. `contract_differs` fires on none. The engineer's
  prediction therefore already holds of the prototype, and at the landing it tests
  only that what lands is this prototype.

---

## 1. An unchecked transfer is a new way to silence defect 075: **yes, and the diagnostic invites it**

`transfers` is emitted as `hero_handle_consumed(h, NULL, NULL)`
(`e1_fp_popen_V1.c:179`). The runtime skips the family check whenever `by` is NULL
(`P/runtime/parts/alloc.c:517`). My programs are in `w/q1/`.

| program | what it is | `heroes-176` Darwin | Linux arm64 | Linux x86-64 |
|---|---|---|---|---|
| `c_popen_fclose_consumes` | defect 075 as filed: `popen`, then `fclose` marked `consumes` | **check 1**, `unadmitted_release` | | |
| `a_popen_fclose_transfers` | the same, with `fclose(stream: File transfers)` | **check 0, run 0 (28 B / 0 B)** | **0** | **0** |
| `b_popen_fopen_fclose_transfers` | the same, plus `fopen … acquires fclose` in the module | check 1, `unread_releaser` (*"`fclose` is marked `transfers`"*) | | |
| `xmod_t/` | the relabel in `main`, `fclose … consumes` in another module | check 1, `contract_differs` | | |
| `d_vk_cross_transfers` | `buf_destroy_pooled` relabelled `transfers` | **check 0, run 0** | **0** | **0** |
| `c2_popen_fclose_set` | `popen … acquires pclose \| fclose`, the note's other repair | check 0, run 0 | | |

- **The relabel is what the new diagnostic's own note offers first.**
  `unadmitted_release`'s note (`P/selfhost/check/consuming.hero:196`, printed on
  `c_popen_fclose_consumes`) reads *"if `fclose` hands the `File` into another value
  that ends it later, mark its parameter `transfers`; if it is one more way to end
  what a call begins, name it in that call's mark"*.
  - Both of its repairs, applied to defect 075 itself, give a silent 0.
  - `--json` shows `"fixes": []`, so neither is machine-applied. A model that
    follows the first sentence of the note still silences the defect.
- **What still catches it:** another declaration naming `fclose`, in the same
  module (`unread_releaser`) or in another one (`contract_differs`). Defect 075's
  own shape, a program whose only `FILE` producer is `popen`, has neither.
- **An honest transfer carries a wrong release too.** OpenSSL's `BIO_new_fp(FILE
  *stream, int close_flag)` is at `openssl/bio.h:729`, with `BIO_CLOSE` at `:85`.
  `e1_fp_popen_V1` marks the stream `transfers`, hands it a `popen` stream with
  `BIO_CLOSE`, then calls `BIO_free`:
  - **check 0, run 0**, three of three, on all three platforms;
  - in C (`fp_zombie.c`, Darwin, three runs), `BIO_free` leaves a child nobody
    waited for: `waitpid(-1, …, WNOHANG)` returns its pid, and the `pclose`
    control returns `-1 (No child processes)`.
  - So this is defect 075, reached through a call that really is a transfer.

**Two things this falsifies, both checkable:**

- **The engineer's § 3 says of V2** *"I could not construct a program where V2's
  extra check catches something true"*. `e3_fp_popen_V2` does: `fopen … acquires
  fclose | BIO_new_fp` and `popen … acquires pclose` give **134 before C** (227 B,
  all three platforms), where V1 (`e1`) is a silent 0. `e4_fp_fopen_V2`, the same
  program over an `fopen` stream, runs at 0. That is the engineer's own condition
  for revisiting *V1 over V2*: a transfer valid for only some acquisitions of its
  type.
- **The ffi seat's *"V1, V2 and V3 give the same verdict on all 16 programs"*** is
  true of its 16, and of complete V2 sets. `e1`/`e3` is a 17th program where V1 and
  V2 differ.

### The route nobody built: V1c, the historian's

The historian proposed that *the transfer names the releaser it hands the life
towards, checked against the handle's set before C runs* (the historian's
report, *Risk 1*). It was argued from Clang's `ownership_holds` and built by
nobody.

**The runtime already has the mechanism.** `hero_handle_consumed`'s third argument
`as`, which the prototype added for `freopen`, is compared with `hero_handle_meet`
(`alloc.c:442`). I emulated V1c by editing only the transfer's emitted line in the
C that `heroes-176` produced: `hero_handle_consumed(h, "<transfer>",
"<releaser>")`, one line per program, compiled at the project's sixteen flags with
0 errors.

| program | the line becomes | run ×3, Darwin |
|---|---|---|
| `xfer_cj_t` | `(h, "cJSON_AddItemToObject", "cJSON_Delete")` | 0 (24 B / 0 B) |
| `xfer_jsonc_t` (real json-c) | `(h, "json_object_object_add", "json_object_put")` | 0 (21 B / 0 B) |
| `xfer_ssl_t` (real OpenSSL) | `(h, "SSL_set0_rbio", "BIO_free")` | 0 (46 B / 0 B) |
| `e2_fp_fopen_V1` | `(h, "BIO_new_fp", "fclose")` | 0 (41 B / 0 B) |
| `e1_fp_popen_V1` | the same | **134 before C** (0 B / 227 B) |
| `a_popen_fclose_transfers` | `(h, "fclose", "fclose")` | **134 before C** (0 B / 223 B) |
| `d_vk_cross_transfers` | `(h, "buf_destroy_pooled", "buf_destroy_pooled")` | **134 before C** (0 B / 240 B) |

- **V1c gives V2's true positive at V1's cost profile**: one releaser name per
  transfer function, not one per acquirer. It closes the relabel, and it changes
  0 runtime lines.
- **The compiler side is UNRUN.** A named `transfers <releaser>` would reuse
  `acquires_marker(word:)` and `releaser_reads`. My guess is a few tens of lines,
  and it is not a measurement.
- **What V1c cannot stop is a deliberate lie**, `fclose(stream: File transfers
  pclose)`. That is the same standing as any false mark.

---

## 2. The conditional transfer: **the prototype does what `hv.c` does; a success-only spelling runs**

The ffi seat's two programs with `consumes` changed to `transfers` (`w/q2/`, real
json-c 0.19, `json_object_array_put_idx(arr, SIZE_MAX, child)`, which returns -1):

| program | the C | stock (`consumes`) | `heroes-176` (`transfers`) |
|---|---|---|---|
| `jsonc_failed_add` | the add fails and the caller frees the child: **correct** | 134 (20 B / 396 B) | **134** (20 B / 396 B), the stray message |
| `jsonc_failed_leak` | the add fails and nobody frees the child: **a leak** | 0 | **0** |

So the prototype ends the obligation before the call, unconditionally, exactly as
`hv.c` does: it refuses the correct failure path and runs the leaking one.

**An emulated spelling that ends the obligation only on success.** After the C call
I inserted `if (<result> != 0) hero_handle_acquired(h, "json_object_put");`,
which does not touch what C receives (`w/q2/*.cond.c`):

| program | run ×3 |
|---|---|
| `jsonc_failed_add_t` (correct) | **0** (20 B / 0 B) |
| `jsonc_failed_leak_t` (the leak) | **134** at exit, *"1 C handle(s) never given back"* (20 B / 166 B) |
| `jsonc_ok_add_t` (`idx: 0`, the add succeeds) | 0, `put_idx 0 length 1` |
| `xfer_jsonc_t` (`json_object_object_add`) | 0, `add 0 fields 1` |

**What a real spelling needs, and nobody has priced it:**

- **It must name the success value**, because the libraries disagree:
  - json-c returns 0 on success;
  - OpenSSL's `add0`/`set0` return 1 on success (the ffi seat quotes the pages);
  - cJSON returns true;
  - `SSL_set0_rbio` returns `void` and transfers unconditionally.
- **It must restore the entry's own releaser set on failure.** My emulation writes
  the set by hand, and the runtime does not keep an ended entry.
- **It must stay inside the ffi seat's veto line.** It rewrites only mark lines, as
  the emulation did, so it does.

---

## 3. The call-site rule against the ergonomist's veto: **the verdict is a function of the program's declarations, not of the line**

`unadmitted_release` refuses a consuming call when **no mark in the program** names
the function (`w/q3/`). Here are the same line and the same acquiring declaration
with a third declaration added or moved:

| program | what differs from `vk_cross` | Darwin | arm64 · x86-64 |
|---|---|---|---|
| `vk_cross` (brief) | nothing | **check 1** | check 1 · check 1 |
| `vk_decl_never_called` | adds `buf_make_pooled() -> Buf acquires buf_destroy_pooled`, never called | **check 0, run 134** (0 B / 240 B, before C) | |
| `imp/main` | that declaration in an imported module, never called | **check 0, run 134** | |
| `split/lib.hero` checked alone | the crossing, in a module | **check 1** | |
| `split/main.hero` | the same module, plus that declaration in `main` | **check 0, run 134** | |
| `split2/pool.hero` checked alone | a **correct** module: `buf_destroy_pooled` of a buffer `main` made with `buf_make_pooled` | **check 1**, `unadmitted_release` | check 1 · check 1 |
| `split2/main.hero` | the whole correct program | **check 0, run 0** (61 B / 0 B) | 0 · 0 |

- **The run-time abort is still a function of the acquiring declaration and the
  ending call:** 134 before C in every wrong row. **The check-time verdict is
  not.** It moves between 1 and 0 when a declaration that neither line mentions is
  added, is moved into an imported module, or is never called.
- **`split2` is a correct program whose module is refused when checked alone.**
  That is defect 085's shape (`unread_releaser` answering 1 alone and 0 inside a
  program), arriving in a second rule. Stock `heroes check pool.hero` is 0.
  - Route A's diff touches only `emit/handle_traffic.hero`, `emit/decls.hero`,
    `alloc.c` and the header (`grep '^+++' work/A.diff`), so route A's `check`
    equals stock's. That is a reading of the file list: route A's `check` was not
    run.
  - So `split2` meets the engineer's own condition (*"a correct program … that
    the rule refuses and route A runs"*) at module granularity. It also
    contradicts the engineer's *"Every check-time refusal in this table is a
    program route A already aborted at run time"* for `heroes check <module>`.
- **The ergonomist never judged this rule.** Its brief's variants are V1, V2, V3,
  R1 and A2 (`llm-ergonomist.md:13-31`). Its *"No veto: none of them makes a line's
  meaning depend on anything outside the line and its signature"* is therefore about
  five texts, not about the rule the landing would carry.
- **The spec-warden's C1 text has no sentence stating it** (§ 7). The warden's
  condition, *"I approve C1 if the checker makes the missing transfer word a
  compile error"*, rests on a rule no reader was shown and no priced sentence
  states.

Whether this is the veto is the ergonomist's to say. What is measured is that the
check-time verdict is a function of the program's declaration set.

---

## 4. R1's two readings: **the engineer's `retains` is "first if not held"; two shapes nobody ran break it**

Under `heroes-176` (`w/q4/`: `json_object_get … -> Json retains json_object_put`,
adders `transfers`, and OpenSSL's `_up_ref` on the parameter), three runs each:

| program | Darwin | arm64 · x86-64 |
|---|---|---|
| `jsonc_borrow_get_r`: json-c's documented borrow, then `json_object_get` | **0**, `outlived its parent: 42` | UNRUN, no json-c in the images |
| `jsonc_two_refs_r` | 0 | UNRUN |
| `jsonc_into_borrowed_r` | 0 | UNRUN |
| `jsonc_put_child_r`: put a child after handing it in | 134 before C | UNRUN |
| `jsonc_move_field` (`add(b, get(borrowed n))` then put `a`) | 0; without the `get`, 134 | UNRUN |
| `x509_upref_r` (`X509_up_ref(a: X509 retains X509_free) -> i32`) | 0 | 0 · 0 |
| `ssl_upref_set0_r` | 0 | UNRUN |

So the prototype's `retains` on an address not held begins a life at 1
(`alloc.c:465-474`), which is the ffi seat's "first if not held". The
**warden's "live or borrowed" has no instrument**: the runtime's three handle
entries are `acquired`, `retained` and `consumed` (`heroes_runtime.h:205-207`), and
nothing records a `borrows` result. So at run time the warden's reading and the ffi
seat's are the same reading.

**Two shapes nobody ran.**

- **A reference taken on a released address.** `rc_after_release` does
  `obj_new`, `obj_unref` (C frees it), `obj_ref` on it, then `obj_unref`.
  - It is **check 0, run 0** (36 B / 24 B) under `heroes-176` and under `P2`.
  - With `--sanitize` it is **134, `heap-use-after-free`**, three of three.
  - The strict reading (the historian's: the kernel's *"addition on 0;
    use-after-free"*, *"a `retains` on an address that is not live should abort
    before C runs"*) refuses this and refuses json-c's documented idiom. The ffi
    seat measured the second with `HV_R1_STRICT`.
  - The tolerant reading admits both. Only "live or borrowed" separates them, and
    only with a borrowed record that no seat priced.
- **`retains` naming a releaser outside the live set.** `fam.h` has one refcounted
  type with two releasers. `pipe_open` has `acquires pipe_close` and `fam_ref` has
  `retains file_close`:

| program | `heroes-176` (`P/runtime`) | `P2/runtime` |
|---|---|---|
| `retains_switch`: both references given to `file_close`, **the wrong release** | **0**, and C prints *"file_close on a kind-1 object: the wrong release"* twice (all three platforms) | **134 before C** |
| `retains_switch_orig`: both to `pipe_close`, correct C | **134**, a correct program refused (all three platforms) | 134 at `fam_ref`: the mark shares no releaser with the life |
| `retains_switch_set` (`retains pipe_close \| file_close`), the wrong release | **0**, silent | **134 before C** |
| `retains_switch_orig_set`, correct C | 0 | 0 |

- **The newest mark wins, and the prototype applies that to `retains` too**
  (`alloc.c:473`, `hero_handle_set[i].by = by`). A reference widens or replaces the
  set of the life it adds to, so defect 075 comes back through a reference.
- **The warden's open question**, *"what happens when `retains` names a releaser
  outside the live entry's set?"*, has this answer: the prototype replaces the set.
- **`P2` adds 12 lines to `hero_handle_begun`**: `retains` on a LIVE address keeps
  the live set and must share a name with it. It catches both wrong releases before
  C, and it leaves every other R1 row unchanged: `refcount_r` 0,
  `jsonc_borrow_get_r` 0, `jsonc_two_refs_r` 0, `x509_upref_r` 0, and
  `getter_wrong_repaired` 134, all Darwin. Its message is miscast: it says *"given
  back to `file_close`"* for a call that took a reference.

**One more mismatch between seats**: the engineer's prototype writes `retains
<names>`, a set, and the warden's C1 grammar writes `"retains" ident`, one name
(`176-sw-work/C1_final.md`).

---

## 5. Question 2's false refusal: **by the engineer's own standard, `realpath` is the right refusal**

The engineer calls `realpath` in two modes a *measured false refusal*, and calls
`permode`, one module per pointer mode of `sqlite3_bind_text`, *the right refusal:
that declaration does not restrict the destructor, so `lent` is false of some call
it admits*. I reran both (`w/q5/`):

| program | stock | `heroes-176` | `heroes-n1` |
|---|---|---|---|
| `realpath/` (the engineer's) | check 0, run 0, `/private` twice | `contract_differs` | **check 0** |
| `permode/` (the engineer's) | check 0, run 0 | `contract_differs` | `contract_differs` |
| `xmod-lent`, `xmod-owned`, `xmod-xacquires` | 0 | `contract_differs` ×3 | `contract_differs` ×3 |
| `twoarity` | 0 | 0 | 0 |
| **`realpath_static/`**: `alloc.hero`'s own declaration, `-> cstr owned free`, handed a static buffer | **check 0, run 134 (0 B / 0 B)**; `--sanitize` *"attempting free on address which was not malloc"* ×3 | | |

- **The same test gives the same answer.** `realpath`'s `owned free` declaration
  does not restrict `resolved`, so `owned` is false of a call it admits, and the
  last row runs that call. `realpath` is a value-dependent contract on a result,
  and `permode` is one on a parameter.
- **A narrowing that keeps `xmod-lent` refused and makes `realpath` legal exists.**
  `heroes-n1` changes one line in `P3/selfhost/check/contracts.hero`, so that on a
  RESULT a mark against no mark is not compared. It was run on the six programs
  above and not on the 510; since it only relaxes, it cannot add a verdict there,
  and that is an inference.
  - It separates `realpath` from `permode` by POSITION only, and I cannot find a
    reason in either program for that.
- **No declaration rule can keep `xmod-lent` refused and `permode` legal.** Their
  declaration pairs are the same shape: `keep(s: cstr lent)` against `keep(s:
  cstr)`, and `text: cstr lent` against `text: cstr`, both over a `cstr`
  parameter, read from the files.
- **How common a value-dependent contract bound twice is:**
  - **in the tree, zero**: `contract_differs` fires on 0 of 510 files (the sweep);
  - **in the world, unmeasured**: there is no corpus of Heroes programs beyond the
    tree.
  - The shape itself is frequent. SQLite 3.54.0 (SDK `sqlite3.h`) has **12**
    `bind_`/`result_` blob and text functions taking a `void(*)(void*)` destructor
    (`tr '\n' ' ' | grep -oE …`, 27 functions with any such destructor). The ffi
    seat counted SDL3's 9 `closeio` and leptonica's 46 `copyflag`, carried and not
    rerun.

---

## 6. `SSL_set_bio(s, b, b)`: **correct C refused, and the general rule is right to refuse; a per-function exception is not this sitting's word**

`w/q6/`, three runs each; `leaks --atExit` on Darwin:

| program | Darwin | arm64 · x86-64 | C leaks |
|---|---|---|---|
| `ssl_same_bio_t` (`rbio` and `wbio` both `transfers`) | **134**, the stray message (0 B / 396 B) | 134 · 134 | |
| `ssl_upref_same`: `BIO_up_ref` first, the workaround a reader reaches for | **0** | | **4 leaks, 288 B**: one BIO reference is never dropped, since one BIO in both positions consumes one |
| `ssl_shim`: `static inline void SSL_set_bio_one(SSL *s, BIO *b) { SSL_set_bio(s, b, b); }`, bound `b: Bio transfers` | **0** | 0 · 0 | **0 leaks** |
| emulated alias rule: the second `hero_handle_consumed` skipped when `wbio == rbio` | 0 | | 0 leaks |

- **A general rule of *one handle in two consuming positions is one reference*
  is wrong.** `ECDSA_SIG_set0(sig, r, r)` (`openssl/ec.h:1366`) in C (`sig_same.c`,
  ASan) is **134, SEGV in `BN_clear_free`**, three of three. The same shape in
  Heroes (`sig_same.hero`, `r` and `s` both `transfers`) is **134 before C**, with 0
  stdout bytes: the refusal that saves it is the one that refuses `SSL_set_bio`.
- **So `SSL_set_bio`'s exception is per function**, stated on its own page. A word
  for it would be a word for one function, and no seat has found a second. The
  measured route with no compiler line is a thin header shim (§1.11), which runs
  clean.
- **What stays wrong is the message.** It calls a correct program *"a double
  release [that] may already have corrupted memory"*. That is defect 084's, already
  filed.

---

## 7. Found along the way

- **Using a handle after its `consumes` call is check 0 and an AddressSanitizer
  use-after-free**, on the STOCK compiler (`w/q7/`).
  - `read_after_consume`: `cJSON_Delete(item: b)`, then `b` as the non-consuming
    `object` of an add. It is check 0 and run 0, and with `--sanitize`
    `heap-use-after-free`, three of three.
  - `read_after_transfer_parent_gone` under `heroes-176`: a child handed into a
    parent, the parent deleted, then the child written into. Check 0, run 0, and
    with `--sanitize` `heap-use-after-free` ×3.
  - The spec says at `:385` *"mark the parameter `@` and the value does not survive
    the call"*, and C1 extends it to `transfers`. `helper_consume_then_use` passes
    `@b` to a consuming helper and reuses `b`: check 0.
  - I searched `docs/work/DEFECTS.md` for `use.after`, `survive` and `after
    consum` and found no hit, so it looks unfiled as far as those words reach. It
    predates this sitting. It matters here because C1 widens the sentence.
- **The spec-warden's C1 text** (`176-sw-work/C1_final.md` against `pristine.md`)
  states:
  - the set;
  - `transfers`;
  - `retains … live or borrowed`, one name;
  - A2 *at one type*;
  - *"Ending one with any other call, or more often than it was taken, aborts"*.

  It **does not state** that a release no mark names is refused at CHECK
  (`unadmitted_release`). So the +182 real is for a text that omits the rule the
  warden's approval depends on.
- **The warden's first prediction already holds of the prototype.** `xfer_*`,
  written `consumes`, give `check` 1 with a note naming `transfers` (§ 0). Like the
  engineer's, it will be scored at the landing only if something other than this
  prototype lands.
- **Q4's option 2 holds on the shapes beside it.** On `heroes-q4` (`w/q8/`),
  `@out: cstr owned free_out` over each of the following is **build 1,
  `ffi_owned_const_cell`**, where stock gives **build 2, `internal error`**:
  - `const gchar **` (a typedef of `char`);
  - `cstring_t *` (a typedef of `const char *`);
  - `char const **`;
  - the brief's `s4_cstr_owned`.
- **The `_up_ref` counts differ by method, not by fact.** Over Homebrew
  OpenSSL 3.6.4 headers:
  - **31** distinct names (`grep -ohE '\b[A-Za-z0-9_]*_up_ref\b' | sort -u`);
  - **29** declared `int X_up_ref(`, the warden's figure;
  - **1** more is `X509_chain_up_ref`, which returns a new `STACK_OF(X509) *`
    rather than a reference to the same address;
  - **27**, the ffi seat's figure, is the AST-visible count.

  All agree the family needs R1's parameter form.
- **Two of the shared brief's framing facts, checked:**
  - **`twoarity` is one arity, not two.** Both `printf` declarations take two
    parameters (`as_int.hero`, `as_text.hero`, opened). The warden is right; the
    fixture's own comments call them arities.
  - ***"no program in `examples/` has a consuming function that transfers"*
    holds**: 5 `consumes` declarations, `sqlite3_close` and `sqlite3_finalize` in
    two programs and `curl_easy_cleanup`, one per handle type (`grep -rn consumes
    examples --include='*.hero'`).
- **Using a child after its transfer is legal under the prototype**, which answers
  the historian's *Risk 2* (cJSON's README order).
  - `readme_order` checks 0, runs 0, and is ASan-clean ×3.
  - Through a helper it needs `child: @resolutions` at the call.
    `@child: resolutions` is a parse error (`expected_args_close`), which I note as
    a spelling a reader may try.

---

## 8. Per route: what it was RUN against, and what it was only argued against

| route | run against | argued only |
|---|---|---|
| the set (`a \| b`) | engineer: prototype, 31 programs, 510 files, the fixpoint. ffi: `hv.c` on 16 programs × 3 platforms. critic: 8 + 20 programs × 3 platforms | ergonomist and warden (text) |
| **V1** `transfers` | engineer: the brief's programs, `_t` variants, `t_*`. ffi: runtime side on 16. critic: **the relabel and `BIO_new_fp`, silent** (§ 1) | ergonomist (reading), historian (precedent, which predicted the relabel) |
| **V1c** (a transfer names its releaser) | **critic only, emulated** by editing one emitted line (§ 1); the compiler is UNRUN | historian |
| V2 | engineer: `xfer_cj_v2`, `jsonc_v2_miss`. ffi: 16 programs with complete sets; bindings counted, not compiled. critic: `e3`/`e4` | warden (bytes and tokens), ergonomist |
| V3 | ffi: runtime only (`hv_moved_into`) on 16. **Nobody built the compiler side** | engineer (estimate), warden (`BIO_new_fp`'s receiver is a result), historian |
| R1 `retains` | engineer: result and parameter, `refcount_r`, `upref*`. ffi: 16 programs, strict and tolerant. critic: json-c, OpenSSL, **the set switch and `rc_after_release`** (§ 4) | warden (text), historian (strict, from the kernel) |
| call-site rule `unadmitted_release` | engineer: 510 files, 31 programs. critic: the locality shapes (§ 3) | **nobody else. No reader saw it and no priced sentence states it** |
| Q2 `contract_differs` | engineer: the `xmod-*`, `twoarity`, `realpath`, `permode` and `three`; 510 files. critic: reran them, plus `realpath_static` and `heroes-n1` | ffi (reading: `s2/` UNRUN as a rule), warden, ergonomist, historian |
| Q3 header shim | engineer: `bind_text_copy`. ffi: a 3-function shim, `--sanitize`, `leaks` | historian (the wrapper, UNRUN; it is the ffi seat's measured `conservative_right` inside a function) |
| Q3 pinned argument | nobody | engineer (40-60 lines, UNRUN) |
| Q4 option 2 (refuse) | engineer: `s4`, `pzTail`. ffi: census of 589, Tesseract run. critic: 3 typedef shapes | |
| Q4 option 1 (the header's qualifier) | nobody | engineer (40-60 lines, UNRUN) |
| a success-only transfer | ffi (the failure shapes, `hv.c`). critic: shapes on the prototype, plus an emulated spelling (§ 2) | historian (predicts the first `examples/` transfer is one) |

---

## 9. Contradictions between seats, and which side is checkable

1. **V2 has a true positive** (§ 1). The engineer says none could be built; `e3`
   is one on three platforms. The ffi seat's *"same verdict on all 16"* holds of its
   16 and not of `e1`/`e3`. Checkable by `e1`/`e3`, and settled by V1c, which gives
   the same answer as V2 there.
2. **R1 on an address not held.** The historian (*abort*) is against the ffi seat
   (*begin a life*) and the warden (*live or borrowed*). Checkable:
   - `jsonc_borrow_get` refuses the strict reading (ffi seat, `HV_R1_STRICT`);
   - `rc_after_release` refuses the tolerant reading (`--sanitize`, § 4).

   The warden's reading is the only true one, and it has no runtime record.
3. **`realpath` a false refusal, `permode` a right one** (the engineer, one seat).
   Checkable by `realpath_static` (§ 5).
4. **R1 as a set or one name**: the engineer's grammar against the warden's C1.
   Checkable by reading the two.
5. **The ergonomist's *no veto*** stands on five texts. The rule that would land,
   and whose verdict depends on third declarations (§ 3), was not among them.

---

## 10. The questions the sitting should have asked

1. **Is a transfer checked against the family it hands the life to?** Only the
   historian asked, from precedent, and nobody built it. The prototype's
   unchecked `transfers` restores defect 075 through a relabel its own diagnostic
   suggests, and through a real transfer, `BIO_new_fp`. V1c answers it at zero
   runtime lines (§ 1).
2. **What does a transfer owe on failure?** Every route and the prototype end the
   obligation before the call. A spelling that names the success value runs
   correctly on all four json-c shapes (§ 2), and nobody priced it.
3. **Is `unadmitted_release` local?** It is the most consequential rule in the
   landing, a new diagnostic class (CLAUDE.md §4), and it was put to no reader. Its
   module-alone verdict repeats defect 085 (§ 3).
4. **What does a reference do to the set of the life it joins, and to a released
   address?** In the prototype it replaces the set, and it begins a life on a freed
   one (§ 4).

---

## 11. Unrun

- **Windows**, everything.
- **json-c on Linux**: neither image has its headers.
- **V1c, the success-only spelling and the alias rule as compilers.** Each was
  emulated by editing emitted C lines, and each compiled at the project's flags
  with 0 errors.
- **`heroes-n1` over the 510 files.** The prototype's own suites on my build: I
  relied on the engineer's transcripts for them and did not rerun them.
- **`P2` under the `run` or `runtime` suites.**
- **The zombie child in C on Linux.**
- **Route A's own `check`**: inferred equal to stock's from its diff's file list.
- **Any reader-side effect.**

## Files

All under `<scratchpad>/176-completeness-critic/`:

- **Compilers:** `heroes`, `heroes-176`, `heroes-n1`, `heroes-q4`.
- **Patched sources:** `P/` (the prototype's `selfhost/` and `runtime/`),
  `P2/runtime/` (the `retains` change), `P3/selfhost/` (N1), `PQ4/selfhost/`.
- **Harness:** `w/r.sh`, and `w/sweep.sh` with its outputs `w/sweep_stock.txt` and
  `w/sweep_176.txt`.
- **§ 1:** `w/q1/`: the relabels, `e1`-`e6`, `fp_zombie.c`, the `*.V1c.c` edits.
- **§ 2:** `w/q2/`: the `*_t.hero` programs and the `*.cond.c` edits.
- **§ 3:** `w/q3/`: `vk_decl_never_called`, `imp/`, `split/`, `split2/`.
- **§ 4:** `w/q4/`: the `*_r.hero` programs, `fam.h`, `retains_switch*`,
  `rc_after_release`.
- **§ 5:** `w/q5/`: `realpath/`, `permode/`, `realpath_static/`.
- **§ 6:** `w/q6/`: `ssl_*`, `sig_same.*`, `ssl_one.h`.
- **§ 7:** `w/q7/` (use after consume) and `w/q8/` (Q4 typedefs).
- **Linux:** `w/linux/`: `seed176.c`, `run.sh`, `arm64.txt`, `x86.txt`.
