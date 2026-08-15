//! The delta gate: the spec cannot grow without a commit saying so (§1.6).
//!
//! Panel 024 raised the ceiling to 4096 and queued two instruments, because the
//! raise put the budget's veto out of reach: at the observed ~+30 per amendment,
//! 4096 binds after about sixty more panels where 3000 would have bound after
//! twenty-five. Its own precedent is PEP 8 — 79 columns is intact in the document
//! and irrelevant in practice, because Black's 88 decides. **A budget that is not
//! checked on every run is whatever the author's editor tolerates.**
//!
//! Panel 031 adopted the gate as a **test rather than a command**. CLAUDE.md §10's
//! stopping rule refuses it as a subcommand or a flag — it types no fixpoint
//! invocation, no golden harness and no Part 11 harness, which is the refusal that
//! struck `outline` and `explain` — and the golden harness types it here for free.
//!
//! The mechanism is one constant. A spec amendment turns this test red, and the
//! only way to green is to write the new number into the commit that made it true.
//! That is precisely the moment panel 012's rule asks for a named removal or a
//! registered falsifiable prediction, so the gate does not enforce the rule: it
//! makes the rule's question unavoidable, and the answer lands in the commit body
//! where a reader can check it.
//!
//! **What "a prediction" means here was narrowed by panel 046**, and the narrowing
//! came out of this very table: six of its rows were paid with a prediction and not
//! one was ever collected, because each named metric 2 — an instrument scheduled at
//! M-selfhost-fixpoint. A prediction now pays only if it names the instrument that
//! will score it *and* that instrument exists on the day it is registered; one that
//! does not is registered as an observation and pays nothing. The gate still checks
//! none of this — it reads no commit, calls no git, and any form that made it do so
//! is refused by the same §10 argument three paragraphs up. The rows below are the
//! record a reader audits it against.
//!
//! **The six prediction-bought rows — `2588`, `2627`, `2675`, `2745`, `2768`,
//! `2959` — were re-decided at M-program-corpus** (panel 046 R2, ratified
//! 2026-08-13): scored, or `lapsed` with the clause re-argued in `DECIDE.md` under
//! the removal branch, never renewed. The date is here because `/step` greps here.
//!
//! **Done, 2026-08-13. Two scored, four lapsed**, and the split is exactly the one
//! panel 046 R1 predicts: the two that named an instrument existing on the day of
//! registration were collectable, and the four that named metric 2 were not.
//!
//! | row | prediction named | outcome |
//! |---|---|---|
//! | `2588` | first-try rates (metric 2) | **lapsed** |
//! | `2627` | the next spec-only writing experiment | **scored — half held, half falsified** |
//! | `2675` | a `mutate --survivors` run over bitwise code | **scored — held, and vacuously** |
//! | `2745` | first metric-2 harness run | **lapsed** |
//! | `2768` | first-try failures (metric 2) | **lapsed** |
//! | `2959` | first-try failures (metric 2) | **lapsed** |
//!
//! The four lapsed rows keep their clauses in the spec — R1 governs what may be
//! *offered* as payment and is not retroactive — and each clause is back in
//! `DECIDE.md` to be re-argued under the removal branch. None is renewed with a
//! new milestone name, which is the one thing R2 forbids.
//!
//! ## The four, re-decided — 2026-08-14, by author decision
//!
//! **Not renewed. Re-registered**, which is the distinction R2 exists to draw: a
//! renewal differs from its registration only in a date or a milestone name, and
//! each of these differs in **what is claimed and what will answer it**. Every one
//! names an instrument that exists today *and has an arm for the question* (R1 as
//! amended the same day), and every one is countable with `grep` or `heroes
//! mutate` rather than with the metric-2 harness that does not run until the
//! fixpoint.
//!
//! The honest starting position is the one `DECIDE.md` stated: **three of the four
//! are §1.0 compiler-need** — the widths, `i64` and the bases are all on the road
//! to the port — so for those the re-decision is *keep, and here is what would
//! now falsify the clause*. `2588` is the one that is not, and it is the one whose
//! new prediction is written to be able to fail.
//!
//! | row | the clause | re-registered prediction | instrument, and its arm | scored at |
//! |---|---|---|---|---|
//! | `2588` | a group's `constant` has no body | at the next FFI rung, **≥12** `extern constant` declarations exist across `examples/` and `tests/golden/`, and every one names a value **no `.hero` file spells as a literal** | `grep -c "constant [A-Z_]*:"` beside a grep for the same names as literals — both arms exist; today the count is 107 declarations in total | M-ffi-ladder rung 5 |
//! | `2745` | the four bases and `_` | the corpus holds **44** non-decimal literals today; at the next milestone that touches bit manipulation it is **≥44**, and **0** of them are a decimal spelling of a mask | `grep -coE "0x[0-9a-fA-F_]+\|0b[01_]+\|0o[0-7_]+"` over `examples/`, `library/`, `tests/golden/run/` | next milestone touching masks |
//! | `2768` | `int` deleted, `i64` everywhere | `grep -c "\bint\b"` over every `.hero` in the repository stays **0** while `i64` stays **≥374**, and no diagnostic transcript in `tests/golden/` names a width the author did not write | two greps and the golden corpus, both live | every milestone close |
//! | `2959` | the eight widths | the corpus holds **50** narrow-width annotations today; **≥40 of them are at an FFI boundary**, and removing the widths would make each a silent truncation that `ffi_parameter_type` now refuses — countable by deleting the widths in a scratch tree and counting the diagnostics that stop firing | `heroes check` plus the `ffi_parameter_type` class built at M-binding-fidelity, which is the arm that did not exist when the row was written | M-ffi-ladder rung 5 |
//!
//! **What makes `2959`'s the strongest of the four**: its original prediction
//! named metric 2 because, on the day it was registered, nothing else could see
//! the widths at all. `ffi_parameter_type` can — it exists because of them — so the
//! re-registration is not a rewording, it is the first time the question has an
//! instrument. That is the shape R1's second half asks for, arriving on its own.
//!
//! The two scored rows are in `docs/measurements/007`, and both found something
//! the row itself did not predict. `2627` bought a reader who avoids `+` in a
//! loop and reaches for `push` in one instead — the clause names `push`'s copy
//! and the reader did it anyway, because with no lambda and no `repeat` the
//! document leaves no other way to build the array `join` needs. `2675` holds
//! because **no operator in `heroes mutate` makes the mistake it is about**: a
//! prediction can name a live instrument and still be uncollectable if the
//! instrument has no arm for the question.

