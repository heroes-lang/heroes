# M-discard-refusal — `_ =` stops swallowing a failure *(closed 2026-09-08)*


**Closed 2026-09-08**, tag `m-discard-refusal`, journal
[038](journal/038-discard-refusal.md), sitting `docs/panel/118`. What the
milestone measured, broke and repaired is there; what a later milestone has to
honour is here.

**The rule closes TWO positions and states the third, and the third is a ruling
rather than an omission.** `_ = e` on a written `T?` and a `_` parameter written
one are refused. A discard of a **type parameter** is not, because refusing it
relocates the hole instead of closing it — `function drop<A>(_: A)` swallows a
failure with no discard statement anywhere — and refusing that too leaves
`_ = [x]`, which is legal, means the same, and works on a **known** fallible, so
`_ = [make_dir(path: p)]` defeats the rule itself. **Anything that reopens this
owes a local rule that catches `_ = [x]`**, and the sitting's blind reader holds a
veto against closing it non-locally: a line whose legality depends on a call site
in another file is what that veto is for.

**The guarantee is outermost-only and the spec says so.** A fallible inside a
container or a record survives, and that is reachable with the spec's own
vocabulary and no generic: `_ = args_checked()` drops a whole `[str?]` in one
line. The class the rule does not reach — a fallible stashed in a container and
never inspected — needs **reachability** rather than a type judgment, and is
filed rather than owed here.

**No fix is `certain`, measured**: the same line's three repairs print 1, 9 and a
panic. A rule that exists to make a decision visible must not ship a repair that
takes the decision. **`M-check-completeness` inherits nothing from this** except
the generic hole's shape, which is now written in `spec:99` instead of waiting
for it.

**The `ignore` built-in is queued and not refused.** Two seats argued for a
named, greppable form; it is blocked by `selfhost/check/builtins.hero` sitting at
**374 of a DECIDED 374**, so it needs a ceiling raise, which is a sitting of its
own. The historian's naming ruling stands if it ever lands: `ignore`, being
Midori's own keyword for this job, where Rust's `drop` imports destructor timing
and Haskell's `void` returns a still-wrapped value.

**The departure is deliberate and recorded**: Hare 0.26.0, released 2026-02-13,
introduced `_ = os::remove(path)` as *the* explicit way to ignore an error. Zig
ships the rule this milestone adopted and has not retreated from it. Across
seventeen languages searched, none banned the silent drop with no valve at all.

*******************************************************************************
**OPEN: 0**

*******************************************************************************
