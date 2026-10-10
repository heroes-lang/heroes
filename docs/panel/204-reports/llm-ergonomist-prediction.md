# Panel 204, llm-ergonomist (blind seat): the coordinator's prediction before the readings

Written by the coordinator at 01:31 on 2026-10-10 (`date`), before any of the
sessions starts. Folders `<scratchpad>/readings-204/<label>`, outside the
repository and outside any git tree (the author's exception of 2026-10-09;
the budget the author's answer of about 01:13, *10 dollars*). The spec in
every folder is the trunk's at `635e8f67`. The labels' mapping, never in a
folder:

- **h1-a, h1-b**: predict `build` and the run of four programs: `limit_ab`
  and `limit_ba` (panel 204's `cfgone`, today `3 50` and `3 10`, the second
  with clang's `-Wmacro-redefined`), `jpeg_ab` and `jpeg_ba` (today 1, and
  refused *unknown type name 'FILE'*).
- **h2-a, h2-b**: repair `jpeg_ba` given today's message (its last note
  *repair the header, or name the one that declares this group's C*).
- **h3-a, h3-b**: repair the critic's `rec` (a `stdio.h` group holding only
  `record CFile tag FILE`, written above `jpeglib.h`), refused with the same
  message although `stdio.h` is written first.

**What I expect.**

1. h1: at least one reader predicts `limit_ab` and `limit_ba` print the same
   (§ 4's *Declaration order never matters*), and at least one predicts
   `jpeg_ba` builds for the same reason.
2. h2: both move the `stdio.h` group above the `jpeglib.h` one; at least one
   names the message's advice as unhelpful in its `choice_points`.
3. h3: neither names the emission order as the cause; at least one adds a
   function to the `stdio.h` group, and at least one writes a header of its
   own or adds another group.
4. Every `context` names only the folder and the harness's environment.

**What it would falsify**: in h1, both readers predicting `3 10` for
`limit_ba` and a refusal for `jpeg_ba`; in h3, a reader naming the emission
order.

## The second round, written at 01:51 on 2026-10-10 (`date`), before its sessions

On the spec-warden's drafts (`docs/panel/204-reports/spec-warden.md`): **F7**
replaces § 4's sentence with *Declaration order never changes what a program
means*; **G1m** adds to § 13 *C reads a module's headers in the order its
groups are written, so one that needs another's names comes after it*.

- **j1-a, j1-b** (F7) and **j2-a, j2-b** (F7 and G1m): the first round's four
  predictions. The landed route's answer: `limit_ab` and `limit_ba` refused
  (the detector), `jpeg_ab` 1, `jpeg_ba` refused with a note naming the
  `stdio.h` group to move.
- **k1-a to k1-d** (F7) and **k2-a to k2-d** (F7 and G1m): write a program
  binding `fopen`, `fclose` and `jpeg_stdio_src` (the spec-warden's P3).

**What I expect**: j1, both still predict per-group isolation, so `jpeg_ab`
refused; j2, both predict `jpeg_ab` builds and `jpeg_ba` refused, and at
least one predicts the `limit` pair refused or names the contradiction
between F7 and G1m. k: the spec-warden's P3, at least 3 of 4 in k2 write the
`stdio.h` group above, at most 2 of 4 in k1; if both reach 3, G1m comes out.
