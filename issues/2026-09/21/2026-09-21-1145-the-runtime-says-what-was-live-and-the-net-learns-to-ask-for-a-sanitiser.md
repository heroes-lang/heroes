# The runtime says what was live, and the net learns to ask for a sanitiser

2026-09-21. M-declared-extents step 28, the landing of panels 172 and 173, and
the close of defect 070 — the milestone's last open defect.

## The decisions

| | |
|---|---|
| date | 2026-09-21 |
| decision | **the runtime names the live leases when the process dies by a signal it did not raise**, on the handler `runtime/parts/stack.c` already installs and the balance `runtime/parts/alloc.c` already keeps; every runtime-initiated death passes one `hero_abort()` funnel that says so first; the handler yields to the sanitiser at compile time and chains to the disposition it found |
| reason | panels 172 and 173 measured that no word on a declaration can reach the class — three of defect 070's seven shapes free the lease through a C function with no pointer parameter — and that the runtime can reach every shape where a lease is live, turning zero bytes of stderr into a named report |
| design.md § | §1.12, §4.17; Part 6 :2667 stands, :2340 corrected at step 26 |
| panel | 172 (full), 173 (soundness lane), both provisional, author ratification pending |

| | |
|---|---|
| date | 2026-09-21 |
| decision | **a golden may declare that the sanitiser must speak**: `!sanitizer: <needle>` in a `tests/golden/run/` expectation waives `suite_run.hero`'s refusal AND replaces it with an assertion on the sanitiser's own words |
| reason | the three cases that close defect 070 are programs C corrupts, so a silent sanitiser there is the defect; without the line `run` went 132 passed to 133 passed and 4 failed, every one *tripped a sanitiser*, whatever the expectation said (panel 173's ffi seat predicted it, the engineer measured it). The waiver and the assertion are one line on purpose: a case that stops provoking the sanitiser goes red |
| design.md § | §4.15's golden discipline, `.claude/rules/diagnostics-and-goldens.md` § An instrument watches the world |
| panel | 173, resolution item 5 |

## What both seats vetoed, and what the landing says instead

The mechanism's first draft asserted *a C function freed bytes this program
still leases*. The engineer wrote two programs whose C side frees nothing — a
library's own `abort()`, a library's failed `assert` — and both printed it; the
ffi seat reached the same place by enumeration and found the sentence **false
on six of nine measured paths**. Both then measured that `siginfo_t` cannot
tell the true case from the false ones: byte-identical on Darwin, `si_code` −6
for every self-raised SIGABRT on Linux.

The landed line states the two facts the runtime holds and names the C free as
a condition the reader checks:

    panic: the process died with 1 lease(s) still live, in lease070.main
      `end_lease` is the only thing that may free a `.lease()`. If one reached a C
      function that frees what it is handed, that is this death; if not, this says
      only what was live when the process ended.

§4.17's `guess` register, and true on all nine paths.

## The measurement neither seat could make, and the coordinator ran

The ffi seat judged the fifty-line prototype and found it destroys a C
library's SIGABRT disposition — exit 77 becoming 133, ten runs of ten on two
platforms — a break `runtime/parts/stack.c:363-375` had already settled by
saving the previous disposition and calling it. The engineer's build already
chained and marked that branch unrun, having written no such library. The two
seats measured two different builds, so the coordinator ran the ffi seat's
program against the engineer's: **77 ten times of ten, with the report still
printing.**

## What the spec gained, and what it costs

§ 13's lease paragraph: *No word says C frees what it is handed; one that frees
a lease kills the process, naming the leases live. Bytes C owns come from its
own allocator, with their disposer.* **+40 vendored, +54 real**, under
`DELTA_GATE`'s 50 — and the first complete draft measured **+55 vendored** and
was compressed three times, priced on the real instrument each time, rather
than split across two commits to get under the gate. The falsifier CL-005 asks
of a refusal is the ffi seat's question: *does the documentation name an
ARGUMENT that disposes of this pointer?*

## The author's standing instruction, reaffirmed today

In the author's words, translated: *on cases this hard the attention to
limiting tokens yields, and the robustness of the solution wins.* Already
CLAUDE.md § Precedence (robustness rank 3 above token cost rank 6), and applied
here three times: the panic-path funnel is fifteen sites rather than the one
the prototype had; the goldens got a harness form rather than a relaxed rule;
and the sanitiser case that must stay silent is a golden of its own, so the
report can never claim more than it knows.
