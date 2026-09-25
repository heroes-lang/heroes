<!-- Written out by the coordinator from the critic's final message, 2026-09-25:
the harness refused its write to REPORT.md. Its first run stopped on an API error
(HTTP 403, `oauth_org_not_allowed`) and was resumed once from its transcript.
One mechanical change: this comment. The text below is the critic's, unchanged.
The files it names are copied beside this one, in `completeness-critic-work/`.
The listener it names at the end was stopped by the coordinator on 2026-09-25
(PID 32086, cwd its `w/readers`), and `/tmp/app.sock` removed. -->

# Panel 178, completeness critic

No verdict. This report names what the five reports and six briefs leave out, and checks the disagreements that can be checked. The harness refused the write to REPORT.md, so this message is the whole report.

Everything was run in `<scratchpad>/178-completeness-critic/`, a git archive of `57679005`, using the seed compiler (`./heroes`) and copies of the seats' compilers in `bin/`: `ce-heroes-{base,091,r1,a,z}`, `ffi-heroes-next`, plus two I built there (`heroes-al` = A + the CE's L, `heroes-c094`). Linux legs use `heroes-linux-arm64` and `heroes-linux` (x86-64 emulated), with the compiler built inside the container. **Windows was not run for anything.**

## The eight findings that change the picture

1. **R1's padding sentence is false on Linux x86-64, and the fault is not R1's.** I ran the Heroes pipeline's own emitted C (runtime included) under MemorySanitizer (`w/pad/msan.sh`). A control inside the same binary fires on every run. The probe record is `{char c; double d}`: it is built, passed by value to a Heroes function that returns it, and then handed to `write()`. The result is `Uninitialized bytes in write at offset 1 inside [..., 16)` at -O0, -O1, -O2 and -O3 on x86-64 (`w/pad/small.hero:29`, `logs/msan-x86-2.txt`). Linux arm64 is clean at all four levels. The same shape built with today's `partial` construction loses its padding the same way, because the loss happens in the by-value call and not in the construction. Two other shapes are clean at all four levels on both legs: `addrinfo` copied through a return, a parameter, a record field and a `[T]` element (`copies.hero`, `copies_r1.hero`), and a union bound by one arm (`union_arm.hero`). This settles the dispute between the seats:
   - the historian and the ffi-pragmatist were right to condition the clause;
   - the spec-warden's recommended wording, *padding too*, promises something the emitter does not deliver on one of the three legs;
   - the compiler-engineer's measurements are true for the shapes it measured, and those shapes did not include a padded small struct passed by value.
