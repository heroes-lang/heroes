# Panel 226, the ffi-pragmatist's brief

Read `00-shared.md` first. Your copy: `<scratchpad>/226-ffi-pragmatist/`, `git archive` of
`a0a8d1249`; build your own compiler there from the seed. Write your report as you go to
`docs/panel/226-reports/ffi-pragmatist.md` in the repository's lane
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/thesis/`. **No paid run, no credential
printed**: you never run `gh auth token`, never print `ANTHROPIC_API_KEY`, never send a request to
`api.anthropic.com` (the one allowed endpoint, `/v1/messages/count_tokens`, is the spec-warden's and
already run); `curl --version` and `openssl version` are yours.

Your mandate here is the boundary the instrument crosses: processes the harness spawns, the
encryption of the archive, the sandbox the model's program runs in, and the platforms. Compile and
run, do not opine.

1. **The transport's shape.** `selfhost/cli/refresh.hero:59-83` spawns `curl` with `--variable
   %ANTHROPIC_API_KEY --expand-header "x-api-key: {{ANTHROPIC_API_KEY}}"` so the key is in neither
   argv nor a file. For `POST /v1/messages` the plan adds `--include`, `-H "Expect:"` (so no `100
   Continue` precedes the status line `tests/harness/issue_wire.hero`'s `reply_of`, line 207, reads;
   nothing there handles a `100 Continue` today, the critic read)
   and `--max-time`. Against a local listener of your own (`nc -l` or a Python `http.server` on
   127.0.0.1, nothing outward), confirm what `curl 8.7.1` prints with `--include` on an HTTP/1.1
   reply, and whether `-H "Expect:"` is needed for a body under 1 KiB and over 1 MiB. For the
   OpenAI-compatible dialect, `--variable "%OPENAI_API_KEY=none"`: does an unset variable with a
   default expand as the plan says?
2. **The sealing, across the two OpenSSLs and the platforms.** Encrypt a tar with
   `/usr/bin/openssl` (LibreSSL 3.3.6) `enc -aes-256-cbc -pbkdf2 -iter 600000 -a -pass env:K` and
   decrypt with brew's OpenSSL 4.0.3, and the reverse; with `-A` against line-wrapped `-a`; measure
   the ciphertext's growth and whether `unseen`'s refused characters can appear in base64 (none should:
   say which alphabet). Both OpenSSLs list `-iter` (`enc -help`, the critic): measure that a file
   encrypted with one decrypts with the other at 600,000 iterations. Does `-pass env:` leave the key out of `ps`
   (run `ps -o command` while a slow encryption runs)? **The Windows box is on** (the author,
   2026-10-20), but `.claude/rules/platforms.md` names no shell route to it (the critic), so unless
   you find one there, the Windows facts go out **unrun**, as questions: `openssl version`,
   `curl --version` (is it ≥ 8.3?), `tar --version` and whether `sandbox-exec` has any counterpart
   there.
3. **The sandbox.** Write a `sandbox-exec` profile that denies everything, then allows
   `process-exec` of one binary, reads under the copy, writes under one scratch directory, and no
   network. Build a Heroes program (your compiler) that tries, in order, to `write_file` into `$HOME`,
   to `read_file` of `/etc/hosts`, and to run forever; run it under the profile with `ulimit -t 10
   -f 1024` from `sh -c`, and report what each attempt did (exit status, stderr). Then the same
   under the harness's limit: does `hero_run_limit` (`runtime/hero_os.h:359-362`) kill a child that
   `sandbox-exec` started, i.e. is the group kill reaching the grandchild? **Note `RLIMIT_AS` on
   macOS is known not to bind**: measure `ulimit -v` against a program that allocates 2 GiB.
4. **Running a model's program as another identity**: is a dedicated unprivileged user on this Mac
   reachable from a harness program without a password (it is not, unless `sudo` is configured):
   say what it would take, and whether `sandbox-exec` alone is the robust route here.
5. **The `harness` transport**: `claude -p` with `--disallowedTools` of every tool,
   `--no-session-persistence`, `--bare`, the prompt on stdin, in an empty directory with `HOME` set
   to an empty folder: does 2.1.292 accept that combination and answer (use a two-word prompt and
   `--max-budget-usd 0.05`, **this is the one paid call your brief allows, under five cents, the
   second of the sitting's two beside the blind seat's session**, and say in the report that you ran
   it or that you did not)? Judge with stage 2 where you compile a Heroes program. Does `command -v heroes` inside that child
   see the trunk's binary when the parent's PATH does not name it?

Output the structure your definition asks for; `experiment` holds each command and what it printed.
Your veto stands where the boundary breaks: a key that leaks, a sandbox that does not hold, a
platform where the instrument cannot run and the plan says it does.
