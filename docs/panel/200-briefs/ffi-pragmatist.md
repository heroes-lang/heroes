# Panel 200, ffi-pragmatist's brief

Read `00-shared.md` beside this file first. Your seat writes and runs the C.

- **Q2 (465), yours first**: build, in your copy's `runtime/parts/run.c` (and
  the program's start, if a route needs it), the routes that end a child on
  macOS when the `heroes` that started it is killed with SIGKILL: (a) a
  sentinel process per runner holding a pipe; (b) a watcher inside the started
  program, a thread on `kqueue` `EVFILT_PROC` with `NOTE_EXIT` on its parent's
  pid, armed by the runtime's start only when the program was started by a
  runner (say how it knows); (c) any other you find. For each: does the child
  end 10 of 10 when its `heroes` is `kill -9`ed mid-run (a program that loops
  writing to `/dev/null`, bounded by `timeout`); what it costs a run that ends
  normally (instructions, `/usr/bin/time -l`), what it adds to every program
  or every runner (a thread, a process, a file descriptor), and what it does
  on Linux and Windows (none, or which arm). A route that leaves a process or
  a thread behind a normal exit is refused by its own measurement.
- **Q1 (453)**: the flags `-fsyntax-only` must take to read the artifact as
  the build would (the build's own words, a package's `--cflags`); one
  artifact clang refuses, made by hand, as the case.
- **Q4 (472)**: on the Windows box if it answers (`ssh win`), the debugger's
  view of a `step` with and without the prologue's line, if the
  compiler-engineer's variant reaches you; otherwise say it is unrun.

Report per question: verdict, the C you wrote and ran, the numbers, what you
could not run.
