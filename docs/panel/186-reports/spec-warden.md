# Panel 186, the spec-warden's report

Written 2026-10-02 by the spec-warden. Status: **complete** (first pass,
before reading any other seat's report; the session stopped once on the
account's limit at about 11:35 and resumed at 11:41 on the same copy, which
had kept every file listed below).

Inputs read: `docs/panel/186-briefs/spec-warden.md`, `00-shared.md`,
`blind/brief.md`, `blind/spec-marker.diff`, `blind/sa.h`, the critic's first
pass (`docs/panel/186-reports/completeness-critic-briefs.md`), CLAUDE.md
§ Hard stops, § RUN IT and § 2, `spec/heroes-spec.md` in full, panel 073
(lines 52-175), panel 077 (lines 4-125), panel 035 (lines 120-200),
`.claude/rules/spec-shape.md`, the ledger's newest rows.

**The ceiling, grepped today**: design.md §1.6 (`docs/design/design.md:255-256`)
says *10240 tokens, measured by `claude-opus-5` through `POST
/v1/messages/count_tokens`*. This seat's own file says the same.

**My copy**: `<scratchpad>/186-spec-warden/`, `git -C <trunk> archive
779139d0 | tar -x -C <copy>` (its `.claude/` holds `agents hooks rules
settings.json skills`, no `worktrees`); compiler built inside it from the
seed (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, `heroes
0.2.0`); `HEROES_RUNTIME` set to the copy's `runtime/`. The briefs' probes are
untracked in the trunk, so `git archive` does not carry them: I copied
`docs/panel/186-briefs/probes/` and `blind/` into `<copy>/work/` and ran them
there. No paid run (`measure` never with `--refresh`), at most one process at
a time, nothing timed. The splicer is `<copy>/work/splice.py`, every anchor
asserted to match exactly once; the spliced files are `<copy>/work/spec/*.md`.

## 0. The baseline, measured in my copy

`./heroes measure spec/heroes-spec.md`, 11:09, exit 0 (the tool's em dashes
written here as `--`):

```
  claude-legacy      6716   (64995 ranks)
  cl100k_base        6838   (100256 ranks)
  maximum            6838   a lower bound, not the reader's tokeniser
  spread              122   between the two vendored tables (1%)
  real               9060   claude-opus-5, 2026-09-28 -- the binding number
Headroom: 1180 against the 10240 ceiling -- but the FFI floor mortgages 60 of
it (panel 030 R3), so what is measured against the ceiling is 9120
```

Identical to the brief's input. **Spendable: 1120 real** (10240 less 9120).
`DELTA_GATE` is 50 vendored, compared as `moved > DELTA_GATE`
(`selfhost/cli/measure.hero:136`), so +50 is inside the window; the payment
rule binds at every size regardless (design.md §1.6, panel 123 R7).

**The real count of a spliced file is not measured here** (no paid run). The
prediction rests on the last five ledger rows that carry both deltas
(`docs/measurements/010-spec-budget-ledger.md`, panels 164, 166, 169, 173,
177): real over vendored was 66/48, 48/37, 47/33, 54/40 and 37/32, so
**1.16 to 1.42**. Every real figure below is that range applied, an
inference until the landing's `--refresh` scores it.

## 1. The reproductions, in my copy

`./heroes build <case> -o <bin>` then the binary, one at a time:

| case | build | run | stderr |
|---|---|---|---|
| `u17_anon_constructed` (`SA` naming `kind i f x`, built `i: 7, f: 0.5`) | 0 | `1056964608` | 3 warnings: excess elements in struct initializer (the probe `SA v = {0,0,0,0};`), initializer overrides (the construction) |
| `u18_anon_compared` | 0 | `true` | 2 warnings |
| `u07_anon_omits_x` | 0 | `12` | silent |
| `lane-literals/u08_anon_one_member` (`kind i x`) | 0 | `15` | silent, correct |
| `u19_struct_omits_middle` | 1 | | *`S3` does not name `c`* with `c` declared |
| `read` (`W` naming `i n`, read only) | 0 | `7` | 2 warnings, excess elements in union initializer |
| `critic/one_arm` (`SB` naming `kind b`, `==`) | 0 | `true` | silent, wrong |
| `critic/one_arm_union` (`UB` naming `b`, `==`) | 1 | | `ffi_union_field` |
| `critic/bf` (bit-fields) | 2 | | *internal error*, clang's *invalid application of 'sizeof' to bit-field* and *address of bit-field requested* |
| `lane-literals/u10_partial` (`W2 partial` naming `i n`) | 0 | `7` | silent |

