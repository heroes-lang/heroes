# Defect 210's recovery arm: two runs of plans that are not the plan read over

`base/` and `head/` are each the records of a run of `heroes mutate --recovery`
that judged nothing (no single, no pair) and replayed a plan whose digests are
none of `tests/golden/recovery`'s, and not each other's, so neither stales when
that plan is pinned again. `suite_surface`'s rows read them three ways: a run
folder that already holds something is refused before anything runs, `--compare`
without `--recovery` is refused by name, and the two read against each other
over the frozen plan are refused for replaying another plan (lane b14-harness-b,
2026-10-07; the arm is lane b14-mutate's, merged at the round).
