# Panel 256, the historian (advisory, no veto)

Written 2026-11-22 from about 22:40, as it went. Read first, whole: `256-briefs/historian.md`,
`256-briefs/00-shared.md`, `256-reports/critic-briefs.md`, and the agent file at the frozen commit
(`lane-p256/.claude/agents/historian.md`; the seat's own copy `p256-scratch/historian/` does not exist,
`Read` of it answered *File does not exist*, and this seat builds and runs nothing, so none was needed).

**Discipline.** Every line below quotes a page fetched today (2026-11-22) with `WebFetch`, its URL beside
it. A line says what the page states and nothing of what Heroes might do about it; which of the sitting's
routes it bears on is said separately, after the quotation, as this seat's reading. Anything not confirmed
by a page read today says *unverified*. Searches sent the names of languages and mechanisms only. The
fetch tool returns a small model's extraction of the page, not the raw bytes; each quotation below is what
it returned as verbatim, and where it said it paraphrased, the line says *paraphrased by the fetch*. One
source was read with this seat's own eyes, page by page: Stroustrup's P0557r1, pages 1 to 3 (the PDF the
fetch saved, opened with `Read`).

The routes named below are the shared brief's: Q1 **P1.1** (a mailbox a source a wait takes, rung by the
send, level-triggered), **P1.2** (a set inside the UI's wait), **P1.3** (the scoped stop as a server's own
mailbox; SIGTERM counted), **P1.4** (the waker, two acts); the unlisted **descriptor** route (the queue
hands out a descriptor, as `fs/watch`), the **forwarder**, the **one wait in the runtime**, a **stop with
a grace**, the **stop token**, the **pump**, a **language-level select**. Q2 **gc**, **gi**, **gr**, a
written **bound**, **after `mono`**, the obligation **inferred and shown**.

## Q1: one wait for everything a program waits on

### Erlang: the socket's data as messages, and the flow-control lesson (verified)

- `inet` reference, https://www.erlang.org/doc/apps/kernel/inet.html (read 2026-11-22):
  *"If the value is `true`, which is the default, everything received from the socket is sent as messages
  to the receiving process."* *"If the value is `once` (`{active, once}`), one data message from the
  socket is sent to the process. To receive one more message, setopts/2 must be called again with option
  {active, once}."* And the lesson, in the page's words: *"Notice that `{active, true}` mode provides no
  flow control; a fast sender can easily overflow the receiver with incoming messages."* *"The same is
  true for `{active, N}` mode, while the message count is greater than zero."* *"Use active mode only if
  your high-level protocol provides its own flow control (for example, acknowledging received messages) or
  the amount of data exchanged is small."* *"Once the socket's message count reaches `0` ... the process
  is then notified by a special message, specific to the type of socket, that the socket has entered
  passive mode."*
