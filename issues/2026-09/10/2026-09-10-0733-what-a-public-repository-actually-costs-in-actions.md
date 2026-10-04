- [x] **M-publication-gate** | what a public repository actually costs in Actions minutes, measured on the account rather than read in a document | `.github/workflows/ci.yml` header · `.github/workflows/deploy-site.yml` | **DECIDED 2026-09-10, it widens** (author instruction: build the complete version on all three platforms, once, and start no second workflow). Every push to `main` runs the three legs; `m-*` tags start no run, because the commit they tag has already been judged on every platform and four tags pushed with a branch were starting five runs of one commit's work; `v*` release tags keep their trigger, their per-leg asserts and the `release` job. The five steps that were tag-only run on every push now. The mutation score stays gated on a release tag or a request: 20 to 30 minutes a leg for a table that came out identical on all three

    **Origin:** M-open-repository, 2026-09-08.

    Four comments explained the CI matrix by the price of a runner minute, and
    the narrowing to Linux between tags rests on it. GitHub's billing
    documentation says *"the use of standard GitHub-hosted runners is free: in
    public repositories"* and says nothing on that page about the operating
    system, so whether the reason has gone away for three legs, two or none is
    **unmeasured**. The comments were moved to the past tense rather than
    replaced with a reading.

    **Measured 2026-09-08, and the answer is zero for all three legs.** The
    endpoint the first attempt reached is retired (410); the current one is
    `/organizations/<org>/settings/billing/usage` and it needs no extra scope.
    Per day, either side of the flip:

        07 Sep, private:  Linux 196 min $1.18 · macOS 50 min $3.10 · Windows 86 min $0.86  ->  $5.14
        08 Sep, public:   Linux 110 min · macOS 37 min · Windows 50 min · gross $3.45  ->  $0.00

    The day before, the monthly allowance was exhausted and every minute was
    charged, macOS at ten times Linux. The day of the flip the gross is
    discounted in full, **macOS included**, which is the operating-system half
    the documentation did not answer. One day of data, so it is a measurement
    and not yet a law.

    **What is still owed is a decision and not a number**: the matrix narrows to
    Linux between tags for a reason that has now expired, so either it widens to
    every push or `ci.yml`'s header states the new reason it does not. Widening
    would find a Windows or macOS break on the day it is made rather than at the
    next tag. Filed at the gate because the question exists only because the
    repository became public, and the gate owns the outward state.

    **Why it matters:** every platform fact here is measured on a platform, and
    a cost is a platform fact.
