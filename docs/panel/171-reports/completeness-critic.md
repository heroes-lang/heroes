# Panel 171: completeness critic

**Not a sixth judge, no verdict.** This seat names what is missing, before the
synthesis. Every number below was produced by a command run on 2026-09-20,
Darwin arm64, in a copy of the tree at `b6e26fcc` under
`<scratchpad>/completeness-critic-171/tree` (`target` and `build` removed, seed
compiler built in `real 3.29`), with probe programs under
`<scratchpad>/completeness-critic-171/probes/`. **The built rule was RUN, not
read**: the compiler is the engineer's `<scratchpad>/ce-copy/heroes-B2`, the
fixpoint binary its report names, with `HEROES_RUNTIME` pointed at my copy's
`runtime/`. Nothing in `ce-copy` was modified. The main tree was touched only
by this file.

**One correction to the brief before anything else.** The brief says a previous
launch of this seat *died before reading a single report; nothing of it exists
on disk*. False as run: `probes/` already held 45 `.hero` programs, 44 of them with
`.A2.out`/`.B2.out` outputs timestamped 22:16:42, three minutes before my copy.
I read them as data, wrote 31 of my own beside them (my `c19_lent_twice`
overwrote theirs of the same name), and **re-ran all 75 under `heroes-B2`
myself** (`ls c*.hero | wc -l` reads 79 after the four alias probes of § 1
were added); no number here is taken from those outputs. One of the earlier
programs (`c38`) does not parse and tested nothing.

---

## 1. The next b10, found and run: the rule sees the lend EXPRESSION, and the hazard is the POINTER

The engineer's b10 launders a lend through a Heroes function **on the way in**,
and the built rule refuses it (`lend_needs_a_header`, run: exit 1). The shape
beside it is a lend laundered through C **on the way back**: C hands the lent
pointer back to the program, the program now holds a *C-returned* `cstr` or
`ptr`, and the rule admits that value at any unmarked parameter, because it
takes a value C answered for a value C owns. **C can answer the program's own
bytes**, and libc does so constantly. Five programs, three doors, two types,
each `check` **exit 0** under the rule and each a use-after-free under
`--sanitize`:

| probe | door | what C hands back | `check` | `run` ×3 | `--sanitize` |
|---|---|---|---|---|---|
| `c50_strchr_alias` | **result** | `strchr(s: cstr lent, c: i32) -> cstr`, real libc, the answer points into `s`; passed straight to `keep(s: cstr)` unmarked | **0** | 0 0 0, prints `0` | **134**, `heap-use-after-free` |
| `c50b_strchr_bound` | result, bound first | same, `p = strchr(...)` then `keep(s: p)` | **0** | 0 0 0 | 134, `heap-use-after-free` |
| `c51_ptr_alias` | result, the `ptr` twin | `memchr(p: ptr counted_by n lent, ...) -> ptr` into a field; `keep_p(p: ptr)` unmarked | **0** | 0 0 0, prints `7` | 134, `stack-use-after-scope` |
| `c07_out_alias` | **`@` out-cell** | `split(s: cstr lent, @rest: cstr)` writes `s + 1` into `rest`; `keep2(s: rest)` unmarked | **0** | 0 0 0 | 134, `heap-use-after-free` |
| `c08_callback` | **callback argument** | `each(s: cstr lent, cb: (function(cstr) -> ()))` calls back with `s`; the Heroes callback forwards its parameter to `keep3(s: cstr)` unmarked | **0** | 0 0 0 | 134, `heap-use-after-free` |

In every row the lend itself lands on a parameter truthfully marked `lent`:
`strchr`, `memchr`, `split`, `each` keep nothing. The keeper is **unmarked**,
which under the flip means *assumed to keep*, and it is handed a pointer into
bytes the program freed when `launder()` returned. **The flip's own sentence
promises the reader that this is exactly what is now refused**, and it is not.

**The flip did not open this door; it left it.** The same program without the
word, under today's compiler: `check` 0, `run` 0, `--sanitize` 134
`heap-use-after-free` (`c50_today`). What changes is the promise: today's spec
says *nothing checks it*, which is true; every draft on the table says an
unmarked parameter is refused a lend, which is true of the expression and false
of the pointer.

