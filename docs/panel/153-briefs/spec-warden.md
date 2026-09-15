# Panel 153 — brief for the spec-warden

Read `docs/panel/153-briefs/00-shared.md` first. Your report goes to
`docs/panel/153-reports/spec-warden.md` — write it in your COPY and hand the
text back in your final message; the coordinator writes the file.

## What you judge

The indicator (design.md §1.2 cost formula, §1.6 spec budget) and Principle 0's
burden of proof. You have a veto on a budget breach.

## The measured base, taken 2026-09-15 before this brief

| | |
|---|---|
| `spec/heroes-spec.md`, real (`claude-opus-5`) | **7974** |
| vendored maximum (`cl100k_base`) | **5989** |
| digest | `2e77c4e72ce69512` |
| ceiling | **10240** (`selfhost/measure/judged.hero`, design.md §1.6) |
| free | **2266**, 2206 net of `FFI_FLOOR` 60 |
| `DELTA_GATE`, one commit's growth in vendored tokens | 50 |

**The −3 real removal panel 152 named is spent**: *what the header leaves
opaque* left § 13 today in defect 037's ledger row
(`docs/measurements/010-spec-budget-ledger.md`, row 5989). Do not allocate it
again.

## The instrument, and how to take the real figure

`heroes measure <draft>` counts any file offline on the vendored ranks. **The
real figure is taken by `heroes measure spec/heroes-spec.md --refresh` on the
spec path**, so in your COPY write each draft to that path and refresh there:

```sh
cd <your copy> && set -a && . ./.env && set +a && ./heroes measure spec/heroes-spec.md --refresh
```

Your copy's pinned record will be stale after the first refresh; the tool says
so and still prints the count. Take **at least** these drafts, each alone
against the base, and say which line of § 13 each sentence joins:

- **Q2**: the fence gains `acquires sqlite3_close` and `consumes`. Panel 152
  priced it at +7 vendored on the 5988 base; take the real figure.
- **Q1, Route A**: a sentence in § 13 saying a handle may list fields, read
  through the pointer, a field of the handle's own type walking a list, a null
  read aborting; and whatever the marker word costs in the `Member` production
  at the end of § 13 (`"record" ident [ "tag" ident ] [ "partial" ] ( Fields |
  NEWLINE )`).
- **Q1, Route B**: the widening of *two records may not name one tag*, the
  `read` name in § 11's `Built-ins:` sentence — **which `tests/harness/spec_text.hero`
  reads, so it must stay one sentence ending at `range`** — and the prose for
  it.
- **The `hints` question**: whether `@` on an in-parameter needs a sentence, or
  whether *A C out-parameter is an `@` parameter* already covers it.

Merging beats appending (panel 122, 2.7×): price each as a merge into a
sentence already there where one exists.

## What pays

design.md §1.6: an addition owes a **named removal** or a **registered
prediction naming an instrument that exists today** and the milestone at
which it is scored. For Q2 the llm-ergonomist's blind task is an instrument —
it measures how many models omit the marks reading the current fence versus the
corrected one — and its result is in that seat's report by the time you write
yours. For Q1 name a removal if one exists in § 13 after today's edits, and say
plainly if none does.

## What your report must carry

Every draft's real and vendored figure with the delta against the base; the
payment for each; whether any package crosses `DELTA_GATE` in one commit and
must therefore say in its body what paid; your verdict on Q1's two routes and
on Q2; **one prediction with the command that scores it**, and the number it
predicts, with its unit in the same sentence. Run it, or say it is unrun.
