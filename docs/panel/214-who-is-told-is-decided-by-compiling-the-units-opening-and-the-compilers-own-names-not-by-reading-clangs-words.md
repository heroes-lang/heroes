# Panel 214: who is told is decided by compiling the unit's opening and by the compiler's own names, not by reading clang's words

**Convened and written 2026-10-14, on the tree frozen at `8c8783ac7`**, by the author's choice at
M-typed-inspection's close (decision issue 2055, item 4). **Full panel**: who is told decides a
diagnostic's class and its exit code (CLAUDE.md § 4). Seats: compiler-engineer, ffi-pragmatist,
spec-warden, historian, llm-ergonomist, and a completeness critic twice. The llm-ergonomist ran as a
subagent, not blind, as at panels 212 and 213 by the author's choice, no paid run; it answered
`context: yes`. **Resolution: provisional, author ratification pending.**
Briefs: `docs/panel/214-briefs/` (shared brief, five seats, the critic, seven label-stripped samples
and their key). Reports: `docs/panel/214-reports/` (the five seats, `critic-briefs.md`,
`critic-reports.md`, the ffi-pragmatist's sources and transcripts in `ffi-pragmatist-work/`; the
compiler-engineer's prototype is its report's appendix).

## The proposal, verbatim

From the shared brief, `docs/panel/214-briefs/00-shared.md` § The sitting:

```
When clang refuses a unit that holds a header a group of the program names, the compiler must say
whose mistake it is: the program's header, told ffi_header_refused at exit 1 on the group's
string, or the compiler's own, told internal error at exit 2. Defect 360 ruled the line: a stack
or note reaching the runtime's or the build's files stays the compiler's. From the night of
2026-10-12 to the evening of 2026-10-13 that line was carved repair after repair, each finding the
next shape (defects 2034, 2036, 2035, 2040, 2044, 2045, 2049, 2051, 2052, 2053). Routes: A shape
by shape; B the first error's own location decides; C the compiler's C names reserved in the
language; D isolation; E a discriminating probe; F the compiler's names in C's reserved space;
G clang's diagnostics read as structure; and, named by the critic's pass 1, H the runtime read
after the groups' headers, I the unit compiled without the group's header, J the header's names
scanned first, K a message that assigns no blame.
```

## What the sitting found before it judged

1. **The briefs were wrong in eleven places** (critic, pass 1), among them route E's premise
   (`cli/header_alone.hero` already includes the runtime and drops every `-W` flag), the cases (55
   `.hero` files, not 125 files), the census (878 programs with an `extern` outside `docs/`, BUILT,
   since `heroes check` exits 0 on every reproducer), and an experiment the ergonomist could pass by
   always answering *my header*. Repaired before any seat ran: a seventh sample where the compiler
   is at fault (defect 2030's shape, an innocent header in the unit), neutral file names, routes H
   to K added.
2. **The night's readers cost 1,669 lines in 21 hours**, and in one more hour the compiler-engineer
   wrote eight header mistakes this tree still tells as `internal error` at exit 2, two of them
   beside readers landed that day (compiler-engineer, W1 and W4).
3. **One probe reaches what the readers reached, by construction** (compiler-engineer, W7): on the
   tree before the chain, `08230cbfa`, compiling the unit's opening with and without the groups'
   include lines tells 18 of its 23 golden cases at exit 1 with +168 lines, where the chain spent
   +1,506; the five left were two emitter-name cases (route C) and three of defect 2052's warning
   flag.
4. **That probe, as first built, blamed a correct header** (critic, pass 2): its fallback compiled
   each group's header alone, and § 13 lets a header need an earlier group's names. Compiled in
   the groups' order instead, the first prefix refused is the one to name (the historian's cgo
   issue 50710, the same lesson).
5. **Reserving the compiler's prefixes in the specification would make working programs wrong**
   (spec-warden): 26 tracked headers declare their own names with them (`hero_mutex`, `h_open`),
   12 of them in `run` programs that build. A blame rule that acts only after clang has refused
   cannot touch them: 29 programs built with the prototype, 0 changed (critic, pass 2).
6. **Isolation does not survive a real header** (ffi-pragmatist): one shared declaration file is
   refused against raylib (`redefinition of 'Vector2'`); the forms clang accepts copy every record
   both ways or lose today's refusal of a field of the wrong type. Renaming into C's reserved space
   makes the emitted C itself undefined (C11 7.1.3p2) and collides with glibc's `h_errno_location`.
