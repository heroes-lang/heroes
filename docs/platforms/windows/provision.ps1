# provision.ps1 --- the Heroes Windows hunting box, from a bare Windows image.
#
# THE ONLY THING THAT RUNS ON THE MACHINE BY HAND. Everything after this script
# is done from the Mac over SSH, and `docs/ref/environment/windows/WINDOWS-MACHINE.md`
# section "Rebuilding the box" is the map of both halves.
#
# Why this file exists: the first box was provisioned by hand on 2026-08-31 and
# nothing wrote down HOW. It died on 2026-09-21 and the knowledge died with it,
# while the Linux machine next door is a Dockerfile anybody can re-run. This is
# the Windows Dockerfile: the same shape, in the only language a fresh Windows
# image already speaks.
#
# Run it from an elevated PowerShell inside the VM (RDP or the provider's web
# console):
#
#     powershell -ExecutionPolicy Bypass -File C:\w\provision.ps1
#
# It asks for one thing it cannot know --- the Mac's public SSH key --- and, if
# no Tailscale auth key is passed, it prints a URL to open in a browser.
# Everything else it discovers or installs.
#
# It is written to be RE-RUNNABLE: every step checks the world before changing
# it, so a run interrupted by a reboot is finished by running it again.

#Requires -RunAsAdministrator

[CmdletBinding()]
param(
    # One line, exactly as `cat ~/.ssh/id_ed25519.pub` prints it on the Mac.
    # Left empty, the script asks for it. It is NOT stored in this file: a
    # public repository is no place for a targeting profile, which is the same
    # reason WINDOWS-MACHINE.md gives the shape of the connection and not its
    # values (2026-09-08, M-open-repository).
    [string] $AuthorizedKey = '',

    # The tailnet name the Mac's `~/.ssh/config` points at. Keep it identical
    # across machines and the alias `win` survives the next box dying --- which
    # is the whole lesson of 2026-09-21. DELETE THE DEAD NODE in the Tailscale
    # admin console first, or this one joins as `apponfly-vps-1` and the name
    # silently does not match.
    [string] $MachineName = 'apponfly-vps',

    # A pre-authorised key from the Tailscale admin console (`tskey-auth-...`).
    # Given, the machine joins the tailnet unattended; omitted, `tailscale up`
    # prints a URL to authenticate in a browser.
    [string] $TailscaleAuthKey = '',

    # The MSVC toolset is the long step (gigabytes, several minutes). Skip it
    # only to re-run the cheap half; clang cannot LINK without it.
    [switch] $SkipBuildTools,

    # Where the work lives. `/c/w` in the Git Bash spelling every document and
    # every SSH command uses.
    [string] $Root = 'C:\w',

    # The public repository. Cloning from here rather than pushing 80 MiB from
    # the Mac is what being public since 2026-09-08 buys: no credential is
    # needed for a read, and none is ever put on this box.
    [string] $Upstream = 'https://github.com/heroes-lang/heroes.git'
)

$ErrorActionPreference = 'Stop'
$script:StepNumber = 0

function Step([string] $title) {
    $script:StepNumber++
    Write-Host ''
    Write-Host ("=== {0}. {1}" -f $script:StepNumber, $title) -ForegroundColor Cyan
}

function Note([string] $text)  { Write-Host "    $text" -ForegroundColor DarkGray }
function Good([string] $text)  { Write-Host "    $text" -ForegroundColor Green }
function Caution([string] $text) { Write-Host "    $text" -ForegroundColor Yellow }

# winget writes PATH into the registry; the session that launched it keeps the
# PATH it started with. Every install below is followed by this, or the very
# next step cannot find what was just installed.
function Sync-Path {
    $machine = [Environment]::GetEnvironmentVariable('Path', 'Machine')
    $user    = [Environment]::GetEnvironmentVariable('Path', 'User')
    $env:Path = "$machine;$user"
}

# Is the thing on the disk? A path is tested as a path and a bare word as a
# command, and the PATH is refreshed first because winget writes it into the
# registry and the running session keeps the one it started with.
function Probe-Present([string] $probe) {
    Sync-Path
    if ($probe -match '[\\/]') { return (Test-Path -LiteralPath $probe) }
    return [bool](Get-Command $probe -ErrorAction SilentlyContinue)
}

