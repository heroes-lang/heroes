# Panel 143 — two warts: one erases the evidence of itself, and the other is not the inconvenience it was filed as

**Sat** 2026-09-13 · **milestone** M-deferral-ledger, step 8 · **status**
`provisional — author ratification pending`

**Lane: five seats plus a completeness critic — and four of the five wrote
files.** The historian's brief gives it no file write, so its report reaches the
coordinator through the run's own journal and never the sitting's directory. **The
critic caught that and it is a real gap**: a seat whose report never touches disk
cannot be audited by the seat whose job is auditing, and the first draft of this
synthesis carried the historian's findings with nothing behind them. It is the same
shape panel 140 found about briefs, one door further along. These are the **two
watch-list entries with no home at all** until this milestone gave them one on
2026-09-03, and they are the last two items of its original list. The gate
CLAUDE.md § 4 asks once per milestone was given at step 1.

## The two warts, and the three verdicts available

**Wart 15**: *"`"C:\temp"` is silently a tab (panel 008). Reserving the backslash
makes `"\d+"` a loud error, but it cannot catch a path whose next letter happens
to name a legal escape … the remedy is raw string literals, which v1 does not
have."*

**Wart 16**: *"`print` took only half of `WriteLn` … there is still no way to print
without a trailing newline."*

Each gets ENTERS, REFUSED, or **STAYS A WART with a dated verdict and a return
condition** — that third shape having its precedent in panel 034, which ruled
wart 5 to stay one.

## What was measured

**Wart 15 is real, and the reader wrote the bug unprompted.** Asked to store a
Windows path, the seat that sees only the specification typed
`"C:\temp\notes.txt"` **before reading any proposal**. It compiles and prints
`C:` TAB `emp` NEWLINE `otes.txt`: the path breaks across two lines, exit 0, zero
diagnostics.

**The silent set is exactly five letters.** Of twelve realistic first path
components, **seven pass in silence** — `temp notes readme new run tools rc` —
and **five are loud** — `Windows Users data Program logs`. The silent ones begin
`n`, `t`, `r`, `\` or `"`.

**Two escapes are eaten, not one, and the failure erases its own evidence**
(FFI seat, reproduced by the coordinator byte for byte). `"C:\temp\report.txt"` is
**16 bytes**: `C : \t e m p \r e p o r t . t x t`. Then:

- **The emitted C hides half of it.** `HERO_STR_STATIC(hero_str_…, "C:\temp\015eport.txt")` — the `\t` half is **textually identical to the Heroes source**, so `--emit-c` is not a witness for it; only the carriage return shows, as `\015`.
- **And C's own diagnosis is destroyed on the terminal.** `perror` writes the bytes `C:` TAB `emp` **CR** `eport.txt: No such file or directory`. The carriage return is a control character **the language injected**, so the terminal rewinds and the reader sees `eport.txt: No such file or directory` — **the path erased from the screen**. The one line that would name the bug deletes the evidence, and that is not in the wart's text.

**But the boundary is not bitten today, counted**: **24** `.hero` files carry an
`extern` group across `examples/` and `selfhost/`, and **exactly one** string
literal among them contains a literal backslash — the printable-ASCII table in
`examples/tally/main.hero`, correctly written `\\`.

**The correct spelling costs two characters and does not compound.**
`"C:\\temp\\report.txt"` is 18 bytes, and the emitter re-escapes rather than
passing through, so the emitted C is byte-identical to the Heroes source:
**Heroes' escaping level and C's are the same level**. `"C:\\\\temp"` gives **two**
backslashes — it is the wrong spelling, not the cautious one.

**Wart 16 is real and absolute** — the specification says `print` writes *"exactly
one trailing newline"* — and it was filed as a missing convenience. **It is not
one**, as the critic measured below; but the first thing the seats established is
that the gap is reachable. The FFI seat compiled the C route in **nine lines**: one `extern`, one signature, a
call-scoped `.cstr()` lend, no `lease`. The emitted C passes `const char *`
straight through with no box and no copy, and the `importc` probe type-checks it
against the real `<stdio.h>`.

**And the caveat that seat contributes is the sharper half of wart 16.** Because
`stdout` is **not nameable** — `constant stdout: ptr` is `error[ffi_not_constant]`
— the textbook route pushes an author to `fdopen(1, "w")`, which compiles and
**silently reorders the output**: source order `fputs`, `print`, `fputs`, `print`
produces `value\nagain\nlabel: second: ` at exit 0 with zero diagnostics, because
`print` and the C stream hold separate buffers.

## Verdicts