And the read-only and one-member bindings that every route must keep, re-run
rather than taken from the lane's `summary.txt`: `u01_two` 7, `u02_three` 8,
`u04_aggregate_first` 10, `u05_aggregate_second` 11, `u06_anon_all` 15,
`u09_named_union_inside` 13, `u14_two_unions` 15 (each exit 0, correct, with
defect 150's 2 or 4 warnings); `u15_one_of_two` 7 (`W2` naming one of two) and `u03_one` 9 (`W1`, a
union of one member) (exit 0,
zero warnings).

**Three programs of mine over `blind/sa.h`** (`<copy>/work/sw/`), asking
whether `partial` already gives the blind task a sound form on today's
compiler:

| program | build | run |
|---|---|---|
| `p1_partial_omit`: `record SA partial` naming `kind i f x`, `SA(kind: 2, f: 1.5, x: 4)` | 1 | `missing_fields`: *`SA` is built with every field, named* |
| `p2_partial_all`: the same record, `SA(kind: 2, i: 0, f: 1.5, x: 4)` | 0, 1 overrides warning | `12`, `1.5`, `4` |
| `p3_two_records`: `record SA partial` naming `kind f x` | 0, silent | `1.5`, `4` (and `i` cannot be read) |

So p2 is right **by declaration order**: `f` is declared after `i`, and C
keeps the last write, which is u17's `0.5` winning over `7`. **There is no
sound, warning-free program today that reads `i` from `make_sa()` and builds
an `SA` holding `f`**, which is the critic's § 7 confirmed from the other
side.

**One shape the routes owe and nobody listed: an anonymous struct inside a
union** (`<copy>/work/d2/d2.h`, `typedef struct { int32_t kind; union {
struct { int32_t a; int32_t b; }; int64_t q; }; } SD;`).
`/usr/bin/clang -std=gnu11 -c -o /dev/null -Xclang -fdump-record-layouts`:

```
         0 | SD
         0 |   int32_t kind
         8 |   union SD::(anonymous at ./d2.h:2:32)
         8 |     struct SD::(anonymous at ./d2.h:2:40)
         8 |       int32_t a
        12 |       int32_t b
         8 |     int64_t q
           | [sizeof=16, align=8]
```

and under `-Winitializer-overrides`, `{.kind = 1, .a = 2, .b = 3}` is silent
while `{.kind = 1, .a = 2, .q = 3}` warns. C11 6.7.2.1p13 makes `a`, `b` and
`q` all members of the union, by promotion. So a sentence saying *built
naming exactly one member of each union*, read in C's own vocabulary,
**refuses `{kind, a, b}`, the correct construction of the struct arm**. M and
N both say it. This is a truth question about the words, and it is the one
the candidates as drafted do not survive.

## 2. Must the spec say anything? (task 2)

**Yes, and L is false today, before any route.** Spec § 13 says *A group's
`record` is the header's struct: all its fields*. For `SA` the header's
fields are `kind`, `i`, `f`, `x` (C11 6.7.2.1p13), so the spec tells a reader
to write u17's record. § 9 then licenses `SA(kind: 1, i: 7, f: 0.5, x: 3)`,
and § 7 says *`==` is structural equality on any two values of one type*. u17
holds `i` and `f` in one 4-byte slot, so **no compiler can give that
construction the meaning the spec promises**: u17 prints `1056964608` for
`i: 7`. CLAUDE.md § 12 says the spec beats the compiler. That presumes a spec
a compiler could satisfy, and for `SA` this one is not.

