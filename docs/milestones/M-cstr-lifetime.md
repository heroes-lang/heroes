# M-cstr-lifetime — a lent string may not outlive its lender *(closed 2026-09-09)*


**Placed first and alone by author decision 2026-09-09**, ahead of both the
budget instrument and the interpolation implementation, out of the three the
coordinator recommended. The reason is `CLAUDE.md` § Precedence: robustness is
rank 3, above elegance, token cost, ergonomics and compiler size, and a
milestone is tagged only over a clean list.

**The defect, found by the ffi-pragmatist at panel 121 and confirmed the same
hour on its own sixteen-line reproducer.** `return "static".cstr()` is sound
because `HERO_STR_STATIC` gives a literal static storage and an immortal
refcount. `return ("heap-" + n.to_str()).cstr()` is a **use-after-free**: the
owner slot that keeps the built `str` alive is decref'd at function exit, and
the `cstr` the caller holds points into freed memory. It builds at **exit 0
with zero diagnostics** under all fourteen flags; `strlen` answers **0** where
17 is the answer, three runs of three; and `--sanitize` says
`AddressSanitizer: heap-use-after-free`, READ of size 18, freed by
`hero_release_block` at `runtime/parts/alloc.c:151`.

**`spec/heroes-spec.md:253` is the sentence it breaks**: `s.cstr()` lends a
`str` to C *"for that call"*. Inside one function that promise is kept and
kept longer than it says; **returned**, it is not kept at all.

**What is owed is a refusal at the CLASS and not at the witness.** A `cstr`
derived from anything but a literal may not leave the function that built it,
and the diagnostic says which of the two spellings the program used, because
the two are one brace apart and a reader cannot see the difference. The
`tests/golden/fixedbugs/` cases owed are one per shape: a returned `cstr`, one
stored in a record, one pushed into a `[cstr]`, and the literal case that must
stay legal.

**Why it is this milestone and not interpolation's.** The defect predates panel
121 and needs none of its work to reach. What the sitting added is urgency: a
hole makes the sound and the dangling spelling one brace apart, so the class has
to be shut before a form lands that walks people into it.

*******************************************************************************
**OPEN: 0**

*******************************************************************************
