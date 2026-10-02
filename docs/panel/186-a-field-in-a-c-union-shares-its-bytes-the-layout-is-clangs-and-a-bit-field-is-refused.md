# Panel 186: a field in a C union shares its bytes, the layout is clang's, and a bit-field is refused

2026-10-02, sat from 11:08 to 14:00 on the trunk frozen at `779139d0`, a full
panel: `compiler-engineer`, `ffi-pragmatist`, `spec-warden`, `historian`,
`llm-ergonomist` (two blind sessions, run outside the repository), and the
completeness critic over the briefs first and over the reports after. The
briefs are `docs/panel/186-briefs/`, the reports `docs/panel/186-reports/`.
The brief first proposed the soundness lane; the critic's first pass showed
a spec sentence behind the question (§ 13's *all its fields*), and the
sitting became a full panel before any seat was launched
(`186-briefs/00-shared-before-the-critic.md` is the brief as it stood).

## The proposal

Three defects at one boundary, a group's `record` against its C header:

- **151**: a struct holding an anonymous union, `SA { int32 kind; union {
  int32 i; float f; }; int32 x; }`, bound as `kind`, `i`, `f`, `x`:
  `SA(kind: 1, i: 7, f: 0.5, x: 3).i` prints `1056964608` at exit 0, `==` is
  accepted, a field left out is unreported, and `S3 {a, b, c}` bound by `a`
  and `c` is told *does not name `c`*; widened by the critic to ONE declared
  field (`SB` bound by `kind` and `b`, whose `==` prints `true` for two values
  that differ).
- **150**: a correct program reading a C union naming two members gets
  clang's *excess elements* warning at its own line.
- **156** (filed at 11:05 from the critic's first pass): a bit-field member
  stops `build` at exit 2 with clang's text.

The questions (`186-briefs/00-shared.md`): Q1, how `build` learns which
declared fields lie in a union at any depth, routes (1a) to (1h); Q2, the
completeness probe's form; Q3, where the routes meet and their cost; Q4,
what a record over such a struct is for the author, the spec and the
message.

## The verdict table

| seat | verdict | cost or delta | prediction | condition |
|---|---|---|---|---|
| compiler-engineer | (1e) via `-fdump-record-layouts` behind a C-judged screen **approve, built**; (1e) via the JSON dump approve as an unbuilt alternative; (1a), (1b), (1c), (1f), (1g) object; (1d) veto (§1.7); (1h) object, filed; Q2's layout form approve, (2a) object | +723 lines in the layout unit, four new modules, none over 300, no lexer, parser, checker or IR change; one extra clang process per cold build with a group record | every new module at or under 300 lines; the census moves only the sitting's probes | the dump's text measured different on a clang outside its control record |
| ffi-pragmatist | (1e) via the JSON dump approve; (1a), (1b) approve for the records a program builds or compares; (1h) approve; an adjacency probe approve; (1c), (1g) as a blanket rule, and the any-arity `==` rule as written **veto** (`glob_one.hero`, `in6.hero`, the `Color` golden); (1d), (1e) via the layout dump, (1f) object | the C of every route on five clangs and three targets | with (1e) JSON and (1h), an SDL3 event loop reading `e.key.scancode` builds at exit 0 with no shim, and `in6.hero` stays at exit 0 | the JSON dump's shape differs on any clang |
| spec-warden | L (no sentence) **veto** (the spec false for `SA`); M +58, N +45 object; **O_final +61 approve with (1e)**; B1_merged +15 and B3_abort +39 approve; (1d) veto | vendored, the real delta the landing's | O_final's real delta in +71 to +87 | void if (1e) cannot see `SB`'s single member or `SD`'s struct |
| historian | (1e) JSON, (1h), (1g) as the floor approve; (1a), (1b), (1c), (1f) object; (1d) object (mild) | | every binder that lasted asked the C compiler for layout | (1g) reverses on a tracked program comparing two group records |
| llm-ergonomist, reading 1 (11:09, 0.48 USD) | L **veto** (meaning depends on an unstated write order), M approve, N object | | under L at least 30% silently wrong | |
| llm-ergonomist, reading 2 (13:57, 0.38 USD) | today's text **veto**; O_cover (with (1h)) approve; **R_build_cover (the adopted text) object**: correct and loud, but the natural program needs a bit-pattern trick | | under R_build_cover at most 5% silently wrong and at most 15% one-turn success; under O_cover at most 5% and 25% | C to approve if (1h)'s *built naming exactly one* produced more silent errors than the refusal |

## What the sitting measured

- **The coordinator built the first blind reading's program on today's
  compiler** (`186-reports/coordinator-blind-program-built.md`): it prints
  the meant `1.5` only because the header declares `f` after `i`, the value
  given to `i` is discarded in silence, and over the same struct with the
  union's members in the other order the same program prints `0.0` at exit
  0. The first reading's veto on L, measured.
- **The causes**: the union detector asks C whether the TYPE is a union
  (`__builtin_classify_type`, panel 077), and a struct holding one answers
  struct; panel 073's sum-of-sizes form had caught `SA` (`073...:75-76`) and
  panel 077 replaced it the same day without the shape. The completeness
  probe writes positional zeros per declared field, so it miscounts an
  anonymous member.
- **No clang this project meets reports a field a designated initializer
  leaves out in C**: Apple 21, Homebrew 22.1.8 (the seats), Ubuntu 18.1.3,
  Debian 18.1.8 and 20.1.8 (on Linux and both Windows targets) and the
  Windows box's 23.1.1 (the coordinator,
  `186-reports/coordinator-platform-readings.md`); clang's PR #81364 keeps
  that check off for C on purpose (the historian). Route (2a) cannot exist.
- **The built route on five clangs**: the compiler-engineer's screen, dump,
  check and control files read identically on Ubuntu 18.1.3 (the CI's own),
  Debian 18.1.8 and 20.1.8, Apple 21 and Homebrew 22.1.8, once the
  parenthesised type spans its reader never reads are masked; the Windows
  box (23.1.1) is unrun for them, having stopped answering at 13:16.
- **The built route's census**, 447 files: 397 identical on exit, stdout and
  stderr; every move under this sitting's probes; files printing an
  initializer warning 15 to 0; the thousand-deep case builds as before. It
  refused libc's real `struct sigaction` in its first form, which the
  ffi-pragmatist's report showed, and was repaired: a field the dump does
  not show is asked by name under `#ifdef`.
- **The cost the critic found in it**: a FLAGGED record over a deep by-value
  nest brings the large dump back, 348 MB at depth 1000 and 10.4 MB at depth
  300 (a synthetic header).
- **The `==` rule's soundness** (the critic, on today's compiler): a field
  that covers its union by SIZE is not enough, since a padded struct arm and
  an `f32` arm (`-0.0` against `0`) each compare two different C values as
  `true`; a field that covers the union and compares bit for bit (an
  integer, a pointer, an array of them) is.
- **Real headers** (the ffi-pragmatist): anonymous unions in `glob_t`,
  `mach_port_options`, `processor_basic_info`; union members reached through
  macros in `struct sigaction` and `struct in6_addr`; bit-fields in curl's
  `curl_hstsentry`, `tcphdr` and eleven mach descriptors; SDL3's
  `SDL_Event`. No SDL3 key event is readable today without hand-written C.

## Disagreements, unsmoothed

- **Which source for (1e).** The compiler-engineer built the layout dump
  behind a screen and a control record; the ffi-pragmatist objects to that
  source (two texts across clangs, a partial print under `-fsyntax-only`,
  blind to macro-reached members) and approves the JSON dump, unbuilt. The
  engineer answered the three: its reader never reads the span the texts
  differ in, every wrapper forces its layout and a missing block is exit 2,
  and the macro blindness was real and is repaired. The critic measured two
  parts of the JSON route failing: its adjacency probe refuses
  `mach_port_options_t` bound by one arm, which builds and prints `13`
  today, and the member-access dump does not hold the union's other arm.
- **(1h).** Approved by the ffi-pragmatist, the historian, the spec-warden
  (inside O_final) and the second blind reading; objected to by the
  compiler-engineer, because `missing_fields` is a `check` refusal and the
  checker knows nothing of unions. Built by nobody.
- **The `==` rule.** The engineer built two rules (any arity for an anonymous
  union, *fills its union* for a macro-reached field in a named one); the
  ffi-pragmatist vetoes the any-arity rule over `in6.hero`; the critic
  measured size-covering unsound and named bit-for-bit covering.
- **The adopted sentence was objected to by the reader who read it**:
  reading 2 approved O_cover, which needs (1h), over R_build_cover.

## The resolution: `provisional, author ratification pending`

The most robust and complete one that builds (CLAUDE.md § 4; a route that
does not build is not adopted, `.claude/skills/panel/SKILL.md` § 3c); what
conservative would have been is written beside each.

**R1. (1e): the header's layout is read from clang, as the compiler-engineer
built it.** A screen first, one never-called C function per record under
three warnings armed as errors, so clang vouches for every record that names
exactly its struct's members; the layout dump only for the records the screen
flags, its text supplying names and never a verdict; every verdict a
`_Static_assert` on the header's own offsets and sizes or
`__builtin_classify_type`; a control record whose dump must come back as
written, or exit 2 naming the dump; a field the dump does not show asked by
name under `#ifdef`; the cache keeping clang's answers only. The landing
**bounds the dump for a flagged record over a deep nest** (the critic's 348
MB at depth 1000) with a case at that depth, and runs the Windows box on the
route before it is called done. *Not adopted*: the JSON route, unbuilt and
measured failing on a real header; recorded as the alternative.

**R2. Construction**: a record naming two fields that share bytes, an
anonymous struct's fields counting as one, is refused at `build`, panel
073's rule now reaching a struct (built: `sig_both` refused, *`sa_handler`
and `sa_sigaction` are the same bytes*). Reading is untouched.