# Two things this function refuses to take on trust, both learned on
# 2026-09-21 while the second box was being built.
#
# **`--exact` matches the id CASE SENSITIVELY.** `tailscale.tailscale` answers
# "No package found matching input criteria" where `Tailscale.Tailscale`
# installs. Confirmed against microsoft/winget-pkgs itself, where
# `manifests/t/Tailscale/Tailscale` is a directory and the lowercase spelling
# of the same path is a 404. The other three ids in this file were checked the
# same way rather than left to luck.
#
# **And an exit code is not a measurement of the world.** The version of this
# function that shipped an hour earlier read winget's codes from a table
# written from memory, and the code for *no package found* sat in the list of
# codes meaning *already installed*: a package that does not exist would have
# been reported as present, and the run would have gone green having installed
# nothing. So the code is not consulted at all. The question asked is whether
# the program is on the disk afterwards.
function Install-Winget-Package([string] $id, [string] $human, [string] $probe) {
    if (Probe-Present $probe) { Good "$human was already there"; return }

    Note "winget install $id"
    & winget install --id $id --exact --silent --disable-interactivity `
        --accept-source-agreements --accept-package-agreements 2>&1 | Out-String | Write-Host
    if (Probe-Present $probe) { Good "$human installed"; return }

    # Second attempt without --exact, which is case insensitive and matches on
    # name and moniker too. A fallback and never the default, because a loose
    # match can install the wrong package; it runs only after the precise id
    # has failed AND the probe has already said the program is not there.
    Caution "the exact id did not put $human on the disk --- retrying without --exact"
    & winget install --id $id --silent --disable-interactivity `
        --accept-source-agreements --accept-package-agreements 2>&1 | Out-String | Write-Host
    if (Probe-Present $probe) { Good "$human installed on the second attempt"; return }

    throw "$human is still not at '$probe' after two winget attempts (id: $id)"
}

Write-Host ''
Write-Host 'Heroes --- Windows hunting box' -ForegroundColor White
Write-Host ("Windows: " + (Get-CimInstance Win32_OperatingSystem).Caption + `
            " build " + [Environment]::OSVersion.Version.Build)
Write-Host ("CPUs: " + $env:NUMBER_OF_PROCESSORS + `
            "   RAM: " + [math]::Round((Get-CimInstance Win32_ComputerSystem).TotalPhysicalMemory / 1GB, 1) + " GB")

# ---------------------------------------------------------------- the key
Step 'The key the Mac will open this machine with'

if (-not $AuthorizedKey) {
    Write-Host '    On the Mac:  cat ~/.ssh/id_ed25519.pub'
    Write-Host '    Paste the whole single line here.'
    $AuthorizedKey = Read-Host '    public key'
}
$AuthorizedKey = $AuthorizedKey.Trim()
if ($AuthorizedKey -notmatch '^(ssh-ed25519|ssh-rsa|ecdsa-sha2-\S+) \S+') {
    throw "that does not look like an SSH public key: '$AuthorizedKey'"
}
Good ('key accepted: ' + $AuthorizedKey.Split(' ')[0] + ' ...' + $AuthorizedKey.Substring($AuthorizedKey.Length - 12))

# --------------------------------------------------------------- the root
Step "The work directory, $Root"
if (-not (Test-Path $Root)) { New-Item -ItemType Directory -Path $Root | Out-Null }
Good "$Root is there"

# ----------------------------------------------------------- the keyboard
Step 'The Italian keyboard, because a person types on this machine'

