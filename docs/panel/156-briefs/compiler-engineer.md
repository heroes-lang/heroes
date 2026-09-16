# Panel 156 — compiler-engineer brief

Read `docs/panel/156-briefs/00-shared.md` first.

You judge the ceiling — design.md §1.1, §1.7, Part 5 — and implementation cost.
You have a veto on soundness. **§1.12 robustness outranks everything else in
this sitting** (CLAUDE.md § Precedence rank 3), so if you find a corruption or a
wrong answer at exit 0, say so and it decides.

## The one thing only you can settle

**Why does the frame walk find nothing on arm64 macOS and succeed on x86-64
Linux?** That is R2, it is the sitting's hinge, and it is measurable on the Mac
you are running on. The walk is `runtime/parts/stack.c:297-315`, quoted whole in
the shared brief. Its break conditions are the bounds check, the 8-alignment,
`ret == 0`, and `next_fp <= fp`.

Candidate causes, handed to you unpriced and none endorsed:

- `hero_stack_regs` extracting the wrong register as `fp` from a Darwin arm64
  `ucontext` — on AArch64 the frame pointer is `x29`, and the frame record is
  `{next_fp, lr}` at `[fp]`, `[fp+8]`, which is what the loop assumes;
- `hero_stack_lo`/`hero_stack_hi` being wrong or unset on the thread that runs
  `main`, so the bounds check breaks on the first iteration — note the loop
  breaks *before* reading anything, so a bad bound looks exactly like a short
  stack;
- the fixture's C compiled without a frame pointer, so the chain is broken at
  `node_value` itself;
- `dladdr` on macOS returning a `dli_sname` for the return address that is not
  the mangled Heroes name the predicate expects — check what it actually returns
  rather than assuming; `nm` on the built binary is cheap.

**Instrument it rather than reason about it.** A few `write(2, …)` lines in a
copy of `stack.c`, printing `fp`, `lo`, `hi` and each `dli_sname` the loop sees,
answers this in one build of the fixture. That is a seconds-long compile, not a
compiler rebuild.

## What else you are asked

1. **R1 and R2.** Which name is right, and if the walk is broken, what does the
   repair cost in lines and where does it live? Name the file.
2. **R3.** Is a flush on the abort path safe? `abort()` runs from a signal
   handler; `fflush` is not async-signal-safe by POSIX. Say what is actually
   safe here — the honest answer may be that `print` must not be buffered, or
   that the handler must write through a different path. Price both.
3. **R5.** The red `main`. Give a recommendation with its cost. Note that
   `UPDATE_GOLDEN=1` does not exist and is a hard stop, so nothing regenerates
   `.expected` for you.
4. **Whether any of this is core or sugar** by §1.7's test, and whether the
   runtime's three platform arms are diverging in a way that will keep costing.

## The route nobody has listed

CLAUDE.md § RUN IT says a recommendation is a claim about the option **set**.
Ask what would have to be true for a route nobody named to exist. One the
coordinator noticed and did not price: the walk could name **both** — the C
function that faulted and the Heroes caller — which would make all three
platforms' answers a subset of one line and might dissolve R1 rather than
answering it. Not endorsed; priced by nobody.

## Your verdict owes

A verdict per R1-R5, measured costs, a falsifiable prediction naming the
instrument that would score it, the condition under which you would change your
vote, and whether you cast your veto. Say explicitly what you left UNRUN.

Write your report to `docs/panel/156-reports/compiler-engineer.md` **first**,
before any long work.
