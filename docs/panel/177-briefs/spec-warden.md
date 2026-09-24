# Panel 177 — spec-warden

Read `00-shared.md` first. Your directory is `<scratchpad>/177-spec-warden/`,
HEAD `521c5e02`; build the compiler there from the seed. You judge the
indicator (design.md §1.2, §1.6) and Principle 0's burden of proof, with veto
on budget breach.

**The instrument.** `./heroes measure spec/heroes-spec.md` in your copy, with
the trunk's `.env` loaded so the real row is measured: `set -a; .
/Users/joseph/Temp/heroes/heroes-lang/.env; set +a`, and print
`${#ANTHROPIC_API_KEY}` if you need to know it loaded, never the value. At HEAD
the coordinator read **real 8361, maximum 6282, headroom 1879** against 10240,
the FFI floor mortgaging 60.

## What to price, on the real instrument

1. **The text panel 176 adopted, as it would land.** Panel 176's spec-warden
   priced a text it called C1 at `<scratchpad>/176-sw-work/C1_final.md`
   against `<scratchpad>/176-sw-work/pristine.md` (you may read both). Its
   synthesis then changed four things: the transfer names its releaser and the
   runtime checks it against the set; a transfer only on success says so; a
   reference keeps the live set and is a mark on a parameter as well as a
   result; and the call-site rule does NOT land, which C1 did not state anyway.
   Write the § 13 text that says the adopted resolution — items 1 to 8 of the
   synthesis — and price it, **once for each placement of the success clause**
   the ergonomist is reading (on the parameter, `transfers json_object_put on
   0`; on the result, `-> i32 when 0`).
2. **Question 1's sentence per route.** For each of M, R, P and S, the sentence
   that would replace *"Giving one back twice aborts, unless C has since reused
   its address"* (`spec/heroes-spec.md:387-388`), priced. And D, which is the
   sentence as it stands.
3. **What pays.** The author's word of 2026-09-23 is recorded in panel 176's
   synthesis; design.md §1.6's payment rule is not lifted by it
   (`docs/measurements/010-spec-budget-ledger.md` is the ledger). Name for
   each candidate what pays it: a removal, or a pre-registered falsifiable
   prediction.
4. **Principle 0 for each form**: the closure list, or a measured thesis
   effect. The success clause and `retains` have no program in `examples/`
   that needs them today (`grep -rE "\b(transfers|retains)\b" examples` is
   empty at HEAD — run it); say what that does to their burden.

## Say for every number which of these it is

Measured with the real instrument, with the digest; or estimated, and why.
