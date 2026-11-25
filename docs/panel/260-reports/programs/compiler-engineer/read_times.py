#!/usr/bin/env python3
"""read_times.py <file> [prefix]: the table of a timing file written by time.sh (Mac) or run-linux.sh (Linux).

A line is `<program> <level> <label> <mode> <round> <real> <user> <sys> exit=<c> load=<l>` (Linux's open with
`time `, given as the prefix). Rounds 2 and up are kept; the best `real` per program, level, label and mode is
printed with its own `user`+`sys`, the median `user`+`sys` of the kept rounds, the loads read and every exit, and
each row's ratio to `old` and to `base` of the same program, level and mode, by `real` and by `user`+`sys`. A run
whose `real` stands above twice its `user`+`sys` plus one second waited, and is dropped and counted.
"""
import statistics, sys

path = sys.argv[1]
prefix = sys.argv[2] if len(sys.argv) > 2 else ""
rows = {}
dropped = {}
for line in open(path):
    if line.startswith("#") or (prefix and not line.startswith(prefix)):
        continue
    w = line.split()
    if prefix:
        w = w[1:]
    if len(w) < 10:
        continue
    pn, lv, label, mode, rnd = w[0], w[1], w[2], w[3], w[4]
    if mode not in ("warm", "cold") or int(rnd) < 2:
        continue
    try:
        real, user, sys_ = float(w[5]), float(w[6]), float(w[7])
    except ValueError:
        continue
    code = w[8].split("=")[1]
    load = w[9].split("=")[1]
    if real > 2 * (user + sys_) + 1.0:
        dropped[(pn, lv, mode, label)] = dropped.get((pn, lv, mode, label), 0) + 1
        continue
    rows.setdefault((pn, lv, mode), {}).setdefault(label, []).append((real, user + sys_, code, load))
print("| program | level | mode | compiler | best real (its user+sys) | median user+sys | kept (dropped) | exits | load | real / old | u+s / old | real / base | u+s / base |")
print("|---|---|---|---|---|---|---|---|---|---|---|---|---|")
for (pn, lv, mode) in sorted(rows):
    group = rows[(pn, lv, mode)]
    best = {l: min(v) for l, v in group.items()}
    for label in ("old", "base", "route", "mine"):
        if label not in group:
            continue
        r, us, _, _ = best[label]
        med = statistics.median(x[1] for x in group[label])
        exits = ",".join(sorted({x[2] for x in group[label]}))
        loads = f"{min(float(x[3]) for x in group[label]):.0f}-{max(float(x[3]) for x in group[label]):.0f}"
        def ratio(other):
            if other not in best or other == label:
                return "", ""
            o = best[other]
            return f"{r / o[0]:.2f}", f"{us / o[1]:.2f}"
        ro, uo = ratio("old")
        rb, ub = ratio("base")
        print(f"| {pn} | {lv} | {mode} | {label} | {r:.2f} ({us:.2f}) | {med:.2f} | {len(group[label])} ({dropped.get((pn, lv, mode, label), 0)}) | {exits} | {loads} | {ro} | {uo} | {rb} | {ub} |")