/// The spec's measured size, `max` over both vendored instruments — the binding
/// number, never an estimate.
///
/// Every change to this constant belongs in the same commit as the spec change
/// that caused it, with the delta and its justification in the commit body.
///
/// | value | when | what moved it |
/// |---|---|---|
/// | 2231 | panel 023 | the `[T]` clause §1.6 says must survive a raise |
/// | 2363 | M-generics-library | `range`'s labels, the tier phrase, panel 028's prelude |
/// | 2434 | panel 031 | modules: `use`, qualification, the transitivity rule |
/// | 2422 | panel 035 | the thirteen silences, closed **net −12**: one line deleted, four added |
/// | 2560 | panel 036 | the FFI group and the program's edges, **net +138** against two removals worth −31 |
/// | 2588 | panel 038 | a group's `constant`, **+28** with no removal available: the two the record still listed had already been spent by `6a58d47`, so this is panel 012's *other* branch — a registered prediction (the llm-ergonomist's first-try rates, scored at M-program-corpus). +1 of the 28 is the `@out: ptr` repair, which fixed an example that ran to completion doing nothing |
/// | 2627 | panel 037 | the cost of building a string or an array, **+39**, panel 012's registered-prediction branch again — the prediction is written into `docs/panel/037-array-growth.md` § Ratification and scored at M-program-corpus. The clause the ergonomist asked for was the `join` half at +23; the extra 16 buy `push`'s copy, without which the sentence would send a reader to build the `[str]` quadratically instead of the `str` |
/// | 2675 | author decision, `docs/panel/040` | the bitwise set becomes real, **+48** — a row in the operator fence and five levels in the precedence chain. Panel 012's registered-prediction branch: no removal was available, because panel 036 had already deleted the spec's own sentence naming the six reserved spellings. The prediction is in the panel file and scored at the next FFI rung |
/// | 2745 | author decision, `docs/panel/041` ratified | the four bases and the `_` separator, **+70** in three lines — `0x1f` `0o37` `0b11111` `31`, a leading zero refused, and the value reading stated so a reader does not have to guess whether `0xffffffffffffffff` is `-1`. Panel 012's registered-prediction branch, and the **removal was refused rather than unavailable**: the spec-warden priced spec lines 152–153 at −39 as stray performance advice, and those lines are `2627`'s own entry two rows above — panel 037 added them deliberately on the llm-ergonomist's ask, and their prediction is still unscored. Deleting them would have spent another sitting's live measurement to fund this one. The prediction registered instead is in `docs/panel/041` § Predictions scored, clause 7's live half: **≥25% of unprompted attempts at a mask or a copied header constant reach for a base other than decimal**, scored at the first metric-2 harness run, and under 10% the notation is a road nobody takes |
/// | 2768 | author decision, `docs/panel/042` ratified | **`int` is deleted**, `i64` everywhere, **+23** — and the delta is entirely the tokeniser rather than the language: cl100k spends one token on ` int` and two on `i64`, and the document names an integer 21 times. No rule was added, no rule removed. The author's ground is §1.1's: `int` is a word carrying forty years of conflicting widths and a reader must know the platform to know what it means, while `i64` is ambiguous to nobody — and the 545 occurrences across 168 `.hero` files that this cost to migrate were 545 places carrying a number nobody could read off the name. Panel 012's registered-prediction branch, and the prediction is `docs/panel/042`'s #4: integer-width mismatch diagnostics are **≤5%** of first-try failures, above **15%** the widths are a net §1.2 loss |
/// | 2959 | author decision, `docs/panel/042` ratified | **the eight widths**, `+191`: two table rows in place of one, the no-implicit-conversion rule extended to widths, the `fit_<width>` family, the literal's context rule with its example, and "overflow aborts at every width". That is the whole of design.md:833's *"most expensive spec item that exists"* — and it came in at less than the +189 first drafted because **half of it was already paid**: §4.3 forbids implicit conversions outright, so there is no promotion lattice to specify, which is the part of C's rules that does the damage. Panel 012's registered-prediction branch, no removal available and none proposed after the one on offer turned out to be another sitting's unscored deliverable (see the 2745 row). The predictions are `docs/panel/042` #4 and #5 |
/// | 2956 | same sitting, `docs/panel/042` | `s[i]` becomes a `u8` and a widening becomes infallible, **−3**: *"yields an `i64` in 0..255 (a byte)"* was a type and a range and a gloss where *"yields a `u8`"* is a type, and the conversion clause gains six words. The rule the author gave is *"the precise type, not the widest one that holds it"*, and the spec got shorter by following it |
/// | 2963 | `docs/panel/043` | the cost paragraph restated declaratively and `join`'s shape given, **+7 net** — and this is a **removal disguised as an addition**. Out: the imperative *"Build a long string with `join`, not repeated `+`"*, and *"a long array in chunks"*, which **advised the impossible** (`[i64] + [i64]` is `bad_operand`; the language has no array concatenation and never had). In: `join(xs, sep)`'s shape, which panel 043's llm-ergonomist had to guess in **both** blind variants and named as the paragraph's real payload. The rule the sitting established: the spec may state **what an operation costs** — design.md §4.10:1327 already models it, *"Price, declared: mutating a shared array copies it, O(n)"* — and may **not** instruct a reader to prefer a construct for speed, because that binds the author on a premise about the implementation, which is CLAUDE.md §11's world-premise. C++17 removed `register` for being the second kind and broke real builds |
/// | 2974 | author decision, `docs/panel/045` | **`fit_<width>` becomes `to_<width>`**, +11, and the delta is bought rather than spent: the family joins the scheme panel 017 established when it renamed `.str()` to `to_str` *"so the three conversions share one scheme and a model can derive the third from the two the spec already lists"*. `fit_` broke exactly that — a model knowing `to_i64`, `to_f64` and `to_str` cannot derive `fit_u8` — and panel 044's blind judge proved it by reaching for `to_i64(text[i])` and noting its own miss. `to_i64` absorbs the old `f64` conversion and becomes fallible with the rest, because one name cannot carry two failure models; the clause now states the rule (*the name says whether it can fail*) instead of listing two families, which is what the +11 buys |
/// | 2983 | panel 048 | **a false sentence repaired**, +9 and the cheapest kind of amendment there is. § FFI said *"A group names its header and its library"* — and **13 of 20 `extern` groups in this repository name no library**, including `library/source.hero`'s `extern "hero_os.h"`, which is the **closure list's only `extern` group**. Read under §12, the spec made the port's own library source illegal and the compiler buggy for accepting it. Now: *"A group names its header, and `link` a library when the symbols need one."* **Paid as §1.0 compiler-need, not with a prediction and not with a removal** — the warden's removal search over six candidates returned nothing, and four of the six are themselves compiler-need. Three judges found this sentence independently while judging something else entirely: the two clauses the sitting was convened about (+38 and +54) were both **refused**, and the only thing that landed in the spec is the repair of what was already there |
/// | 3106 | author decision, `docs/panel/050` | **`package`**, +123, and the author's instruction was to spare no expense: *"the FFI is central, spend the tokens needed, make it work fully."* Paid as **§1.0 compiler-need** on the strictest available reading — §4.19's own acceptance ladder had a rung that could not be climbed, measured four ways at panel 049. One spelling replaces a whole platform axis: `pkg-config` answers with frameworks on macOS and `-lGL -lX11 -lm -ldl` on Linux, so the +144 platform proposal panel 049 refused is subsumed at less cost and without putting a machine's name in a program. The clause names its allow-list in the spec because the allow-list *is* the feature's safety: Go shipped this idea without one and it became CVE-2018-6574 |
/// | 3140 | `docs/panel/051` | **the assertion checks results and never parameters**, +34. The sentence said *"clang checks every signature … so a wrong FFI type is a compile error"*, and two judges falsified it with running programs: `putchar(c: i64)` given `4294967361` prints `A` at exit 0, and a C `int narrow(int)` given `4294967301` prints `5` under this project's exact flags, silently. The half that is true is the result type, which `_Generic` checks; the half that is false is every parameter, and **267 of 643 bindable entry points across `sqlite3.h`, `curl/curl.h` and `raylib.h` — 42% — take one that is not 64 bits**. §12 says the compiler has the bug, so the repair is §1.0 compiler-need. The asymmetry the clause now states is **principled and was measured, not assumed**: a result may be wider than C's because an `unsigned int` always fits an `i64`, and a parameter may not be, because that narrowing is the one clang performs in silence. `examples/sqlite/` moved to the header's own widths in two edits and no shim; `examples/curl/` moved its parameters and **could not move its results**, because `CURLcode`'s compatible type is unsigned and `HERO_RET_I32` refuses it correctly. Panel 012's registered-prediction branch: the llm-ergonomist's first-try width rates, scored at the next ladder rung. The warden's `−8` removal is **not** spent here — it belongs to panel 037's live clause and funding this sitting from it would repeat what panel 041 refused |
/// | 3141 | `docs/panel/052` | **and sign**, +1 — the cheapest amendment in this table, and it exists because the compiler grew a check the sentence did not describe. Panel 051 wrote *"at the header's own width"* and `examples/curl/` immediately produced the other half: `curl_easy_setopt` takes `CURLoption` and `curl_easy_strerror` takes `CURLcode`, **two enums in one header whose compatible integer types have different signedness**, so `i32` is right for one parameter and wrong for the next. `-Werror=sign-conversion` catches it and `ffi_parameter_type` reports it as one class with the width, because the repair is one sentence. Paid as §1.0 compiler-need under §12: the compiler now refuses what the spec permitted |
/// | 3158 | `docs/panel/053` | **the sentence about `==` was false, in the silent direction**, +17. It said *"structural equality on everything"* while `==` on a `cstr` compares **addresses**: measured, `a == b` is `true` and `a.cstr() == b.cstr()` is `false` for the same two strings, at exit 0 with no diagnostic. Panel 048's exact class — a spec that describes something the compiler does not do — and §12 says the compiler has the bug **or the spec does**. Here it is the spec, and the robustness rule decides which: a structural `==` on a `cstr` would have to dereference, so a null one would be the crash CLAUDE.md §12 forbids, while an address comparison is total and is what `v == nullptr` asks. Paid as §1.0 compiler-need with no removal and no prediction, on panel 048's precedent at row `2983`. Landed the same day as panel 053's own `nullptr`, which is the feature that made the false sentence load-bearing: the operator you check nullability *with* was described wrongly in the one document that describes it |
/// | 3191 | author decision, `docs/panel/048` | **X returns, at the wording that is true**, +45 against a −12 removal, net **+33**. Panel 048 refused *"the headers are read as a POSIX C compiler presents them"* because the ffi-pragmatist compiled 30 probes across four libcs and found it **false on glibc** — `strptime`, `wcswidth`, `wcwidth` and `swab` are declared on Darwin, musl and FreeBSD and hidden on glibc under the ratified `gnu11`. What lands is that judge's own counter-wording, the only version true 4-for-4: *"A header shows more than ISO C's names — `M_PI`, `strdup` and `fileno` are usually there. How much more is the platform's answer, not this language's."* It states a **floor** rather than an identity, which is the whole repair. The removal is panel 052's R2, verified unspent and spent here: `to_i64` was listed **twice in one line** of Built-ins, and its truncation is already stated in § Types. Panel 051's −8 stays unspent on panel 053's warden's advice — a blind reader quotes that sentence as the document's only cost claim. Panel 012's registered-prediction branch, under R1 **as amended today**: the instrument must have an arm for the question |
/// | 3200 | `docs/panel/054` | **`repeat(s, n)`, and a cost clause that finally names an exit**, +14 against the −5 removal the sitting found, net **+9**. `docs/measurements/007` measured a blind reader avoiding the quadratic `+` — naming the clause that stopped it — and then writing `push`-in-a-loop in **two of three** programs, because the clause names `join` as the linear escape and `push`, the only way to build the array `join` needs, as the trap: **the escape hatch and the trap were the same sentence**. The rule the sitting adopts is that a cost claim is admissible only where the same paragraph names a construct the reader can reach for, so `join` gains `repeat` beside it. The count is a **`u64`** — the historian's Nim precedent written in this language's own types — so a negative count does not compile and `repeat(" ", col)` on a computed `col` goes through `to_u64().must()`, which aborts at the subtraction rather than inside the primitive. That is design.md §1.12's test applied: a defensive check must **surface** a defect, not hide it, which is why returning `""` lost despite `range(from: 0, to: -1)` doing exactly that. **`repeat` is not compiler-need and the ledger should say so**: two judges checked the port's own diagnostic renderer and found its padding is a tab-preserving per-character map, not `repeat(" ", col)`. It is here for the banner, which is the one program with no route. The removal is R-a, found this sitting: `spec:144`'s *"integer overflow aborts"* duplicated `spec:62`'s *"Overflow aborts at every width"*. Panel 048's −8 stays unspent — spending it here would make the row read *"a built-in for +6"*, which is the laundering panel 041 refused |
/// | 3208 | `docs/panel/055` | **a group head names, it does not locate**, +14 against a −6 repair, net **+8**. The sitting inverted twice, both times on a measurement. It was convened to decide whether to **add** a way to name a search path, and found one **already there**: `extern "/opt/foo/include/foo.h"` compiled and ran, because the string is passed to `#include` and C accepts a path — inherited from Nim's `header` pragma, which CLAUDE.md §6 says to copy, so §6 worked and bit in one act. Then it found the other door shut by this project's own hand: **28 of 285 `.pc` files (10%)** answer with a flag panel 050's allow-list rejected, and **SDL2 was bindable through neither clause** — `package` refused `-D_THREAD_SAFE`, `link` could not find the header, and each diagnostic sent the author to the other. Panel 050 had cited Go's allow-list and adopted one **narrower than Go's**, which documents `-D`, `-U`, `-I`, `-l`. The refusal is narrowed to **absolute** spellings, because `sub/bar.h` resolves against the `.hero` file's own directory and is portable, and `curl/curl.h` is path-shaped and right. The **−6** is a §12 repair rather than a saving: the operator fence still said *"i64 with i64, f64 with f64"* after panel 042 landed the eight widths, while `u8 200 + u8 55` prints `255` at exit 0 — the third false sentence found in this spec in one day, and the third in the **silent** direction |
/// | 3210 | `docs/panel/058` | **`s.cstr()` *lends* a `str` to C *to read***, **+2** — the cheapest amendment in this table after `3141`'s +1, and the sitting's own finding is that it should have been. Panel 058 was convened with two candidate clauses on the ballot, measured at **+32** and **+55**; the spec-warden re-ran both, then wrote the sentence they were both reaching for, and it is two words. **The expensive one was also unsafe**: *"may read … and never write"* **licenses** a C function that *keeps* the pointer, which is the use-after-free design.md:2064 defers to Part 7 item 10 — and the llm-ergonomist, reading only the spec, reached the same hole blind. The clause pays for itself as §1.0 compiler-need under §12: the compiler now refuses a `char *` parameter at exit 1 (`ffi_writable_parameter`), and a compiler refusing what the spec permits has the bug. `.cstr()` is zero-copy over a **refcounted copy-on-write** buffer, so `b = a` then `strtok(a.cstr(), …)` changed **both** at exit 0, and on a literal it is SIGBUS — design.md §1.12 twice over |
/// | 3235 | `docs/panel/059` | **the read direction, folded into the sentence that was already there**, +25 — and the ballot's *cheap* option was the dangerous one. Three clauses were tabled: name the conversion (+11), name it and say a null aborts (+28), make it fallible (+26). **The +11 is refused on a measurement**: the llm-ergonomist, blind, wrote `getenv(…).to_str()` then `len(v) == 0` **first and confidently**, because spec:58 says *"the name says whether it can fail: `to_str` and `to_f64` cannot"* — naming the conversion inside that law positively asserts a null converts. Not knowing whether `to_str` even applied to a `cstr` is what had been sending readers hunting, and A ends the hunt without ending the hazard. **The fallible form took a veto** (+37 honest once spec:58 is repaired too) and an objection no wording answers: ncurses `tigetstr` returns `(char *)-1`, so a `str?` advertises a guarantee the boundary cannot honour — SIGSEGV with the null guard passing. What lands states three facts and buys the escape with the last eight tokens: it **copies** (`str.c:241` memcpy, proven against `ctime` returning one pointer twice), a null **aborts**, and the guard is `c == nullptr` **first**. The abort is a **floor, not an identity** — there are two paths, null and malformed UTF-8, both measured — which is the shape panel 048 ratified and the fourth false-in-the-silent-direction sentence avoided. Paid as a **§12 repair** with no removal, on rows `2983`, `3141` and `3158`: design.md §4.19:1989 and a shipped diagnostic (`types/decls.rs:237`) both name `to_str` already, so the spec had the bug |
/// | 3329 | `docs/panel/060` + author instruction | **a struct crosses as the header's own type, and `f32` arrives with it**, +116 against the −22 removal below, net **+94** — the largest amendment in this table, and it is two decisions rather than one. The **record clause** (+38 with its removal) is panel 060's, at the spec-warden's wording rather than the proposal's +137: that draft was false in both directions, because `ptr` and `cstr` are **not in § Types** — so *"fields are this document's types"* forbade a pointer field, and **130 of the 192** no-`f32` raylib entry points need one, shipping **62** of the 191 it was bought for. It also promised *"a wrong order is a compile error"*, which the adopted mechanism **cannot emit**: no typedef is written and every access is by name, so order is immaterial and clang has nothing to check — a sentence that would have made the compiler buggy by fiat under §12. **`f32` (+56) is the author's decision of 2026-08-15 overturning the sitting's own deferral**, and the record says so rather than dressing it as a panel conclusion. Panel 060 had priced deferral honestly — under this mechanism the layout is never ours, so an `f64` field over a header `float` is ABI-safe, measured against real raylib — which took `f32` from a **reachability** question to a **precision** one. **That escape clause was written into this document and then taken back out the same day**, and the reason is §12 rather than tidiness: the field probe asks type *identity*, so the compiler refuses `x: f64` over a C `float`, and a spec sentence permitting what the compiler refuses is a false sentence. It existed only to make the deferral survivable, and there is no deferral. What the width costs in this document is not the table row: it is the row (+22 measured) plus `to_f32` in a closed built-ins list that must not disagree with the prose, plus the sentence saying nothing fails to fit a float, plus printing. Panel 052's own warden had called the +22 *"understated"* for exactly the second of those. The **removal** is `spec:207`'s package allow-list enumeration, and it is a **§12 repair** as much as a payment: *"Only `-I`, `-L`, `-l`, `-F` and `-framework` are accepted back"* has been false since panel 055 — `commands/libraries.rs:138` accepts nine spellings, `-D` and `-U` among them, landed because SDL2 was bindable through neither clause — so read under §12 the spec made `package "sdl2"` illegal. The **fifth** false sentence of that class, and the fourth in the silent direction. Panel 012's registered-prediction branch, under R1 as amended by 046: the instrument is `heroes check` plus the `run/` golden harness plus `heroes mutate` under `--sanitize`, all three of which exist today and all three of which have an arm — `ffi_type` fires on a struct at the boundary right now |
/// | 3360 | `docs/panel/061` + author ratification | **`partial`, and the sitting that found the feature already shipping**, +35 for the clause against a **−4** removal, net **+31**. The ballot offered three spellings and the spec-warden showed all three were **false**: A declares this document's own `Point(x: 3, y: 4)` a compile error and re-uses `...` nine words after defining it (vetoed); B bought room by deleting *"all its fields"*, which **legalises the hazard by omission**; E breaks every group `record` written and rests on a reading of Nim the source refuses. What lands is B's word at the warden's wording, and the removal is a §12-shaped dedup rather than a saving: *"at the header's own width and sign"* was stated twice, fifteen lines apart, and is now stated once for **a parameter and a field** — true both ways, since `spec:194` already says clang checks fields. **The word buys exactly one thing**: `extern_record.rs`'s completeness probe stops asking. Everything else it does is a refusal — `==`, `hash`, a map key, **transitively**, because `[Font] == [Font]` reaches `Font_eq` through `hero_array_eq` and a variant case's payload reaches it through `variant_equality_body`, both measured firing | **Construction is permitted, and the ffi-pragmatist threatened a veto to keep it there.** Refusing it deletes the only expression in the language that produces an `SDL_Event`, so `SDL_PollEvent(@e)` — the binding the feature was adopted for — needs a hand-written shim forever; and it buys nothing, because the zero-fill is **deterministic** rather than undefined and the identical SEGV is reachable from a **complete** binding whose `ptr` field is `nullptr`, measured at exit 134. What defends the author instead costs **zero spec tokens**: a **positional** completeness probe, `Font f = {0,0};`, which `-Wmissing-field-initializers` names by field — found independently by two judges and on nobody's ballot, and armed inside a local `#pragma` region because the emitter writes `= {0}` for every refcounted slot (panel 021) and a global flag fires on its own output. The reach is **not** uniform and making it uniform was measured to cost the milestone its use case: comparison is transitive, construction is **per-declaration**, because raylib's own `RenderTexture { Texture texture; }` would otherwise be unbuildable. Panel 012's registered-prediction branch under R1 as amended: `heroes check` plus the `run/` golden harness, both live, both with an arm — `ffi_partial_operation` and `ffi_incomplete_record` fire today | 060, 061
/// | 3374 | `docs/panel/062` + author instruction | **a C array member, and the sitting closed a veto rather than a question**, +20 against a **−6** §12 repair, net **+14** — against the ballot's own three options at **+52/+53/+54**. Two of the ballot sentence's three clauses were **false against the compiler being written under it**: it promised indexing, `len` and `for`, and `c.params[0]` was `not_indexable`, `len` was `bad_operand`, `for` was `not_iterable`. The sitting that repaired the seventh false sentence would have written the eighth at 2.6× the price of a true one. *"never a `[T]`"* forbids all four in five tokens and stays true after indexing lands, because it is a claim about **representation** rather than a list of operations — panel 048's floor-not-identity, one type later. The removal is the **eighth** false sentence: `spec:204`'s *"Both are names, never paths"* while `extern "sub/bar.h"` runs at exit 0 **and `machine_locked_path`'s own note recommends it** — the spec forbade what the compiler advises | **The compiler-engineer vetoed the shape and the veto was worth more than the ballot.** Three reachable paths could not be emitted soundly and the whole harness was green through all of them, the worst being that **no field assertion was emitted at all**: `c_spelling` returns a name the caller suffixes with ` *`, and C's pointer-to-array declarator is `T (*)[N]` rather than `T[N] *`, so there is no string for which that composes — the arm answered `None` and `leftLensCenter: f64[2]` over a header's `float[2]` ran at **exit 0 with the field unchecked**, which is the class this milestone exists to prevent. It arrived, in that judge's words, *"as a `None` nobody chose"*, through the arm whose own comment says the next type to reach it should be a `None` somebody did. Closed as a **branch** rather than a row. The other three: `hash` folds elementwise so it agrees with `==` (a `_ =>` there would have let two byte-identical structs collide), the form is **refused outside an `extern` group** — where all three judges who examined Q3 arrived independently — and a computed index carries a compiler-emitted guard, because the historian established that **ASan cannot see an intra-object overflow in a C struct at all** and `-Warray-bounds` was measured not to fire on a temporary subscript, so the subscript stays on an array-typed lvalue and the check is ours. raylib: **all 35 structs, all 349 by-value crossings, zero `partial`** | 060, 061, 062
pub const SPEC_TOKENS: usize = 3377;

