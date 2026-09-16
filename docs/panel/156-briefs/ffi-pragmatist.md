# Panel 156 — ffi-pragmatist brief

Read `docs/panel/156-briefs/00-shared.md` first.

You judge the founding constraint — design.md §1.11 and §4.19, everything comes
from C — and you have a veto on ABI breakage. **This sitting is at your boundary
more than any other seat's**: the fault happens inside a C function, the handler
is C, and the three platforms' answers are three C runtimes' answers.

## Your half

**R4, Windows, and it is the half where the language breaks its own written
promise.** `spec § 6` says an abort *ends the program at once, saying why*. On
Windows it says nothing: both streams empty, per CI. The shared brief's reading
of `runtime/parts/stack.c:567-590` is that no arm reads
`ExceptionInformation[1]`, so an access violation with a valid PC falls to
`EXCEPTION_CONTINUE_SEARCH`.

**THE BOX IS UP AND THE MEASUREMENT IS ALREADY TAKEN.** The author powered it
during this sitting's preparation and the coordinator ran the fixture there:
**exit 139, stdout empty, stderr empty, `Segmentation fault`**. The tree is
already unpacked at `/c/w/p156` with `heroes.exe` built, reachable as
`ssh win 'cd /c/w/p156 && …'`. That is not a missing blame line — it is the
state this fixture's comment calls the thing §1.12 forbids by name, which
defect 045 was filed to end. **So your half is now a repair to build and
measure, not a reading.** What you are asked:

1. **Is that reading correct?** Read the arms yourself. If it is, write the
   third arm and say exactly what it should test. `EXCEPTION_ACCESS_VIOLATION`'s
   `ExceptionInformation[0]` is the access type (0 read, 1 write, 8 execute) and
   `[1]` is the address touched. Compare `[1]` against `HERO_NULL_WINDOW` as the
   POSIX arm compares `si_addr`.
2. **Build the third arm and measure it on the box.** Work in `/c/w/p156` over
   `ssh win`, which already holds the tree and a built `heroes.exe`; a rebuild
   of the seed there is about a minute, so change `runtime/parts/stack.c`, send
   the one file with `scp` or a heredoc, rebuild, and run the fixture. Report
   the exit code and both streams before and after. **Do not shut the box
   down** — it is billed by the hour and the author powers it.
3. **The asymmetry worth naming**: the POSIX arm reads the PC for the
   null-function-pointer case and `si_addr` for the null-read case. The Windows
   arm reads only `ExceptionAddress`. Is the Windows side simply missing the
   second witness, or is there a reason the platform makes it different?

## The question that outranks the rest if the answer is yes

**Can a null read through a C boundary produce a wrong answer at exit 0 rather
than an abort, on any of the three platforms?** The fixture's own comment says
it can at `-O2`:

> At `-O2` clang is entitled to assume the pointer is non-null and folds the
> fault away: the same program measured there prints a value for a node that
> does not exist and exits 0.

Panel 154 closed that with `-fno-delete-null-pointer-checks` in the flag list.
**Verify that flag is actually in the list and actually reaches this program**,
and measure the `-O2` behaviour today. If a wrong answer at exit 0 is
reproducible, this is §1.12 and it outranks every other question in this
sitting — say so and it decides.

## Also yours

**R3, the flush.** `fflush` is not async-signal-safe. The handler ends in
`abort()`. Whether the `7` should survive is R3's question; whether it CAN
survive safely is yours. Compile the alternatives rather than describing them:
an unbuffered stdout, a `write`-based `print`, a flush before the message, or
nothing. Say which are safe in a handler and which merely usually work.

## Process

Build in a copy; `rm -rf target build` after `cp -r`. Seed:
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, a few seconds.
**No command over ~60 seconds** — three seats died on the watchdog at panel 155.
Never read `archive/bootstrap-rs/`. Capture exit codes directly.

## Your verdict owes

A verdict per R1-R5 insofar as they touch C, the C you compiled and what it did,
what you left UNRUN and why, a falsifiable prediction, your condition, and
whether you cast your veto.

Write your report to `docs/panel/156-reports/ffi-pragmatist.md` **first**.
