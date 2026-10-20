# Panel 226, the ffi-pragmatist's report

Written as the work went, 2026-10-20, in the seat's own copy
`<scratchpad>/226-ffi-pragmatist/` (a `git archive` of `a0a8d1249`, 12,801 files), its own compiler
built from the seed there (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, 7.94 s real,
`sources 5f45d6514b22d650`) and stage 2 from it (`./heroes build selfhost/main.hero -o heroes-next`,
`sources 7792ecfe50a531e8`), which is the compiler every Heroes program below was compiled with. No
file of the lane was touched but this one. No credential was printed, read or sent. **The one paid
call the brief allowed, the five-cent `claude -p` probe of item 5, was started, hung two hours,
killed by the coordinator; unrun as a measurement**, and it was not started again. The first run of
this seat was stopped at 06:45 inside that probe; this file resumes from its § 1 (unchanged below)
and the coordinator's measurements in the draft synthesis's § What was measured, checked rather than
redone where it says so.

- `verdict`: **object** (not veto: nothing crosses the C ABI wrongly, and no key leaks under the
  protocol as written today; the objection is a measured hole one bucket-check deep, § 3, and two
  cheap narrowings). **On the C boundary and the ABI alone: approve**, the stamp stays (§ 6).
- `section`: design.md §1.11 (the harness is a real program and spawns through C, `hero_os.h`),
  §1.12 (robustness: the instrument's boundary is the sandbox and the watchdog), §4.19 (the
  `extern` groups the harness spawns through, and `refused_extern` as the one rule the key's secrecy
  rests on), Part 11's provenance sentence. **design.md does not cover a process's environment as a
  channel, nor `sandbox-exec`'s operations**: the sandbox is panel 226's own R11 and no section of
  the design names it; I say so rather than cite one.
- `experiment`: §§ 1 to 6 below, each command and what it printed.
- `argument`: The coordinator's profile holds where it was measured (writes outside refused, the
  home unreadable, no fork, `timeout` and the group kill reach the one pid the chain exec()s into),
  and the two runtime declarations change no shape: clang refuses the emitted call against an older
  header (*call to undeclared function*) and ld against an older object (*Undefined symbols*), so
  the stamp's job, skew becoming a compile error, is done without moving it. But the seal's key
  lives in the grader's environment for the whole run, and inside the profile a program that can call
  `sysctl(KERN_PROCARGS2)` reads it (`hits=1`), with `sysctl-read` and `process-info*` removed too.
  Only `refused_extern` stands between a model's program and the key. Open the archive in a process
  that exits first; narrow `process-exec*` to the binary; bound `claude -p` by `run_bounded`.
- `prediction`: At M-thesis-harness's close, with `HEROES_TASKS_KEY` exported in the grader's
  process as `thesis_seal.hero` does today, a C program calling `sysctl({CTL_KERN, KERN_PROCARGS2,
  <grader pid>})` run inside `thesis_sandbox.hero`'s profile on this Mac prints the key's value;
  with the archive opened by a separate invocation that exits before the first `build`, the same
  program prints `hits=0`. Falsified if the profile as written hides it.
