# Panel 254, the historian's report (advisory, no veto)

Seat: historian. Written 2026-11-22 under `docs/panel/254-briefs/00-shared.md` and
`docs/panel/254-briefs/historian.md` (both read whole), with `docs/design.md:4118-4189` read. Every
precedent below is a page read on **2026-11-22** with WebFetch, quoted in its own words, and marked
`verified`; a claim no page read today carries is `unverified` and is not admissible as prior art.
Each line says what the cited page states, and nothing of what Heroes did about it
(`.claude/rules/records.md` § And the record says whose idea it was). Where this seat infers rather
than quotes, the sentence says *inference*.

**One caveat on every quote**: WebFetch hands a page to a small reading model, which returns the
quotes; a quote here is what that reader returned as verbatim, not a byte-for-byte copy this seat
made. The session that files a `## Prior art` line re-reads the page (the records rule), which closes
that gap. This seat has no shell (its tools are Read, Write, WebSearch, WebFetch): it ran no command
of the tree, built nothing, started no paid run, printed no credential and wrote nothing to GitHub;
the one file it wrote is this report. *Heroes of code* was not consulted, neither as a route nor as a
source. Panel 251's historian report (`docs/panel/251-reports/historian.md`) was read as an index of
URLs only; every page cited below was fetched again today.

Status: **complete**, 2026-11-22. **Corrected by this seat before hand-over, the same day**, four
sentences of earlier drafts that said more than their pages:
- Node's `transferList` sentence was quoted with `<net.BlockList>` as its last item; a second reading
  of the page returned `<net.BoundSocket>` there, as the first had.
- The argument said every overtaking stop *"returns or reports"* the values it overtakes; Python's
  `immediate=True` discards them.
- Erlang was said to have kept stops behind the queue *"for decades"*, a span no page read gives.
- Go's leak profile was dated *"fifteen years after its channels"*, a date no page read gives.

Each is mended below.

---

## verdict

**approve route B (advisory)**: every piece of it has a working precedent on a page read today. I
also **object (advisory) to four rows as written**, each for a departure from every precedent read
that the proposal does not name:

1. **S6 with S11**: what becomes of a handle moved by `m.send(@v)` into a mailbox that answers
   `closed`. Rust hands the value back inside the error, and Kotlin hands it to a callback written
   *"to close a resource"*. The proposal says nothing.
