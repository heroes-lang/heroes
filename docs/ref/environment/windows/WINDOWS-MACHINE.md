# The Windows machine — a real one, reachable in seconds

**There is a real Windows box for this project, and it is not the CI.** Until
2026-08-31 every Windows fact in this repository was learned through a seven-minute
GitHub Actions run, one question per round. This machine answers the same question
in seconds, so a bisection that took an afternoon takes a coffee.

**What it is and what it is not.** It is the **hunting instrument**: it finds where
something dies, it prices a hypothesis, it lets a fix be tried before it is
committed. It is **not the judge**. M-argv-execution's acceptance criterion is
*the Windows leg of CI is green* (`docs/ROADMAP.md` § The chain, row 27), and that
stays true — a green run on this box is evidence, not the criterion. The tag goes
on when CI says so.

## It is billed by the hour, so it may be off

The machine is an [appOnFly](https://apponfly.com) Windows VPS and **the author
pays for the hours it is awake**. **It is off between sessions — that is the
resting state, not an accident** (author instruction 2026-08-31, given while
turning it off after the milestone closed). Every session that needs it starts
by asking the author to power it on, in so many words, and does the
machine-free parts of the work while waiting.

- **The symptom of "off"** is an SSH connect timeout, or the Tailscale row reading
  `offline` instead of `idle`. It is not an authentication error — a key failure
  means something else broke.
- **Nobody but the author can turn it on**: it is powered from the appOnFly web
  console. So when it is off, the assistant's move is to **ask the author to start
  it** and, while waiting, do the parts of the work that do not need it. Do not
  wait on it in a loop; do not treat it as always-available infrastructure.
- **Shut nothing down on the author's behalf** unless asked, and do not assume a
  session survives: treat every visit as arriving on a machine that may have been
  rebooted since the last one. Everything below is re-checkable in one command.

## How to reach it

Measured 2026-08-31, from the author's Mac.

**This table gives the shape and not the values, since 2026-09-08**
(M-open-repository): it named the machine's tailnet address, its login user, the
path of the key that opens it and the exact OS build, which together are a
targeting profile for a box whose whole defence is that it sits behind a private
network. None of it was ever a credential and the address was never routable
from the internet, so nothing here was leaked; it is removed because publishing
it buys a reader nothing and costs the machine its obscurity. **The author's own
`~/.ssh/config` holds every value**, which is where they belong and where the
alias below already sends you.

| | |
|---|---|
| Reached over | a private tailnet, by the SSH alias below |
| SSH alias | `win` (and a second one naming the provider), in `~/.ssh/config` on the Mac |
| User | the box's administrator account |
| Key | an ed25519 key on the Mac, no passphrase in the loop |
| OS | Windows Server 2025 |
| Machine | **2 logical CPUs, 2.1 GB RAM, 85.6 GB free** |

The size matters and is stated rather than discovered: this is a small box. One
seed build and one module through one stage is what it is good at. The whole net
(1191 checks, each compiling and linking C) will be slow here, and a timing taken
on it is **not** comparable to a timing taken on the Mac.

**The default SSH shell is Git Bash**, set deliberately:

```
HKLM\SOFTWARE\OpenSSH  DefaultShell              = C:\Program Files\Git\bin\bash.exe
HKLM\SOFTWARE\OpenSSH  DefaultShellCommandOption = -c
```

So `ssh win '<any bash>'` works directly — no `cmd`, no PowerShell, no
`"C:\Program Files\Git\bin\bash.exe" -lc` wrapper. That was not cosmetic: under
`cmd.exe` git-over-SSH fails, because git wraps the repository path in single
quotes and `cmd` does not strip them (`fatal: ''C:/w/heroes.git'' does not appear
to be a git repository`, measured). Deleting those two registry values reverts it.

For anything longer than one line, **pipe a script over stdin** —
`ssh win 'bash -s' < script.sh`. Escaping `$?` through two shells is how a probe
comes back reporting `clang_exit=$?` instead of a number, which happened here
before the script channel replaced it.

Two nuisances to expect in output, both harmless: OpenSSH prints a
`post-quantum key exchange` warning on every connection, and `tasklist` filters
need `//FI` rather than `/FI` under MSYS.

## How the code gets there: pushed from the Mac, no credentials on the box

**No GitHub credential lives on this machine, and that is on purpose.** It is a
rented VPS; a private key copied onto it is not revocable without rotating it
everywhere it is used. The code arrives over Tailscale from the Mac, which already
holds the only key involved:

```
# on the Mac, from the repository
git push win:/c/w/heroes.git main:main         # 80.36 MiB on the first push
# on Windows
git clone /c/w/heroes.git /c/w/heroes
```

`/c/w/heroes.git` is a bare repository created with `git init --bare`. Later
pushes are incremental. **An uncommitted fix is `scp`-ed as a file** — which is
the normal case while hunting, since what is being tested is usually not committed
yet:

```
scp selfhost/cli/flags.hero win:C:/w/heroes/selfhost/cli/flags.hero
```

**That destination said `win:/c/w/heroes/…` until 2026-09-06 and it does not
work**, measured that day while shipping a runtime change: every file comes back
`scp: dest open "/c/w/heroes/…": No such file or directory`, and the same path in
the same session is fine over plain `ssh`. The reason is that the two go through
different servers. `ssh win '<bash>'` lands in Git Bash, where `/c/…` is MSYS's
own spelling of the drive; `scp` speaks to the **SFTP subsystem**, which is
Windows' and knows only Windows paths. So the one place in this document where a
path is not typed into bash is the one place it must be spelled `C:/`. The tell
is that the failure is a *destination* error and not an authentication one — and
`git status` on the box right afterwards is what says whether the file arrived.

**A deploy key was tried first and GitHub refused it**: `HTTP 422 — Deploy keys
are disabled for this repository`, an organisation policy on `heroes-lang`.
Read-only deploy keys are the better answer *if* that policy is changed in the
GitHub settings; until then, pushing from the Mac is both available and the
smaller blast radius, so nothing is owed here.

`gh` is installed but **not authenticated**, deliberately: reading CI runs is done
from the Mac, where the author's credentials already are. The hunt needs to read
code on Windows, not to act as the author from Windows.

## What is installed

Measured 2026-08-31, all via `winget`:

| Tool | Version | Note |
|---|---|---|
| Git for Windows | 2.55.0.windows.3 | brings `bash`, `git`, `ssh`, `curl`, `scp` |
| LLVM / clang | 22.1.8 | **target `x86_64-pc-windows-msvc`** |
| GitHub CLI | 2.98.0 | present, not logged in |
| Windows SDK | 10.0.26100.0 | `Program Files (x86)\Windows Kits\10` |
| MSVC toolset | 14.44.35207 | confirmed 2026-08-31: `clang t.c -o t.exe` links and the binary runs |

**The MSVC toolset row above was `NOT CONFIRMED` for half a day**, because the
first install was cut short when the machine went offline mid-run. The second
attempt (same bootstrapper line, launched detached over SSH) completed on
2026-08-31 and was believed only after the check that matters: a three-line C
program compiled, linked and ran. If a compile ever fails with the `libcmt.lib`
error below, re-run the one-line check further down before trusting this table.

**clang's target is the same one CI uses**, which is what makes this box a
faithful instrument rather than an approximation. It also means clang needs the
MSVC CRT: without it every link fails with

```
lld-link: error: could not open 'libcmt.lib': no such file or directory
```

which is the state the machine was in before the Build Tools went on, and the
first thing to re-check if a compile fails for no visible reason. The MSVC toolset
is installed from the bootstrapper at `C:\w\vs_BuildTools.exe`:

```
vs_BuildTools.exe --quiet --wait --norestart --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64
```

Two traps met while installing it, both worth not meeting twice: `winget`'s
`--override` string is split when passed through PowerShell's `Start-Process
-ArgumentList`, so winget prints its own help and installs nothing — run the
bootstrapper directly instead; and the installer must be **detached**
(`Start-Process`) rather than run inline, because it outlives an SSH session.

Check the toolset in one line, and believe the check rather than this table:

```
ssh win 'ls -d "/c/Program Files/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/"*/'
```

## What `~/.bashrc` adds, and why each line is load-bearing

Non-interactive SSH (`ssh win '<command>'`) reads `~/.bashrc`, not
`.bash_profile` — measured, the first PATH attempt landed in the wrong file and
`clang` stayed invisible. The file adds two directories, both required:

```
export PATH="/c/Program Files/LLVM/bin:$PATH"
export PATH="/c/Program Files/LLVM/lib/clang/22/lib/windows:$PATH"
```

The first is clang itself. The second is **the ASan runtime DLL**
(`clang_rt.asan_dynamic-x86_64.dll`): a `--sanitize` binary loads it at start
and dies `0xC0000135` (DLL not found) without it — that was `run/`'s entire 0
of 91 on this box before the line existed. The CI leg gets the same directory
through `clang -print-resource-dir` in the workflow; here the clang version is
in the path, so a clang upgrade must update this line.

## Two lessons about sessions, paid for on this box

- **Windows sshd kills every process of a session when the session closes** —
  `nohup` does not save them. A run longer than the SSH command that started it
  must be started from a session that is KEPT ALIVE (a backgrounded `ssh` from
  the Mac that simply stays connected), or it dies mid-write.
- **A killed run leaves orphans, and orphans hold files.** Two `heroes.exe` and
  a `clang.exe` survived one interrupt, held `build/harness/stdout` open, and
  Windows refuses to delete a file a handle still holds — so the NEXT run could
  not clear `build/` (`Device or resource busy`). The sweep is
  `taskkill //F //IM heroes.exe //T` (note `//` under MSYS) before any cleanup.

## The one line that builds the compiler here

From `CLAUDE.md` § Commands, plus the `/STACK:` reserve that M-argv-execution
step 13 measured as necessary (`8c10d5a` — Windows gives the main thread 1 MB
where POSIX gives 8, and the recursive-descent frontend spends it):

```
clang -I runtime seed/heroes.c runtime/runtime.c \
      -Wl,/STACK:67108864 -o heroes-from-seed.exe
```

Without the reserve, `heroes build selfhost/lexer.hero --dump-ir` exits **127**
with both streams empty — a killed process, not a panic. With it, exit 0, and so
does `--emit-c` on the whole compiler.

**And 127 is the wrong number to die with, because the runtime already means
something by it.** `hero_run_go` decodes a program that could not be started as
127, so a silent stack death and a missing `clang` wear the same exit code. On
2026-09-03 that cost eight diagnostic steps: a compiler built from the bare seed
line passed `heroes check` on `examples/ctime/main.hero`, died 127 on `build
--dump-ir` with `build/` left empty, and every half of the program's `extern`
group passed alone — so the chase went to `pkg-config`, `uname` and
`xcode-select` before it went to the stack. The tell is the pair: **`check`
passes and `build --dump-ir` dies with nothing written**. No spawn happens
between those two on the IR path, so a 127 there cannot be a missing program.
Read it as the stack first, rebuild with the line above, and only then look for
a spawn. That flag lives in
`selfhost/cli/flags.hero::link_flags()` for every binary the compiler links —
alongside `-Wl,/INCREMENTAL:NO`, added after MSVC's incremental linker printed
its full-link notice into program stdout the harness compares byte for byte —
so a seed built by the line above passes both on.

**Since M-robustness-guards step 4 (2026-09-03) a stack death on this box is not
silent.** `runtime/parts/stack.c`'s vectored exception handler writes `panic:
stack exhausted` to stderr before the process dies, measured here with
`down(10000000)` at `-O0` and at `-O2`: the line, then exit 127. So the pair
above splits: a 127 WITH that line is the stack, a 127 with both streams empty
is a program that could not be started. The line names the failure and not the
function — `SymFromAddr` needs dbghelp initialised before the fault and a PDB
beside the binary, neither measured on this box yet — where the Mac and Linux
say `… in main.down`.

**Seed build time here: 7.7 s**, measured 2026-08-31 (`real 0m7.715s`, first
build after a clone) against 3.5 s on the Mac. A full cold net run is roughly
12 minutes. The sync loop that matters: a delta `git push win main:main` from
the Mac is ~9–14 s once the bare repository has refs — the first 80 MiB push is
the only slow one.

## `$?` inside a `printf` that also runs a substitution is not the exit you think

Found 2026-09-04, measuring defect 010's repair on this box. A loop printed
`exit 0` for three programs that must fail:

```sh
./heroes.exe build "$f" -o /tmp/b.exe 2>/dev/null; printf "%s exit %s\n" "$(basename $f)" "$?"
```

In bash — the shell this box's SSH lands in — `$?` is expanded **after** the
`$(basename $f)` in the same argument list has run, so it is `basename`'s 0.
Measured here: `false; printf "%s %s\n" "$(basename /x/y)" "$?"` prints `y 0`.
The identical script in the Linux image had printed `exit 1`, because Debian's
`sh` is dash, which expands `$?` before running the substitution. Same words,
two shells, opposite readings — the shape of § "Escaping `$?` through two
shells" above, one level down. Capture the exit into a variable on the line
after the command (`code=$?`) and print the variable; never let `$?` share an
argument list with a `$(…)`.
