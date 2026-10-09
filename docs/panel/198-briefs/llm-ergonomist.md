# Panel 198, llm-ergonomist: the blind seat, run by the coordinator

Written on 2026-10-08 from 08:06 by the clock, for the coordinator: this seat
is never a subagent and never reads this file. The author convened the
sitting full on 2026-10-08 (between 01:57 and 07:37 by the clocks read before
and after), *the blind seat included, 6 USD in all*.

## How it runs

Fresh `claude -p` sessions outside the repository
(`.claude/skills/panel/SKILL.md` § 2, the 2026-09-30 and 2026-10-01
instructions), the command of `SKILL.md:161-165` with the model panel 196
ran (`<scratchpad>/p196/blind-run.sh`; `modelUsage` in
`/tmp/b196-t1-C/run.json` names `claude-opus-5-5`):

```
claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Write report.md here. Answer in English." \
  --restricted --safe-mode --strict-mcp-config --model claude-opus-5-5 \
  --tools "Read,Write" --allowedTools "Read,Write" \
  --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
  --max-budget-usd 1 --output-format json > run.json 2> run.err < /dev/null
```

`claude --version` read `2.1.285` at 07:50; whether it accepts the model is
read from the first session's `run.json` (`modelUsage`), and the synthesis
records the model each session ran on.

**The budget: six sessions at `--max-budget-usd 1` each, 6.00 USD at most in
all**, three at a time. Panel 196's nine sessions cost 0.167 to 0.516 USD
each, 2.42 in all, by their `run.json` `total_cost_usd` (read between 07:50
and 07:52 from `/tmp/b196-t*/run.json`) on a `spec.md` of 24,508 bytes
(`ls -la /tmp/b196-t1-C`); this one is 25,247. A
session stopped at its bound keeps the `report.md` it wrote as it went, is
recorded as stopped, and is not re-run: nothing past 6 USD.

**The folders**: `<scratchpad>/198-llm-ergonomist/b198-t<task>-<label>/`, six
of them, each holding `brief.md`, `spec.md`, `main.hero`, `machine.txt` and
`build-output.txt` and nothing else. No `CLAUDE.md` and no `.git` stands in
the scratchpad or any directory above it (checked between 07:50 and 07:52,
seven levels from `/`). `spec.md` is `spec/heroes-spec.md` at `56def9b4`
(456 lines, 25,247 bytes, SHA-256 beginning `75407a13724c3758`), the same
for every session unless the spec-warden drafts a sentence a route owes
(below). When the sitting is written, each folder's `brief.md`, `main.hero`,
`machine.txt` and `build-output.txt` are copied into
`docs/panel/198-briefs/blind/<folder>/` and each `report.md` into
`docs/panel/198-reports/blind/` with a header saying how it was run
(`SKILL.md:187-190`).

## The question the seat answers

Given only the specification and the compiler's message, does a reader make
a refused program build, and **by which way out**: one that keeps the list's
safety (naming the library with `link`, rebuilding the library without the
word) or one that routes around it (editing a system `.pc` file, a
`pkg-config` wrapper on `PATH`, a word passed on for every build)? And does a
route's note change which?

## The two tasks

- **t1, defect 444's own shape.** `main.hero` is
  `tests/golden/run/ffi-a-construction-polls-an-sdl3-event.hero` without its
  13 comment lines and the blank line after them (54 lines, SHA-256 beginning
  `faf2b7ec88ace6a7`, `<scratchpad>/198-briefs-work/probe/va-t1/main.hero`).
  `machine.txt`, from `ci-step2-out.txt:2` and `:891` and `ci-step2.sh:16-20`:

  ```
  Ubuntu 24.04.5 LTS on arm64 (aarch64), clang 18.1.3.
  SDL 3.2.10 was built from its source with CMake and installed into /usr/local, then `ldconfig` was run.
  `pkg-config --cflags --libs sdl3` prints:
  -I/usr/local/include -L/usr/local/lib -Wl,-rpath,/usr/local/lib -Wl,--enable-new-dtags -lSDL3
  ```