2. **Padding is visible only to C.** Heroes' own `==` and `hash` compare fields, never bytes (`selfhost/emit/structural.hero:9-14`, `runtime/heroes_runtime.h:338-340`). So the padding clause promises nothing a Heroes program can see. It matters only to a C callee that copies the bytes out, which is what defect 092's `write()` does.
3. **Nobody listed a route that exists today at zero spec tokens: C fills the bytes itself.** Two shapes, both run on three legs:
   - *The header returns the value.* A `static inline` in the binding's own header returns a zeroed `struct utsname`, or `(pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER` (`w/cfill/zshim.h`). On Darwin the mutex then locks with 0 and unlocks with 0. `utsname` prints `Darwin` or `Linux`, and every field stays declared, so `==` still works.
   - *C writes the path through the field's lend.* `strlcpy(dst: a.sun_path.ptr(), src: path.cstr(), size: 104)`, declared `dst: ptr counted_by size lent` (spec § 13, `f.ptr()`). It needs no 091 repair, no new form and no sign arithmetic. It writes the UTF-8 path `/tmp/hèroes-178-critic.sock`, then bind and connect return 0 and 0 on Darwin, Linux arm64 and Linux x86-64 (`sun_c_*.hero`). A clean ASan+UBSan build (`--sanitize`) on Darwin prints the same.
   - An overstated literal extent is refused at `check` (`field_lend_extent`). An overstated run-time extent panics with exit 134 (`strl_over_*.hero`).
   - Real cost (the spec-warden's `realcount.py` replica, which I checked against its rows 154 / 4016 / 113 / 166): `utsname` 162 plus a 65-token header, against R1's 154; `sun_path` 223 plus 75, against R1+L's 166 and T's 113.
4. **A third answer to "who says zero is valid" was not listed: the header's own initialiser.** This is defect 094 repaired. I prototyped the repair in `trees/c094` with 19 added lines: a struct constant's accessor becomes `T hero_v = MACRO; return hero_v;`, and the file-scope `__typeof__` probe is skipped. Its own tests pass (676 of 676). `PTHREAD_MUTEX_INITIALIZER` then locks and unlocks with 0 on Darwin, and `PTHREAD_RECURSIVE_MUTEX_INITIALIZER` locks twice with 0. **The limit I found:** a brace-list macro fits any struct of a compatible shape. `constant PTHREAD_COND_INITIALIZER: Mutex` compiles and runs, and prints `__sig` = 1018212795, which is the condition variable's signature 0x3CB0B1BB (`w/c094/wrongtype.hero`). So on this route C says the value is valid but cannot say which type the value belongs to.
5. **Reader test, 50 blind readers, every program compiled and run.** The ergonomist's rates were beliefs. I ran them as a measurement (details in § 6).
   - **Nobody wrote a false `zero` claim on the mutex under Variant II: 0 of 10.** The prediction was at least 7 of 20.
   - **All 10 readers of Variant II claimed `zero` on `utsname`, and all 10 wrote the mutex's 56 zeros by hand.** That is what Z1 costs: it refuses the zero-then-init pattern for the reader who is being careful.
   - **No reader in 50 used a zeroed mutex without calling `pthread_mutex_init`.**
6. **The biggest first-try failure, under every variant, is a gap in the spec that none of the routes touch: how C's `char` is spelled.** 22 of 50 readers wrote `u8[N]`. The emitter passes `-fsigned-char` on every leg (`selfhost/cli/flags.hero:109`), so `u8[N]` is refused with `ffi_field_type`. When `u8[` is read as `i8[`, task 1 compiles and runs for 10 of 10 readers under II, III, IV and V alike. The spec-warden's condition for A was a gap of 20 points or more. The raw gap is 30 points (IV 7/10 against III 4/10), and after the char normalisation it is 0 (10/10 against 10/10). So the gap measures the char sign, not the form.
7. **Defect 091 reaches further than the brief's element store.** Lending one element of a fixed array to an `@` parameter, `bump(@o.pts[i])`, also exits 134 with `hero_unreachable` at HEAD (`w/x091/n_lend_elem.hero`). A Heroes record that holds a group record, inside a shared `[Box]`, does the same (`n_heroes_box.hero`). **The two independent lowerings agree on every shape I ran:** each seat's own probes, the two above, and an index of -1 or one past the end. Both print `5 9 0 0` (copy-on-write holds) and `13`, and both panic with *index out of range* at `i = 3`.
8. **The ffi-pragmatist's F2 is a spelling problem, not a gap.** The F2 claim is that glibc's `pthread_mutex_t` cannot be held by value. Bound under its typedef name with no `tag`, `record pthread_mutex_t partial` with `__align: i64`, it locks with 0 and unlocks with 0 on Linux arm64 (`w/glibc/mutex_untagged.hero`; x86-64 unrun). What failed was `tag pthread_mutex_t`, which spells `struct pthread_mutex_t`.

## 1. The question put directly: does "`[x; N]` writes a length true on one platform" separate the routes?

**Not today. It moves the length and doubles the places it is written, and nothing becomes less safe.** What I ran:

- Under A, a wrong N at the construction is refused at `check`: `[0; 65]` against `i8[256]` gives `fixed_array_length` exit 1 (`w/len/a_wrongN.hero`, CE's `heroes-a`).
- A Linux declaration on Darwin is refused under both routes, `ffi_field_type` exit 1 (`a_decl_linux_on_darwin.hero`, `r1_decl_linux_on_darwin.hero`).

So both routes are per-platform bindings and both fail loudly. Porting under A edits N once per array field and once more per construction site; under R1 it edits N once per array field. The routes separate only if H (a length taken from the header) lands. Even then, the unlisted spelling `[x; _]` erases the difference. It costs **+32 real** (8393, digest `6b6e7f9319c23e04`), the same as R1 without the padding clause.

**H itself serves less than the brief suggests.** I dumped every record on the three legs (`w/census/dump.sh`, `classify.py`) and took the public records with an array longer than 8:

| records | count |
|---|---|
| total | 49 |
| not on every leg | 39 |
| identical on all three | 3 |
| on all three and different | 7 |

Of those 7, none becomes one binding under H alone. `utsname` gains `__domainname` on Linux, and `sockaddr_un`, `sockaddr`, `sockaddr_storage`, `termios`, `dirent` and `ifreq` differ in scalar types or field sets. With `partial` added, the fields common to all legs make `utsname` (5 fields) and `dirent` (4) one binding, and both are buffers C fills.

## 2. Disagreements, and which side is checkable

- **Z1 (historian approves, four object).**
  - Checkable, and the objectors' premise holds. `partial` reaches the zero mutex with no claim: three seats measured it independently, and I did not rerun it.
  - The reader test adds a number: Z1's feared harm (false claims) was 0 of 10, and its cost to correct programs was 10 of 10 hand-written literals.
  - The historian's precedent (pin-init `..Zeroable::zeroed()`) has no side door: omitting a field without it is a compile error. Heroes' `partial` is a side door. The analogy holds only under the ffi-pragmatist's and spec-warden's condition that Z1 also gates `partial`, and 12 `.hero` files under `tests/golden` and `examples` declare a partial record (`grep -rlE '^\s+record .* partial'`).
  - The proposal text and the Z1 prototype disagree on where the word goes. *"After its name"*, `record Utsname zero tag utsname`, is refused as `empty_record`. The prototype accepts `tag utsname zero` and `partial zero`. All 10 Variant II readers used the tag slot.
- **T, a value versus a place form.**
  - The compiler-engineer's structural point (the value form needs `T[N]` storage) shows up in the readers. 11 of 50 wrote a fixed-array local binding, for example `path: i8[104] = "...".to_fixed().must()` or `zeros: u8[256] = [...]`. That position is refused today (`fixed_outside_a_group`). It accounts for every II and III task-3 failure after normalisation, and for I's two task-1 failures.
  - The place form was never priced or reader-tested. I priced it: `s.copy_into(@f)`, merged into the `validated_bytes` sentence, is **+61 real** (8422, `0758c0ec18996d2d`), the same as the value form merged.
  - Finding 3's `strlcpy` route does T's job at 0 tokens.
- **A (compiler-engineer and historian approve, ffi-pragmatist and spec-warden object).**
  - The ffi-pragmatist's *"a non-zero x needs a GNU range, 6 errors under -std=c11 -pedantic-errors"* does not apply. The compiler-engineer's A emits N copies (`.pts = {t6, t6, t6}, .f = {t8, t8}`, `w/achk/kinds_a.c:172`), and the emitter compiles with `-std=gnu11` and no `-pedantic-errors` (`flags.hero:92`).
  - `[0; 256]` emits 376 lines of C for `uname_a`. Route S emits 5555 (the ffi-pragmatist's figure).
- **R1's padding sentence.** Settled by finding 1: it cannot stand as written on x86-64. Dropping it costs less, not more. The spec-warden's merged wording without the clause is **+32 real** (8393, `36a1c34f7eaceaba`) against +36 with it. I re-measured the spec-warden's own +36 text as a check on the instrument and got 8397, `7b3f1e885cadf0b7`, identical. Every `--refresh` exited 0, and the baseline read 8361.
- **R1's layout objection.** Checkable in the history, read-only in the lane (`git log -p -- tests/harness/suite_layout.hero`). Since 2026-09-02 the DECIDED rows have moved:
  - `check/walk.hero`: 6 times, 1583 → 1870;
  - `ast.hero`: 6 times, 455 → 527;
  - `print/fmt.hero`: 2 times, 1150 → 1175.

  So moving a named row is how landings normally go here. The objection is procedural, and the compiler-engineer's own condition (name the rows first) is that normal shape.
- **"The net ran with neither 091 lowering"** (coordinator's note). This is contradicted for the compiler-engineer's lowering. `heroes-r1` contains it, and `suites-r1.log` shows run 146/0, corpus 55/0, check 135/0, ir 24/0, emit 8/0 and unsupported 15/0. The ffi-pragmatist's lowering alone was never run through the net.
- **`missing_fields` unguarded** (compiler-engineer). Confirmed: its code and its message text (*"all of them, always"*) appear only in `selfhost/data_errors.hero` and `check/walk.hero:2036`.

## 3. Routes nobody listed

| route | spec cost | status |
|---|---|---|
| **C-fill**: the binding header returns the zeroed or initialised struct | 0; a one-sentence pointer to it is +36 (8397, `564b25ae3543a707`) | works today, three legs |
| **C-write**: `strlcpy` or `memcpy` through `f.ptr()` + `counted_by` | 0 | works today, three legs, UTF-8 included |
| **Z3**: the header's `*_INITIALIZER` as a group constant | 0 spec; the compiler change is defect 094's repair (19 lines prototyped) | works; a brace list is untyped (finding 4) |
| **`[x; _]`**: A that restates no N | +32 | priced, not built |
| **A spec line for `char`** (`i8`, as `-fsigned-char` already makes it) | unpriced | the largest measured first-try failure, 22 of 50 |
| **Arrays-only zero** (the historian's observation on Apple's six macros) | unpriced | not built, unrun |

## 4. Claims the seats asserted, now measured

- **The Linux leg of the header initialisers.** `PTHREAD_MUTEX_INITIALIZER`, `COND` and `RWLOCK` compile as compound literals of their own type. They equal all zeros on both Linux legs and differ from zero on Darwin (`w/init/cl.c`). `PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP` is non-zero on Linux. This closes the brief's "cond and rwlock on Linux unrun".
- **The compiler-engineer's -O2 claim for the whole pipeline.** It came from the stack-dirtying method without a control. MSan with a control, run above, confirms it for `addrinfo`, and refutes it for small by-value structs on x86-64.

## 5. Questions the sitting should have asked

1. **What does the padding clause protect, and from whom?** Only C can see padding, and it does not survive a by-value call on x86-64. Keeping it requires the emitter to pass records by address or copy them with `memcpy`: a change to how every record crosses a call, not a sentence in § 13.
2. **Is a mutex a value?** 19 of 50 readers raised copy-in/copy-out on their own. The emitted C passes the cell's address (`pthread_mutex_lock(&h0_m)`), so a local cell does not move between calls. But `m2 = m` copies a mutex, which POSIX does not allow, and nothing refuses it. **Unrun:** copying a locked mutex.
3. **Which census structs will M-core-packages actually construct, and in how many bindings?** The spec-warden's payment condition depends on this, and no seat counted it.
4. **Would the language gain more from the `char` sentence than from any of the routes?** Measured: yes, on first tries.

## 6. The reader test

- **Protocol.** 50 general-purpose subagents. Each read only the spec with one variant inserted (the ergonomist's variants verbatim; V is R1 alone) and the ergonomist's three tasks. The harness may load `CLAUDE.md`, which is panel 175's open question.
- **Judging.** Every program was compiled and run on Darwin:
  - I with HEAD, II with `ce-heroes-z`, III and V with `ce-heroes-r1`, IV with `heroes-al`;
  - task 2 against `w/readers/app.h`, a real `sockaddr_un` on Darwin, with a listener on `/tmp/app.sock`.
- **"ok"** means exit 0 with the right output.
- **"norm"** reads `u8[` as `i8[`, and `record X zero tag Y` as `record X tag Y zero`.

| variant | task 1 raw / norm | task 2 raw | task 3 raw / norm | `u8` readers | `zero` on the mutex | zeroed mutex used without init |
|---|---|---|---|---|---|---|
| I (nothing) | 8 / 8 | 7 | 9 / 10 | 1 | — | 0 |
| II (R1+Z1+T) | 4 / 10 | 0 (`to_fixed` unbuilt) | 1 / 6 | 6 | **0** | 0 |
| III (R1+T) | 4 / 10 | 0 (`to_fixed` unbuilt) | 2 / 8 | 6 | — | 0 |
| IV (A + element write) | 7 / 10 | 5 | 7 / 10 | 3 | — | 0 |
| V (R1 alone) | 4 / 10 | 4 | 3 / 9 | 6 | — | 0 |

The ergonomist's predictions, against these numbers:

- **Primary prediction, falsified.** At the predicted rate of 7 in 20, the chance of seeing 0 of 10 is about 0.013.
- **Secondary prediction, falsified.** I on task 1 was not at most half of III or IV (8 against 4 and 7).
- **What the readers reached for:** 20 of 20 named `sun_family` under `rest: zero`, and 3 reached `record pthread_mutex_t partial` with no fields, which is refused as `empty_record`.
- **Limits of the test:**
  - one attempt per reader;
  - the readers are the same model family as this critic;
  - task 2 under II and III cannot be judged until T is built.

## 7. Unrun

- Windows, for everything.
- The x86-64 leg of `mutex_untagged.hero`.
- The net with the ffi-pragmatist's lowering, and with `heroes-c094` (only its own tests ran).
- Copying a locked mutex.
- The arrays-only zero route.
- MSan on Darwin, which has no MSan.

## Files

All under `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/07a257f7-6606-4944-b558-0be3c97d5c11/scratchpad/178-completeness-critic/`:

- **Padding:** `w/pad/` (`msan.sh`, `copies*.hero`, `small.hero`, `union_arm.hero`, `ctl.h`), `logs/msan-x86*.txt`.
- **C-fill and C-write routes:** `w/cfill/`.
- **Defect 094 and Z3:** `w/c094/`, `trees/c094/`, `logs/c094-owntests.txt`.
- **Defect 091 cross-check:** `w/x091/`.
- **glibc mutex spelling:** `w/glibc/`.
- **Header initialisers:** `w/init/`.
- **Census by record:** `w/census/`.
- **Length question:** `w/len/`.
- **Spec prices:** `w/spec/` (`drafts.py`, `prices.tsv`, `refresh-*.txt`).
- **Program prices:** `w/price/`.
- **Reader test:** `w/readers/` (`*/answer.md`, `t*.hero`, `all.tsv`, `judge2.sh`).

A listener process (`w/readers/listener.py`) may still be holding `/tmp/app.sock`; it can be killed.