# The one step in this file that serves the human rather than the compiler.
# Provisioning happens over a remote desktop with an Italian keyboard in front
# of it, and a US layout turns every backslash, brace and at sign into a hunt
# --- on a box whose whole purpose is to type paths like /c/w/heroes and flags
# like -Wl,/STACK:. It is a convenience and not a dependency, so it is wrapped:
# a failure here reports and the rest of the script stands.
$itTag = 'it-IT'
$itTip = '0410:00000410'   # Italian language, Italian (Italy) keyboard layout
try {
    $list = Get-WinUserLanguageList
    if (-not ($list | Where-Object { $_.LanguageTag -eq $itTag })) {
        $list.Add($itTag)
        # The existing language is KEPT and Italian added beside it. Replacing
        # the list would take the machine's own language away from it, and
        # nothing here needs that: what is wanted is a layout, not a new
        # Windows display language.
        Set-WinUserLanguageList -LanguageList $list -Force
        Good "$itTag added beside the layouts already there"
    } else {
        Good "$itTag was already in the user's language list"
    }
    Set-WinDefaultInputMethodOverride -InputTip $itTip
    Good "default input method is now $itTip"

    # Carry it to the sign-in screen and to accounts made later, so the NEXT
    # session does not open on a US layout again. The cmdlet is Windows 11 and
    # Server 2022 upwards; older Windows skips this rather than failing, and
    # the per-user setting above still stands.
    if (Get-Command Copy-UserInternationalSettingsToSystem -ErrorAction SilentlyContinue) {
        Copy-UserInternationalSettingsToSystem -WelcomeScreen $true -NewUser $true
        Good 'copied to the welcome screen and to new accounts'
    } else {
        Note 'Copy-UserInternationalSettingsToSystem is not on this Windows --- this user only'
    }
    Caution 'A desktop session already open picks the layout up after a sign-out and back in.'
} catch {
    Caution "the keyboard step did not complete: $($_.Exception.Message)"
    Caution 'a convenience and not a dependency --- everything below is unaffected'
}

# ------------------------------------------------------------- the server
Step 'OpenSSH server: installed, automatic, running, and through the firewall'

$sshd = Get-Service -Name sshd -ErrorAction SilentlyContinue
if (-not $sshd) {
    Note 'not present --- adding the Windows capability (this can take a minute)'
    # Whether a given Windows build ships the server already is not something
    # this script guesses: it looks, and installs only if it is missing.
    $cap = Get-WindowsCapability -Online -Name 'OpenSSH.Server*' |
           Where-Object { $_.State -ne 'Installed' } | Select-Object -First 1
    if ($cap) { Add-WindowsCapability -Online -Name $cap.Name | Out-Null }
    $sshd = Get-Service -Name sshd -ErrorAction SilentlyContinue
    if (-not $sshd) { throw 'OpenSSH.Server did not install --- install it by hand and re-run' }
}
Set-Service -Name sshd -StartupType Automatic
if ($sshd.Status -ne 'Running') { Start-Service sshd }
Good ('sshd ' + (Get-Service sshd).Status + ', startup ' + (Get-Service sshd).StartType)

if (-not (Get-NetFirewallRule -Name 'heroes-sshd' -ErrorAction SilentlyContinue) -and
    -not (Get-NetFirewallRule -Name 'OpenSSH-Server-In-TCP' -ErrorAction SilentlyContinue)) {
    New-NetFirewallRule -Name 'heroes-sshd' -DisplayName 'OpenSSH Server (sshd)' `
        -Enabled True -Direction Inbound -Protocol TCP -Action Allow -LocalPort 22 | Out-Null
    Good 'firewall rule created for TCP 22'
} else {
    Good 'a firewall rule for sshd already exists'
}

# ---------------------------------------------------------- the key, filed
Step 'The key in the file Windows actually reads for an administrator'

# For a member of the Administrators group, the stock sshd_config carries a
# `Match Group administrators` block that reads THIS file and not
# ~/.ssh/authorized_keys. A key in the obvious place is silently ignored.
$keyFile = Join-Path $env:ProgramData 'ssh\administrators_authorized_keys'
$existing = if (Test-Path $keyFile) { [IO.File]::ReadAllText($keyFile) } else { '' }
if ($existing -notmatch [regex]::Escape($AuthorizedKey.Split(' ')[1])) {
    $text = ($existing.TrimEnd() + "`n" + $AuthorizedKey + "`n").TrimStart("`n")
    # No BOM, LF endings. A UTF-8 BOM in this file makes sshd read the first
    # key as garbage --- the failure looks like a rejected key, not a bad file.
    [IO.File]::WriteAllText($keyFile, $text, (New-Object Text.UTF8Encoding $false))
    Good "key written to $keyFile"
} else {
    Good 'key already present'
}
# And sshd refuses the file outright unless only SYSTEM and the administrators
# can write it. StrictModes is on by default; this is not optional hardening.
& icacls $keyFile /inheritance:r /grant 'Administrators:F' /grant 'SYSTEM:F' | Out-Null
Good 'permissions restricted to SYSTEM and Administrators'

