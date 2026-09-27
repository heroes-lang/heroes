# Panel 180, shared brief: where a line inside brackets may break

Written 2026-09-27 at 12:05 by the coordinator, for every seat. Every number
and path below names the command that produced it, run while this brief was
written, on the trunk at `29ed5601` (`git log -1 --format=%h`; `git status
--short | wc -l` read 0).

## The sitting and why

**Defect 104** (`docs/work/DEFECTS.md`, the item whose first field is `104`):
spec § 0 ends, at `spec/heroes-spec.md:11-12` (`sed -n 11,12p`):

> Inside `(` `[` `{` a NEWLINE never ends a statement: where it separates, a
> production writes it; elsewhere it may fall between any two tokens.

The last clause is false. The lexer plants a terminator after a line-ending
token everywhere, brackets included (design.md §4.15, `docs/design/design.md`
lines 1932-1946, `sed -n 1932,1948p`: *"terminators, which are inserted
unchanged everywhere, brackets included"*, which §4.9's one-element-per-line
literals rely on), so a line inside brackets that ends with a name, a literal
or a closing bracket carries a NEWLINE token, and only some productions accept
one. The defect was filed as a false spec sentence to be repaired at a
sitting, priced on the reader's instrument. **Measuring the shapes beside it
for this brief found that the compiler is not uniform either**, so the sitting
decides two things: what the spec says, and whether the compiler is made to
say one thing.

**Sitting 180**: `ls docs/panel | grep -E '^1[78][0-9]'` on the trunk lists
170 to 177 and 179; 178 sits in a peer session's lane
(`/Users/joseph/Temp/heroes-lane-panel-178`, `ls` of its `docs/panel` shows
`178-…`). Four seats and a critic: the compiler-engineer (what the compiler
does, and what each resolution costs in it), the llm-ergonomist (what a reader
of the spec predicts), the spec-warden (the price in the reader's tokens and
Principle 0), the historian (how Go's and others' specs state this rule). The
ffi-pragmatist is left out because no resolution reaches C or a binding
(CLAUDE.md § 4, CL-023, choosing only the seats whose input differs); the
author can overrule that.

## What the compiler accepts today, measured

Programs of the shape `function main()` / `    xs = [1, 2]` / one statement
broken across two lines, run with `heroes parse` (syntax only, so a checker
error cannot mask the answer), the trunk's compiler built from its seed. The
files and their outputs are in
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/e7e3d0df-8db3-44f2-b628-af9bb1a6080e/scratchpad/p180/`
(`m_*.hero`, `r*.hero`); the table is their exit codes.

| the break | inside | exit | diagnostic |
|---|---|---|---|
| after an operator, `(1 +` / `2)` | a group | 0 | |
| after `.`, `(xs.` / `len())` | a group | 0 | |
| after `(`, after `,`, after `:` | a call's arguments | 0 | |
| after `[` | an index | 0 | |
| after `:` | a map literal | 0 | |
| **before** an operator, `(1` / `+ 2)` | a group | 1 | `expected_group_close` |
| **before** `.`, `(xs` / `.len())` | a group | 1 | `expected_group_close` |
| **before** `:`, `f(a` / `: 1, b: 2)` | a call's arguments | 1 | `expected_args_close` |
| **before** `:`, `{"a"` / `: 1}` | a map literal | 1 | `expected_map_entry_colon` |
| **before** `,`, `f(a: 1` / `, b: 2)` | a call's arguments | **0** | |
| **before** `,`, `[1` / `, 2]` | an array literal | 1 | `expected_expression` |
| before the closer `)` | a group, a call | 0 | |
| before the closer `]` | an array literal | 0 | |
| before the closer `}` | a map literal | 0 | |
| **before the closer `]`, `xs[0` / `]`** | **an index** | **1** | `expected_index_close` |
| a trailing `,` then the closer, `f(a: 1, b: 2,` / `)` | a call | 1 | `trailing_comma` |
| one element per line, no commas | an array literal | 0 | |

## Where the compiler does it

- The lexer's rule: `selfhost/layout.hero:40-54`, `is_line_ender` (an
  identifier, the literals, `true`/`false`/`nullptr`, `return`, `break`,
  `continue`, `???`, `?`, `)`, `]`, `}` end a line; every other token does
  not), and `:131-138`, `maybe_terminator`, which plants the terminator only
  after a line ender, at any bracket depth.
- The parser crosses a terminator only where it calls
  `cursor.skip_terminators` (`selfhost/cursor.hero:190`). `grep -rn
  skip_terminators selfhost/grammar_expr.hero` finds 14 calls, in `group`
  (468, 470), `array_literal` (484), `map_literal` (510), `separator` (546),
  `call_args` (597, 604, 641, 646), `block` (709, 725), `match_expr` (1002,
  1018) and `arm` (1071); `parse/members.hero` 7, `parse/tails.hero` 3,
  `parse/decl.hero` 2, `parse/group.hero` 2 (`grep -c`).
- The index is parsed in `postfix` (`selfhost/grammar_expr.hero:265`); its
  branch at lines 287-302 calls `parse_expr` and then `cursor.expect` for
  `.rbracket`, with no `skip_terminators` between (read, `sed -n 287,302p`).
- design.md §4.15 defers Nim's trailing-operator continuation **at depth zero**
  (the same bullet, *"considered and deferred: it enters only if the
  measurement baseline shows models actually produce that break shape"*); it
  says nothing about a break BEFORE an operator inside brackets. `grep -rlniE
  'before an operator|break before|leading operator|leading-operator'
  docs/panel docs/records/log docs/design` finds panel 179's synthesis and
  this brief, nothing else; a ruling in other words is a question.
- The formatter relies on today's behaviour in one place: lane g's fifth round
  keeps the author's parentheses around a comment inside an index *because*
  *"the parser skips a line's end before a group's `)` but not before an
  index's `]`"* (the commit body of `574711c3`, `git show -s 574711c3`).

## The spec's size today

`./heroes measure spec/heroes-spec.md` on the trunk: vendored maximum 6693,
`real` **8861** (the cached reading: *claude-opus-5, 2026-09-25*), ceiling
10240, headroom 1379 of which the FFI floor mortgages 60. The spec is 422
lines (`wc -l`). A new `real` reading needs `measure --refresh`, which needs
the author's key: **the coordinator runs it** on the sitting's final wording
and on today's text; no seat is asked to.

## Rules that bind this sitting

- CLAUDE.md § RUN IT: a number is measured in the session that writes it; a
  negative claim goes out as a question naming what was searched.
- CLAUDE.md § Precedence and § 4: the resolution adopted is the most robust
  and complete one, never the cheapest; where robust and conservative
  disagree, robust is taken and conservative is recorded.
- CLAUDE.md § 12: spec beats compiler, but here the spec contradicts design.md
  as well as the compiler; design.md is the source of truth.
- **Every seat works in its own directory**,
  `/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-180/<seat>/` (on the
  real disk; the session scratchpad is wiped by a reboot). A seat that builds
  copies the frozen trunk there, `git -C /Users/joseph/Temp/heroes/heroes-lang
  archive 29ed5601 | tar -x -C <its directory>`, builds its own compiler
  there (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, a few
  seconds; `-O2` for speed if it rebuilds selfhost), `rm -rf build` after
  runs, and never uses the trunk's `./heroes` or another seat's directory.
  The trunk is frozen until the synthesis.
- **The Mac overheats**: at most 2 concurrent heroes processes per seat.
- Each seat writes its report to `<its directory>/report.md`. English, no em
  dashes. A claim not run is written as unrun.
