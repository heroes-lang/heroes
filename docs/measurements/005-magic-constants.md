# 005 — the copied constant, measured: five sites, five survivors

Date: 2026-08-12 · after M-ffi-ladder · taken **for panel 038's sitting**, so the
panel judges a number rather than a worry.

## Provenance

| what | value |
|---|---|
| compiler | `3496140` plus this commit (the operator is new here) |
| spec | sha256 `50e07aff010ef033c8fdd926fe44a188…` — measured max **2560**, unchanged by this run |
| corpus | `examples/` — **19** programs (the gallery, `calculator.hero` and its four modules, `sqlite/`, `curl/`) |
| operators | `harness/mutations/operators.md`, now **twelve** — `typo-digit` added |
| arms | `check` and `check --permissive` — unchanged |
| command | `heroes mutate` |

## The question

§4.19 says clang verifies an `extern`'s signature against the real header, and
panel 036 made that sentence true with a `_Static_assert` per declaration. The
*values* a C library is driven by are not signatures, and there is no form to
declare one, so a program copies the number: `examples/curl/main.hero` writes
`10002` for `CURLOPT_URL`, `examples/sqlite/main.hero` writes `100` for
`SQLITE_ROW`.

The instrument had never asked about this. `heroes mutate` had eleven operators
and none touched the digits of a number, so a wrong transcription was outside
what metric 3 could see. `typo-digit` asks: move the last digit of a `constant`'s
value by one, and see what the compiler says.

The site is deliberately narrow, and the narrowing is a fact about the value
rather than a premise about the program (CLAUDE.md §11). A number *inside an
expression* is a choice the program is making, and nothing outside the file can
contradict it — `MAX_DEPTH: int / 64` is right because the author says so. A
`constant` exists to **name** a number, and where that number was copied from
somewhere else, the somewhere else is an authority the compiler could have
consulted.

## Result

```
| operator   | mutants | excluded | killed (check) | killed (--permissive) |
| typo-digit |       5 |        0 |        0 (0%)  |               0 (0%)  |
```

**Five sites in the corpus, and all five are C constants:**

| site | value | authority | caught by |
|---|---|---|---|
| `examples/curl/main.hero:37` `CURLOPT_URL` | `10002` | `curl/curl.h` — `CURLOPTTYPE_STRINGPOINT + 2` | nothing |
| `examples/curl/main.hero:40` `CURLE_OK` | `0` | `curl/curl.h` | nothing |
| `examples/curl/main.hero:43` `CURLE_UNSUPPORTED_PROTOCOL` | `1` | `curl/curl.h` | nothing |
| `examples/sqlite/main.hero:38` `SQLITE_OK` | `0` | `sqlite3.h` | nothing |
| `examples/sqlite/main.hero:41` `SQLITE_ROW` | `100` | `sqlite3.h` | nothing |

There is no sixth site. Every `constant` in `examples/` whose body is a single
integer is a value some C header owns — which is not a coincidence the instrument
arranged, but what the corpus turned out to be once it started calling C.

The mutants are legal programs. `CURLOPT_URL` at `10003` type-checks, compiles,
links, runs, and asks libcurl for an option nobody meant; `SQLITE_ROW` at `101`
makes a loop that never sees a row. Every other stage agrees: `heroes check`,
clang with `-Wall`, the linker, the leak gate, ASan. Metric 3's own definition
of a silent error, five times over, in the only two programs in the corpus that
talk to a real library.

## What it does not measure, said plainly

The rate cannot improve. There is no rule that could look at `10002` and know it
is wrong, because a number in a file has no authority behind it — which is why
this row's "thesis mechanism" column says **nothing**, like `typo-code`'s.

The honest outcome of §4.19 gaining a header-valued `constant` is therefore not a
better rate: it is that **the site disappears**, because the digit is no longer in
the file to be moved. A harness reporting per-operator rates cannot tell a defence
from an empty denominator (`mutate/mod.rs` rule 3), and `docs/debrief/QUEUE.md`
already carries that trap for `typo-code`, so the number to report after the fix
is the **site count**, not the kill rate:

> `typo-digit` on `examples/` — sites **5 → 0**, with the two programs still
> naming the same five constants.

A residual site in a `.hero` file that names a C constant afterwards is a defect
in the conversion, not in the instrument. A site that *appears* later, on a
constant the program itself owns, is correct and must survive: nothing outside the
program knows what `MAX_DEPTH` should be.

## Fixed by

Pending: panel 038 and `M-header-constants`. This file is the before.