- **t2, Q1's class.** `main.hero` (5 lines, SHA-256 beginning
  `bb55e8ac2de3488b`, `<scratchpad>/198-briefs-work/probe/va-t2/main.hero`):

  ```
  extern "curl/curl.h" package "libcurl"
      function curl_version() -> cstr

  function main()
      print(validated(c: curl_version()).default("<unnamed>"))
  ```

  `machine.txt`, from `docker-b14-answers.txt` (07:53, `sdl3-b14`) and
  `docs/platforms/linux/Dockerfile.arm64:64`:

  ```
  Debian GNU/Linux 13 (trixie) on arm64 (aarch64), clang 22.1.8.
  libcurl's development files were installed with the system's package manager.
  `pkg-config --cflags --libs libcurl` prints:
  -I/usr/include/aarch64-linux-gnu -isystem /usr/include/mit-krb5 -I/usr/include/p11-kit-1 -lcurl
  ```

## The variants, under neutral labels

Each reader sees one `build-output.txt` and is not told another exists. The
labels are `K`, `M` and `R`; **this mapping is the coordinator's and enters no
folder**:

- **M is variant A**, today's message, exactly as the frozen compiler printed
  it for each `main.hero` (built from the seed in a copy, a stand-in
  `pkg-config` on `PATH` answering `machine.txt`'s line, exit 1, 08:01;
  `<scratchpad>/198-briefs-work/probe/variant-A-t1.txt` and `-t2.txt`).
  t1:

  ```
  error[ffi_package]: the package `sdl3` answered with `-Wl,--enable-new-dtags`, which this compiler does not pass on
    at main.hero:1:29
      |
    1 | extern "SDL3/SDL.h" package "sdl3"
      |                             ^^^^^^
    note: only `-D`, `-U`, `-I`, `-L`, `-l` and `-F`, each with its value joined or as the next word, `-framework <name>`, and `-Wl,-framework,<name>` and `-Wl,-rpath,<dir>`, in one word or split as `-Wl,-rpath -Wl,<dir>`, are accepted: everything else is a flag a package file could use to run code during the build (Go's CVE-2018-6574)
    note: name the library directly with `link` if you need it
  ```

  t2: the same seven lines with the headline *the package `libcurl` answered
  with `-isystem`, which this compiler does not pass on*, the location
  `at main.hero:1:30`, and the excerpt's line `1 | extern "curl/curl.h"
  package "libcurl"` with nine carets under `"libcurl"`.

- **K is variant B1**: the message under the route the compiler-engineer
  builds, copied whole from its `report.md` § `notes for the blind seat`.
  Where that route admits the task's word, so the program builds and there is
  no message, K takes route (A)'s note instead (the refusal naming the way
  out), the compiler-engineer's text where it wrote one, else the draft below.
- **R is variant B2**: route (G)'s note (the person building admits a word),
  the compiler-engineer's text if it built (G), else the draft below; or
  route (J)'s, if the synthesis has dropped (G) before the runs, and the run
  log says so.

**DRAFTS, by the coordinator's hand on 2026-10-08, unbuilt and unrun.** Each
keeps the headline, the excerpt and the first note of variant A and replaces
its last line (*name the library directly with `link` if you need it*) with
the line below; the synthesis says which text every reader saw, quoted.

K's fallback, t1:

```
  note: name the library directly, `link "SDL3"` in place of `package "sdl3"`, with the directories the package answered, /usr/local/include and /usr/local/lib, in the build's `CPATH` and `LIBRARY_PATH`; or build the library from its source again, configured not to write this word into its `.pc` file
```

K's fallback, t2:

```
  note: name the library directly, `link "curl"` in place of `package "libcurl"`, with the directories the package answered, /usr/include/aarch64-linux-gnu, /usr/include/mit-krb5 and /usr/include/p11-kit-1, in the build's `CPATH`