| seat | wart 15 | wart 16 |
|---|---|---|
| `llm-ergonomist` | **adopt-with-condition** — the only seat to go further than STAYS, and the one that wrote the bug first try | **stays a wart** |
| `compiler-engineer` | **stays a wart**, remedy named and the spelling **reserved**; sugar, not core — no new expression kind, no IR instruction, no checker rule | **stays a wart**, with `write` named and priced: *"the cheapest thing this panel has seen"*, **one line** in the built-ins table, the positional assert surviving |
| `ffi-pragmatist` | **object to the wart's TEXT, not to STAYS** — *"the sentence recording it under-states the cost by the one measurement my seat exists to make, and names the expensive remedy as if it were the only one"* | **approve, stays a wart**: a convenience, and *"only one of the three C routes is safe"* |
| `spec-warden` | measured: MIN **+22**, closed **+77** | name only **+5**, name and prose **+17**, `print` gaining `end: ""` **+8** |
| `historian` | **approve, stays a wart** | **object**: *"stays a wart is admissible only with a pre-committed, parameter-shaped return route"* |

## What the historian found, and it reprices the remedy rather than the wart

**Python spent a decade on the strict route and it does not help.** Unrecognised
escapes went to `DeprecationWarning` in 3.6, `SyntaxWarning` in 3.12 and a future
`SyntaxError` — and the example on python-dev is `'..\training\new_memo.doc'`,
*"which would produce a corrupted file name, but no warning"*, because `\t` and
`\n` are valid. **Refusing unknown escapes is the majority design and none of its
adopters catches this**, C# included.

**The remedy has a permanent hole, exactly on this path.** In Python *"even a raw
string cannot end in an odd number of backslashes"*, so `r"C:\temp\"` is a syntax
error — **the cure fails on the path that ends with a separator**, which is the
Windows path. C++11, Rust and Swift all needed a **customisable** delimiter for
that class, and Swift's SE-0200 was revised *away* from a simple `r` prefix
toward Rust-style pounds; Go took the fixed delimiter and paid with a silent
transformation of its own, discarding carriage returns from raw strings.

**And there is a free workaround nobody in the sitting had named**: Windows
normalises forward slashes to backslashes, documented by Microsoft, so a Heroes
program writes `"C:/temp"` today **at zero language cost**.

## What the critic found, and one of it changes a verdict

**Wart 16 is not purely an inconvenience, and the proof uses no FFI at all.** The
language already ships a third route — `write_file(path: "/dev/stdout", …)` —
which writes without a trailing newline at exit 0, zero built-ins, zero spec
tokens. **And interleaved with `print` it silently reorders.** Reproduced by the
coordinator, source order `label: ` / `value` / `second: ` / `again`:

```
label: second: value
again
```

Both writes emerge before both prints, exit 0, zero diagnostics, **using only
what the language ships**. So the cause is not §4.19's refusal of `stdout`, as the
FFI seat argued from its own `fdopen` case — it is that `print` and `write_file`
reach the same descriptor through different buffers, and a reader who finds the
route finds the hazard with it.

**Wart 15 has a sibling rule already in the compiler, and no seat cited it.**
`selfhost/literals.hero:82-89` refuses a **raw** carriage return inside a literal,
with a `certain` fix, on the stated ground that an invisible character survives
`heroes fmt` and is deleted by the next reader who retypes the line — *"word for
word the argument for refusing a decoded control byte the author transcribed"*.
What separates an intended `\t` from a transcribed one is heuristic and owes its
own sitting.

**And the "warn" both the reader and the warden reached for does not exist**: a
grep for a severity finds only the parser for **clang's** severities, and
`selfhost/diag.hero` has no severity field. **A Heroes diagnostic is exit 1 or
nothing.**

**Two corrections that bite.** The `write` name is not free: `function print(…)`
in a user file gives `error[builtin_name_taken]`, so a `write` built-in would
break `examples/markdown/main.hero` in the same commit — the engineer's *"cheapest
thing this panel has seen"* is one line **plus renaming a shipped example**. And
the warden's three partial-line stdout sites are **newline de-duplicators** that
strip a newline and call `print`, which re-adds it: the true non-FFI partial-line
count is **zero**, not three.

**The coordinator's three counts all reproduce and none states its unit.**
`print(` is **646 lines** or **676 occurrences**; backslash literals are **138
literals** or **221 backslashes**; `.join(` is 132 either way. Two seats quoted
the two different numbers for one world, which is CL-017's unit rule.

## The resolution adopted, provisionally

**Both warts STAY, with dated verdicts, return conditions, and their text
amended — because in both cases the entry is wrong about something.**

