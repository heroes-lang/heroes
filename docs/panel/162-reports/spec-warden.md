# Panel 162 — spec-warden

**verdict: approve, and only in one shape** — the two halves MERGED into § 13's
existing sentences, `validated` widened to a fixed byte field and `repeat`
widened to build one. **I object to route 1 as § 11 text** (+99 against +45,
measured, and it takes on an instrument obligation the merge does not). **I
refuse route 4, refusing**, because the fact that would make that refusal wrong
is already in the repository and I ran it: the plausible wrong program compiles
at exit 0 today. **No budget veto**: the full resolution lands at **8041** real
against a ceiling of **10240**.

Every number below was produced in this session, on 2026-09-18, in a scratchpad
COPY of the tree with its own seed compiler
(`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`). The repository
working tree was not touched.

---

## 1. The budget, on the instrument design.md §1.6 names

`docs/design/design.md` §1.6, grepped this session rather than recalled:
**10240 tokens, measured by `claude-opus-5` through
`POST /v1/messages/count_tokens`.** The payment rule is unconditional at every
level.

`./heroes measure spec/heroes-spec.md --refresh`, `. ./.env` first, run in the
copy. **Baseline: 7984 real** (digest `6bdb9b497a141864`), which is the brief's
number confirmed rather than copied.