**The corpus already knows the shape.** `tests/golden/run/lease-tail-points-into-the-bytes.hero`
(panel 124, defect 024's second half) declares `after_dash(s: cstr, @tail: cstr)`
over a header whose body is `*tail = strchr(s, '-') + 1`, and its own comment
reads: *C does not keep the pointer here: it hands back a pointer INTO the bytes
it was handed.* Panel 124 closed it with a lease in the caller's cell, under the
old sentence. The ffi seat's § 5 names the same shape in `sqlite3_prepare_v2`'s
`*pzTail` (`sqlite3.h:4472`) and calls the flip *still safe there* because the
alias is valid *for as long as it can be named*; my `c07` names it and hands it
to a keeper in the same function, and the keeper reads it after the frame dies.
So the safety argument holds only while nothing KEEPS the alias, which is the
one thing the unmarked default now says a parameter does.

**Where the three sentences over-promise, exactly:**

- the ergonomist's, *an unmarked `cstr` or `ptr` parameter ... takes a lease or
  a pointer C owns*: a pointer C **answered** is not a pointer C **owns**, and
  `c50` is the counterexample;
- the warden's M2, *a lend reaches only one so declared*: true of the lend
  expression, silent about where the pointer goes next, so the sentence is not
  false, and it is not the guarantee a reader takes from it;
- the engineer's, *an unmarked parameter ... takes a lease*: false for `nullptr`
  and for a C-returned value (both admitted, run: `c21_lent_takes_lease_and_c`
  exit 0), which is the warden's condition (iv) landing on the engineer's draft.

**What would close it: named, not chosen.** (a) A word on the RESULT or the
`@` cell saying *this answer points into what was lent*, which the vocabulary
does not have: `owned f`, `acquires f`, `borrows` are the result words and none
relates an answer to an argument. (b) Treat every C-returned `cstr`/`ptr` as a
lend: it *stands only as an argument of a call* and reaches only a `lent`
parameter unless its declaration says C owns it. That refuses `getenv -> puts`
until `puts` is marked (truthfully), and the ffi seat's condition **objects** to
exactly that. (c) Accept it and WRITE it: the sentence names the boundary, the
rule reads the lend expression and not the pointer's later life, and
design.md §4.19 or the diagnostic says so, because CL-005 holds a refusal to the
standard of naming what would make it wrong. The synthesis must pick one and
say which; this seat only says that (c) with the ergonomist's *a pointer C owns*
left in is not a choice, it is a false sentence.

### 1b. The shapes that held, each run under the rule

Refused, with the class that refused it: a lend bound by `=`, by `: cstr @`,
assigned into an `@` cell, in a record constructor (a group's record with a
`cstr` field, `Opt(name: x.cstr())`), in an array literal, in an `if`
expression's arms, in a binary expression, written to a Heroes `@out: cstr`
parameter, all `cstr_escapes` (the existing rule); a `ptr` lend into a group
record's `ptr` field, `field_lend_needs_a_header`; a lend into a generic
`id<T>(x: T)`, into `ok(...)`, into the library's own `validated(c: cstr)`, into
a wrapper by UFCS (`"12".cstr().wrap()`), through a function value (`h = wrap;
h("p".cstr())`), all `lend_needs_a_header`; a lend as UFCS receiver of an
unmarked extern (`"12".cstr().atoi()`), in prefix form `cstr(s)`, positional,
from a `str` field, a literal, all `lend_kept`; a lend returned from a Heroes
function, `cstr_out_of_heroes`; named arguments reordered (`two(b: ..., a:
...)`), `wrong_label` twice, **so the rule's position-indexed lookup
`params[position]` is safe**; the same C function declared twice in one module,
`declared_twice`; `lent` on `i64`, on `@`, on `@rest: cstr`, `lent_shape`; `lent`
before `counted_by`, twice, on a result, inside a function type, on a Heroes
parameter, after `owned f`, `lent consumes`, all refused at parse or as
`unread_mark`. Admitted where it should be: a lease at a marked parameter, a
C-returned value and `nullptr` at a marked one, `lent` on a `static inline` in
a local header (`c24`, exit 0, the ffi seat's shim route lives), a parameter
**named** `lent` beside `counted_by lent` and a Heroes function named `lent`
(`c23_ident_lent`, exit 0).

### 1c. Three smaller gaps, run

- **`lent` on a `ptr` without `counted_by` is accepted in silence** (`c14`,
  `c20_ptr_lent_uncounted_decl`: exit 0). Nothing can ever be lent to such a
  parameter (a `.ptr()` there is `field_lend_uncounted`), so the mark is dead
  text, and the ergonomist's invention 2 asked for it to be refused. One more
  arm of `lent_shape`.
- **Two modules, one C function, two marks: admitted.** `alt.hero` declares
  `keep(s: cstr lent)` and lends; `main.hero` declares `keep(s: cstr)` and
  leases; `check` 0, `run` prints `7`. The historian's B.4 mechanism (*two
  declarations of one C function disagreeing on the mark*) is live and no rule
  relates them. Not a soundness hole of this rule, since each module's
  declaration governs its own lends, but a wrong `lent` in one module is not
  caught by an honest declaration in another, and the record should say so.
- **The `lend_kept` notes are not shaped by the lend's kind.** For a `.ptr()`
  lend (`c16`) the first note suggests `` `p: cstr lent` `` for a `ptr`
  parameter, and the second offers `` `x: cstr @ s.lease()` ``, a lease route
  that does not exist for a field (spec § 13: *Nothing lends a field to `cstr`*;
  the field lease was route A and was struck at panel 168). The ergonomist's
  invention 5 asked for exactly this not to happen. Wording, two lines.

## 2. The prices, reconciled on the real tokenizer

Measured in my copy with `./heroes measure spec/heroes-spec.md --refresh`
(`.env` sourced, key length 108), each draft applied to the real path and the
file restored byte-identical afterwards (`git status --short spec/` empty):

| draft | real | Δ | cl100k |
|---|---|---|---|
| baseline | 8201 | | 6159 |
| **E**, the engineer's diff as written: its sentence + `CParam` slot, no example | 8247 | **+46** | 6194 |
| E + G, the same with the sqlite example marked | 8249 | +48 | 6195 |
| **M2 + G**, the warden's merged sentence + slot + example | 8216 | **+15** | 6171 |
| the ergonomist's 39-word sentence + slot + example | 8243 | +42 | 6188 |

**No, they are not the same sentence.** The engineer's +46 is reproduced
exactly; the warden's M2 measures +15 here against its own +13 (my line wrap
differs; the warden's number stands as theirs). They are 31 to 33 real tokens
apart, and the difference is two clauses the engineer's carries and M2 drops:
a gloss (*which says the call keeps nothing of it*) and *takes a lease*, the
clause the warden's condition (iv) flags as false before the admitted set is
settled, which § 1 shows it is. The engineer called its own *a draft for the
warden*; the synthesis should take it at its word.

**Yes, M2 covers the header clause by construction.** *A lend reaches only one
so declared*: a Heroes function's parameter cannot be declared `lent`, run,
`function wrap(s: cstr lent)` is `expected_params_close` at parse (`c21`), so
under M2 no lend may reach a Heroes parameter, which is exactly what the built
rule does at `c09b`, `c22`, `c06` and b10. M2 says nothing the engineer's veto
condition needs and nothing it forbids. Its one silence is § 1's: it speaks of
the lend and not of the pointer.

## 3. The landing count, from the world

Every extern parameter typed `cstr` or `ptr` under `selfhost/` (code, plus the
library text), and under `tests/harness/`, by `grep -E 'function .*: (cstr|ptr)'`
over the four files that carry an extern group with one (`selfhost/cli/io.hero:34`
also carries a group, `hero_write_err(text: str)` and `hero_word_bits()`, **no
`cstr` or `ptr` parameter**, run). **No `ptr` parameter exists in any of the
three trees**: the only `: ptr` hits are string fixtures in
`selfhost/cli/syntax_cmds.hero:320-328`.

| tree | functions with a `cstr` param | params | of which receive a lend | lend expressions (lines) |
|---|---|---|---|---|
| `selfhost/cli/process.hero:42-53` + `selfhost/emit/literal.hero:42` | 9 | 13 | **13 / 13** | 13 (12) |
| `selfhost/library_source.hero:152,153,163` | 3 | 3 | **2 / 3** (`hero_str_try_from_cstr(p: cstr)` at `:163` receives the name `c` at `:196`, never a lend) | 2 (2) |
| `tests/harness/shell.hero:44-57` | 9 | 13 | **12 / 13** (`hero_fs_remove(path: cstr)` at `:49` is declared and **never called**: one grep hit, the declaration) | 16 (15; line 400 carries two) |
| **total** | **21** | **29** | **27 on 19 functions** | 31 (29) |

**So 28 is what the engineer MARKED, not what the landing requires.** Its
`ce-copy` diff adds 12 + 1 + 2 + 13 = 28 `lent` tokens, including
`hero_fs_remove(path: cstr lent)` in the harness (truthful, unrequired) and
leaving the library's `:163` unmarked (correct). The minimum is **27 parameters
on 19 functions**; the census is 29 on 21. The chain of numbers is therefore
9/12 (brief, `selfhost/` only, lines not parameters) → 11/15 (ffi seat, adds
the library, omits the harness) → 20/28 (engineer, marks one parameter nothing
lends into) → **19/27 required, 21/29 in the census**. The engineer's two
findings stand as run: the library's two are in every compilation, and the
harness is a third tree.

**A prediction this count falsifies before it is scored**: the warden's P2 says
the word appears *exactly 13 times in `selfhost/` extern declarations*. The
library text is `selfhost/library_source.hero`, so the landing writes **15**
under `selfhost/`, and P2 as worded scores false for a reason it did not mean.
It should say 13 in `process.hero` and `literal.hero` together, plus 2 in the
library.

## 4. The ergonomist's six inventions against the built rule

| # | the ergonomist assumed | the built rule, run | same? |
|---|---|---|---|
| 1 | a `lent` parameter also takes a lease or a C-owned pointer | lease, C-returned value, `nullptr` all admitted at a marked parameter (`c13`, `c21_lent_takes_lease_and_c`: exit 0) | **yes** |
| 2 | `lent` on a `ptr` without `counted_by` is refused as dead text | **accepted in silence** (`c14`, `c20`: exit 0) | **no**, § 1c |
| 3 | `lent` on a handle, a number, an `@` parameter is refused | `lent_shape` on `i64`, `@i64`, `@cstr`, five shapes in `c42` | yes |
| 4 | `f.ptr()` now needs `lent` beside `counted_by n` | unmarked counted `ptr`: `lend_kept` (`c16`); marked: exit 0 (`c18b`); `lent` before `counted_by` refused at parse (`c18`) | yes |
| 5 | the diagnostic names both routes, and offers no lease for a FIELD lend | both routes named; **the lease route is offered for `.ptr()` too, and the first note says `cstr` of a `ptr` parameter** (`c16`) | **no**, § 1c |
| 6 | where `end_lease` goes is the program's | not the compiler's; nothing to run | n/a |

Two of six differ, and in both the spec sentence is silent: nothing in any
draft says whether a mark nothing can read is an error, and nothing says what
the refusal offers a field. The synthesis should say both, or say they are the
implementation's.

## 5. Defect 070, run against the built rule

| shape | `check` | `run` ×3 | stderr | `--sanitize` |
|---|---|---|---|---|
| the reproducer: a lease into unmarked `eat(s: cstr)` that frees (`c17_070_lease`) | **0** | **133 133 133** | empty | 134, `bad-free` |
| the fourth shape: a plain `.cstr()` lend into the unmarked freer (`c17b`) | **1**, `lend_kept` | | | |
| the fourth shape after following the diagnostic's own first note, `eat(s: cstr lent)` (`c17c`) | **0** | **134 134 134** | empty | 134, `BUS` |
| a lease into the freer through a Heroes wrapper (`c24b`) | 0 | | | |

**070 is exactly where it was**: the rule admits a lease at an unmarked
parameter by design, and the give-away hands a lease. What moves is its fourth
shape: the plain lend into a freer is now refused, and the refusal's first note
(`guess`) is *say so where it is declared: `s: cstr lent`*; an author who
follows it gets `check` 0 and a bus error, because `lent` says *does not keep*
and says nothing about *frees*. The second note names this in its last
sentence (*a lease is wrong too where C FREES*), which is the right sentence in
the right place, and the first note should not offer the word without it. The
class panel 170 vetoed a single word for is unchanged.

## 6. What the synthesis must say, in one list

1. Whether a C-returned `cstr`/`ptr` is inside or outside the guarantee (§ 1),
   and if outside, the sentence must not say *a pointer C owns*.
2. Which sentence lands: M2 at +13/+15 covers the header clause by
   construction; the engineer's carries a clause § 1 shows false.
3. The landing minimum is 27 parameters on 19 functions across three trees;
   28/20 is fine and one of them is unrequired.
4. `lent` on an uncounted `ptr`: refused or documented as inert.
5. The `lend_kept` notes shaped by the lend's kind, and the `lent` suggestion
   never offered without the FREES sentence beside it.
6. Two modules may carry two marks for one C function; write down that nothing
   relates them, or that something should.
7. Re-word the warden's P2 before it is scored.

## What I did not do, said plainly

I did not build the rule myself; I ran the engineer's `heroes-B2`, whose
provenance (commit-A seed → heroes-B → seed-B → clang) is the engineer's
report and not re-verified here beyond `b10` and `b1` behaving as it says. I did
not run any suite. I did not measure the vendored figures the `DELTA_GATE`
judges beyond what `measure` prints offline (in the table). The C11 text on
`strchr` (7.24.5.2, *returns a pointer to the located character*) is cited from
the man page's sense and not re-read; the run is the measurement. I did not
search for a fourth door beyond result, out-cell and callback; a struct field
of a group's record that C fills with the lent pointer is the one I would try
next, and it is unrun.
