# 002 — What "separate heaps" would mean, concretely

**Origin.** 2026-08-10 · reasoning session · the author brought in a prose
argument that answers the three questions Part 7 item 13's one paragraph
leaves to whoever implements it, "so they are answered here rather than
reinvented later". Read: `design.md` Part 7 item 13, Part 6, Part 2, Part 9,
§1.11 (lines 357–362), §4.10, §4.20 · `docs/ROADMAP.md` M5a. No file of code,
spec or design modified.

## The question

Part 7 item 13 is one paragraph: concurrency is "the largest gap", value
semantics means "no data races by construction", Erlang got there in 1986, "the
road is open and wide; it is simply far away". Three questions it does not
answer, and each of them is the kind that gets decided by accident by the first
person to write code: what is a heap, what is a message, and what carries it.

## The reasoning, as brought to the session

### What "separate heaps" actually means

*What a heap is, in this context.* The region where runtime-sized data lives —
arrays, maps, strings, records. Not small scalars in registers or on the stack.
"Separate heaps" means each thread allocates from, and frees into, a region no
other thread touches. There is no shared allocator arena and therefore no lock
on allocation.

*What a message is.* A message is **any Heroes value.** There is no message
type, no envelope, no wrapper, and no serialisation format. This falls straight
out of value semantics: a Heroes value has no pointers to data it doesn't own
and no shared references, so it is already a self-contained thing. An `int`, a
`str`, a `[f64]`, a record, a map — all equally valid, all handled by one rule.

*What the transport is.* A queue per thread — a mailbox. The sender copies the
value into the receiver's heap and appends it to the tail; the receiver pops
from the head when it is ready. Single entry point, arrival order, nobody
reaching into anybody's region. The queue itself is deliberately boring, and it
can be boring precisely because what goes into it is isolated by construction.
In a shared-memory language the hard question is "what if the sender mutates it
while it's in flight" — here that question does not exist, because the receiver
got its own copy at the moment of sending.

*The consequence for refcounting.* Each copy carries its own counter in its own
heap. Counters never cross a thread boundary, which is exactly why the counter
can stay non-atomic without any of the usual danger. This is worth stating
explicitly, because it is the whole reason this shape was chosen over atomic
refcounting.

### Two copy regimes, and they are not a contradiction

Inside a single thread, the normal rule applies: values are shared physically
and copied only on mutation of shared data (copy-on-write). At a thread
boundary, the copy is unconditional and real. The first regime exists to avoid
waste; the second exists to guarantee isolation. Implementers should expect to
write two distinct code paths here and should not try to unify them.

### The cost rule, so the model is used where it works

A boundary copy costs proportional to the size of the value: sending a short
string is nothing, sending a ten-megabyte one is ten megabytes. The model pays
off when **the message is small relative to the work it triggers** — send a
slice, get back a number. It is a bad fit for shuttling large values back and
forth to do little with them. This is why the first and probably only rung
Heroes needs is pure data parallelism: the message in is a slice, the message
out is a result, and both are small against the computation between them.

### What is explicitly not being built

Three things are commonly confused and none of them belong in Heroes v1 or,
likely, ever:

- *OS threads* — few, real, OS-scheduled, genuine parallelism. The only kind
  Heroes will use.
- *Green threads* — cheap language-scheduled workers multiplexed onto OS
  threads. Require a scheduler written and maintained by the language. Not a
  goal.
- *Coroutines* — functions able to suspend and resume; the mechanism green
  threads are usually built from. Already listed in Part 6 as permanently
  rejected.

Threads give parallelism; green threads and coroutines give concurrency. Heroes
wants parallelism (split work across cores) and does not want concurrency
(thousands of interleaved logical tasks). Do not build a scheduler.

**Net effect on v1 code: none.** The two future-proofing rules stated above are
the entire obligation — keep refcount operations behind a narrow, never-inlined
boundary, and reach the allocator through a single point. Everything in this
section is design intent recorded so that the eventual implementation is a
mechanical exercise rather than a redesign.