- `condition`: approve when (1) the key is never in the environment of the process that spawns a
  model's program (open the archive in its own run of `thesis.hero`, or pass the key into
  `openssl`'s environment alone), (2) `(allow process-exec*)` becomes `(allow process-exec (literal
  "<binary>"))`, measured to run the probe and refuse `/bin/sleep`, and (3) a `harness` transport
  is spawned by `run_bounded` with its seconds, stdin from a file and `--tools ""`. Veto if a
  `harness` transport or an `extern`'d program is ever run under the profile with the key exported.

## Tools on this Mac, measured first

```
$ curl --version
curl 8.7.1 (x86_64-apple-darwin26.0) libcurl/8.7.1 (SecureTransport) LibreSSL/3.3.6 zlib/1.2.12 nghttp2/1.69.0
Release-Date: 2024-03-27
$ openssl version            # brew's, first on PATH
OpenSSL 4.0.3 29 Sep 2026 (Library: OpenSSL 4.0.3 29 Sep 2026)
$ /usr/bin/openssl version
LibreSSL 3.3.6
$ claude --version
2.1.292 (Claude Code)
$ command -v codex; echo rc=$?
rc=1
$ ls -la /usr/bin/sandbox-exec
-rwxr-xr-x  1 root  wheel  135136 Sep 24 09:10 /usr/bin/sandbox-exec
$ sw_vers
ProductVersion: 27.0.1   BuildVersion: 26A434
$ tar --version
bsdtar 3.5.3 - libarchive 3.7.4 zlib/1.2.12 liblzma/5.4.3 bz2lib/1.0.8
$ command -v gtar; ls /opt/homebrew/opt/gnu-tar/bin      # no GNU tar on this Mac
```

## 1. The transport's shape, against a listener of my own on 127.0.0.1:18226

A Python `http.server` (`BaseHTTPRequestHandler`, `protocol_version = "HTTP/1.1"`, which answers
`Expect: 100-continue` with `100 Continue` as the real server does), started with `python3 -I`,
polled with `nc -z` until it listened (6 polls of 0.2 s; a first attempt without the wait sent four
of seven requests before it was up and they are discarded), stopped by `kill` after the matrix;
`lsof -nP -iTCP:18226` printed 0 lines afterwards. Every request body was zeros turned into
letters; every "key" below is a fake string of my own.

```
$ curl --silent --show-error --include -H 'content-type: application/json' --data-binary @small.bin http://127.0.0.1:18226/small
rc=0
HTTP/1.1 200 OK^M$            # cat -ve: lines end \r\n, the blank line too
Server: BaseHTTP Python/3.15.0^M$
...
^M$
{"ok": true, "n": 500}
server saw: no Expect header, body_len=500

$ curl ... --data-binary @large.bin http://127.0.0.1:18226/large            # 1,572,864 bytes
rc=0
HTTP/1.1 100 Continue^M$
^M$
HTTP/1.1 200 OK^M$
...
server saw: Expect: 100-continue, body_len=1572864

$ curl ... -H 'Expect:' --data-binary @large.bin .../large-noexpect
rc=0
HTTP/1.1 200 OK^M$           # the 100 block is gone; server saw no Expect header, body_len=1572864

$ body of exactly 1,048,576 bytes:  first line HTTP/1.1 200 OK        (server saw no Expect)
$ body of 1,048,577 bytes:          first line HTTP/1.1 100 Continue  (server saw Expect: 100-continue)
```

So the threshold in curl 8.7.1 is **exactly 1 MiB, exclusive**: a body up to 1,048,576 bytes sends
no `Expect`, one byte more sends it, and with `--include` the output then opens
`HTTP/1.1 100 Continue\r\n\r\n` before the real status line. `tests/harness/issue_wire.hero`'s
`reply_of` (line 207) cuts at the first `\r\n\r\n` and reads `first[1]`, so it would answer **status
100** with the whole real reply as the body. `-H "Expect:"` is therefore needed for a body over 1 MiB
and does nothing under it; since the plan's bodies are a 26.6 KB spec plus a transcript, under 1 MiB
today, the header is cheap insurance against the day a five-turn transcript with thinking blocks
crosses it, and I would have `reply_of` refuse a status 100 outright (the harness never wants one).

```
$ env -u OPENAI_API_KEY curl ... --variable '%OPENAI_API_KEY=none' --expand-header 'Authorization: Bearer {{OPENAI_API_KEY}}' ...
rc=0      server saw: Authorization: Bearer none
$ OPENAI_API_KEY=fake-local-value-226 curl ... (same flags)
rc=0      server saw: Authorization: Bearer fake-local-value-226
$ env -u FAKE_KEY curl ... --variable '%FAKE_KEY' --expand-header 'x-api-key: {{FAKE_KEY}}' ...   # the Anthropic spelling, no default, unset
rc=2      curl: option --variable: variable expansion failure        # no request was sent
$ FAKE_KEY=fake-anthropic-226 curl --limit-rate 100K ... (held for a second so ps could read it)
ps -o command= : curl --silent --include --limit-rate 100K --variable %FAKE_KEY --expand-header x-api-key: {{FAKE_KEY}} --data-binary @large.bin http://127.0.0.1:18226/anthropic-shape
                 # the name, never the value, in argv; the server saw x-api-key: fake-anthropic-226
