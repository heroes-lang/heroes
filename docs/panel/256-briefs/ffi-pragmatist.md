# Panel 256, ffi-pragmatist

Read `00-shared.md` first, whole, then `docs/panel/256-reports/critic-briefs.md` (the critic's first
reading; its findings 2, 5, 12, 13, 14 and 28 bear on you). **The Windows box is not started in this
sitting** (`00-shared.md` § 2). Your copy is
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p256-scratch/ffi-pragmatist/`, a detached worktree
of the frozen commit; build its compiler from the seed and stage 2 there (§ 2 of the shared brief). Your
report: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-p256/docs/panel/256-reports/ffi-pragmatist.md`,
written as you go. **Compile, don't opine**: every mechanism you argue for is C you built and ran.

## Read

- design.md §1.11, §4.19, §1.12 (robustness), by grep;
- panel 246's R6 (a socket crosses as a number), panel 248's R2 (one poller) and R4 (`ask_stop`), panel
  254's R3, R4, R6 (`docs/panel/2{46,48,54}-*.md`);
- `runtime/parts/mail.c`, `runtime/platform/hero_poll.c` and `runtime/hero_poll.h`, `hero_net.c` (its head
  comment, `hero_net_nudge_all` at 897, `hero_net_number`), the three UI arms' waits
  (`hero_ui_win32.c:2421-2436`, `hero_ui_gtk.c:2270-2277`, `hero_ui_appkit.swift:1280`);
- Rebel's `poll.hero`, `poll.ledger`, `os.hero` (`catch_stop`, `ask_stop`, `count_signal`, `take_signals`),
  `ui.hero`'s header; the workarounds of `examples-rebel/chat/server/readers.hero` and
  `examples-rebel/notes/http/workers.hero`;
- `docs/platforms/linux/LINUX-MACHINE.md`: Docker answered `29.9.3 linux/aarch64` at 21:25, and the images
  `heroes-linux-arm64:gtk4-ui` and `heroes-linux-arm64:gtk4-web` stand (`docker images`); a container you
  start, you stop.

## Q1, first: one wait for everything

1. **The mailbox as an entry of a set** (P1 item 1), built in C on this Mac: the runtime's watch on a
   queue (a number resolved by slot and life, a ready predicate, a ring called outside the queue's lock and
   taking no lock of the layer's, R6's order), and the layer's entry (`kqueue`: `EVFILT_USER` or the set's
   own pipe; and in the Linux container `epoll` with an `eventfd`); level-triggered: told while the queue
   holds a value, is closed or answers `stopped`. **Stress it**: 4 and 16 sending threads, 100,000 sends
   each, one loop draining with `receive(within_ms: 0)`; count every value left in a queue while the loop
   slept past one second (a lost wake), plain, under ASan+UBSan and under TSan; the wake's latency.
2. **The waker** (P1 item 4, 2693's own sketch), built the same way, and the two mistakes its users can
   write, each run: the ring forgotten, and the ring put before the send, against a loop that takes one value
   per ring where rings are told together. Does a value sit unseen, and for how long?
3. **The stop scoped to a server** (P1 item 3 against 2660's `os.stop_token()`): build the smallest server
   whose loop, on a counted SIGTERM seen through `poll.add_signals`, stops accepting, finishes a closing
   exchange with a connected client under a deadline, while a second loop in the same process keeps serving;
   and price the token (which calls take it, what the layer must hold per token). **A call blocked outside
   any set** on another thread (a worker's read): what ends it under each route, and what R3 (a handle
   belongs to the thread holding it) forbids. **Windows**: `add_signals` is `unsupported` there
   (`poll.hero:286`); what the stop's loopback mechanism (`hero_poll.c:37-42`) would need to carry a count,
   priced from the code, the box being unreachable from here (say if it is not).
4. **The user interface's wait taking a set** (P1 item 2): on this Mac, whether AppKit's loop
   (`NSApp.nextEvent(matching:until:...)`) wakes on a `kqueue` descriptor through `CFFileDescriptor` or a
   dispatch source, built and run headless or with a window, as the machine allows; in the
   `heroes-linux-arm64:gtk4-ui` image, whether `g_main_context_iteration` wakes on an `epoll` descriptor
   through `g_unix_fd_add`; Win32's half priced from Microsoft's pages (`MsgWaitForMultipleObjectsEx` with a
   handle, `WSAEventSelect`), unrun unless you can run it.
4b. **The descriptor route** *(critic 12)*: `fs/watch.add_to` (`packages/rebel/0.1.0/files/fs/watch.hero:298-330`)
   already joins a C-side queue to a set by a pipe it rings (a loopback datagram socket on Windows), with no
   call from the runtime into the layer. Build the mailbox's version of it beside P1 item 1's watch and say
   which you would ship, by R6's words and by its measurement *(critic 28)*.
4c. **The Windows stop bell** *(critic 13, read in the code, unrun on Windows)*: `hero_poll.c:369-397` binds
   it to the loopback at a port the system chooses and never connects it; `:1294-1305` puts it in every
   set; `:1477-1479` and `:1504` read any readiness of it as the stop. So one datagram from any process on
   the machine would fail every later `poll.wait` of the program `stopped`, and P1 item 3 and 2693's waker
   would build on it. Confirm what you can on this Mac (the same logic compiled for POSIX does not exist:
   say what can and cannot be read without the box), and say what a repair needs (a connected pair, a
   check of the sender, a read of the stop flag).
5. **What 2688 and 2675 ask of "one wait"**: bytes a library holds above the socket. Is that the wait's
   question or each library's, by what you built?

## Q2, the C half

6. **A `ptr` crossing a thread through a generic** (§ 4.2's `p5`, `p7`, `p8` cross `nullptr`): build one that
   crosses a real allocation, one thread freeing what the other reads, plain, under ASan and under TSan.
   Does the class corrupt memory, and is that a soundness veto on any route that leaves it open?
7. **`gc`'s `@` for a letter's value that holds no handle**: does the emitted C change for an `i64` or a `str`
   instance? Compile what the compiler-engineer's question 4 would produce, if you can, or say what you would
   need.

End with the verdict structure of your agent file, then one line per question: the route you would adopt,
and the one measurement that would change your mind.
