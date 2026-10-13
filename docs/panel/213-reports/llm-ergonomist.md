# Panel 213, the llm-ergonomist's report

Inputs read, by absolute path and nothing else of the project: the spec copy, `symptom.txt`,
`frame-1.txt`, `frame-2.txt`, `frame-3.txt` under
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-panel-213/llm-ergonomist/`.
Read in the order 1, 2, 3, so the readings of frames 2 and 3 are primed by frame 1's; the
line counts for 2 and 3 below are what a fresh reader would need, estimated, not what I lived.

## verdict

**approve** frame 3's rendering (source names only, values written in the spec's own syntax)
as what a stop shows a reader; **object** to frames 1 and 2 as reader-facing output. No veto:
a rendering of a stop is not a language construct, and none of the three makes the meaning of
a source line depend on anything beyond the line and its signature.

## experiment

The program each frame let me reconstruct (line 12 is `return winner`, which fits a
three-line `record Player`, a blank line, then `best`):

```
record Player
    name: str
    score: i64

function best(players: [Player]) -> str
    top @= 0
    winner @= ""
    for p in players
        if p.score > top
            top @ p.score
        winner @ p.name
    return winner
```

The fix I would write first try, under every frame:

```
        if p.score > top
            top @ p.score
            winner @ p.name
