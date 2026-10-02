# Panel 186, the spec-warden's brief

Read `00-shared.md` in this directory first, whole, and the critic's first
pass (`docs/panel/186-reports/completeness-critic-briefs.md`), above all its
§ 8 on why this sitting is a full panel.

## Your input

The spec's measured size on the sitting's tree, `./heroes measure
spec/heroes-spec.md` on the trunk at 11:08 (`779139d0`, the spec unchanged
since `ae08ed93`):

```
  claude-legacy      6716   (64995 ranks)
  cl100k_base        6838   (100256 ranks)
  maximum            6838   a lower bound, not the reader's tokeniser
  spread              122
  real               9060   claude-opus-5, 2026-09-28 — the binding number
Headroom: 1180 against the 10240 ceiling — but the FFI floor mortgages 60 of
it (panel 030 R3), so what is measured against the ceiling is 9120
```

The sentence behind the question, spec § 13, `spec/heroes-spec.md:354-362`:
*A group's `record` is the header's struct: all its fields, and the same
name unless ...*, then `partial`, *Its size stays C's, not the field
list's*. `grep -n -i "union\|overlap\|anonymous" spec/heroes-spec.md`: no C
union anywhere. The blind seat reads three candidates for one sentence after
*not the field list's* (`docs/panel/186-briefs/blind/brief.md`): L, none; M,
*A field that lies in a C union, named or anonymous, shares its bytes with
the union's other members: a record names every member, reads any of them,
and is built naming exactly one member of each union; comparing it and using
it as a map key are compile errors.*; N, *A C union, named or anonymous, is
bound one member at a time: a record names exactly one member of each union
in the header's struct, and comparing it and using it as a map key are
compile errors.* They are the coordinator's drafts, unpriced.

## Your task

1. In your own copy `<scratchpad>/186-spec-warden/` (`git archive 779139d0 |
   tar -x -C <copy>`, its compiler from the seed), price every candidate
   sentence and every one a route of Q1 to Q4 implies (the bit-field's of
   defect 156 included), each spliced into a copy of the spec, with `heroes
   measure` on the VENDORED tables only: `--refresh` is a paid call and this
   sitting makes none, so the real count is the landing's, and you say what
   the vendored delta predicts for it.
2. Judge, under Principle 0 (CLAUDE.md § 2), whether the spec must say
   anything at all: is § 13's *all its fields* false for a struct holding an
   anonymous union under each route, and does a route that changes what
   `build` refuses without a spec sentence leave the spec contradicting the
   compiler (CLAUDE.md § 12: the spec beats the compiler)?
3. Read the `ffi_union_field` and `ffi_incomplete_record` messages a route
   would print (`selfhost/emit/ffi_record.hero:38-60`, `:169-200`) against
   the spec's words: a message must be true and must use the spec's terms.
4. A verdict per route and per candidate sentence: approve, object or veto
   (your veto is on a budget breach and on a spec that would be false), with
   its token delta, a falsifiable prediction and the condition that would
   change it.

No paid run. At most three processes at once. Write your report into
`docs/panel/186-reports/spec-warden.md` in the trunk as you go.