1. **Wart 15 stays, and its text gains the three findings it lacks**: two escapes
   are eaten and not one; the emitted C is not a witness, since the `\t` half is
   textually identical to the source; and **the injected carriage return erases
   C's own diagnosis from the terminal**, which is the cost the entry never
   names. **Its remedy sentence is also corrected**: *"the remedy is raw string
   literals"* is an unenumerated option set (CL-057) — Python's own raw literal
   fails on the trailing-separator path, three languages needed customisable
   delimiters, forward slashes work today at zero cost, and `hero_cstr_nonnull`
   is already emitted at every lend, so a literal lent as `cstr` carrying a C0
   control character is checkable **at one existing site with no new syntax**.
   That route is named and **not adopted**: nobody built it.
2. **Wart 15's return condition is a boundary condition**, the FFI seat's: *a
   program in `examples/` or on the §1.0 closure list whose `cstr` lend carries a
   path or a pattern wrong at exit 0 — the count is **zero** today, one literal
   in twenty-four extern-carrying files and that one correct — **or** the Windows
   leg landing a real path-taking binding, which turns one-in-twenty-four into
   the common case.*
3. **Wart 16 stays, and its text is corrected on what it IS.** It was filed as a
   missing convenience; the sitting measured that the workaround a reader finds
   **silently reorders output**, at exit 0, with zero diagnostics, **using only
   built-ins** — `write_file(path: "/dev/stdout", …)` interleaved with `print`
   puts both writes before both prints. The FFI route has the same hazard through
   `fdopen(1, "w")`, so the cause is not §4.19's refusal of `stdout` but two
   buffers on one descriptor. **A wart whose workaround fails silently is not an
   inconvenience**, and the entry must say so.
4. **Wart 16's remedy is named and priced rather than left open**: a second
   Tier-1 built-in `write`, **+5** spec tokens for the name and **+17** with
   prose, or `print` gaining `end: ""` at **+8**. The engineer priced the
   compiler side at one line in the built-ins table, and **the critic corrected
   it**: `builtin_name_taken` makes the name reserved everywhere, so it is one
   line **plus renaming a shipped example** in the same commit. Its return
   condition is the historian's demand — *a pre-committed, parameter-shaped
   route* — sharpened by the buffering finding: **the wart returns when a corpus
   program interleaves unterminated output with `print` and the reordering is
   what stops it**, which is now a measured shape rather than a hypothesis.

**What conservative would have been**: leave both entries as written and record
only the dates. The sitting amends them because three seats independently found
the entries wrong about where the cost lands, and a wart is a **stated** cost —
an entry that understates it is not doing the one job Part 8 has.

**What this sitting does not decide**: whether raw string literals ever enter —
the return condition decides that; whether the `cstr`-lend check is the right
answer, which nobody built; and whether a diagnostic could separate an intended
`\t` from a transcribed one, which is heuristic and owes its own sitting. **One
thing it records rather than decides**: the language has no warning level at all —
a diagnostic is exit 1 or nothing — so every "just warn about it" reached for in
this sitting was reaching for something that does not exist.

## Author's verdict

Pending — queued in `docs/work/DECIDE.md` as `panel 143`.

## Predictions to score

| prediction | instrument | scored at |
|---|---|---|
| FFI: no binding on §4.19's ladder and no `extern` group in this tree is bitten by wart 15; one literal in 24 files, correct | `grep`, `heroes build` | whichever milestone lands a path-taking binding |
| engineer: `write` lands in one line of the built-ins table with the positional assert intact | `git diff --stat`, the `spec` suite | whichever milestone adds it, if any |
| warden: a closed raw-literal clause measures ≥ +77 vendored | `heroes measure` | any sitting that drafts one |
| the return conditions' own clock: a corpus program whose `cstr` lend is wrong at exit 0, or a Windows path-taking binding | a sitting that proposes one | every later sitting of this ledger |

## What the seats could not source or could not run

The coordinator reproduced that `sqlite3_open` **and** `sqlite3_exec` both return
`SQLITE_OK` at exit 0 on a path the author never wrote, and **could not reproduce**
the FFI seat's further claim that a file with that name appears in the tree: it
looked and found none, `heroes run` building and running in a temporary directory.
The confirmed half is narrower and still decisive — the program believes it
succeeded. The warden's drafts carry no `real` row and its deltas are vendored
floors rather than converted numbers, which it says. The historian's PowerShell
rationale is secondary-sourced with no designer statement found. And one seat
overwrote another's scratch file before noticing and said so, which is why its
work sits in a subdirectory of its own.