2. **S7's stop**: every system read that lets a stop overtake values already queued does one of two
   things. Either the stop is chosen per call or per receiver (Python's `immediate=True`, Erlang's
   EEP 76, Tokio's `biased;`), or the overtaken values go back to the program (Java's
   `shutdownNow` list, Kotlin's `onUndeliveredElement`). **None read is process-wide and
   permanent.**
3. **S5's life**: one value that both sends and receives, and goes with its last copy, is Go's
   channel shape with a count in place of a tracer. Go added a reachability-based leak profile (1.26,
   experimental) for the blocked receiver. Rust splits the two ends: the end of every sender is the
   close, and the end of the receiver discards the queue. *Inference*: neither remedy is open to a
   count of copies of one value. The proposal names neither.
4. **S9's function values**: JavaScript refuses every callable at the boundary (`DataCloneError`).
   The departure is unnamed.

The rows where precedent supports the proposal as written: S8 (close drains, a second close does
nothing), S10 (a `ptr` and a value holding one, Rust's reason word for word), S11's move checked
when compiled (Rust, Pony, Nim 2), S7's `-1` (POSIX `poll`), and S3's consuming `wait` (Rust's
`join(self)`).

### Per row, advisory (the historian reads precedent; the measurements are the compiling seats')

| row | reading | the precedent it rests on (Q below) |
|---|---|---|
| S1 `spawn` gives `(thread T)?`, failing `limit` | **adopt** | Rust `Builder::spawn` *"yields an io::Result to capture any failure to create the thread at the OS level"* (the free `spawn` panics) |
| S2 `spawn_sized` | **adopt**; no precedent read for *"`n` below 0 aborts"* | Rust `stack_size`: *"The actual stack size may be greater than this value if the platform specifies a minimal stack size."* |
| S3 `t.wait()` consumes | **adopt** | Rust `pub fn join(self) -> Result<T>` takes the handle by value |
| S4 `mailbox()` | no precedent bears on the spelling | (none read) |
| S5 a value, the queue going with its last copy | **object (advisory)**: amend, or name the departure | Q5, Q7: Go (a tracer, a 1.26 leak profile), Rust (split ends, discard on receiver drop), Racket (GC'd place channels) |
| S6 unbounded send, `closed` after the close | **adopt the unbounded queue** (the copying family's default); **object to the silence on the moved handle** | Q4: Erlang, Dart, Racket, Python, Nim 1.x unbounded; Go refused it (issue 20352). Q3: Rust `SendError` returns the value, Kotlin `onUndeliveredElement` |
| S7 `receive(within_ms:)`, `timeout`, `closed` after drain, `stopped` wins | **adopt `-1` and the abort below it; adopt close-then-drain; object to the stop as written** | Q6: POSIX `poll`, CPython's `ValueError`, Erlang's range. Q3/Q8: Python, Kotlin, Java, EEP 76 |
| S8 `close` for every copy, again does nothing | **adopt** | Kotlin `close`: *"If the channel was already closed, this function does nothing and returns `false`."*; Java `shutdown`: *"Invocation has no additional effect if already shut down."*; departs from Go (panic) |
| S9 every value whole, a mailbox inside a value | **adopt, function values excepted until the compiler-engineer says what one carries** | Q1: Erlang, Dart, HTML, Nim 1.x, Python 3.14; a channel inside a message: Racket, Python (a `Queue` passed to a thread), Dart (`SendPort`) |
| S10 `ptr`, `cstr`, a value holding one | **adopt** | Rustonomicon: raw pointers *"neither Send nor Sync"*, and it matters *"to prevent types that contain them from being automatically marked as thread-safe"*; Racket is the one read that passes C pointers |
| S11 a handle crosses with `@`, emptied | **adopt** | Q2: Pony `consume`, Rust move, Nim `Isolated`, Node `transferList` with `net.Socket` and `FileHandle` |
| S12 the stop | see S7 | Q8 |
| S13 names | no precedent read today (Go's shadowable `close` is panel 251's reading, *carried*, not re-read: `unverified` here) | (none read) |
| S14 the doors | no precedent bears | (none read) |

**On A as the conservative route**: no precedent read argues against a library mailbox of text.
Every isolate system read sends richer values than text (Dart, Python, Racket, JavaScript), so A's
*"a program would serialise every richer value into text"* is the shape all of them moved past, by
copying or by pickle. That is a reading, not a measurement.

---

## precedents

### Q1. Copying at the boundary, with isolated heaps (S9, S10)

**How each copies, one line each** (the brief's three mechanisms: a per-type walk, a serialisation,
a flat block rebuilt on the other side):

| system | separate heaps? (its page) | how a value crosses (its page) | what may not cross (its page) |
|---|---|---|---|
| Erlang/OTP 29.1.2 | (not on the page read) | copied, except refc binaries and literals; the mechanism is not on the page | (not on the page read) |
| Dart 3.14.1 | *"can't "see" inside each others' memory"* | *"copy the transitive object graph"*, immutable objects shared; `Isolate.exit` reassigns the graph uncopied | native resources (`Socket`), `ReceivePort`, `DynamicLibrary`, `Finalizable`, ... |
| JavaScript (HTML standard) | separate realms | **a flat record**: serialised to *"a realm-independent form"*, rebuilt *"in targetRealm"* | callables, non-serialisable platform objects (DOM nodes): `DataCloneError` |
| Nim 1.x (`--threads:on`) | *"thread-local, GC-managed memory"* | *"msg is deeply copied"* | cyclic data (*"does not work"*); a channel itself (*"cannot be passed between threads"*) |
| Python 3.14 subinterpreters | *"effectively isolated"* | **a serialisation**: *"most objects are copied with pickle"*, immutables shared or copied | anything raising `NotShareableError` |
| Racket places | *"separate garbage collection of separate places"* | the listed kinds only, mutable ones made immutable | anything `place-message-allowed?` refuses; **C pointers are allowed** |
| Node.js v26.13.1 `worker_threads` | (not on the page read) | *"compatible with the HTML structured clone algorithm"* | native objects other than a listed ten |
| OCaml 5.4 | a minor heap per domain, major-heap pools per domain | not copied: *"immutable values may be freely shared"* | (none: a shared-heap design) |

- **Erlang** (`verified`), *Efficiency Guide, Processes*,
  https://www.erlang.org/doc/system/eff_guide_processes.html, OTP 29.1.2, read 2026-11-22: *"All data
  in messages sent between Erlang processes is copied"*, *"except for refc binaries and literals on the
  same Erlang node."*; *"When a literal is sent to another process, it is _not_ copied."*; sharing is
  lost: `{SubTerm, SubTerm}` measures *22* words by `erts_debug:size/1` and *4094* by
  `erts_debug:flat_size/1`, and one of the listed cases is *"When a term is passed as the initial
  process arguments in the `spawn` call."* **The page does not say how the copy is done** (nothing on
  a size-first walk or a heap fragment). Across nodes, a term *"is first encoded to the Erlang
  External Format"*. **Departure**: S9 copies every value, a `str` among them, where Erlang shares
  refc binaries and literals. design.md 7.13 names it (`design.md:4175-4181`); S9's row does not.
- **Dart** (`verified`), `SendPort.send`, https://api.dart.dev/stable/dart-isolate/SendPort/send.html,
  Dart 3.14.1, read 2026-11-22: *"Objects that are identified as immutable (e.g. strings) will be shared
  whereas all other objects will be copied."*; *"The send happens immediately and may have a linear time
  cost to copy the transitive object graph."* Between isolates that share code, anything may be sent
  except *"Objects with native resources"* (the page names `Socket`), `ReceivePort`, `DynamicLibrary`,
  `Finalizable`, `Finalizer`, `NativeFinalizer`, `UserTag`, `MirrorReference`, and classes marked
  `@pragma('vm:isolate-unsendable')`. Between isolates that do not share code, a closed list applies,
  and `SendPort` is on it. `Isolate.exit`, https://api.dart.dev/stable/dart-isolate/Isolate/exit.html:
  *"this final message object graph will be reassigned to the receiving isolate without copying"*, and
  *"the receiving isolate will in most cases be able to receive the message in constant time."*
  *Concurrency in Dart*, https://dart.dev/language/isolates (updated 2025-12-10): isolates *"can't
  "see" inside each others' memory"*; of `Isolate.run()`'s result, *"It _does not copy_ the data."*
  **Departures**: (a) S3's `wait` copies the answer back, where `Isolate.exit` and `Isolate.run` hand
  it over uncopied (not named in S3); (b) Dart refuses a socket outright, where S11 moves it (Dart's
  refusal is not cited); (c) Dart shares immutables, where S9 copies them (as with Erlang, above).
- **JavaScript, the HTML standard** (`verified`), *Safe passing of structured data*,
  https://html.spec.whatwg.org/multipage/structured-data.html, read 2026-11-22:
  StructuredSerializeInternal *"serializes it to a realm-independent form, represented here as a
  Record"*; StructuredDeserialize *"deserializes it into a new JavaScript value, created in
  targetRealm"*; *"Otherwise, if IsCallable(value) is true, then throw a "DataCloneError"
  DOMException."*; *"Otherwise, if value is a platform object, then throw a "DataCloneError"
  DOMException."* **MDN**, *The structured clone algorithm*,
  https://developer.mozilla.org/en-US/docs/Web/API/Web_Workers_API/Structured_clone_algorithm (last
  modified 2025-06-29): *"It clones by recursing through the input object while maintaining a map of
  previously visited references, to avoid infinitely traversing cycles."*; *"`Function` objects cannot
  be duplicated by the structured clone algorithm; attempting to throws a `DataCloneError`
  exception."*; *"Cloning DOM nodes likewise throws a `DataCloneError` exception."*; *"The prototype
  chain is not walked or duplicated."* **This is the brief's third mechanism, a flat record rebuilt on
  the other side, standardised.** **Departure**: S9 lets a function value cross, and JavaScript refuses
  every callable. **Not named in S9.** Whether a Heroes function value carries captured state is the
  compiler-engineer's to say (a question, not a premise). Dart's same-code isolates send closures;
  JavaScript sends none.
- **Node.js** (`verified`), *Worker threads*, https://nodejs.org/api/worker_threads.html, v26.13.1,
  read 2026-11-22: *"`value` is transferred in a way which is compatible with the HTML structured clone
  algorithm."*; *"The message object is cloned immediately, and can be modified after posting without
  having side effects."*; *"`value` may not contain native (C++-backed) objects other than"* ten listed
  kinds; `new URL(...)` posted throws *"DataCloneError: Cannot clone object of unsupported type."*; of
  `workerData`, an error is thrown if an object cannot be cloned, *"e.g. because it contains
  `function`s."*
- **Nim 1.x** (`verified`), *channels*, https://nim-lang.org/1.4.0/channels.html and
  https://nim-lang.org/1.0.0/channels.html, read 2026-11-22: *"Sends a message to a thread. msg is
  deeply copied."*; *"The current implementation of message passing does not work with cyclic data
  structures."*; (1.4.0) *"objects created using the default allocator will use thread-local,
  GC-managed memory"*, while global channel variables *"will use a process-wide (thread-safe) shared
  heap"*; *"Channels cannot be passed between threads. Use globals or pass them by ptr."*; *"Channels
  are designed for the Thread type. They are unstable when used with spawn"*. **Of the pages read,
  this is the nearest ancestor of 7.13's whole shape**: thread-local heaps, a deep copy at the send, a
  queue outside both heaps. It is also **the one read that copies with no shared exception**. **What
  came next**: Nim's `std/isolation` (its sources linked under `version-2-4`),
  https://nim-lang.org/docs/isolation.html: *"Isolated data can only be moved, not copied."*,
  *"Isolation is checked at compile time."* (Q2). **Why** Nim added the move is on no page read:
  `unverified`. **Departure**: S9 sends a `(mailbox U)` inside a value, where Nim 1.x's channel could
  not be passed between threads at all.
- **Python 3.14, `concurrent.interpreters`** (`verified`),
  https://docs.python.org/3/library/concurrent.interpreters.html (3.15.2 documentation), read
  2026-11-22: *"Added in version 3.14."*; *"Each interpreter is effectively isolated from the others
  (with a limited number of carefully managed process-global exceptions to the rule)."*; *"By default,
  most objects are copied with pickle when they are passed to another interpreter. Nearly all of the
  immutable builtin objects are either directly shared or copied efficiently."*; *"There are a small
  number of Python types that actually share mutable data between interpreters: `memoryview`,
  `Queue`."*; `NotShareableError`, *"raised when an object cannot be sent to another interpreter"*, a
  subclass of `TypeError`. The page's example passes a `Queue` to
  `interp.call_in_thread(worker, queue)`. **The newest precedent read for isolated heaps in one
  process, with a queue that crosses**; its copy is a serialisation, not a walk.
- **Racket places** (`verified`), https://docs.racket-lang.org/reference/places.html, read 2026-11-22:
  a place is *"effectively a separate instance of the Racket virtual machine, although all places run
  within a single operating-system process"*, with *"separate garbage collection of separate places"*.
  `place-message-allowed?` admits numbers, characters, booleans, keywords, void, symbols, strings and
  byte strings (mutable ones become immutable), paths, pairs, lists, boxes, vectors, immutable prefab
  structures, hash tables, **place channels**, **file-stream ports and TCP ports**, **C pointers from
  `ffi/unsafe`**, and the shared vectors and byte strings. *"place channels themselves can be sent
  across channels to establish new (possibly more direct) lines of communication"*. A port's
  underlying representation *"is duplicated in the sending place and attached to a fresh port in the
  receiving place"*. **Departures**: (a) **S10 refuses a `ptr`, and Racket passes a C pointer** (not
  named in S10); (b) Racket **duplicates** a port and leaves the sender's open, where S11 moves a
  handle and empties the sender (not named). **Precedent for S9's mailbox inside a value**: Racket's
  place channel inside a message.
- **OCaml 5.4** (`verified`), *Parallel programming*, https://ocaml.org/manual/5.4/parallelism.html,
  read 2026-11-22: *"Each domain has its own domain-local minor heap arena"*; *"immutable values may be
  freely shared between multiple domains and may be accessed in parallel"*; of racy programs, *"there
  are no crashes."* The page says nothing about copying at `Domain.spawn`. **Not a copying
  precedent**: the brief named it, and it is a shared-heap design, recorded here so that nobody cites
  it as one.
- **Pony** (`verified` for what is quoted), *Garbage Collection*,
  https://tutorial.ponylang.io/appendices/garbage-collection.html, read 2026-11-22: each actor has its
  own heap and *"the runtime performs garbage collection on that actor's heap"*. Whether a send copies
  is **not on the pages read** (the reference-capabilities page says `iso` data may be given *"to
  another actor"* and `val` shared): that Pony sends without copying is `unverified` here.

**S10, what may not cross**: the pages read refuse **callables** (HTML, MDN, Node's `workerData`),
**native resources** (Dart, whose example is `Socket`), **unshareable objects** (Python's
`NotShareableError`) and **raw pointers and every type holding one** (Rust). The Rustonomicon,
https://doc.rust-lang.org/nomicon/send-and-sync.html, read 2026-11-22: *"raw pointers are neither Send
nor Sync (because they have no safety guards)"*; *"However it's important that they aren't
thread-safe to prevent types that contain them from being automatically marked as thread-safe."*;
*"`Rc` isn't Send or Sync (because the refcount is shared and unsynchronized)."* **That is S10's
*"or a value holding one"*, with its reason.** Racket is the one read that **admits C pointers**.

### Q2. A value whose life moves with it (S11)

- **JavaScript, transfer** (`verified`), MDN *Transferable objects*,
  https://developer.mozilla.org/en-US/docs/Web/API/Web_Workers_API/Transferable_objects (last modified
  2025-09-18), read 2026-11-22: *"Transferable objects are objects that own resources that can be
  transferred from one context to another, ensuring that the resources are only available in one
  context at a time. Following a transfer, the original object is no longer usable; it no longer points
  to the transferred resource, and any attempt to read or write the object will throw an exception."*
  The example prints `byteLength` *8388608* before `postMessage(uInt8Array, [uInt8Array.buffer])` and
  *0* after. The transferables listed include **handles**: `MessagePort`, `ReadableStream`,
  `WritableStream`, `TransformStream`, `RTCDataChannel`, `WebTransportReceiveStream`,
  `WebTransportSendStream`. The HTML standard (URL in Q1): *"Set transferable.[[Detached]] to
  true."*; an `ArrayBuffer` already detached, or one listed twice (*"If memory[transferable]
  exists"*), throws `DataCloneError`. **The sender's binding becomes** the same object, detached;
  **a read of it** throws, at run time.
- **Node.js, an OS handle moved** (`verified`), https://nodejs.org/api/worker_threads.html, v26.13.1,
  read 2026-11-22 (twice, for this sentence): *"`transferList` may be a list of `<ArrayBuffer>`,
  `MessagePort`, `FileHandle`, `<net.Server>`, `<net.Socket>`, and `<net.BoundSocket>` objects. After
  transferring, they are not usable on the sending side of the channel anymore (even if they are not
  contained in `value`)."* `net.Socket` may be in `value` only *"TCP only, when listed in
  `transferList`"*. The method's changelog row: *"Added `FileHandle` to the list of transferable
  types."* (v14.5.0, v12.19.0); no row dates `net.Socket`'s entry. `markAsUntransferable`: *"If
  `object` occurs in the transfer list of a `port.postMessage()` call, an error is thrown."* **The
  nearest precedent read for S11's socket crossing between threads of one process, the sender left
  unable to use it.**
- **Erlang, `gen_tcp:controlling_process/2`** (`verified`),
  https://www.erlang.org/doc/apps/kernel/gen_tcp.html, OTP 29.1.2, read 2026-11-22: *"Assigns a new
  controlling process `Pid` to `Socket`."*; *"If this function is called from any other process than
  the current controlling process, `{error, not_owner}` is returned."*; *"If the socket is in _active
  mode_, this function will transfer any messages from the socket in the mailbox of the caller to the
  new controlling process."*
- **Dart, `TransferableTypedData`** (`verified`),
  https://api.dart.dev/stable/dart-isolate/TransferableTypedData-class.html, Dart 3.14.1, read
  2026-11-22: *"sending it through a send port will only take constant time."*; *"When sent this way,
  the local transferable can no longer be materialized, and the received object is now the only way to
  materialize the data."* `materialize`,
  https://api.dart.dev/stable/dart-isolate/TransferableTypedData/materialize.html: *"The
  TransferableTypedData is a cross-isolate single-use resource."*; *"This method must not be called more
  than once on the same underlying transferable bytes, even if the calls occur in different
  isolates."* Which error a second call raises is not on the page: `unverified`.
- **Rust, `Sender::send`** (`verified`), *The Rust Programming Language*, 16.2,
  https://doc.rust-lang.org/book/ch16-02-message-passing.html, read 2026-11-22: *"The `send` function
  takes ownership of its parameter, and when the value is moved the receiver takes ownership of it.
  This stops us from accidentally using the value again after sending it"*. Listing 16-9's error:
  *"error[E0382]: borrow of moved value: `val`"*, *"value moved here"*, *"value borrowed here after
  move"*. **A read of the moved binding** is refused when compiled.
- **Pony, `consume`** (`verified`),
  https://tutorial.ponylang.io/reference-capabilities/consume-and-destructive-read.html, read
  2026-11-22: *"When you `consume` a variable you take the value out of it, effectively leaving the
  variable empty."*; *"No code can read from that variable again until a new value is written to
  it."*; and *"the compiler will complain"*. `iso`,
  https://tutorial.ponylang.io/reference-capabilities/reference-capabilities.html: *"If you have an
  `iso` variable then you know that there are no other variables that can access that data."*, *"So you
  can change it however you like and give it to another actor."* **The closest in shape to `@v`**: a
  marked expression that empties the binding, checked when compiled.
- **Nim 2.x, `Isolated[T]`** (`verified`), https://nim-lang.org/docs/isolation.html, read 2026-11-22:
  *"Isolated data can only be moved, not copied."*; `extract`, *"The value is moved from src."*;
  `isolate`, *"Isolation is checked at compile time."*; `unsafeIsolate`, *"The proc doesn't check
  whether value is isolated."*

**Where S11 stands among them**: it is a move the program writes (`@`) that empties the sender's
binding. That is Pony's `consume` and Rust's move, checked when compiled, where JavaScript, Node and
Dart check at run time. Of the systems that move an OS handle: Node moves `net.Socket` and
`FileHandle` (the sender's is unusable afterwards), Erlang moves a socket's owner (only the owner
may), Racket duplicates a port, and Dart refuses a `Socket`. Whether a read of an emptied Heroes
binding is refused when compiled or aborts is for the spec-warden and the compiler-engineer to say
(a question, not a premise).

### Q3. Close, drain and stop (S6, S7, S8)

| system | send after close | receive on closed, values left | close twice | does a stop outrank queued values? |
|---|---|---|---|---|
| Go 1.27 | *"causing a run-time panic"* | drains, then zero value and `ok == false` | panic | `select` picks *"uniform pseudo-random"*: neither |
| Rust std 1.100.0 `mpsc` | `Err`, value returned in `SendError` | drains (*"messages sent before the disconnect will still be properly received"*), then `Err` | no close; the drop is the close | (no stop) |
| crossbeam-channel 0.5.17 | (not on the page) | *"any remaining messages can still be received"* | no close | `select!` random; `select_biased` exists |
| Tokio 1.54.1 | (permits already taken still send) | drains until `None` | (not on the page) | `select!` random; `biased;` and the advice to put shutdown first |
| Kotlin coroutines | `ClosedSendChannelException` | drains, then `ClosedReceiveChannelException` | *"does nothing and returns `false`"* | yes: the prompt-cancellation guarantee, the element passed to `onUndeliveredElement` |
| Python 3.13+ `queue.Queue` | `ShutDown` | `immediate=False`: drains, then `ShutDown` | (not on the page) | **both**: `immediate=True` discards the queue |
| Java SE 25 | `BlockingQueue` has no close; `ExecutorService` rejects new tasks | `shutdown()` runs the queued tasks | `shutdown()`: *"no additional effect"* | `shutdownNow()` halts them and **returns** them |
| Erlang OTP 29.1.2 | a send to a pid *"never fails"* | (no close) | (no close) | a trapped exit is queued *"to the end"*; **EEP 76 (OTP 28)** lets an opted-in receiver have a message *"overtake all ordinary messages"* |

- **Go** (`verified`), *The Go Programming Language Specification*, https://go.dev/ref/spec, *"Language
  version go1.27 (May 26, 2026)"*, read 2026-11-22 by offsets 0, 100000, 135000 and 160000: *"A send
  on a closed channel proceeds by causing a run-time panic. A send on a `nil` channel blocks
  forever."*; *"A receive operation on a closed channel can always proceed immediately, yielding the
  element type's zero value after any previously sent values have been received."*; the `ok` form is
  *"`false` if it is a zero value generated because the channel is closed and empty"*; `close`:
  *"Sending to or closing a closed channel causes a run-time panic. Closing the nil channel also causes
  a run-time panic."*; `select`: *"If one or more of the communications can proceed, a single one that
  can proceed is chosen via a uniform pseudo-random selection."* *A Tour of Go*, its source
  https://raw.githubusercontent.com/golang/website/master/_content/tour/concurrency.article: *"Only the
  sender should close a channel, never the receiver."*; *"Sending on a closed channel will cause a
  panic."*
- **Rust** (`verified`), `Sender::send`,
  https://doc.rust-lang.org/std/sync/mpsc/struct.Sender.html, std 1.100.0, read 2026-11-22:
  *"Attempts to send a value on this channel, returning it back if it could not be sent."*; *"Note that
  a return value of `Err` means that the data will never be received, but a return value of `Ok` does
  _not_ mean that the data will be received."*; the example `drop(rx);
  assert_eq!(tx.send(1).unwrap_err().0, 1);`. `Receiver::recv`,
  https://doc.rust-lang.org/std/sync/mpsc/struct.Receiver.html: *"If the corresponding `Sender` has
  disconnected, or it disconnects while this call is blocking, this call will wake up and return
  `Err` to indicate that no more messages can ever be received on this channel. However, since
  channels are buffered, messages sent before the disconnect will still be properly received."*
- **crossbeam-channel 0.5.17** (`verified`), https://docs.rs/crossbeam-channel/latest/crossbeam_channel/,
  read 2026-11-22: *"When all senders or all receivers associated with a channel get dropped, the
  channel becomes disconnected."*; *"No more messages can be sent, but any remaining messages can still
  be received."*; *"Send and receive operations on a disconnected channel never block."*; `select!`:
  *"If multiple operations are ready at the same time, a random one among them is selected."*
- **Tokio 1.54.1** (`verified`), `Receiver`,
  https://docs.rs/tokio/latest/tokio/sync/mpsc/struct.Receiver.html, read 2026-11-22: *"To guarantee
  that no messages are dropped, after calling close(), recv() must be called until None is
  returned."*; `recv` *"returns None if the channel has been closed and there are no remaining messages
  in the channel's buffer."* `select!`, https://docs.rs/tokio/latest/tokio/macro.select.html: *"By
  default, `select!` randomly picks a branch to check first."*; with `biased;`, *"This will cause
  `select` to poll the futures in the order they appear from top to bottom."*; and *"If for example you
  are selecting between a stream and a shutdown future, and the stream has a huge volume of messages
  and zero or nearly zero time between them, you should place the shutdown future earlier in the
  `select!` list to ensure that it is always polled, and will not be ignored due to the stream being
  constantly ready."*
- **Kotlin coroutines** (`verified`), `SendChannel.close`,
  https://kotlinlang.org/api/kotlinx.coroutines/kotlinx-coroutines-core/kotlinx.coroutines.channels/-send-channel/close.html,
  read 2026-11-22: *"If the channel was already closed, this function does nothing and returns
  `false`."*; *"The existing elements in the channel remain there"*; *"All subsequent calls to send
  will throw a ClosedSendChannelException, whereas calling receive will throw a
  ClosedReceiveChannelException after there are no more elements."* `ReceiveChannel.receive`,
  https://kotlinlang.org/api/kotlinx.coroutines/kotlinx-coroutines-core/kotlinx.coroutines.channels/-receive-channel/receive.html:
  *"There is a **prompt cancellation guarantee**: even if receive managed to retrieve the element from
  the channel, but was cancelled while suspended, CancellationException will be thrown, and, if the
  channel has an `onUndeliveredElement` callback installed, the retrieved element will be passed to
  it."* `Channel`,
  https://kotlinlang.org/api/kotlinx.coroutines/kotlinx-coroutines-core/kotlinx.coroutines.channels/-channel/:
  *"As a result of the prompt cancellation guarantee, when a closeable resource (like an open file or a
  handle to another native resource) is transferred via a channel, it can be successfully extracted
  from the channel, but still be lost if the receiving operation is cancelled in parallel."*; *"A
  typical usage for `onUndeliveredElement` is to close a resource that is being transferred via the
  channel."*; *"Resources are never lost."*; the callback is called *"When the channel was cancelled,
  in which case `onUndeliveredElement` is called on every remaining element in the channel's buffer."*
  **This is the closest precedent read to S5's *"a handle among them told"***, and its answer is a
  callback the program writes, not a count told at the end.
- **Python** (`verified`), `queue`, https://docs.python.org/3/library/queue.html (3.15.2 docs), read
  2026-11-22: `Queue.shutdown(immediate=False)`, added in 3.13: *"The queue can no longer grow. Future
  calls to `put()` raise `ShutDown`."*; *"If _immediate_ is false (the default), the queue can be wound
  down normally with `get()` calls to extract tasks that have already been loaded."*; *"Once the queue
  is empty, future calls to `get()` will raise `ShutDown`."*; *"If _immediate_ is true, the queue is
  terminated immediately. The queue is drained to be completely empty"*; `get` *"Raises `ShutDown` if
  the queue has been shut down and is empty, or if the queue has been shut down immediately."* CPython's
  source, https://raw.githubusercontent.com/python/cpython/main/Lib/queue.py: under `if immediate:`,
  `while self._qsize(): self._get()`, so the items go to nobody. **The one system read that offers both
  readings of a stop on one queue, drain by default.**
- **Java SE 25** (`verified`), `BlockingQueue`,
  https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/concurrent/BlockingQueue.html,
  read 2026-11-22: *"A `BlockingQueue` does _not_ intrinsically support any kind of "close" or
  "shutdown" operation to indicate that no more items will be added."*; *"a common tactic is for
  producers to insert special _end-of-stream_ or _poison_ objects"*. `ExecutorService`,
  https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/concurrent/ExecutorService.html:
  `shutdown()`, *"Initiates an orderly shutdown in which previously submitted tasks are executed, but
  no new tasks will be accepted. Invocation has no additional effect if already shut down."*;
  `shutdownNow()`, *"Attempts to stop all actively executing tasks, halts the processing of waiting
  tasks, and returns a list of the tasks that were awaiting execution."*, *"Returns: list of tasks that
  never commenced execution"*.
- **Erlang** (`verified`), *Processes* reference,
  https://www.erlang.org/doc/system/ref_man_processes.html, OTP 29.1.2, read 2026-11-22: a process that
  traps exits has the exit signal *"converted to a message signal and added to the end of the message
  queue of the receiver"*, as `{'EXIT', SenderID, Reason}`; a receiver not trapping exits *"is
  terminated"* for a reason other than `normal`; *"if an entity sends multiple signals to the same
  destination entity, the order is preserved"*, with an exception for priority messages. **EEP 76,
  *Priority Messages***, https://www.erlang.org/eeps/eep-0076 (Rickard Green, created 2025-01-07,
  *"Final/28.0 Implemented in OTP release 28"*), read 2026-11-22: *"Sending it a message informing
  about the long message queue will not work."*; the case of wanting to *"terminate the worker prior to
  having to handle all other requests"*; *"the priority message will overtake all ordinary messages in
  the message queue"*; *"Since the receiver process needs to opt-in in order to get any special
  handling of priority messages, this will be completely backwards compatible."* **The language that
  7.13 cites queues a trapped exit behind the messages already there, and in OTP 28 (EEP 76) added a
  way for a receiver to opt in to a stop that overtakes them.** *Supervisor*,
  https://www.erlang.org/doc/apps/stdlib/supervisor.html, OTP 29.1.2: *"`brutal_kill` means that the
  child process is unconditionally terminated using `exit_signal(Child,kill)`"*; with an integer, the
  child is sent `shutdown` and killed if *"no exit signal is received within the specified number of
  milliseconds"*.

**Where the proposal departs**:
- **S6 against Go**: `closed` as an answer, where Go panics. It sides with Rust, Kotlin and Python.
  The departure is not named.
- **S6 with S11, unnamed and, I think, unanswered**: Rust's failed send **returns the value**
  (`unwrap_err().0`), and Kotlin passes it to `onUndeliveredElement` *"to close a resource"*. Under
  S11, `m.send(@v)` on a closed mailbox has already emptied `v`, and no row says whether the handle
  goes back, is ended, or is told at `main`'s end.
- **S8 against Go**: a second close does nothing, where Go panics. It sides with Kotlin and Java's
  `shutdown()`. Not named.
- **S7's stop**: it sides with Python's `immediate=True`, Kotlin's prompt cancellation and EEP 76,
  and against Erlang's trapped exits and Python's default. **What no precedent read does**: make the
  overtaking stop the only stop, process-wide, with the overtaken values neither returned (Java) nor
  passed to the program (Kotlin). Not named in S7.

### Q4. Unbounded against bounded (S6)

- **Go** (`verified`): the specification (Q3's URL), *"If the capacity is zero or absent, the channel
  is unbuffered and communication succeeds only when both a sender and receiver are ready. Otherwise,
  the channel is buffered and communication succeeds without blocking if the buffer is not full (sends)
  or not empty (receives)."* Go has no unbounded channel. **Issue 20352**, *"proposal: spec: add
  support for unlimited capacity channels"*, https://github.com/golang/go/issues/20352, read
  2026-11-22: opened by rgooch on 2017-05-13; cznic the same day, *"Unlimited capacity channels ask for
  a machine with unlimited memory."*; ianlancetaylor on 2018-02-13, *"most programs do need
  backpressure"*, an *"attractive nuisance"*; griesemer on 2018-02-14, *"Using an unlimited channel for
  this scenario is simply shifting the problem elsewhere"*; closed by ianlancetaylor on 2018-02-13,
  *"Closing on the assumption that we will get some type of generics."* (labels `LanguageChange`,
  `Proposal`, `Proposal-Hold`, `v2`). **Go was asked for S6's queue and did not take it into the
  language.**
- **Erlang** (`verified`): **EEP 42**, *"setrlimit(2) analogue for Erlang"*,
  https://www.erlang.org/eeps/eep-0042 (Richard A. O'Keefe, created 2013-02-07, status *"Draft"*,
  *"None in this draft"* under its reference implementation), read 2026-11-22: *"It is also possible
  for the message queue of an Erlang process to grow without limit."*; *"If the message queue length
  limit is exceeded, the process is exited with reason `message_queue_len`."* The page records no
  acceptance. The efficiency guide (Q1) says only that searching is *"very expensive for processes
  with long message queues"*.
- **Rust** (`verified`), `std::sync::mpsc`, https://doc.rust-lang.org/std/sync/mpsc/index.html, std
  1.100.0, read 2026-11-22: `channel`, *"An asynchronous, infinitely buffered channel"*, *"The channel
  conceptually has an infinite buffer."*; `sync_channel`, *"A synchronous, bounded channel"*, *"a bound
  of 0 is allowed, causing the channel to become a "rendezvous" channel"*.
- **Tokio 1.54.1** (`verified`), https://docs.rs/tokio/latest/tokio/sync/mpsc/index.html, read
  2026-11-22: of the bounded channel, *"In other words, the channel provides backpressure"*;
  `unbounded_channel`, *"Creates an unbounded mpsc channel for communicating between asynchronous tasks
  without backpressure."*
- **crossbeam-channel**, **Kotlin**, **Python**, **Java**, **Nim 1.x** (`verified`, URLs in Q1 and
  Q3), all read 2026-11-22. crossbeam: `bounded`, `unbounded` and a zero-capacity channel, which
  *"cannot hold any messages"*. Kotlin: `Channel.UNLIMITED` *"creates a channel with an unlimited
  buffer, which never suspends the sender"*, beside `RENDEZVOUS`, `CONFLATED` and `BUFFERED`. Python:
  *"If _maxsize_ is less than or equal to zero, the queue size is infinite."* (the default is 0, and so
  is the subinterpreter `Queue`'s). Java: *"A `BlockingQueue` may be capacity bounded."* Nim 1.x:
  *"For unlimited queue set maxItems to 0."* **Dart** (*"The send happens immediately"*) and
  **Racket** (*"place-channel-put calls are non-blocking"*) give no bounded form on the pages read.

**What the pages read say the choice costs**: memory (cznic's line, EEP 42) and backpressure (Go's
maintainers, Tokio). **No page read states in its own words the deadlock a bounded send admits**:
searched for on the Go specification, the Go tour, Rust's `mpsc`, crossbeam's and Tokio's module
pages. **A pattern across the table (inference)**: the systems that copy into isolated heaps (Erlang,
Dart, Racket, Python's subinterpreters, Nim 1.x by default) never make a send wait, or do so only by
choice, while Go, which shares memory, refused the unbounded queue. S6 sits with the first family,
and so with 7.13's *"appends to the tail"*. It departs from the issue's own sketch (*"a bounded
send"*), which § 5's question 3 names.

### Q5. The life of the queue (S5)

- **Go** (`verified`): *A Tour of Go* (Q3's URL), *"Channels aren't like files; you don't usually
  need to close them. Closing is only necessary when the receiver must be told there are no more values
  coming, such as to terminate a `range` loop."* The GC guide, https://go.dev/doc/gc-guide, read
  2026-11-22: *"garbage collection refers to **tracing** garbage collection, which identifies in-use,
  so-called **live**, objects by following pointers transitively."* **Go 1.26 release notes**,
  https://go.dev/doc/go1.26 (*"February 2026"*), read 2026-11-22: a leaked goroutine is *"a goroutine
  blocked on some concurrency primitive (channels, `sync.Mutex`, `sync.Cond`, etc) that cannot possibly
  become unblocked."*; *"The runtime detects leaked goroutines using the garbage collector: if a
  goroutine G is blocked on concurrency primitive P, and P is unreachable from any runnable goroutine or
  any goroutine that _those_ could unblock, then P cannot be unblocked, so goroutine G can never wake
  up."*; *"the runtime may fail to identify leaks caused by blocking on concurrency primitives
  reachable through global variables or the local variables of runnable goroutines."*; enabled by
  `GOEXPERIMENT=goroutineleakprofile`; *"We aim to enable goroutine leak profiles by default in Go
  1.27."* **That is question 1's *"a worker blocked in `receive` forever"*, and Go's answer is to find
  it by reachability** (when Go's channels first shipped is on no page read today).
- **Rust** (`verified`): the Book (Q2's URL), *"A channel is said to be _closed_ if either the
  transmitter or receiver half is dropped."*; `mpsc`'s *Unbounded receive loop* example, *"Drop the last
  sender to stop `rx` waiting for message. The program will not complete if we comment this out.
  **All** `tx` needs to be dropped for `rx` to have `Err`."*; the counter,
  https://raw.githubusercontent.com/rust-lang/rust/master/library/std/src/sync/mpmc/counter.rs, read
  2026-11-22: `destroy` is *"Set to `true` if the last sender or the last receiver reference
  deallocates the channel."*; `Sender::release`, *"Function `disconnect` will be called if this is the
  last sender reference."* (and likewise the receiver). The reader's account of the code: the channel's
  memory is freed only after both sides have released.
- **Racket** (`verified`, Q1's URL): *"Place channels are subject to garbage collection, like other
  Racket values."* The reader's account: a thread blocked reading a place channel can be collected once
  the writing end is unreachable, and threads mutually blocked on place channels reachable only from
  them are treated as reachable.
- **Python, Dart, Erlang**: Python's subinterpreter `Queue` has no close (*"except for `task_done()`,
  `join()` and `shutdown()`"*); Dart's `ReceivePort.close` page says only *"Closes the receive port."*
  (what pending messages and senders see is not on it); Erlang's mailbox belongs to its process (*"A
  process with no more code to execute terminates normally."*), with no close.

**Which is closest to S5 (inference, from the pages above)**: **Go's channel**, one value that both
sends and receives, closed separately, with S5's count of copies in place of Go's tracer. The
difference is what makes the forgotten close cost more. Go's tracer can (since 1.26, as an
experiment) **find** a receiver blocked on a channel nobody else reaches. A count cannot: the blocked
worker's own copy keeps the count above zero, so the queue never goes, and nothing is told
(*"nothing is told at `main`'s end for a mailbox itself"*). **Rust's split halves** avoid it by
construction: the end of the last `Sender` is the close, and `recv` then answers `Err`. **S5's
reading of the brief's question 1 therefore has no precedent read that tells the program anything.**
Panel 251's handle text (*"one nobody closes aborts at `main`'s end"*) is the one alternative on the
table that tells something. No system read aborts over an unclosed channel; the nearest is Go 1.26's
profile, which reports and does not abort.

### Q6. Timed receive and `timeout` (S7)

- **Erlang** (`verified`), *Expressions*, https://www.erlang.org/doc/system/expressions.html, OTP
  29.1.2, read 2026-11-22: *"`ExprT` is to evaluate to an integer, or the atom `infinity`."*; *"The
  allowed integer range is from 0 to 4294967295, that is, the longest possible timeout is almost 50
  days."*; *"With a zero value, the timeout occurs immediately if there is no matching message in the
  message queue."* *Errors and Error Handling*, https://www.erlang.org/doc/system/errors.html:
  `timeout_value`, *"The timeout value in a `receive...after` expression is evaluated to something other
  than an integer or `infinity`."* (what a negative integer raises is not stated on either page).
- **Rust** (`verified`), `Receiver::recv_timeout` (Q3's URL), *"returning an error if the corresponding
  channel has hung up, or if it waits more than `timeout`"*, the example showing
  `Err(mpsc::RecvTimeoutError::Timeout)`; stable since 1.12.0.
- **Java** (`verified`), `BlockingQueue.poll(long timeout, TimeUnit unit)` (Q3's URL): *"Retrieves and
  removes the head of this queue, waiting up to the specified wait time if necessary for an element to
  become available."*, returning *"`null` if the specified waiting time elapses before an element is
  available"*, throwing `InterruptedException` *"if interrupted while waiting"*.
- **Python** (`verified`): `Queue.get` (Q3's URL), *"If _timeout_ is a positive number, it blocks at
  most _timeout_ seconds and raises the `Empty` exception if no item was available within that
  time."*; the page is silent on a negative one, and CPython's `queue.py` holds `elif timeout < 0:
  raise ValueError("'timeout' must be a non-negative number")`.
- **POSIX and Linux** (`verified`): `poll`, IEEE Std 1003.1-2024,
  https://pubs.opengroup.org/onlinepubs/9799919799/functions/poll.html, read 2026-11-22: *"If the
  timeout argument is -1 for poll() or a null pointer for ppoll(), then the call shall block
  indefinitely"*; the reader found no sentence for another negative value. Linux `poll(2)`,
  https://man7.org/linux/man-pages/man2/poll.2.html (man-pages 6.19, 2026-02-08): *"Specifying a
  negative value in timeout means an infinite timeout."*
- **Go** (`verified`): `select` (Q3) with `time.After`, https://pkg.go.dev/time#After, go1.27.4, read
  2026-11-22: *"After waits for the duration to elapse and then sends the current time on the returned
  channel. It is equivalent to NewTimer(d).C."*; *"As of Go 1.23, the garbage collector can recover
  unreferenced, unstopped timers."*

**Where S7 stands**: `-1` for no limit is POSIX's `poll`. S7's abort below `-1` makes a rule where
POSIX is silent, and departs from Linux, which reads every negative value as infinite. It sides with
CPython's `ValueError` and Erlang's stated range. Rust, Java and Python mark no limit with a separate
call or `None`, and Erlang with an atom, not with a number. Not named in S7; no departure of
consequence.

### Q7. A channel inside its own queue (question 11)

- **Go** (`verified`, GC guide above): live objects are those found *"by following pointers
  transitively"* from *"the program's **roots**"*. The guide does not state the general rule for
  cycles; it names a cycle only as a finalizer hazard: *"if the object is part of a reference cycle it
  can never be freed"* (for an object with a finalizer). That a channel holding itself and reachable
  from no root is collected is an **inference** from the reachability definition, `unverified` as a
  sentence of Go's.
- **Rust** (`verified`): `Arc`, https://doc.rust-lang.org/std/sync/struct.Arc.html, std 1.100.0, read
  2026-11-22: *"A cycle between `Arc` pointers will never be deallocated. For this reason, `Weak` is used
  to break cycles."* The channel's list flavour,
  https://raw.githubusercontent.com/rust-lang/rust/master/library/std/src/sync/mpmc/list.rs, read
  2026-11-22: `disconnect_receivers`, *"// If receivers are dropped first, discard all messages to
  free"* *"// memory eagerly."*; `discard_all_messages`, *"/// This method should only be called when
  all receivers are dropped."* **The source gives freeing memory as its reason, not cycles** (the
  reader looked). That this discard also frees a `Sender` sitting in its own channel's queue once the
  `Receiver` is gone is an **inference** from that code, not a sentence on the page.
- **Erlang** (`verified`, Q3's and Q6's URLs): *"Sending a message to a pid never fails, even if the pid
  identifies a non-existing process."*; *"If the receiver has terminated, the signal does not arrive"*;
  *"A process with no more code to execute terminates normally."* No page read says a pid held by
  anyone keeps a process alive. The mailbox's life is its process's, so a pid inside its own mailbox
  is no cycle of lifetimes; that is an inference from those sentences.
- **Pony** (`verified`, Q1's URL): a *"dedicated cycle detector actor"* scans for *"groups of blocked
  actors that reference each other but are unreachable from any alive actor"*. With `--ponynoblock`,
  *"dead actor cycles will never be collected, and their finalisers will not run."* **The one
  precedent read that met cycles among per-heap counted entities and had to add a collector for
  them.**
- **Racket** (`verified`, Q1): place channels are collected *"like other Racket values"*.
- **Nim 1.x** (`verified`, Q1): *"Channels cannot be passed between threads"*, so a channel inside its
  own message cannot be written; and *"message passing does not work with cyclic data structures."*
- **Dart**: `SendPort` may be sent (Q1). What a `SendPort` inside its own port's queue costs is on no
  page read: `unverified`.
- **Refused by any system?** Nim 1.x refuses the precondition (a channel inside a message). **No page
  read refuses the cycle itself.** Searched: the Go GC guide, Rust's `Arc` and `mpsc` pages and
  `list.rs`, Erlang's processes reference, Pony's GC appendix, Racket's places, Nim's channels and
  isolation pages, Dart's `SendPort` and `ReceivePort`.

**Reading (inference)**: each precedent read that lets a channel ride in its own queue has one of
three things. Go and Racket trace. Rust separates the ends and discards the queue when the reading
end goes. Erlang ties the queue to a process, not to who holds its name. S5 has a count of copies
and none of the three. Pony's page is the warning: per-heap counting without a cycle collector
*"will never"* collect a dead cycle. Question 11's leak is real by every precedent read, and its
known repairs are a tracer, split ends, an owner (Erlang's process, panel 251's handle), or a refusal
when compiled (none read).

### Q8. A process-wide stop and queued replies (question 12)

- **Go** (`verified`): `signal.NotifyContext`, https://pkg.go.dev/os/signal#NotifyContext, go1.27.4,
  read 2026-11-22: the returned context is marked done *"when one of the listed signals arrives"*, its
  `Done` channel closed; *"The stop function releases resources associated with it."* So a
  process-wide signal becomes one context. A receive of a reply against that context is a `select`,
  chosen *"via a uniform pseudo-random selection"* when both are ready (Q3): **a queued reply may win
  or lose.**
- **Kotlin** (`verified`, Q3): cancellation outranks even an element already taken (*"even if receive
  managed to retrieve the element from the channel, but was cancelled while suspended,
  CancellationException will be thrown"*), and the element goes to `onUndeliveredElement`, so that
  *"Resources are never lost."* The pages read do not say that one cancellation reaches every receive
  in a process; that is a question, not a premise.
- **Python** (`verified`, Q3): `shutdown(immediate=True)` is per queue, and the drained items go to
  nobody (CPython's `while self._qsize(): self._get()`).
- **Java** (`verified`, Q3): `shutdownNow()` is per executor and **returns** the waiting tasks.
- **Erlang** (`verified`, Q3): a supervisor's `shutdown` to a worker that traps exits arrives *"to
  the end of the message queue"*, behind the replies already there. A worker that does not trap exits
  is terminated. EEP 76 makes overtaking opt-in, per receiver, through an alias the receiver hands to
  senders it trusts.

**Answer to the brief's question 8**: **no system read has a shutdown signal that makes every channel
receive in the process fail at once.** Searched: Go's `os/signal`, Kotlin's `receive` and `Channel`,
Python's `queue`, Java's `ExecutorService` and `BlockingQueue`, Erlang's processes reference,
supervisor and EEP 76, Tokio's `select!`. Every stop read that overtakes a queue is scoped (a queue,
an executor, a coroutine, a receiver that opted in) and hands the overtaken values somewhere:
returned (Java), to a callback (Kotlin), or discarded by a call the program made on that queue
(Python). **S12 with S7 departs from all of them**: process-wide, permanent, the reply already
queued in a worker's reply mailbox unreachable to the waiter. **Not named in the proposal.**

---

## argument

Every piece of route B has a working ancestor. Erlang, Dart, Nim 1.x and Python 3.14 copy values
between isolated heaps; HTML standardises the flat record. Rust refuses raw pointers and the types
holding them. Pony, Rust and Nim 2 check a move when compiled, and Node moves a socket between
threads. Kotlin and Python close, then drain. Four unnamed departures remain. JavaScript refuses
callables. Rust returns a value a closed channel refused; Kotlin passes it to a callback. Every stop
read that overtakes queued values is scoped to one queue, executor or receiver. Go answers S5's
forgotten close with a reachability leak profile, Rust with split ends; a count of copies has
neither. (114 words, counted by hand)

## condition

- **A system whose shutdown fails every receive in the process, permanently, and drops queued
  replies**, on a page, would make S7 and S12 a precedent rather than a departure, and lift objection
  2.
- **A runtime that counts copies of a bidirectional channel and tells or frees a channel left inside
  its own queue** (or one that refuses it when compiled) would give S5 an ancestor and lift objection
  3.
- **A copying system that sends closures** across heaps that share no code, or the compiler-engineer
  showing that a Heroes function value carries no captured state, would lift objection 4.
- **A precedent for a moved handle being ended in silence when a send is refused** would turn
  objection 1 into a choice between precedents rather than a gap.
- **Nim's own reason for moving from deep-copying channels to `Isolated[T]`**, if a page gives it
  (none read did), would say whether 7.13's unconditional copy met a cost Nim paid.

## search

**Queries (WebSearch, standard mode)**, each run 2026-11-22:
1. *Nim channels_builtin "deep copied" thread-local heap channel send message documentation*
2. *Dart 2.15 isolate groups "Isolate.exit" message passing faster deep copy jank announcement*: no
   announcement page surfaced; Dart's API pages were read instead.
3. *Pony garbage collection actors per-actor heap ORCA "cycle detector" tutorial ponylang*
4. *golang proposal unlimited capacity channels unbounded channel declined issue 20352*
5. *Go 1.26 goroutine leak profile goroutineleak GOEXPERIMENT garbage collector detects goroutines
   blocked on unreachable channels release notes*
6. *erlang documentation process message queue unbounded memory "max_heap_size" message queue
   "on_heap" "off_heap" message_queue_data*: it found EEP 42, not a current page on queue memory.
7. *Erlang OTP 28 priority messages EEP 76 priority alias receive ahead of message queue*

**Pages read (WebFetch), every one 2026-11-22**: Erlang's efficiency guide (processes), expressions,
processes reference, errors, supervisor, `gen_tcp`, EEP 42, EEP 76. Dart's `SendPort.send`,
`Isolate.exit`, `TransferableTypedData` and its `materialize`, `ReceivePort`, *Concurrency in Dart*.
MDN's structured clone and transferable objects. The HTML standard's *Safe passing of structured
data*. Node's `worker_threads` (twice). Nim's channels (1.0.0 and 1.4.0) and `std/isolation`.
Python's `concurrent.interpreters` and `queue`, and CPython's `Lib/queue.py`. Racket's places. OCaml
5.4's parallelism chapter. Pony's consume, reference capabilities and GC appendix. The Go
specification (offsets 0, 100000, 135000, 160000), the tour's `concurrency.article`, the GC guide,
the 1.26 release notes, `time.After`, `signal.NotifyContext`, issue 20352. Rust's Book 16.2, `mpsc`
(module, `Sender`, `Receiver`), `Arc` (first 100,000 of 210,443 characters), `thread::Builder`,
`JoinHandle`, the Rustonomicon's *Send and Sync*, and `library/std/src/sync/mpmc/list.rs` and
`counter.rs` at `master`. crossbeam-channel 0.5.17. Tokio 1.54.1 (`mpsc`, `Receiver`, `select!`).
Kotlin's `SendChannel.close`, `ReceiveChannel.receive`, `Channel`. Java SE 25 `BlockingQueue`,
`ExecutorService`. POSIX.1-2024 `poll`, Linux `poll(2)`.

**Not read, and why**:
- `go.dev/tour/concurrency/4` rendered empty; its source article was read instead.
- Swift's SE-0430 and `AsyncStream` were not re-read today (panel 251's readings, *carried*, not
  cited).
- Akka's mailboxes and .NET's `System.Threading.Channels` cancellation: not searched, for time.
  Whether .NET's `ReadAsync` lets a cancelled token outrank a queued item is a question.
- Erlang's binary-handling page (refc binaries' 64 bytes): design.md's citation stands, not re-read.
- The Dart 2.15 announcement: not found by query 2.
- Hylo, Cyclone, Oberon, Idris, Zig, K/APL and cfront for message passing: none is known to this seat
  to own the route. That is an assumption, not a finding, and no query was run for them.

---

## Premises found false or narrower than their pages (00-shared § 10)

1. **design.md 7.13, `design.md:4175`, *"The one industrial precedent"*** (cited by the brief § 4):
   today's pages show at least four industrial systems that copy at an isolation boundary. They are
   Dart (`SendPort.send`), JavaScript (HTML's structured clone), Python 3.14 (`concurrent.interpreters`)
   and Nim 1.x (deep-copying channels). Each but Nim 1.x also shares immutables or offers a transfer.
   The sentence's point, that the copy is not absolute even where isolation is famous, holds more
   strongly than it says. Its count does not.
2. **My brief, Q5, *"Rust's channel ends when every `Sender` or the `Receiver` is dropped"***: the
   channel **disconnects** when either side's last reference goes (the Book; `counter.rs`'s
   `release`). By the reader's account of `counter.rs`, its **memory is freed only when both sides
   have released**. The two are different events, and the difference is what Q7 turns on.
3. **My brief, Q5, *"Go's channel is garbage-collected"***: true by the GC guide's definition. No
   Go page read says it of channels in those words; the 1.26 notes' *"P is unreachable"* is the
   nearest.
4. **My brief, Q6, *"Go's `select` with `time.After`"***: correct. It omits that a `select` between a
   value and a stop picks *"uniform pseudo-random"*, which is the precedent that bears on S7's order.
5. The brief's other precedent claims checked true on the pages: Go's drain then zero value and
   `false`; Go's panic on closing twice; Rust and crossbeam draining then `Disconnected`/`Err`; Tokio's
   `close` then drain; Erlang's unbounded mailbox (EEP 42's *"grow without limit"*); Dart's
   `TransferableTypedData`; Rust's move on `send`; Java's `poll(timeout, unit)`.

## Predictions (each scoreable later)

- **P1**: under S5 as written, a run of question 11's program (a record holding `[(mailbox R)]` sent
  into its own mailbox, every copy the program holds then dropped) will show the queue's block still
  allocated at exit, read by the runtime's leak gate or LeakSanitizer as one block or more.
  *Checkable* when the compiler-engineer or the critic builds it on route B.
- **P2**: the blind seat's t2 session on *"a send after the close"* will predict a run-time abort (Go's
  rule) more often than the `closed` answer S6 states, on arm `m`. *Checkable* when the
  llm-ergonomist's t2 results are written.
- **P3**: no seat finds a shipped system whose stop is process-wide and permanent and also fails a
  receive with a reply already queued. *Checkable* at the synthesis, against every seat's report.
