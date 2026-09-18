# Panel 164 — ffi-pragmatist

Read `00-shared.md` first. Veto on ABI breakage. You are the seat that found
this defect, at panel 162, beside another question.

## What you are asked

**Say what a sound route must GUARANTEE, and measure which real C parameters
need which guarantee.** The design question under every route is the same one:
a `char[N]` field hands C a pointer, and C's `const char *` parameters assume a
terminator that panel 162 measured only 13 of 50 such fields reliably have.

1. **Classify the parameters, not the fields.** Walk the same 16 headers panel
   162 walked, and this time classify the `const char *` and `const void *`
   PARAMETERS: which read to a terminator (`strlen`, `puts`, `open`), which take
   a sibling length (`write`, `memcpy`, `send`), which take a fixed width the
   header states. Counts with the command. **That split is what decides between
   route 2 (a lend to `cstr`) and route 3 (a lend to `ptr` only)**: if most
   pointer parameters are length-carrying, a `ptr` lend serves them soundly and a
   `cstr` lend is the exception rather than the rule.

2. **Verify what the existing lend guarantees**, by reading `emit/ops.hero`'s
   `guard_arguments` and `check/lending.hero`'s position rule, and say which of
   those guarantees a FIELD lend would inherit for free and which it would lack.
   A `str`'s bytes are always terminated by the runtime's own layout; a field's
   are not. Name that difference precisely, because it is the whole soundness
   argument.

3. **Build the sound shapes and show they work.** A field handed to a
   length-taking parameter alongside its declared length; a field handed to a
   terminator-reading parameter only after `validated_bytes` has confirmed a
   zero is present. Compile and run each under `--sanitize` and report what the
   program printed. These are the programs the resolution has to make writable.

4. **Say what each route refuses**, in the author's terms: which real bindings
   become writable and which stay refused, with one named header function for
   each.

## The shapes beside it (CL-061)

A field of `u8[N]` as well as `i8[N]`; a field inside a nested record; a field
of a record obtained from a function; the same field lent twice in one call.

## How to work

Copy the tree, `rm -rf build`, seed with
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`. Never rebuild from
`selfhost/`; never `archive/bootstrap-rs/`. Read the real header before binding
it. Every verdict from a command you ran.

## Deliver

Verdict · the section it rests on · the parameter classification with its
command · the programs you built and what they printed · a falsifiable
prediction · any ABI veto as a refusal.