# ------------------------------------------------------------- the tools
Step 'The toolchain, by winget'

if (-not (Get-Command winget -ErrorAction SilentlyContinue)) {
    Caution 'winget is not on PATH.'
    Caution 'Windows Server 2025 carried it on the box of 2026-08-31, so this is'
    Caution 'a surprise rather than an expected branch. Install "App Installer"'
    Caution '(https://aka.ms/getwinget) and run this script again; or install by'
    Caution 'hand: Git for Windows, LLVM, GitHub CLI, Tailscale.'
    throw 'winget missing'
}

Install-Winget-Package 'Git.Git'             'Git for Windows' 'C:\Program Files\Git\bin\bash.exe'
Install-Winget-Package 'LLVM.LLVM'           'LLVM and clang'  'C:\Program Files\LLVM\bin\clang.exe'
Install-Winget-Package 'GitHub.cli'          'GitHub CLI'      'gh'
Install-Winget-Package 'Tailscale.Tailscale' 'Tailscale'       'C:\Program Files\Tailscale\tailscale.exe'

# ------------------------------------------------- the shell SSH lands in
Step 'Git Bash as the default SSH shell'

# Not cosmetic. Under cmd.exe, git-over-SSH fails: git wraps the repository
# path in single quotes and cmd does not strip them, so a push dies with
# `fatal: ''C:/w/heroes.git'' does not appear to be a git repository`.
$bash = 'C:\Program Files\Git\bin\bash.exe'
if (-not (Test-Path $bash)) { throw "Git Bash not found at $bash --- did Git install?" }
$openssh = 'HKLM:\SOFTWARE\OpenSSH'
if (-not (Test-Path $openssh)) { New-Item -Path $openssh -Force | Out-Null }
New-ItemProperty -Path $openssh -Name DefaultShell `
    -Value $bash -PropertyType String -Force | Out-Null
New-ItemProperty -Path $openssh -Name DefaultShellCommandOption `
    -Value '-c' -PropertyType String -Force | Out-Null
Good "DefaultShell = $bash"
# **sshd is NOT restarted, and that is measured rather than assumed.** It was,
# in the version of this file written before the second box existed. Then the
# two registry values were written from an SSH session on that box and the very
# next connection landed in bash: `echo A; echo B` printed two lines where cmd
# had echoed the whole string back, and `uname -s` said MINGW64_NT-10.0-26100.
# sshd reads these values when it spawns a session, so a new connection gets
# the new shell and an open one keeps the old. The restart bought nothing and
# would cost this script its own session on the day it is run over SSH.
Note 'not restarting sshd: the value is read per session, so the next connection has it'

# --------------------------------------------------------------- .bashrc
Step 'What ~/.bashrc must add, and why each line is load-bearing'

# Non-interactive SSH reads .bashrc and NOT .bash_profile --- measured
# 2026-08-31, when the first attempt landed in the wrong file and clang stayed
# invisible to `ssh win 'clang --version'`.
$bashrc = Join-Path $env:USERPROFILE '.bashrc'
$block = @'
# --- Heroes: written by docs/ref/environment/windows/provision.ps1 ---
# Non-interactive `ssh win '<command>'` reads THIS file, not .bash_profile.
export PATH="/c/Program Files/LLVM/bin:$PATH"
# The ASan runtime DLL (clang_rt.asan_dynamic-x86_64.dll) lives under clang's
# resource directory, whose path carries clang's MAJOR VERSION. A --sanitize
# binary dies 0xC0000135 without it: that was run/'s entire 0 of 91 on the
# first box. The glob asks the machine for the version instead of pinning a
# number a clang upgrade falsifies in silence --- CLAUDE.md section 11, a
# narrowing asks the value and never the world.
for _d in "/c/Program Files/LLVM/lib/clang/"*/lib/windows; do
  [ -d "$_d" ] && PATH="$_d:$PATH"