**Panel 073's +0 does not reach `SA`.** That warden priced the union rule at
+0 because *a union is not a struct* (`073...:54`, `:111-113`), so a record
over a union lay outside § 13's promise and the compiler could refuse
inside a silence. `SA` is a struct. Its record is inside the promise, and the
promise is what is wrong.

**Under each route, without a sentence:**

- Any route that refuses u17's construction or u18's `==` (every sound one)
  contradicts § 9 and § 7 as written. § 12 then calls the compiler wrong for
  the only behaviour that is right.
- A route that keeps them (today) keeps defect 151.
- The precedent that lets a rule stay out of the spec, panel 035 R4 and panel
  077 item 7 (*already loud ... buys no silence for its tokens*), covers a
  **silence** whose every wrong guess is exit 1. Here the existing text
  **affirmatively licenses** the refused program, so R4 does not apply.

**The same holds for the plain union record, more weakly.** `one_arm_union`
is refused at `==` today with no spec sentence (measured, exit 1), which §
7's *any two values* contradicts. It has stood since panel 077 on the +0
reading, and it is a silence rather than a falsehood. Once the spec says
*C union* at all, spec-shape.md's *every rule has exactly one home* puts the
union record's rule in the same sentence. Every candidate below does.

**`partial` is no escape in the spec's own words.** *`record Font partial`
names only some*, and p1 shows a `partial` record is still built naming
every declared field. So the spec gives no form for the blind task, and
`partial` does not change that.