$ curl --max-time 2 ... /slow            # the server sleeps 6 s before answering
rc=28     curl: (28) Operation timed out after 2008 milliseconds with 0 bytes received; 0 bytes on stdout; real 2.02
```

An unset variable with a `=default` expands to the default as the plan says; one without a default
is a loud exit 2 before any socket opens, which is the right failure for the Anthropic key. The
`--max-time` failure is exit 28 with nothing on stdout, so `reply_of`'s *no status line reads as
status 0* is what a `transport_error` looks like from inside.

## 2. The sealing, across the two OpenSSLs (coordinator's round trip checked, the cross-opening mine)

Read first: `thesis_seal.hero` (257 lines, from `steps-2-to-9.tar` extracted into
`<copy>/coordinator-tar/`) spawns `tar -C <parent> -cf` and `openssl enc -aes-256-cbc -pbkdf2
-iter 600000 -a -in … -out … -pass env:<VAR>` (its test at line 202 pins those words), checks the
ciphertext's sha256 before any decrypt, and draws canaries from `openssl rand -hex 16`. A 300,000-byte
random file, key `K226` exported in my shell alone:

```
$ /opt/homebrew/bin/openssl enc -aes-256-cbc -pbkdf2 -iter 600000 -a -in plain.bin -out by-brew.enc  -pass env:K226
$ /usr/bin/openssl        enc -aes-256-cbc -pbkdf2 -iter 600000 -a -in plain.bin -out by-libre.enc -pass env:K226
$ /usr/bin/openssl        enc -d ... -a -in by-brew.enc  -out brew-opened-by-libre.bin -pass env:K226   # rc=0
$ /opt/homebrew/bin/openssl enc -d ... -a -in by-libre.enc -out libre-opened-by-brew.bin -pass env:K226 # rc=0
$ cmp plain.bin brew-opened-by-libre.bin && cmp plain.bin libre-opened-by-brew.bin                      # both identical
$ wc -c plain.bin by-brew.enc by-libre.enc
  300000 plain.bin
  406295 by-brew.enc          # 1.354x: base64 in 64-character lines, 6,251 lines, both start U2FsdGVkX1 (Salted__)
  406295 by-libre.enc
```

**OpenSSL 4.0.3 opens LibreSSL 3.3.6's archive and the reverse, byte for byte, at 600,000
iterations.** The Ubuntu runner: `ci.yml:301` names `ubuntu-latest`, its comment at line 342 says
that is 24.04 today; **its OpenSSL version is in no document of the tree (`grep -rn -i 'openssl 3'
.github docs/platforms` printed nothing), so it goes out unrun**, as a question for the
`thesis-attest` job's first run.

```
$ brew  openssl enc -d -A -a -in by-libre.enc (64-char lines)   rc=0, 300000 bytes   # 4.0.3 tolerates wrapped input under -A
$ libre openssl enc -d -A -a -in by-brew.enc  (64-char lines)   rc=1  error reading input file
$ libre openssl enc    -A -a -in plain.bin -out libre-A.enc     one line, 400,044 bytes, no newline
$ brew  openssl enc -d    -a -in libre-A.enc                    rc=1  error reading input file
$ cat by-brew.enc by-libre.enc | tr -d 'A-Za-z0-9+/=\n' | wc -c   0
$ brew  openssl enc -d -iter 100000 -a -in by-brew.enc          rc=1  bad decrypt      # wrong count
$ K226=wrong-key brew/libre openssl enc -d ... -in by-brew.enc  rc=1  bad decrypt      # both
$ printf x >> by-brew.enc; libre openssl enc -d ... -a          wrote 299,392 bytes, "bad decrypt" on stderr
```

So `-a` and `-A` are **not interchangeable across the two**: the archive is sealed and opened with
`-a` on both ends, which the coordinator's words do, and `-A` must never enter. The alphabet is
`A-Za-z0-9+/=` and the line feed, none of which is in `unseen`'s `REFUSED` (C0 but tab and LF, DEL,
C1, the bidi controls, the zero-width kin: `suite_unseen.hero:108-`). A wrong iteration count and a
wrong key both fail loudly at exit 1. **A tampered archive does not**: one byte appended, LibreSSL
still wrote 299,392 bytes of plaintext, CBC authenticating nothing, so the sha256 beside the
ciphertext that `thesis_seal` checks before decrypting (its test at line 246) is load-bearing and
stays.

**`-pass env:` and `ps`.** `ps -o command=` of the running `openssl` showed `... -pass env:K226`:
the name, never the value, in argv. **`ps -E -o command=` of the same process showed the value**
(1 match), as it does for any process of the same user whose binary is not a platform binary (a
`sleep` with the variable showed 29 bytes and no environment: macOS hides a platform binary's
environment, a Heroes-built harness is not one). What that means inside the sandbox is § 3.

**GNU tar**: no `gtar` on this Mac, so *GNU tar opens bsdtar's archive* is **unrun**; measured only
that `file by-bsdtar.tar` says `POSIX tar archive` (ustar magic at offset 257), the format GNU tar's
manual lists as readable, an inference until the runner runs `tar -xf`.

**Windows**: `.claude/rules/platforms.md` names no shell route to the box; `openssl version`,
`curl --version`, `tar --version` and any `sandbox-exec` counterpart there go out **unrun**.

## 3. The sandbox, the coordinator's profile taken as written

The profile, `thesis_sandbox.hero:64`, with `/bin/sh -c 'ulimit -f 4096; exec /usr/bin/env -i
PATH=/usr/bin:/bin /usr/bin/sandbox-exec -f <profile> "$0" "$@"'` in front, copied byte for byte
into `probe.sb` with my scratch and my binary. A Heroes program with **no `extern`**, spec § 11's
built-ins alone, compiled by stage 2 (`heroes-next build probe.hero -o probe`; its first draft was
refused by `cell_redeclared` for `n @= n + 1`, the thesis at work):

