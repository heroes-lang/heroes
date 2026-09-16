# Panel 159 — historian report

Written to disk by the coordinator, verbatim from the seat's final message,
because this seat has no write tool. **Date checked: 2026-09-16.** Six
retrievals, one at a time, as briefed.

## verdict

**approve** (advisory, no veto). Precedent is strong and one-directional on item
1 — silence about sort ordering is a known, repeatedly-paid-for cost — and clear
but **incomplete** on item 2: the RFC that ADDED a fallible entry point was found
and quoted; **no written reason from any language that REFUSED one could be
verified**. That gap is the finding, not a footnote.

## P1 — Rust RFC 1937, `?` in `main` — VERIFIED

Source:
https://raw.githubusercontent.com/rust-lang/rfcs/master/text/1937-ques-in-main.md

Verbatim, Motivation:

> "It is currently not possible to use `?` in `main`, because `main`'s return
> type is required to be `()`. This is a trip hazard for new users of the
> language, and complicates 'programming in the small'."

Also verified: the mechanism is a `Termination` trait whose `report` consumes
self and returns an `i32` exit code; the runtime *"guarantees to call this
function after `main` returns, but at a point where it is still safe to use
`eprintln!` or `io::stderr()` to print error messages"*; and for `Result<T, E>`
the error is **printed as a diagnostic to stderr** using `Display`, following the
`cause` chain where one exists.

**Two things that cut the other way.** Rust did not make `main` return a value
the shell reads and nothing else — it made `main` return a value the runtime
**prints to stderr** and then converts to an exit code. That is remarkably close
to Heroes' own `error[main_returns]`: *"a program reports failure by what it
prints, not by what it returns."* Rust's answer is **both**, with the printing
done by the runtime rather than by the programmer.

**UNVERIFIED**: the Rust release that shipped it. Do not write a version number
on this seat's authority.

## P2 — Go: `main` may not return — the refusal VERIFIED, its reason NOT

Sources: https://github.com/golang/go/issues/34673 (opened 2019-10-03, closed)
and https://github.com/golang/go/issues/44750 (opened 2021-03-03, closed,
labelled `LanguageChange`, `Proposal-FinalCommentPeriod`, `v2`).

**Verified**: the compiler's refusal text is *"func main must have no arguments
and no return values"*, and the motivation Go users bring is RFC 1937's — CLI
authors wrapping logic in `run()` and calling `os.Exit`.

**NOT verified, and this is the important part**: neither fetch surfaced a
maintainer comment giving the reason. Both pages rendered without their comment
threads, so there is **no quotable Go-team rationale**, and the seat declined to
paraphrase one from memory. The honest statement for the record:

> Go refuses a fallible entry point and has declined at least two proposals to
> allow one. **The reason was not retrievable within this sitting's budget.**
> Settled by: `gh issue view 44750 --comments -R golang/go`, or the
> `golang/proposal` review minutes.

So the brief's most-wanted finding is **unverified**. Heroes currently holds
Go's position with a **better artifact than Go's**: a diagnostic that states the
philosophy in one sentence.

## P3 — PHP 8.0, sorts made stable — VERIFIED

Source: https://wiki.php.net/rfc/stable_sorting

Before: *"Sorting functions in PHP are currently unstable, which means that the
order of 'equal' elements is not guaranteed."* The documentation did not
guarantee it. The RFC's own examples of the cost: sorting users by age scrambles
a prior sort by name; `asort()` on `['a'=>0,'b'=>0,'c'=>1,'d'=>1]` may legally
return `['b'=>0,'a'=>0,'d'=>1,'c'=>1]`.

RFC dated 2020-05-12, vote 3–17 June 2020, **45 yes / 0 no**, target PHP 8.0,
Implemented. The RFC cites no specific bug reports.

**A decade-plus of unspecified behaviour closed unanimously, with zero dissent.
Nobody argued the silence was worth keeping.**

