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

**The two sentences above pull apart for a new capability with an artifact
class of its own** (panel 179's completeness critic): the first makes it a
subcommand, the second admits no new verb that is not an overload. Panel 179
read them this way for `heroes probe`: the capability entered by the author's
decision of 2026-09-26, which the stopping rule was not stretched to cover, and
the sitting ruled its shape by the first sentence, since its artifact is a
verdict over generated programs and its operand a directory, which `fmt`'s row
cannot carry. The second sentence governs a verb split out of an existing one.
**`heroes new` entered the same way, by the author's decision of 2026-11-10**
(issue 2441, meant as: *a command that creates a project's stub, as cargo
does*), and panel 244's R2 ruled its shape by the first sentence: its artifact
is a new project's folder and its operand a name, so a subcommand, and
`--package` a flag, since it changes how one question is answered.

**The contract of every invocation**: the artifact on stdout, diagnostics on
stderr; exit 0 clean, exit 1 the input has diagnostics, exit 2 the tool could not
run. Inspection is `--dump-<stage>` (design.md §3.5 has the list); `--json` says
*how* to print, never *what*; a mutating flag is `--in-place`; `--emit-c` is an
output, not a dump. One argv table parses the arguments and prints the help, so
the two cannot disagree.

## Four input classes: the catalogue is the fourth, and there is no fifth

The shapes above answer *what does this capability do to the inputs the tool
already has*: a `.hero` file, argv, the machine's environment, and **one
per-project file, the program's catalogue**, `heroes.toml` beside the file
compiled, holding the dependencies a `use <name>/…` reaches and the address of
the index the verbs read, and nothing else (spec § 1). It is admitted by the
author's call (panel 235, 2026-11-07, recorded `retro-record`, ratified the same
day), not by panel 056's three return conditions: its entries repeat the names
the `use` lines write, so the second condition's letter cannot hold, and what is
kept is that condition's purpose, as the falsifier below. **A program with no
`use <name>/…` has no catalogue and reads none.** The catalogue holds, per
entry, its source, its exact version or tag and its fingerprint; a compiler
flag, a search path, a library or package of spec § 13, a language version, a
mode or the program's own name is refused by the reader, never ignored.

**The falsifier**: deleting the catalogue, deleting one of its entries, or
mistyping any field of one never changes which module compiles, only whether
the program builds, and a fetched copy left behind after its entry is gone
compiles nothing; a `path` entry, a local folder while a package is developed,
is the one exemption, and every build that uses one says so (panel 235's R6).
The `check` golden form holds one case for each; one program that compiles
another module after such an edit returns the admission to a sitting.

**A fifth input class is refused** on panel 056's three conditions, its only
amendment path: a second per-project file beside the catalogue, a lock, a
workspace or a configuration, is a fifth class.

`heroes add` and `heroes fetch` are subcommands when they arrive (design.md
§3.5, cited by its section, CL-037).

## What this rule refuses today

`--no-line`, because emitter debugging is the test helper's job (panels 016,
020). A second binary of any kind, which is what the retired Nim clause was
reaching for without its false premise (CL-015).
