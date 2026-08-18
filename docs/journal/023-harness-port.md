# 023 — M-harness-port: the net, in Heroes

**Open.** Sections grow per step; the closing block is appended at the close.

**Record note, step 1.** The six `tests/harness/*.hero` files, this journal and
step 1's `DESIGN-LOG` line were **committed by another session** as `92e8b00`
(*"site: it/author.html and it/panel.html"*), which ran `git add -A` while this
session held them staged: two Claude Code sessions were splitting the repository
that day, one on `site/` and one on the compiler. The history is not rewritten —
another session is working in it — so the commit that carries this step's subject
adds this note rather than the code, and `git log` reads correctly only with this
paragraph. The lesson is cheap and general: with two sessions in one tree, stage
and commit in the same breath, because a staged file belongs to whoever commits
first.

## Goal

Everything that asks whether this compiler is right is written in Rust and lives
under `crates/heroes-cli/tests/` — **4,711 lines** measured today (`surface` 2139 ·
`golden` 1095 · `corpus` 544 · `milestones` 319 · `differential` 296 ·
`seed` 202 · `layout` 116), and `expectation/mod.rs` adds 109 more. All of it dies with the
bootstrap, and panel 085 R4 measured what the archive would leave behind: nothing
runs `selfhost/`'s own tests, no selfhost test touches a golden, and **208** golden
cases go dark. This milestone rewrites the net in Heroes so that the archive is a
move rather than an amputation.

It is a program and not a subcommand: §10's stopping rule refuses the flag,
because two existing invocations — `heroes run`, and the compiler under test —
already compose to it.

## What surprised

**The directory walk needed no new extern, and the panel's own premise was the
thing that was wrong.** Panel 085 R4 wrote that a directory listing has no sound
cheap route — `ls > file` plus `read_file` *"splits a filename containing a
newline into two entries at exit 0"*, measured, which §1.12 forbids — and named
`popen`/`fgets`/`pclose` or `readdir` as the alternatives, one of which is
blocked by a construction question (`DECIDE.md:462`).

There is a third, and it costs **one `find` and zero externs**: `find -print0`
separates names with the NUL byte, and a Heroes `str` carries that byte through
`read_file` and byte indexing without trouble. Measured on the hostile directory
the panel's own falsifier describes — a name holding a newline, a name holding an
apostrophe, one plain name — **3 entries for 3 files, the newline name whole,
every one readable**, identical from the Rust bootstrap and from the seed-built
self-hosted compiler. The measurement is now `shell.hero`'s third test rather
than a note, so the day the route stops working is the day a test says so.

Worth naming precisely, because it is the shape CLAUDE.md §1 keeps warning
about: the panel's claim was a **failed search** (two routes found) written as an
**impossibility** (no third route exists). The language already had the answer.

## What broke and why

**A captured stream that arrived on the terminal, and a capture file written into
the wrong directory — one cause.** `shell.run` built
`<command> > out 2> err`, and a shell redirection binds to **one** command, not
to a list. So `printf a; printf b > out` sent only `b` to the file and printed
`a` where nobody was looking, and `cd dir && touch x > out` resolved `out`
*inside* `dir` — `sh: build/harness-selftest/stdout: No such file or directory`,
exit 1. Two symptoms, both diagnosed by the two tests that had just been written
to watch where the output went; the fix is the subshell,
`( <command> ) > out 2> err`, which takes the redirection for the whole line in
the working directory the harness started in, and through which `exit 3` still
answers 3.

The lesson is the ordinary one and it paid immediately: the harness's own tests
found the harness's own defect within a minute of existing. A capture that
silently loses half its output would have made every later suite report
mismatches whose cause was in this file.
