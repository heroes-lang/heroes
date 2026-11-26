# Task t2: six programs, each built once

You have the specification of the language Heroes (`spec.md`) and the folder `programs/`. Nothing else is yours: no
other file of this folder but those this task names, no tool but Read and Write.

`programs/` holds six programs, `p1/` to `p6/`. Each is built from its own folder with `heroes build main.hero`, and each
uses one package, whose folder stands beside `main.hero` and which the program's catalogue `heroes.toml` names. Every
file of them, by its path under `programs/`:

- `p1/`: `main.hero`, `heroes.toml`, `ticks/clock.hero`
- `p2/`: `main.hero`, `heroes.toml`, `ticks/clock.hero`
- `p3/`: `main.hero`, `heroes.toml`, `ticks/clock.hero`, `ticks/raw.hero`
- `p4/`: `main.hero`, `heroes.toml`, `ticks/clock.hero`
- `p5/`: `main.hero`, `heroes.toml`, `ticks/clock.hero`, `ticks/myclock.h`
- `p6/`: `main.hero`, `heroes.toml`, `num/ops.hero`

The C headers they name and do not hold are the language's and the system's: `hero_time.h` declares `int64_t
hero_time_monotonic_ns(void);`, nanoseconds on a clock that never goes back; `hero_os.h` declares `int64_t
hero_os_pid(void);`, the running process's id; `stdlib.h` is C's own, and declares `int abs(int);`.

## For each program, `p1` to `p6`

- **builds** or **refused**: whether `heroes build main.hero`, run in its folder, writes a program or refuses it;
- for a refusal, **the file the compiler's message names**, as a path from the program's folder (`main.hero`,
  `ticks/clock.hero`, ...).

Then, once, in the structure `brief.md` gives: every place where the specification left you a choice, the choice you
made, and what the other would produce.
