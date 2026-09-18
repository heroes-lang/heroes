# Panel 163 — the wall was not there, and the brief said it was

**2026-09-18. M-readable-bytes. Full panel, five seats plus the completeness
critic.** Briefs at `docs/panel/163-briefs/`, reports at
`docs/panel/163-reports/`.

## The proposal, verbatim as it went to the seats

> **How does a program obtain a value of a header record with a 256-element
> array field, so it can hand it to a C out-parameter?**

Four routes: a call typed by context; a zero default; an out-parameter needing no
initial value; or refusing.

## The resolution, in one line

**There was no wall.** A program obtains such a record by asking for one —
`function blank() -> Utsname` in the `extern` group, or in Heroes — and
`partial` lets it declare only the fields it reads. It runs today, it is already
used seven times in this repository's own golden tests, and it costs **zero
compiler lines and zero spec tokens beyond one clarifying clause**.

## What the sitting measured, and it is the whole of the argument

Three seats independently ran the experiment the brief did not. The
ffi-pragmatist's, verbatim from `./heroes run`: `0` / `Darwin` / `arm64`, exit 0,
**not one diagnostic**, identical under `--sanitize`. The emitted C is
`t1 = hero_blank(); h0_u = t1; t2 = uname(&h0_u);` — no marshalling.

**And the completeness critic separated two variants nobody else had.** Three
seats ran the version with a hand-written C shim; there is one with **no author C
at all**. Priced on `heroes measure`'s `maximum` row, a lower bound:

| variant | `.hero` | `.h` | total | author C |
|---|---|---|---|---|
| the full literal, five fields | 3979 | — | 3979 | none |
| the literal plus `partial` | 860 | — | 860 | none |
| **a Heroes producer plus `partial` (4b)** | 874 | — | **874** | **none** |
| a `static inline` shim plus `partial` (4a) | 98 | 61 | **159** | 9 lines |

**That distinction is what saves the resolution from its own rule.**
`.claude/rules/c-boundary.md` says the FFI must be complete, because *a library
Heroes cannot bind is a library the author must leave C code around for* — and a
resolution reading *the answer is a shim* would be adopting the thing that rule
names as failure. **4b binds `uname` with zero C**, so the checkable form of the
question — *is there a library Heroes cannot bind* — answers no. The critic's
condition is honoured here in the resolution's own words: **4b is the route and
4a is an optimisation.**

Two records neither seat cited and the critic found: `design.md:3553` already
says `sqlite3_exec` binds through 28 lines of `static inline` in a header the
group names, and `design.md:497` already says *a C shim you write yourself. This
is a real limitation and it is accepted.* **The sitting rediscovered an accepted
cost and then found the version that does not pay it.**

## The verdict table

| seat | verdict | section | cost measured | condition or veto |
|---|---|---|---|---|
| **compiler-engineer** | **veto** routes 1 and 3; object 2; **route 4 runs today** | design.md §1.7, Part 5 | route 2 is **120-180 lines across five or six modules**, not a checker rule; route 4 is **0** | veto on 1: C cannot return an array, so the call must be erased at compile time and `grep -rn "const_eval\|constant_fold" selfhost/` returns **nothing** |
| **ffi-pragmatist** | **veto** route 3; object 1 and 2; **approve 4, amended** | design.md §4.19, §1.11, §1.12 | the route is **already in the repository**: 25 local headers, 23 with `static inline`, **7 returning a struct by value** in this project's own goldens | veto on 3 is a refusal: uninitialised bytes flowed into `validated_bytes()` as a `str`, a prior frame's `SSSS…` out of `nodename`, **exit 0 with and without ASan and UBSan**; MSan is unavailable on this target |
| **spec-warden** | **veto** 1, 2 and 3; **approve 4** | Principle 0, §1.2, §1.6, §1.12 | real deltas: route 1 **+60**, route 2 **+32**, route 3 **+67**, route 4 **0** | withdraws on route 2 only when three `examples/` programs declare a fixed array longer than 8, **and** a named removal is measured in the same commit |
| **llm-ergonomist** | **veto** route 3; approve 2 conditionally; object 1 and 4 | spec § 5, § 9, § 13 | wrote the 256 zeros by hand and reported it | veto on 3 is non-locality; § 5's four words buy *no reader or writer ever performs definite-assignment analysis*, and only route 3 spends them |
| **historian** (advisory) | approve, standing objection to route 3 | precedent | — | **Rust shipped route 3 and withdrew it in writing**: `mem::uninitialized`, deprecated 1.39.0, *"immediate undefined behavior … including integer types and arrays of integer types, and even if the result is unused"* |
| **completeness critic** | — | — | — | — |

