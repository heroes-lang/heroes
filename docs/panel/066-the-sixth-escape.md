# Panel 066 — the sixth escape, and the error that fired on the correct program

**Lane: full** (compiler-engineer, llm-ergonomist, spec-warden, ffi-pragmatist,
historian). Convened 2026-08-15, synthesis 2026-08-16.
**Provisional — author ratification pending.**

**Verdict: `\r` enters as the sixth escape** — 2 approve, 3
approve-with-conditions, and the sitting's one veto was **lifted by its own
judge on a second ruling**, after verifying grounds it had not been shown.
Measured spec cost **+3** (3374 → 3377, headroom 719).

## The proposal, verbatim

> spec § escapes: "Five escapes, and no others: `\n` `\t` `\\` `\"` in a
> string, `\'` instead of `\"` in a character literal" becomes SIX, adding
> `\r` (value 13, legal in both contexts). design.md's escape table (panel
> 008) gains the `\r` row; the frozen-set sentence stays for
> `\0`/`\xNN`/`\u{…}`/octal (the freeze's stated reason is interior NUL,
> which `\r` cannot produce). Measured spec cost: +3 tokens.
> Why now (measurement 009, gaps 7+10): the self-hosted lexer recognises CR
> via `constant CR: u8 = 13`, but the `stray_carriage_return` test cannot be
> written in Heroes — a CR inside a string literal has no spelling — so the
> fixpoint compiler's test suite cannot express a test its Rust twin has.

**The proposal's own premise died in the sitting and the verdict does not
rest on it.** The spec-warden verified that no Rust unit test writes `\r` —
the "Rust twin" never existed; the diagnostic's one firing test is the golden
`tests/golden/check/stray-carriage-return.hero` (a raw CR byte between
tokens), which both compilers share, so unit-level parity was already exact.
The coordinator briefed a false premise that measurement 009 gap 10 and
`selfhost/scan.hero` had already absorbed; the corrections land with this
panel regardless of ratification. What the verdict rests on instead is below.

## Verdict table

| judge | verdict | the finding that decides it | prediction (to score) |
|---|---|---|---|
| **llm-ergonomist** (spec-only, blind) | approve | The loud error launders into a silent defect: a model writing HTTP writes `\r\n` with near-certainty, the five-escape spec offers **no writable repair**, so the cheapest repair is deleting `\r` — compiles clean, wrong bytes on the wire against strict servers. *"The compile error fires on the correct program — the thesis inverted."* CRLF *reading* survives on the integer-13 idiom; *construction* has no route at all | the first corpus/golden program speaking a CRLF protocol contains `\r` in a literal on its first draft; under five escapes that program cannot exist in any form (checkable now — the milestone it named, M-ffi-ladder, has passed: scored in § Predictions) |
| **historian** (advisory, sourced) | approve | Ten escape sets verified (Go, Rust, Zig, Lua, JSON RFC 8259, Swift, Kotlin, OCaml, Hare, Gleam): **every one includes `\r`; no counterexample exists**, including the three that cut `\0`/`\x`/octal. The exact bootstrap precedent is Thompson, CACM 1984: the C compiler adding `\v`, interim workaround *hardcoded decimal 11* — Heroes' `CR: u8 = 13`, forty-two years early. The regretted class (interior NUL, CVE-2009-2408) does not contain `\r` | before M-selfhost-fixpoint, `\r` completes the set self-hosting needs: no byte outside {9,10,13,34,39,92} is needed in any `.hero` string literal in the fixpoint corpus |
| **ffi-pragmatist** | approve-with-conditions | All executed on today's compiler: byte 13 is **unspellable by any composition of the inventory** (proven per constructor); `"…\r\n"` errors with a **`certain` fix that repairs toward garbage** (`\\r` — wrong for every protocol use, and `--apply` is machine-applicable); a **raw CR pasted mid-literal compiles silently, survives `fmt` byte-for-byte, renders invisibly** — retyping the visible text drops the byte and still compiles. Cost at the boundary: zero runtime lines, zero ABI; the emitter already octal-escapes byte 13 | by M-selfhost-fixpoint, gap 10 closes with `"\r"` in an ordinary literal and raw byte-13s inside literals stay at zero |
| **spec-warden** | **veto → lifted; approve-with-conditions** | First ruling (stands): the parity premise is false on the repository; corrections owed by append. Second ruling, after verifying grounds 1–5 itself (it *executed* the raw-CR trap: exit 0, no diagnostic, invisible, retype-drops-byte-still-compiles): **grounds 2+3+4 discharge Principle 0's burden under the §1-derived branch** — a correction round that injects the bug is the worst cell of §1.2's matrix, and +3 against it is decisively net-positive. Panel 008 is reaffirmed *for the question it answered* (lexer-side need, pre-FFI) and legitimately reopened for the one it never heard (programs) | **the payment, registered** (panel-046-admissible): at M-selfhost-port close, (i) `selfhost/scan.hero` carries the `stray_carriage_return` test written with `"\r"`, its untranslatable-note deleted; (ii) the golden corpus carries a CRLF-literal case compiling clean and a raw-CR-in-literal case firing the new diagnostic, `#~`-annotated. If (i) fails, the clause is `lapsed`, re-argued under the removal branch, refunded by a named removal of ≥3 |
| **compiler-engineer** | approve-with-conditions | Implemented end-to-end in its copy: **8 files, +26/−15**, spec **+3 exactly**, golden churn **one hand-written word in one line of one file**, zero `#~` changes, 473 lib tests + 13 goldens green after. Found the copy the proposal missed: `ir/print.rs` re-encodes only the five escapes, so a CR reaching the IR puts a **raw invisible byte into every `--dump-ir`**. Found the real cost: the silent-trap set grows to {`\n`,`\t`,`\r`} and `r` is a common path-initial letter (`"C:\results"` becomes exit 0). Cheaper alternatives lose: a byte-from-int builtin costs ~10× in spec and **manufactures the interior NUL the freeze exists to prevent** — it would veto that | at M-selfhost-port close, `heroes test selfhost/scan.hero` passes a `stray_carriage_return` test and `grep -c $'\r' selfhost/*.hero` is 0 for every file; at M-selfhost-fixpoint, both binaries render byte-identical `unknown_escape` messages (fails if the two `legal()` strings ever order the six differently) |