- `gen_tcp` reference, https://www.erlang.org/doc/apps/kernel/gen_tcp.html (read 2026-11-22): *"The
  controlling process is the process that the socket sends messages to."* *"`{tcp, Socket, Data}` -
  Inbound data from the socket."* *"Any process can receive data from a passive socket, even if that
  process is not the controlling process of the socket."* (Paraphrased by the fetch: when an `{active,
  N}` socket's count runs out the owner receives `{tcp_passive, Socket}`; on a change of owner, messages
  already queued may stay in the old owner's mailbox if another process uses the socket meanwhile.)
- **When `{active, N}` came**: Erlang/OTP 17.0's page, https://www.erlang.org/patches/OTP-17.0, as the
  search engine quoted it: *"Add the {active,N} socket option for TCP, UDP, and SCTP, where N is an
  integer in the range -32768..32767"* (the search's excerpt; **the page itself not fetched**, so the
  release *unverified* by a read of its own). The patch's announcement,
  https://erlang.org/pipermail/erlang-patches/2013-September/004249.html (read 2026-11-22), Steve
  Vinoski, 4 September 2013: *"This pull request adds support for an {active,N} option for TCP, UDP, and
  SCTP sockets."* *"It's similar to {active,true} except the socket enters passive mode once N messages
  have been sent"*. The page does not state why `once` was insufficient; **the motivation is
  unverified** (it points to a commit message the page does not hold).
- **Bears on**: the **pump** (the system's readiness delivered into the one queue a process waits on) and
  on any route that delivers without the receiver asking: the page's own warning is that delivery without
  a per-message re-arm has no flow control.

### Go: the poller under the scheduler, `context`, `Shutdown` (verified; `select` unverified)

- The runtime's poller, https://raw.githubusercontent.com/golang/go/master/src/runtime/netpoll.go (read
  2026-11-22): *"Integrated network poller (platform-independent part)."* A platform defines
  `netpollopen`, *"Arm edge-triggered notifications for fd."*, `netpoll`, *"Poll the network. If delta <
  0, block indefinitely."*, and `netpollBreak`, *"Wake up the network poller, assumed to be blocked in
  netpoll."* `pollDesc` *"contains 2 binary semaphores, rg and wg, to park reader and writer"*, with
  states *"pdReady - io readiness notification is pending;"* and *"pdWait - a goroutine prepares to park
  on the semaphore, but not yet parked;"*.
- Its Linux waker, https://raw.githubusercontent.com/golang/go/master/src/runtime/netpoll_epoll.go (read
  2026-11-22): `netpollEventFd uintptr // eventfd for netpollBreak`, made by `linux.Eventfd(0,
  linux.EFD_CLOEXEC|linux.EFD_NONBLOCK)`; and the coalescing, in the code's own comment: *"Failing to cas
  indicates there is an in-flight wakeup, so we're done here."* before `if
  !netpollWakeSig.CompareAndSwap(0, 1) {`.
- `context`, https://pkg.go.dev/context (read 2026-11-22): *"Package context defines the Context type,
  which carries deadlines, cancellation signals, and other request-scoped values"*; *"Done returns a
  channel that's closed when work done on behalf of this context should be canceled."*; *"Done is
  provided for use in select statements"*; *"Do not store Contexts inside a struct type; instead, pass a
  Context explicitly to each function that needs it."*
- **What a blocked system call honours**: `net.Conn`, https://pkg.go.dev/net#Conn (read 2026-11-22):
  *"The deadline applies to all future and pending I/O, not just the immediately following call to Read
  or Write."*; `SetReadDeadline` *"sets the deadline for future Read calls and any currently-blocked Read
  call."*; `Close`: *"Any blocked Read or Write operations will be unblocked and return errors."* No
  `Read` of `net.Conn` takes a `Context` (the method list read on that page; a search for proposals to add
  one found issues 20280, 21423, 36973 on golang.org, **none fetched, their status unverified**).
- `http.Server.Shutdown`, https://pkg.go.dev/net/http#Server.Shutdown (read 2026-11-22, from offset
  160000): *"Shutdown gracefully shuts down the server without interrupting any active connections."*
  *"Shutdown works by first closing all open listeners, then closing all idle connections, and then
  waiting indefinitely for connections to return to idle and then shut down."* *"If the provided context
  expires before the shutdown is complete, Shutdown returns the context's error"*. **And the half that
  bears on feature 2660's WebSocket case**: *"Shutdown does not attempt to close nor wait for hijacked
  connections such as WebSockets. The caller of Shutdown should separately notify such long-lived
  connections of shutdown and wait for them to close, if desired."* `RegisterOnShutdown`: *"This can be
  used to gracefully shutdown connections that have undergone ALPN protocol upgrade or that have been
  hijacked. This function should start protocol-specific graceful shutdown, but should not wait for
  shutdown to complete."* *"Once Shutdown has been called on a server, it may not be reused"*.
- **When**: Go 1.8's notes, https://go.dev/doc/go1.8 (read 2026-11-22): *"The HTTP Server now has support
  for graceful shutdown using the new Server.Shutdown method and abrupt shutdown using the new
  Server.Close method."* *"The new Server.Shutdown takes a context argument."* (The page gives no date;
  the year is *unverified* here.)
- **`select`**: the spec's section, https://go.dev/ref/spec#Select_statements, was **not read**: the
  fetch stopped at 100,000 of 263,947 characters, before it, and offered the wording from its own memory,
  which is not a reading. *Unverified* today, and so is anything said of how a channel send wakes a
  goroutine parked in a `select`.
- **Bears on**: the **one wait in the runtime** (Go's poller is the scheduler's, its waker an `eventfd`
  with a compare-and-swap that coalesces); the **stop token** and the **scoped stop** (Go's stop is a
  value per server and per request, `Shutdown(ctx)`, not process-wide; a blocked `Read` is ended by a
  deadline or a close, never by the `Context` itself); and 2660's first case (Go's own graceful stop
  leaves a WebSocket's close to the program, through a registered function).

### Rust: `mio::Waker`, `tokio::sync::Notify`, `CancellationToken` (verified)

- `mio::Waker`, https://docs.rs/mio/latest/mio/struct.Waker.html (read 2026-11-22): *"Waker allows
  cross-thread waking of `Poll`."* *"`Waker` events are only guaranteed to be delivered while the `Waker`
  value is alive."* *"Only a single `Waker` can be active per `Poll`."* *"What happens if multiple
  `Waker`s are registered with the same `Poll` is unspecified."* *"On platforms that support kqueue this
  will use the `EVFILT_USER` event filter."* *"On Linux it uses eventfd."* (Windows is not described on
  that page; its mechanism there is *unverified*.)
- `tokio::sync::Notify`, https://docs.rs/tokio/latest/tokio/sync/struct.Notify.html (read 2026-11-22),
  the lost wake-up answered by a stored permit: *"Otherwise, a permit is stored in this Notify value and
  the next call to notified().await will complete immediately"*; *"At most one permit may be stored by
  Notify."*; and of `notify_waiters`: *"Unlike with notify_one(), no permit is stored to be used by the
  next call to notified().await."*
- `tokio_util::sync::CancellationToken`,
  https://docs.rs/tokio-util/latest/tokio_util/sync/struct.CancellationToken.html (read 2026-11-22):
  *"A token which can be used to signal a cancellation request to one or more tasks."* *"Tasks can call
  CancellationToken::cancelled() in order to obtain a Future which will be resolved when cancellation is
  requested."* `child_token`: *"Creates a CancellationToken which will get cancelled whenever the current
  token gets cancelled."* *"Unlike a cloned CancellationToken, cancelling a child token does not cancel
  the parent token."* Its example waits in `tokio::select!` on `cloned_token.cancelled()` beside a sleep.
- **Bears on**: **P1.4** (mio's waker is 2693's waker, the same mechanisms 2693 names for Darwin and
  Linux, one per `Poll`); the **lost wake-up** (Notify's permit is a ring that survives being rung before
  the wait); the **stop token** (a scoped, tree-shaped stop, a child cancelled by its parent and never the
  reverse).

### libuv, GLib: a loop woken from another thread (verified)

- `uv_async_t`, https://docs.libuv.org/en/v1.x/async.html (read 2026-11-22): *"Async handles allow the
  user to “wakeup” the event loop and get a callback called from another thread."* *"It’s safe to call
  this function from any thread."* *"libuv will coalesce calls to uv_async_send(), that is, not every call
  to it will yield an execution of the callback."* *"If uv_async_send() is called again after the
  callback was called, it will be called again."*
- `g_main_context_wakeup`, https://docs.gtk.org/glib/method.MainContext.wakeup.html (read 2026-11-22):
  *"Wake up context if it’s currently blocking in g_main_context_iteration(), causing it to stop
  blocking."* *"Otherwise, if context is not currently blocking, this function causes the next invocation
  of g_main_context_iteration() to return without blocking."* *"Another related use for this function is
  when implementing a main loop with a termination condition, computed from multiple threads"*; its
  example loops `while (g_atomic_int_get (&tasks_remaining) != 0) g_main_context_iteration (NULL, TRUE);`
  and a thread calls `g_main_context_wakeup (NULL)` after `g_atomic_int_dec_and_test`.
- `g_unix_fd_add_full`, https://docs.gtk.org/glib-unix/func.fd_add_full.html (read 2026-11-22): *"Sets a
  function to be called when the IO condition, as specified by `condition` becomes true for `fd`."* (The
  page says nothing of level or edge triggering; *unverified*.)
- **Bears on**: **P1.4** and TH01 (both are a ring from any thread, coalesced, the state the ring
  announces read by the woken loop from shared memory: GLib's own example changes the counter, the state,
  before the wake, and its loop re-reads it); **P1.2** (GLib's loop takes a descriptor, which is how a
  `kqueue` or `epoll` descriptor would join it; the brief's *"a `kqueue` or `epoll` descriptor is itself
  pollable"* is the brief's, not checked here).

### Win32: one wait over handles and the message queue, and its documented lost wake-up (verified)

- `MsgWaitForMultipleObjectsEx`,
  https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-msgwaitformultipleobjectsex
  (read 2026-11-22): *"Waits until one or all of the specified objects are in the signaled state, an I/O
  completion routine or asynchronous procedure call (APC) is queued to the thread, or the time-out
  interval elapses. The array of objects can include input event objects"*; *"The maximum number of
  object handles is MAXIMUM_WAIT_OBJECTS minus one."* The handle kinds it lists: *"Change notification,
  Console input, Event, Memory resource notification, Mutex, Process, Semaphore, Thread, Waitable timer"*
  (no socket). **The lost wake-up, in Microsoft's words**: *"MsgWaitForMultipleObjectsEx does not return
  if there is unread input of the specified type in the message queue after the thread has called a
  function to check the queue, unless you use the MWMO_INPUTAVAILABLE flag."* *"If this flag is not used,
  the existing unread input (received prior to the last time the thread checked the queue) is ignored."*
  And `MWMO_INPUTAVAILABLE`: *"The function returns if input exists for the queue, even if the input has
  been seen (but not removed) using a call to another function, such as PeekMessage."*
- `WSAEventSelect`, https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsaeventselect
  (read 2026-11-22), the bridge from a socket to a waitable event: *"The WSAEventSelect function specifies
  an event object to be associated with the specified set of FD_XXX network events."* *"For FD_READ,
  FD_OOB, and FD_ACCEPT network events, network event recording and event object signaling are
  level-triggered."* *"no further actions are taken for that network event until the application makes
  the function call that implicitly reenables the setting of that network event"*; *"The WSAEventSelect
  function automatically sets socket s to nonblocking mode"*; *"It is not possible to specify different
  event objects for different network events."*
- **Bears on**: **P1.2**'s open Windows half (Windows' own one wait over the UI's queue and kernel objects
  takes an event, and a socket reaches it through `WSAEventSelect`, not as itself; the 63-handle ceiling
  is the page's `MAXIMUM_WAIT_OBJECTS minus one`, the constant's value *unverified* here); the **lost
  wake-up** (a wait that reports only what is new since the queue was last looked at ignores what was seen
  and not taken, and the page's remedy is a flag that asks whether input *exists*); P1.1's
  *"level-triggered as every entry is"*.

### Python, eventfd, the self-pipe: a queue that hands out a descriptor (verified, one unverified)

- `signal.set_wakeup_fd`, https://docs.python.org/3/library/signal.html#signal.set_wakeup_fd (read
  2026-11-22): *"When a signal your program has registered a signal handler for is received, the signal
  number is written as a single byte into the fd."* *"This can be used by a library to wakeup a poll or
  select call, allowing the signal to be fully processed."* *"If not -1, fd must be non-blocking. It is up
  to the library to remove any bytes from fd before calling poll or select again."* And the lesson of
  losing signals in a full pipe: *"if too many signals arrive too quickly, then the buffer may become
  full, and some signals may be lost."* *"In the second approach, we use the wakeup fd only for wakeups,
  and ignore the actual byte values. In this case, all we care about is whether the fd's buffer is empty
  or non-empty; a full buffer doesn't indicate a problem at all."* *"Changed in version 3.7: Added
  `warn_on_full_buffer` parameter."*
- `eventfd(2)`, https://man7.org/linux/man-pages/man2/eventfd.2.html (read 2026-11-22): *"creates an
  "eventfd object" that can be used as an event wait/notify mechanism"*; *"A write(2) call adds the 8-byte
  integer value supplied in its buffer to the counter."*; readable *"if the counter has a value greater
  than 0."*; *"Applications can use an eventfd file descriptor instead of a pipe ... in all cases where a
  pipe is used simply to signal events."* History: *"Linux 2.6.22, glibc 2.8."*
- D. J. Bernstein's *The self-pipe trick*, https://cr.yp.to/docs/selfpipe.html: **unverified**, the
  fetch failed twice (*"Parse Error: Content-Length can't be present with Transfer-Encoding"*); only a
  search engine's summary was read, which is not a reading.
- **Bears on**: the **descriptor** route (a readiness a poller can hold whose truth is a counter or a
  buffer's non-emptiness, so level-triggered by construction, and coalescing for free); Python's page
  states the trade the descriptor route meets if its bytes carry meaning.

### A scoped stop: Java's interrupt, .NET's token (verified)

- `Thread.interrupt()`,
  https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Thread.html#interrupt() (read
  2026-11-22): *"If this thread is blocked in an I/O operation upon an InterruptibleChannel then the
  channel will be closed, the thread's interrupt status will be set, and the thread will receive a
  ClosedByInterruptException."* *"If this thread is blocked in a Selector then the thread's interrupt
  status will be set and it will return immediately from the selection operation, possibly with a
  non-zero value, just as if the selector's wakeup method were invoked."* *"If none of the previous
  conditions hold then this thread's interrupt status will be set."*
- .NET, https://learn.microsoft.com/en-us/dotnet/standard/threading/cancellation-in-managed-threads (read
  2026-11-22): *".NET uses a unified model for cooperative cancellation of asynchronous or long-running
  synchronous operations."* *"Cancellation is cooperative and is not forced on the listener."* *"Listeners
  can be notified of cancellation requests by polling, callback registration, or waiting on wait
  handles."* *"Some operations can become blocked in such a way that they cannot check the value of the
  cancellation token in a timely manner. For these cases, you can register a callback method that
  unblocks the method when a cancellation request is received."* *"you can use the
  CancellationToken.WaitHandle property to enable the operation to wait on both the event and the
  cancellation request."* *"cancellation tokens cannot be reused after they have been canceled."* *"If an
  object supports more than one concurrent cancelable operation, pass a separate token as input to each
  distinct cancelable operation. That way, one operation can be cancelled without affecting the others."*
- **Bears on**: the **stop token** against the **process stop**, and **P1.3**. Three answers to *"what
  does a call blocked in the system do"*: Java closes the channel (the stop destroys the resource), Go
  sets a deadline or closes (the same, by the program's hand), .NET registers a callback that unblocks, or
  puts the token's own wait handle in the same wait. Java's stop is scoped to a thread, Go's and .NET's to
  operations; none of the three read is process-wide (a statement about these three pages, not a search
  for a process-wide stop elsewhere). .NET's *"wait on both the event and the cancellation request"* is
  P1.3's shape, the stop a source in the wait it ends.

### Not reached in the time box (Q1)

Qt (`QSocketNotifier`, `QCoreApplication::postEvent`), AppKit/CoreFoundation (`CFFileDescriptor`,
`CFRunLoopWakeUp`, `postEvent(_:atStart:)`), `PostMessageW`, Linux's `signalfd`, `asyncio`'s
`call_soon_threadsafe`, Erlang's ports (`open_port`): **not fetched**, nothing said of them here.

## Q2: a rule on what a type holds, asked of a generic function

### Rust: `Send` inferred from fields, `T: Send` written by a generic (verified; two unverified)

- `Send`, https://doc.rust-lang.org/std/marker/trait.Send.html (read 2026-11-22): *"Types that can be
  transferred across thread boundaries."* *"This trait is automatically implemented when the compiler
  determines it's appropriate."* Declared `pub unsafe auto trait Send { }`. *"An example of a non-`Send`
  type is the reference-counting pointer `rc::Rc`."*
- The Reference's auto traits, https://doc.rust-lang.org/reference/special-types-and-traits.html (read
  2026-11-22): *"The Send, Sync, Unpin, UnwindSafe, and RefUnwindSafe traits are auto traits."* *"If no
  explicit implementation or negative implementation is written out for an auto trait for a given type,
  then the compiler implements it automatically according to the following rules"*: *"`&T`, `&mut T`,
  `*const T`, `*mut T`, `[T; n]`, and `[T]` implement the trait if `T` does."* *"Structs, enums, unions,
  and tuples implement the trait if all of their fields do."*
- `std::thread::spawn`, https://doc.rust-lang.org/std/thread/fn.spawn.html (read 2026-11-22): `pub fn
  spawn<F, T>(f: F) -> JoinHandle<T> where F: FnOnce() -> T + Send + 'static, T: Send + 'static,`; *"The
  Send constraint is because the closure will need to be passed by value from the thread where it is
  spawned to the new thread. Its return value will need to be passed from the new thread to the thread
  where it is joined."*
- **Where the error is told**: the Book, https://doc.rust-lang.org/book/ch10-01-syntax.html (read
  2026-11-22), for a bound missing on a letter (`PartialOrd`, not `Send`): `error[E0369]: binary
  operation `>` cannot be applied to type `&T`` pointing into the body, then `help: consider restricting
  type parameter `T` with trait `PartialOrd`` showing the signature `fn largest<T: std::cmp::PartialOrd>`;
  the text: *"this error states that the body of `largest` won't work for all possible types that `T`
  could be."* That the same shape holds for a missing `Send` is **unverified** by a page read today (a
  search found forum threads, none fetched). That a bare `T` is moved, not copied, inside a generic body
  is **unverified** too (the error index's E0382 page, read, holds no generic example).
- **Bears on**: the written **bound** (a property inferred from fields for a concrete type and written
  for a letter); **gr** (the body judged once against what the letter is declared to allow, the error in
  the body); the obligation **shown in the signature** (the help line writes the bound into it).

### Swift: `Sendable`, generic parameters, and the migration (verified)

- SE-0302, https://github.com/swiftlang/swift-evolution/blob/main/proposals/0302-concurrent-value-and-concurrent-closures.md
  (read 2026-11-22): *"Implemented (Swift 5.7)"*. *"The core of this proposal is a marker protocol
  defined in the Swift standard library"*. Non-public structs and enums get *"the `Sendable` conformance
  ... implicitly provided"*; *"implicit conformance to `Sendable` is only available for non-generic
  types."* *"Generic types conditionally conform to the `Sendable` protocol"*; *"Public non-frozen structs
  and enums do not get an implicit conformance"*. Its example error, told at the type's declaration:
  *"MyPair cannot conform to Sendable due to 'T' member which may not itself be a Sendable."* (The page
  holds no sentence about a generic *function* sending a `T`.)
- SE-0337, https://github.com/swiftlang/swift-evolution/blob/main/proposals/0337-support-incremental-migration-to-concurrency-checking.md
  (read 2026-11-22): *"Implemented (Swift 5.6)"*; *"None of this code specifies any of its concurrency
  behavior in a way that `Sendable` checking can understand"*; *"a special kind of import, `@preconcurrency
  import`, which silences or downgrades these warnings."*
- Swift 6, https://www.swift.org/blog/announcing-swift-6/ (read 2026-11-22), 17 September 2024:
  *"Swift 6 now includes a new, opt-in language mode that extends Swift's safety guarantees to prevent
  data races"*.
- **Bears on**: the written **bound** and the obligation **inferred and shown** (inferred for a
  non-public, non-generic type; for a generic one, written as a condition); and on what a late rule cost a
  language with users: a migration attribute and an opt-in mode. (Swift's migration guide,
  https://www.swift.org/migration/documentation/swift-6-concurrency-migration-guide/commonproblems,
  rendered nothing to the fetch; its source on GitHub was read and holds no sentence on a letter; nothing
  is said of it here.)

### C++: templates judged at instantiation, and why concepts came (verified, read page by page)

- Stroustrup, *Concepts: The Future of Generic Programming*, P0557r1, 31 January 2017,
  https://www.stroustrup.com/good_concepts.pdf (read 2026-11-22, pages 1 to 3): *"In about 1987, I tried
  to design templates with proper interfaces [Str94]. I failed."*; what came instead, *"Lousy interfaces
  (basically compile-time duck typing)"*; *"The lack of well-specified interfaces led to the
  spectacularly bad error messages we saw over the years."* Of an unconstrained `sort`: *"The
  requirements of sort on its argument type are implicit ("hidden") in its function body."* *"The error
  message for sort(d) will appear only when the template is instantiated, and that may be long after the
  point of call."* *"To use sort, we need to provide its definition, rather than just its declaration"*.
  With a concept: *"That message is generated immediately at the point where the compiler sees the
  erroneous call (sort(d))."* And the history: *"Many tried to find a solution, notably members of the C++
  standards committee aiming for C++0x"*; *"Together with Gabriel Dos Reis and Andrew Sutton, I started to
  design concepts from scratch in 2009"*; *"We now have a ISO TS ("Technical Specification") for
  concepts"*. That concepts entered C++20 is **unverified** by a page read today.
- **Bears on**: **gi** and **after `mono`** (a generic's legality decided per instance by what its body
  does, the requirement hidden in the body: the shape this paper names as the cause of the messages); and
  on the **bound** (the requirement written where the call can be judged against it).

### Ada: the contract model (verified)

- Annotated Ada Reference Manual 12.3, https://www.adaic.org/resources/add_content/standards/05aarm/html/AA-12-3.html
  (read 2026-11-22), Language Design Principles: *"The legality of an instance should be determinable
  without looking at the generic body."* *"Likewise, the legality of a generic body should be
  determinable without looking at any instances."* *"the generic_declaration forms a contract between the
  body and the instances"*. And its exception: *"In the visible part and formal part of an instance,
  Legality Rules are enforced at compile time"*, because *"we need to check the rules again in the visible
  part of the instance"*.
- Ada 95 Rationale § 12, https://www.aise.ics.saitama-u.ac.jp/~cheng/ADA/ada95rat/rat2_12.htm (read
  2026-11-22; a mirror, not the AdaIC copy): Ada 83 had *"a serious violation of the contract model
  because of the lack of distinction between unconstrained and constrained formal parameters."* Ada 95
  *"ensures that the legality of an instantiation never depends on the parameter usage patterns present
  in the generic body."* The principle: *"assume the "worst" in the body so that legality does not depend
  upon the instantiation."*
- **Bears on**: **gr** (assume the worst of a letter in the body) and the **bound** (the declaration the
  contract); **against gi**, whose refusal at a call depends on what the body does with the letter, the
  shape the Rationale calls Ada 83's violation.

### Standard ML: the restricted letter (verified, inference unverified)

- MLton's guide, *EqualityTypeVariable*, https://sources.debian.org/src/mlton/20100608-2/doc/guide/EqualityTypeVariable
  (read 2026-11-22; MLton's own site refused the fetch on its certificate): *"An equality type variable
  is a type variable that starts with two or more primes, as in ''a or ''b."* `fun f (x: ''a, y: ''a):
  bool = x = y` is accepted; `fun f (x: 'a, y: 'a): bool = x = y` is refused, MLton saying *"Function
  applied to incorrect argument."*, *"expects: [<equality>] * [<equality>]"*, *"but got: [<non-equality>]
  * [<non-equality>]"*.
- *PolymorphicEquality*, https://sources.debian.org/src/mlton/20100608-5/doc/guide/PolymorphicEquality
  (read 2026-11-22): `val = : ''a * ''a -> bool`; *"The ''a in the specification are equality type
  variables, and indicate that polymorphic equality can only be applied to values of an equality type."*
  *"So, 13.0 = 14.0 is not type correct."* *EqualityType*,
  https://sources.debian.org/src/mlton/20100608-5/doc/guide/EqualityType (read 2026-11-22): *"Notably,
  exn and real are not equality types."* *"Neither is t1 -> t2, for any t1 and t2."*
- That SML **infers** `''a` for an unannotated function that uses `=` (the textbook `member : ''a * ''a
  list -> bool`) is **unverified**: the page that states it (Gilmore's notes, at the University of Genoa's
  mirror and Edinburgh's) refused the connection or answered 404 today.
- **Bears on**: a route the brief lists only in part: **a restricted letter**, the property spelled in
  the letter's own name rather than as a trait or a `where`, the error told in the body against the
  letter's spelling. It is a form of the written **bound** with no constraint system, and, if the
  inference were verified, of the obligation **inferred and shown**.

### Pony: sendable capabilities on a generic (verified)

- Tutorial, *Passing and Sharing*, https://tutorial.ponylang.io/reference-capabilities/passing-and-sharing.html
  (read 2026-11-22): *"If you send an `iso` object to another actor, you will be giving up the ability to
  read from or write to that object."* *"you can freely send `val` objects to other actors, without
  needing to give up the ability to read from that object."* *"you can freely pass `tag` objects to other
  actors"*; of *"`trn`, `ref`, or `box`"*: *"That's because you can't do it."*
- *Constraints*, https://tutorial.ponylang.io/generics/generic-constraints.html (read 2026-11-22): `#send`,
  *"Anything you can send to an actor"*; `#share`, *"Anything you can send to more than one actor"*;
  *"Without the constraint, the generic must work for all possible capabilities."* And
  https://tutorial.ponylang.io/generics/generics-and-reference-capabilities.html (read 2026-11-22): *"For
  a generic class to compile it must be compilable for all possible types and reference capabilities"*.
- **Bears on**: **gr** (unconstrained, the body must hold for every capability) and the **bound**
  (`#send`, a class of capabilities a letter may be restricted to); **gc** (sending an `iso` gives up the
  sender's access, the page's words; whether Pony requires `consume` at that send is not on the pages
  read, *unverified*).

### Go: `comparable`, from static to a run-time check (verified)

- Go 1.20's notes, https://go.dev/doc/go1.20 (read 2026-11-22): *"Comparable types (such as ordinary
  interfaces) may now satisfy comparable constraints, even if the type arguments are not strictly
  comparable (comparison may panic at runtime). This makes it possible to instantiate a type parameter
  constrained by comparable (e.g., a type parameter for a user-defined generic map key) with a
  non-strictly comparable type argument such as an interface type, or a composite type containing an
  interface type."*