7. **A silent wrong value outside the question** (critic, pass 2, on the ergonomist's question): a
   header's `#define fabs my_fabs` takes over the `fabs` another group binds from `<math.h>`, the
   program printing `1000.0` at exit 0. Filed as defect 2056, blocking
   (https://github.com/heroes-lang/heroes/issues/2056).

## Verdicts

| seat | verdict | section | cost or delta | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **veto** A (ceiling) and H (robustness); **approve** O + C, built | design.md §1.1, §1.12 | O + C +296 −11 lines in four `cli/` files, built; the namespace test about 40 lines, estimated | the eight shapes close with those lines and each later shape adds 0 lines to its exit code; under A, at least 400 more | — |
| ffi-pragmatist | **veto** D and F; **object** to H as a move and C as a prefix; **approve** I and G (`.dia`) | design.md §1.11, §4.19, §1.12, §3.1 | D: a copy per call or a lost field check; H: 12 to 29 of 306 group units stop compiling | no D builds raylib and the SDL3 event with no copy and a field check; I's stand-ins compile all 306 group units | one shared record declaration clang holds against raylib.h would lift D's veto |
| spec-warden | **veto** every sentence reserving prefixes (C1 to C3); **object** to B1, E1, F1; **approve** a 0-token ruling | design.md §1.6, §1.2, CLAUDE.md § 2, § 12 | C1 +39 cl100k (46 to 51 real, estimated); the ruling 0 | the ruling lands with cl100k at 7655 and `spec` 31/0; the 12 prefixed `run` cases build at exit 0 | — |
| historian (advisory) | **approve** a general rule; **object** to keeping A | precedent | — | if A stays, the first clang version change on a CI leg moves at least one of the 55 cases | a text-reading layer stable across clang versions for five years |
| llm-ergonomist | **approve** the structured messages over the raw ones | the thesis, as a reader | the right file edited sooner in all six pairs | (the harness's, unrun) | object if a structured message makes a model adopt the runtime's `HeroStr` more often |

## Disagreements, stated plainly

- **Does route C need the specification?** The compiler-engineer wrote that it does; the spec-warden
  vetoed every sentence reserving the prefixes. **The critic's pass 2 settles it**: C as built is a
  blame rule that acts only once clang has refused, so it touches no header that compiles (29
  programs, 0 changed) and needs no sentence. The veto and the adoption stand together at 0 tokens.
- **O or I?** The compiler-engineer built O and refused I on cost; the ffi-pragmatist approved I,
  whose stand-ins it compiled against four real libraries. **They are different questions** (critic,
  pass 2): O cannot see a refusal that needs the program's C; I, if its stand-ins come from the C the
  emitter writes, would carry an emitter defect into both sides. The sitting adopts O, which is
  built and measured, and records I, and the two together, as unrun.
- **Route B** has the historian's support (cgo labels its generated regions and reads locations) and
  the compiler-engineer's objection (dominated by O, blind to the poisoned-`int64_t` shape). The
  critic measured that `#line` already names the compiler's own C (`<heroes library>`, the unit's
  `.c`), so B's misattribution through `#line` is closed for those regions and open for the
  program's own lowered code, which must keep its `.hero` lines for the debugger.

## Resolution (provisional: author ratification pending)

**R1, who is told is no longer decided by clang's words, shape by shape.** Route A is retired as the
rule (the compiler-engineer's veto; the cgo precedent below). Today's readers stay, as **how** a
refusal is told, never as **whether** it is the header's; a new shape owes no reader to get its exit
code right.

**R2, the opening decides (route O), compiled in the groups' order.** Where the readers would print
`internal error`, the compiler compiles the unit's opening, its own headers, the groups' headers and
the C library's headers it reads, with none of the program's C, under the unit's own flags; then the
same text without the groups' include lines. Refused with them and compiled without them, the
headers are what clang refused, told at exit 1 on the group whose header the opening refuses first
when the groups' include lines are added one by one in their order (the critic's repair of the
prototype's fallback). Asked last, so no message measured today moves (the census: 878 programs, 0
moved, exit and message).

**R3, the compiler's own names (route C), a blame rule and no sentence.** A first error in the
program's C at a name of the compiler's forms that no group binds (`h_`, `hero_`, `Hero`, `HERO_`,
`HEROES_`, `main`, and the emitter's `f_` and `t<N>` where a header's macro meets them, the critic's
addition), whose note stands in a group's header at that same name, is the header's, both
declarations named. The specification stays silent (the spec-warden's veto); the rule's home is
`.claude/rules/c-boundary.md`, the message's design.md §4.17.

**R4, the runtime's names held to those forms by a test** (the compiler-engineer's NC1, which every
route shares): the unit's opening with the C11 and POSIX headers in a group's place compiles clean
on the three platforms (the critic ran 67 headers on this Mac and Linux arm64, 0 warnings), so a
runtime defect that collides with C's own names fails the net instead of being told as an author's
header.

**R5, the compiler's files for blame** include `<heroes library>` and the unit's own `.c`, which
`#line` already names (critic, pass 2); a location in the program's lowered code is the compiler's
unless R3 makes it the header's.

**R6, the routes refused**: D and F (the ffi-pragmatist's vetoes), H (the compiler-engineer's veto:
18 of 220 headers that compile today would not), the prefixes in the specification (the
spec-warden's veto). Not adopted, recorded: I and O with I together (unrun as a build), G (`.dia`
nests every note under its error on clang 21 to 23, the ffi-pragmatist; a reader of it is not
built), J and K (they shape a message, not its party).

**R7, Windows**: the opening is compiled by the platform's own clang, so the probe decides alike
where clang's words differ (defect 2053's cases stay judged where their words are).

**R8, the open issues**: 2047 (absolute paths in notes) and 2054 (the `-Werror` note's list) stay
`adjacent`; 2046 (the specification's silence on the compiler's names) is answered by R3's silence
and stays open as the author's question below; 2056 (a header's macro taking over another group's
binding) is blocking and is repaired in the landing, apart from the rule.

**The question to the author** (critic, pass 2): is a header that is correct C, refused only because
of names the compiler put in its unit, a wrong program? The specification does not say the runtime
shares the header's unit. **Recommendation: keep the specification silent** and tell the collision as
what it is, both declarations named, the rename a `guess`: no collision was found outside the shapes
agents planted, and a sentence would spend 40 to 50 of the 132 real tokens left.

### What the conservative resolution would have been, so the author can choose it

Route A kept as the rule, each new shape a reader, plus R4's namespace test, which closes the one
false blame every route shares. The sitting takes R1 to R3 because the readers grew 1,669 lines in a
day and still left eight shapes at exit 2, and because one probe reached the same exit codes for a
tenth of the code.

### What a veto would compel

The compiler-engineer's vetoes compel that no new reader is written to fix a shape's exit code, and
that the runtime is not moved after the headers. The ffi-pragmatist's compel that no binding is
isolated in a unit apart and that no emitted name moves into C's reserved space. The spec-warden's
compels that the specification reserves no prefix.

## Prior art

Panel 214 (`docs/panel/214-who-is-told-is-decided-by-compiling-the-units-opening-and-the-compilers-own-names-not-by-reading-clangs-words.md`,
`docs/panel/214-reports/historian.md`), every link re-read 2026-10-14. The sitting's decision issue
carries the same lines.

- **Taken from Go**: [cmd/cgo: do not look at error message text](https://github.com/golang/go/issues/6596): opened on 2013-10-16; cgo looked at the error message text from a trial compilation to deduce what a C name refers to, and the issue asks it to stop, which "will make cgo much less dependent on the compiler details".
- **Taken from Go**: [cmd/go: reports errors for a correct preamble when it contains a builtin C type and a C.* name can't be resolved](https://github.com/golang/go/issues/50710): opened on 2022-01-20; a type defined in cgo's builtin prolog was not added to the preamble when checking it, so a perfectly fine preamble was reported as an error.

## Predictions to score

At the landing's batch gate:

- **compiler-engineer**: the eight shapes it wrote close with O + C's lines, and each later shape adds
  0 lines to its exit code (`git diff --numstat` over `selfhost/cli/`).
- **ffi-pragmatist**: no implementation of D is attempted; I's stand-ins are not built, so its
  prediction about the 306 group units is not scorable unless I is.
- **spec-warden**: cl100k stays 7655 and `spec` reads 31 passed, 0 failed; the 12 prefixed `run` cases
  build at exit 0.
- **historian**: scorable only if A had stayed; it does not, so the prediction lapses unless a clang
  version change moves a case anyway.
- **llm-ergonomist**: the harness's, unrun.
- **the sitting's own**: the census of the 878 programs reads 0 moved for the worse, exit and message,
  and the correct header behind another group's typedef (the critic's `a.h`, `b.h`, `c.h`) is told as
  `c.h`'s.

## Process notes

- **The critic's first pass found eleven errors in the briefs, the second a false blame in the adopted
  prototype**, a wrong value outside the question, and the question the sitting had not asked.
- **Two seats and the critic wrote helper files to the session scratchpad**, outside the repository's
  root, and deleted them; the briefs said not to.
- **Two of the ffi-pragmatist's transcripts, `route-g/e_std.win.sarif` and `route-g/hs.win.sarif`,
  carry Windows' carriage returns**, which `unseen` refuses in a document; the coordinator moved them,
  bytes unchanged, into `214-reports/ffi-pragmatist-work-windows.tar.xz` before the sitting was
  committed (`tar -xJf` in `214-reports/` puts them back).

## Author's verdict

Pending: https://github.com/heroes-lang/heroes/issues/2057 asks for it, naming the sitting as `panel 214`.
A yes settles R1 to R8 as written and the specification's silence the way the author answers the
question above; it does not settle routes I or G, which stay recorded and unbuilt.