/// The reserved-word registry's own size, gated separately — **not** part of
/// §1.6's budget, and that is the ruling rather than an omission (panel 035 D).
///
/// §1.6 says the spec "is **the prompt**", singular, and
/// `harness/prompts/first-try.md` makes it operational: the context is the
/// template plus `spec/heroes-spec.md` plus one task. The registry is §4.17's
/// *compiler output*, delivered at the moment of the mistake; it can never be in
/// a prompt, so it cannot spend a prompt's budget.
///
/// It gets a number anyway, because it grew **+85 across eight milestones with
/// no commit ever naming a delta** — 831 at M-day-zero, 916 today — for the plain reason
/// that `heroes measure` defaults to the other file. Gate it, do not merge it.
pub const RESERVED_WORDS_TOKENS: usize = 916;

/// The ceiling panel 024 set, asserted at **compile time**: `SPEC_TOKENS` is a
/// constant, so a runtime `assert!` on it is optimised out and tests nothing.
/// Raising either number is a panel, and this line is what makes forgetting one
/// of them a build failure rather than a green suite.
const _: () = assert!(SPEC_TOKENS < 4096, "the spec is over panel 024's ceiling");

#[cfg(test)]
mod tests {
    use super::*;
    use crate::measure::{measure, spec_path, vendor_dir};