- Griesemer, *All your comparable types*, https://go.dev/blog/comparable, 17 February 2023 (read
  2026-11-22): before 1.20, in the post's account, *"comparable contains only types that the compiler
  guarantees will not panic with =="*; the cost, a generic map *"could be used just like a regular map
  type, but produced a compile-time error when `any` was used as the key type"*, and *"it prevented us
  from writing the kind of generic libraries we were hoping to write with generics in the first place."*
  *"Users recognized the problem early on and filed a multitude of issues and proposals in short order"*;
  *"after almost a year of publicly discussing numerous alternatives"*; the result: *"in Go 1.20, generic
  functions that rely on `comparable` are not statically type-safe anymore."* *"We have given up static
  type safety for a run-time check."*
- **Bears on**: **after `mono`** with a run-time guard, and panel 155's generic map key (the brief's
  `e2`): one language took a run-time panic for `==` reached through a type parameter, deliberately and
  after a year of discussion, and wrote down what it gave up. Also on the **bound** (Go has one,
  `comparable`, and still chose the run-time check for interfaces).

### Not reached in the time box (Q2)

Haskell and OCaml (type classes, equality and polymorphic comparison): **not fetched**, nothing said.

## The lines in the decision issue's form

Each is a precedent marked verified above; which route it bears on follows the colon's sentence, in
brackets, and is this seat's reading, not the page's.

