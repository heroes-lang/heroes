# M-witnessed-seed — the seed witnessed by a second implementation

**Scheduled 2026-10-14, at row 68, by the author's instruction**, before
`archive/bootstrap-rs/` leaves the tree. No warrant is claimed.

**The fact that schedules it.** The fixpoint proves that the seed reproduces
itself, not where it came from: a seed carrying something that reproduces
itself whenever it compiles `selfhost/` is a fixpoint too, which is the
construction of the 1984 Turing Award lecture *Reflections on Trusting Trust*
(CACM 27(8)). The CI's `cmp` proves determinism, on one clang. The one check
against a second implementation was made while one still ran: on 2026-08-18
the Rust bootstrap and the self-hosted compiler emitted the same C for 127 of
127 programs (`docs/records/journal/022-selfhost-fixpoint.md`), and
`archive/bootstrap-rs/README.md` calls that agreement *the only reason anybody
can trust the seed without reading 22 MB of C*. And "any C compiler" has been
proved on one clang (`seed/README.md`), while the seed carries 66
`#pragma clang` lines.

**What it delivers.**

1. The archived Rust bootstrap rebuilt with a pinned `rustc` and its
   comparison at `m-bootstrap-archive` run again and recorded as a
   measurement. Whether it still builds is unrun: nothing has built it since
   2026-08-19.
2. A CI leg that compiles the seed with gcc as well as clang and compares the
   C each resulting compiler emits for `selfhost/`: a second C compiler is
   one half of diverse double-compiling (ACSAC 2005,
   https://dwheeler.com/trusting-trust/).
3. The chain walked from that root to today's seed through the sources of
   the milestones, in two stages where a link does not build alone, as
   background work that blocks no other row.
4. Only then `archive/bootstrap-rs/` out of the tree, reachable at its tag,
   and the 186 living citations of it saying *ported from the Rust bootstrap,
   archived at `m-bootstrap-archive`*.

**Named for the deliverable**, the seed witnessed. Not `M-trusted-seed`, which
claims the trust the milestone exists to earn; not `M-diverse-compiling`,
which names the method.
