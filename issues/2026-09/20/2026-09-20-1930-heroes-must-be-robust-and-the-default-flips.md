# Heroes must be robust, and the default flips

2026-09-20. M-declared-extents step 21, on the author's decision, after the
measurement they made a condition of it.

## The decision

| | |
|---|---|
| date | 2026-09-20 |
| decision | **the default flips**: a C pointer parameter is assumed to KEEP what it is handed unless its declaration says otherwise, so a lend may reach only a parameter so marked. **The examples are rewritten as needed.** Robustness enters the Zen |
| reason | the author, reasoning aloud and then ruling, in these words: *"Heroes is a language that has existed for a short time; nobody in the world is really using it yet; the examples are published but the site says plainly that everything is still under construction, so if we have to change examples, no problem. I have not yet written real applications in Heroes, because I am missing fundamental parts that are the object of the next steps. One of Heroes' principles, which perhaps I forgot to state precisely, is that Heroes must be robust. It must absolutely be robust. We must go and catch all these problems. One doubt remains: how much does this choice cost in performance? If it makes every call into C cost many more CPU cycles and slows programs down a lot, we must measure it. Otherwise we go straight to the pessimistic assumption, the most robust one. This is what I have ruled."* |
| design.md § | §1.12, §4.19, §1.11 |
| panel | 170's default flip, taken by the author after the sitting priced it |

## What was measured before it took effect, because the author made it a condition

`docs/measurements/038-the-copy-costs-thirty-three-nanoseconds-and-the-mark-costs-nothing.md`:
twenty million calls into a C function that reads a 56-byte string, timed
sequentially with the machine still. **The lend is 40 ns a call and the copy is
73.5 ns, so copying where the author marks nothing costs 33.5 ns a call**; and
where the author marks the function as non-keeping the emitted C is today's
`hero_str_cstr`, a field read, **zero**. The author's phrase was *many more
cycles*. It is not, and the instruction takes effect.

## What changed since panel 170 recorded the flip as the author's to weigh

Panel 170 priced it at **eight refusals of eight correct programs** in
`examples/` and the coordinator recommended not flipping yet, on the ground that
nobody had measured what the ninth costs in the author's own programs. **The
author answered that ground directly**: there are no such programs yet, the
examples are declared under construction, and robustness is a principle, not a
price. That is a better answer than the recommendation, and the record says so:
the coordinator was weighing a cost the author knows to be zero.

## What it settles and what it does not

**Settled**: the polarity. The unmarked case is the safe one, which is what
panel 170's two objections asked for — the spec-warden's *the three existing
marks fail safe when omitted*, and the llm-ergonomist's *a mark must not
subtract the warning from the unmarked case*. Under the pessimistic default the
unmarked case is **refused**, not trusted, so both objections dissolve rather
than being overridden. And the historian's finding is followed rather than
departed from: Swift chose exactly this at its own C boundary in 2016 and never
withdrew it.

**Not settled, and it is the next sitting's**: the word, and the exact rule.
Every enforced-at-the-caller mark in the historian's survey is stated in the
negative polarity — Clang's `noescape`, C#'s `scoped`, Hylo's `let` — and Heroes
has no such word yet. Panel 170's own resolution binds whatever it is: **its own
grammar slot beside `counted_by`, never inside the `consumes|acquires|borrows`
alternation**, which `check/marks.hero` sweeps and refuses on exactly the types
this is for.

## The Zen

The author asked that robustness be stated in the mantra. Laws 17 and 18 are
near it — *Crashes and leaks are bugs* and *When pretty code and safe code
disagree, safe code wins* — and the author felt it was not said precisely
enough. The Zen is the author's voice and it is pinned byte for byte in
`selfhost/cli/doctor.hero` and mirrored on the site, so a law is **proposed to
the author in their words rather than landed in the coordinator's**.

## And the push waits

The author, the same evening: *as for pushing, at this point I would wait until
everything is finished.* Twenty commits sit on `main` locally and none has left.