```

**Frame 1** (C types, source names demangled, compiler names and temporaries kept, 53 lines).
Finding: `top = 9` (line 4) beside `winner = "alan"` (line 5) and the loop's last `p`,
`alan`, `score = 4` (lines 8 to 11): the maximum was tracked and the name was not tracked with
it; `winner` is written on every iteration, so it holds the LAST player. Lines read to the
finding: **11**. Lines read to learn who holds the 9: **35** (`t16`, `grace`, line 34). The
whole 53 to be sure nothing contradicted it. `ada` appears nowhere in the frame: one of the
three players is invisible, because `players` (line 3) is an address and not a value.

**Frame 2** (raw C: `h0_`, `h1_`, `f_`, `h_program_` prefixes everywhere, 53 lines). Same
finding, same values, same line numbers: **11** to the finding, **35** to `grace`, 53 to be
sure, plus the demangling on every line. Line 1 names `h_program_best` where the symptom says
`best`: a moment of *am I in the right function?*

**Frame 3** (spec syntax, source names in scope, 5 lines). `players = [Player(name: "ada",
score: 7), Player(name: "grace", score: 9), Player(name: "alan", score: 4)]`, `top = 9`,
`winner = "alan"`. Finding: `top` is grace's 9, `winner` is the array's last element. Lines
read to the finding: **3** (lines 3 to 5); the whole frame is **5**. Nothing to guess about
the values; the one remaining question (is `winner` written unconditionally or in an `else`?)
is the source's to answer, and both readings produce the same fix.

## hesitation_points

Frame 1 and frame 2:

- **`players = 0x00000001005c9860`** (line 3). The spec says every value is an independent
  copy and *no aliasing exists*; the frame shows five names (`players`, `h3_xs0`, `t3`, `t6`,
  `t9`) holding one address. A spec reader either distrusts the spec or the frame. Wrong
  guess: *the loop iterates the caller's array by reference*, a silently wrong mental model
  that leads nowhere. The input is not readable at all, so `ada` is lost.
- **`h3_xs0`, `h4_i0`** (lines 6 and 7). Guessed to be the `for`'s copy of the array and its
  index. Nothing in the spec names them; in frame 2 they wear the same `h<N>_` prefix as the
  program's own names, so the frame does not say which names are mine.
- **`p`** (line 8). Out of scope at `return winner` in the source (the `for` binds it), but
  shown. Useful here as *the last element visited*, and also a claim about scope the spec
  contradicts.
- **`t1` to `t27`** (lines 12 to 53, 42 of 53 lines). I had to assign each a role to use any
  of them (`t1 = 0` and `t2 = ""` as the cells' initial values, `t15 = false` as `4 > 9`,
  `t19 = "alan"` as the value written to `winner`). Every role is a guess.
- **`t23 = (ptr = "", len = 1)`, `t24 = (ptr = 0x0, len = 0)`, `t25 = NULL`** (lines 46 to
  48). An empty string of length one and two null values, in a language whose spec says *all
  bindings are initialised*. Read as evidence they point at memory corruption: a false lead,
  and an expensive one for a model, which will chase it.
- **`t26`, `t27` = `grace`** (lines 49 to 53), AFTER `t19 = "alan"`. Read in order, this
  says the last thing computed was grace's name: the opposite of the truth. A model that
  reads temporaries as a trace can conclude *winner was grace and something overwrote it
  afterwards*, a silently wrong diagnosis.
- Frame 2 only: `h_program_best` against the symptom's `best`, and `f_name` / `f_score`
  against the spec's `name` / `score`. Strippable, but every one is a mapping the reader
  performs and can get wrong when two names mangle close together.

Frame 3:

- **`top = 9`, `winner = "alan"` written with `=`** though both are `@=` cells in the source.
  A spec reader learns `=` as *binds once, forever*; harmless for this bug, a small
  misstatement of what could change.
- **No types.** `9` is the default `i64` here, but a `u8` cell holding `9` would render the
  same, and for an overflow or width bug the type is the evidence.
- **One 110-character line for three elements.** Unbounded for a large array; no elision
  policy is visible.

None of the frame-3 points produced a guess about THIS bug; every frame-1 and frame-2 point
did, and the two marked *silently wrong* would compile into a wrong fix (for instance, guarding
against a later overwrite of `winner` that never happens).

## argument

Frame 3 is the only one written in the language the reader was taught: a spec reader parses
`Player(name: "grace", score: 9)` and `"alan"` with no mapping, sees the whole input, and finds
the bug in three lines. Frames 1 and 2 show the C the compiler emitted: addresses where the
spec promises copies, a loop variable out of scope, compiler names indistinguishable from the
program's, and 42 lines of temporaries, three of them uninitialised-looking and two holding
`grace` after `alan`, which reads as a trace pointing the wrong way. They reach the right
answer here only because the bug is shallow; their extra lines carry no fact the reader needs
and two that mislead. Frame 2 is frame 1 with demangling left to the reader.

## prediction

On a stop-diagnosis task set (symptom plus one frame; name the wrong value and write the fix),
with programs of at least five locals or an array whose contents decide the bug:

- first-try correct diagnosis with frame-3 rendering at least **15 points** above frame 1 and
  at least **20 points** above frame 2;
- answers citing a temporary (`t<N>`), a compiler name (`h<N>_`) or an address as evidence:
  **0** under frame 3, at least **5%** under frames 1 and 2, half of those wrong;
- on THIS program alone, all three at or near 100%, the difference showing as lines and tokens
  read (5 lines against 53; the token counts are unrun, I had no instrument).

## condition

I would move to **object** on frame 3 if the harness shows a class of stop where the decisive
fact lives below the source: a `ptr`, `cstr` or handle whose address matters (the spec itself
says those compare as addresses, so frame 3 must render them as addresses), a group record's
union or `partial` bytes, a lease, or C's writes past a lent array. A rendering that drops the
address of a `ptr` would be wrong. I would reconsider frames 1 and 2 only if a measured
task set shows models diagnosing better WITH temporaries, which I would not expect.

What each rendering would need:

- frames 1 and 2 to drop: every `t<N>`, the compiler's own names (`h3_xs0`, `h4_i0`), names
  out of scope at the line (`p`), the C types, the mangling prefixes; and to ADD: the contents
  of every array instead of its address;
- frame 3 to add: the type where the literal's default (`i64`, `f64`) would not say it, in
  the spec's own form (`b: u8 = 255`); a mark for a cell, so `top @= 9` against `x = 9`, if
  the panel accepts a display that looks like a declaration; a long container broken one
  element per line as the spec's container literal allows, and an elision written as a `#`
  comment (`# 9,997 more`), so the rendering stays valid Heroes; `T?` as `ok(...)` and
  `fail(code: ..., msg: ...)`, a variant as `.case(field: ...)`, a `ptr` and a handle as an
  address and `nullptr`.

## context

**Yes.** Running as a subagent of the coordinator's session, as the brief says. Reached my
context: the project's `CLAUDE.md` (the operating contract: hard stops, the run-it rule,
precedence with robustness first, the panel process, conventions); the auto-memory index
`MEMORY.md` (local ledgers, among them *M-typed-inspection ledger* and a panel 209 sitting on
`@=`, which told me the milestone's name and nothing of its content); and, loaded when I read
the brief inside the tree, `.claude/rules/verification.md` and `.claude/rules/records.md` in
full. One sentence of `verification.md` bears on this reading: since panel 182 *the prologue
gives a temporary no initialiser*, so a temporary can hold stack bytes. It corroborates my
reading of `t23` to `t27` as uninitialised; I reached that reading from the frame itself (a
null pointer, a length-one empty string, a temporary in a language that says every binding is
initialised), but I cannot unknow the sentence. By my definition this voids the reading until
it is re-run from a copy; the brief records that the author chose the non-blind run.