done
unset _d
export PATH
# --- end Heroes ---
'@
$current = if (Test-Path $bashrc) { [IO.File]::ReadAllText($bashrc) } else { '' }
if ($current -notmatch '--- Heroes:') {
    $out = ($current.TrimEnd() + "`n`n" + $block + "`n").TrimStart("`n")
    [IO.File]::WriteAllText($bashrc, ($out -replace "`r`n", "`n"),
                            (New-Object Text.UTF8Encoding $false))
    Good "wrote $bashrc"
} else {
    Good "$bashrc already carries the Heroes block"
}

# ------------------------------------------------------- the repositories
Step 'The bare repository the Mac pushes to, and the checkout beside it'

Sync-Path
$git = 'C:\Program Files\Git\cmd\git.exe'
$bareRepo = Join-Path $Root 'heroes.git'
$workRepo = Join-Path $Root 'heroes'

if (-not (Test-Path $bareRepo)) {
    Note "cloning $Upstream --- bare, no credential needed, the repository is public"
    & $git clone --bare --quiet $Upstream $bareRepo
    if ($LASTEXITCODE -ne 0) { throw "bare clone failed ($LASTEXITCODE)" }
    Good "$bareRepo"
} else { Good "$bareRepo was already there" }

# A bare repository refuses a push to the branch its HEAD points at only when
# it is NOT bare; this one is, so `git push win main:main` from the Mac lands
# directly. What it must not do is refuse a non-fast-forward silently.
& $git --git-dir $bareRepo config receive.denyCurrentBranch ignore
& $git --git-dir $bareRepo config receive.denyNonFastForwards false

if (-not (Test-Path $workRepo)) {
    & $git clone --quiet $bareRepo $workRepo
    if ($LASTEXITCODE -ne 0) { throw "clone failed ($LASTEXITCODE)" }
    Good "$workRepo"
} else { Good "$workRepo was already there" }

& $git -C $workRepo config core.autocrlf false
& $git -C $workRepo config core.fileMode false
Good 'autocrlf off --- the golden harness compares bytes, and CRLF would rewrite them'

# ------------------------------------------------------------ MSVC's CRT
Step 'The MSVC toolset, which is what clang links against here'