## What every seat agrees on, and it is the strongest signal here

**Route 3 is dead four times over**, and each seat killed it differently:
Rust withdrew it after four years and replaced it with a type; Zig keeps it only
with a mandatory keyword *and* runtime poisoning, neither of which Heroes would
have; POSIX's commonest out-parameter idiom is value-result, and the
ffi-pragmatist measured `getsockname` and `getsockopt` **returning success while
writing nothing** and 48 raylib entry points dereferencing before writing; and
the repository already records the hazard at `selfhost/ir/inout.hero:92-105`,
where a deleted out-cell store printed the same four lines at `-O0` and under
both sanitisers because the indeterminate local happened to be zero.

## The disagreements, and what the critic settled

**1. The llm-ergonomist's objection to route 4 does not survive.** It rests on
*"the only producer of a `struct utsname` is `uname` itself"*, which four runs
refute. **Its reasons survive and its verdict on that route does not**, which is
the honest way to record a seat being right about the world and wrong about one
fact in it.

**2. The spec-warden's break-even arithmetic is overstated 12.7×** — it compared
route 1 against the full literal, a baseline the sitting rejects. Against the
route adopted the saving is 60 tokens, not 761, so break-even moves from one
program in thirteen to **one in one**. **Its conclusion survives, strengthened**:
route 4a's `.hero` is 98 tokens against route 1's 99, so all of route 1's saving
is a 61-token header written once per struct per platform rather than once per
program.

**3. `partial` costs something the sitting had not counted.** It makes `==`,
`hash` and map-key compile errors *for it and for any value holding it*, and it
does **not** cut the emitted C: the literal-plus-`partial` form still emits 256
`int8_t` temporaries and 1070 lines where the shim form emits 0 and 304. The
cost is in the ADVICE — recommending `partial` as the default shape for a header
record propagates that loss transitively — and the resolution says so rather
than recommending it blindly.

**4. The llm-ergonomist's F1 is falsified.** It reported that § 13's own
`record FileStat tag stat partial` example might be uncallable. The critic ran
it: `st_size` and `st_nlink` declared, `stat("/etc/hosts")` prints `544` and `1`,
and `ls -l` agrees. `missing_fields` ranges over the **declared** set.

**5. The historian's falsifier does not fire on this platform.** `sigset_t` on
Darwin is `__uint32_t`, four bytes — a scalar, not an array — so the one C API
where a zero could be wrong keeps its field mandatory. Linux is **unchecked**:
the critic had no Docker daemon and declined to assert glibc's layout from
memory.

## The finding about the brief, and it is this sitting's most useful output

**Six of the brief's premises were false, and the critic named the pattern.**
Three of them — *a call typed by context does not exist*, *`zero_of` already
provides a zero-default mechanism*, *`partial` does not help* — are all of the
form **"the compiler does not have X"**, and every one was written by a
coordinator who had a shell and did not run it. A fourth, *route 4 is a
refusal*, handed every seat a false framing of the option the sitting would
adopt.

The critic's sentence, adopted here as the rule:

> **A brief's negative sentences are run, or they go out as questions.**

CL-077 already binds every NUMBER in a brief to a command. This is the same rule
for the other half: CLAUDE.md § RUN IT says a negative claim rests on the
searcher's vocabulary rather than the world, and a brief is exactly where that
failure is invisible, because the seats check the world against the brief and
nothing checks the brief against the world.

## The resolution — `provisional — author ratification pending`

**Adopted: nothing is added to the language. The route is written down.**

1. **A header record is obtained by asking for one.** An `extern` group may
   declare a function returning it, and a Heroes function may return one too;
   the second spends no C at all and is the route the resolution names. `partial`
   lets a program declare only the fields it reads.

