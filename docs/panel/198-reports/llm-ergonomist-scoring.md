# Panel 198, llm-ergonomist (blind seat): the coordinator's scoring of the six readings

Written by the coordinator at 10:52 on 2026-10-09 (`date`). Six fresh sessions,
`claude-opus-5-5`, from 10:47 to 10:50, 1.5732 USD together of the 6 the author
approved (each `run.json`'s `total_cost_usd`). Each answer applied where its
world exists, as the brief says: in `heroes-linux-arm64:sdl3-b14` (SDL 3.2.10
built from source, Debian's libcurl), the frozen compiler built from
`56def9b4`'s seed, the reader's own build command, the binary under `timeout 10`.

| folder | variant | way out | build | run prints | weakens the list |
|---|---|---|---|---|---|
| `u1-k` | K, the (A) note | `link "SDL3"` and the note's `--include`/`--library` (and `lent` on the two `@event` parameters) | 0 | `true true true 42 true` | no |
| `u1-m` | M, today's message | `link "SDL3"` | 0 | `true true true 42 true` | no |
| `u1-r` | R, the (G) note | `link "SDL3"`, the allowance considered and refused | 0 | `true true true 42 true` | no |
| `u2-k` | K, the (A) note | `link "curl"` and the note's three `--include`s | 0 | `libcurl/8.14.1 OpenSSL/3.5.7 ...` | no |
| `u2-m` | M, today's message | `link "curl"` | 0 | `libcurl/8.14.1 OpenSSL/3.5.7 ...` | no |
| `u2-r` | R, the (G) note | `link "curl"`, the allowance considered and refused | 0 | `libcurl/8.14.1 OpenSSL/3.5.7 ...` | no |

**Every reading builds and runs, and no reader weakens the list in any
variant**: no `.pc` file edited, no `pkg-config` wrapper, no word passed on.
The three t2 readers also rewrote `validated(c: curl_version())` as
`curl_version().validated()`, the spec giving the built-in's form and not its
parameter's name; both forms build. Every `context` named the folder and the
harness's environment information, no project rule.

**Against the prediction** (`llm-ergonomist-prediction.md`, written before any
session): clause 1 held in its first and third halves (both M readers took
`link`, neither edited a `.pc`) and was falsified in its second (no M reader
wrote a `--include` it did not need); clause 2 held for both K readers'
`--include` and `--library`, and its loader half did not appear (the machine
had run `ldconfig`, which `machine.txt` says); **clause 3 was falsified**: both
R readers weighed `--allow-word` and took `link`, `u2-r` because the allowance
means *accepting a flag the compiler blocks on purpose as a code-execution risk
(CVE-2018-6574)*, and `u1-r` to keep the build command as given; clause 4 held.

**What it says, and what not.** Today's message already leads a reader to the
safe way out on both tasks, so the readings do not discriminate K's note from
M's; what they show is that (G)'s allowance, offered beside `link`, was not
taken, its own note's CVE clause turning the readers away. They measure a
reader repairing a refused build, not a package author or a later reader of a
route that builds with no message at all (K's own case, where nobody reads a
note).
