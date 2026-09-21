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

# $ErrorActionPreference does not trap a native program's exit code, so every
# winget call is judged here. 0x8A15002B and 0x8A150061 are "already installed"
# and "no applicable upgrade": both are success for our purposes.
function Install-Winget-Package([string] $id, [string] $human) {
    $already = @(-1978335189, -1978335135, -1978335212)
    Note "winget install $id"
    & winget install --id $id --exact --silent --disable-interactivity `
        --accept-source-agreements --accept-package-agreements 2>&1 | Out-String | Write-Host
    $code = $LASTEXITCODE
    Sync-Path
    if ($code -eq 0)            { Good "$human installed" }
    elseif ($already -contains $code) { Good "$human was already there" }
    else { throw "winget failed for $id with exit code $code ($('0x{0:X}' -f $code))" }
}

Write-Host ''
Write-Host 'Heroes --- Windows hunting box' -ForegroundColor White
Write-Host ("Windows: " + (Get-CimInstance Win32_OperatingSystem).Caption + `
            " build " + [Environment]::OSVersion.Version.Build)
Write-Host ("CPUs: " + $env:NUMBER_OF_PROCESSORS + `
            "   RAM: " + [math]::Round((Get-CimInstance Win32_ComputerSystem).TotalPhysicalMemory / 1GB, 1) + " GB")

# ---------------------------------------------------------------- 1. the key
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

# --------------------------------------------------------------- 2. the root
Step "The work directory, $Root"
if (-not (Test-Path $Root)) { New-Item -ItemType Directory -Path $Root | Out-Null }
Good "$Root is there"

# ------------------------------------------------------------- 3. the server
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

# ---------------------------------------------------------- 4. the key, filed
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

# ------------------------------------------------------------- 5. the tools
Step 'The toolchain, by winget'

if (-not (Get-Command winget -ErrorAction SilentlyContinue)) {
    Caution 'winget is not on PATH.'
    Caution 'Windows Server 2025 carried it on the box of 2026-08-31, so this is'
    Caution 'a surprise rather than an expected branch. Install "App Installer"'
    Caution '(https://aka.ms/getwinget) and run this script again; or install by'
    Caution 'hand: Git for Windows, LLVM, GitHub CLI, Tailscale.'
    throw 'winget missing'
}

Install-Winget-Package 'Git.Git'             'Git for Windows'
Install-Winget-Package 'LLVM.LLVM'           'LLVM and clang'
Install-Winget-Package 'GitHub.cli'          'GitHub CLI'
Install-Winget-Package 'tailscale.tailscale' 'Tailscale'

# ------------------------------------------------- 6. the shell SSH lands in
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
Restart-Service sshd
Good 'sshd restarted so the new shell takes effect'

# --------------------------------------------------------------- 7. .bashrc
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

# ------------------------------------------------------- 8. the repositories
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

# ------------------------------------------------------------ 9. MSVC's CRT
Step 'The MSVC toolset, which is what clang links against here'

# clang targets x86_64-pc-windows-msvc on this platform --- the same triple CI
# uses, which is what makes this box a faithful instrument. It therefore needs
# the MSVC CRT: without it every link dies with
#     lld-link: error: could not open 'libcmt.lib': no such file or directory
$msvcRoot = 'C:\Program Files\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC'
if ($SkipBuildTools) {
    Caution 'skipped by -SkipBuildTools'
} elseif (Test-Path $msvcRoot) {
    Good ('already installed: ' + ((Get-ChildItem $msvcRoot | Select-Object -First 1).Name))
} else {
    $boot = Join-Path $Root 'vs_BuildTools.exe'
    if (-not (Test-Path $boot)) {
        Note 'downloading the Build Tools bootstrapper'
        Invoke-WebRequest -Uri 'https://aka.ms/vs/17/release/vs_BuildTools.exe' -OutFile $boot
    }
    Note 'installing the VC tools workload --- gigabytes, several minutes, no output until it ends'
    # Two traps met on 2026-08-31, both worth not meeting twice: winget's
    # --override string is split when passed through PowerShell's
    # -ArgumentList, so winget prints its own help and installs nothing; and
    # the installer outlives an SSH session, so over SSH it must be detached.
    # Here it runs from the console, where -Wait is what we want.
    $btArgs = @('--quiet', '--wait', '--norestart', '--nocache',
                '--add', 'Microsoft.VisualStudio.Workload.VCTools',
                '--includeRecommended')
    $p = Start-Process -FilePath $boot -ArgumentList $btArgs -Wait -PassThru
    # 3010 is "success, a reboot is pending" and is not a failure.
    if ($p.ExitCode -ne 0 -and $p.ExitCode -ne 3010) {
        throw "vs_BuildTools exited $($p.ExitCode)"
    }
    if (-not (Test-Path $msvcRoot)) { throw 'Build Tools reported success and installed no MSVC toolset' }
    Good ('installed: ' + ((Get-ChildItem $msvcRoot | Select-Object -First 1).Name))
}

# ------------------------------------------------------------ 10. the tailnet
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

# ------------------------------------------------------------ 11. the checks
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
Write-Host ('    MSVC       ' + $(if (Test-Path $msvcRoot) { (Get-ChildItem $msvcRoot | Select-Object -First 1).Name } else { 'MISSING' }))

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

# ------------------------------------------------------------- 12. the seed
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