## Audit against the document

**What checks out.** The Part 6 citation is correct: coroutines are there, with
the reason — "require stack switching (`ucontext` or stack copying) — a step
change in complexity". Part 2's "No concurrency in v1. This is the largest gap
and it is deliberate" and item 13's "the largest gap" agree with the text's
framing. And the message-is-any-value argument is a clean derivation from §4.10
rather than a new claim: §4.10's rule is that every value behaves as a copy,
which is exactly what makes an envelope unnecessary.

**Dangling — three rules the text cites as already stated, which the document
does not contain.** This is the audit obligation in this directory's README,
and it fires here:

1. **"The deferred concurrency direction above says 'isolated per-thread heaps
   with true copying at boundaries'."** It does not. Part 7 item 13 contains no
   sentence about heaps, threads or boundaries; `thread` occurs three times in
   `design.md` and all three are the verb ("threads a mutable list through its
   passes"). The words *mailbox*, *per-thread*, *green thread* and *scheduler*
   do not occur at all.
2. **"Keep refcount operations behind a narrow, never-inlined boundary."** Not
   in the document — `never-inlined` returns nothing. The *shape* half exists:
   §1.11's Tier 1 and §4.20 both list `incref` / `decref` as C runtime
   functions, so there is already one place they live. What is missing is the
   obligation: nothing says they may not be inlined or bypassed, and inlining a
   refcount operation is exactly what a later change would do for speed —
   which Part 2 renounces, but not by name here.
3. **"Reach the allocator through a single point."** Same shape, same gap:
   §4.20 has "allocator (a wrapper over `malloc`)" and §1.11 lists it in Tier 1,
   so one wrapper exists; no rule says all allocation goes through it.

The distinction matters because the text's own conclusion depends on it: "net
effect on v1 code: none" is true **only if** those two rules are written down
and honoured. If they are not, the cost of the deferred model silently stops
being zero and becomes a retrofit. The ownership pass at M5a is where refcount
operations start being emitted, so that is where the boundary either exists or
does not.

**The non-atomic counter is a new claim, and it is the load-bearing one.**
`atomic` does not occur in `design.md`. §4.10 explains the counter's job —
deciding when to copy — and says nothing about its atomicity, so today the
document neither promises nor forbids a non-atomic refcount. The text makes
non-atomicity a *consequence* of the concurrency model (counters never cross a
boundary), which is the right direction of dependency and the reason it cannot
be settled as an implementation detail: it is a decision about the concurrency
model, taken in the runtime.

**Genuinely new, and normative.** Four statements that no part of the document
makes, and that would constrain future work if adopted: OS threads are the only
kind Heroes will use · green threads are rejected, with a reason (a scheduler
the language would have to write and maintain) · pure data parallelism is the
first and probably only rung · "do not build a scheduler". Green threads are
absent from Part 6's rejection table even though coroutines — which the text
identifies as the mechanism they are usually built from — are in it.

## What was settled

The three questions have consistent answers, and the reason they are consistent
is §4.10: because a value is already self-contained, the transport can be a
plain queue and the message needs no envelope. The two-regimes point is the
part most likely to be lost — copy-on-write inside a thread and an
unconditional copy at the boundary are two code paths on purpose, and an
implementer trying to unify them would be undoing the design.

## What stayed open

Nothing lands in `design.md` from here. Part 7 is within Parts 1–11, and the
concurrency model is architecture: CLAUDE.md §4 makes both a panel path twice
over. Handed off:

- `docs/debrief/QUEUE.md` — the panel trigger for item 13's shape (mailbox, two
  regimes, OS threads only, no scheduler); and the decision on whether the two
  future-proofing rules are adopted as M5a obligations, since the ownership
  pass is where they stop being free.
- `docs/panel/OPEN-QUESTIONS.md` watch list — the non-atomic refcount, on the
  record as undecided rather than assumed.
