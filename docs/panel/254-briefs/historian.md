# Panel 254, historian (advisory, no veto)

Read `00-shared.md` first, whole (repaired after the completeness critic's first reading,
`docs/panel/254-reports/critic-briefs.md`): the proposal (§ 5, its rows S1 to S14 and the open questions) is what
you find precedent for. **Every precedent you report is verified by web search and a page you read**,
quoted in its own words, with its URL and the day you read it, and marked `verified`; one you could not
read is marked `unverified` and is not admissible as prior art (`.claude/rules/records.md` § And the record
says whose idea it was: a line says what the cited page itself states, and nothing of what Heroes did
about it). **Your report**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-p254/docs/panel/
254-reports/historian.md`, written as you go. No paid run; no file outside the repository's root.

## Your questions

1. **Copying at the boundary, with isolated heaps** (design.md Part 7.13, `docs/design.md:4118-4180`,
   which already names Erlang and its refc binaries; read those lines): which languages or runtimes send
   any value between threads or isolates **as a copy into the receiver's heap**, and how each does the
   copy (a per-type walk, a serialisation, a flat block rebuilt on the other side). Dart's isolates and
   `SendPort.send`, JavaScript's structured clone (`postMessage`), Erlang's message passing, OCaml 5's
   domains, Pony and others you find. What each says about **which values may not cross** (S10: a raw
   pointer; JavaScript's functions and DOM nodes; Dart's native resources).
2. **A value whose life moves with it** (S11, `m.send(@v)` emptying `v`): JavaScript's transfer list
   (an `ArrayBuffer` detached in the sender), Dart's `TransferableTypedData`, Rust's move semantics on
   `Sender::send`, Pony's `iso`. What the sender's binding becomes and what reading it does.
3. **Close, drain and stop** (S6, S7, S8): what a send to a closed channel does (Go's `panic`, Rust's
   `SendError`), what a receive does on a closed channel with values left (Go drains then answers the zero
   value and `false`; Rust's `mpsc` and `crossbeam` drain then `Disconnected`; Tokio's `Receiver::close`),
   whether closing twice is an error (Go panics), and **whether a shutdown request outranks values still
   queued** in any system you can verify (S7 says the stop wins, as this project's `hero_poll.h` does).
4. **Unbounded against bounded** (S6 never waits): Erlang's mailboxes are unbounded; Go's channels and
   Rust's `sync_channel` are bounded; what each system's own documentation says the choice costs
   (deadlock, memory).
5. **The life of the queue** (S5: a value whose copies reach one queue, given back with its last copy,
   against panel 251's handle that a close ends): Go's channel is garbage-collected and closed
   separately; Rust's channel ends when every `Sender` or the `Receiver` is dropped. Which is closest to
   S5, and what each tells a program that forgets to close.
6. **Timed receive and `timeout`**: Erlang's `receive ... after`, Rust's `recv_timeout`, Java's
   `BlockingQueue.poll(timeout, unit)`, Go's `select` with `time.After`.
7. **A channel inside its own queue** (`00-shared.md` § 5 question 11, the completeness critic's finding
   29: a mailbox sent inside a value into its own queue is a reference cycle, which a count of copies
   never frees): what Go (a garbage collector), Rust (`Arc`, `mpsc::Sender` inside a message on its own
   channel), Erlang (a pid is a name, not a count) and Dart do with it, verified, and whether any system
   refuses it.
8. **A process-wide stop and queued replies** (`00-shared.md` § 5 question 12): any system whose
   shutdown signal makes every channel receive fail at once, and what its documentation says of a reply
   already queued.

For each question: the precedent, verified, its words; and in one line each, **where the proposal
departs from it** and whether the departure is named in the proposal. Give your verdict on route B as a
whole (approve, object), advisory, and say which precedent, if any, a resolution would rest on, so the
decision issue's `## Prior art` can be written from your verified lines alone.

## Your output

`.claude/agents/historian.md`'s structure, and `00-shared.md` § 10 where it applies (predictions,
premises of this brief you found false).
