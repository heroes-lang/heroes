# Panel 154 — brief for the ffi-pragmatist

Read `00-shared.md` first. You judge the founding constraint (design.md §1.11,
§1.12, §4.19). You have a veto on ABI breakage.

**The measurement this sitting rests on and nobody has taken: how often does a
real C library accept NULL for a pointer parameter?** Route A refuses every such
call and route D guesses from the marks. Go and count, on this Mac's own SDK.
Take at least `sqlite3.h`, `curl/curl.h`, `netdb.h`, `stdlib.h`, `stdio.h`,
`dirent.h` and `time.h`; for each pointer parameter say whether the documented
contract permits NULL, name where you read it, and give the totals. **Write and
compile the C** for the cases that matter: `freeaddrinfo(NULL)`,
`sqlite3_close(NULL)`, `free(NULL)`, `fclose(NULL)` — run each and report the
exit code, because "legal" is a claim and some of these are undefined rather than
permitted.

**Then price each route against the five shipped bindings.** `examples/sqlite/`,
`examples/ledger/db/sqlite.hero`, `examples/curl/`: does any route change one
byte of the C they emit, or refuse a call they make today? Build each before and
after reasoning about it.

**And test route D's premise directly**: is a `consumes` parameter more likely to
accept NULL than an unmarked one, in the headers you counted? That is the whole
of D and nobody has checked it.

One prediction with the command that scores it. Every compile's exit code and
first stderr line goes in the report.