**R3. Comparison, one rule for a struct-held union and a union type**:
`==`, `hash` and a map key are compile errors for a record holding a field
that lies in a union, unless that field is an integer, a pointer or an array
of them as wide as the union (the critic's bit-for-bit cover). It keeps
`in6.hero` and `sig_eq` legal and refuses `SB`, the padded arm and the
`f32` arm. **The landing builds this narrowing in place of the seat's two
rules** and runs the critic's cases (unbuilt in the sitting). It changes
panel 077's ratified any-arity rule for a union TYPE: the two goldens that
hold it, `tests/golden/unsupported/fixedbugs-140-a-union-compared-sixteen-deep-is-refused`
(`U { int32 i; float f; }` by `i`) and `...-a-union-in-a-variant-case-is-refused`
(`W { int32 i; uint32 n; }` by `i`), name an integer as wide as the union and
become legal comparisons, rewritten by hand. *Conservative*: panel 077's
any-arity refusal kept for a union type, the cover rule for a struct-held
union only; two rules that can disagree.

**R4. Completeness (Q2), the layout form**: a member a record leaves out is
complete only where C says it has no bytes, shares a byte with a declared
field, or sits in a union; it replaces the positional probe, so defect 150's
warning leaves the author's line, and u19 names `b`. A misspelt field is told
first, as `ffi_unknown_field` on its own line (lane ffi-macro's `6559facf`,
whose five goldens keep their message), completeness after it: the landing
orders the two and its census shows it.

**R5. Bit-fields (defect 156)**: refused at `build`, exit 1 on the field's
line, `partial` being how a record binds the rest (built). *Robust
completion, recorded*: a bit-field bound as a field, the ffi-pragmatist's
run-time fit check (`((T){.f = v}).f == v`, measured in C on two clangs,
unbuilt) and the warden's B3_abort sentence; the next question at this
boundary.

**R6. Spec § 13** says it: R_build_cover's text in place of `partial`'s
three lines (+88 vendored, the critic's price) with the warden's B1_merged
clause (+15), priced together in the landing's tree with one `--refresh`.
§ 13's *all its fields* stops being false for `SA`. Reading 2 objected to
this text and found it loud, never silent; it approved O_cover, which is
R7.

**R7. (1h), the construction-arity form, is the robust completion and the
author's question**: a record over a union names one or more members and is
built naming exactly one, an anonymous struct's fields counting as one. It
is the one route that lets an `SA` be built holding `f` while `i` is read,
and an SDL3 event be read with no hand-written C. It needs a home for the
refusal of an omitted field, today `check`'s: **(a)** `check` stops
refusing an omitted union sibling in a group record and `build` judges it
(the refusal moves later for that one class); **(b)** a marker on the record
that `check` reads and `build` verifies (a surface form, a sitting's); **(c)**
`check` asks clang (panel 077 refused it). Recommended to the author: (a),
because it adds no surface and `build` already judges every group record
against its header; if (a), the landing prototypes it in a scratch copy and
runs reading 2's task before O_cover replaces R_build_cover.

**R8. Not adopted**: (1a), (1b) and (1f) alone (blind to `SB`), (1c) under
the ffi-pragmatist's veto (`glob_t`, u08), (1d) under the
compiler-engineer's and the warden's (§1.7, and the lexer refuses `union`),
(1g) as a blanket rule under the ffi-pragmatist's veto, its own historian's
reversal condition met (`tests/golden/run/fixedbugs-a-group-record-in-every-container.hero`
compares and keys raylib's `Color`).

**R9. Filed beside the sitting**: defect 157, `ffi_tag_is_a_union`'s note
(*a group's `record` is the header's STRUCT ... A union has no `record`
spelling*) is false (`read.hero` binds a union as `record W`, exit 0) and
against panel 077's ratified item 4, with no golden holding it; and panel
073's *one record per arm by `tag`* escape is refused on the tree, so Q4's
escape for a tagged union is unsettled with it.

**R10. Where it is written**: spec § 13 (R6); design.md §4.19 (the union
rule's one predicate, the layout's source); `docs/work/DEFECTS.md`: 150, 151
and 156 close with the landing, a lane at the C boundary, so after the
push's Linux arm64 and Windows legs (the author's instruction of
2026-10-02); 157 filed.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | every new module at or under 300 lines; the census moves only the sitting's probes | the landing |
| ffi-pragmatist | `in6.hero` stays at exit 0; with (1h), an SDL3 loop reading `e.key.scancode` builds with no shim | the landing; (1h)'s |
| spec-warden, by the critic's calibration | R_build_cover's real delta in +102 to +125 | the landing's `--refresh` |
| llm-ergonomist, reading 2 | under R_build_cover at most 5% silently wrong; under O_cover at most 5% silent and at most 25% one-turn success | a generation run, unrun |

## Author's verdict

Pending: `docs/work/DECIDE.md`'s item `panel 186`, with R7's three
mechanisms and the recommendation.

## The critic's two passes

`186-reports/completeness-critic-briefs.md` (the briefs, before any seat:
eleven repairs, every one taken, among them the one-arm `SB`, the route the
defect itself named, the CI's two clangs, (2a)'s missing warning, the
sentence misattributed to panel 073, panel 077's regression, the absent
escape for `SA`, the full panel, and defect 156) and
`186-reports/completeness-critic.md` (the reports: which (1e), the `==`
rule's soundness, (1h)'s home, O_final false under the built route, lane
ffi-macro's interaction, the `tag` refusal, (1g)'s own condition, and the
second blind reading it asked for, which the coordinator ran). It changed
this resolution in R1, R3, R4, R6, R7 and R9.