    /// The gate. It fails on any spec change, which is the point: the number
    /// below it is a record of every amendment this project has made.
    #[test]
    fn the_reserved_word_registry_is_gated_too() {
        let path = spec_path().parent().expect("spec/").join("reserved-words.md");
        let text = std::fs::read_to_string(&path).expect("the registry must exist");
        let measured = measure(&text, &vendor_dir()).expect("the tables load");
        assert_eq!(
            measured.max(),
            RESERVED_WORDS_TOKENS,
            "the registry measures {} and the record says {RESERVED_WORDS_TOKENS}. It is \
             not in §1.6's budget (panel 035 D: the spec is *the prompt*, and compiler \
             output can never be in one) — but it grew +85 across eight milestones with \
             no commit naming a delta, because `heroes measure` defaults to the other \
             file. Write the new number down.",
            measured.max()
        );
    }

    #[test]
    fn the_spec_measures_what_the_record_says_it_measures() {
        let text = std::fs::read_to_string(spec_path()).expect("the spec must exist");
        let measured = measure(&text, &vendor_dir()).expect("the tables load");
        assert_eq!(
            measured.max(),
            SPEC_TOKENS,
            "the spec measures {} and the record says {SPEC_TOKENS} — a delta of {}. \
             If the change is intended, write the new number into `SPEC_TOKENS` in the \
             SAME commit, and put the delta plus its payment in the commit body: either \
             a named removal, or a registered falsifiable prediction that names BOTH the \
             instrument that will score it and the milestone at which it is scored — and \
             the instrument has to exist today (panel 012, as amended by panel 046). A \
             prediction naming an instrument nobody has built is an observation, not a \
             payment, and six rows of this ledger were bought with one. If it is not \
             intended, the spec grew by accident, which is the case this test exists for.",
            measured.max(),
            measured.max() as i64 - SPEC_TOKENS as i64,
        );
    }

    /// The one thing §1.6 says must survive every raise: the headroom is real, not
    /// notional. Panel 024's own arithmetic put the justified total at 3181.
    #[test]
    fn the_headroom_the_remaining_mortgages_need_is_still_there() {
        // The floor for file I/O, `args()` and `exit(code)` — panel 030 R3, which
        // refuted "they are plain externs" by compiling it. The floor belongs to the
        // FFI, not to the milestone that pays it, so the name says so.
        let ffi_floor = 60;
        assert!(
            SPEC_TOKENS + ffi_floor < 4096,
            "modules plus the FFI floor is {} against 4096",
            SPEC_TOKENS + ffi_floor
        );
    }
}
