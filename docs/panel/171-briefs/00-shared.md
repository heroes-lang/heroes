# Panel 171 — shared brief: the word for a parameter that does not keep, under a default that assumes it does

**Full panel**, five seats and a completeness critic. It adds a contextual word
to `spec § 13`'s grammar, a sentence to its prose, and a refusal to the checker.

**The DIRECTION is the author's and is not on the ballot.** On 2026-09-20 the
author ruled, after making one condition and having it measured: *the default
flips.* A C pointer parameter is assumed to **keep** what it is handed unless
its declaration says otherwise, so **a lend may reach only a parameter so
marked**. `docs/records/log/2026-09-20-1930-heroes-must-be-robust-and-the-default-flips.md`
carries the ruling in the author's words. **This sitting judges the word, the
exact rule, and the landing. It does not re-argue the default**: a seat that
does is recording a real objection for the record, which is welcome, and not
casting a vote on something already decided.

**Every number below was produced by a command run on 2026-09-20 while this
brief was written, or is cited to the measurement file that holds it.** Re-run
anything you rest a verdict on.

## Why the default is safe to flip, in one measurement

`docs/measurements/038-…`: twenty million calls into a C function reading a
56-byte string, sequentially, machine still. **Lend 40 ns a call, copy 73.5 ns,
so the copy costs 33.5 ns a call in the worst case** — where the author marks
nothing — and **zero where the mark is written**, because the emitted C is
today's `hero_str_cstr`, a field read.

## What the flip closes, and what it does not

**Closes defects 066 and 068**, panel 170's completeness critic measured: both
reproducers lend into an unmarked parameter, and under the flip that lend is
refused. **Does not close 070**: the give-away hands a **lease**, and panel 170's
compiler-engineer vetoed any single word carrying both facts — *retention must
ADMIT a lease; give-away must REFUSE one.* 070 stays open and is not this
sitting's.

## The blast radius, counted

| tree | `.cstr()` sites | `.ptr()` sites | extern functions with a `cstr` or `ptr` parameter |
|---|---|---|---|
| `examples/` | 10 | 0 | 11 |
| `tests/golden/` | 53 | 33 | 90 |
| `selfhost/` | **12 real call sites** (55 raw hits, the rest message text and fixtures) | 0 | **9** |

The `selfhost/` row is the one nobody had counted and it decides the landing.
The compiler's own bindings — `hero_fs_exists`, `hero_fs_mkdir_all`,
`hero_dir_scan`, `getenv`, `atof` and four more in `selfhost/cli/process.hero:42-53`
and `selfhost/emit/literal.hero:42` — lend a `cstr` within the call, every one.
**Under the flip each needs the mark. So the seed compiler must PARSE the mark
before `selfhost/` can write it, or the fixpoint breaks.** The landing is two
steps: the word first, with today's rule still in force; the seed regenerated;
then the rule flipped and the nine marked in the same commit.

## What already binds the word — panel 170's own resolution

- **Its own grammar slot beside `counted_by`, never inside the
  `consumes|acquires|borrows` alternation.** `selfhost/check/marks.hero:64-84`
  sweeps that alternation and `refuse_unread` fires unless a handle is behind
  the type; `cstr` and `ptr` reach no handle. A word landed there is refused on
  exactly the types this is for. Measured at panel 170: `atoi(s: cstr consumes)`
  is `error[unread_mark]`, exit 1. The own slot cost **+3 real**.
- **Negative polarity.** Every enforced-at-the-caller mark in the historian's
  survey says *does not keep*: Clang's `noescape`, C#'s `scoped`, Hylo's `let`,
  Swift's `@escaping`'s absence. The positive polarity — *keeps* — ships only in
  static analysers, nowhere as a compiler error.
- **It names no ending call.** The ffi seat's veto stands: `keeps end_fn` was
  right for no more than two of three retention modes on one declaration.

## The grammar today, and the spec's room

    CParam = [ "@" ] ident ":" Type [ "counted_by" ident ] [ "owned" ident ]
             [ "consumes" | "acquires" ident | "borrows" ] .

`./heroes measure spec/heroes-spec.md`: **8201 real**, 6159 vendored, ceiling
10240, `DELTA_GATE` 50 in vendored tokens. `.env` is present, so `--refresh`
prices a draft: apply to the real path, measure, revert, **in a copy**.

## The questions

1. **The word.** It says *this parameter does not keep what it is handed* and it
   is read on `cstr` and `ptr`. Candidates the coordinator will not rank, so the
   ergonomist can test them blind: something in Heroes' plain-English register.
   `borrows` is already taken — on a result it means *the call hands back one it
   keeps*, the opposite — and the sitting should say whether a second meaning on
   a parameter is tolerable or a trap.
2. **The exact rule.** A lend (`s.cstr()`, `f.ptr()`) at an **unmarked** `cstr`
   or `ptr` parameter is refused at `check`. What does the unmarked parameter
   still admit — a lease, `nullptr`, a `ptr` from C? Does the rule reach an `@`
   out-parameter, whose pointer is to the program's own cell? Does it reach a
   handle parameter, which has its own marks? **Say what is refused, what is
   admitted, and at which stage.**
3. **The diagnostic.** A new class. What it says, what its `Fix` is, and whether
   the fix is `certain`: adding the mark to the declaration changes a claim about
   C and cannot be certain; passing a lease instead changes the program's
   ownership and may be.
4. **The landing.** Two steps, as above. Say what each commit holds and which
   suites judge it. **A change to what the checker refuses is judged by every
   golden tree.**
5. **What it costs in the corpus.** 12 + 10 + 86 lend sites, 110 extern
   parameters. Which are refused on day one, and how many declarations gain the
   word.

## Working rules

- **Build in a copy that is YOURS**: `cp -r` the tree to a directory under your
  own scratchpad **named for your seat**, `rm -rf target build` in it, and check
  `git log -1` in the copy reads `b6e26fcc` or later. Panel 170 lost a
  measurement to two seats sharing one scratchpad and one rebuilding the other's
  compiler underneath it.
- A compiler in about four seconds:
  `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`. **Never rebuild
  from `selfhost/`.**
- Never `archive/bootstrap-rs/`.
- Report to `docs/panel/171-reports/<your seat>.md`.
- A negative sentence is run, or it goes out as a question naming what you
  searched.