```

R, t1 (and t2 with `--allow-flag=-isystem`):

```
  note: name the library directly with `link` if you need it, or pass this word on for one build: `heroes build main.hero --allow-flag=-Wl,--enable-new-dtags`
```

The flag's spelling is a placeholder no seat has priced; a CLI surface is
`.claude/rules/cli-surface.md`'s, and the compiler-engineer's spelling
replaces it. Where the spec-warden writes a sentence a route owes (its
`report.md` § `drafts for the blind seat`), that route's sessions read
`spec.md` with the sentence in place, and the run log says so.

**The six sessions**: `b198-t1-K`, `b198-t1-M`, `b198-t1-R`, `b198-t2-K`,
`b198-t2-M`, `b198-t2-R`. They run once the compiler-engineer has written its
notes, or at the coordinator's call with the drafts, said in the run log.
Before the first starts, the coordinator writes in that log what it expects
under each label, so the readings score a prediction rather than a story.

## Each folder's `brief.md`

Written with no project name; `<TASK>` is t1's *`main.hero` uses SDL3 to
push one user event and poll it back, printing five lines* or t2's
*`main.hero` prints the version string of the libcurl library the machine
has*.

```
# Your brief

You make a program in a small programming language build, and say where its
specification left you a choice. Your input is the files of this directory
and nothing else: `spec.md`, the language's specification (read it in full
first); `main.hero`, the program; `machine.txt`, the machine it is built on;
and `build-output.txt`, what the language's compiler printed when it built
`main.hero` there with `heroes build main.hero -o main`. Read no file outside
this directory and use no tool but reading and writing files here; you
cannot run anything.

**The task**: <TASK>. Make it build on that machine and do what it does
now. You may change the program, and you may name anything you would write,
change or run on the machine.

Write `report.md` in this directory as you go, with exactly these headings:
`program` (the whole program as you would build it), `files` (every other
file you would write or change, each whole, and every command you would run
on the machine, in order; or *none*), `choice_points` (every place the
specification or the compiler's message left you a choice, the choice you
made, and what the other choice would produce: a compile error, a program
that builds and does something else, or a change to the machine beyond this
program), `confidence` (whether you believe the program builds and does what
it did, and what you are least sure of), and `context` (whether anything
other than this directory's files reached your context, and what). English,
no em dashes.
```

## What is scored, from each `report.md`

1. **The way out**, one of: a `link` group; the library rebuilt without the
   word; a `.pc` file edited or replaced; a `pkg-config` wrapper; the
   allowance a note names; other; none.
2. **Whether it builds and runs**, by `build` and `run`, never `check`: the
   coordinator applies each answer where the answer's world exists. t1 in
   `heroes-linux-arm64:sdl3-b14` with the stand-in `sdl3.pc` of the
   ffi-pragmatist's brief (refused at exit 1, 07:56 to 07:57), or in the
   ffi-pragmatist's `ci-step2.sh` container where it kept one, the program
   built by the frozen compiler and by the compiler-engineer's prototype,
   the event case's `.expected` the judge (`true true true 42 true`); t2 in
   `sdl3-b14` itself, whose `libcurl` answer the frozen compiler refuses
   (07:57). The baselines, measured over Debian's install at 07:57: `link
   "SDL3"` and `link "curl"` build and run.
3. **Whether the answer weakens the list**: it passes a refused word on, or
   any word, by editing, wrapping or allowing.
4. **`choice_points` and `confidence`**: what the reader was least sure of,
   and whether the message's wording is what it hesitated on.
5. **`context`**: a yes voids that reading until it is re-run from a copy.

The seat's verdict in the synthesis is built from these readings, with the
structure of `.claude/agents/llm-ergonomist.md` (`verdict`, `experiment`,
`hesitation_points`, `argument`, `prediction`, `condition`, `context`), and
says which variant text each reader saw.