```
$ /usr/bin/time -p timeout 5 /bin/sh -c "ulimit -f 4096; exec /usr/bin/env -i PATH=/usr/bin:/bin /usr/bin/sandbox-exec -f probe.sb \"\$0\" \"\$@\"" $S/probe
home write: write_failed              # write_file("/Users/joseph/x226-probe.txt"): refused; ls afterwards: No such file
hosts read: 544 bytes                 # read_file("/etc/hosts"): allowed
library read: 23713 bytes             # read_file("/Library/Preferences/SystemConfiguration/preferences.plist"): ALLOWED
env read: read_failed                 # read_file of the trunk's .env, under /Users: refused
scratch write: ok                     # inside.txt exists afterwards
rc=124   real 5.01 user 4.67          # the endless loop ended by timeout's SIGTERM inside the sandbox; pgrep: probe gone
```

**The chain is one pid.** `sandbox-exec` exec()s into the target (no `sandbox-exec` process alive
while `sleep` ran), and so do `sh` and `env`: `sh pid 23981 ; sleep pid 23981`. So `hero_run_limit`'s
`kill(-child)` and even a plain `kill(child)` reach the model's program directly; there is no
grandchild. The coordinator's `run_bounded` ending `sleep 30` at 124 is consistent with this.

**Fork is denied, so a fork bomb is impossible.** The profile has `process-exec*` and no
`process-fork`: `/bin/sh -c 'echo child-ran; /bin/echo forked'` inside printed `child-ran` then
`/bin/sh: fork: Operation not permitted`, rc 128. `process-exec*` is reachable only by exec in place.
**Narrowing it to the binary works**: with `(allow process-exec (literal "<scratch>/probe"))` the
probe ran (`home write: write_failed`, `hosts read: 544 bytes`) and `/bin/sleep` was refused
(`execvp() of '/bin/sleep' failed: Operation not permitted`). The grader builds with `build <path> -o
<binary>` and no `--sanitize` (`thesis_grade.hero:152`), so no symbolizer needs exec; and a
`--sanitize` build's bounds panic printed inside the profile anyway (`panic: array index out of
range`). **Narrow it**: the cost is nil and the allowance then names what runs.

**What `sysctl-read` leaks.** `sysctl kern.hostname hw.model kern.boottime` inside printed the
author's hostname (redacted here), `Mac15,13` and the boot time: a fingerprint a program that can call
`sysctl` could print into a transcript. A no-`extern` program cannot; it can detect the sandbox only
by a refused read, which costs it the hidden cases, so detection is not the fear.

**The environment is.** A C program calling `sysctl({CTL_KERN, KERN_PROCARGS2, pid})` on a
stand-in process of mine (`sleeper`, a non-platform binary with `SECRET226=the-sealed-key-value-226`
in its environment):

```
outside, same user:                              procargs bytes=4908 hits=1
inside the coordinator's profile:                procargs bytes=4908 hits=1
inside, process-info* removed:                   procargs bytes=4908 hits=1
inside, sysctl-read removed:                     procargs bytes=4908 hits=1
```

**The profile does not hide another same-user process's environment, and neither of the two
allowances is what lets it through.** `thesis_seal.hero` reads the key by `-pass env:<VAR>` from the
grader's own environment, so during a sealed run the key is in the environment of the process that
spawns every model program, and a model program that can call `sysctl` reads it. Today nothing can:
the task rules forbid `extern` and R5 buckets such a program `refused_extern` before any build. That
is one rule deep, and the R11 sentence *denying everything but execution and writes inside the
sample's scratch* is measured false for this channel. The repair is cheap and process-shaped: open
the archive in a run of `thesis.hero` that exits before the grading loop starts, or hand the key to
`openssl`'s environment alone, so the grader's environment never holds it; `/Users` being denied,
the opened copy in the scratch is the only thing a program could then read, and it must be the
sample's scratch and not the suite's.

**`/Library` and `/private/var/db`**, `head -c 20` exec'd directly under the profile: READABLE
`/etc/hosts`, `/Library/Preferences/SystemConfiguration/preferences.plist` (hostnames, network
services), `/Library/Keychains/System.keychain` (the file's bytes), `/Library/Preferences/com.apple.loginwindow.plist`,
`com.apple.TimeMachine.plist`, `/private/var/log/system.log`, `/opt/homebrew/bin/openssl`; refused
by Unix mode alone, not by the profile: the user record under `/private/var/db/dslocal`,
`/private/etc/master.passwd`, `/private/etc/ssh/ssh_host_ed25519_key`. A Heroes binary needs
`/usr/lib` and `/System/Library/dyld`, nothing under `/Library` or `/private/var`. **So yes: deny
both**, or better turn the reads into an allow-list (`/usr`, `/System`, `/bin`, `/sbin`, `/dev`,
the scratch, the binary); **the allow-list profile is unrun** here, and its first run should trace
what the binary opens.

**No memory ceiling**: `/bin/sh -c 'ulimit -v 1000000'` and `/bin/zsh -c 'ulimit -v …'`, `-d` both
answer *Invalid argument* / *setrlimit failed*; there is no limit to fail to bind, so the 2 GiB
allocation test is moot and the wall bound is the only ceiling on this Mac.

**The Linux half, unrun, every word**: no one here has run `bwrap`. On the runner (24.04 by
`ci.yml:342`) it would need `bubblewrap` installed (whether the image has it: unrun),
`kernel.apparmor_restrict_unprivileged_userns` read and, where 1, the package's own AppArmor
profile; `/bin`, `/lib` and `/lib64` are symlinks into `/usr` on merged-usr Ubuntu, so `--symlink
usr/bin /bin` is the usual word rather than `--ro-bind-try /bin /bin`; `--unshare-all` needs the
`--proc /proc` the module gives; `ulimit -v` inside is enforced there (RLIMIT_AS), which is the one
memory ceiling the instrument would have on any platform.

## 4. Another identity

```
$ sudo -n true            sudo: a password is required   rc=1      # no passwordless sudo on this Mac
$ dscl . -list /Users | grep -v '^_' | wc -l                 4      # no user was created
$ kill -0 1               kill 1 failed: operation not permitted    # a signal across uids fails for non-root
```

A dedicated unprivileged user would add a kernel-enforced uid fence that `sandbox-exec`, a
deprecated API, does not give: it would close § 3's environment channel (the kernel hides another
uid's `KERN_PROCARGS2` from a non-root reader: **an inference**, measured only on a platform
binary's hidden environment, unrun across uids since I created no user) and make the author's files
unreadable by mode and not by profile. Its cost is measured in the third line: **`hero_run_limit`'s
group kill would fail with EPERM** against another uid's process unless the kill itself goes through
`sudo`, and `sudo -n` needs a sudoers rule only the author can write (a root act, asked for), plus a
`chown` of the scratch and a `sudo` for its removal. **The profile alone answers R11 for the
programs the instrument runs today** (no `extern`, `refused_extern` before build); a user is the
robust route the day a `harness` transport's own process or an `extern`'d program runs under it.

## 5. The `harness` transport, from the hang itself

`claude --help` (read, not run): no `timeout`, `max-time` or `deadline` word anywhere;
`--max-budget-usd` bounds dollars, not seconds. **Under `--bare`, *Anthropic auth is strictly
ANTHROPIC_API_KEY or apiKeyHelper via --settings (OAuth and keychain are never read)***, so a
`--bare` child with `HOME` on an empty folder and no key in its environment has no credential at
all; what 2.1.292 does then on a non-tty, a login prompt it waits on or an exit, is **a question,
unrun**, and it is the most plausible cause of the two-hour hang beside a prompt on stdin that never
saw EOF. Either way: a `claude -p` that never returns is a sample that is never judged and a wall
budget that never ends, and today's shape has `--max-time 900` for `curl` and nothing for `claude`.

What the transport owes, in the harness's own words: spawn through `shell.run_bounded` with
`seconds` (the coordinator's `run_bounded` refuses a bound over `WATCHDOG_SECONDS`, 600, so 900 is
not available: either 600 or a constant of the transport's own, said in the sitting), `bytes` for the
answer, and `stdin_path` a file holding the prompt so EOF is certain; the words `/bin/sh -c 'cd "$1"
&& HOME="$2" exec claude -p …'` so that, as § 3 measured for the sandbox chain, the child is one pid
and the runtime's `kill(-child)` ends it inside `seconds + 5`; `--output-format json` so a
truncated answer is a parse failure and not a program; an elapsed time and the bytes received in the
`transport_error` row. On the tools, by `--help`'s own words: `--disallowedTools` is *a list of tool
names to deny*, so a tool not named escapes it; `--restricted` *removes the built-in tools that run
commands or code (…) and WebFetch* and names no `WebSearch`; **`--tools ""` *disables all tools***,
which is the one word that denies by construction. So `--bare --tools "" --disallowedTools
WebSearch,WebFetch --no-session-persistence --max-budget-usd …`, the last two belt and braces, and
`--strict-mcp-config` with no config so no server is read. Whether `command -v heroes` inside sees
the trunk's binary when the parent's `PATH` does not name it: **unrun** (it needs the probe).

