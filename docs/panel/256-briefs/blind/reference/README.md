# Panel 256, the blind seat's reference programs (the critic's first reading, finding 26)

Written 2026-11-22 by the sitting's coordinator, before the registration: one program per arm that the row
calls right, its language half checked by a compiler that implements the arm.

- `t1/main_w.hero`, `t1/main_m.hero`, beside stubs of the page's `net`, `time` and `poll` (`t1/*.hero`):
  `heroes-next check` (the frozen commit's stage 2, `sources 031aaabb358218fd`) exit 0 for both. The stubs
  stand for the language half only: their `wait` never sleeps, so the programs are checked, not run.
- `t3/main_gi.hero`: `heroes-gi2 check` (the compiler-engineer's gi build, `sources 0e8d15027f0726af`,
  copied to the coordinator's folder) exit 0; built and run, `counter: 42`, `closer: true`, exit 0.
- `t3/main_gc.hero`: `heroes-sw` with `P256_route()=gc` (the compiler-engineer's switch build, `sources
  5cafe2523b320e66`) exit 0; built by it and run, `counter: 42`, `closer: true`, exit 0.
- `t3/main_gr.hero`: `heroes-sw` with `P256_route()=gr` exit 0, and the frozen compiler exit 0; built by the
  frozen compiler and run, `counter: 42`, `closer: true`, exit 0.