**Principle 0** (CLAUDE.md § 2): **`needed_for_self_hosting: no`**. The
compiler's five `extern` groups (`selfhost/cli/link.hero`, `process.hero`,
`io.hero`, `files.hero`, `selfhost/emit/literal.hero`) declare **zero**
records (an `awk` over each group's body). The sentence is no new form. It is
the spec made true about a defect repair, and the thesis is served by
construction: u17, u18 and `one_arm` are plausible mistakes that exit 0 with
a wrong answer today (panel 073's ergonomist predicted at least 8 in 10
readers write that program), and any route below turns them into exit 1.
The burden for a **route** is met. The burden for a **surface form**, (1d),
is not (§ 4).

## 3. The candidates, priced (task 1)

Each spliced into a copy of the spec at its anchor, `./heroes measure
<copy>/work/spec/<id>.md`, vendored only. Δ is against 6716 / 6838. My drafts
use a comma where the spec's `partial` sentence has its dash; the comma and
dash forms measured identically on every draft I ran both ways.

| id | the text (inserted after *not the field list's.* unless marked) | legacy | cl100k | Δ vendored | real, predicted |
|---|---|---|---|---|---|
| L | none | 6716 | 6838 | +0 | +0 |
| M | the coordinator's M, verbatim | 6774 | 6896 | **+58** | +67 to +83 |
| N | the coordinator's N, verbatim | 6761 | 6883 | **+45** | +52 to +64 |
| M_hold | M, ending *compile errors, for it and for any value holding it.* | 6783 | 6905 | +67 | +77 to +95 |
| N_hold | N, the same ending | 6770 | 6892 | +54 | +62 to +77 |
| O | *A field that lies in a C union, named or anonymous, shares its bytes with the union's other members: a record names one or more members of each union and reads any of them, is built naming exactly one, and comparing it and using it as a map key are compile errors, for it and for any value holding it.* | 6784 | 6906 | +68 | +79 to +97 |
| O_short | *A field in a C union, named or anonymous, shares its bytes with its other members: a record names one or more of each union, reads any, and is built naming one; comparing it and using it as a map key are compile errors, for it and for any value holding it.* | 6776 | 6898 | +60 | +69 to +85 |
| O_depth | O, with *is built naming exactly one, an anonymous struct's fields counting as one member,* | 6794 | 6916 | +78 | +90 to +111 |
| M_depth | M_hold, with *an anonymous struct's fields counting as one* | 6792 | 6914 | +76 | +88 to +108 |
| O_merged | partial's sentence rewritten to share the refusal clause (8 diff lines) | 6757 | 6879 | +41 | +47 to +58 |
| M_merged | the same merge with M's rule | 6757 | 6879 | +41 | +47 to +58 |
| N_merged | the same merge with N's rule | 6743 | 6865 | +27 | +31 to +38 |
| O_merged_depth | O_merged with the depth clause | 6766 | 6888 | +50 | +58 to +71 |
| **O_final** | **replaces** spec § 13's *`record Font partial` names only some ... not the field list's.*, below | 6777 | 6899 | **+61** | **+71 to +87** |
| A_pairs | *Building or comparing a record that names two fields sharing bytes, as two members of one C union do, is a compile error.* | 6742 | 6864 | +26 | +30 to +37 |
| C_refuse | *A struct holding an anonymous union is no group's `record`, not even a `partial` one.* | 6737 | 6859 | +21 | +24 to +30 |
| F_sum | *A record whose fields together outsize C's struct, as two members of one C union do, cannot be built or compared.* | 6742 | 6864 | +26 | +30 to +37 |
| G_broad | partial's sentence rewritten: *`record Font partial` names only some. Comparing a group's record that has fields and using one as a map key are compile errors, for it and for any value holding it. Its size stays C's, not the field list's.* | 6721 | 6843 | +5 | +6 to +7 |
| D_nested | *A C union inside the struct, named or anonymous, is written `union` with its members indented below; a record is built naming one of them, and comparing it and using it as a map key are compile errors, for it and for any value holding it.* and `Member`'s record arm takes `CFields = INDENT { ident ":" Type NEWLINE \| "union" CFields } DEDENT .` | 6795 | 6918 | +79 / +80 | +92 to +114 |
| B1_refuse | *A bit-field has no width a field can declare, so a struct holding one is bound `partial`, leaving it out.* | 6742 | 6864 | +26 | +30 to +37 |
| B1_merged | after *as many elements as the type says.*: *A bit-field is none of these: leave it to `partial`.* | 6731 | 6853 | **+15** | +17 to +21 |
| B2_read | *A bit-field is declared at the type before its colon, `flag: u32` for `unsigned flag : 1`, and read like any field; building a record holding one is a compile error.* | 6758 | 6881 | +42 / +43 | +50 to +61 |
| B3_abort | *A bit-field is declared at the type before its colon, `flag: u32` for `unsigned flag : 1`, and building one with a value its bits cannot hold aborts.* | 6755 | 6878 | **+39 / +40** | +46 to +57 |
| O_final + B1_merged | both, one file | 6792 | 6914 | **+76** | +88 to +108 |
| O_final + B3_abort | both, one file | 6816 | 6939 | **+100 / +101** | +117 to +144 |

**O_final**, the text that replaces spec § 13's three lines from
*`record Font partial` names only some* to *not the field list's.*
(`<copy>/work/O_final.txt`):

> `record Font partial` names only some, and its size stays C's, not the field
> list's. A field in a C union, named or anonymous, shares its bytes with the
> union's other members: a record names one or more of each union, reads any,
> and is built naming exactly one, an anonymous struct's fields counting as
> one. Comparing such a record or a `partial` one and using it as a map key are
> compile errors, for it and for any value holding it.

**The suites that read the spec, run with O_final in my copy's
`spec/heroes-spec.md` and restored after (`cmp`):** `spec` 16 passed, 4
failed, the four being `budget`, `spendable`, `real` and `ledger`, the pinned
numbers every spec change moves (the baseline read 20 and 0); `grammar` 9 and
0; `special` 10 and 0. So the sentence breaks no structural check (`shape`,
`anchors`, `offered`, `named`, `rejected`, `inventory` all pass). **With
D_nested**: `spec` 15 and 5, the fifth being **`spec/rejected`**: *these
appear in the spec's code and the lexer refuses them with a prescribed fix:
union*. `spec/reserved-words.md:13` reads *`union` is not a word in this
language*, use `variant`. `grammar` stays 9 and 0 with D_nested, which says
that suite does not cross-check the `Member` production.

**Every candidate fits the budget.** The dearest pair, O_final + B3_abort, is
predicted at +117 to +144 real against 1120 spendable. **No budget veto
exists in this sitting.** What decides is truth, and Principle 0 for (1d).

## 4. Verdicts per candidate sentence

- **L: veto.** The spec is false for `SA` today (§ 2). Under any sound route,
  L makes § 7 and § 9 contradict the compiler, and § 12 then reads the right
  behaviour as the bug. This is a veto on a spec that would be false, not on
  tokens.
- **M: object.** It is true only under a route that knows union membership
  at any depth, (1e). *A record names every member* turns u15 (`W2` naming `i`), panel
  073's one-member binding and its one record per arm by `tag` (the SDL
  pattern) into incomplete records needing `partial`. That breaks a reading
  the shared brief says every route keeps, though loudly and by one word.
  *Exactly one member* refuses `SD`'s correct `{kind, a, b}` (§ 1). It omits
  *for any value holding it*, which the detector enforces
  (`extern_union.hero` `reach`, total since defect 140).
- **N: object.** Read as written, *a C union ... is bound one member at a
  time* refuses the seven read-only programs re-run in § 1 (`u01`, `u02`,
  `u04`, `u05`, `u06`, `u09`, `u14`) and `read.hero`, all exit 0 and correct
  today. That is panel 073's first reading, which every route must keep. It
  leaves the blind task inexpressible, since `SA` has one record (the
  critic's `ffi_unknown_tag`). It shares M's depth-2 error and omission. It
  is true under a compiler that enforces it, so object, not veto.
- **O_final: approve**, conditional below. It keeps all three of panel 073's
  readings: `read.hero`, u15 and one record per arm stay legal. It is panel
  073's filed construction-arity form (`073...:119-123`). It expresses the
  blind task (`record SA` with `kind i f x`, built `SA(kind: 2, f: 1.5, x:
  4)`, `make_sa().i` read), names the transitive refusal the compiler already
  makes, and is true for `SD`. It folds partial's own copy of the refusal
  clause into one, so *merging beats appending* (spec-shape.md) holds:
  O_final costs 17 vendored less than O_depth appended.
- **O, O_short, O_depth, O_merged, O_merged_depth: object** as dominated.
  O, O_short and O_merged are false at depth 2. O_depth costs more for the
  same rule. O_merged_depth says *its other members* and *either record*, two
  ambiguous references O_final repairs for 11 tokens.
- **M_hold, N_hold, M_merged, N_merged, M_depth: object**, on M's and N's
  grounds. The merges fix the transitive omission and nothing else.
- **A_pairs: object.** True only under (1a) or (1b), and it ratifies
  `one_arm`'s `true` for two different C values (measured).
- **C_refuse: object.** True under (1c), which refuses u08 (prints 15,
  correct). CLAUDE.md § 12 holds a refusal to a feature's standard, and u08 is
  the program that falsifies it.
- **F_sum: object.** True under (1f), but its rule lives in the header's
  layout: the spec has no `sizeof`, so no reader can apply it from the line
  (design.md §1.3).
- **G_broad: object as the whole answer.** It is the cheapest true comparison
  clause (+5) under an unconditional (1g). It says nothing of construction,
  so u17 stays licensed. It cuts `==` from every union-free group record.
  How many tracked programs compare two group records is unrun from this seat.
- **D_nested: veto** on Principle 0. No compiler need (zero records in the
  compiler's groups) and no measured thesis effect (metric 2 has zero tasks:
  `harness/tasks/` holds only its README; design.md:3048 says such a
  prediction *pays nothing*). It also collides with a measured refusal:
  `spec/rejected` goes red on `union`. Under (1e) it restates what the header
  already says, which gives a second source for the grouping that can
  disagree with the first.
- **B1_merged: approve** as the conservative bit-field sentence (+15).
- **B3_abort: approve** as the production-ready one (+40). The bit-field is
  read and built, and a value its bits cannot hold aborts, consistent with §
  7's *Overflow aborts at every width*. spec-shape.md says *each site that
  aborts says so beside its operation*, so this route owes the sentence. It is
  only true if the route gives the emitter the bit width (the AST or the
  layout of (1e)).
- **B1_refuse: object**, dominated by B1_merged (the same rule for 11 more
  tokens). **B2_read: object**: a compromise that reads the field and never
  builds the struct. **L for the bit-field: veto** under a refusing route
  (*a field is a number ... at the header's own width* licenses `flag: u32`)
  and **object** under B3 (the abort would be unstated).

## 5. Verdicts per route

- **(1a) per-pair ranges: object alone, veto with M, N or O.** It is blind to
  `SB` (measured `true`). Each of the three sentences says comparing such a
  record is a compile error, and under (1a) the compiler would accept it.
  Alone it can only carry A_pairs, which ratifies the wrong `true`.
- **(1b) designated probe under `-Winitializer-overrides`: object alone, veto
  with M, N or O**, on (1a)'s grounds. Emitted for every record it also
  refuses `read.hero` (the critic's § 9).
- **(1c) refuse a struct holding an anonymous union: object**, on u08. **Veto
  without C_refuse**, since § 13 then licenses what it refuses.
- **(1d) a nested group: veto** (§ 4, D_nested).
- **(1e) the header's layout read from clang: approve, with O_final in the
  same commit.** It is the only route listed that can make O_final's words
  true at every shape in § 1: `SA`'s two arms, `SB`'s one arm compared, `SD`'s
  struct arm, `u07`'s and `u16`'s omissions, and the bit-field's width for
  B3. My approval is void if it cannot see one of those, because a word of
  O_final then becomes false.
- **(1f) classify or sum: veto with M, N or O** (false for `PADU` and `SB`,
  the critic's § 6), **object** with F_sum (§ 4).
- **(1g) refuse `==` on every group record not proven union-free: object as
  the whole answer** (§ 4, G_broad). If the proof is (1e)'s, the sentence is
  O_final's and not G_broad.
- **(1h) the construction-arity form**: its rule is O_final's construction
  clause. Approve inside O_final, object in M's wording.

**Q2's union-record half is decided by this sentence, not by the probe.**
O_final says one member named is complete. That is the code comment's reading
at `extern_record.hero:91-92`, now made the spec's, so panel 073's hole 1
(*claims completeness falsely*) closes by being stated true, with `==`
refused regardless. M says the opposite. Whichever the sitting adopts, the
probe follows it.

## 6. The messages against the spec's words (task 3)

1. **`ffi_union_field`, both texts** (`selfhost/emit/ffi_record.hero:176`,
   `:194`) open *`` `T` is a `union` in `<header>` ``*. That is true for
   `UB` and `W`, which are unions, and **false for `SA` and `SB`**, which are
   structs. Under O_final the struct case needs its own text in the
   sentence's words, e.g. *`b` lies in a C union inside `SB` (`one_arm.h`),
   so comparing an `SB` or using one as a map key asks about bytes `b` does
   not cover*. The union-type form can keep its opening, so the two goldens'
   first clauses need not move.
2. **`hash` is not a spec word.** The one-member text says *comparing or
   hashing one*, and its note *`==`, `hash` and being a map key* (`:176-178`).
   `grep -c -i hash spec/heroes-spec.md` reads **0**, and no built-in is
   called `hash`, so the backticked `hash` names something a program cannot
   write. The spec's words are *comparing it and using it as a map key*.
   Object, low: the two goldens holding the text would move.
3. **The two-member note** (`:196`), *a `record` over a union may name ONE
   member ... Two or more is what has no answer*, is **false under O_final
   and M**: two declared members are legal, and what is refused is building
   naming two, and comparing at any arity. It is true under N only. It must
   change with O_final.
4. **The one-member note's** *Mark the `record` `partial` if you want that
   refusal stated at the declaration* is true today and under O_final. Under
   M it becomes the requirement.
5. **`ffi_incomplete_record`** (`:54`): *a group's `record` IS the header's
   struct* is the spec's own sentence, which is right. *says the struct has
   it* is false for a union type under M. Under O_final a record naming no
   member of `SA`'s union needs *names no member of the union holding `i` and
   `f`*. **u19's text is false today** (*`S3` does not name `c`* with `c`
   declared, exit 1 in my copy), against design.md §4.17. The critic's guard
   in `incomplete_record` is the repair whatever the probe becomes.
6. **The note** *the assertions above check every field you DO name*
   (`:58`) refers to generated C the author never sees. It is no spec term.
   Pre-existing, a smaller repair.
7. **The bit-field** is exit 2 with clang's text (§ 1), against
   `.claude/rules/c-boundary.md`. Under B1 or B3 its message must say
   *bit-field* and *`partial`* (B1) or *aborts* (B3), the sentence's words.

## 7. The warden's structure

- **verdict**: **veto** L (the spec is false for `SA` without a sentence),
  D_nested and (1d) (Principle 0, and the measured `union` refusal), and M, N
  or O paired with (1a), (1b) or (1f) (false for `SB` or `PADU`). **Approve**
  O_final with (1e), plus B1_merged or B3_abort for defect 156. **Object** to
  the rest as listed.
- **section**: design.md §1.6 (the budget and the payment rule), §1.3
  (locality, against F_sum), CLAUDE.md § 2 (Principle 0, against (1d)) and
  § 12 (a spec that would be false).
- **spec_token_delta**: measured vendored, O_final **6838 to 6899 (+61)**;
  with B1_merged **6914 (+76)**; with B3_abort **6939 (+101)**. Real
  predicted +71 to +87, +88 to +108 and +117 to +144, unmeasured (no paid
  run). Against 1120 spendable.
- **removal**: the merge. Partial's own copy of the refusal clause and its
  *and then* fold into one shared clause, 17 vendored below appending the
  same rule (O_final against O_depth). Nothing else in § 13 is made redundant
  by the rule, and I looked. The rest is paid by the registered predictions
  below, whose instruments exist today: a compile and its transcript, and
  `heroes measure`. A reader prediction would pay nothing while metric 2 has
  zero tasks.
- **needed_for_self_hosting**: no (zero records in the compiler's five
  groups).
- **argument**: § 13's *all its fields* licenses u17 and u18 for `SA`, whose
  meanings C cannot hold: `i: 7` prints `1056964608`. The defect is in the
  document, so a route without a sentence leaves § 7 and § 9 calling the right
  compiler wrong. Only O_final keeps panel 073's three readings, expresses the
  blind task, says what the detector already refuses transitively, and stays
  true for an anonymous struct inside a union (`SD`). M breaks the one-member
  binding, N seven running programs, and both are false at depth 2. By §1.2
  alone the sentence does not pay (unions are rare). It pays as a truth repair,
  which § 12 and the author's 2026-09-28 instruction rank above token economy.
- **prediction** (registered as payment, scored at the landing commit):
  (P1) a compile and its transcript: with (1e) and O_final, u17, u18,
  `critic/one_arm`, u07 and u16 exit 1, each message naming the fields and
  using *comparing* and *map key*, never `hash`. `read.hero` (7), u15 (7),
  u08 (15), `SD` built `{kind, a, b}`, and the blind task built naming `f`
  alone (`12`, `1.5`, `4`) exit 0 with zero clang warnings. Any one the other
  way scores O_final as not bought. (P2) `heroes measure --refresh` at the
  landing reads O_final's real delta in **+71 to +87**, and outside that the
  1.16 to 1.42 calibration is wrong.
- **condition**: I approve L, at +0, if a route is shown under which u17 and
  u18 compile and print what § 9 and § 7 promise; I believe none exists. I
  switch from O_final to M if the sitting rules that a one-member union
  binding claims completeness falsely (panel 073's hole 1 read strictly), and
  then M_depth with its transitive clause is the text. I withdraw the
  approval of (1e) if its layout cannot place `SB`'s one arm or `SD`'s struct
  arm. I raise the (1a), (1b) and (1f) pairings from veto to object if the
  sitting adopts A_pairs or F_sum's narrower text instead, since those are
  true.

## 8. What I did not run

The real count of any splice (paid). Any route built into the compiler. The
census over the 447 tracked files (the engineer's). `-fdump-record-layouts`
on any clang but this Mac's Apple 21. The `SD` shape through today's
compiler. Whether a union built naming one member leaves the other members'
bytes zero on every clang: C11 6.2.6.1p7 calls them *unspecified*, and
`extern_union.hero:47-48`'s *C zeroes the rest* is a premise about clang. That
is a question for the engineer and the pragmatist, and O_final promises
nothing about those bytes.
