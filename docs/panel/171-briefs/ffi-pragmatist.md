# Panel 171 — brief for the ffi-pragmatist

Read `docs/panel/171-briefs/00-shared.md` first. The direction is the author's;
you judge whether a binding author can live with it, and whether the word can be
TRUE.

## Your axis

design.md §1.11: everything comes from C. Veto on ABI breakage. And your own
finding from panel 170, which this sitting rests on: **977 functions across
SQLite, raylib and curl, 542 pointer parameters, zero classifiable as retaining
from the header**. Under the flip every one of those 542 is assumed to keep until
the author says otherwise.

## The part only you do

**Rewrite the bindings this repository already has, under the flip, and count.**

1. `examples/ledger/db/sqlite.hero` — four `.cstr()` lends at `:226`, `:271`,
   `:286`, `:342`, into `sqlite3_open`, `sqlite3_exec`, `sqlite3_prepare_v2`,
   `sqlite3_bind_text`. Which of those four functions truthfully does not keep?
   `sqlite3_bind_text` keeps or not **by its fifth argument** — the ledger passes
   `SQLITE_TRANSIENT`, so it copies. **Can the word be written on that
   parameter truthfully?** Say what a binding author does with an API whose
   retention is per call.
2. `examples/sqlite/main.hero`, `examples/curl/main.hero` and the other eight
   `.cstr()` sites in `examples/` — list every extern function they reach and
   say for each, from the real header and the real library's documentation,
   whether it keeps.
3. **The nine compiler bindings** in `selfhost/cli/process.hero:42-53` and
   `selfhost/emit/literal.hero:42` — `hero_fs_*`, `hero_dir_scan`, `getenv`,
   `atof`. All within-call? `getenv` returns a pointer INTO the environment;
   does it keep its argument? Say so from the standard, not from memory.
4. **Is there a real API where the truthful answer is "sometimes"** and the word
   can therefore never be written honestly? `curl_easy_setopt` is variadic;
   `sqlite3_bind_*` decides by argument. Name them and say what the author
   writes there — a lease, presumably, and say what that costs them.
5. **Does anything break ABI?** The mark is compile-time; the emitted call
   should be byte-identical. Prove it with `--emit-c`.
6. **Register a falsifiable prediction** with an instrument that exists today.

Copy named for your seat, `git log -1` at `b6e26fcc` or later. Never rebuild
from `selfhost/`. Report to `docs/panel/171-reports/ffi-pragmatist.md`. Veto on
ABI breakage.
