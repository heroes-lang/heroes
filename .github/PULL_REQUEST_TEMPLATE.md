<!--
  Read `CONTRIBUTING.md` first. A pull request here comes from an authorized
  contributor and follows an issue the maintainer accepted; one from anybody
  else, or with no issue behind it, is closed with a link to that file, and the
  defect or the idea inside it is reopened as an issue and answered there, so
  nothing is lost. The list below is what the maintainer checks before merging,
  written out so you can check it first.
-->

**The issue this answers:** Closes #

**What it changes, in one line:**

**Checked before opening**

- [ ] the maintainer has authorized me to send code (`CONTRIBUTING.md` § Who may send code)
- [ ] the issue above is accepted, and this is the change it agreed, nothing beside it
- [ ] every `.hero` file touched is formatted: `heroes fmt <file> --in-place`
- [ ] the cases are in: a golden case per shape, each diagnostic annotated in its source and snapshotted
- [ ] the three suites are green on my machine, with the commands `README.md` gives
- [ ] `seed/heroes.c` is untouched
- [ ] no new binary, script or Makefile; a new capability is a `heroes` subcommand or flag
- [ ] everything is in English
