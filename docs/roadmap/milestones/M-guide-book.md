# M-guide-book — the guide


**Scheduled**, and it is the one milestone after the fixpoint that has a warrant.
The classic language guide, the K&R shape: read it front to back and you can
write Heroes; open it in the middle and you find the thing you were looking for.

- **Organised by subject, not by chronology**: values and types, bindings and
  `@`, control flow, records and variants, `T?` and failure, generics, the
  module, the FFI, the test blocks — each with the smallest program that shows it
  and one that gets it wrong on purpose.
- **It is not the spec, and it must never try to be.** `spec/heroes-spec.md` is
  the control instrument, budgeted at 4096 tokens precisely so that it can never
  become a teaching text; the guide is where the explanations, the worked
  examples and the "why it is like this" live, at whatever length clarity needs.
- **Its warrant is §1.1**: comprehension is this project's objective, measured
  rather than asserted, and a guide the author can read to rebuild the reasoning
  is that objective's final artifact.
- Where the guide and the spec disagree, **the spec wins and the guide has the
  bug** (CLAUDE.md §12) — and where the guide and the *compiler* disagree, that
  is a defect report on one of them, which is what the M-documentation-site check
  over `examples/` is for.
- **Metric 2's held-out tasks land here** (author decision 2026-08-24): they must
  be author-written or they measure the assistant's priors, and this is the one
  milestone whose own work *is* the author writing programs.