| draft | real | delta |
|---|---|---|
| **baseline** | **7984** | — |
| one name added to § 11's `Built-ins:` sentence, nothing else | 7989 | **+5** |
| **route 1 as § 11 text**: the name plus a conversion sentence | 8083 | **+99** |
| **read half merged into § 13's `validated` sentence**, verbose wording | 8041 | +57 |
| **read half merged**, tight wording | 8029 | **+45** |
| **build half merged into § 13's field sentence** (`repeat(x, n)`) | 8002 | **+18** |
| both halves, merged, unpaid | 8047 | +63 |
| **both halves + the named removal** | **8041** | **+57** |
| the named removal alone (§ 3's two `cstr`-address sentences merged) | 7978 | **−6** |

**Answer to the seat's question 2, exactly: a name in the `Built-ins:` sentence
costs 5 real tokens.** That prices route 1's floor and nothing else.

**Answer to question 1 — where the rule lives: § 13.** `.claude/rules/spec-shape.md`
§ Where a rule lives puts a rule in the section of the operation it governs, and
the operation here is *bytes that arrived through the boundary become text*.
`validated` is already that operation and is already in § 13. Two instrument
facts make this decisive rather than aesthetic, both read out of the harness this
session:

- `tests/harness/suite_spec.hero:653` — the `offered` check is **one-directional**:
  every name in the spec's sentence must exist in the compiler, never the
  reverse. So an FFI name is not owed a place in § 11's sentence, which is why
  `cstr`, `lease` and `end_lease` are not in it.
- `tests/harness/suite_spec.hero:629` — `named` requires each compiler built-in
  to appear in **some** code span. `validated` already appears in § 13, so
  **widening it costs zero instrument work**; a new § 11 name costs +5 and takes
  on the `offered` obligation for ever.

**Merging beats appending, measured: 54 real tokens** (+45 against +99). Panel
122's claim now has a second instance on the binding instrument.

---

## 2. What I ran, and the two things it corrects in the shared brief

Seed compiler built in the copy; every line below is a command's output.

**(a) The silent wrong program compiles, exit 0.** This is the measurement the
sitting turns on and no seat had run it:

```
function show(u: Utsname)
    out: str @ ""
    for i in range(from: 0, to: 256)
        out @ out + u.sysname[i].to_str()
    print(out)
```

`heroes check` → no diagnostic, **exit 0**. It prints `68101...` where the
program meant `Darwin`. A plausible mistake that is not a compile error is the
project's thesis inverted, and **a refusal cannot close it**: `to_str` on a `u8`
is a legitimate conversion and will not be taken away.

**(b) A `char *` field is ALREADY readable. The one-way door is `char[N]` and
nothing else.** Measured:

```
extern "pwd.h"
    record Passwd tag passwd partial
        pw_name: cstr
    function getpwuid(uid: u32) -> Passwd
…  print(p.pw_name.validated().must())      # heroes check: exit 0
```

So the shared brief's route-2 premise — *"`spec § 13` currently forbids a record
holding a `cstr`, so this moves an existing rule rather than adding one"* — is
**false as measured**. § 13's *"outside a group nothing answers `cstr` and no
record holds one"* is scoped by *outside a group*, § 13's own field sentence
already lists `cstr` among a group's field types, and the compiler enforces
exactly that split (`cstr_in_a_record` on an ordinary record, with a note saying
*"A group's `record` may hold a `cstr`"*). Route 2 is therefore an **addition**,
not a move, and it buys the one case that already works. I have no need to reach
the llm-ergonomist's locality veto: route 2's stated saving does not exist.

**(c) The build wall, both routes refused, each by a rule and not by an
oversight.** `u: Utsname` with no initialiser → `expected_binding_symbol`
(§ 5 has two binding symbols and no third). `Utsname(sysname: repeat(0, 256))`
→ `bad_operand: repeat takes str and u64`. And a **zero default** for a fixed
array contradicts design.md **§4.9** in its own words — *"no default values"* —
so that route is not +18, it is an amendment to §4.9 and a separate bill. The
one build route that keeps §4.9 intact uses a name the language already has:
**widen `repeat` to build a fixed array**, still named at construction, still no
default.

**(d) Today's only correct program runs.** The 5 × 256 literal version checks,
builds and prints `68`. So the language is not blocked — it is **priced**, which
is §1.2's subject and the next section.

---

## 3. §1.2 — the rewrite arithmetic, and it is not close

`heroes measure` on the two programs (vendored tables, a **lower bound**, in
those words — `--refresh` refuses every path but the spec and CLAUDE.md):

| the same program | cl100k | legacy |
|---|---|---|
| today's only writable form, one field of five spelled out | **3983** | 2697 |
| the same program after this resolution | **100** | 92 |

**≈ 3883 program tokens saved, lower bound, by one program reading one field.**
§1.2 is `real cost = program tokens × (1 + rewrite rate)`, and both factors move
the same way: a hand-written 256-element literal is a counting task, every
miscount costs a correction round trip of 500–2000 tokens, and `fixed_array_length`
fires on each one. The spec pays **+57 once**; the first program that uses it
repays that **sixty-eight times over** on the pessimistic instrument. I do not
usually get to write a sentence like that, and I checked it twice because of it.

---

## 4. Principle 0, and why I am not refusing

The compiler-need branch is closed and the brief is right that it is: zero
`examples/` programs declare a fixed byte array and the compiler self-hosts
without any of this. So the thesis branch carries it alone, and **I checked
whether the thesis branch can even be measured today: it cannot, by the route
the sitting assumed.** Enumerated from `tests/harness/main.hero` — twenty-one
suite names, and **no first-try-rate instrument and no mutation instrument
exist**. design.md Part 11's metric 2 and metric 3 are still *"build this"*.
**Every prediction in the llm-ergonomist's report names an instrument that does
not exist**, and §1.6's payment rule asks for *an instrument that exists today*.
That is why the payment below is a named removal and not a prediction.

What discharges the burden instead is §2's second branch as CLAUDE.md states it,
*a measured argument the panel accepts*, and the measured argument is (a): the
wrong program compiles at exit 0. **A refusal is held to design.md Part 6's
standard — it must name the program or compiler fact that would make it wrong —
and that fact is `heroes check`'s exit 0 on the byte loop, run on this Mac
today.** A Part 6 row saying *a byte field is read one element at a time* would
have to be written beside a compiler that accepts the loop silently. I will not
sign it.

**The build half enters on a different and weaker warrant, and the sitting
should say so out loud.** Its failure mode is LOUD, not silent, so it is not a
thesis case at all; it is §1.2 and § 13's own opening promise. At **+18** merged
into a sentence that already says how to build a fixed array, and with §4.9 left
untouched, I approve it. At any higher price, or by any route that touches §4.9
or § 5's binding symbols, I would not.

---

## 5. The payment

**Named removal: § 3's two `cstr`-address sentences merged into one, −6 real.**

> *"A `ptr` is a copied ADDRESS, wherever it sits: … A `cstr` copies an address
> too."* → *"A `ptr` or `cstr` is a copied ADDRESS, wherever it sits: …"*

No rule is lost; one sentence is. It is a merge in the same neighbourhood as the
addition, which is what § Where a rule lives asks for. Net **+57**, landing at
**8041 of 10240**: **2199 free, 2139 after §4.19's 60-token FFI floor.**

**And the warden's sentence that must go in the synthesis: headroom is not an
entitlement.** 2199 free is not a licence for the verbose wording. The verbose
read-half draft and the tight one differ by **12 real tokens** for the same rule,
and the tight one is what I approve.

---

## 6. What I approve, in words that can be pasted

Read half, replacing § 13's `validated` sentence:

> `c.validated()` copies one back as a `str?`, and a fixed byte field copies the
> same way, to its first zero or whole; a null `cstr` fails `null_cstr`, bytes
> that are not UTF-8 `not_text`.

Build half, extending § 13's field sentence:

> … never a `[T]`; build one with `[a, b, c, d]`, or `repeat(x, n)` for n of the
> same.

Two things that make this cheaper than it looks, both read out of the compiler:
`validated` is **written in Heroes** (`selfhost/library_source.hero:192`), not a
built-in, and **`not_text` already exists** (line 199) and is already shared with
`read_file` (line 217). So the spec is not choosing a failure mode here — it is
finally **stating one the compiler has had all along**. § 13 names `null_cstr`
and has never named `not_text`, which is a spec that understates its own
compiler; this amendment repairs that at the same time, and that is part of what
the +45 buys.

---

## 7. Prediction, falsifiable, instruments that exist

1. **At the M-readable-bytes close, `heroes measure spec/heroes-spec.md --refresh`
   reads ≤ 8060.** Above that, the landed text is not the text priced here and
   the sitting owes a re-price. Instrument: `heroes measure`, `spec/budget`,
   `spec/recorded`, `spec/ledger`.
2. **`examples/` gains one program that reads a `char[N]` field, and it is ≤ 300
   bytes** against the 4234 of the literal route measured here — a 14× cut,
   checkable with `wc -c`; the `corpus` suite keeps it green.
3. **The byte-loop program gets a golden case.** Either the checker refuses it,
   or `tests/golden/run/` records that it prints digits — one of the two, at the
   milestone close. A silent wrong program that nothing in `tests/golden/`
   mentions after this sitting is the failure I am predicting against.
4. **Zero new names reach § 11's `Built-ins:` sentence**, so `offered` reads the
   same count it reads today.

## 8. Condition — what changes my verdict

- **If the resolution admits `[u8]` as well as a fixed field, the home moves to
  § 11 and so does the price: +99, not +45.** `[u8]` is a language-owned value
  and its conversion is not an FFI rule. I would then ask for the two to be
  priced as one § 11 sentence covering both, or for `[u8]` to wait.
- **If the build half is spelled as a default value, I object**: design.md §4.9
  says *no default values*, and that amendment is a larger bill than this
  sitting has priced.
- **If any wording lands that says a `char[N]` field is NUL-terminated**, I
  object: the shared brief is right that C does not promise it, and *to its first
  zero or whole* is the phrasing that survives a 4-byte tag field.
- **If a first-try-rate instrument is built**, the ergonomist's seven predictions
  become live and the thesis branch gets the measurement it lacks today. Nothing
  in this verdict depends on it.
- **Budget veto**: none at 8041. It would be one at any draft reading above
  10240, and 2199 of headroom is not an argument for a single token more than
  the rule needs.