## Two condition conflicts, resolved

1. **"Six" as a numeral** (warden: land the exact measured wording; historian:
   prefer the enumeration to carry the count, §7's dead-eleven lesson). The
   numeral stays: unlike the flags count, whose list lived in another file,
   this count sits in the same sentence as its enumeration and cannot drift
   from it without the sentence being visibly wrong. The measured wording is
   what lands; any drift re-measures.
2. **The raw-CR golden vs "no raw CR in fixtures"** (the new
   raw-CR-in-literal diagnostic needs a golden that carries the byte it
   refuses; historian: fixtures spell byte 13 as `\r`, never raw). Both hold:
   `\r` serves every fixture that needs the *byte*; the one golden that tests
   the *raw spelling itself* carries it deliberately, with a header saying so
   — the same standing the stray-carriage-return golden already has.

## The resolution — provisional, author ratification pending

All conditions adopted. One landing, one commit (plus this record's own):

1. spec § escapes: "Five" → "Six", `` `\r` `` after `` `\t` `` in both
   context lists — the measured wording, 3374 → 3377.
2. `measure/gate.rs`: `SPEC_TOKENS` → 3377, ledger row paid with the
   warden's registered prediction (instruments exist today; scored at
   M-selfhost-port close).
3. `lexer/escape.rs`: the one table gains the `\r` arm; both `legal()`
   strings list six.
4. `ir/print.rs::escape` gains `'\r' => "\\r"` — dumps stop carrying raw
   bytes.
5. **A raw CR inside a string or char literal becomes a diagnostic**
   (`raw_carriage_return`, certain fix `\r`) — the coherence condition: the
   grounds that admit the escape condemn the invisible second spelling, and
   the escape makes the repair writable for the first time. Verified not to
   touch the stray-carriage-return golden (its CR sits between tokens). New
   golden, `#~`-annotated, carrying the raw byte deliberately.
6. Tests: the two message asserts; `\r` decode asserts in both contexts; the
   residual-trap test gains `"C:\results"`; a scan-level stray/tolerated
   pair (the tolerated `\r\n` direction was untested everywhere).
7. `tests/golden/check/escapes.expected`: one line hand-edited, diff quoted
   in the commit body; `UPDATE_GOLDEN` unused (forbidden there).
8. design.md: the escape table row; the frozen-set sentence amended to keep
   `\0`/`\xNN`/`\u{…}`/octal frozen on the NUL ground; the residual-trap
   paragraph names the third letter.
9. The port mirrors it: `selfhost/escape.hero` (table + `legal`),
   `selfhost/literals.hero` (validator + the raw-CR diagnostic),
   `selfhost/layout.hero` (the constant's body becomes `'\r'`, its
   now-false comment rewritten to the scanner's own fact),
   `selfhost/scan.hero` (the untranslatable-note deleted, the two-direction
   test written — the warden's payment, collected early).
10. Records, by append: panel 008 (in its vocabulary: the freeze reaffirmed
    for the NUL class, reopened for the question it never heard);
    measurement 009 gaps 7+10 (closed, and the false Rust-twin premise
    corrected); every "five escapes" prose site updated — acceptance:
    `grep -ri "five escapes"` hits nothing outside dated records.

**What a veto at ratification would compel**: reverting items 1–9 wholesale
(one revert commit — the record stays); the corrections in item 10's false
premise land regardless, on the warden's first ruling, which no seat
disputed.

## Predictions to score

| judge | scored at |
|---|---|
| ergonomist: under five escapes, no corpus program can emit CRLF in pure Heroes | **scored now, CONFIRMED**: its named milestone (M-ffi-ladder) has passed, and the corpus at this sitting's date contains zero string literals with CR — the program could not exist, which is this sitting's own ground 1 |
| historian: no byte outside {9,10,13,34,39,92} needed in fixpoint-corpus literals | M-selfhost-fixpoint |
| ffi-pragmatist: gap 10 closes with `"\r"`; raw byte-13s inside literals stay 0 | M-selfhost-fixpoint |
| spec-warden (the payment): the scan test with `"\r"` + the two goldens | M-selfhost-port close |
| compiler-engineer: scan test green, `grep -c $'\r' selfhost/*.hero` = 0; byte-identical `unknown_escape` from both binaries | M-selfhost-port; M-selfhost-fixpoint |

## Process notes

The working tree was frozen from briefs to this synthesis; all five judges
worked in copies with `target`/`build` removed (panel 056's rule).
**The coordinator briefed a false premise** — "the Rust twin test exists" —
that two seats independently caught (warden by grep and git history, engineer
by the same grep); it had already propagated from measurement 009 into
`scan.hero` and from there into this panel's own brief, which is CLAUDE.md
§11's expiring-premise class traveling at record speed: three files in one
day. **The second-ruling mechanism**: the warden's veto adjudicated only the
briefed premise; four seats' grounds were put to it after its first ruling,
it verified them itself (executing ground 3 live), conceded that its own
cited mitigation *was* the trap, and lifted — the veto was not argued down,
it was out-evidenced. The sitting also leaves one standing defect on the
record independent of this proposal: `unknown_escape`'s `certain` fix has
two plausible intents for escape-like inputs, and §8's tag semantics say
that is a `guess` — repaired for `\r` by this landing, and worth a look for
the general class.

## Author's verdict

**Ratified in full, 2026-08-16** (author instruction *"ratifica e sistema
tutto"*, blanket). All ten items of the resolution stand as landed: the spec edit
and its ledger row, the lexer table arm, `ir/print.rs`'s escape so dumps stop
carrying raw bytes, the tests, the hand-edited `escapes.expected` with
`UPDATE_GOLDEN` unused, the design.md amendments keeping `\0`/`\xNN`/`\u{…}`/
octal frozen on the NUL ground, the port's four mirrored files, and the appended
records.

**The coherence condition is ratified as the substance rather than as a rider.**
`raw_carriage_return` is what makes this sitting more than a table row: the
grounds that admit the escape condemn the invisible second spelling, and the
escape is what made the repair writable for the first time.

**And the process note is ratified with it.** The spec-warden's veto adjudicated
only the coordinator's own false parity premise, and that seat lifted it on its
second ruling — after verifying the other seats' grounds itself, including
executing the raw-CR trap live rather than reasoning about it. A veto that
investigates its own basis and withdraws is the mechanism working. It is worth
recording here because 066, 067 and 068 were each briefed with something false
and the seats caught all three; this is the sitting where the catching was done
by the seat that had raised the objection.


## The payment, registered at M-selfhost-port close (2026-08-17)

Both seats' conditions are **MET**, measured at the close rather than asserted.

The spec-warden's (i): `selfhost/scan.hero` carries
`test "a stray carriage return is named and a crlf line ending is tolerated"`,
whose body is `line_body(@l, "print(1)\rprint(2)\n")` — **written with the
escape this panel bought**, which is the whole point of the clause: before it,
that test could not be written at all. A second test,
`"a raw carriage return inside a literal is refused with the escape as its
repair"`, asserts the fix's replacement is `"\\r"`.

The compiler-engineer's: `grep -c $'\r' selfhost/*.hero` is **0 for every one
of the 143 files**.

The clause is paid, not lapsed. Its fixpoint half — *"both binaries render
byte-identical `unknown_escape` messages"* — is discharged by a stronger
measurement than it asked for: the two compilers' emitted C is byte-identical
over the whole 20,886,539-byte translation unit, and the differential compares
every diagnostic of five commands over 257 inputs with zero divergences.