# clang targets x86_64-pc-windows-msvc on this platform --- the same triple CI
# uses, which is what makes this box a faithful instrument. It therefore needs
# the MSVC CRT: without it every link dies with
#     lld-link: error: could not open 'libcmt.lib': no such file or directory
#
# **WHERE the toolset lands is asked and never assumed, and that cost an hour
# on 2026-09-21.** This line held the path WINDOWS-MACHINE.md records for the
# first box, `C:\Program Files\Microsoft Visual Studio\...`. The Build Tools
# put themselves under `C:\Program Files (x86)\...` here, so the check said
# MISSING while the bootstrapper's own log said `VS setup process exited with
# code 0` and `Bootstrapper Successfully completed`. A run that succeeded read
# as a run that died, and the next move would have been to reinstall a toolset
# that was already on the disk.
#
# `vswhere.exe` is Microsoft's answer to this question and its own location IS
# fixed --- that is what it is for. So the path comes from the machine, and the
# only thing pinned here is the one path Microsoft guarantees.
function Find-MsvcToolsetRoot {
    $vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
    if (-not (Test-Path $vswhere)) { return $null }
    $install = & $vswhere -products '*' -latest `
        -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
        -property installationPath 2>$null | Select-Object -First 1
    if (-not $install) { return $null }
    $root = Join-Path $install 'VC\Tools\MSVC'
    if (Test-Path $root) { return $root }
    return $null
}
$msvcRoot = Find-MsvcToolsetRoot
if ($SkipBuildTools) {
    Caution 'skipped by -SkipBuildTools'
} elseif ($msvcRoot) {
    Good ("already installed: $msvcRoot -> " + ((Get-ChildItem $msvcRoot | Select-Object -First 1).Name))
} else {
    $boot = Join-Path $Root 'vs_BuildTools.exe'
    if (-not (Test-Path $boot)) {
        Note 'downloading the Build Tools bootstrapper'
        Invoke-WebRequest -Uri 'https://aka.ms/vs/17/release/vs_BuildTools.exe' -OutFile $boot
    }
    Note 'installing the VC tools workload --- gigabytes, several minutes'
    # Two traps met on 2026-08-31, both worth not meeting twice: winget's
    # --override string is split when passed through PowerShell's
    # -ArgumentList, so winget prints its own help and installs nothing; and
    # **the installer must be DETACHED**, because Windows sshd kills every
    # process of a session when the session closes and this one outlives the
    # command that starts it.
    #
    # So it is launched detached and then WAITED FOR BY THE WORLD --- the poll
    # below asks whether the toolset is on the disk yet. That is the one shape
    # that is correct from a desktop console and over SSH both, where
    # `-Wait` is correct from a console and fatal over SSH. The same choice as
    # Install-Winget-Package's: the question asked is about the disk and never
    # about a process's report of itself.
    $btArgs = @('--quiet', '--wait', '--norestart', '--nocache',
                '--add', 'Microsoft.VisualStudio.Workload.VCTools',
                '--includeRecommended')
    Start-Process -FilePath $boot -ArgumentList $btArgs | Out-Null

    # The toolset root is re-asked on EVERY turn and not computed once before
    # the loop: vswhere itself does not exist until the installer has extracted
    # it, so a path resolved before the install is $null for reasons that say
    # nothing about the outcome.
    $deadline = (Get-Date).AddMinutes(45)
    $spent = 0
    while (-not $msvcRoot -and (Get-Date) -lt $deadline) {
        Start-Sleep -Seconds 30
        $spent += 30
        $running = @(Get-Process -Name 'vs_bootstrapper*', 'vs_installer*', 'setup' -ErrorAction SilentlyContinue).Count
        $msvcRoot = Find-MsvcToolsetRoot
        Note ("  {0,4}s --- installer processes: {1}" -f $spent, $running)
        if (-not $running -and $spent -ge 120 -and -not $msvcRoot) {
            throw ("no installer is running any more and vswhere reports no VC toolset. " +
                   "Read the bootstrapper's own log: the newest dd_bootstrapper_*.log in " +
                   $env:TEMP + " ends with either 'Bootstrapper Successfully completed' " +
                   "or the reason it did not.")
        }
    }
    if (-not $msvcRoot) { throw 'the Build Tools did not produce a VC toolset within 45 minutes' }
    Good ("installed: $msvcRoot -> " + ((Get-ChildItem $msvcRoot | Select-Object -First 1).Name))
}

# ------------------------------------------------------------ the tailnet
Step 'Joining the tailnet'

$ts = 'C:\Program Files\Tailscale\tailscale.exe'
if (-not (Test-Path $ts)) { throw "tailscale.exe not found at $ts" }
function Tailscale-State {
    # `tailscale status` exits non-zero when the backend is not up, and
    # ConvertFrom-Json throws on the empty string, so the absence of a tailnet
    # is read as a state and not as a crash.
    try { $raw = (& $ts status --json 2>$null | Out-String) } catch { return $null }
    if (-not $raw.Trim()) { return $null }
    try { return ($raw | ConvertFrom-Json) } catch { return $null }
}
$state = Tailscale-State
if ($state -and $state.BackendState -eq 'Running') {
    Good ('already up as ' + $state.Self.HostName + ' --- ' + ($state.Self.TailscaleIPs -join ', '))
} else {
    if ($TailscaleAuthKey) {
        & $ts up --authkey $TailscaleAuthKey --hostname $MachineName --unattended
    } else {
        Caution 'No auth key given. Tailscale prints a URL below: open it in a'
        Caution 'browser (the Mac is fine) and approve this machine.'
        & $ts up --hostname $MachineName --unattended
    }
    if ($LASTEXITCODE -ne 0) { throw "tailscale up exited $LASTEXITCODE" }
    Good 'tailnet joined'
}

# ------------------------------------------------------------ the checks
Step 'What was actually installed --- run, not assumed'

Sync-Path
function Version-Of([string] $exe, [string] $flag) {
    $c = Get-Command $exe -ErrorAction SilentlyContinue
    if (-not $c) { return 'MISSING' }
    return ((& $exe $flag 2>&1 | Select-Object -First 1) -join ' ')
}
Write-Host ('    git        ' + (Version-Of 'git' '--version'))
Write-Host ('    clang      ' + (Version-Of 'clang' '--version'))
Write-Host ('    gh         ' + (Version-Of 'gh' '--version') + '   (deliberately NOT logged in)')
Write-Host ('    tailscale  ' + (Version-Of 'tailscale' 'version'))
$msvcRoot = Find-MsvcToolsetRoot
Write-Host ('    MSVC       ' + $(if ($msvcRoot) { (Get-ChildItem $msvcRoot | Select-Object -First 1).Name + "   ($msvcRoot)" } else { 'MISSING' }))

Step 'clang compiles, links and runs a C program --- the check that decides'

# The table above is a list of installers that said yes. This is the only
# statement about the machine that is a measurement: three lines of C through
# the whole toolchain, with the /STACK: reserve seed/README.md prints for
# Windows, because that is the line the compiler is actually built with.
$csrc = @'
#include <stdio.h>
int main(void) { printf("ok\n"); return 0; }
'@
$t = Join-Path $Root 'toolchain-check.c'
[IO.File]::WriteAllText($t, ($csrc -replace "`r`n", "`n"))
Push-Location $Root
try {
    # The flag is QUOTED, and it has to be: PowerShell reads the comma in
    # `-Wl,/STACK:...` as its array operator and refuses the line with
    # "Missing argument in parameter list" --- caught by the parser on the Mac
    # before this file ever reached a Windows box.
    & clang $t '-Wl,/STACK:67108864' -o (Join-Path $Root 'toolchain-check.exe')
    if ($LASTEXITCODE -ne 0) { throw "clang failed ($LASTEXITCODE) --- if it says libcmt.lib, the MSVC toolset is the answer" }
    $out = & (Join-Path $Root 'toolchain-check.exe')
    if ($out -ne 'ok') { throw "the linked binary printed '$out' instead of 'ok'" }
    Good 'clang compiled, lld-link linked, the binary ran: the toolchain is real'
} finally { Pop-Location }

# ------------------------------------------------------------- the seed
Step 'The Heroes compiler, built from the seed'

Push-Location $workRepo
try {
    $sw = [Diagnostics.Stopwatch]::StartNew()
    & clang -I runtime seed/heroes.c runtime/runtime.c '-Wl,/STACK:67108864' -o heroes-from-seed.exe
    if ($LASTEXITCODE -ne 0) { throw "the seed did not build ($LASTEXITCODE)" }
    $sw.Stop()
    $v = & .\heroes-from-seed.exe --version
    Good ("$v --- built in " + [math]::Round($sw.Elapsed.TotalSeconds, 1) + ' s')
} finally { Pop-Location }

# -------------------------------------------------------------- the handover
Write-Host ''
Write-Host '=== Done. What the Mac needs from you ===' -ForegroundColor White
$final = Tailscale-State
$ips  = if ($final) { ($final.Self.TailscaleIPs -join ', ') } else { 'unknown --- tailscale is not up' }
$name = if ($final) { $final.Self.HostName } else { 'unknown' }
Write-Host ''
Write-Host "    tailnet name:  $name"
Write-Host "    tailnet IPs:   $ips"
Write-Host ''
Write-Host '    Tell the assistant those two lines and nothing else is owed.'
Write-Host '    Everything from here is done from the Mac over SSH.'
Write-Host ''
if ($name -ne $MachineName) {
    Caution "The name is '$name' and not '$MachineName'. The old node is probably"
    Caution "still in the Tailscale admin console: delete it, then run"
    Caution "    tailscale up --hostname $MachineName --reset"
}
