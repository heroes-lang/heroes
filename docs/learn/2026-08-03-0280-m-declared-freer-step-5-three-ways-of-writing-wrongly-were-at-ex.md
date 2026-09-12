- [ ] **M-declared-freer step 5** | Three ways of writing `owned` wrongly were `internal error` at exit 2, and the probe could catch none of them. Read `check/freer.hero`'s `refusal_for_mark` and say, for each of `owned bogus_name`, `owned my_heroes_function` and `owned two_parameter_extern`, which check catches it and why a probe structurally could not

    **Where to look:** selfhost/check/freer.hero § mark_refusals, refusal_for_mark · selfhost/emit/extern_probe.hero
    **Why it matters:** a probe is emitted per `extern`, so a name nobody declares has no probe — the shape of the instrument decides what it can never see