**Q1** (routes of `00-shared.md` § 3.3):
- **Reached first by Erlang**: [inet, `{active, true | once | N}`](https://www.erlang.org/doc/apps/kernel/inet.html): a socket's data is sent as messages to the owning process; `{active, true}` *"provides no flow control; a fast sender can easily overflow the receiver"*, and `once` and `N` re-arm delivery. [the pump]
- **Reached first by Go**: [runtime/netpoll_epoll.go](https://raw.githubusercontent.com/golang/go/master/src/runtime/netpoll_epoll.go): the runtime's poller is woken through an `eventfd`, a compare-and-swap skipping the write when *"there is an in-flight wakeup"*. [the one wait in the runtime; P1.4's coalescing]
- **Reached first by Go**: [net/http Server.Shutdown](https://pkg.go.dev/net/http#Server.Shutdown): a graceful stop per server, bounded by a context; it *"does not attempt to close nor wait for hijacked connections such as WebSockets"*, left to `RegisterOnShutdown`. [P1.3, the stop token, feature 2660]
- **Reached first by Rust's mio**: [mio::Waker](https://docs.rs/mio/latest/mio/struct.Waker.html): cross-thread waking of a `Poll`, `EVFILT_USER` on kqueue and `eventfd` on Linux, one active per `Poll`. [P1.4]
- **Reached first by tokio**: [Notify](https://docs.rs/tokio/latest/tokio/sync/struct.Notify.html): a notification with no waiter stores one permit, so the next wait completes at once. [the lost wake-up; P1.1]
- **Reached first by libuv**: [uv_async_send](https://docs.libuv.org/en/v1.x/async.html): safe from any thread, and coalesced, *"not every call to it will yield an execution of the callback."* [P1.4, TH01]
- **Reached first by GLib**: [g_main_context_wakeup](https://docs.gtk.org/glib/method.MainContext.wakeup.html): wakes a blocked iteration or makes the next one return at once; its example changes shared state, then wakes. [TH01, P1.4]
- **Reached first by Win32**: [MsgWaitForMultipleObjectsEx](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-msgwaitformultipleobjectsex): input already seen and not removed is ignored unless `MWMO_INPUTAVAILABLE` asks whether input exists. [the lost wake-up, level against edge; P1.2's Windows half]
- **Reached first by Winsock**: [WSAEventSelect](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsaeventselect): a socket's readiness signals an event object, level-triggered for `FD_READ`, `FD_OOB` and `FD_ACCEPT`. [P1.2's Windows half]
- **Reached first by Linux**: [eventfd(2)](https://man7.org/linux/man-pages/man2/eventfd.2.html): a counter a write adds to, readable while it is above zero, usable *"instead of a pipe ... where a pipe is used simply to signal events"*. [the descriptor route]
- **Reached first by Python**: [signal.set_wakeup_fd](https://docs.python.org/3/library/signal.html#signal.set_wakeup_fd): a byte written to a non-blocking descriptor wakes a `poll` or `select`; used only for wakeups, *"a full buffer doesn't indicate a problem at all."* [the descriptor route]
- **Reached first by .NET**: [Cancellation in Managed Threads](https://learn.microsoft.com/en-us/dotnet/standard/threading/cancellation-in-managed-threads): cooperative, per operation; a blocked wait takes the token's `WaitHandle` beside its own event. [the stop token; P1.3]
- **Reached first by Java**: [Thread.interrupt()](https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Thread.html#interrupt()): a thread blocked on an `InterruptibleChannel` has the channel closed; one blocked in a `Selector` returns as if woken. [what a blocked call honours]

**Q2** (routes of `00-shared.md` § 4.5):
- **Reached first by Rust**: [Send](https://doc.rust-lang.org/std/marker/trait.Send.html) and [auto traits](https://doc.rust-lang.org/reference/special-types-and-traits.html): implemented automatically for a struct *"if all of their fields do"*; [thread::spawn](https://doc.rust-lang.org/std/thread/fn.spawn.html) writes `T: Send` on its letters. [the bound; gr]
- **Reached first by Swift**: [SE-0302](https://github.com/swiftlang/swift-evolution/blob/main/proposals/0302-concurrent-value-and-concurrent-closures.md): `Sendable` inferred only for non-public, non-generic types; generic types conform conditionally. [the bound; inferred and shown]
- **Reached first by C++**: [Stroustrup, P0557r1](https://www.stroustrup.com/good_concepts.pdf): an unconstrained template's requirements are *"hidden"* in its body and its error *"will appear only when the template is instantiated, and that may be long after the point of call"*. [against gi and after mono without a signature]
- **Reached first by Ada**: [AARM 12.3](https://www.adaic.org/resources/add_content/standards/05aarm/html/AA-12-3.html): *"The legality of an instance should be determinable without looking at the generic body"*, and the reverse. [gr, the bound; against gi]
- **Reached first by Standard ML**: [EqualityTypeVariable](https://sources.debian.org/src/mlton/20100608-2/doc/guide/EqualityTypeVariable): a letter written `''a` admits only equality types; `=` on a plain `'a` is refused. [a restricted letter]
- **Reached first by Pony**: [Constraints](https://tutorial.ponylang.io/generics/generic-constraints.html): `#send` is *"Anything you can send to an actor"*; *"Without the constraint, the generic must work for all possible capabilities."* [gr, the bound]
- **Reached first by Go**: [All your comparable types](https://go.dev/blog/comparable): Go 1.20 let interfaces satisfy `comparable`, *"We have given up static type safety for a run-time check."* [after mono with a run-time guard; panel 155's map key]

## Verdict

- `verdict`: **approve** (advisory), Q1 on the principle of P1.1 (the wait's truth a predicate on the
  queue, the send its ring) and Q2 on `gr` or a written bound, each with the condition below; **object**
  (advisory) to `gi` as the adopted reading if it is adopted without a signature that shows the
  obligation. Precedent does **not** refuse P1.4: mio, libuv and GLib ship exactly that waker.
- `precedents`: the sections above, each marked; verified: Erlang `inet`/`gen_tcp`, Go
  `netpoll`/`netpoll_epoll`/`context`/`net.Conn`/`Shutdown`/1.8 notes, mio `Waker`, tokio `Notify`,
  `CancellationToken`, libuv `uv_async_send`, GLib `g_main_context_wakeup`/`g_unix_fd_add_full`,
  `MsgWaitForMultipleObjectsEx`, `WSAEventSelect`, Python `set_wakeup_fd`, `eventfd(2)`, Java
  `Thread.interrupt`, .NET cancellation, Rust `Send`/auto traits/`spawn`/Book ch. 10, Swift SE-0302 and
  SE-0337 and Swift 6, Stroustrup P0557r1, AARM 12.3 and Ada 95 Rationale § 12, MLton's three guide
  pages, Pony's three tutorial pages, Go 1.20 and *All your comparable types*. Unverified: OTP 17.0's own
  page, the `{active, N}` motivation, Go's `select` spec and a channel's wake of it, Go's `net` proposals,
  Bernstein's self-pipe, mio on Windows, GLib's triggering, Rust's message for a missing `Send`, Rust's
  move of a bare `T`, C++20, SML's inference of `''a`, Pony's `consume` at a send.
- `argument`: Q1: the systems read fall in two kinds. Rings (mio, libuv, GLib, Go's `netpollBreak`)
  coalesce and leave the state where the woken loop re-reads it, GLib's example writing the state before
  the ring. Waits that answer a lost wake-up make their truth a predicate on what is queued: eventfd's
  counter, tokio's stored permit, Winsock's level-triggered `FD_READ`, Win32's `MWMO_INPUTAVAILABLE`
  against a loss its page documents. Every stop read is scoped, and Go's graceful stop leaves a WebSocket
  to the program. Q2: Stroustrup and Ada's contract model name per-instance legality decided by a body's
  hidden use as the failure; Rust, Pony and SML tell it in the body against the letter; Go chose a
  run-time check knowingly.
- `prediction` (not asked by this seat's agent file; given because the coordinator's message names it):
  a `gi` adopted with no signature showing the obligation will produce messages, told at a call, whose
  reason is a line inside the generic body, the shape P0557r1 describes; a P1.4 waker puts on every
  program the order GLib's example keeps by hand (state first, then the ring), and some program will
  break it. Both are inferences from the pages, unrun here.
- `condition`: Q1 would read otherwise if a system were found in which a wake rung by the program,
  separate from the state it announces, shipped without a documented lost wake-up and without a
  re-check of the state by the woken loop (looked for in: Erlang, Go, mio, tokio, libuv, GLib, Win32,
  Winsock, Python, eventfd, Java, .NET; not looked for in: Qt, CoreFoundation, `PostMessageW`, `signalfd`,
  `asyncio`). Q2 would read otherwise if a language were found that judges a generic per instance by what
  its body does, with no declared requirement, and whose designers call its messages adequate (looked for
  in: C++, Ada, Rust, Swift, SML, Pony, Go; not looked for in: Haskell, OCaml, D's templates, Zig's
  `comptime`, Nim's generics, each a place to look, none read here).
- `search`: the queries sent (names of languages and mechanisms only): *Erlang OTP 17 {active, N} socket
  option introduced tcp_passive release notes*; *golang issue net.Conn Read cannot be cancelled by context
  use SetReadDeadline proposal*; *"self-pipe trick" Bernstein select signal handler race write byte pipe*;
  *"cannot be sent between threads safely" "consider restricting type parameter `T`" "T: Send"
  thread::spawn error*; *Pony tutorial "#send" capability constraint generics iso val tag sendable*;
  *Standard ML equality type variable ''a "equality types" real function types not equality admits
  equality*; *Ada 95 Rationale generic "contract model" formal parameters body checked instantiation
  legality*; *SML "''a * ''a list -> bool" member equality type inferred*. Pages fetched: every URL above
  (read), plus refusals: cr.yp.to (parse error, twice), mlton.org (certificate), mlton.sourceforge.io (404,
  twice), Edinburgh's and Genoa's SML notes (404, connection refused), UW's slides (login), Swift's
  migration guide (rendered empty), the Rust Reference's impl-Trait page (no auto-trait sentence), the
  Rust error index E0382 (no generic example), Go's spec (stopped before `select`). *Heroes of code* was
  not used as a route this sitting, and nothing here rests on it.

**One line per question.**
- **Q1**: the precedent favours the wait's truth being a predicate on the queue, rung by the send itself
  (P1.1, or the descriptor route, which eventfd and Python's wakeup descriptor make level-triggered by
  construction), and a stop scoped to a server or an operation that is itself a source in the wait (P1.3,
  .NET's `WaitHandle` shape); P1.4 is not refused by precedent (mio, libuv, GLib ship it), only asked to
  carry the order GLib's example keeps by hand. The finding that would change it: a shipped
  program-rung waker, with no re-check of state, and no documented lost wake-up.
- **Q2**: the precedent favours telling the rule in the body against what the letter is declared to
  allow (`gr`, a written bound, or SML's restricted letter), never per instance by a body's hidden use
  (`gi` without a shown signature), with Go 1.20's run-time check as the one documented, deliberate
  fallback; the finding that would change it: a language that judges per instance with no declared
  requirement and whose designers call its messages adequate.