2. **`spec § 13` gains one clarifying clause, and it is a correction rather than
   an addition.** The llm-ergonomist could not answer its own task because **no
   sentence says a fixed-array literal's length must equal the field's** — the
   compiler enforces it (`fixed_array_length`, with both counts in the message)
   and the document is silent. That is CLAUDE.md § 12's shape with the rarer
   sign, the one defect 054 already wore: the compiler is right and the document
   has the gap.

3. **Routes 1, 2 and 3 are refused**, each with its measured ground, and the
   refusals are recorded here rather than in design.md Part 6 — because
   Principle 0 says a form that has not earned its way in **waits**, and a
   permanent row is the cheap move rather than the robust one (the spec-warden's
   own reading, adopted).

**What conservative would have been** (CL-040): route 2, the zero default, at
+32 real and 120-180 lines. It is conservative because it asks the author for
nothing at all. It was not taken because three seats measured that the capability
already exists, and buying a second spelling of something the language does is
what Principle 0 exists to refuse.

**What this resolution does NOT close, named with its trigger:**

- **`[0; 256]`, a repetition literal on the array, is unpriced.** The historian
  found Rust's precedent for it and the llm-ergonomist reached it independently;
  the critic measured that the compiler-engineer's veto does **not** transfer to
  it, because `selfhost/parse/type.hero:258` already reads an integer literal as
  a length and `walk.hero:616` already compares a count to `x.length`. **Nobody
  priced it.** It is the first thing to weigh if the advice above proves too
  awkward in practice.
- **Linux and Windows are unrun** for every variant here; every run in this
  sitting is `arm64-apple-darwin`, and Linux's `struct utsname` has six fields
  to Darwin's five.
- **`partial`'s transitive cost** — `==`, `hash` and map-key — is measured and
  not resolved.

## Predictions to score

| # | seat | prediction | checkable at |
|---|---|---|---|
| 1 | compiler-engineer | route 4 is 0 lines under `selfhost/` and `--refresh` moves at most +8 real from 8030 | M-readable-bytes close |
| 2 | spec-warden | with nothing landed, `--refresh` reads **8030**, digest `199b5f0a66d147d1` | M-readable-bytes close |
| 3 | spec-warden | at M-selfhost-fixpoint, no `examples/` program declares a fixed array longer than 8 | that milestone |
| 4 | historian | route 2 scoped to fixed arrays breaks no C API Heroes binds, falsifiable by one function reading a fixed-array field before writing it | whenever route 2 is weighed again |
| 5 | ffi-pragmatist | SQLite rung 3 and raylib rung 5 gain zero lines under this rule | M-core-packages |
| 6 | llm-ergonomist | under the current spec, first-try compile on this task is under 10%, and ≥70% of attempts invent a route rather than emit 1280 zeros | M-thesis-harness |

## Author's verdict

**Ratified 2026-09-18 BY DELEGATION AND NOT BY READING.** What it rests on is
named exactly rather than stretched: the author's instruction of this date, after
being told the second wall needed a sitting, was **continue in the most robust
way**. They did not pronounce on the route, because the sitting had not sat.

**What the yes settles.** Nothing is added to the language. `spec § 13` gains one
clause — *as many elements as the type says* — which is a correction of a silence
and not an addition, measured at **+10 real**. A header record a C function fills
is obtained from a function that returns it, and `partial` names only the fields
read.

**What it does NOT settle, each with its trigger:**

- **`[0; 256]` is unpriced** and is the first thing to weigh if the advice proves
  awkward. The critic measured that the compiler-engineer's veto does not reach
  it: `selfhost/parse/type.hero:258` already reads an integer literal as a
  length, and `walk.hero:616` already compares a count to `x.length`.
- **Every run in this sitting is `arm64-apple-darwin`.** Linux's `struct utsname`
  has six fields to Darwin's five, and no variant here was run on either Linux
  leg or on Windows.
- **`partial` carries a transitive cost** — `==`, `hash` and map-key become
  compile errors for it and for anything holding it — measured and not resolved.
- **The refusals live here and not in design.md Part 6**, because Principle 0
  says a form that has not earned its way in *waits*: a permanent row is the
  cheap move rather than the robust one.
- **The rule this sitting adopted about its own procedure** — *a brief's negative
  sentences are run, or they go out as questions* — is written into
  `.claude/skills/panel/SKILL.md` beside CL-077, which binds a brief's numbers to
  a command and said nothing about its negations.
