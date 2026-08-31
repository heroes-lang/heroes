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
pays for the hours it is awake**. Expect it to be off.

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

Measured 2026-08-31, from the author's Mac (`venus`):

| | |
|---|---|
| Tailscale name | `apponfly-vps`, `100.88.88.100` |
| SSH alias | `win` (and `apponfly`), in `~/.ssh/config` on the Mac |
| User | `Administrator` |
| Key | `~/.ssh/id_ed25519` on the Mac |
| OS | Windows Server 2025, `10.0.26100.32690` |
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
scp selfhost/cli_flags.hero win:/c/w/heroes/selfhost/cli_flags.hero
```

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
| MSVC toolset | **NOT CONFIRMED** | see the paragraph below before trusting a compile |

**The MSVC toolset is the one row here that was never confirmed present**, and the
reason is written down rather than left as a gap: the install was launched, and
**the machine went offline while it was running** (Tailscale `offline, last seen
1m ago`, 2026-08-31 11:17 local). A Visual Studio installer restarts the machine
in some configurations even under `--norestart`, and this box may equally have
been powered off — the two look identical from here. So the first act of the next
session that needs to compile on Windows is to check the toolset with the one-line
command below and, if it is missing, run the bootstrapper again. Nothing in this
file depends on that being already done.

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
does `--emit-c` on the whole compiler. That flag now lives in
`selfhost/cli_flags.hero::link_flags()` for every binary the compiler links, so a
seed built by the line above passes it on.

**The seed build time on this machine has not been measured** and is not guessed
here; on the Mac it is 3.5 s, and a 2-CPU VPS will not match that. Measure it in
the session that needs the number (`CLAUDE.md` §1).
