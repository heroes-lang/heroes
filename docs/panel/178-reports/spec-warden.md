<!-- Copied by the coordinator from the seat's REPORT.md in its own directory, 2026-09-24,
unchanged below this comment. The seat's work files it names stay in that
scratchpad directory, which is session-specific. -->

# Panel 178, spec-warden

**One row.** No budget veto: the dearest draft, the proposal with its grammar line, reads 8530 real, and adding the FFI floor gives 8590 against 10240. **Veto on D (Principle 0).** **Approve R1** in a merged wording that costs **+36** real and not the proposal's +55. **Approve L** and **Z2** at 0 over R1, and **S** as the conservative fallback. **Object** to R0 (+50 honest), R2 (+41), A (+36 honest), Z1 (+55 over R1), T (+59 to +75) and the proposal as a whole (+169 honest). All sit on §1.6, §1.2 and Principle 0 (§1.0). The prediction: adopted onto `57679005`, R1 reads **8397**, digest `7b3f1e885cadf0b7`. The condition: M-core-packages shows the constructions live in fewer than 3 bindings, and then R1 lapses and waits.

| route | verdict | design.md | real Δ from 8361 (`claude-opus-5`, `--refresh` exit 0) |
|---|---|---|---|
| **R1** | **approve**, merged wording | §1.6, §1.2, Principle 0 (the census accepted as an argument from Part 1) | **+36** (`padding too`) · +41 (`every other byte`) · the proposal's wording +55 |
| **R0** | object | §1.2, §1.6 (the draft must be honest) | +35 bare, which contradicts spec § 9's UFCS · **+50** honest |
| **R2** | object | §1.2 (the rewrite term), Principle 0, §4.9 | **+41** |
| **A** | object | §1.2, §1.6 (the draft must be honest: spec § 1 says *no semicolons*) | +42 bare · **+36** honest |
| **Z1** | object | Principle 0, §1.2 | **+55 over R1** (R1+Z1 with its grammar line: +110) |
| **Z2** | approve (R1's text already means it) | §1.6 | **0 over R1** · the warning sentence costs +36 more: object |
| **T** | object | §1.2, Principle 0 | +59 as written · +61 merged · **+75** honest |
| **L** | **approve** | spec § 5 already rules. CLAUDE.md § 12 | **0** · restating it (variant IV's second sentence) costs +36: object |
| **S** | approve as the conservative fallback | Principle 0 | **0** |
| **D** | **veto** (Principle 0, not the budget) | Principle 0, §4.9 | **+66** |
| whole proposal | object | §1.6, §1.2 | +161 verbatim · **+169** with the `[ "zero" ]` grammar line |
| H (header-length field, `i8[_]`) | observation, outside the question | — | +25 |

- `verdict`: approve R1 (merged wording), L, Z2 and S. Object to R0, R2, A, Z1, T and the whole proposal. **Veto D.** None of it is provisional: every spec count is `--refresh`, exit 0.
- `section`: design.md §1.6 (the payment rule), §1.2 (the cost formula), §1.0 Principle 0. §4.9 bears on R2 and D.
- `spec_token_delta`: measured on `claude-opus-5` through `POST /v1/messages/count_tokens`, 2026-09-24. The baseline is **8361** real, 6282 `cl100k_base`, digest `0159e9b26bd998a2`. The recommended text is **8397 (+36)**, 6313 (+31, under `DELTA_GATE`'s 50), digest `7b3f1e885cadf0b7`.
- `removal`: **nothing, and that is a problem.** I read every sentence of § 13's record paragraph (from *A group's `record` is the header's struct* to *two records may not name one tag but `void`*) under each route, and none of them becomes redundant:
  - `[a, b, c, d]` is still how a named fixed field is built, and A merges into that sentence rather than replacing it.
  - `partial` still declares fewer fields than the header has.
  - *Its size stays C's* still holds.
  R1 therefore pays with a registered prediction (below). It names instruments that exist, `grep` and a line count, and the milestone that scores it, M-core-packages.
- `needed_for_self_hosting`: **no** (the longest fixed array in the tree is `u8[8]`).
- `argument` (≤120 words): The proposal spends +169 real to buy what +36 buys. The census pays for one clause, *the rest is zero when asked*: 23 to 42 public records per platform have an array longer than 8, and a construction drops from 905 real tokens to 117 (`utsname` declared `partial`), or from 4016 to 154 (all five fields declared, keeping `==`). Z1 costs 55 more and guards a spelling, not a state. Today `partial` and a full literal both build the Darwin mutex from zeros with no mark: `check` 0, lock 22. T costs 59 to 75 and saves 53 per program over a loop, which needs no new form once defect 091 is repaired. D's Heroes-record half has no measured argument.
- `prediction`: if R1 lands on `57679005` in the wording *End a construction with `rest: zero` and every field it does not name is zero, padding too; only a group's record has it.*, then `./heroes measure spec/heroes-spec.md --refresh` reads **8397**, digest `7b3f1e885cadf0b7`. On a base that another commit has moved, the reading is **+36 ± 2** above it, unless § 13's field sentence itself changed. **The registered payment**: at the close of M-core-packages,
  1. `find . -name '*.hero' -not -path './docs/*' -not -path './archive/*' | xargs grep -lE '\[(0, ){8,}0\]' | wc -l` reads **0**. It reads 0 today at `57679005`.
  2. `rest: zero` appears in **3 or more** `.hero` files outside `tests/` and `docs/`. It is in 0 today.

  If (2) finds fewer than 3, the ledger row is marked `lapsed` and R1 is argued again under the removal branch.
- `condition`:
  - **R1** changes to object if either of these holds:
    - the compiler seat cannot keep the padding promise with a mechanism C11 guarantees and that survives `-O2`, and the clause stays in the text anyway;
    - M-core-packages' plan puts every long-array construction inside 2 or fewer bindings, so the rent is never repaid.
  - **A** changes to approve if a reader test with 10 or more readers puts `[x; n]`'s first-try compile rate at least 20 points above `rest: zero`'s.
  - **Z1**: I withdraw the objection if a census struct whose all-zero value crashes or corrupts memory is found on some leg (a C error code does not count), **and** Z1 is widened to gate `partial`'s zero-fill as well.
  - **T** changes to approve if M-core-packages binds 3 or more string fields of the third kind **and** a reader test shows the loop failing on the first try at least half the time.
  - **D**: I lift the veto if a Heroes record outside any group is shown to need a zero default, by a program in `examples/` or `selfhost/`.

---

## 1. The instrument, and how every spec number was taken

design.md §1.6 was reached by grep at the start of this sitting. It reads today: *must fit in 10240 tokens, measured by `claude-opus-5` through `POST /v1/messages/count_tokens`*. `./heroes measure` puts the FFI floor at 60, so the check judges the real count plus 60.

Each draft was applied to `spec/heroes-spec.md` **in this directory's copy only**, starting from a pristine copy (`work/pristine-spec.md`, sha256 `eda63482…`). Each one was:

1. measured offline with `./heroes measure spec/heroes-spec.md`;
2. measured with `. /Users/joseph/Temp/heroes/heroes-lang/.env && ./heroes measure spec/heroes-spec.md --refresh`;
3. restored.

The driver scripts are `work/drafts.py` and `work/measure_all.sh`, and the rows are in `work/prices.tsv`. **Every `--refresh` exited 0, so none of these numbers is a lower bound.** The baseline and R1 were measured twice and gave 8361 and 8416 both times. The file's sha256 after the last restore was `eda63482…`, identical to the pristine copy.

| draft | what the text says | real | Δ real | cl100k | Δ cl100k | digest |
|---|---|---|---|---|---|---|
| baseline | today's document | 8361 | — | 6282 | — | `0159e9b26bd998a2` |
| P verbatim | the shared brief's text, after *as many elements as the type says.* | 8522 | +161 | 6404 | +122 | `08fbf86e59e90f21` |
| P honest | P plus `Member`'s `[ "zero" ]`, which Z1 needs as a production | 8530 | **+169** | 6409 | +127 | `5bd4eb153ddfdc65` |
| R1 | the brief's R1 sentences only | 8416 | +55 | 6326 | +44 | `2d2ffca85e8ef92d` |
| R1 without the padding clause | — | 8406 | +45 | 6318 | +36 | `71ea67fe44b1c7c5` |
| R1 without *outside a group* | — | 8403 | +42 | 6316 | +34 | `6688a2b4724c5e91` |
| R1 merged, *bytes between fields too; only a group's record has it* | — | 8402 | +41 | 6316 | +34 | `4cda42ba2dc49d42` |
| R1 merged, *and so is every other byte* | — | 8402 | +41 | 6318 | +36 | `1d102668392d45f2` |
| **R1 merged, *padding too*** | **recommended** | **8397** | **+36** | 6313 | +31 | `7b3f1e885cadf0b7` |
| R1 + Z1, as worded | no grammar line | 8463 | +102 | 6362 | +80 | `117b9d7b16b4528a` |
| R1 + Z1, honest | with `[ "zero" ]` in `Member` | 8471 | +110 | 6367 | +85 | `4ec6ed7149802008` |
| R1 + Z2 | R1's text; Z2 needs no words | 8416 | +55 | — | — | as R1 |
| R1 + Z2, with a warning | R1 plus *Zeros are not a valid value of every struct: …a mutex, comes from its function* | 8452 | +91 | 6354 | +72 | `430b42c31548b936` |
| T verbatim | the brief's T sentence alone | 8420 | +59 | 6324 | +42 | `2c37e38b30da7955` |
| T honest | names the failure code and bit-for-bit copying into `i8` or `u8` | 8436 | +75 | 6341 | +59 | `154f240804a38eb3` |
| T merged | merged into the `validated_bytes` sentence, honest | 8422 | +61 | 6332 | +50 | `4d765ab0207229ee` |
| R1 + T merged | — | 8477 | +116 | 6376 | +94 | `79c8a13046074bf5` |
| R2 | § 13 sentence plus § 9's *no default values* qualified | 8402 | +41 | 6315 | +33 | `632283aa4b2cdcb5` |
| A bare | variant IV's first sentence | 8403 | +42 | 6317 | +35 | `3a6a85c21c51990c` |
| A honest | merged into the literal sentence, plus the `Primary` production, plus § 1 *no semicolons but `[x; n]`'s* | 8397 | +36 | 6311 | +29 | `2b00ebfe0adcf391` |
| R0 bare | `T.zero()` sentence | 8396 | +35 | 6311 | +29 | `3f5b7f4c517d8e6a` |
| R0 honest | plus § 9's UFCS exception | 8411 | +50 | 6324 | +42 | `1f5d628b41ef30e2` |
| R0 + Z1 honest | — | 8440 | +79 | 6344 | +62 | `77bedb784625f2ba` |
| L | no text: § 5 lines 121-122 already admit *a field or element inside one* | 8361 | 0 | 6282 | 0 | unchanged |
| L restated | variant IV's second sentence, a duplicate of § 5 | 8397 | +36 | 6312 | +30 | `64a42bb56df4a3d4` |
| D | § 9 sentence giving each type's zero, plus *no default values* deleted | 8427 | +66 | 6335 | +53 | `751988c01ae71615` |
| H | `i8[_]` takes its length from the header, plus the `Type` production | 8386 | +25 | 6301 | +19 | `22e4f8505b819bf0` |

**Three corrections to the proposal's own text, each measured:**

- **"every byte between fields" is false for trailing padding.** `clang -Wsystem-headers -Wpadded` over the census's 33 Darwin headers (the file is `work/probe/tu.c`) prints `padding size of 'struct timeval' with 4 bytes to alignment boundary`. `timeval` is a struct a program builds, for `setsockopt` and `select`. The wording *padding too* is exact and costs 5 fewer tokens.
- **Z1 needs a production.** Without one, the `zero` mark is a form the document does not derive. The production costs +8.
- **A's `;` is refused by the lexer.** `./heroes check work/probe/semi.hero` gives ``error[unexpected_character]: `;` is not part of the language's syntax``, and spec § 1 says *no semicolons*. **An instrument gap:** the spec suite on the A draft (`work/harness-bin ./heroes spec`) gives 16 passed and 4 failed, and the four are only the record checks (`budget`, `spendable`, `real`, `ledger`). So `spec/rejected` passes a code span holding a character the lexer refuses, because it reads refused *words*. If A lands, the lexer change comes first ("instruments first", `.claude/rules/spec-shape.md`).

The recommended R1 text passes the spec suite the same way: 16 passed, and the 4 failures are the record checks that the landing commit's pasted record settles.

## 2. The program side, which is what the rent is paid against (§1.2)

These programs are in `work/programs/`. Only the S programs compile today. The others are priced texts: `check` refuses `zero`, `;` and `to_fixed` today, as measured. The real count comes from `work/realcount.py`. `heroes measure --refresh` refuses paths other than the two judged documents, so the script is a replica of its request: same endpoint, same `claude-opus-5`, same body, same two-probe offset. It is validated by reproducing the spec's **8361** exactly.

| task | S (today) | R1 | A | R0 | T | notes |
|---|---|---|---|---|---|---|
| `uname`, Darwin, `partial`, one field | **905** real / 882 cl100k; `run` prints `Darwin`, exit 0 | **117** / 96 | 121 / 101 | — | — | S matches the brief's `uname_darwin.hero` byte for byte, minus its comment (`cmp`) |
| `uname`, Darwin, all five fields (keeps `==`) | **4016** / 3993; `run` prints `Darwin`, exit 0 | **154** / 132 (Z1: 156) | 206 / 183 | 151 / 130 | — | |
| `sun_path`, Darwin, runtime path | 502 / 464 (S, then L once 091 is repaired) | 166 / 132 (R1 + L) | 171 / 138 | — | **113** / 87 | |

**Break-even on tokens alone.** The spec is the prompt, so the rent is paid on every generation, while the saving comes only where a construction is written. At +36:

- R1 needs one prompt in **22** to build a `partial` `utsname` (saving 788), or one in **107** to build the full declaration (saving 3862).
- A at +36 needs one in 22 as well.
- T at +61 saves **53** over R1 + L, so it never breaks even on tokens. Its only case is the rewrite term. I paid that term once myself: my first u8-to-i8 conversion was ``error[bad_operand] … found `i64?` `` (below).

## 3. Principle 0, and panel 163's condition measured

- **The condition, today.** `grep -rnE ': [iuf][0-9]+\[([0-9]{2,}|9)\]' examples --include='*.hero' | wc -l` gives **0**. One directory of 56 declares a fixed array at all (`examples/raylib/main.hero`). Across the tree outside `archive/` and `docs/`, the longest is `u8[8]` (11 occurrences). **Unmet.**
- **I withdraw that condition as the wrong instrument, and I say so of a predecessor seat.** It counted the corpus that the status quo produced. When one construction costs 905 to 4016 real tokens, an empty corpus is what the status quo predicts whether or not the need exists. The instrument could not tell *no need* from *need priced out*.
- **The census is a measured argument derived from Part 1**, which Principle 0's second branch admits. §1.11 says every program's reach into the OS comes from C. §1.12 says the boundary must be complete. The census found, on three legs, 23, 29 and 42 public records per platform with an array longer than 8. I recounted that from the three `.tsv` files: `awk -F'\t' '$1>8 && $2 != "" && $2 !~ /^_/ {print $2}' census-<leg>.tsv | sort -u | wc -l` gives Darwin 42, Linux arm64 29, Linux x86-64 23. Combined with §2's program measurements, that is a measured §1.2 case.
- **Its limit is exact.** It measures how many such structs the headers supply, not how many prompts construct one. So it admits the **cheapest honest single form** and not a composition, and the payment prediction scores the demand at M-core-packages. Construction sites may move into packages there, which is §1.5 (*verbose where you declare*) working against the rent.
- **It admits nothing for D.** No Heroes record appears in the census.

## 4. Why Z1 does not pay, run rather than argued

- `work/probe/mutex_partial.hero`: `record Mutex tag _opaque_pthread_mutex_t partial`, then `Mutex(__sig: 0)`. `check` exit 0, `run` prints `22` `22` (lock, unlock), exit 0, on Darwin arm64 26.6.2.
- `work/probe/mutex_full.hero`: both fields declared and every byte spelled `0`. `check` exit 0, lock `22`, exit 0.

So the state Z1 guards can be built today with no mark, by two routes Z1 does not touch. It guards the spelling `rest: zero` and not the value. Its claim, *valid on every platform*, cannot be checked by the compiler, and measurement 7 shows it cannot be checked from one platform either. What goes wrong without Z1, as measured here, is a C error code (`EINVAL`), not a crash: §1.12 is not engaged.

## 5. Why T does not pay yet

- **L carries the capability at 0 tokens.** Spec § 5 already admits the element write, so defect 091 is a lowering (CLAUDE.md § 12).
- **A UTF-8 byte reaches C's signed `char` without a new form.** `work/probe/bits.hero` converts `è`'s bytes `195 168` to `-61 -88` with `(b.to_i64().must() - 256).to_i8().must()`, exit 0.
- **`u8[104]` cannot be declared against Darwin's `char[104]`**, as measured: `error[ffi_field_type]` (`work/probe/sun_u8.hero`).
- **Unrun:** the element store itself, because 091 is unrepaired in this copy.

## 6. The strongest reason the proposal is wrong

It prices three clauses as one. The one clause the census pays for costs **+36**. The proposal costs **+133** more than that: 19 of it is R1's own wording, and 114 is Z1 and T:

- **Z1** guards a door that stands beside two open ones.
- **T** duplicates a form the language already has, once a defect is repaired.

"Robust even if less economical" is the author's instruction, and it is honoured by *padding too* and by L. Robustness does not raise Z1 or T above the rule, because neither closes a crash or a corruption class. Both close a C error code, or a run-time abort, that is already safe.

## Files

In `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/07a257f7-6606-4944-b558-0be3c97d5c11/scratchpad/178-spec-warden/`:

- `work/drafts.py`: every draft's exact text.
- `work/measure_all.sh` and `work/prices.tsv`: the prices.
- `work/realcount.py`: the replica of `--refresh`.
- `work/programs/*.hero`
- `work/probe/{mutex_partial,mutex_full,bits,sun_u8,semi}.hero`
- `work/probe/tu.c`
- `REPORT.md`