## 6. The C boundary: the two runtime declarations and the five guarded words

`step1-build-permissive.tar` extracted into `<copy>/coordinator-step1/`: `heroes_runtime.h:473-474`
adds `_Noreturn void hero_panic_unmatched_case(int64_t tag, HeroStr names);` and `_Noreturn void
hero_panic_unmatched_value(HeroStr shown);` after `hero_panic_assert_sides`, `HERO_RUNTIME_ABI` 30
in both headers; `parts/panic.c` defines them (the case's name cut from `names` at the `tag`-th
comma); `emit/guarded_names.hero` and both `heroes_guard_*.h` gain the two function names and the
three parameter names `names`, `shown`, `tag`, which is what the guard is for (a user header's
`#define tag` must not reach the runtime's prototypes). The header's own sentence, lines 19 to 26:
*moves whenever the declarations change shape … so a `runtime/` from another milestone is a compile
error rather than a wrong answer*. Compiled, a unit calling both, `_Static_assert(HERO_RUNTIME_ABI
== 30)`:

```
$ clang -std=c11 -I new/runtime unit.c new/runtime/runtime.c -o unit-new     # my runtime + the coordinator's header, guards, panic.c
$ ./unit-new                       panic: this match has no arm for .blue     rc=134
$ ./unit-new x                     panic: this match has no arm for 42        rc=134
$ clang -std=c11 -I old/runtime unit.c old/runtime/runtime.c                  # the OLD header, ABI also 30
unit.c:5:19: error: call to undeclared function 'hero_panic_unmatched_value'; ISO C99 and later do not support implicit function declarations
unit.c:6:5: error: call to undeclared function 'hero_panic_unmatched_case'; ...   2 errors generated.
$ clang ... unit2.c old/runtime/runtime.c       # the unit declaring them itself, the cache's shape: an old runtime object
Undefined symbols for architecture arm64: ... ld: symbol(s) not found ... linker command failed with exit code 1
```

**I agree the stamp stays.** An added declaration cannot produce the wrong answer the stamp exists
to prevent: against an older header the emitted C is refused by clang, against an older object by
ld, both before any program runs, and the one direction that links, new runtime under old emitted
C, calls nothing new. No existing prototype changed, `HeroStr` is unchanged (`ptr`, `len`, line
144), nothing is marshalled, and no `importc` verification is weakened: a binding's own header is
still checked against clang as before. The seed's `sources` key moves with the runtime it links
(panel 222), so the cache cannot pair the new emission with the old object in silence either.

The file is written: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/thesis/docs/panel/226-reports/ffi-pragmatist.md`.