## P4 — ECMAScript `Array.prototype.sort`, implementation-defined until ES2019 — VERIFIED (search-level)

Sources: https://github.com/tc39/ecma262/pull/1340, /pull/1433, /pull/1585,
https://v8.dev/features/stable-sort.

**The closest thing to the bug report the brief asked for, and it is the exact
failure shape of this sitting's item 1:**

> Some JavaScript engines used a stable sort for short arrays and an unstable
> sort for larger ones, *"which was confusing as developers would test their
> code, see a stable result, but then suddenly get an unstable result in
> production when the array was slightly bigger."*

A program that compiles, runs, passes its test, and prints a silently different
answer in production because the input grew. JavaScript paid for it across every
engine for roughly two decades before ES2019 pinned it.

**UNVERIFIED**: the specific V8 version numbers and the old element threshold
come from a search summary, not a fetched page. Do not put them in the spec or a
record on this seat's authority.

## P5 — Java `Arrays.sort` stable/unstable split — UNVERIFIED, AND IT IS A COORDINATOR CLAIM

The historian and shared briefs assert that *"Java's `Arrays.sort` splits stable
and unstable by overload and documents which, and the JDK changed its
implementation and had to keep the promise."* **No retrieval was left to check
it.** The shared brief instructed each seat to assume an eighth uncorrected
coordinator claim exists; this seat cannot say this is it, only that **it must
not enter the sitting's record as verified on this report**. Settled by: the
`java.util.Arrays` Javadoc for `sort(Object[])` versus `sort(int[])`.

## P6 — Known and unsourced this sitting, written down rather than dropped

- **C++**: `std::sort` leaves stability unspecified and `std::stable_sort` exists
  as the separate, costlier promise. Unsourced here.
- **Python**: `list.sort()` is documented stable and the guarantee arrived at a
  specific 2.x version. Unsourced here — including the brief's own claim that
  Python documents ascending-by-default explicitly.
- **Item 3 of the brief** (a grammar permitting what the prose never teaches):
  **no budget reached it. Precedent is not thin here; it is unsearched.** Do not
  read the silence as evidence either way.

## argument

Precedent runs one way on ordering. PHP closed its stability silence 45–0; TC39
closed JavaScript's after engines shipped *stable for short arrays, unstable for
long ones* — a program that passes its test and lies in production, exactly the
risk the llm-ergonomist found. Both languages paid for years before writing one
clause. Rust's RFC 1937 names the cost of a `main` that cannot fail: *"a trip
hazard for new users… complicates 'programming in the small'."* But Rust's cure
prints the error to stderr and converts it to an exit code — nearer Heroes'
`main_returns` sentence than it first appears. Go refuses too, in two closed
proposals; **its written reason could not be retrieved, so Heroes' refusal is
currently better documented than the precedent for it.**

## condition

1. **A quoted Go-team closing comment** on #34673 or #44750. If it names a reason
   Heroes does not share — runtime teardown ordering, say — the precedent weakens
   and R3 becomes a cheaper sentence. If it names the same reason as
   `error[main_returns]`, Heroes should cite it and the sentence becomes
   near-free.
2. **A language that pinned sort DIRECTION — not stability — after leaving it
   implementation-defined.** This seat found the stability class twice and the
   direction class **zero times**. If direction was never a live ambiguity
   anywhere, R1's one word is **cheap insurance rather than a documented class of
   failure**, and the sitting should say which of the two it is buying.
3. **A Javadoc or JDK release note confirming P5.** Until then the Java claim in
   the coordinator's briefs is unverified and should be marked so wherever it is
   repeated.

## sources

- https://raw.githubusercontent.com/rust-lang/rfcs/master/text/1937-ques-in-main.md
- https://github.com/golang/go/issues/34673
- https://github.com/golang/go/issues/44750
- https://wiki.php.net/rfc/stable_sorting
- https://github.com/tc39/ecma262/pull/1340 · /pull/1433 · /pull/1585
- https://v8.dev/features/stable-sort
- https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/sort
