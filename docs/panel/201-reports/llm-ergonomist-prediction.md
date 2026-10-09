# Panel 201, llm-ergonomist (blind seat): the coordinator's prediction before the readings

Written by the coordinator before any session of this sitting starts, on
2026-10-09 (the clock read just before the runs is in
`llm-ergonomist-scoring.md`), so the readings score a prediction and not a
story. Folders `<scratchpad>/readings-201/<label>`, outside the repository and
outside any git tree (the author's exception of 2026-10-09; the budget of 6
USD the author's yes of about 19:40), each holding `brief.md`, `spec.md` and
the program; the inputs are copied into `docs/panel/201-briefs/blind/`. The
labels' mapping, never in a folder:

- **r1-a1, r1-a2**: Q1 (488), *would the checker accept `main.hero`?*,
  `app(f: ident, x: 20)` and `ns.map(ident)`, the spec as it stands
  (SHA-256 `75407a13724c3758`). Today's checker refuses both lines
  `cannot_infer` (run by the coordinator).
- **r1-b1, r1-b2**: the same, the spec with the narrowing sentence after
  § 9's inference bullet, the critic's draft: *A generic function used as a
  value takes its types only from a non-generic parameter, a record field or
  a declared return; handed to a generic parameter it is an error.*
  (`bb7e9b9f13fab561`).
- **r2-a1, r2-b1**: Q2 (467), *`heroes build main.hero` fails today; make
  this program compile and do what it evidently means*, a module `geom.hero`
  with no `main` and a `main.hero` calling a missing `geom.area`; spec as it
  stands (a) or line 22 reworded (b: *One file is one module. Only the file you
  compile holds `function main()`, which takes nothing and produces nothing; a
  module it uses holds none.*, `80f365e3dd30c8f8`).
- **r2-a2, r2-b2**: the same with the task *`main.hero` uses `geom.area`,
  which `geom.hero` does not have yet; add it*, which does not say *compile*.

**What I expect.**

1. Q1: at least one of the two (a) readers predicts *accepted* on line 11 or
   12, quoting *from the type the context asks for*; both (b) readers predict
   *refused* on both lines and quote the added sentence.
2. Q2: no reader adds a `main` to `geom.hero` in any cell (the measured 2 of
   12 came under *compile*, with a module given alone; here `main.hero`
   stands beside it); if one does, it is in r2-a1.
3. Every `context` names only the folder and the harness's environment.

**What it would falsify**: a (b) reader predicting *accepted*; both (a)
readers predicting *refused* with the narrowing stated (the spec as it stands
already read as the compiler reads it, which is defect 402's reading); a
`main` added to `geom.hero` under (b).

**What it cannot carry**, said before: two readings per arm cannot tell 2 in
12 from 0 (the critic's anchor); Q2 measures a module given beside its
program, never a module alone, so a null result is not a refutation of
measurement 040.

## Q3, added at 20:42 (`date`) before its two sessions start

- **r3-n**: Q3 (520), `ping` and `pong` returning each other's call with no
  way out, the task *make the program build and do what it evidently means*,
  given the compiler-engineer's prototype refusal at build
  (`error[endless_recursion]: ``ping`` and ``pong`` call each other for
  ever`, its two notes); **r3-m**: the same program given today's tools' answer,
  a build at exit 0 and a run at exit 134, *panic: stack exhausted in
  main.pong, inside the recursion of main.ping and main.pong* (defect 521's
  message). The spec as it stands in both.
- **What I expect**: both readers give one of the two functions a base case
  (a path returning before its call) and keep the calls; neither deletes a
  call or adds an `exit`. The r3-n reader names the note's way out.
- **What it would falsify**: a reader who removes a call, or keeps both
  functions without a way out.

## N1f, added before its two sessions start (the critic's second pass)

The critic's second pass found that the narrowing sentence the (b) readers
read, its own first-pass draft, is false (an annotation and `push` also type
a generic value, the spec-warden's probes), so no candidate that could land
had a measured reader effect. **r1-n1, r1-n2**: Q1's task with the
spec-warden's N1f in § 9's bullet (*..., else from the type the context asks
for, and a generic function's parameter asks for none; a call that says
neither is an error.*). **What I expect**: both predict *refused* on lines 11
and 12 and quote N1f's clause (the spec-warden's P3). **Falsified by**: a
reader predicting *accepted*.
