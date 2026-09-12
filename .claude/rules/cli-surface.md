---
paths:
  - "selfhost/cli/**"
  - "selfhost/main.hero"
---

# The tool surface

Home of CLAUDE.md § 10 since 2026-09-07. What each rule cost to learn is in
`docs/records/contract/case-law.md`, cited as `CL-NNN`.

## One command

Any new capability is a `heroes` subcommand or flag. **Never a second binary,
never a script, never a Makefile.** The one declared exception, `cargo`, expired
at the fixpoint and is spent (CL-022).

**The stopping rule** (panel 016). A capability enters the surface only if the
fixpoint invocation, the golden harness or the design.md Part 11 harness must
type it, or it has a measured Part 11 effect. Its shape is then mechanical:

- a **subcommand** if it answers a different question, meaning a different
  artifact class;
- a **flag** if it changes how one question is answered about the same input;
- **nothing** if two existing invocations already compose to it.

A new top-level verb needs a *proven overload* of an existing one, which is what
every recorded split was (`git checkout` becoming `switch` and `restore`,
`go get` becoming `go install`), never a new capability.

**The contract of every invocation**: the artifact on stdout, diagnostics on
stderr; exit 0 clean, exit 1 the input has diagnostics, exit 2 the tool could not
run. Inspection is `--dump-<stage>` (design.md §3.5 has the list); `--json` says
*how* to print, never *what*; a mutating flag is `--in-place`; `--emit-c` is an
output, not a dump. One argv table parses the arguments and prints the help, so
the two cannot disagree.

## There is no fourth slot, because there is no fourth input class

The three shapes above all answer *what does this capability do to the inputs the
tool already has*: a `.hero` file, argv, and the machine's environment. A
**per-project file** is none of them, so this rule does not admit one (CL-016).
The refusal is **not permanent**: it is conditional on panel 056's three return
conditions, which are its only amendment path.

`heroes add` and `heroes fetch` are subcommands when they arrive
(design.md:772, and that line number is the one citation shape no instrument can
check, CL-037).

## What this rule refuses today

`--no-line`, because emitter debugging is the test helper's job (panels 016,
020). A second binary of any kind, which is what the retired Nim clause was
reaching for without its false premise (CL-015).
